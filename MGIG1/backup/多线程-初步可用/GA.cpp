#pragma once
#include<head.h>
#include<GA.h>

extern set<string> airFurnaceSet;


GeneticAlgorithm::GeneticAlgorithm(int numOfJobRange, int numPop, int totalGenerati, int totalLen) :
	m_numOfJobRangeOfGA(numOfJobRange), m_numOfPop(numPop), m_totalGeneration(totalGenerati), m_totalLenOfGACode(totalLen)
{
	this->m_codeInfoOfGA = { vector<pair<Job*, pair<int, int>>>(numOfJobRange) };
	this->m_popPool = { vector<Chromosome*>(numPop) };

	// 初始化交叉个数和变异个数
	this->m_numForCrossOver = this->m_numOfPop * this->m_crossRate;
	if (this->m_numForCrossOver % 2 == 1) this->m_numForCrossOver++;
	this->m_numForMutation = this->m_numOfPop * this->m_mutatRate;

	// 初始化数组（分配内存），存放交叉个体和变异个体
	this->m_childsForCroOver = { vector<Chromosome*>(m_numForCrossOver) };
	this->m_childsForMutation = { vector<Chromosome*>(m_numForMutation) };

	this->m_popPoolNew = { vector<Chromosome*>(numPop + this->m_numForCrossOver + this->m_numForMutation) };

	// 初始化精英
	this->m_numElites = this->m_eliteRate * this->m_numOfPop;

};

void GeneticAlgorithm::initializePopulation(Chromosome* chromPInit, Chromosome* chromPInit2) {
	const int totalLenOfGACode = this->m_totalLenOfGACode;
	const int numOfPop = this->m_numOfPop;
	const int numOfJobRangeOfGA = this->m_numOfJobRangeOfGA;
	const int numOfParentsForCroOver = this->m_numForCrossOver;
	const int numOfParentsForMutation = this->m_numForMutation;
	vector<Chromosome*>& popPool = this->m_popPool;
	vector<Chromosome*>& childsForCroOver = this->m_childsForCroOver;
	vector<Chromosome*>& childsForMutation = this->m_childsForMutation;
	set<Chromosome*>& totalChromsSet = this->m_totalChromsSet;

	//popPool[0] = chromPInit;
	//popPool[1] = chromPInit2;

	// 排产结果1构成初始解
	for (int i = 0; i < 1; ++i) {
		Chromosome* chromPInitnew = new Chromosome(totalLenOfGACode, numOfJobRangeOfGA);
		popPool[i] = chromPInitnew;
		chromPInitnew->code.assign(chromPInit->code.begin(), chromPInit->code.end());
	}

	// 排产结果2构成初始解
	for (int i = 1; i < 2; ++i) {
		Chromosome* chromPInitnew = new Chromosome(totalLenOfGACode, numOfJobRangeOfGA);
		popPool[i] = chromPInitnew;
		chromPInitnew->code.assign(chromPInit2->code.begin(), chromPInit2->code.end());
	}

	// 随机生成初始解
	for (int i_chrom = 2; i_chrom < 70; ++i_chrom) {
		Chromosome* chromPInitnew = new Chromosome(totalLenOfGACode, numOfJobRangeOfGA);
		popPool[i_chrom] = chromPInitnew;
		for (int i = 0; i < totalLenOfGACode; ++i) {
			int indexOfUndefined = rand() % (totalLenOfGACode - i) + 1;
			int index = 0;
			int j = 0;
			for (; index < indexOfUndefined; ++index, ++j) {
				if (chromPInitnew->code[j] >= 0) --index;
			}
			chromPInitnew->code[--j] = chromPInit->code[i];
		}
	}

	// 由初始排产结果生成解
	double rate = 0.1;
	int numOfMove = totalLenOfGACode * rate;
	for (int i_chrom = 70; i_chrom < numOfPop; ++i_chrom) {
		Chromosome* chromPInitnew = new Chromosome(totalLenOfGACode, numOfJobRangeOfGA);
		popPool[i_chrom] = chromPInitnew;
		popPool[i_chrom]->code.assign(chromPInit->code.begin(), chromPInit->code.end());
		for (int i = 0; i < numOfMove; ++i) {
			int left = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
			int right = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
			swap(popPool[i_chrom]->code[left], popPool[i_chrom]->code[right]);
		}
	}


	// 交叉
	for (int i_chrom = 0; i_chrom < numOfParentsForCroOver; ++i_chrom) {
		Chromosome* chromPInitnew = new Chromosome(totalLenOfGACode, numOfJobRangeOfGA);
		childsForCroOver[i_chrom] = chromPInitnew;
	}

	// 变异
	for (int i_chrom = 0; i_chrom < numOfParentsForMutation; ++i_chrom) {
		Chromosome* chromPInitnew = new Chromosome(totalLenOfGACode, numOfJobRangeOfGA);
		childsForMutation[i_chrom] = chromPInitnew;
	}

	totalChromsSet.insert(popPool.begin(), popPool.end());
	totalChromsSet.insert(childsForCroOver.begin(), childsForCroOver.end());
	totalChromsSet.insert(childsForMutation.begin(), childsForMutation.end());

}

void GeneticAlgorithm::getNewChrome(Chromosome* chromPInit, Chromosome* chromNew) {
	const int totalLenOfGACode = this->m_totalLenOfGACode;
	double rate = 0.02;
	int numOfMove = totalLenOfGACode * rate;

	chromNew->code.assign(chromPInit->code.begin(), chromPInit->code.end());
	for (int i = 0; i < numOfMove; ++i) {
		int left = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
		int right = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
		swap(chromNew->code[left], chromNew->code[right]);
	}

};

void GeneticAlgorithm::initializeInfoOfBef(vector<pair<string, Job*>>* jobOrderBefAirP, map<string, Job*>* jobsMapBefAirP, map<string, Mach*>* machsMapBefAirP) {
	this->m_jobOrderBefAirP = jobOrderBefAirP;
	this->m_jobsMapBefAirP = jobsMapBefAirP;
	this->m_machsMapBefAirP = machsMapBefAirP;

	// 用来解码，计算染色体适值
	this->m_jobOrderTmp2 = { vector<pair<string, Job*>>(this->m_jobOrderBefAirP->size()) };
	initJobsTemp(this->m_jobsMapTmp2, this->m_jobOrderTmp2, *(this->m_jobOrderBefAirP));
	initMachsMapTemp(this->m_machsMapTmp2, *(this->m_machsMapBefAirP));
}

