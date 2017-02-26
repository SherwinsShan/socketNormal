#ifndef _UDPCLIENTNORMAL_H
#define _UDPCLIENTNORMAL_H
#include "tcp_ip_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define UDP_CLIENT_ERR_NO           0
#define UDP_CLIENT_ERR_CREATE       1
#define UDP_CLIENT_ERR_RECV         2
#define UDP_CLIENT_ERR_WRITE        3
#define UDP_CLIENT_ERR_SELECT_READ  4

typedef struct _udpClientNormal
{
    int                 inner_errno;
    int                 outer_errno;
    pthread_mutex_t     mutex;
    int fd;
}udpClient;

void udpClient_showErr(udpClient *client);

int setUdpClient(udpClient *client);
int closeUdpClient(udpClient *client);
int createUdpClient(udpClient *client);



int udpClient_getDataFrom(udpClient *client, char *data, int dataLength, struct sockaddr_in *addr);
int udpClient_getDataFromEx(udpClient *client, char *data, int dataLength, const char *addrStr, int port);

int udpClient_getDataFrom_timeout(udpClient *client, char *data, int dataLength, struct sockaddr_in *addr, long s, long us);
int udpClient_getDataFromEx_timeout(udpClient *client, char *data, int dataLength, const char *addrStr, int port, long s, long us);

int udpClient_sendDataTo(udpClient *client, const char *data, int dataLength, struct sockaddr_in *addr);
int udpClient_sendDataToEx(udpClient *client,const char *data, int dataLength, const char *addrStr, int port);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
