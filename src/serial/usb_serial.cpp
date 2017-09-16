#include "usb_serial.h"
UsbSerial::UsbSerial(char* PORT, int baud_rate):MyThreadClass(){
	this->PORT = PORT;
	this->baud_rate = baud_rate;
	// Open Serial Port
	fd = open(PORT, O_RDWR | O_NOCTTY | O_SYNC);
	if(fd<0){
		printf("Error : Cannot open serial port %s\n",PORT);
		exit(1);
	}

	test_mode=0;
	set_interface_attribs(fd,this->baud_rate); // baud rate : 115200

}

UsbSerial::UsbSerial(char* PORT, int baud_rate, UdpServer *udpServer):UsbSerial(PORT,baud_rate){
	this->udpServer = udpServer;
}

UsbSerial::~UsbSerial(){
	close(fd);
}

void UsbSerial::set_interface_attribs(int fd,int baud_rate){
	struct termios newtio;
	memset(&newtio,0,sizeof(newtio));
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag=0;

	newtio.c_cflag = CS8|CLOCAL|CREAD;
	switch( baud_rate ) {
		 case 115200 : newtio.c_cflag |= B115200; break;
		 case 57600 : newtio.c_cflag |= B57600; break;
		 case 38400 : newtio.c_cflag |= B38400; break;
		 case 19200 : newtio.c_cflag |= B19200; break;
		 case 9600 : newtio.c_cflag |= B9600; break;
		 case 4800 : newtio.c_cflag |= B4800; break;
		 case 2400 : newtio.c_cflag |= B2400; break;
		 default : newtio.c_cflag |= B115200; break;
	}
	newtio.c_lflag=0;
	newtio.c_cc[VMIN] = 0;
	newtio.c_cc[VTIME] = 5;
	tcflush(fd,TCIFLUSH);
	tcsetattr(fd,TCSANOW,&newtio);
}

void UsbSerial::setHandType(int type){
	if(type!=1 && type!=2){
		std::cout<<"Invalid Hand type."<<std::endl;
		exit(1);

	}
}




void UsbSerial::setTestMode(int mode){
	test_mode = mode;
}

void UsbSerial::setTempUnit(Temp *tempUnit){
	this->tempUnit = tempUnit;
}

void UsbSerial::send(char* data,int size){
	int wr = write(fd,data,size);	
	if(wr<0){
		std::string port_name = this->PORT;
		std::cout<<"Error : Cannot write serial port "
			<<port_name<<std::endl;
	}
	if(test_mode){
		for(int i=0;i<size;i++){
			std::cout<<data[i];
		}
		std::cout<<"\n";
	}

}
void UsbSerial::InternalThreadEntry(){
	while(1){
		recv();
		usleep(1000);
	}
}
void UsbSerial::recv(){
	char buffer[10];	
	int n=0;
	int buf_index=0;
	char buf = '\0';

	do{
		n = read(fd,&buf,1);
		if(buf>='0' && buf<='9'){
			buffer[buf_index] = buf;
			buf_index += n;
		}
	}while(buf != '\n' && n>0);

	if(n<0){
		std::string port_name = this->PORT;
		std::cout<<"Error : Cannot read serial port "
		<<port_name<<std::endl;
	}
	if(n==0){
		std::string port_name = this->PORT;
		std::cout<<"Warning : No data from "<<port_name<<std::endl;
	}
	int raw_data = atoi(buffer); 
	
	// push temp information to Temp Unit.
	tempUnit->setTemp(raw_data);
	// after setTemp() called, Temp Unit will handle temperature function.
	
	// Send Temp Information to Host using UDP.	
	sendTempMessageToHost( tempUnit->getTempInfo() );


	if(test_mode){
		for(int i=0;i<buf_index+1;i++){
			std::cout<<buffer[i];
		}
		std::cout<<"\n";
	}
}



void UsbSerial::sendTempMessageToHost(Temp::TempMessage *msg){
// struct TempMessage{
// 	double currentTemp;
// 	double standardTemp;
// 	double setPoint;
// 	double dT;
// 	int isPeltierOn;
// 	int isPeltierActive;
// }
	char tempData[7][30];
	sprintf(tempData[0],"%d, ",handType);
	sprintf(tempData[1],"%3.2f, ",msg->currentTemp);
	sprintf(tempData[2],"%3.2f, ",msg->standardTemp);
	sprintf(tempData[3],"%3.2f, ",msg->setPoint);
	sprintf(tempData[4],"%3.2f, ",msg->dT);
	sprintf(tempData[5],"%d, ",msg->isPeltierOn);
	sprintf(tempData[6],"%d\n",msg->isPeltierActive);

	for(int i=0;i<7;i++){
		udpServer->send(tempData[i],30);
	}
}


		
	
	
void UsbSerial::setUdpServer(UdpServer *udpServer){
	this->udpServer=udpServer;
}	


