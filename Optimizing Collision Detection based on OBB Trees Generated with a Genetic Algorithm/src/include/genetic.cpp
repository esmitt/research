#include "genetic.h"
//#include "TakeTime.h"
#include "OBB.h"

void MyOFF::setOrientation(CPoint3fv axisX, CPoint3fv axisY, CPoint3fv axisZ)
{
	m_boundingBox.m_orientationX = axisX;
	m_boundingBox.m_orientationY = axisY;
	m_boundingBox.m_orientationZ = axisZ;
}

OBB_struct& MyOFF::getOrientation()
{
	return m_boundingBox;
}

void MyOFF::calculateVolume(std::set<f3*> vertex)
{
	float u1, u2, u3;
	float l1, l2, l3;
	float r1, r2, r3;
	u1 = u2 = u3 = FLT_MIN;
	l1 = l2 = l3 = FLT_MAX;
	set<f3*>::iterator itVertex = vertex.begin();
	 while(itVertex != vertex.end())
	 {
		 r1 = m_boundingBox.getProjection((*itVertex)[0], AXIS_X);
		if(r1 > u1)
			u1 = r1;
		if(r1 < l1)
			l1 = r1;

		r2 = m_boundingBox.getProjection((*itVertex)[0],AXIS_Y);
		if(r2 > u2)
			u2 = r2;
		if(r2 < l2)
			l2 = r2;

		r3 = m_boundingBox.getProjection((*itVertex)[0],AXIS_Z);
		if(r3 > u3)
			u3 = r3;
		if(r3 < l3)
			l3 = r3; 

		 //std::cout <<  << " " << (*itVertex)[0][1] << " " << (*itVertex)[0][2] << std::endl;
		itVertex++;
	 }

	//for(int i = 0;  i < m_nvertex; i++)
	
	
	m_boundingBox.m_minimumXt = l1;
	m_boundingBox.m_minimumYt = l2;
	m_boundingBox.m_minimumZt = l3;
	m_boundingBox.m_maximumXt = u1;
	m_boundingBox.m_maximumYt = u2;
	m_boundingBox.m_maximumZt = u3;

	m_boundingBox.m_deltaX = u1 - l1;
	m_boundingBox.m_deltaY = u2 - l2;
	m_boundingBox.m_deltaZ = u3 - l3;
	
	//m_boundingBox.m_center = (m_boundingBox.m_orientationX*(l1 + u1)) + (m_boundingBox.m_orientationY*(l2 + u2)) + (m_boundingBox.m_orientationZ*(l3 + u3));
	//m_boundingBox.m_center /= 2.f;
}

CGenetic::CGenetic()
{
	m_population = N_POPULATION;
	m_probCross = PROB_CROSS;
	m_probMutation = PROB_MUTATION;
	m_nbits = N_BITS;
	m_ngeneration = N_GENERATIONS;
	m_limitInfXYZ = LIMIT_INFERIOR_VECTOR;
	m_limitSupXYZ = LIMIT_SUPERIOR_VECTOR;
	m_limitInfW = LIMIT_INFERIOR_W;
	m_limitSupW = LIMIT_SUPERIOR_W;
	m_vectorX.set(1,0,0);
	m_vectorY.set(0,1,0);
	m_vectorZ.set(0,0,1);
	m_sizeChromosome = 0;
	m_fadaptation = NULL;
	m_facumProb = NULL;
	m_fProb = NULL;
	m_chromosomes.clear();
	m_newPopulation.clear();
	//NEW
	//m_increment1 = float(m_limitSupXYZ - m_limitInfXYZ)/float((1<<m_nbits) - 1);
	//m_increment2 = float(m_limitSupW - m_limitInfW)/float((1<<m_nbits) - 1);
	m_increment1 = float(m_limitSupXYZ - m_limitInfXYZ)/float(1<<m_nbits) ;
	m_increment2 = float(m_limitSupW - m_limitInfW)/float(1<<m_nbits);
}

CGenetic::~CGenetic()
{
	DELETE_ARRAY(m_fadaptation);
	DELETE_ARRAY(m_facumProb);
	DELETE_ARRAY(m_fProb);
	m_chromosomes.clear();
	m_newPopulation.clear();
}

void CGenetic::getValues(int nChromosome, float& fx, float& fy, float& fz, float& fw)
{
	int x, y, z, w;
	int nbitsx2 = (m_nbits<<1);
	///m_chromosomes[nChromosome].print();
	x = m_chromosomes[nChromosome].getDecimal(0, m_nbits - 1);
	y = m_chromosomes[nChromosome].getDecimal(m_nbits, nbitsx2 - 1);
	z = m_chromosomes[nChromosome].getDecimal(nbitsx2, (nbitsx2 + m_nbits) - 1);
	w = m_chromosomes[nChromosome].getDecimal((nbitsx2 + m_nbits), (nbitsx2 + nbitsx2) - 1);
	
	// apply the formule [a,b]
	// (b-a) / (nBits - 1)
	
	//float inc;
	//inc = float(m_limitSupXYZ - m_limitInfXYZ)/float((1<<m_nbits) - 1);
	// NEW replace inc by m_increment1
	fx = m_limitInfXYZ + x*m_increment1;
	fy = m_limitInfXYZ + y*m_increment1;
	fz = m_limitInfXYZ + z*m_increment1;
	// NEW replace inc by m_increment2
	//inc = float(m_limitSupW - m_limitInfW)/float((1<<m_nbits) - 1);	
	fw = m_limitInfW + z*m_increment2;
}

void CGenetic::dumpInfo()
{
	/*
	printf("Number of Population = %d\n",m_population);
	printf("Number max of Generations = %d\n",m_ngeneration);
	printf("Number of bits = %d\n",m_nbits);
	printf("Size of chromosome = %d\n",m_sizeChromosome);
	printf("Probability of cross = %f\n",m_probCross);
	printf("Probability of mutation = %f\n",m_probMutation);
	*/
}

void CGenetic::createPopulation()
{
	m_sizeChromosome = m_nbits<<2;
	m_fadaptation = new float[m_population];
	m_facumProb = new float[m_population + 1];
	m_fProb = new float[m_population];
	//m_chromosomes = new CBitArray[m_population];
	//m_newPopulation = new CBitArray[m_population];
	m_chromosomes.resize(m_population);
	dumpInfo();

	for(int i = 0; i < m_population; i++)
	{
		m_chromosomes[i].setSize(m_sizeChromosome);
		//m_newPopulation[i].setSize(m_sizeChromosome);
		m_chromosomes[i].randomChromosome();	//create a randompopulation de ONE!!!
	}
	/*
	float x, y, z, w;
	getValues(1, x, y, z, w);
	printf("%f %f %f %f\n",x,y,z,w);
	*/
}
/*
float CGenetic::evaluationFunction(int indexChromosome, COffLoader* off)
{
	Quaternion q;
	float x, y, z, w;
	// take the valueof of the i-Chromosome
	getValues(indexChromosome, x, y, z, w);
	// set to quaternion
	q.setQuaternion(x, y, z, w);
	//rotate the 3-vector --> (1,0,0) (0,1,0) (0,0,1) and set the new orientation for the OBB
	//off->setOrientation(q.getPointRotated(m_vectorX), q.getPointRotated(m_vectorY), q.getPointRotated(m_vectorZ));
	off->setOrientation(q.getPointRotated(AXIS_X), q.getPointRotated(AXIS_Y), q.getPointRotated(AXIS_Z));
	//calculate the value of the volume
	off->calculateVolume();
	//return the volume value
	return off->m_boundingBox.getVolume();
}
*/

//float CGenetic::evaluationFunction(int indexChromosome, OFF* off)
float CGenetic::evaluationFunction(int indexChromosome, std::set<f3*> vertex)
{
	MyQuaternion q;
	float x, y, z, w;
	// take the valueof of the i-Chromosome
	getValues(indexChromosome, x, y, z, w);
	// set to quaternion
	q.setQuaternion(x, y, z, w);
	//rotate the 3-vector --> (1,0,0) (0,1,0) (0,0,1) and set the new orientation for the OBB
	//off->setOrientation(q.getPointRotated(m_vectorX), q.getPointRotated(m_vectorY), q.getPointRotated(m_vectorZ));
	//off->setOrientation(q.getPointRotated(AXIS_X), q.getPointRotated(AXIS_Y), q.getPointRotated(AXIS_Z));
	//off->setOrientation(q.getPointRotated(AXIS_X), q.getPointRotated(AXIS_Y), q.getPointRotated(AXIS_Z));
	off.setOrientation(q.getPointRotated(AXIS_X), q.getPointRotated(AXIS_Y), q.getPointRotated(AXIS_Z));
	//calculate the value of the volume
	off.calculateVolume(vertex);
	//return the volume value
	return off.m_boundingBox.getVolume();
}


