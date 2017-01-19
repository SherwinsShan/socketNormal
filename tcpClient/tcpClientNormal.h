#ifndef _TCPCLIENTNORMAL_H
#define  _TCPCLIENTNORMAL_H
#include "tcp_ip_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define TCP_CLIENT_ERR_NO           0
#define TCP_CLIENT_ERR_CREATE       1
#define TCP_CLIENT_ERR_CONNECT      2
#define TCP_CLIENT_ERR_SEND         3
#define TCP_CLIENT_ERR_RECV         4
#define TCP_CLIENT_ERR_SELECT_READ  5

#pragma pack(4)
typedef struct _tcpClientNormal
{
    int                 inner_errno;
    int                 outer_errno;
    pthread_mutex_t     mutex;
    int fd;
    struct sockaddr_in  server_addr;
}tcpClient;
#pragma pack()

void tcpClient_showErr(tcpClient *client);

int setTcpClient(tcpClient *client);
int closeTcpClient(tcpClient *client);
int createTcpClient(tcpClient *client);


int tcpClient_connect(tcpClient *client, const char *addr, int port);
int tcpClient_write(tcpClient *client, const char* buffer, int bufferLen);
int tcpClient_read(tcpClient *client, char* buffer, int bufferLen);
int tcpClient_read_timeout(tcpClient *client, char* buffer, int bufferLen, long s, long us);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
