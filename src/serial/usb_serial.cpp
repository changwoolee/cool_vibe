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


	set_interface_attribs(fd,this->baud_rate,0); // 115200, 8N1

}

UsbSerial::~UsbSerial(){
	close(fd);
}

void UsbSerial::set_interface_attribs(int fd,int baud_rate,int parity){
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

	

void UsbSerial::send(char* data,int size){
	int wr = write(fd,data,size);	
	if(wr<0){
		std::string port_name = this->PORT;
		std::cout<<"Error : Cannot write serial port "
			<<port_name<<std::endl;
	}
}
void UsbSerial::InternalThreadEntry(){
	while(1){
		recv();
		usleep(1000);
	}
}
void UsbSerial::recv(){
	char buffer[100];	
	int n=0;
	int buf_index=0;
	char buf = '\0';

	do{
		n = read(fd,&buf,1);
		buffer[buf_index] = buf;
		buf_index += n;
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
	for(int i=0;i<buf_index+1;i++){
		std::cout<<buffer[i];
	}
	std::cout<<"\n";

}


		
	
	
		


