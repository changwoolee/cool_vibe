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

class UsbSerial : public MyThreadClass {
	public:
		UsbSerial(char* PORT,int baud_rate);
		~UsbSerial();
		void set_interface_attribs(int fd, int baud_rate, int parity);
		void send(char* data,int length);
		void recv();
		virtual void InternalThreadEntry();

	private:
		int fd;
		char* PORT;
		int baud_rate;
};




















#endif /* usb_serial.h */
