#include <cstdlib>
#include <iostream>
#include <math.h> 
//#include "Matrix.h"
#include "distance.h"

using namespace std;

distance::distance(double x[], double y[],int n) : x_(x), y_(y), n_(n)
{

}

//Å·ÊÏ¾àÀë 
double distance::getEuclidean(double x[], double y[], int n)
{
	double euclidean = 0;
	for (int i = 0; i<n; i++)
	{
		euclidean += (x[i] - y[i])*(x[i] - y[i]);
	}
	euclidean = sqrt(euclidean);
	return euclidean;
}

double distance::getEuclidean2(double* x, double* y, int n)
{
	double euclidean = 0;
	for (int i = 0; i < n; i++)
	{
		euclidean += (x[i] - y[i])*(x[i] - y[i]);
	}
	euclidean = sqrt(euclidean);
	return euclidean;
}

double distance::getEuclidean2(boost::ptr_vector<double>& v1, boost::ptr_vector<double>& v2)
{
	double euclidean = 0;
	int n = v1.size() == v2.size() ? v1.size() : 0;
	if (n > 0)
	{
		for (boost::ptr_vector<double>::iterator it1 = v1.begin(), it2 = v2.begin(); it1 != v1.end(), it2 != v2.end(); it1++, it2++)
		{
			euclidean += (*it1 - *it2)*(*it1 - *it2);
		}
		euclidean = sqrt(euclidean);
		return euclidean;
	}
	else
		return (double)-1;
}

double distance::getEuclidean2(std::vector<double>& v1, std::vector<double>& v2)
{
	double euclidean = 0;
	int n = v1.size() == v2.size() ? v1.size() : 0;
	if (n > 0)
	{
		for (std::vector<double>::iterator it1 = v1.begin(), it2 = v2.begin(); it1 != v1.end(), it2 != v2.end(); it1++, it2++)
		{
			euclidean += (*it1 - *it2)*(*it1 - *it2);
		}
		euclidean = sqrt(euclidean);
		return euclidean;
	}
	else
		return (double)-1;
}

double distance::getEuclidean()
{
	double euclidean = 0;
	for (int i = 0; i<n_; i++)
	{
		euclidean += (x_[i] - y_[i])*(x_[i] - y_[i]);
	}
	euclidean = sqrt(euclidean);
	return euclidean;
}

//¾ø¶ÔÖµ¾àÀë
double distance::getManhattan(double x[], double y[], int n)
{
	double manhattan = 0;
	for (int i = 0; i<n; i++)
	{
		manhattan += fabs(x[i] - y[i]);
	}
	return manhattan;
}

double distance::getManhattan()
{
	double manhattan = 0;
	for (int i = 0; i<n_; i++)
	{
		manhattan += fabs(x_[i] - y_[i]);
	}
	return manhattan;
}

//ÇĞÊÏ¾àÀë
double distance::getChebyshev(double x[], double y[], int n)
{
	double chebyshev = 0;
	for (int i = 0; i<n; i++)
	{
		double temp;
		temp = fabs(x[i] - y[i]);
		if (temp>chebyshev) chebyshev = temp;
	}
	return chebyshev;
}

double distance::getChebyshev()
{
	double chebyshev = 0;
	for (int i = 0; i<n_; i++)
	{
		double temp;
		temp = fabs(x_[i] - y_[i]);
		if (temp>chebyshev) chebyshev = temp;
	}
	return chebyshev;
}

//Ã÷ÊÏ¾àÀë 
double distance::getMinkowski(double x[], double y[], int n, int m)
{
	double minkowski = 0;
	for (int i = 0; i<n; i++)
	{
		double temp = 1;
		for (int j = 0; j<m; j++)
		{
			temp = temp*fabs(x[i] - y[i]);
		}
		minkowski += temp;
	}
	minkowski = pow(minkowski, 1 / m);
	return minkowski;
}

double distance::getMinkowski(int m)
{
	double minkowski = 0;
	for (int i = 0; i<n_; i++)
	{
		double temp = 1;
		for (int j = 0; j<m; j++)
		{
			temp = temp*fabs(x_[i] - y_[i]);
		}
		minkowski += temp;
	}
	minkowski = pow(minkowski, 1 / m);
	return minkowski;
}

//Camberra¾àÀë
double distance::getCamberra(double x[], double y[], int n)
{
	double camberra = 0;
	for (int i = 0; i<n; i++)
	{
		camberra += (x[i] - y[i]) / (x[i] + y[i]);
	}
	camberra = fabs(camberra);
	return camberra;
}

double distance::getCamberra()
{
	double camberra = 0;
	for (int i = 0; i<n_; i++)
	{
		camberra += (x_[i] - y_[i]) / (x_[i] + y_[i]);
	}
	camberra = fabs(camberra);
	return camberra;
}