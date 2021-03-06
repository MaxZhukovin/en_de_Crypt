#pragma once
#include"Algorithm.h"
//get n lower bits(masks)
#define M_6 0x3F
#define M_5 0x1F
#define M_4 0xF

typedef unsigned long long int64;
typedef unsigned int32;

class BlockManager
{
public:
	void block_process(int32& left, int32& right, int64 key) {

		int32 l_copy = left;

		//swap (left ready)
		left = right;

		//processing right part
		right = F(right, key);
		right ^= l_copy;
	}

private:

	__forceinline int32 F(int32 right, int64 key) {

		return P(S_boxes(E(right) ^ key));
	}

	__forceinline int64 E(int32 source) {

		int32 resl = 0, resr = 0;

		resl = (resl << 1) | ((source >> 0) & 1);
		resl = (resl << 5) | ((source >> 27)& M_5);
		resl = (resl << 6) | ((source >> 23)& M_6);
		resl = (resl << 6) | ((source >> 19)& M_6);
		resl = (resl << 6) | ((source >> 15)& M_6);
		resl = (resl << 6) | ((source >> 11)& M_6);

		resr = (resr << 6) | ((source >> 7)& M_6);
		resr = (resr << 6) | ((source >> 3)& M_6);
		resr = (resr << 5) | ((source)&M_5);
		resr = (resr << 1) | ((source >> 31) & 1);

		return ((int64)resl << 18) | resr;
	}

	char S1[64] = { 14,  0,  4, 15, 13,  7,  1,  4,  2, 14, 15,  2, 11, 13,  8,  1,
					3, 10, 10,  6,  6, 12, 12, 11,  5,  9,  9,  5,  0,  3,  7,  8,
					4, 15,  1, 12, 14,  8,  8,  2, 13,  4,  6,  9,  2,  1, 11,  7,
				   15,  5, 12, 11,  9,  3,  7, 14,  3, 10, 10,  0,  5,  6,  0, 13 };

	char S2[64] = { 15,  3,  1, 13,  8,  4, 14,  7,  6, 15, 11,  2,  3,  8,  4, 14,
					9, 12,  7,  0,  2,  1, 13, 10, 12,  6,  0,  9,  5, 11, 10,  5,
					0, 13, 14,  8,  7, 10, 11,  1, 10,  3,  4, 15, 13,  4,  1,  2,
					5, 11,  8,  6, 12,  7,  6, 12,  9,  0,  3,  5,  2, 14, 15,  9 };

	char S3[64] = { 10, 13,  0,  7,  9,  0, 14,  9,  6,  3,  3,  4, 15,  6,  5, 10,
					1,  2, 13,  8, 12,  5,  7, 14, 11, 12,  4, 11,  2, 15,  8,  1,
				   13,  1,  6, 10,  4, 13,  9,  0,  8,  6, 15,  9,  3,  8,  0,  7,
				   11,  4,  1, 15,  2, 14, 12,  3,  5, 11, 10,  5, 14,  2,  7, 12 };

	char S4[64] = { 7, 13, 13,  8, 14, 11,  3,  5,  0,  6,  6, 15,  9,  0, 10,  3,
					1,  4,  2,  7,  8,  2,  5, 12, 11,  1, 12, 10,  4, 14, 15,  9,
				   10,  3,  6, 15,  9,  0,  0,  6, 12, 10, 11,  1,  7, 13, 13,  8,
				   15,  9,  1,  4,  3,  5, 14, 11,  5, 12,  2,  7,  8,  2,  4, 14 };

	char S5[64] = { 2, 14, 12, 11,  4,  2,  1, 12,  7,  4, 10,  7, 11, 13,  6,  1,
					8,  5,  5,  0,  3, 15, 15, 10, 13,  3,  0,  9, 14,  8,  9,  6,
					4, 11,  2,  8,  1, 12, 11,  7, 10,  1, 13, 14,  7,  2,  8, 13,
				   15,  6,  9, 15, 12,  0,  5,  9,  6, 10,  3,  4,  0,  5, 14,  3 };

