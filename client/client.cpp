#include"client.h"



client::client(){}
client::~client(){}
int client::createsocket(const char * ip,const int port){
	struct sockaddr_in s;
	memset(&s,0,sizeof s);
	s.sin_family 		= AF_INET;
	s.sin_addr.s_addr 	= inet_addr(ip);
	s.sin_port		= htons(port);
	int fd = socket(AF_INET,SOCK_STREAM,0);
	if(connect(fd,(struct sockaddr *)&s,sizeof s)==-1)
		printerror();
	else
		return fd;
	return 0;
	
}
int client::m_send(int fd,CtoS ctos){
	
	memset(snd_buf,0,SND_BUF_SIZE);
	memcpy(snd_buf,&ctos,sizeof ctos);
	int numofsend=0;
	numofsend = send(fd,snd_buf,SND_BUF_SIZE,0);
	if(numofsend==-1)
		printerror();
	else
		return numofsend;
	return -1;

}
StoC client::m_recv(int fd){
	memset(rcv_buf,0,RCV_BUF_SIZE);
	StoC stoc;
	memset(&stoc,0,sizeof stoc);
	if(recv(fd,rcv_buf,RCV_BUF_SIZE ,0)==-1)
		printerror();
	else{
		memcpy(&stoc,rcv_buf,sizeof stoc);	
	}
	return stoc;

}

void client::printerror(){
	printf("errno:%d\n",errno);
	return;
}
