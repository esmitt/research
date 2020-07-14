#include "bitarray.h"

CBitArray::CBitArray()
{
  m_array = NULL;
  m_nBytes = m_nBits = 0;
}

CBitArray::CBitArray(const CBitArray& BA)
{
	m_nBytes = BA.m_nBytes;
	m_nBits = BA.m_nBits;
	if(m_nBytes > 0)
	{
		m_array = new byte[m_nBytes];
		for(int i = 0; i < m_nBytes; i++)
			this->m_array[i] = BA.m_array[i];
	}
}

void CBitArray::operator = (const CBitArray & right)
{
	m_nBytes = right.m_nBytes;
	m_nBits = right.m_nBits;
	for(int i = 0; i < m_nBytes; i++)
		this->m_array[i] = right.m_array[i];

}

CBitArray::CBitArray(int size, bool defaultValue)
{
  m_array = NULL;
  setSize(size, defaultValue);
}

CBitArray::~CBitArray()
{
	DELETE_ARRAY(m_array);
}
/// Retorna el bit <bitPos> del byte  <theChar>. El ordenamiento de los bits es:
///    ---------------
///   |0|1|2|3|4|5|6|7|
///    ---------------
bool CBitArray::getBitOfChar(int bitPos, byte theChar)
{
  theChar = theChar << bitPos;
  return(theChar >= 0x80);	// 0x80 = 10000000b = 128d
}

bool CBitArray::getBit(int bitPos)
{
	return getBitOfChar(bitPos & 0x0007, m_array[bitPos >> 3]);
}

bool CBitArray::getBitOfBuffer(int bitPos)
{
	return getBitOfChar(bitPos & 0x0007, m_array[bitPos >> 3]);
}

/// Cambia un bit de un caracter
void CBitArray::changeBitOfChar(int bitPos, bool newValue, byte& theChar)
{
  byte aux;
  if (newValue == true)
  {
    aux = 0x80 >> bitPos;
    theChar  |= aux;
  }
  else
  {
    aux = 0xFF^(0x80 >> bitPos);
    theChar  &= aux;
  }
}

void CBitArray::changeBitOfBuffer(int bitPos, bool newValue)
{
  changeBitOfChar(bitPos & 0x0007, newValue, m_array[bitPos >> 3]);
}

bool CBitArray::operator [] (int bitPos)
{
  return getBitOfChar(bitPos & 0x0007, m_array[bitPos >> 3]);
}

void CBitArray::setBit(int bitPos, bool value)
{
  changeBitOfChar(bitPos & 0x0007, value, m_array[bitPos >> 3]);
}

void CBitArray::setSize(int size, bool defaultValue)
{
  //DELETE_ARRAY(m_array);
  m_nBits = size;
  m_nBytes= size/8;
  if (size % 8) m_nBytes++;
  m_array = new byte[m_nBytes];
  memset(m_array, defaultValue, m_nBytes);
}

void CBitArray::randomChromosome()
{
	int v;
	for(int i = 0; i < m_nBits; i++)
	{
		//v = rand()%2;
		//setBit(i, (v == 1)?true:false);
		if(CMath::getInstance()->getRandom() < 0.5)
			setBit(i, true);
		else
			setBit(i, false);
	}
}

void CBitArray::print()
{
	/*
	for(int i = 0; i < m_nBits;i++)
		printf("%d",getBitOfBuffer(i));
	*/
	for(int i = m_nBits - 1; i >= 0;i--)
		printf("%d",getBitOfBuffer(i));
	printf("\n");
}

void CBitArray::complementBit(int bitPos)
{
	setBit(bitPos, !getBitOfBuffer(bitPos));
}

///
/// Obtiene el valor decimal de los bits contenidos entre bitStart y bitEnd
///
int CBitArray::getDecimal(int bitStart, int bitEnd)
{
	int r = 0, i;
	for(i = bitStart; i <= bitEnd; i++)
		if(getBitOfBuffer(i) == BIT_ONE)
			r+= (1<<(i - bitStart));
	return r;
}

void CBitArray::swapBits(int bitPos1, int bitPos2)
{
	bool bi, bj;

  	bi = getBitOfBuffer(bitPos1);
  	bj = getBitOfBuffer(bitPos2);
  	changeBitOfBuffer(bitPos1, bj);
  	changeBitOfBuffer(bitPos2, bi);
}
