#ifndef _DEBUG_NEW_H_
#define _DEBUG_NEW_H_

#ifndef NDEBUG
#include "Tracer.h"
#define new new(__FILE__,__LINE__)
//#define delete delete(__FILE__,__LINE__)	//不支持delete接受参数
#endif //NDEBUG

#endif //_DEBUG_NEW_H_