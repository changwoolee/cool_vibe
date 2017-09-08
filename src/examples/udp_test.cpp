#include "../udp/udp_server.h"
#include <string>
int main(void){
	UdpServer udp_server(7000);

	if(!udp_server.StartInternalThread()){
		std::cout<<"Cannot Run Udp Module"<<std::endl;
		exit(1);
	}

	string input_str;
	char wr_buffer[100];

	while(1){
		std::cin>>input_str;
		if(input_str.length()>100){
			std::cout<<"Please write short message"<<std::endl;
			continue;
		}
		strcpy(wr_buffer,input_str.c_str());
		udp_server.send(wr_buffer,input_str.length());
	}

	return 0;
}


