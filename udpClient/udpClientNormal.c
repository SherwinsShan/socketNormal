#include "udpClientNormal.h"

//************************************* ERROR LOG *********************************/
#define UDP_CLIENT_SET_ERR(c,a,b)  socketSetErrno((baseNormal*)c,a,b)


void udpClient_showErr(udpClient *client)
{
    const char* errinfo[] = {
        "No error",
        "Socket create error",
        "Socket connect error",
        "Socket send error",
        "Socket recv error",
        "Select read error",

    };
    socketLock((baseNormal*)client);
    printf("%s",errinfo[client->inner_errno]);
    if(client->outer_errno == 0)
        printf("\n");
    else
        printf(":: %s\n",strerror(client->outer_errno));

    socketUnlock((baseNormal*)client);
}


//******************************* setup function **********************************/
int setUdpClient(udpClient *client)
{
    client->inner_errno = 0;
    client->outer_errno = 0;
    return socketMutexInit((baseNormal*)client);
}

int closeUdpClient(udpClient *client)
{
    socketLock((baseNormal*)client);
    close(client->fd);
    client->inner_errno = 0;
    client->outer_errno = 0;
    socketUnlock((baseNormal*)client);
    return socketMutexDestroy((baseNormal*)client);
}

int createUdpClient(udpClient *client)
{
    int ret = socket(AF_INET, SOCK_DGRAM, 0);
    if(ret == -1)
    {
        UDP_CLIENT_SET_ERR(client, UDP_CLIENT_ERR_CREATE, errno);
        return UDP_FAIL;
    }
    client->fd = ret;
    return UDP_SUCCESS;
}


//******************************* RW Function *************************************/
int udpClient_getDataFrom(udpClient *client, char *data, int dataLength, struct sockaddr_in addr)
{
    int len = sizeof(struct sockaddr_in);
    int ret = recvfrom(client->fd, data, dataLength, 0, (struct sockaddr *)&addr, &len);
    if(ret == -1)
    {
        UDP_CLIENT_SET_ERR(client, UDP_CLIENT_ERR_RECV, errno);
        return UDP_FAIL;
    }
    return ret;
}

int udpClient_getDataFromEx(udpClient *client, char *data, int dataLength, const char *addrStr, int port)
{

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(addrStr);

    return udpClient_getDataFrom(client, data, dataLength, addr);
}

int udpClient_getDataFrom_timeout(udpClient *client, char *data, int dataLength, struct sockaddr_in addr, long s, long us)
{
    int ret = select_read(client->fd, s, us);
    if(ret < 0)//error
    {
        UDP_CLIENT_SET_ERR(client, UDP_CLIENT_ERR_SELECT_READ, errno);
        return UDP_FAIL;
    }
    else if(ret == 0)//timeout
    {
        errno = ETIMEDOUT;    
        return UDP_TIMEOUT;   
    }
    else//readable
    {
        return udpClient_getDataFrom(client, data, dataLength, addr); 
    }
}

int udpClient_getDataFromEx_timeout(udpClient *client, char *data, int dataLength, const char *addrStr, int port, long s, long us)
{

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(addrStr);

    return udpClient_getDataFrom_timeout(client, data, dataLength, addr, s, us);
}



int udpClient_sendDataTo(udpClient *client, const char *data, int dataLength, struct sockaddr_in addr)
{
    int len = sizeof(struct sockaddr_in);
    int ret = sendto(client->fd, data, dataLength, 0, (struct sockaddr *)&addr, len);
    if(ret == -1)
    {
        UDP_CLIENT_SET_ERR(client, UDP_CLIENT_ERR_WRITE, errno);
        return TCP_FAIL;
    }
    return ret;
}

int udpClient_sendDataToEx(udpClient *client,const char *data, int dataLength, const char *addrStr, int port)
{

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(addrStr);

    return udpClient_sendDataTo(client, data, dataLength, addr);
}