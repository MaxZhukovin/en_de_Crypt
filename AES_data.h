#pragma once

#define	N_PARTS			2

template <class part> struct AES_STRUCT
{
public:
	part	data_parts[N_PARTS];

	__forceinline AES_STRUCT<part> operator^ (AES_STRUCT<part>& Val)
	{
		AES_STRUCT<part> tmp;
		tmp.data_parts[0] = this->data_parts[0] ^ Val.data_parts[0];
		tmp.data_parts[1] = this->data_parts[1] ^ Val.data_parts[1];
		return tmp;
	}

	__forceinline AES_STRUCT<part>& operator = (AES_STRUCT<part> Val)
	{


		this->data_parts[0] = Val.data_parts[0];
		this->data_parts[1] = Val.data_parts[1];
		
		return *this;
	}
};


#define NB						4
typedef unsigned long long int64;
typedef unsigned int32;
typedef AES_STRUCT<int64> blocktype_aes;

typedef unsigned char	COL_TYPE[NB];

union S_TYPE
{
private:
	int32* S_Cols;
	COL_TYPE* S_Bytes;

public:

	//…………………
	// Методы для доступа к элементам массива состояний

	inline unsigned& operator [] (unsigned ind)
	{
		return S_Cols[ind];
	}

	inline unsigned char& operator () (unsigned n_col, unsigned n_row)
	{
		return S_Bytes[3-n_col][3-n_row];
	}

	__forceinline S_TYPE& operator = (blocktype_aes& Val)
	{
		S_Cols = ((unsigned*)((void*)(&Val)));
		return *this;
	}


};
