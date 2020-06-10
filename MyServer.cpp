#include"MyServer.h"

MyServer::MyServer(){}
MyServer::~MyServer(){}

struct ARG{
	MyServer* p;
};
/*server_listen(),accept,io,work*/
void MyServer::link_startou(const char *ip,short port){
	if(!server_listen(ip,port)){
		printf("	server_listen error\n");	
	}
	struct ARG arg;
	arg.p = this;
	std::cout << "main threadid = "<<pthread_self()<<std::endl;
	pthread_create(&accept_thread,NULL,accept_thread_proc,&arg);
	
	for(int i=0;i<THREAD_NUM;i++){
		pthread_create(&worker_thread[i],NULL,worker_thread_proc,&arg);
	}

}

/*socket ,bind ,listen,epoll_create,epolladd*/
bool MyServer::server_listen(const char *ip,short port){
	//socket
	listenfd = socket(AF_INET,SOCK_STREAM|SOCK_NONBLOCK,0);
	if(listenfd==-1){
		printf("soket error\n");
		return false;
	}
	int on =1;
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof on);
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEPORT,&on,sizeof on);

	//bind
	struct sockaddr_in serveraddr;
	memset(&serveraddr,0,sizeof serveraddr);
	serveraddr.sin_family= AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(ip);//点分十进制ip转换成一个长整数
	serveraddr.sin_port = htons(port);//转换为网络字节序
	if(bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))==-1){
		printf("bind error\n");
		return false;
	}
	//listen
	if(listen(listenfd,LISTEN_NUM)==-1){//待测试
		printf("listen() error\n");
		return false;
	}
	//epoll
	epollfd = epoll_create(1);

	struct epoll_event e;
	memset(&e,0,sizeof e);
	e.events = EPOLLIN;
	e.data.fd = listenfd;
	if(epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd,&e)==-1){
		printf("epoll_ctl error;\n");
		return false;
	}
	return true;
}

/*create mewclient ,epoll_ctl*/
void *MyServer::accept_thread_proc(void *args){
	MyServer* pthis = ((ARG*)args)->p;
	while(pthis->nstop){
		pthread_mutex_lock(&pthis->accept_mutex);
		pthread_cond_wait(&pthis->accept_cond,&pthis->accept_mutex);//block

		/*accept already,create newclientfd*/
		struct sockaddr_in clientaddr;
		socklen_t addrlen;
		int newfd = accept(pthis->listenfd,(struct sockaddr*)&clientaddr,&addrlen);
		pthread_mutex_unlock(&pthis->accept_mutex);
		if(newfd==-1) continue;
		/*set newfd sndbuf*/
		int nSndBufferLen = 4*1024*1024;
		int nLen          = sizeof(int);
		setsockopt(newfd,SOL_SOCKET,SO_SNDBUF,(char*)&nSndBufferLen,nLen);

		/*set newfd non-blocking;*/
		int oldflag = fcntl(newfd, F_GETFL, 0);
		int newflag = oldflag | O_NONBLOCK;
		if (fcntl(newfd, F_SETFL, newflag) == -1){
			std::cout << "fcntl error, oldflag = " << oldflag << ", newflag = " << newflag << std::endl;
			continue;
		}
		/*insert newfd to the set*/
	pthread_mutex_lock(&pthis->clientset_mutex);
		pthis->clientset.insert(newfd);
		pthread_mutex_unlock(&pthis->clientset_mutex);
		std::cout << "new client connect fd:"<<newfd<<std::endl;
		
		/*add epoll*/
		struct epoll_event e;
		memset (&e,0,sizeof e);
		e.events = EPOLLIN|EPOLLET;
		e.data.fd = newfd;
		if(epoll_ctl(pthis->epollfd,EPOLL_CTL_ADD,newfd,&e)==-1){
			std::cout << "epoll_ctl error,\n"<<std::endl;
		}

	}
	return NULL;
}