float CGenetic::setTheBest(float x, float y, float z, float w, std::set<f3*> vertex)
{
	MyQuaternion q;
	// set to quaternion
	q.setQuaternion(x, y, z, w);
	//rotate the 3-vector --> (1,0,0) (0,1,0) (0,0,1) and set the new orientation for the OBB
	//off->setOrientation(q.getPointRotated(m_vectorX), q.getPointRotated(m_vectorY), q.getPointRotated(m_vectorZ));
	off.setOrientation(q.getPointRotated(AXIS_X), q.getPointRotated(AXIS_Y), q.getPointRotated(AXIS_Z));
	//calculate the value of the volume
	off.calculateVolume(vertex);
	//return the volume value
	return off.m_boundingBox.getVolume();
}
class Individual
{
private:
	float m_fFitness;
	int m_iIndexChromosome;
public:
	Individual();
	Individual(float fFitness, int iIndex);
	~Individual();
	bool operator<(const Individual& right) const;
	void operator=(const Individual& right);
	int GetIndex();
	float GetFitness();
	void SetValues(float fFitness, int iIndex);
	void print();
};

Individual::Individual()
{
	m_fFitness = FLT_MAX;
	m_iIndexChromosome = -1;
}

Individual::Individual(float fFitness, int iIndex)
{
	m_fFitness = fFitness;
	m_iIndexChromosome = iIndex;
}

void Individual::SetValues(float fFitness, int iIndex)
{
	m_fFitness = fFitness;
	m_iIndexChromosome = iIndex;
}

int Individual::GetIndex()
{
	return m_iIndexChromosome;
}

float Individual::GetFitness()
{
	return m_fFitness;
}

Individual::~Individual()
{

}

bool Individual::operator<(const Individual& right) const
{
	return (this->m_fFitness < right.m_fFitness);
}

void Individual::operator=(const Individual& right)
{
	SetValues(right.m_fFitness, right.m_iIndexChromosome);
}

void Individual::print()
{
	///cout << m_iIndexChromosome << ": " << m_fFitness << endl;
}


//void CGenetic::runAlgorithm(OFF* off)
void CGenetic::runAlgorithm(std::set<f3*> vertex)
{
	int i, ngenerations, limit;
	int iPopToReplace = (int)(m_population*PERCENTAGE_GAP);
	double random;
	float adaptTotal = 0.f;
	priority_queue<Individual> Queue;
	Individual theBetter;
	Individual theBest;
	float x, y, z, w;
	//iPopToReplace ALWAYS is par
	if(iPopToReplace&1 && iPopToReplace < m_population)
	{
		iPopToReplace--;
		if(iPopToReplace<0)iPopToReplace = 0;
	}

	//step 1: Create a population = create chromosomes
	//printf("Create a population...\n");
	createPopulation();
	
	//instance of Math class
	CMath* pMath = CMath::getInstance();

	//until all generations
	ngenerations = 0;
	while(ngenerations < m_ngeneration)
	{
		//step 2: Evaluate a adaptation functions
		adaptTotal = 0;
		for(i = 0; i < m_population; i++)
		{
			//m_fadaptation[i] = 1.f/evaluationFunction(i, off);
			m_fadaptation[i] = 1.f/evaluationFunction(i, vertex);
			Queue.push(Individual(m_fadaptation[i], i));
			adaptTotal += m_fadaptation[i];
		}
		//cout << Queue.size() << endl;
		//step 3: Selection x wheel
		m_facumProb[0] = 0.f;
		for(i = 0; i < m_population; i++)
		{
			m_fProb[i] = m_fadaptation[i]/adaptTotal;
			if(i > 0)
				m_facumProb[i] = m_facumProb[i - 1] + m_fProb[i];
		}
		m_facumProb[m_population] = 1.f;

		// gap generacional
		// for the next generation, pass direct the m_population - iPopToReplace individual (the better)
		limit = m_population - iPopToReplace;
		theBest.SetValues(1.f/Queue.top().GetFitness(), 0);
		for(i = 0; i < limit; i++)
		{
			m_newPopulation.push_back(m_chromosomes[Queue.top().GetIndex()]);
			Queue.pop();
		}
		while(!Queue.empty())Queue.pop();
		
		int k1, k2;
		int iNewInd = 0;
		while(iNewInd < iPopToReplace)
		{
			k1 = 0;
			k2 = 0;
			random = pMath->getRandom();
			while(true)
			{
				if(random < m_facumProb[k1])
					break;
				if(k1 == m_population-1) break;
				k1++;
			}
			random = pMath->getRandom();

			while(true)
			{
				if(random < m_facumProb[k2])
					break;
				if(k2 == m_population-1) break;
				k2++;
			}
			
			//step 4: cross k1 individual & k2
			random = pMath->getRandom();
			if(random < m_probCross)
				cross(k1, k2); //select a parent #2 and cross, pass the position inside the population
			else
			{
				m_newPopulation.push_back(m_chromosomes[k1]);
				m_newPopulation.push_back(m_chromosomes[k2]);
			}
			
			iNewInd+=2;
			//step 5: aplicar mutacion
			for(i = 0; i < m_sizeChromosome; i++)
			{
				if(pMath->getRandom() < PROB_MUTATION)
					m_newPopulation[m_newPopulation.size()-2].complementBit(i);
				if(pMath->getRandom() < PROB_MUTATION)
					m_newPopulation[m_newPopulation.size()-1].complementBit(i);
			}
		}
		
		float v;

		//step 6: Replace a population for the new
		m_chromosomes.clear();
		for(i = 0; i < m_population; i++)
		{
			m_chromosomes.push_back(m_newPopulation[i]);
			if(i >= limit)
			{
				v = evaluationFunction(i, vertex);
				if(v < theBest.GetFitness())
					theBest.SetValues(v, i);
			}
		}
		m_newPopulation.clear();
		
		//save the best of the all generations
		if(theBest < theBetter)
		{
			theBetter = theBest;
			///getValues(theBetter.GetIndex(), x, y, z, w);
		}
		
		//step 7: increment the number of generations
		ngenerations++;
		///printf("Generation #%d , Volume = %f\n",ngenerations, theBest.GetFitness());
	}
	
	//end get the time

	//the best individual is: m_chromosomes[indexBetterVol] !!!!
	//printf("Best Volume = %f\n",setTheBest(x, y, z, w, vertex));
	///printf("%3d = Best Volume = %f\n",global++, theBetter.GetFitness());
	//printf("Best FAdaptation = %f\n",fBetterfAdaptation);
	//printf("I finished\n");
}