void GeneticAlgorithm::initObjValsOfGAPop()
{
	int numOfPop = this->m_numOfPop;
	int totalLenOfGACode = this->m_totalLenOfGACode;
	vector<Chromosome*>& popPool = this->m_popPool;


	cout << "tianmin:" << endl;
	for (int i = 0; i < popPool[0]->codeLen; ++i) {
		cout << popPool[0]->code[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < popPool[10]->codeLen; ++i) {
		cout << popPool[10]->code[i] << " ";
	}


	cout << "  popPool.size()=" << popPool.size() << endl;
	for (int num_Chro = 0; num_Chro < numOfPop; ++num_Chro) {
		cout << "    num_Chro=" << num_Chro << endl;
		Chromosome* chromP = popPool[num_Chro];
		// 排退火炉之后的工序
		pair<double, double> objectValues = getObjectValuesOfChromo(chromP, this);

		cout << "bestObjectValues(makespan)=" << objectValues.second << ";  total due time=" << objectValues.first << endl;
		//for (int j = 0; j < totalLenOfGACode; ++j) {
		//	cout << chromP->code[j] << " ";
		//}
		cout << endl;
	}
}

void GeneticAlgorithm::resetChildChroms() {
	const int totalLenOfGACode = this->m_totalLenOfGACode;
	const int numOfParentsForCroOver = this->m_numForCrossOver;
	const int numOfParentsForMutation = this->m_numForMutation;
	vector<Chromosome*>& childsForCroOver = this->m_childsForCroOver;
	vector<Chromosome*>& childsForMutation = this->m_childsForMutation;

	// 交叉
	for (int i_chrom = 0; i_chrom < numOfParentsForCroOver; ++i_chrom) {
		Chromosome* chromPInitnew = childsForCroOver[i_chrom];
		chromPInitnew->code.swap(vector<int>(totalLenOfGACode, -1));
	}

	// 变异
	for (int i_chrom = 0; i_chrom < numOfParentsForMutation; ++i_chrom) {
		Chromosome* chromPInitnew = childsForMutation[i_chrom];
		chromPInitnew->code.swap(vector<int>(totalLenOfGACode, -1));
	}
};

void GeneticAlgorithm::crossoverParentsByPOX(int sizeOfSet1, Chromosome* parent1, Chromosome* parent2, Chromosome* child1, Chromosome* child2)
{
	const int numOfJobRangeOfGA = this->m_numOfJobRangeOfGA;
	const int totalLenOfGACode = this->m_totalLenOfGACode;

	// 随机划分工件集为两个子集
	set<int> setJob1;
	set<int> setJob2;
	cout << "sizeOfSet1=" << sizeOfSet1 << endl;
	for (; setJob1.size() < sizeOfSet1; ) {
		int indexOfJob = rand() % numOfJobRangeOfGA;
		setJob1.insert(indexOfJob);
		cout << "indexOfJob=" << indexOfJob << endl;
	}
	vector<int> restCodesForPare1(totalLenOfGACode, -1);
	vector<int> restCodesForPare2(totalLenOfGACode, -1);

	int reverse = rand() % 2;

	// 生成子序列
	int index_R1 = 0;
	int index_R2 = 0;
	for (int i_code = 0; i_code < totalLenOfGACode; ++i_code) {  // 子序列1、2分别保留父亲序列1、2中子集1（即 setJob1）中的工序
		if ((setJob1.find(parent1->code[i_code]) != setJob1.end()) ^ reverse)
			child1->code[i_code] = parent1->code[i_code];
		else {
			restCodesForPare1[index_R1++] = parent1->code[i_code];
		}

		if ((setJob1.find(parent2->code[i_code]) != setJob1.end()) ^ reverse)
			child2->code[i_code] = parent2->code[i_code];
		else {
			restCodesForPare2[index_R2++] = parent2->code[i_code];
		}
	}
	cout << "sssssssssd" << endl;
	index_R1 = 0;
	index_R2 = 0;
	for (int i_restCode = 0; i_restCode < totalLenOfGACode; ++i_restCode) {  // 子序列1、2分别继承父亲序列2、1中的剩余工序
		if ((restCodesForPare1[i_restCode] < 0))
			break;
		while (child1->code[index_R1] >= 0) { index_R1++; }
		while (child2->code[index_R2] >= 0) { index_R2++; }
		child1->code[index_R1++] = restCodesForPare2[i_restCode];
		child2->code[index_R2++] = restCodesForPare1[i_restCode];
	}

	cout << "    child1_code:" << endl;
	for (int i_code = 0; i_code < this->m_totalLenOfGACode; ++i_code) {
		cout << child1->code[i_code] << " ";
	}
	cout << "    child2_code:" << endl;
	for (int i_code = 0; i_code < this->m_totalLenOfGACode; ++i_code) {
		cout << child2->code[i_code] << " ";
	}

	cout << "    parent1_code:" << endl;
	for (int i_code = 0; i_code < this->m_totalLenOfGACode; ++i_code) {
		cout << parent1->code[i_code] << " ";
	}
	cout << "    parent2_code:" << endl;
	for (int i_code = 0; i_code < this->m_totalLenOfGACode; ++i_code) {
		cout << parent2->code[i_code] << " ";
	}

	cout << "ayyyyyyyy" << endl;
	cout << "ayyyyyyyy" << endl;
};

// 轮盘赌选择，不放回--只适合从集合中选择小批量个体
void GeneticAlgorithm::chosenChromsByRoulette(const int numOfChromsToChose, const int numOfPop, vector<Chromosome*>& popPool, vector<int>& chosenChroms) {
	//static double coeff_pressure = coeff_pressure * coeff_pressureBaseNum;
	double coeff_pressure = this->m_coeff_pressure;
	double coeff_additionalNum = this->m_coeff_additionalNum;

	// 计算最大和最小目标值
	double maxObjVal = DBL_MIN;
	double minObjVal = DBL_MAX;

	for (int i = 0; i < numOfPop; ++i) {
		maxObjVal = popPool[i]->objectValues.second > maxObjVal ? popPool[i]->objectValues.second : maxObjVal;
		minObjVal = popPool[i]->objectValues.second < minObjVal ? popPool[i]->objectValues.second : minObjVal;
	}

	// 计算适值
	double totalFitness = 0.0;
	vector<double> fitnessCumulation(numOfPop);
	for (int i = 0; i < numOfPop; ++i) {
		double fitness = maxObjVal - popPool[i]->objectValues.second + coeff_additionalNum * coeff_pressure;
		//cout << "fitness="<< fitness << "  popPool[i]->objectValues.second="<< popPool[i]->objectValues.second << endl;
		fitness = 0.1 * fitness;
		totalFitness += fitness;
		fitnessCumulation[i] = totalFitness;
	}

	// 按照轮盘赌方法选择染色体
	set<int> chosenChromsSet;
	int index = 0;
	for (; chosenChromsSet.size() < numOfChromsToChose;) {
		double positionInPossibility = totalFitness * (rand() * 1.0) / (double)(RAND_MAX + 1.0);
		int indexOfChrom = -1;
		//cout << "positionInPossibility="<< positionInPossibility<<" ";
		for (int i = 0; i < numOfPop; ++i) {
			if (this->m_curGeneration == 134)  cout << "i=" << i << " fitnessCumulation[i]=" << fitnessCumulation[i] << endl;
			if (fitnessCumulation[i] >= positionInPossibility) {
				indexOfChrom = i;
				break;
			}
		}
		if (indexOfChrom < 0) {
			cout << " totalFitness=" << totalFitness << "  positionInPossibility=" << positionInPossibility << endl;
			cout << "Error With Choosing Chromsome by Roulette!" << endl;
			char t; cin >> t;
		}

		//cout << " indexOfChrom=" << indexOfChrom << "  fitnessCumulation[i]=" << fitnessCumulation[indexOfChrom]
		//	<< "  fitnessCumulation[j]=" << fitnessCumulation[indexOfChrom]<<endl;
		if (indexOfChrom >= 1) cout << "  fitnessCumulation[j-1]=" << fitnessCumulation[indexOfChrom - 1] << endl;

		if (chosenChromsSet.insert(indexOfChrom).second)
			chosenChroms[index++] = indexOfChrom;
	}
}

// 轮盘赌选择，放回--适合所有
void GeneticAlgorithm::chosenChromsByRoulette(const int numOfChromsToChose, const int numOfPop, vector<Chromosome*>& popPool, vector<int>& chosenChroms, int isPutBack) {
	//static double coeff_pressure = coeff_pressure * coeff_pressureBaseNum;
	double coeff_pressure = this->m_coeff_pressure;
	double coeff_additionalNum = this->m_coeff_additionalNum;

	// 计算最大和最小目标值
	double maxObjVal = DBL_MIN;
	double minObjVal = DBL_MAX;

	for (int i = 0; i < numOfPop; ++i) {
		maxObjVal = popPool[i]->objectValues.second > maxObjVal ? popPool[i]->objectValues.second : maxObjVal;
		minObjVal = popPool[i]->objectValues.second < minObjVal ? popPool[i]->objectValues.second : minObjVal;
	}

	// 计算适值
	double totalFitness = 0;
	vector<double> fitnessCumulation(numOfPop);
	for (int i = 0; i < numOfPop; ++i) {
		double fitness = maxObjVal - popPool[i]->objectValues.second + coeff_additionalNum * coeff_pressure;
		fitness = 0.1 * fitness;
		totalFitness += fitness;
		fitnessCumulation[i] = totalFitness;
	}

	// 按照轮盘赌方法选择染色体
	int index = 0;
	for (int index = 0; index < numOfChromsToChose; ++index) {
		double positionInPossibility = totalFitness * (rand() * 1.0) / (double)(RAND_MAX + 1.0);
		int indexOfChrom = -1;
		for (int i = 0; i < numOfPop; ++i) {
			if (fitnessCumulation[i] >= positionInPossibility) {
				indexOfChrom = i;
				break;
			}
		}

		if (indexOfChrom < 0) {
			cout << " totalFitness=" << totalFitness << "  positionInPossibility=" << positionInPossibility << endl;
			cout << "Error With Choosing Chromsome by Roulette!" << endl;
			char t; cin >> t;
		}
		chosenChroms[index] = indexOfChrom;
	}
}

// 锦标赛选择，放回--适合所有
void GeneticAlgorithm::chosenChromsByTournament(const int numOfChromsToChose, const int numOfPop, vector<Chromosome*>& popPool, vector<int>& chosenChroms, int isPutBack) {
	//static double coeff_pressure = coeff_pressure * coeff_pressureBaseNum;
	double coeff_pressure = this->m_coeff_pressure;
	double coeff_additionalNum = this->m_coeff_additionalNum;

	// 计算最大和最小目标值
	double maxObjVal = DBL_MIN;
	double minObjVal = DBL_MAX;

	for (int i = 0; i < numOfPop; ++i) {
		maxObjVal = popPool[i]->objectValues.second > maxObjVal ? popPool[i]->objectValues.second : maxObjVal;
		minObjVal = popPool[i]->objectValues.second < minObjVal ? popPool[i]->objectValues.second : minObjVal;
	}

	// 计算适值
	vector<double> fitnessArr(numOfPop);
	for (int i = 0; i < numOfPop; ++i) {
		double fitness = maxObjVal - popPool[i]->objectValues.second + coeff_additionalNum * coeff_pressure;
		fitness = 0.1 * fitness;
		fitnessArr[i] = fitness;
	}

	// 按照轮盘赌方法选择染色体
	int index = 0;
	for (;index < numOfChromsToChose;) {
		int indexOfChrom = -1;
		int p1 = numOfPop * (rand() * 1.0) / (double)(RAND_MAX + 1.0);
		int p2 = numOfPop * (rand() * 1.0) / (double)(RAND_MAX + 1.0);
		if (fitnessArr[p1] > fitnessArr[p2])
			indexOfChrom = p1;
		else
			indexOfChrom = p2;
		chosenChroms[index++] = indexOfChrom;
	}
}

// 锦标赛选择，挑选不好的扔出来
void GeneticAlgorithm::chosenChromsByTournament2(const int numOfChromsToChose, const int numOfPop, vector<Chromosome*>& popPool, vector<int>& chosenChroms, int isPutBack) {
	//static double coeff_pressure = coeff_pressure * coeff_pressureBaseNum;
	double coeff_pressure = this->m_coeff_pressure;
	double coeff_additionalNum = this->m_coeff_additionalNum;
	int numToDrop =  numOfPop- numOfChromsToChose;

	// 计算最大和最小目标值
	double maxObjVal = DBL_MIN;
	double minObjVal = DBL_MAX;

	for (int i = 0; i < numOfPop; ++i) {
		maxObjVal = popPool[i]->objectValues.second > maxObjVal ? popPool[i]->objectValues.second : maxObjVal;
		minObjVal = popPool[i]->objectValues.second < minObjVal ? popPool[i]->objectValues.second : minObjVal;
	}

	// 计算适值
	vector<double> fitnessArr(numOfPop);
	for (int i = 0; i < numOfPop; ++i) {
		double fitness = maxObjVal - popPool[i]->objectValues.second + coeff_additionalNum * coeff_pressure;
		fitness = 0.1 * fitness;
		fitnessArr[i] = fitness;
	}

	cout <<"yiosdfugh"<< endl;
	// 按照轮盘赌方法选择染色体
	set<int> dropPool;
	for (; dropPool.size() < numToDrop;) {
		int indexOfChrom = -1;
		int p1 = numOfPop * (rand() * 1.0) / (double)(RAND_MAX + 1.0);
		int p2 = numOfPop * (rand() * 1.0) / (double)(RAND_MAX + 1.0);
		if (fitnessArr[p1] < fitnessArr[p2])
			indexOfChrom = p1;
		else
			indexOfChrom = p2;
		dropPool.insert(indexOfChrom);

		cout << indexOfChrom << ' ';
	}

	cout << endl;
	int index = 0;
	for (int i = 0; i < numOfPop; ++i) {
		if (dropPool.find(i) != dropPool.end()) continue;
		chosenChroms[index++] = i;
		cout << i << ' ';
	}
	cout <<"  index="<< index << endl;

	cout <<"end"<< endl;

}


void GeneticAlgorithm::crossoverOfPop()
{
	const int numOfPop = this->m_numOfPop;
	const int totalLenOfGACode = this->m_totalLenOfGACode;
	const int numOfJobRangeOfGA = this->m_numOfJobRangeOfGA;
	vector<Chromosome*>& popPool = this->m_popPool;
	vector<Chromosome*>& childsForCroOver = this->m_childsForCroOver;
	const int numOfParentsForCroOver = this->m_numForCrossOver;

	//int numOfParentsForCroOver = numOfPop * crossRate;
	//if (numOfParentsForCroOver % 2 == 1) numOfParentsForCroOver++;

	cout << "d44444444fffffffk" << endl;
	int sizeOfSet1 = rand() % (numOfJobRangeOfGA / 2) + 1;  // 随机地划分工件为两个子集

	vector<int> chosenChroms(numOfParentsForCroOver);
	this->chosenChromsByRoulette(numOfParentsForCroOver, numOfPop, popPool, chosenChroms);
	cout << "zxxxxggh sizeOfSet1=" << sizeOfSet1 << endl;
	for (int i = 0; i < numOfParentsForCroOver; ++i, ++i) {
		cout << "  ii=" << i << "  chosenChroms[i]=" << chosenChroms[i] << endl;
		if (popPool[chosenChroms[i]]->code[0] == -1) cout << "chosenChroms[i]=" << chosenChroms[i] << endl;
		if (popPool[chosenChroms[i + 1]]->code[0] == -1) cout << "chosenChroms[i+1]=" << chosenChroms[i + 1] << endl;
		cout << "  chosenChroms[i]=" << i << endl;

		this->crossoverParentsByPOX
		(sizeOfSet1, popPool[chosenChroms[i]], popPool[chosenChroms[i + 1]], childsForCroOver[i], childsForCroOver[i + 1]);
	}

	cout << "ghkkkkkkkk" << endl;
	/*
	double tmpCoeff = curGeneration / totalGeneration;
	double baseLine = maxObjVal * coeff_Multiple *(-tmpCoeff +1)+ minObjVal * (coeff_Multiple-1)* (tmpCoeff);
	1500*(-x/200+1)+600*(x/200)
	//计算概率-计算累计概率
	vector<Chromosome*> ;
	double maxFitness = baseLine - minObjVal;
	for (int i = 0; i < popPool.size(); ++i) {
		double fitness = baseLine - popPool[i]->objectValues.first;
		if (fitness< 0.1 * maxFitness && fitness > -0.5 * maxFitness) fitness = 0.1 * maxFitness;
		else if(fitness <= -0.5 * maxFitness) fitness = 0.05 * maxFitness;
	}

	// 轮盘赌选择numOfParentsForCroOver个父亲个体
	//  计算概率
	int index = 0;
	for (; chosenPartsSet.size() < numOfParentsForCroOver;) {
		int indexOfChrom = rand() % numOfPop;
		if (chosenPartsSet.insert(indexOfChrom).second)
			chosenParents[index++] = indexOfChrom;
	}

	for (int i = 0; i < numOfParentsForCroOver; ++i) {
		int i2 = i + 1;
		pair<Chromosome*, Chromosome*> childs = crossoverParentsByPOX(sizeOfSet1, numOfJobRangeOfGA, totalLenOfGACode, popPool[chosenParents[i]], popPool[chosenParents[i2]]);
		popPool[i] = childs.first;
		popPool[i2] = childs.second;
		++i;
	}

	int index = 0;
	for (; chosenPartsSet.size() < numOfParentsForMutation;) {
		int indexOfChrom = rand() % numOfPop;
		if (chosenPartsSet.insert(indexOfChrom).second)
			chosenParents[index++] = indexOfChrom;
	}

	for (int i = 0; i < numOfParentsForMutation; ++i) {
		int i2 = i + 1;
		pair<Chromosome*, Chromosome*> childs = crossoverParentsByPOX(sizeOfSet1, numOfJobRangeOfGA, totalLenOfGACode, popPool[chosenParents[i]], popPool[chosenParents[i2]]);
		popPool[i] = childs.first;
		popPool[i2] = childs.second;
		++i;
	}
	*/
}

void GeneticAlgorithm::mutationOfPop()
{
	const int numOfPop = this->m_numOfPop;
	const int totalLenOfGACode = this->m_totalLenOfGACode;
	const int numOfJobRangeOfGA = this->m_numOfJobRangeOfGA;
	vector<Chromosome*>& popPool = this->m_popPool;
	vector<Chromosome*>& childsForMutation = this->m_childsForMutation;
	const int numOfParentsForMutation = this->m_numForMutation;

	//int numOfParentsForMutation = numOfPop * mutatRate;
	vector<int> chosenChroms(numOfParentsForMutation);
	chosenChromsByRoulette(numOfParentsForMutation, numOfPop, popPool, chosenChroms);

	int j = 0;
	for (int i = 0; i < numOfParentsForMutation; ++i)
		//for (int i = 0; i < 1; ++i)
	{
		int positionOfCode1 = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
		int positionOfCode2 = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
		Chromosome* chromP = popPool[chosenChroms[i]];
		if (chromP->code[positionOfCode1] == chromP->code[positionOfCode2]) {
			--i; continue;
		}
		Chromosome* childOfMutation = childsForMutation[j++];
		childOfMutation->code.assign(chromP->code.begin(), chromP->code.end());
		childOfMutation->code[positionOfCode1] = chromP->code[positionOfCode2];
		childOfMutation->code[positionOfCode2] = chromP->code[positionOfCode1];
	}
}

// 获取变异位置，使得脱期job往前移动
pair<int, int> GeneticAlgorithm::getPositionsForMutation(Chromosome* chromP) {
	const int totalLenOfGACode = this->m_totalLenOfGACode;

	int positionOfCode1 = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
	int positionOfCode2;

	// 随机选择一个脱期job
	int numOfDelaied = chromP->delayJobs.size();
	int jobIndex = numOfDelaied * (1.0 * rand()) / (RAND_MAX + 1.0);
	map<int, double>::iterator iter = chromP->delayJobs.begin();
	for (int i_delayJob = 0; i_delayJob < jobIndex; ++i_delayJob, ++iter);
	int jobNum = iter->first;

	// 获取脱期job的最后一个工序的位置，再在前面随机选择一个位置
	for (int i_pos = 0; i_pos < totalLenOfGACode; ++i_pos) {
		if (chromP->code[i_pos] == jobNum) positionOfCode1 = i_pos;
	}
	positionOfCode2 = positionOfCode1 * (1.0 * rand()) / (RAND_MAX + 1.0);

	return make_pair(positionOfCode1, positionOfCode2);
}

void GeneticAlgorithm::mutationOfPop_JobTardiness() {
	const int numOfPop = this->m_numOfPop;
	const int totalLenOfGACode = this->m_totalLenOfGACode;
	const int numOfJobRangeOfGA = this->m_numOfJobRangeOfGA;
	vector<Chromosome*>& popPool = this->m_popPool;
	vector<Chromosome*>& childsForMutation = this->m_childsForMutation;
	const int numOfParentsForMutation = this->m_numForMutation;

	//int numOfParentsForMutation = numOfPop * mutatRate;
	vector<int> chosenChroms(numOfParentsForMutation);
	chosenChromsByRoulette(numOfParentsForMutation, numOfPop, popPool, chosenChroms);

	int j = 0;
	for (int i = 0; i < numOfParentsForMutation; ++i)
		//for (int i = 0; i < 1; ++i)
	{
		Chromosome* chromP = popPool[chosenChroms[i]];
		pair<int,int> resPos = getPositionsForMutation(chromP);

		int k = 0;
		if (chromP->code[resPos.first] == chromP->code[resPos.second] && rand()%2 == 1) {
			--i; continue;
		}
		Chromosome* childOfMutation = childsForMutation[j++];
		childOfMutation->code.assign(chromP->code.begin(), chromP->code.end());
		childOfMutation->code[resPos.first] = chromP->code[resPos.second];
		childOfMutation->code[resPos.second] = chromP->code[resPos.first];
	}

};

// 邻域方式--翻转部分片段
void  GeneticAlgorithm::getNeighborByReverse(Chromosome* chromP, const int totalLenOfGACode) {
	// 截取一段，翻转
	int left, right;
	left = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);  // 包括前后
	right = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
	if (left > right) swap(left, right);
	int length = right - left + 1;


	int i = left;
	int j = right;
	for (; i < j; ++i, --j) {
		swap(chromP->code[i], chromP->code[j]);
	}
}

