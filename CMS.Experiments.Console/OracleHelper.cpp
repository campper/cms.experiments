#include "DbHelper.h"
#include "OracleHelper.h"
using namespace std;

static const char* USER = "tjcms";
static const char* PASSWORD = "2491";
static const char* SERVERNAME = "orcl";

#define THREADSTACKSIZE (2097152)	//(2*1024*1024)
OracleAgent *OracleAgent::m_pAgent_ = NULL;
OracleAgent::OracleAgent()
{
}


OracleAgent::~OracleAgent()
{
}

OracleAgent *OracleAgent::Instance()
{
	if (NULL == m_pAgent_)
	{
		if (NULL != (m_pAgent_ = new OracleAgent()))
		{
			//if (0 != m_pHelper_->Initial())
			//{
			//	delete m_pHelper_;
			//	m_pHelper_ = NULL;
			//}
		}
	}
	return m_pAgent_;
}

void OracleAgent::Destroy()
{
	if (NULL != m_pAgent_)
	{
		m_pAgent_->Completed();
		delete m_pAgent_;
		m_pAgent_ = NULL;
	}
}

int OracleAgent::Initial(const char *user, const char *password, const char *servername, int n_connnum)
{
	try
	{
		if (NULL == (m_pEnv_ = Environment::createEnvironment()))
		{
			return -1;
		}
	}
	catch (...)
	{
		return -1;
	}

	try
	{
		if (NULL == (m_pConnPool_ = m_pEnv_->createStatelessConnectionPool(user, password, servername, n_connnum, n_connnum, StatelessConnectionPool::HETEROGENEOUS)))
		{
			Environment::terminateEnvironment(m_pEnv_);
			return -1;
		}
		m_pConnPool_->setBusyOption(StatelessConnectionPool::WAIT);
	}
	catch (...)
	{
		Environment::terminateEnvironment(m_pEnv_);
		return -1;
	}
	return 0;
}

Connection *OracleAgent::GetOneConnection()
{
	return this->m_pConnPool_->getConnection();
}

void OracleAgent::ReleaseOneConnection(Connection * conn)
{
	m_pConnPool_->releaseConnection(conn);
}

void OracleAgent::Completed()
{
	this->m_pEnv_->terminateStatelessConnectionPool(m_pConnPool_);
	Environment::terminateEnvironment(this->m_pEnv_);
}

OracleHelper::OracleHelper()
{
	const char* user = USER;
	const char* passwd = PASSWORD;
	const char* servername = SERVERNAME;
	int n_connNum = 10;

	this->m_conn_ = NULL;
	this->m_Stmt_ = NULL;
	this->m_isInitOK = false;

	if (-1 == OracleAgent::Instance()->Initial(user, passwd, servername, n_connNum))
	{
		this->m_isInitOK = false;
	}
	else {
		this->m_isInitOK = true;
	}
}

OracleHelper::OracleHelper(char * user, char * passwd, char * servername, int n_connNum)
{
	this->m_conn_ = NULL;
	this->m_Stmt_ = NULL;
	this->m_isInitOK = false;

	if (-1 == OracleAgent::Instance()->Initial(user, passwd, servername, n_connNum))
	{
		this->m_isInitOK = false;
	}
	else {
		this->m_isInitOK = true;
	}
}

OracleHelper::~OracleHelper()
{
	OracleAgent::Instance()->Destroy();
}

//void * OracleHelper::operator new(size_t size)
//{
//	return ::operator new(size);
//}

unsigned int OracleHelper::Update(const char * sql)
{
	this->m_conn_ = OracleAgent::Instance()->GetOneConnection();
	try
	{
		this->m_Stmt_ = this->m_conn_->createStatement(sql);
	}
	catch (...) {
		OracleAgent::Instance()->ReleaseOneConnection(this->m_conn_);
		return 0;
	}

	unsigned int nRow;
	try
	{
		nRow = this->m_Stmt_->executeUpdate();
		this->m_conn_->commit();
	}
	catch (...) {
		nRow = 0;
	}

	this->m_conn_->terminateStatement(this->m_Stmt_);
	OracleAgent::Instance()->ReleaseOneConnection(this->m_conn_);
	return nRow;
}

ResultSet *OracleHelper::Select(const char * sql)
{
	this->m_conn_ = OracleAgent::Instance()->GetOneConnection();
	try
	{
		this->m_Stmt_ = this->m_conn_->createStatement(sql);
	}
	catch (...) {
		OracleAgent::Instance()->ReleaseOneConnection(this->m_conn_);
		return NULL;
	}
	
	ResultSet *rset;

	try
	{
		rset = this->m_Stmt_->executeQuery();
	}
	catch (...) {
		this->m_conn_->terminateStatement(this->m_Stmt_);
		OracleAgent::Instance()->ReleaseOneConnection(this->m_conn_);
		rset = NULL;
	}
	return rset;
}

void OracleHelper::ReleaseSet(ResultSet *rset)
{
	this->m_Stmt_->closeResultSet(rset);
	this->m_conn_->terminateStatement(this->m_Stmt_);
	OracleAgent::Instance()->ReleaseOneConnection(this->m_conn_);
}

int OracleHelper::Procedure(char *name, char *fmt, ...)
{
	this->m_conn_ = OracleAgent::Instance()->GetOneConnection();

	char sql[256];
	sprintf(sql, "BEGIN %s; END;", name);

	try
	{
		this->m_Stmt_ = this->m_conn_->createStatement(sql);
	}
	catch (...) {
		OracleAgent::Instance()->ReleaseOneConnection(this->m_conn_);
		return -1;
	}

	int nRet;

	va_list ap;
	va_start(ap, fmt);
	char *ptr = fmt;
	char c;
	int i = 1;

	bool out = false;
	char *pos;
	int num;
	try
	{
		do
		{
			c = *ptr;
			if (c == '%')
			{
				switch (*++ptr)
				{
				case 'd':
					this->m_Stmt_->setNumber(i, va_arg(ap, int));break;
				case 's':
					this->m_Stmt_->setString(i, va_arg(ap, char *));break;
				case 'D':
					this->m_Stmt_->registerOutParam(i, OCCIINT);break;
				case 'S':
					this->m_Stmt_->registerOutParam(i, OCCISTRING, 256);break;
				default:
					break;
				}
				++i;
				++ptr;
			}
			else if (c == ':')
			{
				out = true;
				pos = ptr;
				num = i;
				++ptr;
			}
			else
			{
				++ptr;
			}
		} while (*ptr != '\0');

		this->m_Stmt_->execute();

		if (out)
		{
			ptr = pos;
			i = num;
			do
			{
				c = *ptr;
				if (c == '%')
				{
					switch (*++ptr)
					{
					case 'D':
						*(va_arg(ap, int *)) = this->m_Stmt_->getInt(i);break;
					case 'S':
						strcpy(va_arg(ap, char *), this->m_Stmt_->getString(i).c_str());break;
					default:
						break;
					}
					++i;
				}
				++ptr;
			} while (*ptr != '\0');
		}

		va_end(ap);

		nRet = 0;
	}
	catch (...)
	{
		nRet = -1;
	}

	this->m_conn_->terminateStatement(this->m_Stmt_);
	OracleAgent::Instance()->ReleaseOneConnection(this->m_conn_);

	return nRet;
}

int OracleHelper::ExcuteQuery(char * sql)
{
	ResultSet *rset;
	int rt = -1;
	try {
		char* msg = "";
		rset = this->Select(sql);
		while (rset->next())
		{
			rt++;
		}
		return rt;
	}
	catch (int) {
		return -1;
	}
}

