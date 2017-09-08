#ifndef __UDP_SERVER_H__
#define __UDP_SERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include "../common/my_thread.h"
using namespace std;

class UdpServer : public MyThreadClass{
public:
	UdpServer(int port);
	~UdpServer();

	void send(char* data, int length);
	void recv();

	virtual void InternalThreadEntry();
private:
	int server_socket;
	int port;
	struct sockaddr_in clientaddr;
	int addr_length;
	

	char buffer[100];

};








#endif /* __UDP_SERVER_H__ */
