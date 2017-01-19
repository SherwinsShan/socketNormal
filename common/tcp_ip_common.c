#include "tcp_ip_common.h"


//**************************** multi-thread synchronization ***********************//
int socketMutexInit(baseNormal *base)
{
    
    if(pthread_mutex_init(&(base->mutex), NULL) != 0 )  
    {  
        printf("%s\n",strerror(errno));
        return TCP_FAIL;
    } 
    else
    {
        return TCP_SUCCESS;
    }
}
 
int socketMutexDestroy(baseNormal *base)
{
    if(pthread_mutex_destroy(&(base->mutex)) != 0)
    {
        printf("%s\n",strerror(errno));
        return TCP_FAIL;
    }
    else
    {
        return TCP_SUCCESS;
    }
    
} 
int socketLock(baseNormal *base)
{
    if(pthread_mutex_lock(&(base->mutex)) != 0)
    {
        printf("%s\n",strerror(errno));
        return TCP_FAIL;
    }
    else
    {
        return TCP_SUCCESS;
    }
    
} 
int socketUnlock(baseNormal *base)
{
    if(pthread_mutex_unlock(&(base->mutex)) != 0)
    {
        printf("%s\n",strerror(errno));
        return TCP_FAIL;
    }
    else
    {
        return TCP_SUCCESS;
    }
} 
//************************* set errno *****************************************8/
void socketSetErrno(baseNormal *base, int a, int b)
{
    socketLock(base); 
    base->inner_errno = a; 
    base->outer_errno = b; 
    socketUnlock(base);
}

//********************************** select ***************************************/
int select_read(int fd, long sec, long usec)
{
    fd_set fdSet; 
    struct timeval waitTime;  
    int ret = 0; 

    FD_ZERO(&fdSet);    
    FD_SET(fd, &fdSet);      
    
    waitTime.tv_sec = sec;    
    waitTime.tv_usec = usec; 

    ret = select(fd + 1, &fdSet, NULL, NULL, &waitTime);   
    if(ret < 0)//error
    {
        return -1;
    }
    else if(ret == 0)//timeout
    {
        errno = ETIMEDOUT;    
        return 0;   
    }
    else//readable
    {
        return 1;
    }
}

int select_write(int fd, long sec, long usec)
{
    fd_set fdSet; 
    struct timeval waitTime;  
    int ret = 0; 

    FD_ZERO(&fdSet);    
    FD_SET(fd, &fdSet);      
    
    waitTime.tv_sec = sec;    
    waitTime.tv_usec = usec; 

    ret = select(fd + 1, NULL, &fdSet, NULL, &waitTime);   
    if(ret < 0)//error
    {
        return -1;
    }
    else if(ret == 0)//timeout
    {
        errno = ETIMEDOUT;    
        return 0;   
    }
    else//writeable
    {
        return 1;
    }
}

//**************************************** Address ****************************************/
char* getSocketAddr(struct sockaddr_in in)
{
    return (char*)inet_ntoa(in.sin_addr);
}

unsigned short getSocketPort(struct sockaddr_in in)
{
    return ntohs(in.sin_port);
}
//************************************ Other Option ****************************************/

int setSocketReuse(int socket, bool bReuseaddr)
{
    return setsockopt(socket, SOL_SOCKET , SO_REUSEADDR,(const char*)&bReuseaddr, sizeof(bool));
}

int setSocketSendTimeout(int socket,int ms)
{
    return setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&ms, sizeof(int));
}

int setSocketRecvTimeout(int socket,int ms)
{
    return setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&ms, sizeof(int));
}

int setSocketSendBufSize(int socket, int b)
{
    return setsockopt(socket, SOL_SOCKET, SO_SNDBUF,(const char*)&b, sizeof(int));
}

int setSocketRecvBufSize(int socket, int b)
{
    return setsockopt(socket, SOL_SOCKET, SO_RCVBUF,(const char*)&b, sizeof(int));
}

//************************************* keepAlive ****************************************/
// 是否开启keepalive属性
int setSocketKeepAlive(int socket, int keepAlive)
{
    setsockopt(socket, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepAlive, sizeof(keepAlive));
}

// 如该连接在keepIdle秒内没有任何数据往来,则进行探测
int setSocketKeepIdle(int socket, int keepIdle)
{
    setsockopt(socket, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle));
}

// 探测时发包的时间间隔为keepInterval秒
int setSocketKeepInterval(int socket, int keepInterval)
{
    setsockopt(socket, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval));
}

// 探测尝试的次数.如果第1次探测包就收到响应了,则后不再发.
int setSocketKeepCount(int socket, int keepCount)
{
    setsockopt(socket, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount));
}

