#ifndef _SCANNER_H_
#define _SCANNER_H_
#include<string>

enum EToken
{
	TOKEN_END,			//结束标识符
	TOKEN_ERROR,		//错误标识符
	TOKEN_NUMBER,		//数字标识符
	TOKEN_PLUS,			//加法标识符
	TOKEN_MINUS,		//减法标识符
	TOKEN_MULTIPLY,		//乘法标识符
	TOKEN_DIVIDE,		//除法标识符
	TOKEN_LPARENTHESIS,	//左括号标识符
	TOKEN_RPARENTHESIS,	//右括号标识符
	TOKEN_IDENTIFIER,	//变量标识符
	TOKEN_ASSIGN		//赋值标识符
};
class Scanner
{
public:
	explicit Scanner(const std::string& buf);
	void Accept();
	double Number() const;
	std::string GetSymbol() const;
	EToken Token() const;
	bool IsEmpty() const;
	bool IsDone() const;
private:
	void SkipWhite();
	std::string buf_;
	unsigned int curPos_;
	EToken token_;
	double number_;
	std::string symbol_;
	bool isEmpty_;

};


#endif //_SCANNER_H_
