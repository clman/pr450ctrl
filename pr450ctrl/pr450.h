#ifndef _PR450_H_
#define _PR450_H_

#include "serial.h"

#define ERROR_BCC			(-100)

int pr450_open(int port,long bps);
int pr450_close();
int pr450_isopened();
int pr450_cmdsend(const char *data,int len);
int pr450_resprecv(char *data);
int pr450_commclr();
int pr450_changemode(long bps);

#endif