#pragma once

//#include "OffLoader.h"
#include "off.h"
#include "bitarray.h"
#include "MyQuaternion.h"
#include <iostream>
#include <vector>
#include <queue>
#include <set>
using namespace std;

#define N_POPULATION 100
#define N_GENERATIONS 150
#define PROB_CROSS 0.80f
#define PROB_MUTATION 0.03f
#define N_BITS 16

#define LIMIT_INFERIOR_VECTOR -1.f
#define LIMIT_SUPERIOR_VECTOR 1.f
#define LIMIT_INFERIOR_W 0.f
#define LIMIT_SUPERIOR_W 360.f
#define PERCENTAGE_STOP	1.00
#define PERCENTAGE_GAP	0.20

class MyOFF
{
public:
	OBB_struct m_boundingBox;			//object oriented baounding box
	void calculateVolume(std::set<f3*> vertex);	//calculate a volume of the OBB
	void setOrientation(CPoint3fv axisX, CPoint3fv axisY, CPoint3fv axisZ);
	OBB_struct& getOrientation();
};


class CGenetic
{
public:
	MyOFF off;
public:
	int m_population;
	int m_ngeneration;
	int m_nbits;
	int m_sizeChromosome;
	float m_probCross;
	float m_probMutation;
	float m_limitInfXYZ;
	float m_limitSupXYZ;
	float m_limitInfW;
	float m_limitSupW;
	
	std::vector<CBitArray> m_chromosomes;
	std::vector<CBitArray> m_newPopulation;
	float* m_fadaptation;		//adaptation function
	float* m_fProb;				//probability
	float* m_facumProb;			//probability acum

	void createPopulation();
	//void runAlgorithm(OFF* off);
	void runAlgorithm(std::set<f3*> vertex);

	CGenetic();
	~CGenetic();

private:
	float evaluationFunction(int indexChromosome, OFF* off);
	float evaluationFunction(int indexChromosome, std::set<f3*> vertex);
	//float setTheBest(float x, float y, float z, float w, OFF* off);
	float setTheBest(float x, float y, float z, float w, std::set<f3*> vertex);
	void getValues(int nChromosome, float& fx, float& fy, float& fz, float& fw);
	void getValues(CBitArray* pArray, int nChromosome, float& fx, float& fy, float& fz, float& fw);
	float getRandomZeroOne();
	void cross(int parent1, int parent2);
	void dumpInfo();	//only for info

	CPoint3fv m_vectorX;
	CPoint3fv m_vectorY;
	CPoint3fv m_vectorZ;
protected:
	//new values add for speed
	float m_increment1;	//represent the increment to multiply x,y,z value in chomosome
	float m_increment2;	//represent the increment to multiply w value in chomosome
};