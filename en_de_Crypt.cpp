// en_de_Crypt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS

#include <ctime>
#include <iostream>

#include"FileProcess.h"
#include"AES_key.h"
#include"AES_data.h"
#include"AES.h"
int main()
{

	unsigned char res = 19;
	res <<= 4;
	res >>= 4;
	std::cout << (int)res;
	setlocale(LC_ALL, "Russian");
	//AES_key key;
	//char pass[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
	//key.generate(pass, 128, false);

	/*AES<AES_STRUCT<int64>> aes;
	aes.init(&key);*/
	//AES_STRUCT<int64> block;

	//block.data_parts[0] = 0x38373635'34333231;//0x02dc09fb3925841d;//0x885a308d3243f6a8; 
	//block.data_parts[1] = 0x37363534'33323139;//0x196a0b32dc118597;//0xe0370734313198a2;

	//aes.act(block);



	FileProcess< AES_STRUCT<int64>> fp;
	Interface<AES_STRUCT<int64>> in;
	TCHAR out[MAX_PATH] = _T("");
	TCHAR input[MAX_PATH] = _T("");

	do {

		if(!in.input_in_file(input, sizeof(in)))
			MessageBox(NULL, _T("Не удалось выбрать имя входного файла"), _T("en_de_Crypt"), MB_OK | MB_ICONERROR);

		else if (!in.input_out_file(out, sizeof(out)))
			MessageBox(NULL, _T("Не удалось выбрать имя выходного файла"), _T("en_de_Crypt"), MB_OK | MB_ICONERROR);
		else {
			int res = fp.open_files(out, input, true, false);
			if (res == 1)
				MessageBox(NULL, _T("Не удалось открыть входной файл"), _T("en_de_Crypt"), MB_OK | MB_ICONERROR);
			else if (res == 2)
				MessageBox(NULL, _T("Не удалось открыть выходной файл"), _T("en_de_Crypt"), MB_OK | MB_ICONERROR);
			else {

				fp.set_algo(in.get_aes());
				fp.set_mode(in.mode);
				unsigned int start_time = GetTickCount();
				if (fp.process()) {
					printf("Время выполнения действия: %i(миллисекунд)\n", GetTickCount() - start_time);
				}
				else
					MessageBox(NULL, _T("Не удалось обработать файлы"), _T("en_de_Crypt"), MB_OK | MB_ICONERROR);
			}

		}

		if(MessageBox(NULL, _T("Повторить процесс"), _T("en_de_Crypt"), MB_YESNO | MB_ICONINFORMATION) == IDNO)
			break;

	} while (1);

}

