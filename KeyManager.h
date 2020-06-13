#pragma once


#include <bitset>
#include <string>

#define KEYS_AMOUNT 16
#define WRONG_KEYS_AMOUNT 16

typedef unsigned long long int64;
typedef unsigned int32;

class KeyManager
{
private:
	int64 keys[KEYS_AMOUNT];
public:

	bool setPass(int64 pass, bool decrypt) {

		if (check_key(pass))
		{
			generate(pass, decrypt);
			return true;
		}

		return false;
	}

	int64 getKey(unsigned i) {
		return keys[i];
	}

	KeyManager() {}

	KeyManager(KeyManager& source) {

		for (int i = 0; i < KEYS_AMOUNT; i++)
			this->keys[i] = source.keys[i];

	}

private:


	static
		int64 wrong_k[WRONG_KEYS_AMOUNT];
	static
		bool check_key(int64 input) {

		//int64 input = std::stoull(password, nullptr, 16);

		for (int i = 0; i < WRONG_KEYS_AMOUNT; i++)
			if (input == wrong_k[i]) return false;

		return true;

	}

	//set keys
	void generate(int64 input, bool decrypt) {


		//int64 input = std::stoull(password, nullptr, 16);

		//int64 res = G(input);
		int32 left = C(input);
		int32 right = D(input);


		int step = decrypt ? -1 : 1;
		int j = decrypt ? 15 : 0;

		for (int i = 0; i < KEYS_AMOUNT; i++, j += step)
		{

			left = part_shift(left, i);
			right = part_shift(right, i);

			this->keys[j] = H(left, right);
		}

	}

	__forceinline int32 C(int64 source) {

		int32 res = 0;

		res = (res << 1) | ((source >> 7) & 1);
		res = (res << 1) | ((source >> 15) & 1);
		res = (res << 1) | ((source >> 23) & 1);
		res = (res << 1) | ((source >> 31) & 1);
		res = (res << 1) | ((source >> 39) & 1);
		res = (res << 1) | ((source >> 47) & 1);
		res = (res << 1) | ((source >> 55) & 1);

		res = (res << 1) | ((source >> 63) & 1);
		res = (res << 1) | ((source >> 6) & 1);
		res = (res << 1) | ((source >> 14) & 1);
		res = (res << 1) | ((source >> 22) & 1);
		res = (res << 1) | ((source >> 30) & 1);
		res = (res << 1) | ((source >> 38) & 1);
		res = (res << 1) | ((source >> 46) & 1);

		res = (res << 1) | ((source >> 54) & 1);
		res = (res << 1) | ((source >> 62) & 1);
		res = (res << 1) | ((source >> 5) & 1);
		res = (res << 1) | ((source >> 13) & 1);
		res = (res << 1) | ((source >> 21) & 1);
		res = (res << 1) | ((source >> 29) & 1);
		res = (res << 1) | ((source >> 37) & 1);

		res = (res << 1) | ((source >> 45) & 1);
		res = (res << 1) | ((source >> 53) & 1);
		res = (res << 1) | ((source >> 61) & 1);
		res = (res << 1) | ((source >> 4) & 1);
		res = (res << 1) | ((source >> 12) & 1);
		res = (res << 1) | ((source >> 20) & 1);
		res = (res << 1) | ((source >> 28) & 1);

		return res;
	}