	char S6[64] = { 12, 10,  1, 15, 10,  4, 15,  2,  9,  7,  2, 12,  6,  9,  8,  5,
					0,  6, 13,  1,  3, 13,  4, 14, 14,  0,  7, 11,  5,  3, 11,  8,
					9,  4, 14,  3, 15,  2,  5, 12,  2,  9,  8,  5, 12, 15,  3, 10,
					7, 11,  0, 14,  4,  1, 10,  7,  1,  6, 13,  0, 11,  8,  6, 13 };

	char S7[64] = { 4, 13, 11,  0,  2, 11, 14,  7, 15,  4,  0,  9,  8,  1, 13, 10,
					3, 14, 12,  3,  9,  5,  7, 12,  5,  2, 10, 15,  6,  8,  1,  6,
					1,  6,  4, 11, 11, 13, 13,  8, 12,  1,  3,  4,  7, 10, 14,  7,
				   10,  9, 15,  5,  6,  0,  8, 15,  0, 14,  5,  2,  9,  3,  2, 12 };

	char S8[64] = { 13,  1,  2, 15,  8, 13,  4,  8,  6, 10, 15,  3, 11,  7,  1,  4,
				   10, 12,  9,  5,  3,  6, 14, 11,  5,  0,  0, 14, 12,  9,  7,  2,
					7,  2, 11,  1,  4, 14,  1,  7,  9,  4, 12, 10, 14,  8,  2, 13,
					0, 15,  6, 12, 10,  9, 13,  0, 15,  3,  3,  5,  5,  6,  8, 11 };

	__forceinline int32 S_boxes(int64 source) {

		int32 res = 0;

		res = (res << 4) | ((S1[(source >> 42)& M_6])& M_4);
		res = (res << 4) | ((S2[(source >> 36)& M_6])& M_4);
		res = (res << 4) | ((S3[(source >> 30)& M_6])& M_4);
		res = (res << 4) | ((S4[(source >> 24)& M_6])& M_4);
		res = (res << 4) | ((S5[(source >> 18)& M_6])& M_4);
		res = (res << 4) | ((S6[(source >> 12)& M_6])& M_4);
		res = (res << 4) | ((S7[(source >> 6)& M_6])& M_4);
		res = (res << 4) | ((S8[source & M_6]) & M_4);

		return res;
	}

	__forceinline int32 P(int32 source) {

		int32 res = 0;

		res = (res << 1) | ((source >> 16) & 1);
		res = (res << 1) | ((source >> 25) & 1);
		res = (res << 1) | ((source >> 12) & 1);
		res = (res << 1) | ((source >> 11) & 1);

		res = (res << 1) | ((source >> 3) & 1);
		res = (res << 1) | ((source >> 20) & 1);
		res = (res << 1) | ((source >> 4) & 1);
		res = (res << 1) | ((source >> 15) & 1);

		res = (res << 1) | ((source >> 31) & 1);
		res = (res << 1) | ((source >> 17) & 1);
		res = (res << 1) | ((source >> 9) & 1);
		res = (res << 1) | ((source >> 6) & 1);

		res = (res << 1) | ((source >> 27) & 1);
		res = (res << 1) | ((source >> 14) & 1);
		res = (res << 1) | ((source >> 1) & 1);
		res = (res << 1) | ((source >> 22) & 1);

		res = (res << 1) | ((source >> 30) & 1);
		res = (res << 1) | ((source >> 24) & 1);
		res = (res << 1) | ((source >> 8) & 1);
		res = (res << 1) | ((source >> 18) & 1);

		res = (res << 1) | ((source >> 0) & 1);
		res = (res << 1) | ((source >> 5) & 1);
		res = (res << 1) | ((source >> 29) & 1);
		res = (res << 1) | ((source >> 23) & 1);

		res = (res << 1) | ((source >> 13) & 1);
		res = (res << 1) | ((source >> 19) & 1);
		res = (res << 1) | ((source >> 2) & 1);
		res = (res << 1) | ((source >> 26) & 1);

		res = (res << 1) | ((source >> 10) & 1);
		res = (res << 1) | ((source >> 21) & 1);
		res = (res << 1) | ((source >> 28) & 1);
		res = (res << 1) | ((source >> 7) & 1);

		return res;
	}

};