/*
void CGenetic::runAlgorithm(COffLoader* off)
{
	int i, k, ngenerations;
	int indexSelected;
	int pos1, pos2;
	float maximum;
	double random;
	float adaptTotal = 0.f, adaptProm;
	int* selectedInd = new int[m_population];	//array of the selected individual
	bool flag;
	
	//for save the Best indidual of the i-generation and the best individual of ALL generation
	float bestVol;
	int indexBestVol;
	float theBetterVol;
	int indexBetterVol;
	float fBetterfAdaptation;

	// count the number of individual that are upon the average
	int uponAverage;

	float x, y, z, w;	//The best values!!!

	//this is why scale is the MAX value of the side to a object
	maximum = 1.f/off->m_scale;
	maximum = maximum*maximum*maximum;	//3 times.. 1 for each axis
	theBetterVol = maximum;	

	//step 1: Create a population = create chromosomes
	printf("Create a population...\n");
	createPopulation();
	
	//instance of Math class
	CMath* pMath = CMath::getInstance();

	// begin get the time
	
	//int ntime = 0;
	//LARGE_INTEGER ntime1, ntime2;
	//LARGE_INTEGER freq;
	//QueryPerformanceFrequency(&freq);
	//QueryPerformanceCounter(&ntime1);

	//CTakeTime::getInstance()->startTime();
	//until all generations
	ngenerations = 0;
	while(ngenerations < m_ngeneration)
	{
		//step 2: Evaluate a adaptation functions
		//CTakeTime::getInstance()->startTime();
		adaptTotal = 0;
		for(i = 0; i < m_population; i++)
		{
			m_fadaptation[i] = maximum - evaluationFunction(i, off);
			adaptTotal += m_fadaptation[i];
		}
		adaptProm = adaptTotal/float(m_population);
		//CTakeTime::getInstance()->endTimePrint();
		//step 3: Selection x wheel
		// calculate of prob + acum
		m_facumProb[0] = 0.f;
		uponAverage = 0;
		for(i = 0; i < m_population; i++)
		{
			if(m_fadaptation[i] > adaptProm)uponAverage++;	//only to count

			m_fProb[i] = m_fadaptation[i]/adaptTotal;
			if(i > 0)
				m_facumProb[i] = m_facumProb[i - 1] + m_fProb[i];
		}
		m_facumProb[m_population] = 1.f;
		
		// Condition of stop the algorithm. If the PERCENTAGE_STOP% of the population is > at the average then STOP
		// at least exist 1 generation
		if((ngenerations > 0) && (uponAverage >= ((int)m_population*PERCENTAGE_STOP)))
			break;
		
		//change the form of select a individual to cross
		for(i = 0; i < m_population; i++)
		{
			//generate a random number
			//random = CMath::getInstance()->getRandom();
			random = pMath->getRandom();
			for(k = 1; k < m_population + 1; k++)
			{
				if(random < m_facumProb[k])
				{
					selectedInd[i] = k-1;	//save the index of the individual
					break;
				}
			}
		}
		// in the array selected are the index of the all individual selected = m_population
		
		//step 4: cross
		flag = true;
		for(i = 0; i < m_population; i++)
		{
			//random = CMath::getInstance()->getRandom();
			random = pMath->getRandom();
			if(random < m_probCross)
			{
				if(flag == true)
				{
					flag = false;
					indexSelected = selectedInd[i];		//select a parent #1
					pos1 = i;
				}
				else
				{
					flag = true;
					pos2 = i;
					cross(indexSelected, selectedInd[i], pos1, pos2); //select a parent #2 and cross, pass the position inside the population
				}
			}
			else
			{
				m_newPopulation[i] = m_chromosomes[i];
			}
		}


		//if exist a parent alone !!! :( poor chromosome..hehehee
		if(flag == false)
			m_newPopulation[pos1] = m_chromosomes[indexSelected];

		//step 5: Mutation
		//for all gen in the population
		for(i = 0; i < m_population; i++)
		{
			//// begin version original (comentado)
			for(k = 0; k < m_sizeChromosome; k++)
			{
				random = CMath::getInstance()->getRandom();
				if(random < PROB_MUTATION)
				{
					//mutate a bit
					m_newPopulation[i].complementBit(k);
				}
			}
			//// end version original

			//new version - BEGIN
			//random = CMath::getInstance()->getRandom();
			random = pMath->getRandom();
			if(random < PROB_MUTATION)
			{
				//mutate a bit
				//m_newPopulation[i].complementBit(CMath::getInstance()->getRandomFromZeroTo(m_sizeChromosome));
				m_newPopulation[i].complementBit(pMath->getRandomFromZeroTo(m_sizeChromosome));
			}
			//new version - END
		}
		
		bestVol = maximum;
		float v;
		//step 6: Replace a population for the new
		for(i = 0; i < m_population; i++)
		{
			m_chromosomes[i] = m_newPopulation[i];
			v = evaluationFunction(i, off);
			if(v < bestVol)
			{
				bestVol = v;
				indexBestVol = i;
			}
		}

		//save the best of the all generations!!!
		if(bestVol < theBetterVol)
		{
			theBetterVol = bestVol;
			indexBetterVol = indexBestVol;
			fBetterfAdaptation = m_fadaptation[indexBestVol];
			getValues(indexBetterVol, x, y, z, w);
		}
		//printf("%d,%d\n",ngenerations, ((uponAverage)*100)/m_population);
		//step 7: increment the number of generations
		ngenerations++;
		printf("Generation #%d , Volume = %f, FAdaptation = %f, uponAverage = %d\n",ngenerations, bestVol, m_fadaptation[indexBestVol],uponAverage);
	}
	
	//end get the time
	//CTakeTime::getInstance()->endTime();

	//the best individual is: m_chromosomes[indexBetterVol] !!!!
	printf("Best Volume = %f\n",setTheBest(x, y, z, w, off));
	printf("Best FAdaptation = %f\n",fBetterfAdaptation);
	printf("I finished\n");
}
*/

/// @param positionParent1 is the position of the first parent
/// @param positionParent2 is the position of the second parent
void CGenetic::cross(int parent1, int parent2)
{
	//generate a random number between [1, L - 1], where L is the chromosome lenght
	int index = (int)((CMath::getInstance()->getRandomFromZeroTo(m_sizeChromosome - 2)) + 1);
	bool bp1, bp2;	//bit parent1, bit parent 2
	CBitArray* p1,* p2;
	p1 = new CBitArray(m_sizeChromosome);
	p2 = new CBitArray(m_sizeChromosome);

	for(int i = 0; i < m_sizeChromosome; i++)
	{
		//take the bits of the parents
		bp1 = m_chromosomes[parent1].getBit(i);
		bp2 = m_chromosomes[parent2].getBit(i);
		
		if(i <= index)
		{
			//first index-bits
			p1->setBit(i, bp1);
			p2->setBit(i, bp2);
		}
		else
		{
			//last (m_sizeChromosome - index)-bits
			p1->setBit(i, bp2);
			p2->setBit(i, bp1);
		}
	}

	m_newPopulation.push_back(*p1);
	m_newPopulation.push_back(*p2);
	delete p1;
	delete p2;
}