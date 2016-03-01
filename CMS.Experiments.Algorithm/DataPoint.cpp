#include "DataPoint.h"

//设置维度数据
void DataPoint::SetDimension(double* dimension)
{
	this->dimension_ = new double[this->size_];
	for (size_t i = 0; i<this->size_; i++)
	{
		this->dimension_[i] = dimension[i];
	}
}

//获取数据对象值向量
double* DataPoint::GetDimension() const
{
	return this->dimension_;
}

vector<double> DataPoint::GetVecDimension() const
{
	return this->v_dimension_;
}

//获取是否为核心对象
bool DataPoint::IsKey() const
{
	return this->isKey_;
}

//设置核心对象标志
void DataPoint::SetKey(bool isKey)
{
	this->isKey_ = isKey;
}

//获取DpId方法
int DataPoint::GetDpId() const
{
	return dpID_;
}

//设置DpId方法
void DataPoint::SetDpId(long dpID)
{
	this->dpID_ = dpID;
}

//GetIsVisited方法
bool DataPoint::isVisited() const
{
	return this->visited_;
}


//SetIsVisited方法
void DataPoint::SetVisited(bool visited)
{
	this->visited_ = visited;
}

//GetClusterId方法
long DataPoint::GetClusterId() const
{
	return this->clusterId_;
}

//SetClusterId方法
void DataPoint::SetClusterId(int clusterId)
{
	this->clusterId_ = clusterId;
}

string DataPoint::GetName() const
{
	return this->name_;
}

//GetArrivalPoints方法
std::vector<unsigned long>& DataPoint::GetArrivalPoints() const
{
	return const_cast<std::vector<unsigned long>&>(arrivalPoints_); //const_cast取消const指针
}

//获取DataPoint的值向量的最大值
double DataPoint::GetDimensionMaxValue() const
{
	double _max = this->dimension_[0];
	if (this->dimension_ != NULL)
	{
		for (int i = 0;i < sizeof(this->dimension_) / sizeof(double);i++)
		{
			if (this->dimension_[i] > _max)
			{
				_max = this->dimension_[i];
			}
		}
	}
	return _max;
}

double DataPoint::GetDimensionMinValue() const
{
	double _min = this->dimension_[0];
	if (this->dimension_ != NULL)
	{
		for (int i = 0;i < sizeof(this->dimension_) / sizeof(double);i++)
		{
			if (this->dimension_[i] < _min)
			{
				_min = this->dimension_[i];
			}
		}
	}
	return _min;
}

double DataPoint::getReachableDistance() const
{
	return this->reachableDistance_;
}

size_t DataPoint::size() const
{
	return this->size_;
}

void DataPoint::setReachableDistance(double reachableDistance)
{
	this->reachableDistance_ = reachableDistance;
}

void DataPoint::setCoreDistance(double coreDistance)
{
	this->coreDistance_ = coreDistance;
}

double DataPoint::getCoreDistance() const
{
	return this->coreDistance_;
}

//深拷贝 vector 容器push_back时调用
DataPoint::DataPoint(const DataPoint& other)
{
	if (!(*this == other))
	{
		this->clusterId_ = other.clusterId_;
		this->name_ = other.name_;
		this->coreDistance_ = other.coreDistance_;
		this->dpID_ = other.dpID_;
		this->reachableDistance_ = other.reachableDistance_;
		this->size_ = other.size_;
		this->arrivalPoints_ = other.arrivalPoints_;

		if (other.size_ == 0 && other.v_dimension_.size() > 0)
		{
			this->v_dimension_ = other.v_dimension_;
		}
		else
		{
			this->dimension_ = new double[other.size_];
			if (this->dimension_ !=0)
			{
				memcpy(this->dimension_, other.dimension_, sizeof(double)*other.size_);
			}
			
		}
	}
}

//这种调用构造函数有问题，参数赋值失败
DataPoint::DataPoint(double *dimension, size_t len, unsigned long id) : clusterId_(-1), dpID_(id), name_(""), size_(len), coreDistance_(-1), reachableDistance_(-1), visited_(false), isKey_(false)
{
	this->dimension_ = new double[size_];
	for (size_t i = 0; i < this->size_; i++)
	{
		this->dimension_[i] = dimension[i];
	}
}

//深拷贝
DataPoint::DataPoint(double *dimension, size_t len, string name) : clusterId_(-1), name_(name), size_(len), coreDistance_(-1), reachableDistance_(-1), visited_(false), isKey_(false), dpID_(++::index_)
{
	this->dimension_ = new double[size_];
	for (size_t i = 0; i < this->size_; i++)
	{
		this->dimension_[i] = dimension[i];
	}
}

DataPoint::DataPoint(double* dimension) :clusterId_(-1), name_(""), coreDistance_(-1), reachableDistance_(-1), visited_(false), isKey_(false), dpID_(++::index_)
{
	this->dimension_ = new double[size_];
	for (size_t i = 0; i < this->size_; i++)
	{
		this->dimension_[i] = dimension[i];
	}
	//this->dpID_ += ++ ::index_;
}

//私有函数
DataPoint::DataPoint(double* dimension, size_t len) :clusterId_(-1), size_(len), coreDistance_(-1), reachableDistance_(-1), visited_(false), isKey_(false), dpID_(++::index_)
{
	if (this->v_dimension_.size() == 0 && this->dimension_ != NULL)
	{
		this->dimension_ = new double[size_];
		for (size_t i = 0; i < this->size_; i++)
		{
			this->dimension_[i] = dimension[i];
		}
	}
}

//vector充当密度容器的构造函数
DataPoint::DataPoint(vector<double> dimension) :clusterId_(-1), name_(""), coreDistance_(-1), reachableDistance_(-1), visited_(false), isKey_(false), size_(dimension.size()), dpID_(++index_)
{
	this->v_dimension_ = dimension;
}

//默认构造函数
DataPoint::DataPoint()
{
	this->dimension_ = new double[size_];
}

//构造函数
DataPoint::DataPoint(unsigned long dpID, double* dimension, bool isKey) :isKey_(isKey), dpID_(dpID), clusterId_(-1), name_(""), coreDistance_(-1), reachableDistance_(-1), visited_(false), size_(DIME_NUM)
{
	//传递每维的维度数据
	this->dimension_ = new double[size_];
	for (size_t i = 0; i < this->size_; i++)
	{
		this->dimension_[i] = dimension[i];
	}
}

DataPoint::DataPoint(size_t len) : size_(len), clusterId_(-1), name_(""), coreDistance_(-1), reachableDistance_(-1), visited_(false), dpID_(++::index_)
{
	this->dimension_ = new double[size_];
}

//析构函数
DataPoint::~DataPoint()
{
	if(this->dimension_)
		delete[] this->dimension_;

	if(this->arrivalPoints_.size() > 0)
		this->arrivalPoints_.clear();

	if(this->v_dimension_.size() > 0)
		this->v_dimension_.clear();
	
	this->clusterId_ = -1;
	this->coreDistance_ = -1;
	this->reachableDistance_ = -1;
	this->dpID_ = -1;
	this->isKey_ = false;
	this->visited_ = false;
	this->name_ = "";
	this->size_ = 0;

	cout << "~ DataPoint ... " << endl;
}

void DataPoint::clear()
{
	DataPoint::~DataPoint();
}
