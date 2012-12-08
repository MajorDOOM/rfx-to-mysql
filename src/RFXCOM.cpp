#include "RFXCOM.h"


void *thread_mysql(void *arg) {
   MYSQL_RES *res;
   MYSQL_ROW row;
   string server = ini.GetValue("MYSQL", "server", "localhost");
   string user = ini.GetValue("MYSQL", "user", "root");
   string password = ini.GetValue("MYSQL", "password", "");
   string database = ini.GetValue("MYSQL", "database", "test");
   MYSQLconn = mysql_init(NULL);
   /* Connect to database */
   if (!mysql_real_connect(MYSQLconn, server.c_str(),
      user.c_str(), password.c_str(), database.c_str(), 0, NULL, 0)) {
      fprintf(stderr, "MySQL: %s\n", mysql_error(MYSQLconn));
      exit(1);
   }
   printf("MySQL: OK\n");
   /* send SQL query */
   if (mysql_query(MYSQLconn, "show tables")) {
      fprintf(stderr, "%s\n", mysql_error(MYSQLconn));
      exit(1);
   }
   res = mysql_use_result(MYSQLconn);
   /* output table name */
   printf("MySQL: Tables in database:\n");
   while ((row = mysql_fetch_row(res)) != NULL)
      printf("\t%s\n", row[0]);
   /* close connection */
   mysql_free_result(res);
}

