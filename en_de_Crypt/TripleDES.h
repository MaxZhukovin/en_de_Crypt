#pragma once
#include "Algorithm.h"
#include "DES.h"
#define DES_AMOUNT 3

template<class block>
class TripleDES :
	public Algorithm<block>
{
private:
	DES<block> des_arr[DES_AMOUNT];

public:
	/*out*/ void act(/*in*/) override {}
};

