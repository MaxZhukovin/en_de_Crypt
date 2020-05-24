#pragma once
#include "Algorithm.h"
template<class block>
class CBC :
	public Algorithm<block>
{
protected:
	//inv
	Algorithm<block>* algo;
public:
	setAlgo(Algorithm<block>*) {};
};

