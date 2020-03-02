//#ifndef __CLIENT_H
//#define __CLIENT_H
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <arpa/inet.h>  //for htonl() and htons()
#include <fcntl.h>
#include <sys/epoll.h>
#include <signal.h>     //for signal()
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <list>
#include <set>
#include <time.h>
#include <sstream>
#include <deque>
#include <vector>
#include <iomanip> //for std::setw()/setfill()
#include <mysql/mysql.h>
#include <sys/stat.h>
#include <memory>
#include<string>
#include<mysql/mysql.h>
#define LISTEN_NUM 5
#define THREAD_NUM 1
/*CtoS cmd*/
#define PUTIN 1
#define PUTOUT 2
#define ORDER 3

/*StoC cmd*/
#define PUTINFAIL 0
#define PUTINSUCCESS 1
#define PUTOUTSUCCESS 2
#define PUTOUTFAIL 3
#define ORDERSUCCESS 4
#define ORDERFAIL 5

/*buf*/
#define SND_BUF_SIZE 32
#define RCV_BUF_SIZE 88

typedef struct CtoS{
	int cmd;
	int parkid;
	char carid[12];
	char tele[12];
}CtoS;
typedef struct StoC{
	int cmd;
	char carid[12];
	char tele[12];
	char intime[20];
	char outtime[20];
	char ordertime[20];
}StoC;

class client{
public:
	client();
	~client();
	int createsocket(const char*ip,const int port);
	int m_send(int fd,CtoS ctos);
	StoC  m_recv(int fd);
private:
	char snd_buf[SND_BUF_SIZE];
	char rcv_buf[RCV_BUF_SIZE];
	void printerror();

};
//#endif
