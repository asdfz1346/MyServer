#include "sql.h"
#include<iostream>
int main(){
	if(!Server::sql::sql_conn())
	{
		return 0;	
	}
//	std::string S1="SELECT * FROM parking_lot_1;";
//	std::string S1="call putin(1,'======','========');";
	std::string S2="call putout(1,'1234567');";
//	Server::sql::query(S1);
	Server::sql::query(S2);
//	Server::sql::fetchinit();
/*	while((Server::sql::row = mysql_fetch_row(Server::sql::result))){
		for(int i=0;i<Server::sql::num;i++){
			if(Server::sql::row[i]==NULL){
				std::cout << '\t';
			}else
				std::cout << Server::sql::row[i]<<'\t';

		}
		std::cout << std::endl;
	}	
*/		

	return 0;

}
