#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>
#include <fcntl.h> 
#include <string.h>

#define SERVERIP "127.0.0.1" 
#define SERVERPORT 12345

 	
#define PUTIN 	1
#define PUTOUT	2

/*back value*/
#define PUTINSUCCESS	1
#define PUTINFAIL	2
typedef struct msgtoServer{
	int cmd;
	int parkid;
	char carid[8];
	char tele[12];
}CtoS;
typedef struct msgtoClient{
	int cmd;
	char intime[20];
	char outtime[20];
	char ordertime[20];
}StoC;

int main(){
	char snd_buf[30];
	char rev_buf[65];
	/*init ctos */
	int client;
	CtoS t;
	memcpy(t.carid,"0234567",8);	
	t.cmd  = PUTIN;
	t.parkid =1;	
	/*init sockaddr_in */
	struct sockaddr_in s;
	memset(&s,0,sizeof s);
	s.sin_family = AF_INET;
	s.sin_addr.s_addr = inet_addr(SERVERIP);
	s.sin_port   = htons(SERVERPORT);
	client = socket(AF_INET,SOCK_STREAM,0);

	/*copy to snd_buf*/
	memset(snd_buf,0,sizeof snd_buf);
	memcpy(snd_buf,&t,sizeof snd_buf);
	
	/*connect and send*/
	connect(client,(struct sockaddr *)&s,sizeof (struct sockaddr));
	send(client,snd_buf,sizeof snd_buf,0);
	sleep(1);

	/*recv and prinft*/
	recv(client,rev_buf,sizeof rev_buf,0);
	StoC stoc;
	memcpy(&stoc,rev_buf,sizeof stoc);
	if(stoc.cmd==PUTINSUCCESS)
		printf("success;\n");
	else
		printf("fail;");
	return 0;
}