// 邻域方式--交换部分片段获取新个体
void  GeneticAlgorithm::getNeighborByExchangeSub(Chromosome* chromP, const int totalLenOfGACode) {
	// 截取2段，交换
	int left = -1;
	int right = -1;
	int insertPos;

	while (left == right) {
		left = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
		right = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
	}
	if (left > right) swap(left, right);

	int length;
	int tmp = (right - left) < (totalLenOfGACode - right) ? (right - left) : (totalLenOfGACode - right);
	length = rand() % tmp + 1;


	vector<int> cutCodes(length);

	//cout << endl; cout << endl;
	//for (int j = 0; j < chromP->code.size(); ++j) {
	//	if (j == left) cout << "   ";
	//	if (j == right) cout << "   ";
	//	cout << chromP->code[j] << " ";
	//	if (j == left+length) cout << "   ";
	//	if (j == right + length) cout << "   ";
	//}

	memcpy(&cutCodes[0], &chromP->code[left], length * sizeof(chromP->code[0]));
	memcpy(&chromP->code[left], &chromP->code[right], length * sizeof(chromP->code[0]));
	memcpy(&chromP->code[right], &cutCodes[0], length * sizeof(chromP->code[0]));

	//cout << endl; cout << endl;
	//for (int j = 0; j < chromP->code.size(); ++j) {
	//	if (j == left) cout << "   ";
	//	if (j == right) cout << "   ";
	//	cout << chromP->code[j] << " ";
	//	if (j == left + length) cout << "   ";
	//	if (j == right + length) cout << "   ";
	//}
}

