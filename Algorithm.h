#pragma once

typedef unsigned long long int64;
typedef unsigned int32;

template<class block>
class Algorithm
{
public:
	virtual block act(block&) = 0;
	virtual void init(void*) = 0;
};

