#include "../udp/udp_server.h"
#include "../serial/usb_serial.h"
#include "../temp_control/temp.h"


int main(void){

	Temp tempUnitLeft(Temp::LEFT_HAND);
	Temp tempUnitRight(Temp::RIGHT_HAND);
	UdpServer udpServer(7000);
	udpServer.setTempUnits(&tempUnitLeft,&tempUnitRight);


	UsbSerial lhand("/dev/ttyACM0",115200,&udpServer);
	lhand.setTempUnits(&tempUnitLeft,&tempUnitRight);
	//UsbSerial rhand("dev/ttyACM1",115200,&udpServer);
	//rhand.setTempUnits(tempUnitLeft,tempUnitRight);
	
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




