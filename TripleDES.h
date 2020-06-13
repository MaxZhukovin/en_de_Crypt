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

	block act(block &source) override {
		block tmp = des_arr[0].act(source);
		block tmp2 = des_arr[1].act(tmp);
		return des_arr[2].act(tmp2);
	}

	void init(void* manager) {
	
		des_arr[0].init((KeyManager*)manager);
		des_arr[1].init(&((KeyManager*)manager)[1]);
		des_arr[2].init(&((KeyManager*)manager)[2]);
	}

};

