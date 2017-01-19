#ifndef _TCPSERVERNORMAL_H
#define _TCPSERVERNORMAL_H
#include "tcp_ip_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define TCP_SERVER_ERR_NO           0
#define TCP_SERVER_ERR_CREATE       1
#define TCP_SERVER_ERR_BIND         2
#define TCP_SERVER_ERR_LISTEN       3
#define TCP_SERVER_ERR_ACCEPT       4
#define TCP_SERVER_ERR_SELECT_READ  5
#define TCP_SERVER_ERR_RECV         6
#define TCP_SERVER_ERR_SEND         7
#define TCP_SERVER_ERR_SELECT_WRITE 8

#pragma pack(4)
typedef struct _tcpServerNormal
{
    int                 inner_errno;
    int                 outer_errno;
    pthread_mutex_t     mutex;
    int                 socketFd;
    unsigned short      listenPort;
    int                 maxListenCount;   
}tcpServer;
#pragma pack()

typedef struct _tcpConnect
{
    int                 fd;
    struct sockaddr_in  addr;
    tcpServer*          server;
}tcpConnect;

void tcpServer_showErr(tcpServer *server);

int setTcpServer(tcpServer *server, unsigned short port ,int maxListen);
int closeTcpServer(tcpServer *server);
int createTcpServer(tcpServer *server);


int  tcpServer_waitConnect(tcpServer *server, tcpConnect *socketConnect);
int  tcpServer_waitConnect_timeout(tcpServer *server, tcpConnect *socketConnect, long s, long us);
void tcpServer_closeConnect(tcpConnect *socketConnect);

int tcpServer_read(tcpConnect *socketConnect,char* buffer, int bufferLen);
int tcpServer_read_timeout(tcpConnect *socketConnect,char* buffer, int bufferLen, long s, long us);

int tcpServer_write(tcpConnect *socketConnect,const char* buffer, int bufferLen);

char* tcpServer_getClientAddr(tcpConnect *socketConnect);
int   tcpServer_getClientPort(tcpConnect *socketConnect);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif
