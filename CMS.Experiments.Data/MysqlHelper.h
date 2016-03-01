#ifndef _MYSQLHELPER_H_
#define _MYSQLHELPER_H_

#include <winsock.h> 
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "mysql.h"

using namespace std;

//class MysqlAgent
//{
//private:
//	MysqlAgent();
//	~MysqlAgent();
//public:
//	static MysqlAgent *Instance();	//单例模式
//	//static void Destroy();
//private:
//	static MysqlAgent *m_pAgent_;
//};
class DbHelper;
class MysqlHelper : public DbHelper
{
public:
	MysqlHelper();
	~MysqlHelper();
public:
	/*
	主要的功能：初始化数据库; 连接数据库; 设置字符集
	入口参数：
	host ：MYSQL服务器IP
	port:数据库端口
	Db：数据库名称
	user：数据库用户
	passwd：数据库用户的密码
	charset：希望使用的字符集
	Msg:返回的消息，包括错误消息

	出口参数：
	int ：0表示成功；1表示失败
	*/
	int dbConn(char *host, char * port, char * Db, char * user, char* passwd, char * charset, char * Msg);
	int dbConn();
	/*
	主要的功能：查询数据;
	入口参数：
	SQL：查询的SQL语句
	Cnum:查询的列数
	Msg:返回的消息，包括错误消息
	std::string::result 赋值字符串
	MYSQL_RES& result 赋值结果集

	出口参数：
	string 准备放置返回的数据，多条记录则用0x06隔开,多个栏位用0x05隔开
	如果 返回的长度＝ 0，责表示舞结果
	*/
	int SelectData(char * SQL, int Cnum, char * Msg, std::string& result);
	int SelectData(char * SQL, char * Msg, std::string&);
	/*
	主要功能：
	关闭数据库连接
	*/
	void Close();

	/*
	主要功能：
	获得MYSQL_RES类型的数据结果集
	*/
	MYSQL_RES* getResult() const;

	/*
	主要功能：
	设置MYSQL_RES类型的数据结果集
	*/
	void setResult(MYSQL_RES* result);

	/*
	主要功能：
	执行MYSQL 语句
	入口参数：
	sql: 数据库语句

	出口参数：
	int 执行成功的条数
	*/
	int ExcuteQuery(char* sql);
private:
	void InitDb();
	void InitDb(char *host, char * port, char * Db, char * user, char* passwd, char * charset);
private:
	char *dbuser_;			// "root";
	char *dbpassword_;		// "voltampcms100";
	char *dbip_;				// "localhost";
	char *dbname_;			// "cms";
	char *tablename_;		// "tr_oil_olm";
	char *charset_;			// NULL;
	char *msg_;
	char *port_;
	MYSQL *conn_;
	MYSQL_RES *dataset_;		//MySQL数据集
	std::string result_;
};

#endif // _MYSQLHELPER_H_
