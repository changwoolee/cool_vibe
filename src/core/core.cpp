#include "../udp/udp_server.h"
#include "../serial/usb_serial.h"
#include "../temp_control/temp.h"
#include <string.h>

int main(int argc, char* argv[]){
	Temp tempUnitLeft(Temp::LEFT_HAND);
	Temp tempUnitRight(Temp::RIGHT_HAND);
	UdpServer udpServer(7000);
	udpServer.setTempUnits(&tempUnitLeft,&tempUnitRight);


	UsbSerial lhand("/dev/ttyACM0",115200,&udpServer);
	lhand.setTempUnit(&tempUnitLeft);
	//UsbSerial rhand("dev/ttyACM1",115200,&udpServer);
	//rhand.setTempUnit(&tempUnitRight);
	
	udpServer.setLeftHand(&lhand);
	//udpServer.setHands(&lhand, &rhand);

	if(argc>1){
		if(!strcmp(argv[1],"-t")){
			if(!strcmp(argv[2],"udp")||!strcmp(argv[3],"udp")){
				udpServer.setTestMode(1);
			}
			if(!strcmp(argv[2],"serial")||!strcmp(argv[3],"serial")){
				lhand.setTestMode(1);
				//rhand.setTestMode(1);
			}
		}
		else{
			std::cout<<"Invalid options.\n-t : test mode (udp, serial)\n";
		}
	}

	
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




