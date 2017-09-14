#include "udp_server.h"
#include "../serial/usb_serial.h"

UdpServer::UdpServer(int port):MyThreadClass(){
	if((this->server_socket = socket(PF_INET,SOCK_DGRAM,0))<0){
		perror("Cannot create socket\n");
		exit(1);
	}

	this->port = port;
	struct sockaddr_in serveraddr;
	bzero((char *)&serveraddr, sizeof(serveraddr));
	bzero((char *)&this->clientaddr,sizeof(clientaddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_port=htons(port);

	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	test_mode=0;
	if(bind(server_socket, (struct sockaddr*)&serveraddr, sizeof(serveraddr))<0){
		perror("Cannot Bind the UDP Server\n");
		exit(1);
	}

}
UdpServer::UdpServer(int port, UsbSerial *leftHand):UdpServer(port){
	this->leftHand = leftHand;
}

UdpServer::UdpServer(int port, UsbSerial *leftHand, UsbSerial *rightHand):UdpServer(port,leftHand){
	this->rightHand = rightHand;
}
UdpServer::~UdpServer(){
	close(server_socket);
}

void UdpServer::setTempUnits(Temp* lhand, Temp* rhand){
	leftTempUnit = lhand;
	rightTempUnit = rhand;
}
void UdpServer::setTestMode(int mode){
	test_mode = mode;
}

void UdpServer::send(char* data, int length){

	int len = sendto(server_socket,data,length,0,(sockaddr*)&clientaddr, sizeof(clientaddr));
	
	if(test_mode){
		for(int i=0;i<length;i++){
			std::cout<<data[i];
		}
		std::cout<<"\n";
	}

}

void UdpServer::recv(){
	addr_length = sizeof(clientaddr);
	int buf_len = recvfrom(server_socket,buffer,sizeof(buffer),0,(sockaddr*)&clientaddr,
			(socklen_t*)&addr_length);
	


	// control motor & peltier
	commandControl(buffer, buf_len);	
	
	
	if(test_mode){
		for(int i=0;i<buf_len;i++){
			int j = (int) buffer[i];
			printf("%x",j);
		}
		cout<<endl;
	}

}

void UdpServer::commandControl(char* buffer, int length){
	
	// Analyse Command
	//*********************************  signal sequence ************************************
	//	[ header][ cmd type ][ hand ][ finger No. or peltier Mode ][ pwm val][ trailer ]
	//---------------------------------------------------------------------------------------
	// char	|	|peltier:'p'|  'l'  |finger & palm vibration motor| 	    |		|
	// or	|  's'	|	    |  or   |		0~11		  |  0~255  |   'e'	|
	// val	|	|vibe :  'v'|  'r'  |        peltier mode	  |	    |		|
	// range|	|	    |	    |  'c' for cool, 'h' for hot  |	    |		|
	//***************************************************************************************
	
	if(length!=6 || buffer[0]!='s'||buffer[length-1]!='e'){
		std::cout<<"Invalid Udp Command."<<endl;
		return;
	}

	char mode = buffer[1];
	char hand = buffer[2];
	// Peltier(Temp) Mode
	if(mode=='p'){
		char cmd[3] = {buffer[3],buffer[4],buffer[5]};
		Temp* tempModule = (hand=='l') ? leftTempUnit : rightTempUnit;
		tempModule->receiveMessage(cmd,3);
	}
	// Vibration Mode
	else if(mode=='v')
	{
		char cmd[4] = {buffer[3],buffer[4],0,buffer[5]};
		UsbSerial* serialHand = (hand=='l') ? leftHand : rightHand;
		serialHand->send(cmd,4);

	}

}

void UdpServer::InternalThreadEntry(){
	while(1){
		recv();
		usleep(500);
	}
}


	
void UdpServer::setLeftHand(UsbSerial *leftHand){
	this->leftHand = leftHand;
}

void UdpServer::setRightHand(UsbSerial *rightHand){
	this->rightHand = rightHand;
}

void UdpServer::setHands(UsbSerial *leftHand, UsbSerial *rightHand){
	setLeftHand(leftHand);
	setRightHand(rightHand);
}


