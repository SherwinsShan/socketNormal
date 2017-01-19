#include "tcpClientNormal.h"
tcpClient client;

void exit_Handler(void)
{
    printf("Close TCPClient\n");
    closeTcpClient(&client);
}

int main(void)
{
    int ret; 
    atexit(exit_Handler);

    if(setTcpClient(&client) != TCP_SUCCESS)
    {
        return 0;
    }

    if(createTcpClient(&client) != TCP_SUCCESS)
    {
        tcpClient_showErr(&client);
        return 0;
    }

    if(tcpClient_connect(&client, "192.168.10.170", 9001) != TCP_SUCCESS)
    {
        tcpClient_showErr(&client);
        return 0;
    }
    while(1)
    {
        char buf[128];
        memset(buf, 0, 128);
        ret = tcpClient_read(&client, buf, 128);
        if(ret == TCP_FAIL)
        {
            tcpClient_showErr(&client);
            break;
        }
        else if(ret == TCP_END)
        {
            printf("End\n");
            break;
        }

        printf("Get: %s\n", buf);

        ret = tcpClient_write(&client, "Im Client", sizeof("Im Client"));
        if(ret == TCP_FAIL)
        {
            tcpClient_showErr(&client);
            break;
        }
        else if(ret == TCP_END)
        {
            printf("End\n");
            break;
        }
        sleep(1);

    }
    return 0;

}