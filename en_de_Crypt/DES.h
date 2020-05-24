#pragma once
#include "Algorithm.h"

template<class block>
class DES :
	public Algorithm<block>
{
private:
	algo_mode<block>* mode;

	
public:
	void setMode(algo_mode<block>* mode) {}
	/*out*/ void act(/*in*/) override {
		/*return */this->mode->act(/*in*/);
	}

};

