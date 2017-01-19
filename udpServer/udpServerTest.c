#include "udpServerNormal.h"


int main(void)
{
    udpServer server;
    int ret;
    if(setUdpServerEx(&server,"127.0.0.1", 9002) != UDP_SUCCESS)
    {
        return 0;
    }

    if(createUdpServer(&server) != UDP_SUCCESS)
    {
        udpServer_showErr(&server);
        return 0;
    }

    while(1)
    {
        char buf[128];
        memset(buf, 0, 128);

        ret = udpServer_waitData_timeout(&server, buf, 128, 5, 0);
        if(ret == UDP_FAIL)
        {
            udpServer_showErr(&server);
            continue;
        }
        else if(ret == UDP_TIMEOUT)
        {
            printf("Timeout\n");
            continue;
        }

        printf("Get: %s\n",buf);

        ret = udpServer_ackData(&server, "Hello!", sizeof("Hello!"));
        if(ret == UDP_FAIL)
        {
            udpServer_showErr(&server);
            continue;
        }
        else if(ret == UDP_END)
        {
            printf("End\n");
            continue;
        }

        ret = udpServer_sendDataEx(&server, "World!", sizeof("World!"),"127.0.0.1", 6666);
        if(ret == UDP_FAIL)
        {
            udpServer_showErr(&server);
            continue;
        }
        else if(ret == UDP_END)
        {
            printf("End2\n");
            continue;
        }
    }
    closeUcpServer(&server);
    return 0;
}