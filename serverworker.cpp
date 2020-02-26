#include "serversend.h"

void serverworker::serversend(char *snd_buf,int *cmd,char *ordertime,char *intime,char *outtime,int length){
        StoC stoc;                                                                                                                                                              memset(snd_buf ,0, length);
   	stoc.cmd = cmd;  
  	if(ordertime!=NULL)
		memcpy(stoc.ordertime,ordertime,sizeof(stoc.ordertime));
     	if(intime!=NULL) 
		memcpy(stoc.intime,intime,sizeof(stoc.intime)); 
	if(outtime!=NULL)
		memcpy(stoc.outtime,outtime,sizeof(stoc.outtime));   
	return;
}
void serverworker::serverrecv(char *rcv_buf,int* cmd,int *parkid,char *carid,char *tele,int length){
      	CtoS ctos; 
	memset(&ctos,0,sizeof ctos); 
	memcpy(&ctos,rev_buf,sizeof ctos);
	*cmd =ctos.cmd;
	*parkid = ctos.parkid;
        memcpy(carid,ctos.carid,sizeof(ctos.carid)); 
	memcpy(tele,ctos.tele,siezof(ctos.tele));
	return;
}

