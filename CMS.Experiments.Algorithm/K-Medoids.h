/**
 * alxor
 */
#ifndef _KMEDOIDS_H_
#define _KMEDOIDS_H_

#include "K-Result.h"
using namespace std;

class MCluster
{
public:
	struct Pattern					//样本结构
	{
		int			index;			//样本标号
		int			category;		//样本或模板所属类别
		double		*feature;		//特征值
		double		distance;		// 样品到类中心的距离
	};

	struct Medoids					//聚类中心结构
	{
		int 		index;			//中心编号
		double		*feature;		//中心特征值
		int 		patternnum;		//该聚类中心包含的样品数目
	};
public:
	MCluster();
	~MCluster();
private:
	int N;
	Pattern *m_pattern;
	Medoids *m_mediods;
};


#endif // _KMEDOIDS_H_
