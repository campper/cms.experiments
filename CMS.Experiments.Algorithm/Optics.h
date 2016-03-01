/**
 * alxor
 */
#ifndef _OPTICS_H_
#define _OPTICS_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <cmath>
#include <algorithm>
#include <exception>
#include <cassert>

#include "distance.h"

class DataPoint;
//OPTICS cluster aglorithm Based on density 
class Optics
{
private:
	/*
	函数功能：
	入口参数:
	dataPoint:数据对象
	dataPoints:数据对象集合
	radius: epsilon
	ObjectNum: minPts
	&tmpList: 满足条件的数据对象 push_back到tmpList
	出口参数：
	*/
	void isKeyAndReturnObjects(DataPoint* dataPoint, std::vector<DataPoint*>& dataPoints, double radius, int ObjectNum, std::vector<DataPoint*>& tmpList);

	bool isKeyObjects(DataPoint& dataPoint, std::vector<DataPoint*>& dataPoints, double radius, int ObjectNum);	//Is core object

	double getDistance(DataPoint& dp1, DataPoint& dp2);
	int isContainedInList(DataPoint& dp, std::vector<DataPoint*>& dpList) const;
																			//基于排序密度聚类分析,无外部参数
	int compare_byclusterId(const DataPoint& dp1, const DataPoint& dp2);						//list的排序规则

public:
	void InitData(std::vector<double>& data, int N);											//初始化数据
	void InitData(std::vector<DataPoint*> data);
	void startAnalysis(std::vector<DataPoint*>& dataPoints, double radius, int ObjectNum);		//基于排序密度聚类分析
	void startAnalysis(double, int);															//基于排序密度聚类分析,提供邻域半径和数据对象数目
	void startAnalysis();
	void displayDataPoints() const;
	std::vector<DataPoint*> getSeedList() const;													//获取聚类排序后的簇集合，已完成排序
	void setParameters(int epsilon, int minPts);
	int getMinPts() const;
	void setMinPts(int);
	int getEpsilon() const;
	void setEpsilon(int);
	void Normalization(DataPoint& m_datapoint);
public:
	Optics() :datanum_(0), epsilon_(0),minPts_(0), dataSets_(NULL), arrivableObjects_(NULL)
	{

	}

	~Optics()
	{

	}
private:
	std::vector<DataPoint*> dataSets_;			//原始数据集合
	std::vector<DataPoint*> seedList_;			//核心距离队列
	std::vector<DataPoint*> arrivableObjects_;	//可达距离队列
	double epsilon_;							//数据半径 ε 
	int minPts_;								// minPts
	int datanum_;								// 数据量
	std::ofstream log_;							// log
};


#endif // _OPTICS_H_
