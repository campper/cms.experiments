// K-Means.cpp : 定义控制台应用程序的入口点。
#include <iostream>
#include <fstream>
#include <strstream>
#include <map>
#include <string>
#include <cmath>
#include "K-Means.h"
#include "distance.h"
using namespace std;

const int MAX=10000;
//vector <double> line;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction/构造函数
//////////////////////////////////////////////////////////////////////

CCluster::CCluster()
{
	centernum=0;
	patternnum=0;
	m_pattern=NULL;
	m_center=NULL;
}

//析构函数
CCluster::~CCluster()
{
}

/***************************************************************
*函数名称		GetDistance(CCluster::Pattern pattern,  CCluster::Center center, const int distype)
*参数			CCluster::Pattern pattern  样品
*				CCluster::Center center    中心
*返回值			double
*函数功能		计算样品和聚类中心间的距离
************************************************************/
double CCluster::GetDistance(CCluster::Pattern  pattern, CCluster::Center center)
{
	//int i;
	double   result=0;

	if (1)    //应该考虑聚类中心包含的模式数目，如果为0应该。。。。。。。。。。。
	{
		//for (i=0; i<N; i++)
		//	result+=(pattern.feature[i]-center.feature[i])*(pattern.feature[i]-center.feature[i]);
		//return sqrt(result);
		result = distance::getEuclidean(pattern.feature, center.feature, N);
		return result;
	}
	else
	{
	}
}

/*********************************************************
*函数名称		CalCenter(CCluster::Center *pcenter)
*参数			CCluster::Center *pcenter
*返回值		    void
*函数功能		计算中心pcenter的特征值（本类所有样品的均值），及样品个数
************************************************************/	
void CCluster::CalCenter(Center* pcenter)
{
	double *temp;//临时存储中心的特征值
	temp=new double[N];
	int a=0;//临时存储中心的特征值

	for (int i=0;i<N;i++)//中心清空
		temp[i]=0;
	
	for (int i = 0; i < patternnum; i++)
	{
		if (m_pattern[i].category == pcenter->index)//累加中心pcenter的所有样品个数和特征值
		{
			a++;
			for (int j = 0; j < N; j++)
				temp[j] += m_pattern[i].feature[j];
		}
	}

	pcenter->patternnum=a;
	
	for (int i = 0; i < N; i++)
	{
		if (a != 0)   //如果该类包含样本数不为0
		{
			pcenter->feature[i] = (double)(temp[i] / (double)a);//特征值取累加和的均值
		}
		else
		{
			pcenter->feature[i] = temp[i];     //此处考虑改进。。。。。。。。。。
		}
	}
}

/************************************************************
*函数名称		K_means()
*参数			void
*返回值		    void
*函数功能		按照K均值算法对全体样品进行分类(样本pattern数组、样本数目、特征维数、聚类数目均已赋值完毕)
************************************************************/
void CCluster::K_means()
{
//	int		distype;//距离的形式(欧氏、余弦...)
	int		times=10000;//max loop number
	int		i,j;

	bool change=true;//退出标志，false时表示样品所属类别不再变化，中止计算
	int counter=0;//记录当前已经循环的次数
	double distance;//到各中心的距离
	distance=MAX;
	int index;  //////////////////
	
	m_center = new Center[centernum];	//std::auto_ptr<Center>(new Center[centernum]);  //定义centernum个聚类中心
	
	for ( i=0;i<patternnum;i++)
	{
		m_pattern[i].distance = MAX;  //初始化所有样本离聚类中心的距离为MAX
		m_pattern[i].category = -1;  //初始化所有样本中心号为－1
	}

	for ( i=0;i<centernum;i++)//初始化，前centernum个样本为centernum个聚类中心
	{
		m_pattern[i].category=i;
		m_pattern[i].distance=0;

		m_center[i].feature = new double[N];
		for ( j=0;j<N;j++)
			m_center[i].feature[j]=m_pattern[i].feature[j];

		m_center[i].index=i;
	}

	while (change && counter<times)
	{
		counter++;
		change=false;

		for(i=0;i<patternnum;i++)//对所有样品重新归类
		{
			//计算第i个模式到各个聚类中心的最小距离，
			index=-1;  ///////////////////////////////////////////////////////////
			distance=MAX;
            
			for (int j=0;j<centernum;j++)      //对每个聚类中心
				if (distance>GetDistance(m_pattern[i],m_center[j]))
				{
					distance=GetDistance(m_pattern[i],m_center[j]);
					index=j;//找到最小距离,是到第index个聚类中心的距离
				}

			
			//比较原中心号与新中心号
			//如果不同：则归入新中心，并改动change标记
			if (m_pattern[i].category!=m_center[index].index)//不属于原类
			{
				m_pattern[i].category=m_center[index].index;//归入新类
				change=true;   
			}
		}    //对每个样本的重新聚类结束

		for (int j=0;j<centernum;j++)      //对每个聚类中心
			CalCenter(&m_center[j]);//计算新属类中心 CalCenter(&m_center[j]);
	}//end of while
	SetCenter();

	//删除聚类中心指针
	for (i=0;i<centernum;i++)
	{
		delete []m_center[i].feature;
	}
	delete []m_center;
}

