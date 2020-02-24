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

#define ORDER 1
#define IN 2
#define	OUT 3
#define PUTIN 1
#define PUTOUT 2
typedef struct msgtoServer{
	int cmd;
	int parkid;
	char carid[8];
	char tele[12];
}CtoS;

int main(){
	char send_buff[25];
	int client[10]={ 0 };
	for(int i=0;i<10;i++){
		
		CtoS t;
		memcpy(t.carid,"0234567",8);
		t.carid[0]+=i;
		memcpy(t.tele,"18634291111",sizeof t.tele);
		t.cmd  = PUTIN;
		t.parkid =1;
		struct sockaddr_in s;
		memset(&s,0,sizeof s);
		s.sin_family = AF_INET;
		s.sin_addr.s_addr = inet_addr(SERVERIP);
		s.sin_port   = htons(SERVERPORT);
		client[i] = socket(AF_INET,SOCK_STREAM,0);
		if(connect(client[i],(struct sockaddr*)&s,sizeof(struct sockaddr))==-1)
			printf("errno:%d",errno);
		else
			printf("connect success\n");
		memset(send_buff,0,sizeof send_buff);
		memcpy(send_buff,&t,sizeof t);
		printf("%d byte send success,,sizeof send_buffer is%ld\n",int(send(client[i],send_buff,sizeof send_buff,0)),sizeof send_buff);
		sleep(1);	


	}	
}
