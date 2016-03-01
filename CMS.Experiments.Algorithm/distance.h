/**
 * alxor
 */
#ifndef _DISTANCE_H_
#define _DISTANCE_H_
#include <vector>
#include <boost\shared_array.hpp>
#include <boost\ptr_container\ptr_vector.hpp>

/**
*	¾àÀëº¯Êı
**/
class distance
{
public:
	distance() :n_(0)
	{
		
	}
	distance(double x[], double y[], int n);
	~distance()
	{
		delete x_;
		delete y_;
	}
public:
	static double getEuclidean(double x[], double y[], int n);			//Å·Ê½¾àÀë
	static double getManhattan(double x[], double y[], int n);			//¾ø¶ÔÖµ¾àÀë
	static double getChebyshev(double x[], double y[], int n);			//ÇĞÊÏ¾àÀë
	static double getMinkowski(double x[], double y[], int n, int m);	//Ã÷ÊÏ¾àÀë
	static double getCamberra(double x[], double y[], int n);			//Camberra¾àÀë
	static double getEuclidean2(double *x, double *y, int n);			//Å·Ê½¾àÀë Ö¸Õë²ÎÊı
	static double getEuclidean2(boost::ptr_vector<double>& v1, boost::ptr_vector<double>& v2);
	static double getEuclidean2(std::vector<double>& v1, std::vector<double>& v2);
	double getEuclidean();			//Å·Ê½¾àÀë
	double getManhattan();			//¾ø¶ÔÖµ¾àÀë
	double getChebyshev();			//ÇĞÊÏ¾àÀë
	double getMinkowski(int m);	//Ã÷ÊÏ¾àÀë
	double getCamberra();			//Camberra¾àÀë
private:
	double* x_;			//XÏòÁ¿
	double* y_;			//YÏòÁ¿
	int n_;
};

#endif // _DISTANCE_H_
