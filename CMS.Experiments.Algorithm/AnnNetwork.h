/**
** 神经网络算法 BP
** 算法原理:http://www.cnblogs.com/wentingtu/archive/2012/06/05/2536425.html
** 代码实现参考: http://blog.csdn.net/luxiaoxun/article/details/7649945
** BP神经网络参考：http://blog.csdn.net/han6771306/article/details/25476301
**/
#ifndef _ANN_NETWORK_H_
#define _ANN_NETWORK_H_

#include <vector>
#include <map>
#include <cmath>

#define LEARN_RATE 0.9
#define Pi 3.1415926

class DataPoint;

class AnnNetwork
{
public:
	AnnNetwork();
	~AnnNetwork();
	inline int status() const;
	void InitData(std::vector<double>& data, int N);	//初始化数据
	void InitData(std::vector<DataPoint*> data);
	void InitW(std::vector<double>& w);					//初始化权值
	void InitW(double w[], int len);					//初始化权值 数组形式
	int train(const std::vector<DataPoint*> input);		//训练函数
	void train();
	inline int getLearnTimes() const;

	inline void setLearnTimes(int times);

private:
	std::vector<DataPoint*> x_inputv_;					//输入向量
	std::vector<DataPoint*> hiden_inputv_;				//隐含层输入向量
	std::vector<DataPoint*> hiden_outputv_;				//隐含层输出向量
	std::vector<DataPoint*> y_inputv_;					//输出层输入向量
	std::vector<DataPoint*> y_outputv_;					//输出层输出向量
	std::map<DataPoint*, double> expect_;					//期望向量
	int learning_times_;
private:
	size_t size_;										//样本个数
	std::vector<double> w_input_hiden_;					//输入层与隐含层的连接权值
	std::vector<double> w_hiden_output_;				//隐含层与输出层的连接权值
	std::vector<double> rate_hiden_;					//隐含层各神经元阈值
	std::vector<double> rate_output_;					//输出层各神经元阈值

	double _err;										//误差限
	int samples_num_;
	int m_status_;

private:
	void normalize(DataPoint **dp, int size);			//归一化处理  
	void unnormalize(DataPoint **dp, int size);			//反归一化处理
	void activatedfunc();								//激活函数
	void errorfunc();									//误差函数
	void InitW();
	void sampleExpect(std::vector<DataPoint*> inputs, int k);
	void calcHidenLayer(std::vector<DataPoint*> inputs, std::map<DataPoint*, double>& outputs, int k);
	double hiddenFunc(DataPoint* dp, double weight, double threshold);

};

inline int AnnNetwork::getLearnTimes() const
{
	return this->learning_times_;
}

inline void AnnNetwork::setLearnTimes(int times)
{
	this->learning_times_ = times;
}

inline int AnnNetwork::status() const
{
	return this->m_status_;
}
#endif //_ANN_NETWORK_H_
