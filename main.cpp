#include"MyServer.h"

MyServer myserver;
int main(int argc,char* argv[]){
	myserver.link_startou("0.0.0.0",12345);
	myserver.main_loop();
	return 0;
}
