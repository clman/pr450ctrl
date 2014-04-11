// pr450ctrl.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "pr450.h"

// mainreset cmd
const char mainreset[10] = "\x08\x00\x05\x00\x01\x00\x00\x01\x0d";

int _tmain(int argc, _TCHAR* argv[])
{
	char data[1024];
	pr450_open(2,9600);								//port:com2,9600bps

	pr450_cmdsend(mainreset,sizeof(mainreset)-1);	// cmd send

	pr450_resprecv(data);							// resp recv

	pr450_close();									// close

	return 0;
}

