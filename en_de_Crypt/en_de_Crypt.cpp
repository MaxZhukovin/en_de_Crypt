// en_de_Crypt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include"FileManager.h"
#include"BlockManager.h"
#include"DES.h"
#include"encrypt.h"
int main()
{
	DES<BlockManager>* algo = new DES<BlockManager>;
	encrypt<BlockManager>* mode = new encrypt<BlockManager>;
	KeyManager* key = new KeyManager;
	BlockManager* block= new BlockManager;

	mode->setBlockManager(block);
	mode->setKeyManager(key);
	algo->setMode(mode);

	FileManager<BlockManager> fm;
	fm.setAlgo(algo);

    std::cout << "Hello World!\n";
}
