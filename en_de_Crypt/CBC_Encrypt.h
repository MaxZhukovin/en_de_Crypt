#pragma once
#include "CBC.h"
template<class block>
class CBC_Encrypt :
	public CBC<block>
{
	/*out_data*/void act(/*in_data*/) override {}
};

