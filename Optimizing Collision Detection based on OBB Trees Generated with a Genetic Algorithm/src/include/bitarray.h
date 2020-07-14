#ifndef BITARRAY_FILE
#define BITARRAY_FILE

#include "utils.h"

class CBitArray
{
public:
	CBitArray();
	CBitArray(const CBitArray& BA);
  	CBitArray(int size, bool defaultValue=false);
 	~CBitArray();
  
  	bool operator [](int i);
	void operator = (const CBitArray & right);
  	void setBit(int bitPos, bool value);
	void complementBit(int bitPos);
  	void setSize(int size, bool defaultValue=false);
	void swapBits(int bitPos1, int bitPos2);
	void randomChromosome();
	int getDecimal(int bitStart, int bitEnd);
	void print();
	bool getBit(int bitPos);

private:
  	byte* m_array;
  	int m_nBytes;
  	int m_nBits;
  	bool getBitOfChar(int bitPos, byte theChar);
	bool getBitOfBuffer(int bitPos);
  	void changeBitOfChar(int bitPos, bool newValue, byte& theChar);
	void changeBitOfBuffer(int bitPos, bool newValue);
};

#endif	//BITARRAY_FILE
