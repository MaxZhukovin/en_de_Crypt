#pragma once

#include "AES_FUN.h"

#define NB						4
#define NB_COLS_MAX				6
#define	NR_MAX					14
#define	W_COLS_MAX				(NB * (NR_MAX + 1))
#define COL_SIZE				32
#define ROUND_FOR_128			10
#define ROUND_FOR_192			12
#define ROUND_FOR_256			14


#define BLOCK_PARTS 2

#define M_8	0xFF
#define M_16 0xFFFF
#define M_24 0xFFFFFF

typedef unsigned long long int64;
typedef unsigned int32;

class AES_key{
	
private:
	int32 keys[W_COLS_MAX];
	unsigned rounds;
	bool mode;
	unsigned Nk;
	unsigned col_amount;
public:
	int32& get_key(int index) {
		return keys[index];
	}
	unsigned get_nk() {
		return Nk;
	}

	unsigned get_col() {
		return col_amount;
	}

	bool get_mode() {
		return mode;
	}
	unsigned get_rounds() {
		return rounds;
	}

	void generate(char* password, unsigned size, bool mode) {

		col_amount = 44;
		for (int i = 0; i < 7; i+=2)
		{
			keys[0] = (keys[0] << 8 ) | (Char2Hex(password[i],    password[i + 1])   & M_8);
			keys[1] = (keys[1] << 8 ) | (Char2Hex(password[i + 8],  password[i + 9])   & M_8);
			keys[2] = (keys[2] << 8 ) | (Char2Hex(password[i + 16], password[i + 17])  & M_8);
			keys[3] = (keys[3] << 8 ) | (Char2Hex(password[i + 24], password[i + 25])  & M_8);

			if (size > 128)
			{
				keys[4] = (keys[4] << 8) | (Char2Hex(password[i + 32], password[i + 33]) & M_8);
				keys[5] = (keys[5] << 8) | (Char2Hex(password[i + 40], password[i + 41]) & M_8);
			}

			if (size > 192) {
				keys[6] = (keys[6] << 8) | (Char2Hex(password[i + 48], password[i + 49]) & M_8);
				keys[7] = (keys[7] << 8) | (Char2Hex(password[i + 56], password[i + 57]) & M_8);
			}
		}

		col_amount += size <= 128 ? 0 : size <= 192 ? 8 : 16;

		this->mode = mode;

		rounds = size == 128 ? ROUND_FOR_128 : size == 192 ? ROUND_FOR_192 : ROUND_FOR_256;
		Nk = size / COL_SIZE;

		unsigned nblock = 0;
		for (int i = Nk; i < (rounds + 1) * NB; i++) {

			if (i % Nk == 0){
				keys[i] = AES_FUN::SubWord(RotWord(keys[i - 1]), AES_FUN::Sbox) ^ rcon[nblock] ^ keys[i - Nk];
				nblock++;
			}
			else if ((Nk > NB_COLS_MAX) && (i % Nk == NB))
				keys[i] = AES_FUN::SubWord(keys[i-1], AES_FUN::Sbox) ^ keys[i - Nk];
			
			else 	
				keys[i] = keys[i-1] ^ keys[i- Nk];
		}

	}

public:
	
	unsigned char Char2Hex(char str1, char str2) {

		return 16 * GetHexDigit(str1) + GetHexDigit(str2);
	}

	unsigned GetHexDigit(char source) {
		
		unsigned arr[] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xa, 0xb, 0xc, 0xd, 0xe, 0xf }, in = (unsigned)source;
		return in <= 57 ? arr[in - 48] : in <= 70 ? arr[in - 55] : arr[in - 87];
	}


	int32 RotWord(int32 source) {

		int32 res = 0;

		res = source << 8;
		res = res | ((char)(source >> 24) & M_8);
		
		return res;
	}

	int32 rcon[12] = { 0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000, 0x6c000000, 0xd8000000 };

};
