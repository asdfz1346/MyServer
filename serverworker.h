#ifndef __SERVERWORKER_H
#define __SERVERWORKER_H
#include<string.h>
#include<string>
#ifndef __STRUCT
#define __STRUCT
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
#endif

class serverworker{
public:
	void serversend(char *snd_buf ,StoC *stoc ,int length);
	void serverrecv(char *rcv_buf ,CtoS *ctos ,int length);

}


#endif

