#ifndef __USB_SERIAL_H__
#define __USB_SERIAL_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string>
#include <iostream>
#include "../common/my_thread.h"
#include "../udp/udp_server.h"
#include "../temp_control/temp.h"


class UsbSerial : public MyThreadClass {
	public:
		UsbSerial(char* PORT, int baud_rate);
		UsbSerial(char* PORT, int baud_rate, UdpServer *udpServer);
		~UsbSerial();
		void set_interface_attribs(int fd, int baud_rate);
		void send(char* data,int length);
		void recv();
		void setTempUnits(Temp *lhand, Temp *rhand);

		void setUdpServer(UdpServer *udpServer);
		virtual void InternalThreadEntry();

	private:
		int fd;
		char* PORT;
		int baud_rate;
		Temp *leftTempUnit, *rightTempUnit;
		UdpServer *udpServer;
};




















#endif /* usb_serial.h */
