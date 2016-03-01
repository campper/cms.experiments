#include "DbHelper.h"
#include "MysqlHelper.h"

//MysqlAgent *MysqlAgent::m_pAgent_ = NULL;	//代理单例模式
int MysqlHelper::ExcuteQuery(char * sql)
{
	try {
		char* msg = "";
		int rt = this->SelectData(sql, msg, result_);
		//cout << result_ << endl;
		return rt;
	}
	catch (int) {
		return -1;
	}
}
//初始化数据库
void MysqlHelper::InitDb()
{
	dbuser_ = "root";
	dbpassword_ = "";
	dbip_ = "localhost";
	dbname_ = "cms";
	tablename_ = "tr_oil_olm";
	charset_ = "set names \'gb2312\'";
	conn_ = mysql_init((MYSQL*)0);
}

void MysqlHelper::InitDb(char *host, char * port, char * Db, char * user, char* passwd, char * charset)
{
	this->dbuser_ = user;
	this->dbip_ = host;
	this->dbname_ = Db;
	this->dbpassword_ = passwd;
	this->charset_ = charset;
	this->port_ = port;
}

int MysqlHelper::dbConn(char *host, char * port, char * Db, char * user, char* passwd, char * charset, char * Msg)
{
	InitDb(host,port,Db,user,passwd,charset);
	if (mysql_init(conn_) == NULL)
	{
		Msg = "inital mysql handle error";
		return 1;
	}

	if (mysql_real_connect(conn_, host, user, passwd, Db, 0, NULL, 0) == NULL)
	{
		Msg = "Failed to connect to database: Error";
		return 1;
	}

	if (mysql_set_character_set(conn_, charset) != 0)
	{
		Msg = "mysql_set_character_set Error";
		return 1;
	}
	return 0;
}

int MysqlHelper::dbConn()
{
	try
	{
		InitDb();
		if (mysql_init(conn_) == NULL)
		{
			this->msg_ = "inital mysql handle error";
			return 1;
		}

		if (mysql_real_connect(conn_, dbip_,dbuser_,dbpassword_,dbname_, 0, NULL, 0) == NULL)
		{
			msg_ = "Failed to connect to database: Error";
			return 1;
		}
		else
		{
			int rt;
			if (!mysql_select_db(conn_, dbname_))
			{
				cout << "mysqlhelper Select " << dbname_ << " successfully the database!" << endl;
				conn_->reconnect = 1;

				//rt = static_cast<size_t>(mysql_real_query(conn_, charset_, strlen(charset_)));
				rt = (unsigned long)mysql_real_query(conn_, charset_, strlen(charset_));
				if (rt)
				{
					cout << "Error making charset:" << mysql_error(conn_) << endl;
				}
				else
				{
					cout << "charset " << charset_ << " succeed!" << endl;
				}
			}
		}

		if (mysql_set_character_set(conn_, charset_) != 0)
		{
			msg_ = "mysql_set_character_set Error";
			return 1;
		}
		return 0;
	}
	catch(exception* ex) {
		cout << ex->what() << endl;
		return 1;
	}
}

int MysqlHelper::SelectData(char * SQL, int Cnum, char * Msg, std::string& result)
{
	MYSQL_ROW m_row;
	MYSQL_RES *m_res;
	char sql[2048];
	sprintf(sql, SQL);
	int rnum = 0;
	//char rg = 0x06;//行隔开
	//char cg = { 0x05 };//
	char rg = '\n';
	char cg = '\t';

	try{
		if (mysql_query(conn_, sql) != 0)
		{
			Msg = "select ps_info Error";
			return -1;
		}
		m_res = mysql_store_result(conn_);

		if (m_res == NULL)
		{
			Msg = "select username Error";
			return -1;
		}
		string str("");
		while (m_row = mysql_fetch_row(m_res))
		{
			for (int i = 0; i < Cnum; i++)
			{
				str += m_row[i];
				str += cg;
			}
			str += rg;
			rnum++;
		}
		result = str;
		mysql_free_result(m_res);
		return rnum;
	}
	catch (int)
	{
		return -1;
	}
}

int MysqlHelper::SelectData(char * SQL, char * Msg, std::string& result)
{
	MYSQL_ROW m_row;
	MYSQL_RES *m_res;
	char sql[2048];
	sprintf(sql, SQL);
	int rnum = 0;
	//char rg = 0x06;//行隔开
	//char cg = { 0x05 };//
	char rg = '\n';
	char cg = '\t';
	unsigned int nums_field = 0;

	try{
		if (mysql_query(conn_, sql) != 0)
		{
			Msg = "select ps_info Error";
			return -1;
		}
		m_res = mysql_store_result(conn_);

		if (m_res == NULL)
		{
			Msg = "select username Error";
			return -1;
		}
		string str("");
		nums_field = mysql_num_fields(m_res);
		while (m_row = mysql_fetch_row(m_res))
		{
			for (unsigned int i = 0; i < nums_field; i++)
			{
				str += m_row[i];
				str += cg;
			}
			str += rg;
			rnum++;
		}
		result = str;
		mysql_free_result(m_res);
		return rnum;
	}
	catch (int)
	{
		return -1;
	}
}

void MysqlHelper::Close()
{
}

MYSQL_RES* MysqlHelper::getResult() const
{
	return dataset_;
}

void MysqlHelper::setResult(MYSQL_RES* dataset)
{
	this->dataset_ = dataset;
}

MysqlHelper::MysqlHelper()
{
	cout << "mysqlhelper ..." << endl;
}

MysqlHelper::~MysqlHelper()
{
	if (conn_ /*!= NULL*/)
	{
		cout << "~mysqlhelper::close" << endl;
		mysql_close(conn_);
	}

	cout << "~mysqlhelper ..." << endl;
}

//MysqlAgent * MysqlAgent::Instance()
//{
//	if (NULL == m_pAgent_)
//	{
//		if (NULL != (m_pAgent_ = new MysqlAgent()))
//		{
//			//if (0 != m_pHelper_->Initial())
//			//{
//			//	delete m_pHelper_;
//			//	m_pHelper_ = NULL;
//			//}
//		}
//	}
//	return m_pAgent_;
//}