void *thread_read(void *arg) {
	union tRBUF buffer;
	BYTE *pbuffer;
	pbuffer=reinterpret_cast<BYTE*>(&buffer);
	BYTE rec;
	
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		DWORD dwBytesRead = 0;
		while (ReadFile(rfxcom, pbuffer, 1, &dwBytesRead, NULL)) {
			if ((*pbuffer)>0 & (*pbuffer)<37) {
				for (int i=1;i<=(*pbuffer);i++) ReadFile(rfxcom, pbuffer+i, 1, &dwBytesRead, NULL);
	#else
		while (read((int)rfxcom,pbuffer,1)) {
			if ((*pbuffer)>0 & (*pbuffer)<37) {
				for (int i=1;i<=(*pbuffer);i++) read(rfxcom,(pbuffer+i),1);
	#endif
			unsigned int result = analyse_tRBUF(&buffer);
			if (!result) {
				printf("RAWRCV:L:%.2d | T:%02hhX | ST:%02hhX | SEQ=%02hhX | M:", *(pbuffer), *(pbuffer+1), *(pbuffer+2), *(pbuffer+3));
				for (int i=4;i<=(*pbuffer);i++) printf("%02hhX.",(*(pbuffer+i)));
				printf("\n");
				mysqlINSERTRAW(&buffer);
			}
		}
		for (int i=0;i<37;i++) *(pbuffer+i)=0;
	}
}
void send_message(BYTE *message) {
	int i;
	printf("RAWSND:L:%.2d | T:%02hhX | ST:%02hhX | SEQ=%02hhX | M:", message[0], message[1], message[2], message[3]);
	for( i=4 ; i <= message[0] ; i++ ) printf("%02hhX.",message[i]);
	printf("\n");
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		DWORD dwBytesRead = 0;
		WriteFile(rfxcom, message, message[0] + 1,&dwBytesRead, NULL);
	#else
		write(rfxcom,message,message[0] + 1);
	#endif	
  }
void *thread_write(void *arg) {
	BYTE *message;
	message=(BYTE*)malloc(14);
	for (int i=0;i<14;i++) message[i]=0;
	message[0] = 0x0D;	
	send_message(message);
	Sleep(1000);
	printf("Flush IO données\n");
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		PurgeComm(rfxcom,PURGE_RXCLEAR|PURGE_TXCLEAR);
		FlushFileBuffers(rfxcom);		
	#else
		tcflush(rfxcom, TCIOFLUSH );
	#endif
	Sleep(2000);
	message[4] = 0x02;
	send_message(message);	
	//message[4] = 0x04;
	//send_message(message);
	//message[4] = 0x05;
	//send_message(message);
	free(message);
}

void *thread_debugcom(void *arg) {
	BYTE *message;
	while (1) {
		message=(BYTE*)malloc(14);
		for (int i=0;i<14;i++) message[i]=0;
		message[0] = 0x0D;	
		send_message(message);
		Sleep(10);
		free(message);
	}
}

int main(int argc, char *argv[]) {
	pthread_t t_read,t_write,t_mysql,t_debugcom;
	void *rc_read,*rc_write;	
	printf("RFXCOM RFXTRX433 USB Service %s\n",VERSION); 
	printf("\n");
	if (argc<1) {printf("Usage: RFXCOM [<INI FILE>]\n");return(-1);}

	ini.SetUnicode();
	if (argc<2) {
		printf("Load ini file: %s\n","rfxcom.ini"); 
		ini.LoadFile("rfxcom.ini");
	} else {
		printf("Load ini file: %s\n",argv[1]); 
		ini.LoadFile(argv[1]);
	}
	int debug = (strcmp ("0",ini.GetValue("DEBUG", "active", "0")) != 0);
	int service = (strcmp ("0",ini.GetValue("GENERAL", "service", "0")) != 0);
	const char * comport = ini.GetValue("GENERAL", "comport", "/dev/ttyUSB0");
	if (service) {
		printf("Waiting MySQL Serveur for 1 minute...\n"); 
		Sleep(60000);
	}
	printf("Connexion à %s\n",comport);
	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		rfxcom = CreateFile(comport,GENERIC_READ | GENERIC_WRITE,0,0,OPEN_EXISTING,0,0);
		if (rfxcom == INVALID_HANDLE_VALUE) {perror("INVALID_HANDLE_VALUE");printf("\n");return(-1);}	
		DCB dcbSerialParams = {0};
		dcbSerialParams.DCBlength=sizeof(dcbSerialParams);
		if (!GetCommState((HANDLE)rfxcom, &dcbSerialParams)) {perror("could not get the state of the comport");printf("\n");return(-1);}
		dcbSerialParams.BaudRate=38400;
		dcbSerialParams.ByteSize=8;
		dcbSerialParams.StopBits=ONESTOPBIT;
		dcbSerialParams.Parity=NOPARITY;
		if(!SetCommState((HANDLE)rfxcom, &dcbSerialParams)){perror("could not set the state of the comport");printf("\n");return(-1);}		
		COMMTIMEOUTS timeouts={0};
		timeouts.ReadIntervalTimeout=50;
		timeouts.ReadTotalTimeoutConstant=50;
		timeouts.ReadTotalTimeoutMultiplier=10;
		timeouts.WriteTotalTimeoutConstant=50;
		timeouts.WriteTotalTimeoutMultiplier=10;
		if(!SetCommTimeouts(rfxcom, &timeouts)){perror("could not set the timeout of the comport");printf("\n");return(-1);}	
	#else
		struct termios oldtio,newtio;
		if ((rfxcom = open(comport, O_RDWR )) == -1 ) {perror("Connexion impossible");printf("\n");return(-1);}
		tcgetattr(rfxcom, &oldtio); 
		bzero(&newtio, sizeof(newtio));
		newtio.c_cc[VTIME]    = 0;
		newtio.c_cc[VMIN]     = 1;
		newtio.c_cflag	= ( CS8 | CLOCAL | CREAD ) ;
		cfsetospeed(&newtio,B38400);
		cfsetispeed(&newtio,B38400);
		tcsetattr(rfxcom,TCSANOW,&newtio);
	#endif
		

	pthread_create(&t_read, NULL, thread_read, NULL);
	pthread_create(&t_write, NULL, thread_write, NULL);
	pthread_create(&t_mysql, NULL, thread_mysql, NULL);
	if (debug) pthread_create(&t_debugcom, NULL, thread_debugcom, NULL);
	
	pthread_join(t_read,&rc_read);   
	pthread_cancel(t_write); 
	pthread_cancel(t_mysql); 		
	if (debug) pthread_cancel(t_debugcom); 		
	
	printf("Fermeture de %s\n",comport);

	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		CloseHandle(rfxcom);
	#else
		tcsetattr(rfxcom,TCSANOW,&oldtio);
		close(rfxcom);	
	#endif
	printf("Fermeture de MySQL\n");
	mysql_close(MYSQLconn);
	printf("Bye\n");
	return 0;
}
