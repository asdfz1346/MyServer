#include "client.h"
#include<iostream>
using namespace std;
int main(){
	client C;
	CtoS ctos;
	StoC stoc;

	int fd = C.createsocket("175.24.5.212",12345);
	if(fd==-1)return 0;
	ctos.cmd = 5;
	ctos.parkid = 1;
	memset(&stoc, 0, sizeof stoc);
	C.m_send(fd, ctos);
	int num = 0;
	stoc.cmd = 10;
	while (stoc.cmd==10) {
		stoc = C.m_recv(fd);
		cout << stoc.carid << endl;
		num++;
	}
	cout << num << endl;
	int i=0;
	socklen_t  len=10;
	cout << getsockopt(fd,SOL_SOCKET,SO_SNDLOWAT,&i,&len);
	return 0;
}
