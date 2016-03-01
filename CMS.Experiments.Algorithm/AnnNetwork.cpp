#include "AnnNetwork.h"
#include "DataPoint.h"

AnnNetwork::AnnNetwork()
{
}


AnnNetwork::~AnnNetwork()
{
}

void AnnNetwork::InitData(std::vector<double>& data, int N)
{
}

void AnnNetwork::InitW(std::vector<double>& w)
{
	std::vector<double>::iterator it;
	for (it = w.begin(); it != w.end(); it++)
		*it = (2.0*(double)rand() / RAND_MAX) - 1;
}

void AnnNetwork::InitW(double w[], int len)
{
	for (int i = 0; i < len; i++)
		w[i] = (2.0*(double)rand() / RAND_MAX) - 1;
}

void AnnNetwork::train()
{
}


void AnnNetwork::errorfunc()
{

}

//
//给各个连接权值分别初始化，赋值一个区间(-1,1)内的随机数
//
void AnnNetwork::InitW()
{
	this->InitW(w_input_hiden_);
	this->InitW(w_hiden_output_);
}

//
//随机选取第k个输入样本及对应的期望输出
//
void AnnNetwork::sampleExpect(std::vector<DataPoint*> inputs,int k)
{
	double _mExpect;
	DataPoint* dpPoint = inputs[k];
	if (inputs[k] != NULL)
	{
		double _mSum = 0;		
		for (int i = 0; i < dpPoint->size(); i++)
			_mSum += dpPoint->GetDimension()[i];
		_mExpect = (double)_mSum / dpPoint->size();
	}

	delete dpPoint;

	this->expect_[inputs[k]] = _mExpect;
}
//
//计算隐含层各神经元的输入和输出
//
void AnnNetwork::calcHidenLayer(std::vector<DataPoint*> inputs,std::map<DataPoint*,double>& outputs,int k)
{
	//std::vector<DataPoint*>::iterator dpPoint;
	for (int i = 0; i < inputs[k]->size(); i++)
	{
		DataPoint *dpPoint = inputs[k];
		this->hiden_outputv_[k]->GetDimension()[i] = hiddenFunc(dpPoint, this->rate_hiden_[k], this->rate_hiden_[k]);
	}
		
		
}

double AnnNetwork::hiddenFunc(DataPoint * dp, double weight, double threshold)
{
	double _hSum = 0.0;
	for (int i = 0; i < dp->size(); i++)
		_hSum += (double)((weight*dp->GetDimension()[i]) - threshold);
	return _hSum;
}
