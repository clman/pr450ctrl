#include <windows.h>
#include "serial.h"
#include "pr450.h"

static SERIAL_HANDLE serial = {0,0,0,0};


int pr450_open(int port,long bps){
	int result;
	if (SUCCESS == (result = serial_open(&serial,port))){
		result = serial_mode(&serial,bps,8,EVENPARITY,ONESTOPBIT);
	}
	return result;
}
int pr450_close(){
	return serial_close(&serial);
}
int pr450_isopened(){
	return serial_isopened(&serial);
}
int pr450_cmdsend(const char *data,int len){
	return serial_send(&serial,data,len);
}
int pr450_resprecv(char *data){
	int result;
	int len;
	int i;
	char bcc = 0;

	result = serial_redv(&serial,data,3,500);
	if (result < 3){
		if (result < 0){ 
			return result;	
		}
		else {	
			return ERROR_TIMEOUT;
		}
	}

	len = ((data[1] << 8) | data[2])+1; //data+bcc
	result = serial_redv(&serial,&data[3],len,500);
	if (result < len){
		if (result < 0){
			return result;
		}
		else {
			return ERROR_TIMEOUT;
		}
	}
	len += 3;  //header+data+bcc
	for (i = 0;i<len ;i++){
		bcc ^= data[i]; 
	}
	if (bcc != 0){
		return ERROR_BCC;
	}
	return SUCCESS;
}
int pr450_commclr(){
	return serial_clear(&serial);
}
int pr450_changemode(long bps){
	return serial_mode(&serial,bps,8,EVENPARITY,ONESTOPBIT);
}
