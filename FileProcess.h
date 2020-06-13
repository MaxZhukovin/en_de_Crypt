#pragma once

#define DATA_BLOCK_SIZE 8

#include <windows.h>
#include <tchar.h>

#include "Algorithm.h"
#include "Interface.h"

template<class block>
class FileProcess {

	Algorithm<block>* algo;

	HANDLE in_hf;              // file handle
	HANDLE out_hf;

	unsigned file_size;

	block* buffer;
	bool mode;

	DWORD clustersize;
	bool opened;
public:

	inline int open_files(TCHAR* out, TCHAR* in, bool flags_out, bool flags_in) {

		out_hf = CreateFile(out,		
			GENERIC_WRITE ,//GENERIC_WRITE,
			0,
			(LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);

		if (out_hf == INVALID_HANDLE_VALUE) return 1;

		in_hf = CreateFile(in,		
			 GENERIC_READ,//GENERIC_READ,
			0,
			(LPSECURITY_ATTRIBUTES)NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);

		if (in_hf == INVALID_HANDLE_VALUE) { 
			CloseHandle(in_hf);
			return 2; 
		}

		DWORD lpBytesPerSector = 0;


		TCHAR drive[4] = { in[0] };
		TCHAR drive2[4] = { out[0] };
		_tcscat_s(drive, 4, _T(":\\"));
		_tcscat_s(drive2, 4, _T(":\\"));
		DWORD clustersize2;

		GetDiskFreeSpace(drive, &clustersize, &lpBytesPerSector, 0, 0);
		clustersize *= lpBytesPerSector;
		GetDiskFreeSpace(drive, &clustersize2, &lpBytesPerSector, 0, 0);
		clustersize2 *= lpBytesPerSector;
		if (clustersize2 > clustersize) clustersize = clustersize2;

		if ((file_size = GetFileSize(in_hf, NULL)) != INVALID_FILE_SIZE)
		{
			opened = true;
			return 0;
		}
		return 1;
	}

	void set_algo(Algorithm<block>* algo) {
		this->algo = algo;
	}
	void set_mode(bool mode) { 
		this->mode = mode; 
	}

	bool process() {
		if (!opened) {
			return false;
		}

		int N = sizeof(block), blocksize=0;
		while (N >>= 1) ++blocksize;

		buffer = (block*)realloc(NULL, clustersize);
		unsigned clusteramount;

		//process full clusters
		clusteramount = file_size / clustersize;
		//last block in last cluster in not full, size in the next cluster(if decrypt)
		if ((file_size % clustersize)==1 && mode && clusteramount>0)
			clusteramount--;

		DWORD nbytesread = 0;
		for (int i = 0; i < clusteramount; i++){
		
			ReadFile(in_hf, buffer, clustersize, &nbytesread, NULL);

			block_process(clustersize >> blocksize);

			WriteFile(out_hf, buffer, nbytesread, NULL, NULL);
		}
		
		//last cluster is not full
		unsigned bytesinlast = file_size - clusteramount * clustersize;
		if (bytesinlast != 0) {

			//compute bytes to read
			unsigned bytestoread = mode ? (bytesinlast > clustersize ? clustersize : bytesinlast - 1) : bytesinlast;

			ReadFile(in_hf, buffer, bytestoread+1, NULL, NULL);
			block_process(ceil((double)bytestoread / sizeof(block)));
			
			int correctsize = 0;
			if (mode) {

				if (bytesinlast % sizeof(block) != 0) {
					ReadFile(in_hf, &correctsize, 1, NULL, NULL);
					correctsize = sizeof(block) - correctsize + 1;
				}

				WriteFile(out_hf, buffer, bytesinlast - correctsize, NULL, NULL);
			}
			else {

				WriteFile(out_hf, buffer, ceil((double)bytestoread /sizeof(block)) * sizeof(block), NULL, NULL);

				correctsize = bytesinlast % sizeof(block);
				if(correctsize != 0)
					WriteFile(out_hf, &correctsize, 1, NULL, NULL);
			}

		}

		CloseHandle(in_hf);
		CloseHandle(out_hf);
		realloc(buffer, 0);

		return true;
	}

	//processes the specified number of blocks
	void block_process(unsigned amount) {

		for (int i = 0; i < amount; i++) {

			buffer[i] = algo->act(buffer[i]);

		}
	}

};