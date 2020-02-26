#include "client.h"
#include<iostream>
int main(){
	client C;
	CtoS ctos;
	StoC stoc;

	int fd = C.createsocket("127.0.0.1",12345);
	if(fd==-1)return 0;
	memcpy(&ctos.carid,"CCCCC",8);
	memcpy(&ctos.tele,"12312312311",12);
	ctos.cmd = PUTIN;
	ctos.parkid = 1;

	memset(&stoc,0,sizeof stoc);
	//fd[i] = C.createsocket("127.0.0.1",12345);
	C.m_send(fd,ctos);
	stoc = C.m_recv(fd);
	std::cout << stoc.cmd;
	if(stoc.cmd==PUTOUTSUCCESS){
		std::cout << stoc.intime<<std::endl<<stoc.outtime<<std::endl;	
	}
	if(stoc.cmd==PUTINSUCCESS)
		std::cout << "PUTINSUCCESS";
	if(stoc.cmd==3)
		std::cout << "PUTOUTFAIL";
	

	return 0;
}
