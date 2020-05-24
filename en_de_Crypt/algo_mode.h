#pragma once

#include "KeyManager.h"

template<class block>
class algo_mode
{
protected:
	block* bl_manager;
	KeyManager* key;

public:
	virtual /*out*/ void act(/*in*/) = 0;
	void setBlockManager(block*) {}
	void setKeyManager(KeyManager*) {}
};

