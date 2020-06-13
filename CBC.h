#pragma once
#include "Algorithm.h"
template<class block>
class CBC:
	public Algorithm<block>
{
protected:
	block IV;
	Algorithm<block>* algo;
public:
	void setIV(block source) {
		IV = source;
	}

	void setAlgo(Algorithm<block>* algo) {
		this->algo = algo;
	}

	void init(void* keys) {
		algo->init(keys);
	}

};