#include "clientworker.h"

void clientworker::clientsend(char * snd_buf,int cmd,int parkid,char *carid,char tele,int length){
	memset(snd_buf,0,length);
	CtoS ctos;
	memset(ctos,0,sizeof stos);
	ctos.cmd = cmd;
	ctos.parkid = parkid;
	memcpy(ctos.carid,carid,sizeof(ctos.carid));
	memcpy(ctos.tele,tele,sizeof(ctos.tele));
	memcpy(snd_buf,&ctos,length);
	return;
}
void clientworker::clientrecv(char * rcv_buf,int *cmd,char *ordertime,char *intime,char *outtime,int length){
	StoC stoc;
	memset(&stoc,0,sizeof(StoC));
	memcpy(&stoc,rcv_buf,length);
	*cmd = stoc.cmd;
	memcpy(ordertime,stoc.ordertime);
	memcpy(intime,stoc.intime);
	memcpy(outtime,stoc.outtime);
	return;
}

//time空值为char(20)"1000-01-01 00:00:00"



