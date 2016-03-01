#include "PautaRule.h"


PautaRule::PautaRule()
{
}


PautaRule::~PautaRule()
{
}

bool PautaRule::IsAbnormal()
{
	return false;
}

void PautaRule::InitData()
{

}


double PautaRule::Average()
{
	double sum = 0.0;
	for (int i = 0; i < len_; i++)
	{
		sum += list_[i];
	}
	
	return sum / len_;
}