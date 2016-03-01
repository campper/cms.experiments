/*
*内存泄露跟踪器
*/
#include <iostream>
#include "Tracer.h"

#ifndef NDEBUG
bool Tracer::Ready = false;
Tracer::Tracer() : lockCount_(0)
{
	Ready = true;
}

Tracer::~Tracer()
{
	Ready = false;
	Dump();
}
void Tracer::Add(void* p, const char* file, long line)
{
	if (lockCount_ > 0)
		return;
	Tracer::Lock lock(*this);
	mapEntry_[p] = Entry(file, line);
}

void Tracer::Remove(void* p)
{
	if (lockCount_ > 0)
		return;
	
	Tracer::Lock lock(*this);
	std::map<void*, Entry>::iterator it;
	it = mapEntry_.find(p);
	if (it != mapEntry_.end())
	{
		mapEntry_.erase(it);
	}
}

void Tracer:: Dump()										//哪里发生了内存泄露
{
	if (mapEntry_.size() > 0)
	{
		std::cout << "*** Memory Leak(s)" << std::endl;
		int tmp = 0;
		std::map<void*, Entry>::iterator it;
		for (it = mapEntry_.begin(); it != mapEntry_.end(); ++it)
		{
			const char* file = it->second.File();
			long line = it->second.Line();
			int addr = reinterpret_cast<int>(it->first);			//指针强制转换
			std::cout << tmp << ": " << "0x" << std::hex << addr << ": "
				<< file << ", line: " << std::dec << line << std::endl;
			tmp++;
		}
		std::cout << std::endl;
	}
	
}

Tracer NewTrace;

void* operator new(size_t size, const char* file, long line)
{
	void* p = malloc(size);
	if (Tracer::Ready)
	{
		NewTrace.Add(p, file, line);
	}
	return p;
}

void* operator new[](size_t size)
{
	void* p = malloc(size);
	if (Tracer::Ready)
	{
		NewTrace.Add(p, "??", 0);
	}
	return p;
}

void* operator new(size_t size)
{
	void* p = malloc(size);
	if (Tracer::Ready)
	{
		NewTrace.Add(p, "??", 0);
	}
	return p;
}

void* operator new[](size_t size, const char* file, long line)
{
	void* p = malloc(size);
	if (Tracer::Ready)
	{
		NewTrace.Add(p, file, line);
	}
	return p;
}

void operator delete(void* p, const char*, long)
{
	if (Tracer::Ready)
	{
		NewTrace.Remove(p);
	}
	free(p);
}

void operator delete(void* p)
{
	if (Tracer::Ready)
	{
		NewTrace.Remove(p);
	}
	free(p);
}

void operator delete[](void* p, const char*, long)
{
	if (Tracer::Ready)
	{
		NewTrace.Remove(p);
	}
	free(p);
}

void operator delete[](void* p)
{
	if (Tracer::Ready)
	{
		NewTrace.Remove(p);
	}
	free(p);
}

#endif //NDEBUG