#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include "./include/gattlib.h"

using namespace std;

static uuid_t g_uuid;

int main(int argc, char* argv[]){
	uint8_t buffer[100];
	
	if(argc<2){return 1;}

	char *uuid = "0000dfb1-0000-1000-8000-00805f9b34fb";

	if(gattlib_string_to_uuid(uuid,strlen(uuid)+1,&g_uuid)<0){
		return 1;
	}
	

	gatt_connection_t* connection;

	cout<<"Start Connection"<<endl;

	connection = gattlib_connect(NULL,argv[1], BDADDR_LE_PUBLIC, BT_SEC_LOW,0,0);

	if(connection==NULL){
		cout<<"Failed to connect to ble device"<<endl;

		return 1;
	}

	int ret;
	int data;
	while(1){
		cin>>buffer;
		cout<<endl;
		ret = gattlib_write_char_by_uuid(connection, &g_uuid, buffer, sizeof(buffer));
		assert(ret);
	}
	gattlib_disconnect(connection);
	return 0;
}
