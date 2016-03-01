/**
 * alxor
 */
#ifndef _KRESULT_H_
#define _KRESULT_H_

class KResult
{
public:
	KResult();
	~KResult();
public:
	int index_;
	double* feature_;
	int patternnum_;
};

#endif // _KRESULT_H_

