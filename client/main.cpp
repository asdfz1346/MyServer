#include "client.h"


int main(){
	client C;
	CtoS ctos;
	StoC stoc;

//	int fd = C.createsocket("127.0.0.1",12345);
//	if(fd==-1)return 0;
	memcpy(&ctos.carid,"asdfz13",8);
	memcpy(&ctos.tele,"12312312311",12);
	ctos.cmd = PUTIN;
	ctos.parkid = 1;
//	C.m_send(fd,ctos);
//	memset(&stoc,0,sizeof stoc);
//	stoc = C.m_recv(fd);
//	printf("%d",stoc.cmd);
	
	

	int fd[100];
	for(int i=0;i<100;i++){
		memset(&stoc,0,sizeof stoc);
		fd[i] = C.createsocket("127.0.0.1",12345);
		C.m_send(fd[i],ctos);
		//stoc = C.m_recv(fd[i]);
	}
	

	return 0;
}
