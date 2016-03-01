#ifndef _ORACLE_HELPER_H_
#define _ORACLE_HELPER_H_

#include <oci.h>
#include <occi.h>
using namespace oracle::occi;

class DbHelper;
class OracleAgent
{
private:
	OracleAgent();
	~OracleAgent();
public:
	static OracleAgent *Instance();	//µ¥ÀýÄ£Ê½
	static void Destroy();
	int Initial(const char*, const char*, const char*, int n_connnum = 10);
public:
	Connection *GetOneConnection();
	void ReleaseOneConnection(Connection *conn);
private:
	void Completed();
private:
	static OracleAgent *m_pAgent_;
	Environment *m_pEnv_;
	StatelessConnectionPool *m_pConnPool_;
};

class OracleHelper : public DbHelper
{
public:
	OracleHelper();
	OracleHelper(char* user, char* passwd, char* servername, int n_connum = 10);
	~OracleHelper();
public:
	//void *operator new(size_t size);
	unsigned int Update(const char*);
	ResultSet *Select(const char* sql);
	void ReleaseSet(ResultSet *);
	int Procedure(char*, char*, ...);
	int ExcuteQuery(char* sql);
	operator bool()
	{
		return m_isInitOK;
	}

private:
	Connection *m_conn_;
	Statement *m_Stmt_;
	bool m_isInitOK;

	Environment *m_pEnv_;
	StatelessConnectionPool *m_pConnPool_;

};

#endif //_ORACLE_HELPER_H_
