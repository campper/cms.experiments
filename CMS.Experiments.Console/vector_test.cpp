#include <vector>
#include <iostream>

using namespace std;

class A
{
public:
	A() :a_(-1)
	{

	}
	~A()
	{
		delete[] b_;
	}

	A(const A& other)
	{
		if (!(*this == other))
		{
			this->a_ = other.a_;
			this->b_ = new double[other.len_];
			for (int i = 0; i < other.len_; i++)
				this->b_[i] = other.b_[i];
		}
	}

	bool operator==(const A& other)
	{
		return this->a_ == other.a_;
	}

	A& operator=(const A& r)
	{
		if (*this == r)
		{
			return *this;
		}

		this->a_ = r.a_;
		this->b_ = new double[r.len_];
		for (int i = 0; i < r.len_; i++)
			this->b_[i] = r.b_[i];
	}

private:
	int a_;
	double *b_;
	int len_;
};
