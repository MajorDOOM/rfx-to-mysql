#include <stdio.h>
#include <fcntl.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <mysql.h>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
typedef unsigned char BYTE;
typedef unsigned int uint;

#include "SimpleIni.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#include <windows.h>
	HANDLE  rfxcom;
#else
	#include <termios.h>
	int rfxcom;
	void Sleep(unsigned int dwMilliseconds) {
		sleep(dwMilliseconds/1000);
	}
#endif

#define VERSION	"0.11"

CSimpleIniA ini;
MYSQL *MYSQLconn;

#include "RFXtrx.cpp"


void *thread_mysql(void *arg);
void *thread_read(void *arg);
void send_message(BYTE *message);
void *thread_write(void *arg);
int main(int argc, char *argv[]);
void *thread_MYSQL(void *arg);