void MyServer::main_loop(){	
	while(nstop){
		struct 	epoll_event ev[1024];
		int n = epoll_wait(epollfd,ev,1024,10);
		if(n<=0)
			continue;	
		int m = std::min(n,1024);
		for(int i=0;i<m;i++){
			if(ev[i].data.fd == listenfd)
				pthread_cond_signal(&accept_cond);
			else if(ev[i].events&(EPOLLIN | EPOLLERR)){
				pthread_mutex_lock(&clientset_mutex);
				if(clientset.find(ev[i].data.fd) != clientset.end()){
					pthread_mutex_unlock(&clientset_mutex);
					pthread_mutex_lock(&clientlist_mutex);
					clientlist.push_back(ev[i].data.fd);
					pthread_mutex_unlock(&clientlist_mutex);
				}else
					pthread_mutex_unlock(&clientset_mutex);	
			}
		}
	}
}



void *MyServer::worker_thread_proc(void *args){

	MyServer *pthis =((ARG*)args)->p;
	//StoC stoc;
	char rcv_buf[SND_BUF_SIZE];
	char snd_buf[RCV_BUF_SIZE];
	while(pthis->nstop){
		pthread_mutex_lock(&pthis->clientlist_mutex);
		if(!pthis->clientlist.empty()){
       			MYSQL *m_conn = mysql_init(NULL);
	        	MYSQL *m_sql = mysql_real_connect(m_conn,"localhost","root","a123456","PARKING",0,NULL,0);
		        if(!m_sql)
			        printf("connect mysql fail\n");
			
			memset(rcv_buf,0,sizeof rcv_buf);
			memset(snd_buf, 0,sizeof snd_buf);
			/*lock clientlist*/

			int fd = pthis->clientlist.front();
			pthis->clientlist.pop_front();
			pthread_mutex_unlock(&pthis->clientlist_mutex);
			
			/*recv CtoS msg*/
			int num_rcv=0;
			int allnum_rcv=0;
			while((num_rcv=recv(fd,&rcv_buf,sizeof rcv_buf,0))>=0){
        			allnum_rcv+=num_rcv;	
        			CtoS ctos;
        			memset(&ctos,0,sizeof ctos);
        			memcpy(&ctos,rcv_buf,sizeof rcv_buf);
        			//printf("fd:%d cmd:%dparkid:%dcarid:%s\n",fd,ctos.cmd,ctos.parkid,ctos.carid);
        			StoC stoc;
        			memset(&stoc,0,sizeof stoc);

				if(ctos.cmd==0){//==0
        				pthread_mutex_lock(&pthis->clientset_mutex);
        				pthis->clientset.erase(fd);
        				pthread_mutex_unlock(&pthis->clientset_mutex);
       					struct epoll_event e;
       					memset (&e,0,sizeof e);
       					e.data.fd = fd;
					e.events=EPOLLIN;
       					epoll_ctl(pthis->epollfd, EPOLL_CTL_DEL, fd,&e);
       					close(fd);
					printf("duanlian");
				}
        			/*putin*/
        			if(ctos.cmd==PUTIN){
        				/*putin(1,'S3','S4');*/
        				std::string S1 = "call putin(";
        				std::string S2 = std::to_string(ctos.parkid);
        				std::string S3 = ctos.carid;
        				std::string S4 = ctos.tele;
        				S1 = S1+S2+",'"+S3+"','"+S4+"');";
        
        				if(!mysql_query(m_sql,const_cast<char *>(S1.c_str()))){
        					stoc.cmd =PUTINSUCCESS;
        					memcpy(snd_buf,&stoc,sizeof stoc);
        					send(fd,&snd_buf,sizeof snd_buf,0);
        					//printf("putinsuccess\n");
        				}
        				else{
						//call putinupdate(1,'345');
						S1 = "call putinupdate(";
						S1 += std::to_string(ctos.parkid);
						S1+=",'";
						S1 += ctos.carid;
						S1 +="');";
						mysql_close(m_sql);
						MYSQL *m_conn = mysql_init(NULL);
						MYSQL *m_sql = mysql_real_connect(m_conn,"localhost","root","a123456","PARKING",0,NULL,0);

						if(!mysql_query(m_sql,const_cast<char *>(S1.c_str()))){
							std::cout << S1.c_str()<<std::endl;
							stoc.cmd =PUTINSUCCESS;
                                        	        memcpy(snd_buf,&stoc,sizeof stoc);
	   						send(fd,&snd_buf,sizeof snd_buf,0);
						}else{
        						stoc.cmd =PUTINFAIL;
        						memcpy(snd_buf,&stoc,sizeof snd_buf);
        						send(fd,&snd_buf,sizeof snd_buf,0);
        						//printf("putinfail\n");
						}
        				}
        
        				//sql_res = mysql_store_result(&m_sql);
        			}
        			if(ctos.cmd==PUTOUT){
        				//putout(1,'carid');
        				std::string S1 = "call putout(";
        				std::string S2 = std::to_string(ctos.parkid);
        				std::string S3 = ctos.carid;
        				S1 = S1+S2+",'"+S3+"');";
					stoc.cmd = PUTOUTFAIL;
        				if(!mysql_query(m_sql,const_cast<char *>(S1.c_str()))){
        					MYSQL_RES *result;
        					MYSQL_ROW row;
        					result = mysql_store_result(m_sql);
        					if(result!=NULL) {
        						//int num;
        						//num= mysql_num_fields(result);	
        						while((row = mysql_fetch_row(result))){
        							stoc.cmd = PUTOUTSUCCESS;
        							memcpy(stoc.carid, ctos.carid,sizeof ctos.carid);
        							if(row[0]!=NULL)
        								memcpy(stoc.ordertime,row[0],16);
        							if(row[1]!=NULL)
        								memcpy(stoc.intime,row[1],16);
        							if(row[2]!=NULL)
        								memcpy(stoc.outtime,row[2],16);
        							memcpy(snd_buf,&stoc,sizeof snd_buf); 
        						}
        						mysql_free_result(result);
        					}
        
        				}
        				else{	
        					memcpy(snd_buf,&stoc,sizeof snd_buf);
        				}
        				send(fd,&snd_buf,sizeof snd_buf,0);
        
        			}
        			if(ctos.cmd==ORDER){
        				//call porder(1,'carid','tele');
        				std::string S1 = "call porder(";
                                        std::string S2 = std::to_string(ctos.parkid);
                                        std::string S3 = ctos.carid;
                                        std::string S4 = ctos.tele;
                                        S1 = S1+S2+",'"+S3+"','"+S4+"');";
        				if(!mysql_query(m_sql,const_cast<char *>(S1.c_str()))){  
        					stoc.cmd = ORDERSUCCESS;
        				}else
        					stoc.cmd = ORDERFAIL;
        				memcpy(snd_buf,&stoc,sizeof stoc);
        				send(fd,&stoc,sizeof stoc,0);
        			}
        			if(ctos.cmd==RESET){
        				//call reset(1);
        				std::string S1 = "call reset(";
        				std::string S2 =std::to_string(ctos.parkid);
        				std::string S3 = ");";
        				S1 = S1+S2+S3;
        				if(!mysql_query(m_sql,const_cast<char *>(S1.c_str()))){
        					stoc.cmd = RESETSUCCESS;
        				}else
        					stoc.cmd = RESETFAIL;
        				memcpy(snd_buf,&stoc,sizeof stoc);
        				send(fd,&stoc,sizeof stoc,0);
        			}
        			if(ctos.cmd ==QUERY){
        				//SELECT * FROM PARKING-LOT-1;
        				std::string S1 ="SELECT * FROM parking_lot_";
        				std::string S2 = std::to_string(ctos.parkid);
        				S1 = S1+S2+" ;";
        				if(!mysql_query(m_sql,const_cast<char *>(S1.c_str()))){
        					MYSQL_RES *result;
        					MYSQL_ROW row;
        					result = mysql_store_result(m_sql);
        					while((row = mysql_fetch_row(result))){
        						memset(&stoc,0,sizeof stoc);
        						stoc.cmd=QUERYBACK;
        						if(row[0]!=NULL)//carid
        							memcpy(stoc.carid,row[0],sizeof stoc.carid );
        						if(row[1]!=NULL)//tele
        							memcpy(stoc.tele,row[1],sizeof stoc.tele);
        						if(row[2]!=NULL)//ordertime
        							memcpy(stoc.ordertime,row[2],sizeof stoc.ordertime);
        						if(row[3]!=NULL)//intime
        							memcpy(stoc.intime,row[3],sizeof stoc.intime);
        						if(row[4]!=NULL)//outtime
        							memcpy(stoc.outtime,row[4],sizeof stoc.outtime);
        						memset(snd_buf,0,sizeof snd_buf);
        						memcpy(snd_buf,&stoc,sizeof snd_buf);
							send(fd,&snd_buf,sizeof snd_buf,0);
							
        					}
						if(result!=NULL)
							mysql_free_result(result);
						stoc.cmd = QUERYOUT;
						memcpy(snd_buf,&stoc,sizeof stoc);
					        send(fd,&snd_buf,sizeof snd_buf,0);	
        				}
        				else{
        					stoc.cmd = QUERYFAIL;
        					send(fd,&snd_buf,sizeof snd_buf,0);
        				}
        			}
				if(ctos.cmd == registe){
					std::string S1 = "INSERT INTO user (user_id,user_password) VALUE ('";
					S1+=ctos.carid;
					S1+="','";
					S1+=ctos.tele;
					S1+="');";
					mysql_query(m_sql,const_cast<char *>(S1.c_str()));
				}
				if(ctos.cmd==login){
					std::string S1 = "SELECT user_password from user where user_id='";
					S1+= ctos.carid;
					S1+="';";
					mysql_query(m_sql,const_cast<char *>(S1.c_str()));
					MYSQL_RES *result;
					MYSQL_ROW row;			
					result = mysql_store_result(m_sql);
					stoc.cmd= loginfail;
					if(row = mysql_fetch_row(result)){
					if(row[0]!=NULL){
							std::string SA=ctos.tele;
							std::string SB=row[0];
							if(SA==SB){
								stoc.cmd=loginsuccess;
							}
						}
					}
					memcpy(snd_buf,&stoc,sizeof stoc);
					send(fd,&snd_buf,sizeof snd_buf,0);
				}
				if(ctos.cmd==user_query){
					std::string S1 ="select * from parking_lot_info;";
					mysql_query(m_sql,const_cast<char *>(S1.c_str()));
					MYSQL_RES *result;
					MYSQL_ROW row;
					result = mysql_store_result(m_sql);
					while((row = mysql_fetch_row(result))){
        					memset(&stoc,0,sizeof stoc);
        					stoc.cmd=QUERYBACK;
        					if(row[0]!=NULL)//parkid
        						memcpy(stoc.carid,row[0], strlen(row[0]) );
        					if(row[1]!=NULL)//tele
        						memcpy(stoc.tele,row[1],strlen(row[1]));
        					if(row[2]!=NULL)//allnum
        						memcpy(stoc.ordertime,row[2],strlen(row[2]));
        					if(row[3]!=NULL)//nownum
        						memcpy(stoc.intime,row[3],strlen(row[3]));
        					if(row[4]!=NULL)//addr
        						memcpy(stoc.outtime,row[4],strlen(row[4]));
        					memset(snd_buf,0,sizeof snd_buf);
        					memcpy(snd_buf,&stoc,sizeof snd_buf);
						send(fd,&snd_buf,sizeof snd_buf,0);	
        				}
				}
			}
        		mysql_close(m_sql);

		}
		else {
			pthread_mutex_unlock(&pthis->clientlist_mutex);
			sleep(0.1);
		}
	}
	return NULL;
}	