// 邻域方式--截取一段，随机找一位置，再插入
void  GeneticAlgorithm::getNeighborByReinsertSub(Chromosome* chromP, const int totalLenOfGACode) {
	// 截取一段，随机找一位置，再插入

	int left = -1;
	int right = -1;
	int insertPos;

	while (left == right) {
		left = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
		right = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
	}
	if (left > right) swap(left, right);
	int length = right - left + 1;


	insertPos = (totalLenOfGACode - length) * (1.0 * rand()) / (RAND_MAX + 1.0);

	vector<int> cutCodes(length);
	cutCodes.assign(chromP->code.begin() + left, chromP->code.begin() + right + 1);

	//cout << endl;
	//cout << "  left=" << left << " right="<<right<<"  length=" << length << endl;

	//cout << endl; cout << endl;
	//for (int j = 0; j < chromP->code.size(); ++j) {
	//	if (j == left) cout << "   ";
	//	cout << chromP->code[j] << " "; 
	//	if (j == right) cout << "   ";
	//}

	if (insertPos >= left)  // 往后插入 
	{
		insertPos += (right - left + 1);
		memmove(&chromP->code[left], &chromP->code[right + 1], (insertPos - right - 1) * sizeof(chromP->code[0]));
		memcpy(&chromP->code[insertPos - length], &cutCodes[0], length * sizeof(chromP->code[0]));
	}
	else {                  // 往前插入
		memmove(&chromP->code[length + insertPos], &chromP->code[insertPos], (left - insertPos) * sizeof(chromP->code[0]));
		memcpy(&chromP->code[insertPos], &cutCodes[0], length * sizeof(chromP->code[0]));
	}

	//cout << endl; cout << endl;
	//for (int j = 0; j < chromP->code.size(); ++j) {
	//	if (j == insertPos) cout << "   ";
	//	cout << chromP->code[j] << " ";
	//	if (j == length + insertPos) cout << "   ";
	//}
}

// 邻域方式--交换两个基因位置
void  GeneticAlgorithm::getNeighborByExchangeTwo(Chromosome* chromP, const int totalLenOfGACode) {

	int left = -1;
	int right = -1;
	int insertPos;

	while (left == right) {
		left = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
		right = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
	}

	swap(chromP->code[left], chromP->code[right]);
}

// 邻域方式--抽取一个，再插入
void  GeneticAlgorithm::getNeighborByReinsertOne(Chromosome* chromP, const int totalLenOfGACode) {

	int selectedPos = -1;
	int insertPos = -1;

	while (selectedPos == insertPos) {
		selectedPos = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
		insertPos =totalLenOfGACode* (1.0 * rand()) / (RAND_MAX + 1.0);
	}

	int tmpCopy = chromP->code[selectedPos];
	if (insertPos > selectedPos)  // 往后插入 
	{
		memmove(&chromP->code[selectedPos], &chromP->code[selectedPos + 1], (insertPos - selectedPos) * sizeof(chromP->code[0]));
		chromP->code[insertPos] = tmpCopy;
	}
	else {                  // 往前插入
		memmove(&chromP->code[insertPos+1], &chromP->code[insertPos], (selectedPos- insertPos) * sizeof(chromP->code[0]));
		chromP->code[insertPos] = tmpCopy;
	}
}


void GeneticAlgorithm::getNewChromeToNeighbor(Chromosome* chromP, const int totalLenOfGACode) {
	int tmp = rand() % 3;
	if (tmp == 0)
		this->getNeighborByReverse(chromP, totalLenOfGACode);
	else if(tmp == 1)
		this->getNeighborByExchangeSub(chromP, totalLenOfGACode);
	else
		this->getNeighborByReinsertSub(chromP, totalLenOfGACode);
	
}

// 对一个染色体做局部搜索
void GeneticAlgorithm::localSearchForCurChrome(Chromosome* chromP, GeneticAlgorithm* populationP, const int maxIter) {
	//const int totalLenOfGACode = this->m_totalLenOfGACode;
	const int totalLenOfGACode = chromP->codeLen;
	Chromosome* chromPTmp = new Chromosome(chromP->codeLen, chromP->rangeOfJob);

	bool isRenew = false;
	chromPTmp->code.assign(chromP->code.begin(), chromP->code.end());

	//cout << " maxIter=" << maxIter << endl;
	cout << "    before--" << "obj_makespan=" << chromP->objectValues.second
		<< " obj_tardiness=" << chromP->objectValues.first << endl;

	for (int iter = 0; iter < maxIter; ++iter) {

		int tmp = rand() % 5;

		//cout <<  "  tmp=" << tmp << endl;
		if (tmp == 0)
			GeneticAlgorithm::getNeighborByReverse(chromPTmp, totalLenOfGACode);
		else if (tmp == 1)
			GeneticAlgorithm::getNeighborByExchangeSub(chromPTmp, totalLenOfGACode);
		else if (tmp == 2)
			GeneticAlgorithm::getNeighborByReinsertSub(chromPTmp, totalLenOfGACode);
		else if (tmp == 3)
			GeneticAlgorithm::getNeighborByExchangeTwo(chromPTmp, totalLenOfGACode);
		else
			GeneticAlgorithm::getNeighborByReinsertOne(chromPTmp, totalLenOfGACode);

		getObjectValuesOfChromo(chromPTmp, populationP);
		//cout << " iter=" << iter  << endl;
		isRenew = false;
		// 如果好，则更新
		if (chromPTmp->objectValues.second < chromP->objectValues.second) {
			chromP->code.assign(chromPTmp->code.begin(), chromPTmp->code.end());

			chromP->objectValues = chromPTmp->objectValues;
			chromP->tardinessOfjobs.assign(chromPTmp->tardinessOfjobs.begin(), chromPTmp->tardinessOfjobs.end());
			for (auto& ele : chromPTmp->delayJobs)  chromP->delayJobs.insert(ele);
			isRenew = true;

			//cout<<"    isRenew=" << isRenew << endl;
			//cout << "    chromPTmp->objectValues.first(dueTime) =" << chromPTmp->objectValues.second << endl;
		}
		else {
			chromPTmp->code.assign(chromP->code.begin(), chromP->code.end());
		}
		//cout << "   end" << endl;

		if (isRenew) {
			cout << "    after--" << "obj_makespan=" << chromP->objectValues.second
				<< " obj_tardiness=" << chromP->objectValues.first << endl;
			return;
		}
	}

	cout << "    after--" << "obj_makespan=" << chromP->objectValues.second
		<< " obj_tardiness=" << chromP->objectValues.first << endl;

}

