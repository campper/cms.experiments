/*
*Éñ¾­ÍøÂçËã·¨
*/
#ifndef _NEURO_H_
#define _NEURO_H_
#include <ctime>

class Layer
{
protected:
	int inputsCounts_ = 0;
	int neronsCounts_ = 0;


};

class Neuron
{
protected:
	int inputsCount_ = 0;
	double *weights_;
	double output_ = 0;

};

class Neuro
{
public:
	Neuro();
	~Neuro();
};



#endif // _NEURO_H_

