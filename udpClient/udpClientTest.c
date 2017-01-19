#include "udpClientNormal.h"

int main(void)
{
    udpClient client;
    int ret;
    if(setUdpClient(&client) != UDP_SUCCESS)
    {
        return 0;
    }

    if(createUdpClient(&client) != UDP_SUCCESS)
    {
        udpClient_showErr(&client);
        return 0;
    }


    while(1)
    {
        char buf[128];
        memset(buf,0 ,128);
        ret = udpClient_sendDataToEx(&client, "Hello im client!", sizeof("Hello im client!"), "127.0.0.1", 9002);
        if(ret == UDP_FAIL)
        {
            udpClient_showErr(&client);
            break;
        }

        ret = udpClient_getDataFromEx(&client, buf, 128, "127.0.0.1", 9002);
        if(ret == UDP_FAIL)
        {
            udpClient_showErr(&client);
            break;
        }
        printf("Get: %s\n",buf);
        sleep(1);
    }
    closeUdpClient(&client);
    return 0;
}