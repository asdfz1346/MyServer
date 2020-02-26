#ifndef __CLIENTWORKER_H
#define __CLIENTWORKER_H
#include<string.h>
#include<string>
typedef struct msgtoServer{
	int cmd;
	int parkid;
	char carid[8];
	char tele[12];
}CtoS;


typedef struct msgtoClient{
	int cmd;//0.入库失败，1.返回入库成功 2.出库时间
	float rate;
	char intime[20];
	char outtime[20];
	char ordertime[20];

}StoC;

class clientworker{
public:
	void clientsend(char * snd_buf,int cmd,int parkid,char *carid,char tele,int length);
	void clientrecv(char * rcv_buf,int *cmd,char *ordertime,char *intime,char *outtime,int length);

}


#endif

