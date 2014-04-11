#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <windows.h>
typedef struct {
	long ownsize;
	HANDLE handle;
	int  error;
	int  timeout;
}SERIAL_HANDLE;

#define SUCCESS				(0)
#define FAILURE				(-1)

#ifndef NO_ERROR
#define NO_ERROR			(0)
#endif
#define ERROR_PARAMETER		(-1)
#define ERROR_NOTOPEN		(-2)
#define ERROR_OPENED		(-3)



int serial_open(SERIAL_HANDLE *serial,long port);
int serial_close(SERIAL_HANDLE *serial);
int serial_mode(SERIAL_HANDLE *serial,long bps,int data,int pari,int stop);
int serial_isopened(SERIAL_HANDLE *serial);
int serial_send(SERIAL_HANDLE *serial,const char *data,int len);
int serial_redv(SERIAL_HANDLE *serial,char *data,int len,int timeout);
int serial_clear(SERIAL_HANDLE *serial);
int serial_loc(SERIAL_HANDLE *serial);

#endif 