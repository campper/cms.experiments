#include "CMSException.h"

const char* CMSException::what() throw()
{
	return message_.c_str();
}

const char* CMSException::StackTrace() const throw()
{
	return stackTrace_.c_str();
}

void CMSException::FillStackTrace()
{
	//适应于Linux下的栈回溯 Windows下没有::backtrace、::backtrace_symbols函数
	//const int len = 200;
	//void* buffer[len];
	//int nptrs = ::backtrace(buffer, len);
	//char** strings = ::backtrace_symbols(buffer, nptrs);
	//if (strings)
	//{
	//	for (int i = 0; i < nptrs; ++i)
	//	{
	//		stackTrace_.append(strings[i]);
	//		stackTrace_.push_back('\n');
	//	}
	//	free(strings);
	//}

}