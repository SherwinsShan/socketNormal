#include "udpServerNormal.h"

//************************************* ERROR LOG ***************************//

#define UDP_SERVER_SET_ERR(s,a,b)  socketSetErrno((baseNormal*)s,a,b)

void udpServer_showErr(udpServer *server)
{
    static const char* errinfo[] = {
        "No error",
        "Socket create error",
        "Socket bind error",
        "Socket recv error",
        "Select recv error",
        "Socket write error",
    };
    socketLock((baseNormal*)server); 
    printf("%s",errinfo[server->inner_errno]);
    if(server->outer_errno == 0)
        printf("\n");
    else
        printf(":: %s\n",strerror(server->outer_errno));
    socketUnlock((baseNormal*)server);
}

//*************************** SetUp Function *****************************/
int setUdpServer(udpServer* server, int port)
{
    server->socketFd = 0;
    server->listenPort = port;
    server->inner_errno = 0;
    server->outer_errno = 0;
    memset(server->listenAddr, 0, 16);
    return socketMutexInit((baseNormal*)server);
}
int setUdpServerEx(udpServer* server,const char* addr, int port)
{
    server->socketFd = 0;
    server->listenPort = port;
    server->inner_errno = 0;
    server->outer_errno = 0;
    memset(server->listenAddr, 0, 16);
    strcpy(server->listenAddr, addr);
    return socketMutexInit((baseNormal*)server);
}

int createUdpServer(udpServer *server)
{
    struct sockaddr_in server_addr;
    int ret = socket(AF_INET, SOCK_DGRAM, 0);
    if(ret == -1)
    {
        UDP_SERVER_SET_ERR(server, UDP_SERVER_ERR_CREATE, errno);
        return UDP_FAIL;
    }
    server->socketFd = ret;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;  
    if(server->listenAddr[0] != 0)
    {
        server_addr.sin_addr.s_addr = inet_addr(server->listenAddr);
    }
    else
    {
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
    }
    server_addr.sin_port = htons(server->listenPort); 

    ret = bind(server->socketFd, (struct sockaddr *)&server_addr, sizeof(server_addr));  
    if(ret == -1)
    {
        UDP_SERVER_SET_ERR(server, UDP_SERVER_ERR_BIND, errno);
        return UDP_FAIL;
    }

    return UDP_SUCCESS;
}


int closeUcpServer(udpServer *server)
{
    socketLock((baseNormal*)server);//
    close(server->socketFd);
    server->inner_errno = 0;
    server->outer_errno = 0;
    socketUnlock((baseNormal*)server);//
    return socketMutexDestroy((baseNormal*)server);
}

//*************************** RW Function *****************************/
int udpServer_waitData(udpServer *server, char* data, int dataLength)
{
    int len = sizeof(struct sockaddr_in);
    int ret = recvfrom(server->socketFd, data, dataLength, 0, (struct sockaddr*)&(server->clientAddr), &len);
    if(ret == -1)
    {
        UDP_SERVER_SET_ERR(server, UCP_SERVER_ERR_RECV, errno);
        return UDP_FAIL;
    }
    return ret;
}

int udpServer_waitData_timeout(udpServer *server, char* data, int dataLength, long s, long us)
{
    int ret = select_read(server->socketFd, s, us);
    if(ret < 0)//error
    {
        UDP_SERVER_SET_ERR(server, UDP_SERVER_ERR_SELECT_READ, errno);
        return UDP_FAIL;
    }
    else if(ret == 0)//timeout
    {
        errno = ETIMEDOUT;    
        return UDP_TIMEOUT;   
    }
    else//readable
    {
        return udpServer_waitData(server, data, dataLength); 
    }
}

int udpServer_ackData(udpServer *server, const char* data, int dataLength)
{
    int ret = sendto(server->socketFd, data, dataLength, 0, (struct sockaddr *)&(server->clientAddr), sizeof(server->clientAddr));
    if(ret == -1)
    {
        UDP_SERVER_SET_ERR(server, UDP_SERVER_ERR_WRITE, errno);
        return UDP_FAIL;
    }
    return ret;
}

int udpServer_sendData(udpServer *server, const char* data, int dataLength, struct sockaddr_in clientAddr)
{
    int ret = sendto(server->socketFd, data, dataLength, 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
    if(ret == -1)
    {
        UDP_SERVER_SET_ERR(server, UDP_SERVER_ERR_WRITE, errno);
        return UDP_FAIL;
    }
    return ret;
}

int udpServer_sendDataEx(udpServer *server, const char* data, int dataLength, const char* addr, int port)
{
    struct sockaddr_in clientAddr;
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(port);
    clientAddr.sin_addr.s_addr = inet_addr(addr);
    return udpServer_sendData(server, data, dataLength, clientAddr);
}


char* udpServer_getClientAddr(udpServer *server)
{
    return getSocketAddr(server->clientAddr);
}

int  udpServer_getClientPort(udpServer *server)
{
    return getSocketPort(server->clientAddr);
}
