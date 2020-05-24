#pragma once
#include "Algorithm.h"
#include "DES.h"
#define BUFER_SIZE 4096
template<class block>
class FileManager
{
private:
	char buf[BUFER_SIZE];
	Algorithm<block>* algo;
	//fileOut
	//fileIn

public:
	bool OpenFiles(/*out, in*/);
	void setAlgo(Algorithm<block>* al) {}
	bool process() {

		// in = read()

		/*out = */algo->act(/*in*/);

		//write(out)
	}
};

