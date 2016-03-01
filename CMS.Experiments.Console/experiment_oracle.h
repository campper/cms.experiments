#ifndef _EXPERIMENT_OPTICS_H_
#define _EXPERIMENT_OPTICS_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>

#include <ctime>
#include <cctype>
#include <cassert>

#include <boost\shared_array.hpp>
#include <boost\shared_ptr.hpp>
#include <boost\ptr_container\ptr_vector.hpp>

#include <occi.h>
using namespace oracle;
using namespace oracle::occi;

class DataPoint;
class OracleHelper;

class experiment_oracle
{
public:
	experiment_oracle();
	~experiment_oracle();
public:
	void conn();
	void savedata(char* sql);
	//void saveOPTICSResult(std::vector<DataPoint*>,const ofstream&);
	void savedata(char* sql, int flag);	//保存数据到 vector<DataPoint> 
	//std::vector<DataPoint*> getDPdata() const;
	std::vector<DataPoint*> getDPdata() const;
	std::vector<double> getdata() const;
	int getFieldNum() const;
	std::string getResult() const;
	DataPoint* getDataPointPtr() const;
private:
	bool AddKeyMap(std::vector<std::string>& line, std::map<int, std::string> map);
	int getColumnNum(char*);
public:
	std::map<int, std::string> map_;

private:
	
	DataPoint* tmpcell_;
	std::string result_;
	std::vector<double> data_;
	//std::vector<DataPoint*> dpdata_;
	std::vector<DataPoint*> dpdata_;
	int fieldnum_;
private:
	Environment* env_;
	Connection *conn_;
	Statement *stmt_;
	ResultSet *rset_;
	OracleHelper *oracle_helper_;
};



#endif // _EXPERIMENT_OPTICS_H_