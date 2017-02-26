#ifndef _UDPSERVERNORMAL_H
#define _UDPSERVERNORMAL_H
#include "tcp_ip_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define UDP_SERVER_ERR_NO           0
#define UDP_SERVER_ERR_CREATE       1
#define UDP_SERVER_ERR_BIND         2
#define UCP_SERVER_ERR_RECV         3
#define UDP_SERVER_ERR_SELECT_READ  4
#define UDP_SERVER_ERR_WRITE        5


#pragma pack(4)
typedef struct _udpServerNormal
{
    int                 inner_errno;
    int                 outer_errno;
    pthread_mutex_t     mutex;
    int socketFd;
    int listenPort;
    char listenAddr[16];
    struct sockaddr_in clientAddr;
}udpServer;
#pragma pack()


void udpServer_showErr(udpServer *server);

int setUdpServer(udpServer* server, int port);
int setUdpServerEx(udpServer* server, const char* addr, int port);
int createUdpServer(udpServer *server);
int closeUcpServer(udpServer *server);

int udpServer_waitData(udpServer *server, char* data, int dataLength);
int udpServer_waitData_timeout(udpServer *server, char* data, int dataLength, long s, long us);
int udpServer_ackData(udpServer *server, const char* data, int dataLength);

int udpServer_sendData(udpServer *server, const char* data, int dataLength, struct sockaddr_in *clientAddr);
int udpServer_sendDataEx(udpServer *server, const char* data, int dataLength, const char* addr, int port);

char* udpServer_getClientAddr(udpServer *server);
int   udpServer_getClientPort(udpServer *server);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
