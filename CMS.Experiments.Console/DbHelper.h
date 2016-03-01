#ifndef _SQL_HELPER_H_
#define _SQL_HELPER_H_

#include <iostream>
#include <fstream>
using namespace std;

class NoncopyDbHelper
{
protected:
	NoncopyDbHelper() {};
	~NoncopyDbHelper() {};
private:
	NoncopyDbHelper(const NoncopyDbHelper&);
	const NoncopyDbHelper operator=(const NoncopyDbHelper&);
};

class DbHelper : private NoncopyDbHelper {
protected:
	virtual ~DbHelper() {};
public:
	virtual int ExcuteQuery(char* sql) = 0;
};

#endif 
