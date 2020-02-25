#include "sql.h"
#include<iostream>
#include <stdio.h>
//Server::sql::sql(){}
//Server::sql::~sql(){}
MYSQL* 		Server::sql::m_sql;
MYSQL_RES* 	Server::sql::result;
MYSQL_ROW 	Server::sql::row;
int 		Server::sql::num;

 bool Server::sql::sql_conn(){
	MYSQL *m_conn =mysql_init(NULL);
	m_sql = mysql_real_connect(m_conn,"localhost","root","a123456","PARKING",0,NULL,0);
	if(!m_sql)
		printf("connect mysql fail\n");
	else
		return true;
	return false;
}

 bool Server::sql::query(std::string s){
	if(mysql_query(m_sql,const_cast<char *>(s.c_str()))){
		printf("query fail\n");
		return false;
	}
	return true;
}

void Server::sql::free(){

	mysql_free_result(result);
	return ;
}
void Server::sql::fetchinit(){

	result 	= mysql_store_result(m_sql);
	if(result!=NULL)
		num = mysql_num_fields(result);
	else {
		printf("fatch fail\n");
		return;
	}

		
}
