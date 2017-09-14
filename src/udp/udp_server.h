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
#include "../temp_control/temp.h"

//#include "../serial/usb_serial.h"

class UsbSerial;

//using namespace std;

class UdpServer : public MyThreadClass{
public:
	UdpServer(int port);
	UdpServer(int port, UsbSerial *leftHand);
	UdpServer(int port, UsbSerial *leftHand, UsbSerial *rightHand);
	~UdpServer();

	void send(char* data, int length);
	void recv();

	void setTempUnits(Temp* lhand, Temp* rhand);

	void setLeftHand(UsbSerial *leftHand);
	void setRightHand(UsbSerial *rightHand);
	void setHands(UsbSerial *leftHand, UsbSerial *rightHand);


	virtual void InternalThreadEntry();
private:
	int server_socket;
	int port;
	struct sockaddr_in clientaddr;
	int addr_length;
	Temp *leftTempUnit, *rightTempUnit;
	UsbSerial *leftHand, *rightHand;

	char buffer[100];

	void commandControl(char* buffer, int length);


};


#endif /* __UDP_SERVER_H__ */
