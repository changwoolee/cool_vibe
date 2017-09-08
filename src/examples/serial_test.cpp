#include "../serial/usb_serial.h"


int main(void){
	UsbSerial left_hand("/dev/ttyACM0",115200);
	if(!left_hand.StartInternalThread()){
		std::cout<<"Cannot Run recv module"<<std::endl;
	}

	char wrBuffer[10];
	//char rdBuffer[100];
	while(1){
		std::cin>>wrBuffer[0]>>wrBuffer[1]>>wrBuffer[2];
		wrBuffer[3]='p';
		//strcpy(wrBuffer,data.c_str());
		left_hand.send(wrBuffer,4);


	}
	return 0;
}

