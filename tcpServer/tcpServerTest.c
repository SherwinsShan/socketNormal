#include "tcpServerNormal.h"

int main(void)
{
    int ret;
    tcpServer server;
    setTcpServer(&server, 9001, 2);

    ret = createTcpServer(&server);
    if( ret != TCP_SUCCESS)
    {
        printf("createTcpServer Fail\n");
        tcpServer_showErr(&server);
        return 0;
    }
    printf("createTcpServer successful\n");
    tcpConnect connect;
    while(1)
    {
        printf("wait\n");
        ret = tcpServer_waitConnect(&server, &connect);
        if(ret == TCP_FAIL)
        {
            tcpServer_showErr(&server);
            break;
        }
        else if(ret == TCP_END || ret == TCP_TIMEOUT)
        {
            tcpServer_showErr(&server);
            continue ;
        }
        while(1)
        {
            ret = tcpServer_write(&connect, "Hello,Client!\r\n", sizeof("Hello,Client!\r\n"));
            if(ret == TCP_FAIL)
            {
                tcpServer_showErr(&server);
                break ;
            }
            else if(ret == TCP_END)
            {
                printf("End\n");
                break;
            }

            printf("Send!\n");

            char buf[128];
            memset(buf, 0 ,128);
            ret = tcpServer_read(&connect, buf, 128);
            if(ret == TCP_FAIL)
            {
                tcpServer_showErr(&server);
                break ;
            }
            else if(ret == TCP_END)
            {
                printf("End\n");
                break;
            }
            printf("get: %s\n",buf);

        }
        tcpServer_closeConnect(&connect);
        
    }
    closeTcpServer(&server);
    return 0;
}