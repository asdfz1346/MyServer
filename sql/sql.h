#ifndef __SQL_H
#define __SQL_H
#include<mysql/mysql.h>
#include<string>
#include<string.h>

namespace Server{
	class sql{
		public:
			static bool sql_conn();
			static bool query(std::string s);
			static void free();
			static void fetchinit();
			static MYSQL_ROW row;
			static int num;
			static MYSQL_RES *result;
			//sql();
			//~sql();


		private:
			static MYSQL *m_sql;

	};
};

#endif