// 对一个染色体做局部搜索
void GeneticAlgorithm::localSearchForCurChrome_Parallel(Chromosome* chromP, GeneticAlgorithm* populationP, const int maxIter) {
	//const int totalLenOfGACode = this->m_totalLenOfGACode;
	const int totalLenOfGACode = chromP->codeLen;
	Chromosome* chromPTmp = new Chromosome(chromP->codeLen, chromP->rangeOfJob);

	bool isRenew = false;
	chromPTmp->code.assign(chromP->code.begin(), chromP->code.end());

	//cout << " maxIter=" << maxIter << endl;
	//cout << "    before--" << "obj_makespan=" << chromP->objectValues.second
	//	<< " obj_tardiness=" << chromP->objectValues.first << endl;

	for (int iter = 0; iter < maxIter; ++iter) {
		
		int tmp = rand() % 5;

		//cout <<  "  tmp=" << tmp << endl;
		if (tmp == 0)
			GeneticAlgorithm::getNeighborByReverse(chromPTmp, totalLenOfGACode);
		else if (tmp == 1)
			GeneticAlgorithm::getNeighborByExchangeSub(chromPTmp, totalLenOfGACode);
		else if (tmp == 2)
			GeneticAlgorithm::getNeighborByReinsertSub(chromPTmp, totalLenOfGACode);
		else if(tmp == 3)
			GeneticAlgorithm::getNeighborByExchangeTwo(chromPTmp, totalLenOfGACode);
		else
			GeneticAlgorithm::getNeighborByReinsertOne(chromPTmp, totalLenOfGACode);

		//cout<<"tty"<<endl;
		//getObjectValuesOfChromo(chromPTmp, populationP);
		getObjectValuesOfChromo_Parallel(chromPTmp, populationP);
		//cout << "ppo" << endl;

		//cout << " iter=" << iter  << endl;
		isRenew = false;
		// 如果好，则更新
		if (chromPTmp->objectValues.second < chromP->objectValues.second) {
			chromP->code.assign(chromPTmp->code.begin(), chromPTmp->code.end());

			chromP->objectValues = chromPTmp->objectValues;
			chromP->tardinessOfjobs.assign(chromPTmp->tardinessOfjobs.begin(), chromPTmp->tardinessOfjobs.end());
			for (auto& ele : chromPTmp->delayJobs)  chromP->delayJobs.insert(ele);
			isRenew = true;

			//cout<<"    isRenew=" << isRenew << endl;
			//cout << "    chromPTmp->objectValues.first(dueTime) =" << chromPTmp->objectValues.second << endl;
		}
		else {
			chromPTmp->code.assign(chromP->code.begin(), chromP->code.end());
		}
		//cout << "   end" << endl;

		if (isRenew) {
			//cout << "    after--" << "obj_makespan=" << chromP->objectValues.second
			//	<< " obj_tardiness=" << chromP->objectValues.first << endl;
			return;
		}
	}

	//cout << "    after--" << "obj_makespan=" << chromP->objectValues.second
	//	<< " obj_tardiness=" << chromP->objectValues.first << endl;
}

/*
// 对一个染色体做局部搜索
void GeneticAlgorithm::localSearchForCurChrome(Chromosome* chromP, const int maxIter) {
	const int totalLenOfGACode = this->m_totalLenOfGACode;
	Chromosome* chromPTmp = new Chromosome(chromP->codeLen, chromP->rangeOfJob);

	bool isRenew = false;
	chromPTmp->code.assign(chromP->code.begin(), chromP->code.end());

	for (int iter = 0; iter < maxIter; ++iter) {
		// 获取关键路径
		// 随机选择关键路径上的工序
		//cout << endl;
		//cout << " iter=" << iter << endl;

		int positionOfCode1, positionOfCode2;
		//int exORin = 0;
		int exORin = rand() % 2;

		// exchange交换法 insert插入法
		// 
		if (rand() % 3 == 1) {  // 以概率的方式选择
		//if(iter < maxIter/3){    // 前面选脱期，后面随机选
		//if(0){
			// 选择脱期job
			pair<int, int> res = getPositionsForMutation(chromP);
			positionOfCode1 = res.first;
			positionOfCode2 = res.second;
		}
		else {
			// 随机选择工序
			positionOfCode1 = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
			positionOfCode2 = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
		}


		if (positionOfCode1 > positionOfCode2) swap(positionOfCode1, positionOfCode2);
		else if (positionOfCode1 == positionOfCode2) continue;

		if (exORin == 1) {
			chromPTmp->code[positionOfCode1] = chromP->code[positionOfCode2];
			chromPTmp->code[positionOfCode2] = chromP->code[positionOfCode1];
		}
		else {
			memcpy(&chromPTmp->code[positionOfCode1 + 1], &chromP->code[positionOfCode1], (positionOfCode2 - positionOfCode1) * sizeof(chromP->code[0]));
			chromPTmp->code[positionOfCode1] = chromP->code[positionOfCode2];
		}

		getObjectValuesOfChromo(chromPTmp, this);

		//cout << endl;
		//cout << "  positionOfCode1=" << positionOfCode1 << " positionOfCode2=" << positionOfCode2 << endl;
		//for (int j = 0; j < chromP->code.size(); ++j)  cout << chromP->code[j] << " ";

		isRenew = false;
		// 如果好，则更新
		if (exORin == 1) {
			if (chromPTmp->objectValues.second < chromP->objectValues.second) {
				chromP->code[positionOfCode1] = chromPTmp->code[positionOfCode1];
				chromP->code[positionOfCode2] = chromPTmp->code[positionOfCode2];
				chromP->objectValues = chromPTmp->objectValues;
				chromP->tardinessOfjobs.assign(chromPTmp->tardinessOfjobs.begin(), chromPTmp->tardinessOfjobs.end());
				for (auto& ele : chromPTmp->delayJobs)  chromP->delayJobs.insert(ele);
				isRenew = true;

				//cout << " iter=" << iter << "  isRenew=" << isRenew << endl;
				//cout << "chromPTmp->objectValues.first(dueTime) =" << chromPTmp->objectValues.second << endl;
			}
			else {
				chromPTmp->code[positionOfCode1] = chromP->code[positionOfCode1];
				chromPTmp->code[positionOfCode2] = chromP->code[positionOfCode2];
			}
		}
		else {
			if (chromPTmp->objectValues.second < chromP->objectValues.second) {
				memcpy(&chromP->code[positionOfCode1], &chromPTmp->code[positionOfCode1], (positionOfCode2 - positionOfCode1 + 1) * sizeof(chromP->code[0]));
				chromP->objectValues = chromPTmp->objectValues;
				chromP->tardinessOfjobs.assign(chromPTmp->tardinessOfjobs.begin(), chromPTmp->tardinessOfjobs.end());
				for (auto& ele : chromPTmp->delayJobs)  chromP->delayJobs.insert(ele);
				isRenew = true;

				//cout << " iter=" << iter << "  isRenew=" << isRenew << endl;
				//cout << "chromPTmp->objectValues.first(dueTime) =" << chromPTmp->objectValues.second << endl;

				//for (int j = 0; j < chromP->code.size(); ++j) cout << chromP->code[j] << " ";
				//cout << endl;
				//char c; cin >> c;
			}
			else
				memcpy(&chromPTmp->code[positionOfCode1], &chromP->code[positionOfCode1], (positionOfCode2 - positionOfCode1 + 1) * sizeof(chromP->code[0]));
		}

		if (isRenew == true) break;
	}

	getObjectValuesOfChromo(chromP, this);
	delete chromPTmp;
}


// 对一个染色体做局部搜索
void GeneticAlgorithm::localSearchForCurChrome(Chromosome* chromP, const int maxIter) {
	const int totalLenOfGACode = this->m_totalLenOfGACode;
	Chromosome* chromPTmp = new Chromosome(chromP->codeLen, chromP->rangeOfJob);

	bool isRenew = false;
	chromPTmp->code.assign(chromP->code.begin(), chromP->code.end());

	for (int iter = 0; iter < maxIter; ++iter) {
		// 获取关键路径
		// 随机选择关键路径上的工序
		//cout << endl;
		//cout << " iter=" << iter << endl;

		int positionOfCode1, positionOfCode2;
		//int exORin = 0;
		int exORin = rand() % 2;

		// exchange交换法 insert插入法
		// 
		if (rand() % 3 == 1) {  // 以概率的方式选择
		//if(iter < maxIter/3){    // 前面选脱期，后面随机选
		//if(0){
			// 选择脱期job
			pair<int, int> res = getPositionsForMutation(chromP);
			positionOfCode1 = res.first;
			positionOfCode2 = res.second;
		}
		else {
			// 随机选择工序
			positionOfCode1 = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
			positionOfCode2 = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
		}




		if (positionOfCode1 > positionOfCode2) swap(positionOfCode1, positionOfCode2);
		else if (positionOfCode1 == positionOfCode2) continue;

		if (exORin == 1) {
			chromPTmp->code[positionOfCode1] = chromP->code[positionOfCode2];
			chromPTmp->code[positionOfCode2] = chromP->code[positionOfCode1];
		}
		else {
			memcpy(&chromPTmp->code[positionOfCode1 + 1], &chromP->code[positionOfCode1], (positionOfCode2 - positionOfCode1) * sizeof(chromP->code[0]));
			chromPTmp->code[positionOfCode1] = chromP->code[positionOfCode2];
		}

		getObjectValuesOfChromo(chromPTmp, this);

		//cout << endl;
		//cout << "  positionOfCode1=" << positionOfCode1 << " positionOfCode2=" << positionOfCode2 << endl;
		//for (int j = 0; j < chromP->code.size(); ++j)  cout << chromP->code[j] << " ";

		isRenew = false;
		// 如果好，则更新
		if (exORin == 1) {
			if (chromPTmp->objectValues.second < chromP->objectValues.second) {
				chromP->code[positionOfCode1] = chromPTmp->code[positionOfCode1];
				chromP->code[positionOfCode2] = chromPTmp->code[positionOfCode2];
				chromP->objectValues = chromPTmp->objectValues;
				chromP->tardinessOfjobs.assign(chromPTmp->tardinessOfjobs.begin(), chromPTmp->tardinessOfjobs.end());
				for (auto& ele : chromPTmp->delayJobs)  chromP->delayJobs.insert(ele);
				isRenew = true;

				//cout << " iter=" << iter << "  isRenew=" << isRenew << endl;
				//cout << "chromPTmp->objectValues.first(dueTime) =" << chromPTmp->objectValues.second << endl;
			}
			else {
				chromPTmp->code[positionOfCode1] = chromP->code[positionOfCode1];
				chromPTmp->code[positionOfCode2] = chromP->code[positionOfCode2];
			}
		}
		else {
			if (chromPTmp->objectValues.second < chromP->objectValues.second) {
				memcpy(&chromP->code[positionOfCode1], &chromPTmp->code[positionOfCode1], (positionOfCode2 - positionOfCode1 + 1) * sizeof(chromP->code[0]));
				chromP->objectValues = chromPTmp->objectValues;
				chromP->tardinessOfjobs.assign(chromPTmp->tardinessOfjobs.begin(), chromPTmp->tardinessOfjobs.end());
				for (auto& ele : chromPTmp->delayJobs)  chromP->delayJobs.insert(ele);
				isRenew = true;

				//cout << " iter=" << iter << "  isRenew=" << isRenew << endl;
				//cout << "chromPTmp->objectValues.first(dueTime) =" << chromPTmp->objectValues.second << endl;

				//for (int j = 0; j < chromP->code.size(); ++j) cout << chromP->code[j] << " ";
				//cout << endl;
				//char c; cin >> c;
			}
			else
				memcpy(&chromPTmp->code[positionOfCode1], &chromP->code[positionOfCode1], (positionOfCode2 - positionOfCode1 + 1) * sizeof(chromP->code[0]));
		}

		if (isRenew == true) break;
	}

	getObjectValuesOfChromo(chromP, this);
	delete chromPTmp;
}
*/


