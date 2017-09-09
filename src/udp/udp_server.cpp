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


void UdpServer::send(char* data, int length){

	int len = sendto(server_socket,data,length,0,(sockaddr*)&clientaddr, sizeof(clientaddr));

}

void UdpServer::recv(){
	addr_length = sizeof(clientaddr);
	int buf_len = recvfrom(server_socket,buffer,sizeof(buffer),0,(sockaddr*)&clientaddr,
			(socklen_t*)&addr_length);
	


	// control motor & peltier
	commandControl(buffer, buf_len);	
	
	
#ifdef DEBUG	
	for(int i=0;i<buf_len;i++){
		cout<<buffer[i];
	}
	cout<<endl;
#endif

}

void UdpServer::commandControl(char* buffer, int length){
	
	// Analyse Command
	

	// send vibration info to gloves
	leftHand->send(buffer,length);
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


