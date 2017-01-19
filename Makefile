TCPServer=tcpServer_Test
TCPClient=tcpClient_Test
UDPServer=udpServer_Test
UDPClient=udpClient_Test

CC=gcc
cflag=-lpthread -I./common


TCPServerSource=common/tcp_ip_common.c tcpServer/tcpServerNormal.c tcpServer/tcpServerTest.c
TCPServerSource+=common/tcp_ip_common.h tcpServer/tcpServerNormal.h 

TCPClientSource=common/tcp_ip_common.c tcpClient/tcpClientNormal.c tcpClient/tcpClientTest.c
TCPClientSource+=common/tcp_ip_common.h tcpClient/tcpClientNormal.h 

UDPServerSource=common/tcp_ip_common.c udpServer/udpServerNormal.c udpServer/udpServerTest.c
UDPServerSource+=common/tcp_ip_common.h udpServer/udpServerNormal.h

UDPClientSource=common/tcp_ip_common.c udpClient/udpClientNormal.c udpClient/udpClientTest.c
UDPClientSource+=common/tcp_ip_common.h udpClient/udpClientNormal.h 

all: $(TCPServer) $(TCPClient) $(UDPServer) $(UDPClient)


$(TCPServer) : $(TCPServerSource)
	@$(CC) -o $@ $^ $(cflag)	-I./tcpServer
	
$(TCPClient) : $(TCPClientSource)
	@$(CC) -o $@ $^ $(cflag)	-I./tcpClient

$(UDPServer) : $(UDPServerSource)
	@$(CC) -o $@ $^ $(cflag)	-I./udpServer

$(UDPClient) : $(UDPClientSource)
	@$(CC) -o $@ $^ $(cflag)	-I./udpClient

clean:
	@rm -f $(TCPServer) 
	@rm -f $(TCPClient) 
	@rm -f $(UDPServer) 
	@rm -f $(UDPClient) 
	