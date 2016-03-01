/**
 * alxor
 */
#ifndef _DBSCAN_H_
#define _DBSCAN_H_

#include <cmath>
#include <vector>
#include <iostream>
using namespace std;

class DataPoint;
//基于密度聚类算法
class DBSCAN
{
private:
	std::vector<DataPoint> dadaSets_;        //数据集合
	unsigned int dimNum_;				//维度
	double radius_;						//半径
	unsigned int dataNum_;				//数据数量
	unsigned int minPTs_;				//邻域最小数据个数
	std::vector<double> temp_;				//Add by Alxor
	//fstream& out_;						//Add by Alxor

	double GetDistance(DataPoint& dp1, DataPoint& dp2);						//距离函数
	void SetArrivalPoints(DataPoint& dp);									//设置数据点的领域点列表
	void KeyPointCluster(unsigned long i, unsigned long clusterId);			//对数据点领域内的点执行聚类操作
public:

	DBSCAN();						//默认构造函数
	~DBSCAN();						//默认析构函数
	bool Init(char* fileName, double radius, int minPTs);    //初始化操作 需要重写接口
	void InitData(vector<double>& data, double radius, int minPTs, int N);
	bool DoDBSCANRecursive();            //DBSCAN递归算法
	bool WriteToFile(char* fileName);    //将聚类结果写入文件
	bool WriteToFile(fstream& out);
};


#endif // _DBSCAN_H_

