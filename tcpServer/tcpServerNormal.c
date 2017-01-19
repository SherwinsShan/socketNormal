#include "tcpServerNormal.h"


//************************************* ERROR LOG ***************************//

#define TCP_SERVER_SET_ERR(s,a,b)  socketSetErrno((baseNormal*)s,a,b)

void tcpServer_showErr(tcpServer *server)
{
    static const char* errinfo[] = {
        "No error",
        "Socket create error",
        "Socket bind error",
        "Socket listen error",
        "Socket accept error",
        "Select read error",
        "Socket recv error",
        "Socket send error",
        "Select write error"
    };
    socketLock((baseNormal*)server); 
    printf("%s",errinfo[server->inner_errno]);
    if(server->outer_errno == 0)
        printf("\n");
    else
        printf(":: %s\n",strerror(server->outer_errno));
    socketUnlock((baseNormal*)server);
}


//********************** SetUp Function *****************************/

int setTcpServer(tcpServer *server, unsigned short port ,int maxListen)
{
    server->socketFd = 0;
    server->listenPort = port;
    server->maxListenCount = maxListen;
    server->inner_errno = 0;
    server->outer_errno = 0;
    return socketMutexInit((baseNormal*)server);
}


int createTcpServer(tcpServer *server)
{
    struct sockaddr_in server_addr;
    int ret = socket(AF_INET, SOCK_STREAM, 0);
    if(ret == -1)
    {
        TCP_SERVER_SET_ERR(server, TCP_SERVER_ERR_CREATE, errno);
        return TCP_FAIL;
    }
    server->socketFd = ret;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;  
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
    server_addr.sin_port = htons(server->listenPort); 

    ret = bind(server->socketFd, (struct sockaddr *)&server_addr, sizeof(server_addr));  
    if(ret == -1)
    {
        TCP_SERVER_SET_ERR(server, TCP_SERVER_ERR_BIND, errno);
        return TCP_FAIL;
    }

    ret = listen(server->socketFd, server->maxListenCount);  
    if(ret == -1)  
    {  
        TCP_SERVER_SET_ERR(server, TCP_SERVER_ERR_LISTEN, errno);
        return TCP_FAIL;
    } 

    return TCP_SUCCESS;
}


int closeTcpServer(tcpServer *server)
{
    socketLock((baseNormal*)server);//
    close(server->socketFd);
    server->inner_errno = 0;
    server->outer_errno = 0;
    socketUnlock((baseNormal*)server);//
    return socketMutexDestroy((baseNormal*)server);
}

//************************* Control Funciton **************************//
void tcpServer_closeConnect(tcpConnect *socketConnect)
{
    close(socketConnect->fd);
}

int tcpServer_waitConnect(tcpServer *server, tcpConnect *socketConnect)
{
    int addr_len = sizeof(struct sockaddr_in);
    int ret = 0;

    memset(socketConnect, 0, sizeof(tcpConnect));

    ret = accept(server->socketFd, (struct sockaddr *)&(socketConnect->addr), &addr_len);  
    if(ret == -1)
    {
        TCP_SERVER_SET_ERR(server, TCP_SERVER_ERR_ACCEPT, errno);
        memset(socketConnect, 0, sizeof(tcpConnect));
        return TCP_FAIL;
    }

    socketConnect->fd = ret;
    socketConnect->server = server;

    return TCP_SUCCESS;

}


int tcpServer_waitConnect_timeout(tcpServer *server, tcpConnect *socketConnect, long s, long us)
{

    int ret = select_read(server->socketFd, s, us);
    if(ret < 0)//error
    {
        TCP_SERVER_SET_ERR(server, TCP_SERVER_ERR_SELECT_READ, errno);
        return TCP_FAIL;
    }
    else if(ret == 0)//timeout
    {
        errno = ETIMEDOUT;    
        return TCP_TIMEOUT;   
    }
    else//server readable
    {
        return tcpServer_waitConnect(server, socketConnect);
    }

}


int tcpServer_read(tcpConnect *socketConnect, char* buffer, int bufferLen)
{
    int ret = recv(socketConnect->fd, buffer, bufferLen, 0);
    if(ret < 0)
    {
        TCP_SERVER_SET_ERR(socketConnect->server, TCP_SERVER_ERR_RECV, errno);
        return TCP_FAIL;
    }
    else if(ret == 0)
        return TCP_END; 

    return ret;
}

int tcpServer_read_timeout(tcpConnect *socketConnect, char* buffer, int bufferLen, long s, long us)
{
    int ret = select_read(socketConnect->fd, s, us);
    if(ret < 0)//error
    {
        TCP_SERVER_SET_ERR(socketConnect->server, TCP_SERVER_ERR_SELECT_READ, errno);
        return TCP_FAIL;
    }
    else if(ret == 0)//timeout
    {
        errno = ETIMEDOUT;    
        return TCP_TIMEOUT;   
    }
    else//readable
    {
        return tcpServer_read(socketConnect, buffer, bufferLen); 
    }
    
}

int tcpServer_write(tcpConnect *socketConnect,const char* buffer, int bufferLen)
{
    int ret = send(socketConnect->fd, buffer, bufferLen, 0);
    if(ret < 0)
    {
        TCP_SERVER_SET_ERR(socketConnect->server, TCP_SERVER_ERR_SEND, errno);
        return TCP_FAIL;
    }
    else if(ret == 0)
        return TCP_END; 

    return ret;
}



char* tcpServer_getClientAddr(tcpConnect *socketConnect)
{
    return getSocketAddr(socketConnect->addr);
}

int  tcpServer_getClientPort(tcpConnect *socketConnect)
{
    return getSocketPort(socketConnect->addr);
}

