#include "tcpClientNormal.h"



//************************************* ERROR LOG *********************************/
#define TCP_CLIENT_SET_ERR(c,a,b)  socketSetErrno((baseNormal*)c,a,b)


void tcpClient_showErr(tcpClient *client)
{
    const char* errinfo[] = {
        "No error",
        "Socket create error",
        "Socket recv error",
        "Socket send error",
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

//******************************************************************************/

int setTcpClient(tcpClient *client)
{
    client->inner_errno = 0;
    client->outer_errno = 0;
    return socketMutexInit((baseNormal*)client);
}

int closeTcpClient(tcpClient *client)
{
    socketLock((baseNormal*)client);
    close(client->fd);
    client->inner_errno = 0;
    client->outer_errno = 0;
    socketUnlock((baseNormal*)client);
    return socketMutexDestroy((baseNormal*)client);
}

int createTcpClient(tcpClient *client)
{
    int ret = socket(AF_INET, SOCK_STREAM, 0);
    if(ret == -1)
    {
        TCP_CLIENT_SET_ERR(client, TCP_CLIENT_ERR_CREATE, errno);
        return TCP_FAIL;
    }
    client->fd = ret;
    return TCP_SUCCESS;
}

//******************************************************************************/
int tcpClient_connect(tcpClient *client, const char *addr, int port)
{
    int ret;
    struct sockaddr_in addr_ser;  
    memset(&addr_ser, 0, sizeof(struct sockaddr_in));
    addr_ser.sin_family      = AF_INET;  
    addr_ser.sin_addr.s_addr = inet_addr(addr);  
    addr_ser.sin_port        = htons(port);  

    ret = connect(client->fd, (struct sockaddr *)&addr_ser, sizeof(addr_ser));  
    if(ret == -1)
    {
        TCP_CLIENT_SET_ERR(client, TCP_CLIENT_ERR_CONNECT, errno);
        return TCP_FAIL;
    }
    client->server_addr = addr_ser;
    return TCP_SUCCESS;
}


int tcpClient_write(tcpClient *client, const char* buffer, int bufferLen)
{
    int ret = send(client->fd, buffer, bufferLen, 0);
    if(ret < 0)
    {
        TCP_CLIENT_SET_ERR(client, TCP_CLIENT_ERR_SEND, errno);
        return TCP_FAIL;
    }
    else if(ret == 0)
        return TCP_END; 

    return ret;
}

int tcpClient_read(tcpClient *client, char* buffer, int bufferLen)
{
    int ret = recv(client->fd, buffer, bufferLen, 0);
    if(ret < 0)
    {
        TCP_CLIENT_SET_ERR(client, TCP_CLIENT_ERR_RECV, errno);
        return TCP_FAIL;
    }
    else if(ret == 0)
        return TCP_END; 

    return ret;
}

int tcpClient_read_timeout(tcpClient *client, char* buffer, int bufferLen, long s, long us)
{
    int ret = select_read(client->fd, s, us);
    if(ret < 0)//error
    {
        TCP_CLIENT_SET_ERR(client, TCP_CLIENT_ERR_SELECT_READ, errno);
        return TCP_FAIL;
    }
    else if(ret == 0)//timeout
    {
        errno = ETIMEDOUT;    
        return TCP_TIMEOUT;   
    }
    else//readable
    {
        return tcpClient_read(client, buffer, bufferLen); 
    }
    
}

