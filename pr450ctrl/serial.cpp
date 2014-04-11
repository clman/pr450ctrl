#include <windows.h>
#include <stdio.h>
#include "serial.h"

static void serial_handle_initialize(SERIAL_HANDLE *serial){
	serial->ownsize = sizeof(SERIAL_HANDLE);
	serial->handle = INVALID_HANDLE_VALUE;
	serial->error = NO_ERROR;
	serial->timeout = 0;
}
static void serial_handle_clear(SERIAL_HANDLE *serial){
	serial->ownsize = 0;
	serial->handle = INVALID_HANDLE_VALUE;
	serial->error = NO_ERROR;
	serial->timeout = 0;
}

static int check_mode(long bps,int data,int pari,int stop){
	
	switch (bps){
	case 4800:
	case 9600:
	case 19200:
	case 38400:
	case 115200:
		break;
	default:
		return FAILURE;
	}

	if (!(7<=data && 8>=data)){	return FAILURE;}
	if (!(0<=pari && 3>=pari)){	return FAILURE;}
	if (!(0<=stop && 2>=stop)){	return FAILURE;}
	return SUCCESS;
}


static void setTimeout(SERIAL_HANDLE *serial, int timeout)
{
	COMMTIMEOUTS timeouts;
	GetCommTimeouts(serial->handle, &timeouts);

	timeouts.ReadIntervalTimeout = 100;
	timeouts.ReadTotalTimeoutConstant = timeout;
	timeouts.ReadTotalTimeoutMultiplier = 0;

	SetCommTimeouts(serial->handle, &timeouts);
}

int serial_isopened(SERIAL_HANDLE *serial){
	if (serial->ownsize != sizeof(SERIAL_HANDLE)){
		return FALSE;
	}
	if (serial->handle == INVALID_HANDLE_VALUE){
		return FALSE;
	}
	return TRUE;
}

int serial_open(SERIAL_HANDLE *serial,long port){

	char dev[32];
	if (!(0<port && 256 >port)){
		return FAILURE;
	}
	sprintf_s(dev,11, "\\\\.\\COM%d",port);
	serial_handle_initialize(serial);
	serial->handle = CreateFileA(dev, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   
	if (serial->handle == INVALID_HANDLE_VALUE){
		serial_handle_clear(serial);
		return FAILURE;
	}
	return SUCCESS;
}

int serial_close(SERIAL_HANDLE *serial){
	if (FAILURE == serial_isopened(serial)){
		return FAILURE;
	}
	CloseHandle(serial->handle);
	serial_handle_clear(serial);
	return SUCCESS;
}
int serial_mode(SERIAL_HANDLE *serial,long bps,int data,int pari,int stop){

	DCB	dcb;
	if (FALSE == serial_isopened(serial)){
		return FAILURE;
	}
	
	if (FAILURE == check_mode(bps,data,pari,stop)){
		return FAILURE;
	}

	GetCommState(serial->handle, &dcb);
	dcb.fBinary = TRUE;
	dcb.fParity = TRUE;
	dcb.fOutxCtsFlow = FALSE;
 	dcb.fOutxDsrFlow = FALSE;
// 	dcb.fDsrSensitivity = FALSE;
// 	dcb.fTXContinueOnXoff = FALSE;
// 	dcb.fOutX = FALSE;
// 	dcb.fInX = FALSE;
 	dcb.fErrorChar = FALSE;
 	dcb.fNull = FALSE;
// 	dcb.fDtrControl = DTR_CONTROL_ENABLE;
// 	dcb.fRtsControl = RTS_CONTROL_ENABLE;
 	dcb.fAbortOnError = FALSE;

	dcb.BaudRate = bps;
	dcb.ByteSize = data;
	dcb.Parity = pari;
	dcb.StopBits = stop;
	
	SetCommState(serial->handle, &dcb);

	serial_clear(serial);
	return SUCCESS;
}
int serial_send(SERIAL_HANDLE *serial,const char *data,int len){

   DWORD n;
	if (FALSE == serial_isopened(serial)){
		return FAILURE;
	}
	if (len < 0) {
		return 0;
	}
	WriteFile(serial->handle, data, (DWORD)len, &n, NULL);
	return n;
}
int serial_redv(SERIAL_HANDLE *serial,char *data,int len,int timeout){
	DWORD n;

	if (FALSE == serial_isopened(serial)){
		return FAILURE;
	}
	if (len < 0) {
		return 0;
	}
	if (timeout != serial->timeout) {
 		setTimeout(serial, timeout);
		serial->timeout = timeout;
	}

	ReadFile(serial->handle, data,(DWORD)len, &n, NULL);
	return n;

}
int serial_clear(SERIAL_HANDLE *serial){

	PurgeComm(serial->handle, PURGE_RXABORT | PURGE_TXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR);
	return SUCCESS;
}
int serial_loc(SERIAL_HANDLE *serial){

	return SUCCESS;
}
