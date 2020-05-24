#pragma once
#include "algo_mode.h"
template<class block>
class decrypt :
	public algo_mode<block>
{
public:
	/*out*/ void act(/*in*/) override {
		this->bl_manager->block_process();
	};
};