void GeneticAlgorithm::localSearch() {
	// 选择几个好个体
	// 对每一个染色体，迭代max_iter次，当新染色体好时，更新
	const int numOfPop = this->m_numOfPop;
	const int maxIter = 50;  // 最大迭代次数
	const int numBestChrome = 10;  // 选择前几个最好的染色体进行local Search
	const int numRandomChrome = 5;  // 选择前几个最好的染色体，再从后面随机选择一部分

	vector<Chromosome*>& popPool = this->m_popPool;
	sort(popPool.begin(), popPool.end(), GeneticAlgorithm::compareChrom);



	for (int i = 0; i < numBestChrome; ++i) {
		Chromosome* chromP = popPool[i];
		cout << "    " << i << "=" << i << " obj_makespan=" << chromP->objectValues.second
			<< " obj_tardiness=" << chromP->objectValues.first << endl;

		localSearchForCurChrome(chromP, this, maxIter);

		cout << "    " << i << "=" << i << " obj_makespan=" << chromP->objectValues.second
			<< " obj_tardiness=" << chromP->objectValues.first << endl;
		cout << endl;
	}

	for (int i = 0; i < numRandomChrome; ++i) {
		int pos = (numOfPop - numBestChrome) * (1.0 * rand()) / (RAND_MAX + 1.0) + numBestChrome;
		Chromosome* chromP = popPool[pos];
		cout << "    " << i << "=" << i + numBestChrome << " " << pos << " obj_makespan=" << chromP->objectValues.second
			<< " obj_tardiness=" << chromP->objectValues.first << endl;

		localSearchForCurChrome(chromP, this, maxIter);

		cout << "    " << i << "=" << i + numBestChrome << " " << pos << " obj_makespan=" << chromP->objectValues.second
			<< " obj_tardiness=" << chromP->objectValues.first << endl;
		cout << endl;
	}

}


void GeneticAlgorithm::localSearch_Parallel() {
	// 选择几个好个体
	// 对每一个染色体，迭代max_iter次，当新染色体好时，更新
	const int numOfPop = this->m_numOfPop;
	const int maxIter = 50;  // 最大迭代次数
	const int numBestChrome = 10;  // 选择前几个最好的染色体进行local Search
	const int numRandomChrome = 5;  // 选择前几个最好的染色体，再从后面随机选择一部分
	
	const unsigned int NumOfTotalChosen = numBestChrome + numRandomChrome;
	
	vector<Chromosome*>& popPool = this->m_popPool;
	sort(popPool.begin(), popPool.end(), GeneticAlgorithm::compareChrom);
	
	vector<Chromosome*> chosenChroms(NumOfTotalChosen);
	int index = 0;
	for (int i = 0; i < numBestChrome; ++i) 
		chosenChroms[index++] = popPool[i];
	for (int i = 0; i < numRandomChrome; ++i) {
		int pos = (numOfPop - numBestChrome) * (1.0 * rand()) / (RAND_MAX + 1.0) + numBestChrome;
		chosenChroms[index++] = popPool[pos];
	}

	//const unsigned int num_thread = thread::hardware_concurrency();  // 获取硬件所支持的线程个数
	const unsigned int num_thread = 4;  // 7,3,4,8

	vector<thread> threads(num_thread);

	cout << "num_thread=" << num_thread << endl;

	index = 0;
	int round = NumOfTotalChosen / num_thread; // 需要几轮
	for (int i = 0; i < round; ++i) {
		for (int j = 0; j < num_thread; ++j) {
			Chromosome* chromP = chosenChroms[index++];
			cout << " j_thread=" << j << endl;
			//threads[j] = thread(localSearchForCurChrome, chromP, this, maxIter);
			threads[j] = thread(localSearchForCurChrome_Parallel, chromP, this, maxIter);
		}
		for (int j = 0; j < num_thread; ++j) {
			threads[j].join();
		}
	}
	int restNum = NumOfTotalChosen % num_thread;
	if (restNum != 0) {
		for (int j = 0; j < restNum; ++j) {
			Chromosome* chromP = chosenChroms[index++];
			threads[j] = thread(localSearchForCurChrome_Parallel, chromP, this, maxIter);
		}
		for (int j = 0; j < restNum; ++j) {
			threads[j].join();
		}
	}
	 
}


void GeneticAlgorithm::getObjValForChilds() {
	const int numOfParentsForCroOver = this->m_numForCrossOver;
	const int numOfParentsForMutation = this->m_numForMutation;
	vector<Chromosome*>& childsForCroOver = this->m_childsForCroOver;
	vector<Chromosome*>& childsForMutation = this->m_childsForMutation;

	for (int i = 0; i < numOfParentsForMutation; ++i) {
		getObjectValuesOfChromo(childsForMutation[i], this);
		cout << childsForMutation[i]->objectValues.second << endl;
	}

	for (int i = 0; i < numOfParentsForCroOver; ++i) {
		cout << "i_crossover = " << i << endl;
		// 交叉
		Chromosome* chromPInitnew = childsForCroOver[i];
		cout << "code:" << endl;
		for (int i_code = 0; i_code < this->m_totalLenOfGACode; ++i_code) {
			cout << chromPInitnew->code[i_code] << " ";
		}
		getObjectValuesOfChromo(childsForCroOver[i], this);
		cout << childsForCroOver[i]->objectValues.second << endl;
	}
}

