#ifndef TCP_IP_COMMON_H
#define TCP_IP_COMMON_H

#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <sys/socket.h>  
#include <sys/types.h>  
#include <unistd.h>  
#include <netinet/in.h>  
#include <errno.h>
#include <sys/select.h>  
#include <arpa/inet.h> 
#include <pthread.h>
#include <stdbool.h>
#include <netinet/tcp.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


///common
#define TCP_IP_SUCCESS     1
#define TCP_IP_END         0
#define TCP_IP_FAIL        -1
#define TCP_IP_TIMEOUT     -2

#define UDP_SUCCESS     TCP_IP_SUCCESS
#define UDP_END         TCP_IP_END
#define UDP_FAIL        TCP_IP_FAIL
#define UDP_TIMEOUT     TCP_IP_TIMEOUT

#define TCP_SUCCESS     TCP_IP_SUCCESS
#define TCP_END         TCP_IP_END
#define TCP_FAIL        TCP_IP_FAIL
#define TCP_TIMEOUT     TCP_IP_TIMEOUT

#pragma pack(4)
typedef struct
{
    int                 inner_errno;
    int                 outer_errno;
    pthread_mutex_t     mutex;
}baseNormal;
#pragma pack()


int socketMutexInit(baseNormal *base);
int socketMutexDestroy(baseNormal *base);
int socketLock(baseNormal *base);
int socketUnlock(baseNormal *base);
void socketSetErrno(baseNormal *base, int a, int b);



int select_read(int fd, long sec, long usec);
int select_write(int fd, long sec, long usec);

char* getSocketAddr(struct sockaddr_in in);
unsigned short getSocketPort(struct sockaddr_in in);

int setSocketReuse(int socket, bool bReuseaddr);
int setSocketSendTimeout(int socket,int ms);
int setSocketRecvTimeout(int socket,int ms);
int setSocketSendBufSize(int socket, int b);
int setSocketRecvBufSize(int socket, int b);
int setSocketKeepAlive(int socket, int keepAlive);
int setSocketKeepIdle(int socket, int keepIdle);
int setSocketKeepInterval(int socket, int keepInterval);
int setSocketKeepCount(int socket, int keepCount);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
