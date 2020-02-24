main:
	g++ -g -Wall main.cpp MyServer.cpp -o main -lpthread -lmysqlclient  -L/usr/lib64/mysql
clean: 
	rm -rf main