//选择下一代，同时保留精英
void GeneticAlgorithm::selectForNextGeneration() {
	const int numOfPop = this->m_numOfPop;
	const int numOfParentsForCroOver = this->m_numForCrossOver;
	const int numOfParentsForMutation = this->m_numForMutation;
	const int numElites = this->m_numElites;

	vector<Chromosome*>& popPool = this->m_popPool;
	vector<Chromosome*>& childsForCroOver = this->m_childsForCroOver;
	vector<Chromosome*>& childsForMutation = this->m_childsForMutation;
	vector<Chromosome*>& popPoolNew = this->m_popPoolNew;
	set<Chromosome*>& totalChromsSet = this->m_totalChromsSet;
	vector<Chromosome*>& eliteChroms = this->m_eliteChroms;

	int totalSize = numOfPop + numOfParentsForMutation + numOfParentsForCroOver;
	cout << "dfdfffk" << totalSize << endl;
	int i = 0;
	for (auto ele : popPool) popPoolNew[i++] = ele;
	for (auto ele : childsForMutation) popPoolNew[i++] = ele;
	for (auto ele : childsForCroOver) popPoolNew[i++] = ele;
	sort(popPoolNew.begin(), popPoolNew.end(), GeneticAlgorithm::compareChrom);

	vector<int> postionOfChosenChroms(numOfPop);
	chosenChromsByTournament2(numOfPop, totalSize, popPoolNew, postionOfChosenChroms, 0);


	// 保留中前numElites个精英
	for (int i = 0; i < numElites; ++i) {
		if(rand()%(i+1) == 0)
			popPool[i] = popPoolNew[i];
		cout << " " << popPool[i]->objectValues.second << endl;
	}

	for (int i = numElites; i < numOfPop; ++i) {
		int posOfChosenChrom = postionOfChosenChroms[i];
		popPool[i] = popPoolNew[posOfChosenChrom];
		cout << " uobj=" << posOfChosenChrom << " " << popPool[i]->objectValues.second << endl;
	}
	cout << " *****h8**8888" << endl;

	// 1/5+约1/200的概率保留每个交叉个体
	//for (int i = 0; i < numOfParentsForCroOver; i++) 
	for (int i = 0; i < 0; i++)
	{
		int randPos = (totalSize - numElites) * (rand() * 1.0) / (double)(RAND_MAX + 1.0);
		randPos += numElites;
		if (rand() % 5 == 0) popPoolNew[randPos] = childsForCroOver[i];
	}

	// 1/2+约1/200的概率保留每个变异个体
	//for (int i = 0; i < numOfParentsForMutation; ++i) 
	for (int i = 0; i < 0; ++i)
	{
		int randPos = (totalSize - numElites) * (rand() * 1.0) / (double)(RAND_MAX + 1.0);
		randPos += numElites;
		if (rand() % 5 == 0) popPoolNew[randPos] = childsForMutation[i];
	}

	// 为交叉变异分配空的染色体对象
	set<Chromosome*> chosenChromsSet(popPool.begin(), popPool.end());
	int j = 0;
	for (auto& ele : totalChromsSet) {
		if ((chosenChromsSet.find(ele) == chosenChromsSet.end()) && (totalChromsSet.find(ele) != totalChromsSet.end()))
		{
			childsForCroOver[j++] = ele;
			if (j == numOfParentsForCroOver) break;
		}
	}
	j = 0;
	for (auto& ele : totalChromsSet) {
		if ((chosenChromsSet.find(ele) == chosenChromsSet.end()) && (totalChromsSet.find(ele) != totalChromsSet.end()))
		{
			childsForMutation[j++] = ele;
			if (j == numOfParentsForMutation) break;
		}
	}

	/*
	Chromosome* chrom1, * chrom2;
	for (int j = 0; j  < numOfPop; ++j) {
		int position1 = totalSize * (rand() * 1.0) / (RAND_MAX + 1.0);
		int position2 = totalSize * (rand() * 1.0) / (RAND_MAX + 1.0);
		cout << "j = " << j <<endl;
		cout << "  position1="<< position1 <<endl;
		cout << "  position2=" << position2 << endl;
		if (position1 == position2) { --j; continue; }

		if (position1 < numOfPop) chrom1 = popPool[position1];
		else if (position1 < (numOfPop + numOfParentsForMutation))  chrom1 = childsForMutation[position1- numOfPop];
		else  chrom1 = childsForCroOver[position2 - numOfPop - numOfParentsForMutation];

		if (position2 < numOfPop) chrom2 = popPool[position2];
		else if (position2 < (numOfPop + numOfParentsForMutation))  chrom2 = childsForMutation[position2- numOfPop];
		else  chrom2 = childsForCroOver[position2 - numOfPop - numOfParentsForMutation];

		if (chrom1->objectValues.first < chrom2->objectValues.first) {
			posOfChoosenChoms.insert(chrom1);
			popPoolNew[j] = chrom1;
			posOfChoosenChom.insert(position1);
			popPoolTmps[j] = position1;
		}
		else {
			posOfChoosenChoms.insert(chrom2);
			popPoolNew[j] = chrom2;
			posOfChoosenChom.insert(position2);
			popPoolTmps[j] = position2;
		}
	}
	*/

	/*
	// 释放内存
	for (auto& ele : popPool) {
		Chromosome* chrom = ele;
		if (posOfChoosenChoms.find(chrom) == posOfChoosenChoms.end()) delete chrom;
	}

	cout << "giii" << endl;
	for (auto ele : childsForMutation)
		if (posOfChoosenChoms.find(ele) == posOfChoosenChoms.end()) delete ele;
	cout << "zdddd" << endl;
	for (auto ele : childsForCroOver)
		if (posOfChoosenChoms.find(ele) == posOfChoosenChoms.end()) delete ele;
	cout << "jbbbjb" << endl;

	*/

	// 更新种群
	//popPool = popPoolNew;
	cout << "kkeiweie" << endl;
};

void GeneticAlgorithm::printBestObjVal() {
	vector<Chromosome*>& popPool = this->m_popPool;
	list<pair<double, double>>& recordOfBestObjVals = m_recordOfBestObjVals;
	int iter = this->m_curGeneration;

	double minObjVal = DBL_MAX;
	Chromosome* bestChrom = nullptr;
	cout << "  Objectives of the chromesomes are:" << endl;
	for (auto& chromP : popPool) {
		if (minObjVal > chromP->objectValues.second) {
			minObjVal = chromP->objectValues.second;
			bestChrom = chromP;
		}
		cout << chromP->objectValues.second << " ";
	}
	recordOfBestObjVals.emplace_back(bestChrom->objectValues);

	cout << endl;
	cout << "  The best objecive value in iteration " << iter << " is: " << minObjVal << endl;
	cout << "    The best solution in iteration " << iter << " is: " << endl;
	for (int i = 0; i < bestChrom->codeLen; ++i) {
		cout << bestChrom->code[i] << " ";
	}
	cout << endl;
};

void GeneticAlgorithm::runGA() {
	int totalGeneration = this->m_totalGeneration;
	int& curGeneration = this->m_curGeneration;

	cout << "\n\n\n\n\n\n\n" << endl;
	cout << "CurGeneration=" << curGeneration << endl;
	clock_t startTime, endTime;
	startTime = clock();
	char a;
	cin >> a;
	for (; curGeneration <= totalGeneration; ++curGeneration) {
		cout << "CurGeneration=" << curGeneration << endl;
		char a;
		//cin >> a;
		//break;
		
		//this->localSearch();
		this->localSearch_Parallel();

		cout << " localsearch of" << curGeneration << endl;
		this->m_coeff_pressure *= this->m_pressureBaseNum;  // 更新轮盘赌选择压力系数

		cout << " m_coeff_pressure=" << m_coeff_pressure << endl;

		this->resetChildChroms();
		cout << " initializeChildChroms" << endl;
		this->crossoverOfPop();
		cout << " crossoverOfPop" << endl;
		this->mutationOfPop_JobTardiness();  // this->mutationOfPop();  this->mutationOfPop_JobTardiness();
		cout << " mutationOfPop" << endl;
		this->getObjValForChilds();
		cout << " getObjValForChilds" << endl;
		this->selectForNextGeneration();
		cout << " selectForNextGeneration" << endl;
		this->printBestObjVal();

		// if (curGeneration == 40) break;
	}
	endTime = clock();
	double t_sec = (double)(endTime - startTime) / CLOCKS_PER_SEC;

	cout << "The run time is: " << t_sec << "s  " << t_sec / 60 << "min" << endl;

	for (auto& ele : this->m_recordOfBestObjVals) {
		cout << ele.second << ' ';
	}
	cout << endl;

	cout << "why" << endl;
	for (int i = 0; i < this->m_numOfPop; ++i) {
		cout << "numOfChrome:" << i << " obj="<< this->m_popPool[i]->objectValues.second <<  endl;
		for (int j = 0; j < this->m_popPool[i]->tardinessOfjobs.size(); ++j) {
			cout << j << ":" << this->m_popPool[i]->tardinessOfjobs[j] << "   ";
		}	
		cout << endl;
	}

};

GeneticAlgorithm:: ~GeneticAlgorithm() {
	for (auto& chromP : this->m_popPool) delete chromP;
	for (auto& chromP : this->m_childsForCroOver) delete chromP;
	for (auto& chromP : this->m_childsForMutation) delete chromP;
	for (auto& chromP : this->m_popPoolNew) delete chromP;

	for (auto& jobInfo : this->m_jobOrderTmp2) delete jobInfo.second;
	for (auto& machInfo : this->m_machsMapTmp2) delete machInfo.second;
};




// --------GA获取目标函数值相关--------

// GA解码用，可以获取job的拖期时长————获取目标函数值 <总延期时间(小时), 加工所有工件所需的时间长度(小时)>
pair<double, double> getObjValsForChrom(vector<pair<string, Job*>>& jobOrder, map<string, Mach*>& machsMap, Chromosome* chromP)  // get object value
{
	chromP->delayJobs.clear();
	double totalDueTime(0);  // 总延期时间（小时）
	ptime timeOfStart;       // 开始排程的时间点
	ptime timeOfCompletion;  // 所有工件加工完的时间点
	double makeSpan(0);      // 加工所有工件所需的时间长度（小时）
	int i_numOfJob(0);

	for (auto& jobInfo : jobOrder)  // 遍历所有job
	{
		Job* curJobP = jobInfo.second;
		if (0 == curJobP->m_allocatedTimeWin.size())
			continue;
		time_period lastTimeWin = (curJobP->m_allocatedTimeWin.end() - 1)->second;
		ptime dueDate = curJobP->m_dueDateOfOrder;
		double delayTime = timeDuration2Double(lastTimeWin.last() - dueDate);
		delayTime = delayTime >= 0 ? delayTime : 0;
		//cout<<"delayTime="<< delayTime <<endl;
		totalDueTime += delayTime;
		if (0 == i_numOfJob)
		{
			timeOfStart = curJobP->m_startDateOfOrder;
			timeOfCompletion = lastTimeWin.last();
			//cout << "timeOfCompletion_or=" << to_iso_extended_string(timeOfCompletion) << endl;
			//cout << "timeOfStart_or=" << to_iso_extended_string(timeOfStart) << endl;
		}
		else
		{
			if (timeOfCompletion < lastTimeWin.last())
				timeOfCompletion = lastTimeWin.last();
			if ((curJobP->m_startDateOfOrder) < timeOfStart)
				timeOfStart = curJobP->m_startDateOfOrder;
		}
		chromP->tardinessOfjobs[i_numOfJob] = delayTime;
		if (delayTime > 0.0) chromP->delayJobs.emplace(make_pair(i_numOfJob, delayTime));
		i_numOfJob++;
		//std::cout << "delay time of "<<curJobP->m_jobCode << " is: " << delayTime<<std::endl;
	}

	//cout<< "timeOfCompletion=" <<to_iso_extended_string(timeOfCompletion)<<endl;
	//cout<< "timeOfStart=" << to_iso_extended_string(timeOfStart) << endl;

	
	makeSpan = timeDuration2Double(timeOfCompletion - timeOfStart);
	//std::cout << "totalDueTime=　" << totalDueTime << std::endl;
	//std::cout << "makeSpan = " << makeSpan << std::endl;
	//std::cout << std::endl;
	return make_pair(totalDueTime, makeSpan);
};

