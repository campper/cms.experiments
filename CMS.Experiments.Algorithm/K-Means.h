// Cluster.h: interface for the CCluster class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_CLUSTER_H__738A144B_4EA4_45B0_87CF_E11C2A1B634C__INCLUDED_)
//#define AFX_CLUSTER_H__738A144B_4EA4_45B0_87CF_E11C2A1B634C__INCLUDED_
/**
 * alxor
 */
#ifndef _KMEANS_H_
#define _KMEANS_H_
#define FIELDMAX 50

#include <vector>

#include <memory>
#include "K-Result.h"
using namespace std;


class CCluster
{
public:

	struct Pattern					//样本结构
	{
		int			index;			//样本标号
		int			category;		//样本或模板所属类别
		double		*feature;		//特征值
		double		distance;		// 样品到类中心的距离
	};

	struct Center					//聚类中心结构
	{
		int 		index;			//中心编号
		double		*feature;		//中心特征值
		int 		patternnum;		//该聚类中心包含的样品数目
	};

	struct Result					// Add by Alxor
	{
		int			id;
		int			category;
		double		feature[FIELDMAX];
		bool operator<(const Result& other)
		{
			return category < other.category;
		}
	};

private:
	int         N;					//特征维数
	int			patternnum;			//样本总数
	int 		centernum;			//聚类中心数目，也是类别数目
	Pattern* m_pattern;				//std::auto_ptr<Pattern>		m_pattern;			//指向样本的指针
	Center* m_center;				//std::auto_ptr<Center>		m_center;			//指向中心的指针
	std::vector<double> data_;		//聚类N维数据集
	std::vector<Result*>		result_;			//结果集保存
	KResult* pcenter_;				// std::auto_ptr<KResult> pcenter_;
public:
	CCluster();
	virtual ~CCluster();
private:
	double GetDistance(CCluster::Pattern  pattern, CCluster::Center center);		//计算样品和聚类中心间的距离
	void CalCenter(CCluster::Center* pcenter);
	//void CalCenter(std::auto_ptr<CCluster::Center>& pcenter);						//计算中心pcenter的特征值（本类所有样品的均值），及样品个数
	void K_means();																	//k均值算法聚类
	void K_means(KResult* kresult);
	//void K_means(std::auto_ptr<KResult>& kresult);								//k均值算法聚类 返回保存结果
	void Normalization(CCluster& m_cluster);										//样本数据归一化处理
public:
	void InitData(std::vector<double>& data);									//从外部获取聚类所需的数据
	std::vector<Result*> GetResult() const;
	void startAnalysis(int patternNum, int N, int K, KResult* kresult);
	//void startAnalysis(int patternNum, int N, int K, std::auto_ptr<KResult>& kresult);			//对外接口 Add by Alxor 存在指针泄露问题
	void startAnalysis(int patternNum, int N, int K);
	void SetCenter(KResult* pcenter);
	//void SetCenter(std::auto_ptr<KResult>& pcenter);
	void SetCenter();
};


#endif // _KMEANS_H_