	__forceinline int32 D(int64 source) {

		int32 res = 0;

		res = (res << 1) | ((source >> 1) & 1);
		res = (res << 1) | ((source >> 9) & 1);
		res = (res << 1) | ((source >> 17) & 1);
		res = (res << 1) | ((source >> 25) & 1);
		res = (res << 1) | ((source >> 33) & 1);
		res = (res << 1) | ((source >> 41) & 1);
		res = (res << 1) | ((source >> 49) & 1);

		res = (res << 1) | ((source >> 57) & 1);
		res = (res << 1) | ((source >> 2) & 1);
		res = (res << 1) | ((source >> 10) & 1);
		res = (res << 1) | ((source >> 18) & 1);
		res = (res << 1) | ((source >> 26) & 1);
		res = (res << 1) | ((source >> 34) & 1);
		res = (res << 1) | ((source >> 42) & 1);

		res = (res << 1) | ((source >> 50) & 1);
		res = (res << 1) | ((source >> 58) & 1);
		res = (res << 1) | ((source >> 3) & 1);
		res = (res << 1) | ((source >> 11) & 1);
		res = (res << 1) | ((source >> 19) & 1);
		res = (res << 1) | ((source >> 27) & 1);
		res = (res << 1) | ((source >> 35) & 1);

		res = (res << 1) | ((source >> 43) & 1);
		res = (res << 1) | ((source >> 51) & 1);
		res = (res << 1) | ((source >> 59) & 1);
		res = (res << 1) | ((source >> 36) & 1);
		res = (res << 1) | ((source >> 44) & 1);
		res = (res << 1) | ((source >> 52) & 1);
		res = (res << 1) | ((source >> 60) & 1);

		return res;
	}


	//too dificult
	__forceinline int64 G(int64 source) {

		int64 res = 0;

		res = (res << 1) | ((source >> 7) & 1);
		res = (res << 1) | ((source >> 15) & 1);
		res = (res << 1) | ((source >> 23) & 1);
		res = (res << 1) | ((source >> 31) & 1);
		res = (res << 1) | ((source >> 39) & 1);
		res = (res << 1) | ((source >> 47) & 1);
		res = (res << 1) | ((source >> 55) & 1);

		res = (res << 1) | ((source >> 63) & 1);
		res = (res << 1) | ((source >> 6) & 1);
		res = (res << 1) | ((source >> 14) & 1);
		res = (res << 1) | ((source >> 22) & 1);
		res = (res << 1) | ((source >> 30) & 1);
		res = (res << 1) | ((source >> 38) & 1);
		res = (res << 1) | ((source >> 46) & 1);

		res = (res << 1) | ((source >> 54) & 1);
		res = (res << 1) | ((source >> 62) & 1);
		res = (res << 1) | ((source >> 5) & 1);
		res = (res << 1) | ((source >> 13) & 1);
		res = (res << 1) | ((source >> 21) & 1);
		res = (res << 1) | ((source >> 29) & 1);
		res = (res << 1) | ((source >> 37) & 1);

		res = (res << 1) | ((source >> 45) & 1);
		res = (res << 1) | ((source >> 53) & 1);
		res = (res << 1) | ((source >> 61) & 1);
		res = (res << 1) | ((source >> 4) & 1);
		res = (res << 1) | ((source >> 12) & 1);
		res = (res << 1) | ((source >> 20) & 1);
		res = (res << 1) | ((source >> 28) & 1);

		res = (res << 1) | ((source >> 1) & 1);
		res = (res << 1) | ((source >> 9) & 1);
		res = (res << 1) | ((source >> 17) & 1);
		res = (res << 1) | ((source >> 25) & 1);
		res = (res << 1) | ((source >> 33) & 1);
		res = (res << 1) | ((source >> 41) & 1);
		res = (res << 1) | ((source >> 49) & 1);

		res = (res << 1) | ((source >> 57) & 1);
		res = (res << 1) | ((source >> 2) & 1);
		res = (res << 1) | ((source >> 10) & 1);
		res = (res << 1) | ((source >> 18) & 1);
		res = (res << 1) | ((source >> 26) & 1);
		res = (res << 1) | ((source >> 34) & 1);
		res = (res << 1) | ((source >> 42) & 1);

		res = (res << 1) | ((source >> 50) & 1);
		res = (res << 1) | ((source >> 58) & 1);
		res = (res << 1) | ((source >> 3) & 1);
		res = (res << 1) | ((source >> 11) & 1);
		res = (res << 1) | ((source >> 19) & 1);
		res = (res << 1) | ((source >> 27) & 1);
		res = (res << 1) | ((source >> 35) & 1);

		res = (res << 1) | ((source >> 43) & 1);
		res = (res << 1) | ((source >> 51) & 1);
		res = (res << 1) | ((source >> 59) & 1);
		res = (res << 1) | ((source >> 36) & 1);
		res = (res << 1) | ((source >> 44) & 1);
		res = (res << 1) | ((source >> 52) & 1);
		res = (res << 1) | ((source >> 60) & 1);

		return res;
	}


