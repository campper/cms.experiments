#ifndef _SQLSERVER_HELPER_H_
#define _SQLSERVER_HELPER_H_
class DbHelper;
class SqlServerHelper : public DbHelper
{
public:
	SqlServerHelper();
	~SqlServerHelper();
	int ExcuteQuery(char* sql);
};

#endif //_SQLSERVER_HELPER_H_

