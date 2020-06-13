#pragma once
#include"CBC.h"
template<class block>
class CBC_Decrypt :
	public CBC<block>
{
public:
	block act(block &source) {
		block tmp = source;
		block res = this->algo->act(source) ^ this->IV;
		this->IV = tmp;
		return res;
	}


};