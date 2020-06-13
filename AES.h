#pragma once

#include"Algorithm.h"
#include"AES_data.h"

template<class block>
class AES:
	public Algorithm<block>
{

private:

	AES_key* key_manager;
	S_TYPE   data;

	int32(AES::*mix_column)(int32&);
	void(AES::*shift_rows)(int32&, int32&, int32&);
	unsigned char* S_box;

	int step;
	unsigned init_pos;

public:


	__forceinline block act(block &source) {

		 register unsigned r = init_pos;
		 data = source;


		invert();

		/* for (int i = 0; i < 4; i++)
			 std::cout << std::hex << data[i] << std::endl;*/
		 addroundkeys(r);

		 for (register int i = 0; i < key_manager->get_rounds()-1; i++)
		 {
			 r += step;
			 subbyte();
			 ShiftRows();
			 mixcolumns();
			 addroundkeys(r);
		 }

		 subbyte();
		 ShiftRows();
		 addroundkeys(r+step);
		 
		invert();
		 
		/* for (int i = 0; i < 4; i++)
			 std::cout << std::hex << data[i] << std::endl;*/

		 return source;
	 };

	 void init(void* manager) override {

		 key_manager = (AES_key*)manager;


		 for (int i = 0; i <= 256; i++)
		 {
			 gfm255x02[i] = gf_mult(i, 2);
			 gfm255x03[i] = gf_mult(i, 3);
			 gfm255x0e[i] = gf_mult(i, 0xe);
			 gfm255x0b[i] = gf_mult(i, 0xb);
			 gfm255x0d[i] = gf_mult(i, 0xd);
			 gfm255x09[i] = gf_mult(i, 9);
		 }
		 
		 S_box = key_manager->get_mode() ? AES_FUN::InvSbox : AES_FUN::Sbox;
		 this->mix_column = key_manager->get_mode() ? &AES::inv_mix : &AES::mix;
		 this->shift_rows = key_manager->get_mode() ? &AES::invshift : &AES::shift;

		 if (key_manager->get_mode()) 
			 for (int i = key_manager->get_nk(); i < key_manager->get_col() - 4; i++)
				 key_manager->get_key(i) = inv_mix(key_manager->get_key(i));

		 step = key_manager->get_mode() ? -1 : +1;
		 init_pos = key_manager->get_mode() ? key_manager->get_rounds() : 0;
	 };

private:

	__forceinline void invert() {
		register int32  s = data[0], s2 = data[1], s3 = data[2], s4 = data[3];
		register int32  res = s, res2 = s2, res3 = s3, res4 = s4;

		res = (res << 8) | ((s >> 8)& M_8);
		res = (res << 8) | ((s >> 16)& M_8);
		res = (res << 8) | ((s >> 24)& M_8);

		res2 = (res2 << 8) | ((s2 >> 8)& M_8);
		res2 = (res2 << 8) | ((s2 >> 16)& M_8);
		res2 = (res2 << 8) | ((s2 >> 24)& M_8);

		res3 = (res3 << 8) | ((s3 >> 8)& M_8);
		res3 = (res3 << 8) | ((s3 >> 16)& M_8);
		res3 = (res3 << 8) | ((s3 >> 24)& M_8);

		res4 = (res4 << 8) | ((s4 >> 8)& M_8);
		res4 = (res4 << 8) | ((s4 >> 16)& M_8);
		res4 = (res4 << 8) | ((s4 >> 24)& M_8);

		data[0] = res;  data[1] = res2;  data[2] = res3; data[3] = res4;
	}

	__forceinline void subbyte() {

		data[0] = AES_FUN::SubWord(data[0], S_box);
		data[1] = AES_FUN::SubWord(data[1], S_box);
		data[2] = AES_FUN::SubWord(data[2], S_box);
		data[3] = AES_FUN::SubWord(data[3], S_box);
	}

	__forceinline void mixcolumns() {

		data[0] = (this->*mix_column)(data[0]);
		data[1] = (this->*mix_column)(data[1]);
		data[2] = (this->*mix_column)(data[2]);
		data[3] = (this->*mix_column)(data[3]);
	}

	__forceinline void addroundkeys( unsigned round) {
		
		data[0] ^= key_manager->get_key(round * 4);
		data[1] ^= key_manager->get_key(round * 4 + 1);
		data[2] ^= key_manager->get_key(round * 4 + 2);
		data[3] ^= key_manager->get_key(round * 4 + 3);
	}

	__forceinline void ShiftRows() {

		register unsigned char 
		row2[4] = { data(0, 1), data(1, 1), data(2, 1), data(3, 1) },
		row3[4] = { data(0, 2), data(1, 2), data(2, 2), data(3, 2) },
		row4[4] = { data(0, 3), data(1, 3), data(2, 3), data(3, 3) };

		register int32 r2 = *((int32*)row2);
		register int32 r3 = *((int32*)row3);
		register int32 r4 = *((int32*)row4);

		(this->*shift_rows)(r2,r3,r4);

		data(3, 1) = r2 >> 24; data(2, 1) = r2 >> 16; data(1, 1) = r2 >> 8; data(0, 1) = r2;
		data(3, 2) = r3 >> 24; data(2, 2) = r3 >> 16; data(1, 2) = r3 >> 8; data(0, 2) = r3;
		data(3, 3) = r4 >> 24; data(2, 3) = r4 >> 16; data(1, 3) = r4 >> 8; data(0, 3) = r4;
	}





private:
	__forceinline void shift(int32 &r2, int32 &r3, int32 &r4) {

		r2 = (r2 << 8) | ((r2 >> 24)& M_8);
		r3 = (r3 << 16) | ((r3 >> 16)& M_16);
		r4 = (r4 << 24) | ((r4 >> 8)& M_24);
	}
	__forceinline void invshift(int32& r2, int32& r3, int32& r4) {

		r2 = (r2 << 24) | ((r2 >> 8)& M_24);
		r3 = (r3 << 16) | ((r3 >> 16)& M_16);
		r4 = (r4 << 8) | ((r4 >> 24)& M_8);
	}

	__forceinline int32 mix(int32 &source) {
		
		register int32 res = 0;
		unsigned char s4 = source, s3 = source >> 8, s2 = source >> 16, s1 = source >> 24;

		res = gfm255x02[s1] ^ gfm255x03[s2] ^ s3 ^ s4;
		res = (res << 8) | ((s1 ^ gfm255x02[s2] ^ gfm255x03[s3] ^ s4) & M_8);
		res = (res << 8) | ((s1 ^ s2 ^ gfm255x02[s3] ^ gfm255x03[s4]) & M_8);
		res = (res << 8) | ((gfm255x03[s1] ^ s2 ^ s3 ^ gfm255x02[s4]) & M_8);

		return res;
	}

	__forceinline int32 inv_mix(int32 &source) {

		register int32 res = 0;
		unsigned char s4 = source, s3 = source >> 8, s2 = source >> 16, s1 = source >> 24;

		res = gfm255x0e[s1] ^ gfm255x0b[s2] ^ gfm255x0d[s3] ^ gfm255x09[s4];
		res = (res << 8) | ((gfm255x09[s1] ^ gfm255x0e[s2] ^ gfm255x0b[s3] ^ gfm255x0d[s4]) & M_8);
		res = (res << 8) | ((gfm255x0d[s1] ^ gfm255x09[s2] ^ gfm255x0e[s3] ^ gfm255x0b[s4]) & M_8);
		res = (res << 8) | ((gfm255x0b[s1] ^ gfm255x0d[s2] ^ gfm255x09[s3] ^ gfm255x0e[s4]) & M_8);

		return res;
	}

	__forceinline unsigned char gf_mult(unsigned char first, unsigned char second) {
		
		unsigned char r = (first & 1) == 1 ? second : 0, mask = 2;
		unsigned rt = second;

		for (int i = 1; i <= 7; i++)
		{		
			rt = rt << 1;
			
			if (rt > 0xff)
				rt = rt ^ 0x011b;

			if ((first & mask) != 0)
				r = r ^ rt;

			mask <<= 1;
		}

		return r;
	}

	unsigned char gfm255x02[256];
	unsigned char gfm255x03[256];

	unsigned char gfm255x0e[256];
	unsigned char gfm255x0b[256];
	unsigned char gfm255x0d[256];
	unsigned char gfm255x09[256];
};