// 获取染色体的目标函数值
pair<double, double> getObjectValuesOfChromo(Chromosome* chromP, GeneticAlgorithm* populationP, bool isPrint)
{
	const int totalLenOfGACode = populationP->m_totalLenOfGACode;

	//cout <<"uiuiiiiiiiii" <<endl;
	vector<pair<string, Job*>>& jobOrder = populationP->m_jobOrderTmp2;
	map<string, Job*>& jobsMapTemp = populationP->m_jobsMapTmp2;
	map<string, Mach*>& machsMapTemp = populationP->m_machsMapTmp2;
	// 用来排气垫炉之前的工序
	vector<pair<string, Job*>>* jobOrderBefAirP = populationP->m_jobOrderBefAirP;
	map<string, Job*>* jobsMapBefAirP = populationP->m_jobsMapBefAirP;
	map<string, Mach*>* machsMapBefAirP = populationP->m_machsMapBefAirP;

	resetJobsTemp(jobOrder, *jobOrderBefAirP);
	resetMachsMapTemp(machsMapTemp, *machsMapBefAirP);

	// 排入气垫炉中间的工序
	for (int i_pro = 0; i_pro < totalLenOfGACode; ++i_pro) {
		int jobIndex = chromP->code[i_pro];
		Job* curJobP = jobOrder[jobIndex].second;
		int machIndex = curJobP->m_curMachIndex;

		pair<string, unsigned>& machCodeOfCurJob = curJobP->m_proceMachs[machIndex];

		bool isSuccess = false;
		if (CodeOfBellFurn == machCodeOfCurJob.first)  // 是钟罩炉
		{
			Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
			isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
		}
		else if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // 是气垫炉组中的机器
		{
			Mach_AirFurnace* curMachP = static_cast<Mach_AirFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
			isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
		}
		else {  //其他
			Mach* curMachP = machsMapTemp[machCodeOfCurJob.first];
			isSuccess = insertJob(*curJobP, *curMachP, machIndex);
		}
		if (isSuccess) curJobP->m_curMachIndex = machIndex + 1;
	}

	// 排入剩余工序
	Job* curJobP;
	for (auto& jobInfo : jobOrder)
	{
		curJobP = jobInfo.second;
		for (int machIndex = curJobP->m_curMachIndex; machIndex < curJobP->m_proceMachs.size(); ++machIndex)   // 遍历某个job的剩余工序
		{
			pair<string, unsigned>& machCodeOfCurJob = curJobP->m_proceMachs[machIndex];
			bool isSuccess;
			if (CodeOfBellFurn == machCodeOfCurJob.first)  // 是钟罩炉
			{
				Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // 是气垫炉组中的机器
			{
				Mach_AirFurnace* curMachP = static_cast<Mach_AirFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else  //其他
			{
				Mach* curMachP = machsMapTemp[machCodeOfCurJob.first];
				isSuccess = insertJob(*curJobP, *curMachP, machIndex);
			}
			if (isSuccess) curJobP->m_curMachIndex = machIndex + 1;
		}
	}

	// 获取目标值
	pair<double, double> objVals = getObjValsForChrom(jobOrder, machsMapTemp, chromP);
	chromP->objectValues = objVals;

	// 打印最终排程结果
	if (isPrint) { 
		cout << "testPrint!!!" << endl;  printFinalRes(jobsMapTemp, machsMapTemp); 
	}
	return objVals;
}

// 获取染色体的目标函数值
pair<double, double> getObjectValuesOfChromo_Parallel(Chromosome* chromP, GeneticAlgorithm* populationP, bool isPrint)
{
	const int totalLenOfGACode = populationP->m_totalLenOfGACode;

	/*
	vector<pair<string, Job*>>& jobOrder = populationP->m_jobOrderTmp2;
	map<string, Job*>& jobsMapTemp = populationP->m_jobsMapTmp2;
	map<string, Mach*>& machsMapTemp = populationP->m_machsMapTmp2;
	*/

	vector<pair<string, Job*>> jobOrder;  // 引用变拷贝
	map<string, Job*> jobsMapTemp;         // 引用变拷贝
	map<string, Mach*> machsMapTemp;      // 引用变拷贝
	// 须初始化job和mach对象
	initJobsTemp(jobsMapTemp, jobOrder, populationP->m_jobOrderTmp2);  // 拷贝Job--把jobOrder中的所有job拷贝一下，放入到jobsMapTemp中
	initMachsMapTemp(machsMapTemp, populationP->m_machsMapTmp2);       // 拷贝machine--把machsMap中的所有machine拷贝一下，放入到machsMapTemp中
	// 为job和mach预先分配时间窗空间
	allocateTimeWinSpace(jobsMapTemp, machsMapTemp);

	//cout << "cfc" << endl;

	// 用来排气垫炉之前的工序
	vector<pair<string, Job*>>* jobOrderBefAirP = populationP->m_jobOrderBefAirP;
	map<string, Job*>* jobsMapBefAirP = populationP->m_jobsMapBefAirP;
	map<string, Mach*>* machsMapBefAirP = populationP->m_machsMapBefAirP;

	//cout << "aza" << endl;
	resetJobsTemp(jobOrder, *jobOrderBefAirP);

	//cout << "wvw" << endl;
	resetMachsMapTemp(machsMapTemp, *machsMapBefAirP);

	//cout << "aza" << endl;

	// 排入气垫炉中间的工序
	for (int i_pro = 0; i_pro < totalLenOfGACode; ++i_pro) {
		int jobIndex = chromP->code[i_pro];
		Job* curJobP = jobOrder[jobIndex].second;
		int machIndex = curJobP->m_curMachIndex;

		pair<string, unsigned>& machCodeOfCurJob = curJobP->m_proceMachs[machIndex];

		bool isSuccess = false;
		if (CodeOfBellFurn == machCodeOfCurJob.first)  // 是钟罩炉
		{
			Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
			isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
		}
		else if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // 是气垫炉组中的机器
		{
			Mach_AirFurnace* curMachP = static_cast<Mach_AirFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
			isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
		}
		else {  //其他
			Mach* curMachP = machsMapTemp[machCodeOfCurJob.first];
			isSuccess = insertJob(*curJobP, *curMachP, machIndex);
		}
		if (isSuccess) curJobP->m_curMachIndex = machIndex + 1;
	}

	//cout << "nmn" << endl;

	// 排入剩余工序
	Job* curJobP;
	for (auto& jobInfo : jobOrder)
	{
		curJobP = jobInfo.second;
		for (int machIndex = curJobP->m_curMachIndex; machIndex < curJobP->m_proceMachs.size(); ++machIndex)   // 遍历某个job的剩余工序
		{
			pair<string, unsigned>& machCodeOfCurJob = curJobP->m_proceMachs[machIndex];
			bool isSuccess;
			if (CodeOfBellFurn == machCodeOfCurJob.first)  // 是钟罩炉
			{
				Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // 是气垫炉组中的机器
			{
				Mach_AirFurnace* curMachP = static_cast<Mach_AirFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else  //其他
			{
				Mach* curMachP = machsMapTemp[machCodeOfCurJob.first];
				isSuccess = insertJob(*curJobP, *curMachP, machIndex);
			}
			if (isSuccess) curJobP->m_curMachIndex = machIndex + 1;
		}
	}

	// 获取目标值
	pair<double, double> objVals = getObjValsForChrom(jobOrder, machsMapTemp, chromP);
	chromP->objectValues = objVals;


	// 释放内存，释放new出的job和mach-多线程用到
	releaseMemoryOfTmpJobsMachs(jobOrder, machsMapTemp);


	// 打印最终排程结果
	if (isPrint) {
		cout << "testPrint!!!" << endl;  printFinalRes(jobsMapTemp, machsMapTemp);
	}
	return objVals;
}

// --------GA获取目标函数值相关--------



// 由编码信息获取初始解（染色体）
void getJobChromoFromInitSolut(vector<pair<Job*, pair<int, int>>>& codeInfoOfGA, vector<pair<string, Job*>>& jobOrder
	, vector<pair<pair<Job*, int>, ptime>>& jobChromPre, Chromosome* chromPInit, Chromosome* chromPInit2)
{
	int j = 0;
	for (int i = 0; i < jobOrder.size(); ++i) {
		Job* jobP = jobOrder[i].second;
		if (jobOrder[i].second != codeInfoOfGA[i].first) { cout << "error with job squence about GA!" << endl; char c; cin >> c; }
		if (codeInfoOfGA[i].second.first < 0) continue;
		
		for (int i_pro = codeInfoOfGA[i].second.first; i_pro <= codeInfoOfGA[i].second.second; ++i_pro) {
			jobChromPre[j] = make_pair(make_pair(jobP, i), jobP->m_allocatedTimeWin[i_pro].second.begin());
			chromPInit2->code[j] = i;
			++j;
		}
	}
	sort(jobChromPre.begin(), jobChromPre.end(), compareJobPPtime); sort(jobChromPre.begin(), jobChromPre.end(), compareJobPPtime);

	for (int i = 0; i < jobChromPre.size(); ++i) {
		chromPInit->code[i] = jobChromPre[i].first.second;
	}
}