/************************************************************
*函数名称		K_means()
*参数			void
*返回值		    void
*函数功能		按照K均值算法对全体样品进行分类(样本pattern数组、样本数目、特征维数、聚类数目均已赋值完毕)
************************************************************/
void CCluster::K_means(KResult* kresult)
{
	//	int		distype;//距离的形式(欧氏、余弦...)
	int		times = 10000;//max loop number
	int		i, j;

	bool change = true;//退出标志，false时表示样品所属类别不再变化，中止计算
	int counter = 0;//记录当前已经循环的次数
	double distance;//到各中心的距离
	distance = MAX;
	int index;  //////////////////

	m_center = new Center[centernum];  //定义centernum个聚类中心

	for (i = 0; i<patternnum; i++)
	{
		m_pattern[i].distance = MAX;  //初始化所有样本离聚类中心的距离为MAX
		m_pattern[i].category = -1;  //初始化所有样本中心号为－1
	}

	for (i = 0; i<centernum; i++)//初始化，前centernum个样本为centernum个聚类中心
	{
		m_pattern[i].category = i;
		m_pattern[i].distance = 0;

		m_center[i].feature = new double[N];
		for (j = 0; j<N; j++)
			m_center[i].feature[j] = m_pattern[i].feature[j];

		m_center[i].index = i;
	}

	while (change && counter<times)
	{
		counter++;
		change = false;

		for (i = 0; i<patternnum; i++)//对所有样品重新归类
		{
			//计算第i个模式到各个聚类中心的最小距离，
			index = -1;  ///////////////////////////////////////////////////////////
			distance = MAX;

			for (int j = 0; j<centernum; j++)      //对每个聚类中心
				if (distance>GetDistance(m_pattern[i], m_center[j]))
				{
					distance = GetDistance(m_pattern[i], m_center[j]);
				index = j;//找到最小距离,是到第index个聚类中心的距离
				}


			//比较原中心号与新中心号
			//如果不同：则归入新中心，并改动change标记
			if (m_pattern[i].category != m_center[index].index)//不属于原类
			{
				m_pattern[i].category = m_center[index].index;//归入新类
				change = true;
			}
		}    //对每个样本的重新聚类结束

		for (int j = 0; j<centernum; j++)      //对每个聚类中心
			CalCenter(&m_center[j]);//计算新属类中心
	}//end of while 

	SetCenter(kresult);
	//删除聚类中心指针
	for (i=0;i<centernum;i++)
	{
		delete []m_center[i].feature;
	}
	delete []m_center;
}

/***********************************************************
*函数名称		InitData(vector<double> data)
*参数			data:vector容器
*返回值			void
*函数功能		获取接口提供的vector<double>数组
************************************************************/
void CCluster::InitData(vector<double>& data)
{
	this->data_ = data;
}

/***********************************************************
*函数名称		GetResult()
*参数			
*返回值			Result结构体
*函数功能		获取聚类结果结构体
************************************************************/
vector<CCluster::Result*> CCluster::GetResult() const
{
	return result_;
}

/***********************************************************
*函数名称		K_meansPort(int patternNum,int N,int K,KResult* kresult)
*参数			patternNum:样本数 N:指定特征矢量的维数 K:聚类数
*返回值			void
*函数功能		提供对外的K-means聚类接口(提供样本pattern数组、样本数目、特征维数等为赋值参数)
*作者			Alxor
************************************************************/
void CCluster::startAnalysis(int patternNum, int N, int K, KResult* kresult)
{
	int i, j, id, count;
	string str, word;
	double tf_idf = 0.0;
	int line_num = 0;
	CCluster m_cluster;

	try
	{
		//cout << "K_means Start ..." << endl;
		m_cluster.patternnum = patternNum;
		m_cluster.N = N;
		m_cluster.centernum = K;
		count = data_.size();

		//新建m_cluster.patternnum++个模式，每个模式都是m_cluster.N维的
		m_cluster.m_pattern = new CCluster::Pattern[m_cluster.patternnum];
		for (i = 0; i < m_cluster.patternnum; i++)
			m_cluster.m_pattern[i].feature = new double[m_cluster.N];

		//将多维特征输入到模式样本集
		for (i = 0; i < count; i++)
		{
			id = i / m_cluster.N;
			//cout << data_[i] << endl;
			m_cluster.m_pattern[id].index = id;
			j = i % m_cluster.N;
			m_cluster.m_pattern[id].feature[j] = data_[i];
			cout << m_cluster.m_pattern[id].feature[j] << " ";
		}
		cout << endl;
		Normalization(m_cluster);
		m_cluster.K_means(kresult);//K-means
		for (i = 0; i < m_cluster.patternnum; i++)
		{
			Result* current = new Result;//std::auto_ptr<Result> current;
			//std::auto_ptr<Result> current = std::auto_ptr<Result>(new Result);
			current->id = m_cluster.m_pattern[i].index;
			current->category = m_cluster.m_pattern[i].category;
			if (m_cluster.N < FIELDMAX)
			{
				for (j = 0; j < m_cluster.N; j++)
				{
					current->feature[j] = m_cluster.m_pattern[i].feature[j];
				}
			}
			result_.push_back(current);
			//cout << m_cluster.m_pattern[i].category << endl;
			//delete current;
		}
		//cout << "K_means End ..." << endl;
	}
	catch (exception* ex)
	{
		throw ex;
	}
	//cin.get();
}

