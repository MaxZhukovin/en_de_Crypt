#pragma once

#include<conio.h>
#include<tchar.h>

#include"Algorithm.h"
#include"KeyManager.h"
#include"AES_key.h"

#include"AES.h"
#include"DES.h"
#include"TripleDES.h"
#include"CBC_Encrypt.h"
#include"CBC_Decrypt.h"

#define AMOUNT_ALGO 5
#define DES_KEY_SIZE 16
#define INT64_SIZE 64 
#define AES_IV_SIZE 32

template<class block>
class Interface
{

private:
	KeyManager keys[3];
	int keys_amount[AMOUNT_ALGO] = { 1,3,3,2,2 };
	AES_key aes_key;

	AES< AES_STRUCT<int64>> aes;
	DES<block> des;
	TripleDES<block> tdes;
	CBC_Encrypt<block> ecbc;
	CBC_Decrypt<block> dcbc;
	bool flags[10][3] = {

		{false, false, false}, //des(en)
		{false, false, false}, //3DES EEE3(en)
		{false, true, false},  //3DES EDE3(en)
		{false, false, false}, //3DES EEE2(en)
		{false, true, false},  //3DES EDE2(en)

		//{true, true, true},    //des(de)
		//{true, true, true},    //3DES EEE3(de)
		//{true, false, true},   //3DES EDE3(de)
		//{true, true, true},    //3DES EEE2(de)
		//{true, false, true},    //3DES EDE2(de)
	};

	

public:
	bool mode;
	Algorithm< AES_STRUCT<int64>>* get_aes() {
	
		mode = input_mode();

		unsigned key_size = input_key_size();
		printf("Введите ключ в hex формате(%i символов: 0-9, A-F, a-f)\n",key_size);
		char pass[INT64_SIZE];
		get_int64(key_size, pass);

		aes_key.generate(pass, key_size*4, mode);
		aes.init(&aes_key);


		if (input_algo_mode()) {  //CBC(0)

			Algorithm<block>* algo = mode ? (Algorithm<block>*) & dcbc : (Algorithm<block>*) & ecbc;

			printf("Введите  начальный вектор в hex формате(%i символов: 0-9, A-F, a-f)\n", AES_IV_SIZE);
			char iv1[INT64_SIZE], iv2[INT64_SIZE];

			//get_int64(AES_IV_SIZE/2, iv1);
			//get_int64(AES_IV_SIZE/2, iv2);
			AES_STRUCT<int64> iv;
			iv.data_parts[0] = 0x1234567812345678;//std::stoull(iv1, NULL, 16);
			iv.data_parts[1] = 0x1234567812345678;//std::stoull(iv1, NULL, 16);

			((CBC<block>*)algo)->setIV(iv);
			((CBC<block>*)algo)->setAlgo(&aes);

			return algo;
		}

		return &aes;	//ECB(1)
	}
private:
	unsigned input_key_size() {
		printf("Выберите размер ключа: 1 - 128бит, 2 - 192бит, 3 - 256бит\n");
		unsigned arr[] = { 32,48,64 };
		unsigned char answer;
		do
		{
			answer = ((unsigned char)_getch()) - 0x31;
			if (answer <= 2)
			{
				printf("%i\n", arr[answer]);
				return arr[answer];
			}
		} while (true);
	}
public:

	Algorithm<block>* run() {

		Algorithm<block>* algo;

		//en(0)/de(1)
		mode = input_mode();

		//get algorithm
		int algo_index = input_algo();

		printf("Введите необходимое количество ключей в hex формате(16 символов: 0-9, A-F, a-f)\n");
		for (int i = 0; i < keys_amount[algo_index]; i++) {
			printf("Введите ключ №%i\n", i + 1);
			keys[i].setPass(input_key(DES_KEY_SIZE), flags[algo_index][i] ^ mode);
		}
		if (keys_amount[algo_index] == 2) keys[2] = keys[0];  //for TDES ...2 mode


		Algorithm<block>* algo_tmp = algo_index > 0 ? (Algorithm<block>*)&tdes : (Algorithm<block>*)&des;
		if (input_algo_mode()) {  //CBC(0)

			algo = mode ? (Algorithm<block>*)&dcbc : (Algorithm<block>*)&ecbc;

			((CBC<block>*)algo)->setIV(inputIV());
			((CBC<block>*)algo)->setAlgo(algo_tmp);
		}
		else
			algo = algo_tmp; //ECB(1)

		algo->init(keys);

		return algo;
	}

private:
	bool get_bool() {
		unsigned char answer;
		do
		{
			answer = ((unsigned char)_getch()) - 0x31;
			if (answer <= 1)
			{
				printf("%i\n", answer + 1);
				return answer;
			}
		} while (true);
	}
	//1 - decrypt
	//0 - encrypt
	bool input_mode() {
		printf("Выберите режим работы: 1 - шифровать, 2 - расшифровать\n");
		return get_bool();
	}
	//1 - ECB
	//0 - CBC
	bool input_algo_mode() {

		printf("Выберите режим работы: 1-ECB, 2-CBC\n");
		return get_bool();
	}

	int input_algo() {

		printf("Выберите алгоритм:\n");
		printf("1 - DES\n");
		printf("2 - 3DES EEE3\n");
		printf("3 - 3DES EDE3\n");
		printf("4 - 3DES EEE2\n");
		printf("5 - 3DES EDE2\n");
		unsigned char answer;
		do
		{
			answer = ((unsigned char)_getch()) - 0x31;
			//answer = (int)_getch() - 48;

			if (answer <= 4) {
				printf("%i\n",answer+1);
				return answer;
			}
		} while (true);
	}


	bool check_hex(int sym) {
		return ((sym > 47 && sym < 58) | (sym > 64 && sym < 71) | (sym > 96 && sym < 103));
	}

	void get_int64(unsigned amount, char* key) {

		char answer;
		int i = 0;
		do
		{
			answer = _getch();

			if (check_hex((int)answer)) {
				putchar(answer);
				key[i] = answer;
				i++;
			}
			else
				if(answer == '\b' && i>0) {
					printf("\b \b");
					i--;
				}

		} while (i < amount);

		printf("\n");
	}

	int64 input_key(unsigned amount) {
		char key[INT64_SIZE];
		get_int64(amount,key);
		return std::stoull(key, NULL, 16);
		//return 0x3b3898371520f75e;
	}

	int64 inputIV() {
		printf("Введите начальный вектор в hex формате(16 символов: 0-9, A-F, a-f)\n");
		char key[INT64_SIZE];
		return get_int64(DES_KEY_SIZE, key);
		//return 0x3b3898371520f75e;
	}

public:
	bool input_out_file(TCHAR* szFile, unsigned size) {

		OPENFILENAME ofn;
		get_ofn(ofn, szFile, size,true);
		return GetSaveFileName(&ofn) == TRUE;
	}

	bool input_in_file(TCHAR* szFile, unsigned size) {

		OPENFILENAME ofn;
		get_ofn(ofn, szFile, size, false);
		return GetOpenFileName(&ofn) == TRUE;
	}

private:
	void get_ofn(OPENFILENAME& ofn, TCHAR* szFile, unsigned size, bool flag) {

		HWND hwnd = NULL;              // owner window
		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hwnd;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = size;
		ofn.lpstrFilter = _T("All\0*.*\0Text\0*.TXT\0");
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = flag? OFN_OVERWRITEPROMPT : OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	}


};
