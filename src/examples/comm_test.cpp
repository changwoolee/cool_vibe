#include "../udp/udp_server.h"
#include "../serial/usb_serial.h"

int main(void){

	UdpServer udpServer(7000);

	UsbSerial lhand("/dev/ttyACM0",115200,&udpServer);
	//UsbSerial rhand("dev/ttyACM1",115200,&udpServer);
	
	udpServer.setLeftHand(&lhand);
	//udpServer.setHands(&lhand, &rhand);
	
	
	if(!udpServer.StartInternalThread()){
		std::cout<<"Cannot Run Udp Module"<<std::endl;
		exit(1);
	}
	if(!lhand.StartInternalThread()){
		std::cout<<"Cannot Run Serial Module"<<std::endl;
		exit(1);
	}

	udpServer.WaitForInternalThreadToExit();
	lhand.WaitForInternalThreadToExit();
	//rhand.WaitForInternalThreadToExit();
	
	return 0;
}