/***********************************************************
*函数名称		K_meansPort(int patternNum,int N,int K)
*参数			patternNum:样本数 N:指定特征矢量的维数 K:聚类数
*返回值			void
*函数功能		提供对外的K-means聚类接口(提供样本pattern数组、样本数目、特征维数等为赋值参数)
*作者			Alxor
************************************************************/
void CCluster::startAnalysis(int patternNum, int N, int K)
{
	int i, j, id, count;
	string str, word;
	double tf_idf = 0.0;
	int line_num = 0;
	CCluster m_cluster;

	try
	{
		//cout << "K_means Start ..." << endl;
		m_cluster.patternnum = patternNum;
		m_cluster.N = N;
		m_cluster.centernum = K;
		count = data_.size();

		//新建m_cluster.patternnum++个模式，每个模式都是m_cluster.N维的
		m_cluster.m_pattern = new CCluster::Pattern[m_cluster.patternnum];
		for (i = 0; i < m_cluster.patternnum; i++)
			m_cluster.m_pattern[i].feature = new double[m_cluster.N];

		//将多维特征输入到模式样本集
		for (i = 0; i < count; i++)
		{
			id = i / m_cluster.N;
			//cout << data_[i] << endl;
			m_cluster.m_pattern[id].index = id;
			j = i % m_cluster.N;
			m_cluster.m_pattern[id].feature[j] = data_[i];
			cout << m_cluster.m_pattern[id].feature[j] << "\t";
		}
		cout << endl;
		Normalization(m_cluster);			//样本归一化处理
		m_cluster.K_means();//K-means
		for (i = 0; i < m_cluster.patternnum; i++)
		{
			Result* current = new Result;//std::auto_ptr<Result> current; // = new Result;
			current->id = m_cluster.m_pattern[i].index;
			current->category = m_cluster.m_pattern[i].category;
			if (m_cluster.N < FIELDMAX)
			{
				for (j = 0; j < m_cluster.N; j++)
				{
					current->feature[j] = m_cluster.m_pattern[i].feature[j];
				}
			}
			result_.push_back(current);
			//cout << m_cluster.m_pattern[i].category << endl;
			delete current;
		}
		//cout << "K_means End ..." << endl;
	}
	catch (exception* ex)
	{
		throw ex;
	}
	//cin.get();
}

//设置聚类中心
void CCluster::SetCenter(KResult* pcenter)
{
	try
	{
		for (int c = 0; c < centernum; c++)
		{
			pcenter[c].feature_ = new double[N];
			for (int n = 0; n < N; n++)
			{
				pcenter[c].feature_[n] = this->m_center[c].feature[n];
			}
			pcenter[c].index_ = this->m_center[c].index;
			pcenter[c].patternnum_ = this->m_center[c].patternnum;
		}
	}
	catch (exception* ex)
	{
		throw ex;
	}
}

//设置聚类中心
void CCluster::SetCenter()
{
	try
	{
		for (int c = 0; c < centernum; c++)
		{
			this->pcenter_[c].feature_ = new double[N];
			for (int n = 0; n < N; n++)
			{
				this->pcenter_[c].feature_[n] = this->m_center[c].feature[n];
			}
			this->pcenter_[c].index_ = this->m_center[c].index;
			this->pcenter_[c].patternnum_ = this->m_center[c].patternnum;
		}
	}
	catch (exception* ex)
	{
		throw ex;
	}
}

//样本数据归一化 Xi = 
void CCluster::Normalization(CCluster& m_cluster)
{

	for (int j = 0; j < m_cluster.patternnum; j++)
	{
		double sum = 0.0;
		for (int i = 0; i < m_cluster.N; i++)
		{
			sum += abs(m_cluster.m_pattern[j].feature[i]);
		}
		
		double current;
		try
		{
			for (int n = 0; n < m_cluster.N; n++)
			{
				current = abs(m_cluster.m_pattern[j].feature[n]);
				if (sum != 0)
				{
					m_cluster.m_pattern[j].feature[n] = (double)(current / sum);
				}
				else
				{
					m_cluster.m_pattern[j].feature[n] = 0.0;
				}
			}
		}
		catch (double)
		{
			throw -1.0;
		}
	}

}