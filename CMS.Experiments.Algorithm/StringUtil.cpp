#include <algorithm>
#include <iostream>
#include <cctype>
#include "StringUtil.h"
using namespace std;

void StringUtil::LTrim(string& s)
{
	string drop = " \t";
	s.erase(0, s.find_first_not_of(drop));
}

void StringUtil::RTrim(string& s)
{
	string drop = " \t";
	//s.erase(s.find_last_not_of(drop)+1,string::npos);
	s.erase(s.find_last_not_of(drop) + 1);
}

void StringUtil::Trim(string& s)
{
	LTrim(s);
	RTrim(s);
}

void StringUtil::Lower(string & s)
{
	transform(s.begin(), s.end(),s.begin(), ::tolower);
}

void StringUtil::Upper(string & s)
{
	transform(s.begin(), s.end(), s.begin(), ::toupper);
}