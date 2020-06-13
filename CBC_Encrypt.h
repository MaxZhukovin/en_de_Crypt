#pragma once
#include"CBC.h"
template<class block>
class CBC_Encrypt :
	public	CBC<block>
{

public:
	block act(block &source) {
		block tmp = source ^ this->IV;
		this->IV = this->algo->act(tmp);
		return this->IV;
	}


};