	int32 shift[KEYS_AMOUNT] = { 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

	__forceinline int32 part_shift(int32 part, unsigned index) {
		return ((part << shift[index]) | (part >> (28 - shift[index]))) & 0xFFF'FFFF;
	}


	//glueing two parts and permut
	__forceinline int64 H(int32 left, int32 right) {

		int64 res = 0;

		res = (res << 1) | ((left >> 14) & 1);
		res = (res << 1) | ((left >> 11) & 1);
		res = (res << 1) | ((left >> 17) & 1);
		res = (res << 1) | ((left >> 4) & 1);
		res = (res << 1) | ((left >> 27) & 1);
		res = (res << 1) | ((left >> 23) & 1);
		res = (res << 1) | ((left >> 25) & 1);

		res = (res << 1) | ((left >> 0) & 1);
		res = (res << 1) | ((left >> 13) & 1);
		res = (res << 1) | ((left >> 22) & 1);
		res = (res << 1) | ((left >> 7) & 1);
		res = (res << 1) | ((left >> 18) & 1);
		res = (res << 1) | ((left >> 5) & 1);
		res = (res << 1) | ((left >> 9) & 1);

		res = (res << 1) | ((left >> 16) & 1);
		res = (res << 1) | ((left >> 24) & 1);
		res = (res << 1) | ((left >> 2) & 1);
		res = (res << 1) | ((left >> 20) & 1);
		res = (res << 1) | ((left >> 12) & 1);
		res = (res << 1) | ((left >> 21) & 1);
		res = (res << 1) | ((left >> 1) & 1);

		res = (res << 1) | ((left >> 8) & 1);
		res = (res << 1) | ((left >> 15) & 1);
		res = (res << 1) | ((left >> 26) & 1);



		res = (res << 1) | ((right >> 15) & 1);
		res = (res << 1) | ((right >> 4) & 1);
		res = (res << 1) | ((right >> 25) & 1);
		res = (res << 1) | ((right >> 19) & 1);
		res = (res << 1) | ((right >> 9) & 1);
		res = (res << 1) | ((right >> 1) & 1);
		res = (res << 1) | ((right >> 26) & 1);

		res = (res << 1) | ((right >> 16) & 1);
		res = (res << 1) | ((right >> 5) & 1);
		res = (res << 1) | ((right >> 11) & 1);
		res = (res << 1) | ((right >> 23) & 1);
		res = (res << 1) | ((right >> 8) & 1);
		res = (res << 1) | ((right >> 12) & 1);
		res = (res << 1) | ((right >> 7) & 1);

		res = (res << 1) | ((right >> 17) & 1);
		res = (res << 1) | ((right >> 0) & 1);
		res = (res << 1) | ((right >> 22) & 1);
		res = (res << 1) | ((right >> 3) & 1);
		res = (res << 1) | ((right >> 10) & 1);
		res = (res << 1) | ((right >> 14) & 1);
		res = (res << 1) | ((right >> 6) & 1);

		res = (res << 1) | ((right >> 20) & 1);
		res = (res << 1) | ((right >> 27) & 1);
		res = (res << 1) | ((right >> 24) & 1);


		return res;
	}

};


int64 KeyManager::wrong_k[WRONG_KEYS_AMOUNT] = {

			0x0101010101010101,
			0xFEFEFEFEFEFEFEFE,
			0x1F1F1F1F0E0E0E0E,
			0xE0E0E0E0F1F1F1F1,

			0x01FE01FE01FE01FE,
			0x1FE01FE01FE01FE0,
			0x01E001E001F101F1,
			0x1FFE1FFE0EFE0EFE,
			0x011F011F010E010E,
			0xE0FEE0FEF1FEF1FE,
			0xFE01FE01FE01FE01,
			0xE0F1E0F1E0F1E0F1,
			0xE001E001F101F101,
			0xFE1FFE1FFE0EFE0E,
			0x1F011F010E010E01,
			0xFEE0FEE0FEF1FEF1

};
