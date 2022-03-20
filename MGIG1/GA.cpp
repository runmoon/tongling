#pragma once

#include<GA.h>
#include<head.h>

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

// 初始化种群（染色体集合）
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

	// 由初始排产结果，随机交换生成初始解
	double rate = 0.1;  // 交换比例的一般
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

// 初始化GA中用于对染色体解码（排产）信息
void GeneticAlgorithm::initializeInfoOfBef(vector<pair<string, Job*>>* jobOrderBefP, map<string, Job*>* jobsMapBefP, map<string, Mach*>* machsMapBefP) {
	
	// 1、用jobOrderBef的Job顺序来进行染色体编码；2、存储解码前的初始的Job和Mach信息
	this->m_jobOrderBefP = jobOrderBefP;
	this->m_jobsMapBefP = jobsMapBefP;
	this->m_machsMapBefP = machsMapBefP;

	// 解码时用的，计算染色体适值
	this->m_jobOrderTmp2 = { vector<pair<string, Job*>>(this->m_jobOrderBefP->size()) };
	initJobsTemp(this->m_jobsMapTmp2, this->m_jobOrderTmp2, *(this->m_jobOrderBefP));
	initMachsMapTemp(this->m_machsMapTmp2, *(this->m_machsMapBefP));
};

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

	cout << endl;
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

	/*
	for (auto& ele : (*this->m_jobOrderBefP)) {
		Job* jobP = ele.second;
		cout << "jobCode:" << jobP->m_jobCode << endl;
		cout << "  curMachIndex:" << jobP->m_curMachIndex << endl;
		cout << "  size:" << jobP->m_allocatedTimeWin.size() << endl;
	}
	for (auto& ele : (*this->m_machsMapBefP)) {
		Mach* machP = ele.second;
		cout << "machPCode:" << machP->m_machCode << endl;
		cout << "  size:" << machP->m_allocatedTimeWin.size() << endl;
	}
	char c; cin >> c;
	*/

	return;
};

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
	for (; index < numOfChromsToChose;) {
		int indexOfChrom = -1;
		int p1 = numOfPop * (rand() * 1.0) / (double)(RAND_MAX + 1.0);
		int p2 = numOfPop * (rand() * 1.0) / (double)(RAND_MAX + 1.0);
		if (fitnessArr[p1] > fitnessArr[p2])
			indexOfChrom = p1;
		else
			indexOfChrom = p2;
		chosenChroms[index++] = indexOfChrom;
	}
};

// 锦标赛选择，挑选不好的扔出来
void GeneticAlgorithm::chosenChromsByTournament2(const int numOfChromsToChose, const int numOfPop, vector<Chromosome*>& popPool, vector<int>& chosenChroms, int isPutBack) {
	//static double coeff_pressure = coeff_pressure * coeff_pressureBaseNum;
	double coeff_pressure = this->m_coeff_pressure;
	double coeff_additionalNum = this->m_coeff_additionalNum;
	int numToDrop = numOfPop - numOfChromsToChose;

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

	cout << "yiosdfugh" << endl;
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
	cout << "  index=" << index << endl;

	cout << "end" << endl;

};


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
};

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
};

// 获取变异位置，使得脱期job往前移动
pair<int, int> GeneticAlgorithm::getPositionsForMutation(Chromosome* chromP) {
	const int totalLenOfGACode = this->m_totalLenOfGACode;

	int positionOfCode1 = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
	int positionOfCode2;

	// 随机选择一个脱期job
	int numOfDelaied = chromP->delayJobs.size();
	int jobIndex = numOfDelaied * (1.0 * rand()) / (RAND_MAX + 1.0);
	int jobNum = chromP->delayJobs[jobIndex];

	// 获取脱期job的最后一个工序的位置，再在前面随机选择一个位置
	for (int i_pos = 0; i_pos < totalLenOfGACode; ++i_pos) {
		if (chromP->code[i_pos] == jobNum) positionOfCode1 = i_pos;
	}
	positionOfCode2 = positionOfCode1 * (1.0 * rand()) / (RAND_MAX + 1.0);

	return make_pair(positionOfCode1, positionOfCode2);
};

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


	// 限制片段长度在小范围内
	const int maxLength = 20;
	if (length > maxLength) {
		int rest = length % maxLength;
		rest = rest == 0 ? maxLength : rest;
		if (rand() % 2 == 1)
			right = left + rest - 1;
		else
			left = right - rest + 1;
		length = rest;
	}
	//cout << "翻转length=" << length << endl;

	int i = left;
	int j = right;
	for (; i < j; ++i, --j) {
		swap(chromP->code[i], chromP->code[j]);
	}
};

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


	// 限制片段长度在小范围内
	const int maxLength = 20;
	if (length > maxLength) {
		int rest = length % maxLength;
		rest = rest == 0 ? maxLength : rest;
		if (rand() % 2 == 1) {
			left = left + length - rest;
			right = right + length - rest;
		}
		else{
			left = left + rest - 1;
			right = right + rest - 1;
		}
		length = rest;
	}
	//cout << "交换length=" << length << endl;


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
};

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


	// 限制截取长度在小范围内
	const int maxLength = 20;
	if (length > maxLength) {
		int rest = length % maxLength;
		rest = rest == 0 ? maxLength : rest;
		if (rand() % 2 == 1)
			right = left + rest - 1;
		else 
			left = right - rest + 1;
		length = rest;
	}
	//cout << "再插入length=" << length << endl;

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
};

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
};

// 邻域方式--抽取一个，再插入
void  GeneticAlgorithm::getNeighborByReinsertOne(Chromosome* chromP, const int totalLenOfGACode) {

	int selectedPos = -1;
	int insertPos = -1;

	while (selectedPos == insertPos) {
		selectedPos = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
		insertPos = totalLenOfGACode * (1.0 * rand()) / (RAND_MAX + 1.0);
	}

	int tmpCopy = chromP->code[selectedPos];
	if (insertPos > selectedPos)  // 往后插入 
	{
		memmove(&chromP->code[selectedPos], &chromP->code[selectedPos + 1], (insertPos - selectedPos) * sizeof(chromP->code[0]));
		chromP->code[insertPos] = tmpCopy;
	}
	else {                  // 往前插入
		memmove(&chromP->code[insertPos + 1], &chromP->code[insertPos], (selectedPos - insertPos) * sizeof(chromP->code[0]));
		chromP->code[insertPos] = tmpCopy;
	}
};


void GeneticAlgorithm::getNewChromeToNeighbor(Chromosome* chromP, const int totalLenOfGACode) {
	int tmp = rand() % 3;
	if (tmp == 0)
		this->getNeighborByReverse(chromP, totalLenOfGACode);
	else if (tmp == 1)
		this->getNeighborByExchangeSub(chromP, totalLenOfGACode);
	else
		this->getNeighborByReinsertSub(chromP, totalLenOfGACode);

};


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
				//for (auto& ele : chromPTmp->delayJobs)  chromP->delayJobs.insert(ele);
				chromP->delayJobs = chromPTmp->delayJobs;
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
				//for (auto& ele : chromPTmp->delayJobs)  chromP->delayJobs.insert(ele);
				chromP->delayJobs = chromPTmp->delayJobs;
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
				//for (auto& ele : chromPTmp->delayJobs)  chromP->delayJobs.insert(ele);
				chromP->delayJobs = chromPTmp->delayJobs;
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
				//for (auto& ele : chromPTmp->delayJobs)  chromP->delayJobs.insert(ele);
				chromP->delayJobs = chromPTmp->delayJobs;
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

// 对一个染色体做局部搜索
void GeneticAlgorithm::localSearchForCurChrome(Chromosome* chromP, GeneticAlgorithm* gaP, const int maxIter) {
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

		cout << "  iter=" << iter;
		cout << "  randm=" << tmp << endl;

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

		getObjectValuesOfChromo(chromPTmp, gaP);
		//cout << " iter=" << iter  << endl;
		isRenew = false;
		// 如果好，则更新
		if (chromPTmp->objectValues.second < chromP->objectValues.second) {
			chromP->code.assign(chromPTmp->code.begin(), chromPTmp->code.end());

			chromP->objectValues = chromPTmp->objectValues;
			chromP->tardinessOfjobs.assign(chromPTmp->tardinessOfjobs.begin(), chromPTmp->tardinessOfjobs.end());
			chromP->delayJobs = chromPTmp->delayJobs;
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
			//break;
		}
	}

	delete chromPTmp;
	cout << "    after--" << "obj_makespan=" << chromP->objectValues.second
		<< " obj_tardiness=" << chromP->objectValues.first << endl;
	return;

};

// 对一个染色体做局部搜索
void GeneticAlgorithm::localSearchForCurChrome_Parallel(Chromosome* chromP, GeneticAlgorithm* gaP, const int maxIter, threadInfoOfLS* threadInfoP) {
	//const int totalLenOfGACode = this->m_totalLenOfGACode;
	const int totalLenOfGACode = chromP->codeLen;
	Chromosome* chromPTmp = new Chromosome(chromP->codeLen, chromP->rangeOfJob);
	
	cout << "thread_" << std::this_thread::get_id() << endl;

	bool isRenew = false;
	chromPTmp->code.assign(chromP->code.begin(), chromP->code.end());

	//cout << " maxIter=" << maxIter << endl;
	//cout << "    before--" << "obj_makespan=" << chromP->objectValues.second
	//	<< " obj_tardiness=" << chromP->objectValues.first << endl;

	for (int iter = 0; iter < maxIter; ++iter) {

		int tmp = rand() % 5;
		//int tmp = rand() % 3;

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

		//cout<<"tty"<<endl;
		//getObjectValuesOfChromo(chromPTmp, gaP);
		getObjectValuesOfChromo_Parallel(chromPTmp, gaP, threadInfoP);
		//cout << "ppo" << endl;

		//cout << " iter=" << iter  << endl;
		isRenew = false;
		// 如果好，则更新
		if (chromPTmp->objectValues.second < chromP->objectValues.second) {
			chromP->code.assign(chromPTmp->code.begin(), chromPTmp->code.end());

			chromP->objectValues = chromPTmp->objectValues;
			chromP->tardinessOfjobs.assign(chromPTmp->tardinessOfjobs.begin(), chromPTmp->tardinessOfjobs.end());
			chromP->delayJobs = chromPTmp->delayJobs;
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
			break;
		}
	}

	//cout << "    after--" << "obj_makespan=" << chromP->objectValues.second
	//	<< " obj_tardiness=" << chromP->objectValues.first << endl;
	delete chromPTmp;
	return;
};

// 一个线程对几个染色体做local search
void GeneticAlgorithm::localSearch_AThread(vector<Chromosome*>& chromPVec, GeneticAlgorithm* gaP, const int maxIter, threadInfoOfLS* threadInfoP) {

	for (int i = 0; i < chromPVec.size(); ++i) {
		Chromosome* chromP = chromPVec[i];
		localSearchForCurChrome_Parallel(chromP, gaP, maxIter, threadInfoP);

		//resetMachsMapTemp(threadInfoP->m_machsMapTmp, *(gaP->m_machsMapBefP));
		//resetJobsTemp(threadInfoP->m_jobOrderTmp, *(gaP->m_jobOrderBefP));
	}
};


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

};


void GeneticAlgorithm::localSearch_Parallel(const int num_thread, vector<threadInfoOfLS*>& threadInfos) {
	// 选择几个好个体
	// 对每一个染色体，迭代max_iter次，当新染色体好时，更新
	GeneticAlgorithm* gaP = this;
	const int numOfPop = this->m_numOfPop;
	const int maxIter = 50;  // 最大迭代次数
	const int numBestChrome = 5;  // 选择前几个最好的染色体
	const int numRandomChrome = 10;  // 再从后面随机选择一部分

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

	vector<thread> threads(num_thread);
	cout << "num_thread=" << num_thread << endl;

	index = 0;
	int round = NumOfTotalChosen / num_thread; // 需要几轮
	int restNum = NumOfTotalChosen % num_thread;  // 剩余
	vector<vector<Chromosome*>> chromeToLocalSearch(num_thread);
	for (int t = 0; t < num_thread; ++t) {
		int numOfChrom = round;
		if (t < restNum) numOfChrom += 1;
		for (int i = 0; i < numOfChrom; ++i) {
			chromeToLocalSearch[t].emplace_back(chosenChroms[index++]);
		}
	}

	for (int t = 0; t < num_thread; ++t) {
		cout << t << ":" << chromeToLocalSearch[t].size() << endl;
	}
	cout << "thread..." << endl;

	for (int t = 0; t < num_thread; ++t)
		threads[t] = thread(localSearch_AThread, chromeToLocalSearch[t], this, maxIter, threadInfos[t]);

	for (int t = 0; t < num_thread; ++t)
		threads[t].join();

	/*
	// 之前的多线程版本
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
	*/

};


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
};

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
	GeneticAlgorithm* gaP = this;
	int totalGeneration = this->m_totalGeneration;
	int& curGeneration = this->m_curGeneration;

	cout << "\n\n\n\n\n\n\n" << endl;
	cout << "CurGeneration=" << curGeneration << endl;
	clock_t startTime, endTime;
	startTime = clock();
	char a;
	cin >> a;
	
	//const unsigned int num_thread = thread::hardware_concurrency();  // 获取硬件所支持的线程个数
	const int num_thread = 5;  // 这里最高设置为6；设置为5时，CPU最高占用85%
	vector<threadInfoOfLS*> threadInfos = initThreadsInfoOfLS(num_thread, gaP->m_jobOrderBefP, gaP->m_machsMapBefP);

	for (; curGeneration <= totalGeneration; ++curGeneration) {
		cout << "CurGeneration=" << curGeneration << endl;
		char a;
		//cin >> a;
		//break;
		
		//this->localSearch();
		this->localSearch_Parallel(num_thread, threadInfos);

		cout << " localsearch of generation:" << curGeneration << endl;
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
	releaseThreadsInfoOfLS(num_thread, threadInfos);


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
	double totalDueTime(0.0);   // 总延期时间（小时）
	ptime timeOfStart;          // 开始排程的时间点
	ptime timeOfCompletion;     // 所有工件加工完的时间点
	double makeSpan(0.0);       // 加工所有工件所需的时间长度（小时）
	int i_numOfJob(0);

	double totalRuntime(0.0);   // 总流水时间

	for (auto& jobInfo : jobOrder)  // 遍历所有job
	{
		Job* curJobP = jobInfo.second;
		if (0 == curJobP->m_allocatedTimeWin.size())
			continue;
		const time_period& lastTimeWin = (curJobP->m_allocatedTimeWin.end() - 1)->second;
		ptime dueDate = curJobP->m_dueDateOfOrder;
		double delayTime = timeDuration2Double(lastTimeWin.last() - dueDate);
		delayTime = delayTime >= 0 ? delayTime : 0;
		//cout<<"delayTime="<< delayTime <<endl;
		totalDueTime += delayTime;

		//计算流水时间
		double runingTime = timeDuration2Double(lastTimeWin.last()- (curJobP->m_allocatedTimeWin.begin())->second.begin());
		runingTime = runingTime >= 0 ? runingTime : 0;
		totalRuntime += runingTime;

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
		if (delayTime > 0.0) chromP->delayJobs.emplace_back(i_numOfJob);
		i_numOfJob++;
		//std::cout << "delay time of "<<curJobP->m_jobCode << " is: " << delayTime<<std::endl;
	}

	//cout<< "timeOfCompletion=" <<to_iso_extended_string(timeOfCompletion)<<endl;
	//cout<< "timeOfStart=" << to_iso_extended_string(timeOfStart) << endl;

	
	makeSpan = timeDuration2Double(timeOfCompletion - timeOfStart);
	//std::cout << "totalDueTime=　" << totalDueTime << std::endl;
	//std::cout << "makeSpan = " << makeSpan << std::endl;
	//std::cout << std::endl;

	//return make_pair(totalDueTime, makeSpan);
	return make_pair(makeSpan, totalRuntime);
};

// 获取染色体的目标函数值
pair<double, double> getObjectValuesOfChromo(Chromosome* chromP, GeneticAlgorithm* gaP, bool isPrint)
{
	const int totalLenOfGACode = gaP->m_totalLenOfGACode;

	vector<pair<string, Job*>>& jobOrder = gaP->m_jobOrderTmp2;
	map<string, Job*>& jobsMapTemp = gaP->m_jobsMapTmp2;
	map<string, Mach*>& machsMapTemp = gaP->m_machsMapTmp2;
	// 用来排气垫炉之前的工序
	vector<pair<string, Job*>>* jobOrderBefP = gaP->m_jobOrderBefP;
	map<string, Job*>* jobsMapBefP = gaP->m_jobsMapBefP;
	map<string, Mach*>* machsMapBefP = gaP->m_machsMapBefP;

	resetJobsTemp(jobOrder, *jobOrderBefP);
	resetMachsMapTemp(machsMapTemp, *machsMapBefP);

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
};

// 获取染色体的目标函数值
pair<double, double> getObjectValuesOfChromo_Parallel(Chromosome* chromP, GeneticAlgorithm* gaP, threadInfoOfLS* threadInfoP, bool isPrint)
{
	const int totalLenOfGACode = gaP->m_totalLenOfGACode;

	/*
	vector<pair<string, Job*>> jobOrder;  // 引用变拷贝
	map<string, Job*> jobsMapTemp;        // 引用变拷贝
	map<string, Mach*> machsMapTemp;      // 引用变拷贝
	// 须初始化job和mach对象
	initJobsTemp(jobsMapTemp, jobOrder, gaP->m_jobOrderTmp2);  // 拷贝Job--把jobOrder中的所有job拷贝一下，放入到jobsMapTemp中
	initMachsMapTemp(machsMapTemp, gaP->m_machsMapTmp2);       // 拷贝machine--把machsMap中的所有machine拷贝一下，放入到machsMapTemp中
	
	// 为job和mach预先分配时间窗空间
	allocateTimeWinSpace(jobsMapTemp, machsMapTemp);
	*/

	vector<pair<string, Job*>>& jobOrder = threadInfoP->m_jobOrderTmp;
	map<string, Job*>& jobsMapTemp = threadInfoP->m_jobsMapTmp;
	map<string, Mach*>& machsMapTemp = threadInfoP->m_machsMapTmp;

	/*
	// 用来排气垫炉之前的工序
	vector<pair<string, Job*>>* jobOrderBefP = gaP->m_jobOrderBefP;
	map<string, Job*>* jobsMapBefP = gaP->m_jobsMapBefP;
	map<string, Mach*>* machsMapBefP = gaP->m_machsMapBefP;
	resetJobsTemp(jobOrder, *jobOrderBefP);
	resetMachsMapTemp(machsMapTemp, *machsMapBefP);
	*/

	resetJobsToOrigin(jobOrder);  	// 无预先排产时--把jobOrder中的job的排产时间窗清0
	resetMachsMapToOrigin(machsMapTemp);  	// 无预先排产时--把machsMap中的排产时间窗清0

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

	/*
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
	*/

	// 获取目标值
	pair<double, double> objVals = getObjValsForChrom(jobOrder, machsMapTemp, chromP);
	chromP->objectValues = objVals;


	// 释放内存，释放new出的job和mach-多线程用到
	//releaseMemoryOfTmpJobsMachs(jobOrder, machsMapTemp);


	// 打印最终排程结果
	if (isPrint) {
		cout << "testPrint!!!" << endl;  printFinalRes(jobsMapTemp, machsMapTemp);
	}
	return objVals;
};

// --------ENDOF GA获取目标函数值相关--------




// 由初始排产结果获取染色体编码：排产结果须存入jobOrderScheduled中；编码信息须存入encodeInfoOfGA中
void initChromCodesByInitSedul(vector<pair<string, Job*>>& jobOrderScheduled, const map<string, pair<int, pair<int, int>>>& encodeInfoOfGA
	, const int totalLenOfGACode, Chromosome* chromPInit, Chromosome* chromPInit2)
{
	vector<pair<pair<Job*, int>, ptime>> jobChromPre(totalLenOfGACode);
	int j = 0;
	
	//cout << "chromPInit2->code" << chromPInit2->code.size() << endl;
	//cout << "jobChromPre->code" << jobChromPre.size() << endl;

	for (int i = 0; i < jobOrderScheduled.size(); ++i) {
		Job* jobP = jobOrderScheduled[i].second;
		const pair<int, pair<int, int>>& encodeInfo = encodeInfoOfGA.at(jobP->m_jobCode);
		int codeNum = encodeInfo.first;

		if (encodeInfo.second.first < 0) continue;
		//cout << "  jobPCode=" << jobP->m_jobCode << "  codeNum="<< codeNum << " "<< encodeInfo.second.first  <<"-" << encodeInfo.second.second << endl;
		//cout << "  jobP->m_allocatedTimeWin.szie()" << jobP->m_allocatedTimeWin.size() << endl;
		if (jobP->m_allocatedTimeWin.size() != (encodeInfo.second.second - encodeInfo.second.first + 1)) {
			cout << "error with job squence about GA!" << endl;
			char cn; cin >> cn;
		}
		for (int i_pro = encodeInfo.second.first; i_pro <= encodeInfo.second.second; ++i_pro) {
			jobChromPre[j] = make_pair(make_pair(jobP, codeNum), jobP->m_allocatedTimeWin[i_pro].second.begin());
			chromPInit2->code[j] = codeNum;
			++j;
		}
	}
	sort(jobChromPre.begin(), jobChromPre.end(), compareJobPPtime); sort(jobChromPre.begin(), jobChromPre.end(), compareJobPPtime);

	for (int i = 0; i < jobChromPre.size(); ++i) {
		chromPInit->code[i] = jobChromPre[i].first.second;
	}

	// 打印
	if (0) {
		cout << " jobChromPre.size()=" << jobChromPre.size() << endl;
		for (int i = 0; i < jobChromPre.size(); ++i) {
			cout << "  " << i << endl;
			cout << jobChromPre[i].first.first->m_jobCode << "    " << jobChromPre[i].first.second << "    "
				<< to_iso_extended_string(jobChromPre[i].second) << endl;
		}

		cout << " 染色体1：" << endl;
		for (auto iter = chromPInit->code.begin(); iter != chromPInit->code.end(); ++iter) {
			cout << *iter << ", ";
		}
		cout << endl;
		cout << " 染色体2：" << endl;
		for (auto iter = chromPInit2->code.begin(); iter != chromPInit2->code.end(); ++iter) {
			cout << *iter << ", ";
		}
		char c4; cin >> c4;
	}

	return;
};


// 由预先代码获取染色体编码
void initChromCodesByPreCode(const int totalLenOfGACode, Chromosome* chromPInit, Chromosome* chromPInit2)
{
	/*
	// NEH--初始排序-总加工时间；目标-makespan
	chromPInit->code = { 3, 0, 22, 3, 43, 38, 21, 43, 79, 3, 79, 31, 91, 31, 47, 79, 44, 47, 79, 31, 44, 8, 15, 79, 84, 72, 79, 96, 91, 10, 91
		, 11, 91, 79, 80, 81, 78, 91, 35, 35, 50, 0, 91, 8, 94, 8, 90, 91, 8, 37, 59, 37, 91, 23, 52, 8, 12, 15, 73, 8, 15, 15, 24, 22, 15, 38
		, 15, 21, 72, 54, 84, 53, 51, 84, 80, 96, 84, 72, 19, 81, 2, 96, 78, 19, 43, 20, 80, 84, 72, 81, 43, 50, 78, 43, 69, 84, 0, 72, 96, 43
		, 74, 2, 69, 47, 94, 50, 43, 20, 74, 0, 72, 47, 96, 80, 59, 0, 94, 47, 16, 1, 47, 44, 96, 0, 81, 47, 44, 0, 44, 30, 78, 29, 44, 0, 39
		, 44, 59, 27, 50, 60, 10, 0, 82, 94, 50, 90, 4, 10, 11, 1, 35, 16, 83, 10, 23, 90, 50, 85, 90, 35, 11, 24, 28, 23, 10, 11, 35, 24, 22
		, 48, 35, 70, 38, 35, 22, 37, 10, 11, 23, 70, 87, 21, 38, 37, 21, 5, 23, 24, 37, 80, 11, 37, 23, 65, 37, 81, 22, 6, 80, 38, 78, 67
		, 88, 21, 94, 81, 80, 52, 93, 94, 52, 12, 57, 52, 68, 73, 12, 78, 81, 12, 94, 64, 73, 52, 68, 88, 19, 89, 94, 73, 12, 78, 19, 75
		, 90, 66, 73, 52, 87, 5, 90, 25, 19, 65, 6, 58, 2, 19, 14, 19, 90, 69, 2, 13, 20, 24, 97, 90, 55, 20, 74, 22, 69, 56, 74, 20, 69
		, 38, 9, 20, 12, 74, 26, 21, 20, 69, 71, 74, 99, 69, 73, 89, 64, 16, 17, 69, 40, 1, 16, 16, 24, 1, 92, 69, 1, 54, 34, 69, 41, 54
		, 53, 22, 25, 66, 70, 95, 54, 69, 53, 36, 70, 46, 53, 54, 7, 38, 45, 70, 53, 18, 51, 76, 21, 1, 51, 16, 42, 51, 77, 48, 1, 54
		, 48, 51, 48, 1, 53, 1, 88, 1, 68, 88, 51, 88, 68, 1, 67, 30, 68, 87, 5, 29, 30, 16, 30, 65, 39, 29, 5, 6, 39, 27, 29, 30, 75
		, 5, 60, 6, 30, 82, 29, 27, 6, 26, 60, 27, 4, 83, 29, 27, 85, 82, 4, 82, 27, 64, 71, 99, 82, 89, 83, 85, 83, 82, 87, 65, 87
		, 64, 85, 83, 28, 17, 40, 66, 89, 65, 28, 25, 83, 66, 85, 28, 64, 25, 89, 85, 66, 48, 48, 28, 28, 87, 65, 87, 1, 67, 92, 34
		, 68, 65, 67, 70, 64, 1, 89, 70, 66, 67, 70, 5, 41, 95, 5, 75, 67, 70, 36, 46, 75, 6, 67, 70, 75, 6, 68, 26, 70, 26, 64, 26
		, 75, 45, 7, 26, 71, 88, 99, 88, 71, 71, 99, 87, 65, 87, 76, 18, 17, 40, 17, 87, 17, 65, 67, 40, 93, 40, 57, 93, 87, 65, 67
		, 57, 87, 65, 67, 89, 57, 57, 65, 66, 67, 99, 88, 67, 88, 25, 88, 25, 25, 88, 42, 77, 88, 25, 68, 92, 25, 92, 34, 68, 64
		, 58, 92, 34, 58, 68, 64, 89, 92, 34, 58, 92, 41, 68, 64, 14, 58, 66, 89, 41, 95, 14, 13, 68, 64, 89, 66, 14, 97, 13, 14
		, 13, 66, 97, 41, 64, 97, 36, 66, 55, 13, 46, 36, 55, 89, 99, 36, 55, 46, 46, 97, 99, 7, 66, 99, 45, 7, 55, 45, 7, 99
		, 45, 18, 56, 40, 34, 36, 34, 18, 76, 56, 46, 36, 40, 56, 76, 45, 18, 46, 76, 9, 56, 45, 40, 9, 9, 40, 9, 41, 42, 42
		, 77, 71, 95, 77, 42, 71, 77, 71, 7, 71, 18, 17, 18, 76, 42, 76, 17, 77, 42, 77 };

	chromPInit2->code = { 3, 3, 3, 43, 43, 43, 43, 43, 43, 43, 79, 79, 79, 79, 79, 79, 79, 31, 31, 31, 91, 91, 91, 91, 91, 91, 91, 91, 47, 47
		, 47, 47, 47, 47, 47, 44, 44, 44, 44, 44, 44, 44, 8, 8, 8, 8, 8, 8, 15, 15, 15, 15, 15, 15, 84, 84, 84, 84, 84, 84, 72, 72, 72, 72
		, 72, 72, 96, 96, 96, 96, 96, 96, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 80, 80, 80, 80, 80, 80, 80, 81, 81, 81, 81, 81
		, 81, 81, 78, 78, 78, 78, 78, 78, 78, 35, 35, 35, 35, 35, 35, 35, 50, 50, 50, 50, 50, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 94, 94, 94
		, 94, 94, 94, 94, 94, 90, 90, 90, 90, 90, 90, 90, 90, 37, 37, 37, 37, 37, 37, 37, 59, 59, 59, 23, 23, 23, 23, 23, 23, 52, 52
		, 52, 52, 52, 52, 12, 12, 12, 12, 12, 12, 73, 73, 73, 73, 73, 73, 24, 24, 24, 24, 24, 24, 22, 22, 22, 22, 22, 22, 22, 38, 38
		, 38, 38, 38, 38, 38, 21, 21, 21, 21, 21, 21, 21, 54, 54, 54, 54, 54, 54, 53, 53, 53, 53, 53, 53, 51, 51, 51, 51, 51, 51, 19
		, 19, 19, 19, 19, 19, 19, 2, 2, 2, 2, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 20, 20, 20, 20, 20, 20, 20, 74, 74, 74, 74
		, 74, 74, 16, 16, 16, 16, 16, 16, 16, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 30, 30, 30, 30, 30, 30, 29, 29, 29, 29, 29, 29
		, 39, 39, 39, 27, 27, 27, 27, 27, 27, 60, 60, 60, 82, 82, 82, 82, 82, 82, 4, 4, 4, 83, 83, 83, 83, 83, 83, 85, 85, 85, 85
		, 85, 85, 28, 28, 28, 28, 28, 28, 48, 48, 48, 48, 48, 48, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 87, 87, 87, 87, 87
		, 87, 87, 87, 87, 87, 87, 87, 5, 5, 5, 5, 5, 5, 5, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 6, 6, 6, 6, 6, 6, 6
		, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 93, 93, 93, 57, 57
		, 57, 57, 57, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 89
		, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89, 75, 75, 75, 75, 75, 75, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66
		, 25, 25, 25, 25, 25, 25, 25, 25, 25, 58, 58, 58, 58, 58, 14, 14, 14, 14, 14, 13, 13, 13, 13, 13, 97, 97, 97, 97
		, 97, 55, 55, 55, 55, 55, 56, 56, 56, 56, 56, 9, 9, 9, 9, 9, 26, 26, 26, 26, 26, 26, 71, 71, 71, 71, 71, 71, 71
		, 71, 71, 99, 99, 99, 99, 99, 99, 99, 99, 99, 17, 17, 17, 17, 17, 17, 17, 40, 40, 40, 40, 40, 40, 40, 40, 40
		, 92, 92, 92, 92, 92, 92, 92, 34, 34, 34, 34, 34, 34, 34, 41, 41, 41, 41, 41, 41, 95, 95, 95, 95, 36, 36, 36
		, 36, 36, 36, 36, 46, 46, 46, 46, 46, 46, 46, 7, 7, 7, 7, 7, 7, 45, 45, 45, 45, 45, 45, 45, 18, 18, 18, 18
		, 18, 18, 18, 76, 76, 76, 76, 76, 76, 76, 42, 42, 42, 42, 42, 42, 42, 77, 77, 77, 77, 77, 77, 77 };
	*/


	// NEH--初始排序-总加工时间；目标-总流水时间
	chromPInit->code = { 4, 0, 22, 4, 31, 38, 21, 39, 4, 31, 59, 39, 17, 17, 10, 59, 31, 16, 10, 16, 11, 80, 11, 81, 80, 39, 25, 81, 25, 99
		, 59, 3, 99, 3, 85, 24, 85, 91, 10, 10, 11, 40, 80, 81, 80, 5, 40, 3, 10, 11, 26, 81, 80, 6, 10, 11, 85, 90, 24, 85, 81, 78, 91, 24
		, 15, 11, 24, 85, 91, 90, 0, 91, 90, 1, 24, 85, 80, 66, 90, 87, 24, 80, 81, 88, 89, 91, 81, 79, 78, 94, 90, 91, 78, 0, 71, 79, 0
		, 67, 91, 90, 0, 79, 68, 91, 90, 78, 7, 5, 17, 26, 64, 79, 17, 1, 6, 16, 17, 65, 78, 16, 17, 70, 16, 78, 17, 16, 60, 25, 7, 64
		, 13, 25, 50, 87, 99, 66, 78, 16, 30, 99, 29, 99, 14, 23, 90, 99, 22, 25, 88, 40, 89, 56, 99, 38, 25, 25, 40, 21, 99, 40, 55
		, 27, 25, 58, 25, 12, 40, 73, 15, 99, 15, 40, 51, 15, 40, 97, 15, 82, 15, 40, 52, 83, 0, 96, 69, 94, 0, 60, 94, 54, 94, 0
		, 53, 13, 60, 79, 13, 50, 0, 94, 79, 48, 13, 30, 28, 13, 29, 8, 94, 26, 5, 57, 79, 14, 26, 94, 50, 55, 5, 6, 26, 30, 50
		, 29, 1, 26, 5, 71, 6, 1, 30, 50, 1, 7, 67, 14, 6, 94, 7, 1, 66, 70, 65, 55, 29, 9, 66, 87, 1, 84, 66, 7, 87, 50, 72
		, 66, 64, 1, 75, 42, 64, 66, 69, 88, 75, 74, 1, 88, 89, 92, 87, 88, 77, 1, 89, 14, 7, 93, 87, 35, 1, 68, 87, 76, 37
		, 14, 66, 89, 44, 34, 87, 66, 36, 2, 66, 88, 87, 19, 45, 88, 87, 66, 95, 20, 66, 64, 47, 87, 18, 41, 87, 88, 43, 89
		, 46, 88, 89, 64, 88, 64, 88, 88, 89, 23, 64, 89, 5, 42, 74, 22, 5, 71, 89, 64, 56, 77, 92, 6, 76, 35, 67, 56, 89
		, 64, 6, 65, 64, 70, 71, 64, 71, 67, 44, 37, 69, 67, 65, 71, 70, 75, 23, 71, 67, 75, 22, 69, 71, 67, 68, 34, 36
		, 1, 68, 75, 65, 67, 71, 1, 75, 65, 67, 67, 68, 68, 67, 68, 67, 65, 68, 68, 65, 68, 56, 65, 56, 38, 65, 70, 68
		, 21, 23, 68, 57, 65, 38, 21, 57, 70, 23, 65, 57, 70, 42, 2, 19, 9, 57, 42, 74, 22, 70, 23, 9, 93, 95, 45, 42
		, 74, 47, 20, 38, 22, 70, 42, 74, 42, 92, 74, 22, 70, 21, 38, 92, 77, 70, 77, 21, 69, 92, 38, 18, 41, 77, 92
		, 92, 35, 77, 93, 77, 76, 35, 21, 76, 35, 46, 43, 30, 76, 35, 35, 55, 37, 76, 30, 29, 76, 44, 37, 27, 55, 44
		, 37, 96, 58, 29, 44, 58, 27, 72, 58, 34, 96, 27, 34, 36, 58, 34, 36, 27, 96, 36, 72, 27, 96, 72, 37, 44, 37
		, 34, 44, 12, 36, 34, 96, 12, 73, 36, 12, 73, 51, 51, 12, 73, 51, 69, 52, 12, 73, 52, 51, 52, 2, 73, 2, 19
		, 69, 45, 19, 51, 45, 69, 19, 45, 19, 19, 69, 97, 45, 95, 45, 69, 97, 20, 97, 47, 20, 69, 97, 82, 47, 20
		, 47, 18, 82, 41, 18, 82, 95, 18, 41, 82, 41, 20, 52, 43, 47, 20, 72, 82, 18, 47, 43, 41, 18, 43, 72, 46
		, 52, 46, 43, 43, 46, 46, 83, 46, 83, 83, 83, 83, 54, 54, 53, 54, 53, 54, 53, 54, 53, 48, 53, 48, 48, 48
		, 48, 28, 8, 28, 28, 8, 8, 28, 28, 8, 9, 8, 9, 84, 84, 84, 84, 84 };

	chromPInit2->code = { 4, 4, 4, 31, 31, 31, 39, 39, 39, 59, 59, 59, 17, 17, 17, 17, 17, 17, 17, 10, 10, 10, 10, 10, 10, 16, 16, 16, 16
		, 16, 16, 16, 11, 11, 11, 11, 11, 11, 80, 80, 80, 80, 80, 80, 80, 81, 81, 81, 81, 81, 81, 81, 25, 25, 25, 25, 25, 25, 25, 25, 25
		, 99, 99, 99, 99, 99, 99, 99, 99, 99, 3, 3, 3, 85, 85, 85, 85, 85, 85, 24, 24, 24, 24, 24, 24, 91, 91, 91, 91, 91, 91, 91, 91
		, 40, 40, 40, 40, 40, 40, 40, 40, 40, 5, 5, 5, 5, 5, 5, 5, 26, 26, 26, 26, 26, 26, 6, 6, 6, 6, 6, 6, 6, 90, 90, 90, 90, 90
		, 90, 90, 90, 78, 78, 78, 78, 78, 78, 78, 15, 15, 15, 15, 15, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
		, 1, 1, 1, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 88, 88, 88, 88
		, 88, 88, 88, 88, 88, 88, 88, 88, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89, 79, 79, 79, 79, 79, 79, 79, 94, 94, 94, 94
		, 94, 94, 94, 94, 71, 71, 71, 71, 71, 71, 71, 71, 71, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 68, 68, 68, 68, 68
		, 68, 68, 68, 68, 68, 68, 68, 7, 7, 7, 7, 7, 7, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 65, 65, 65, 65, 65, 65
		, 65, 65, 65, 65, 65, 65, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 60, 60, 60, 13, 13, 13, 13, 13, 50, 50, 50, 50
		, 50, 50, 30, 30, 30, 30, 30, 30, 29, 29, 29, 29, 29, 29, 14, 14, 14, 14, 14, 23, 23, 23, 23, 23, 23, 22, 22, 22, 22
		, 22, 22, 22, 56, 56, 56, 56, 56, 38, 38, 38, 38, 38, 38, 38, 21, 21, 21, 21, 21, 21, 21, 55, 55, 55, 55, 55, 27, 27
		, 27, 27, 27, 27, 58, 58, 58, 58, 58, 12, 12, 12, 12, 12, 12, 73, 73, 73, 73, 73, 73, 51, 51, 51, 51, 51, 51, 97, 97
		, 97, 97, 97, 82, 82, 82, 82, 82, 82, 52, 52, 52, 52, 52, 52, 83, 83, 83, 83, 83, 83, 96, 96, 96, 96, 96, 96, 69, 69
		, 69, 69, 69, 69, 69, 69, 69, 69, 69, 54, 54, 54, 54, 54, 54, 53, 53, 53, 53, 53, 53, 48, 48, 48, 48, 48, 48, 28, 28
		, 28, 28, 28, 28, 8, 8, 8, 8, 8, 8, 57, 57, 57, 57, 57, 9, 9, 9, 9, 9, 84, 84, 84, 84, 84, 84, 72, 72, 72, 72, 72
		, 72, 75, 75, 75, 75, 75, 75, 42, 42, 42, 42, 42, 42, 42, 74, 74, 74, 74, 74, 74, 92, 92, 92, 92, 92, 92, 92, 77
		, 77, 77, 77, 77, 77, 77, 93, 93, 93, 35, 35, 35, 35, 35, 35, 35, 76, 76, 76, 76, 76, 76, 76, 37, 37, 37, 37, 37
		, 37, 37, 44, 44, 44, 44, 44, 44, 44, 34, 34, 34, 34, 34, 34, 34, 36, 36, 36, 36, 36, 36, 36, 2, 2, 2, 2, 19, 19
		, 19, 19, 19, 19, 19, 45, 45, 45, 45, 45, 45, 45, 95, 95, 95, 95, 20, 20, 20, 20, 20, 20, 20, 47, 47, 47, 47, 47
		, 47, 47, 18, 18, 18, 18, 18, 18, 18, 41, 41, 41, 41, 41, 41, 43, 43, 43, 43, 43, 43, 43, 46, 46, 46, 46, 46, 46, 46 };


	// NEH做初始解（初始排序-总加工时间；目标-makespan），GA得到的最优解
	chromPInit->code = { 3, 3, 3, 43, 43, 43, 43, 43, 43, 43, 79, 79, 7, 79, 79, 79, 79, 31, 31, 31, 91, 91, 91, 91, 91, 44, 47, 47, 47
		, 1, 47, 47, 47, 15, 15, 15, 15, 8, 70, 8, 8, 8, 8, 9, 44, 44, 44, 44, 44, 91, 91, 91, 15, 15, 84, 84, 84, 84, 84, 84, 72, 77
		, 72, 71, 72, 72, 72, 72, 96, 96, 92, 96, 96, 96, 1, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 80, 80, 78, 78, 95, 78
		, 81, 81, 81, 81, 81, 81, 81, 80, 80, 80, 80, 80, 78, 78, 78, 78, 35, 35, 35, 35, 35, 35, 35, 50, 50, 50, 50, 5, 50, 0, 0
		, 0, 0, 0, 0, 0, 52, 71, 0, 94, 92, 17, 94, 94, 94, 94, 94, 90, 90, 90, 90, 90, 90, 90, 90, 37, 37, 37, 37, 37, 37, 37
		, 59, 59, 59, 23, 23, 5, 23, 20, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 93, 93, 93, 48, 57, 57, 57, 73, 24
		, 24, 24, 24, 24, 24, 22, 22, 22, 22, 22, 22, 22, 38, 38, 38, 38, 38, 38, 38, 21, 21, 21, 21, 21, 21, 21, 54, 54
		, 54, 54, 54, 54, 53, 53, 53, 53, 53, 53, 51, 51, 51, 51, 51, 51, 19, 19, 19, 19, 19, 19, 19, 2, 2, 2, 2, 69, 69
		, 69, 69, 69, 69, 69, 69, 69, 69, 69, 20, 23, 20, 20, 20, 20, 20, 74, 74, 74, 1, 1, 71, 1, 1, 1, 1, 1, 1, 1, 1
		, 47, 96, 30, 30, 30, 30, 30, 30, 29, 29, 29, 29, 29, 29, 39, 39, 39, 27, 92, 27, 27, 71, 27, 27, 27, 60, 60
		, 60, 82, 82, 82, 82, 82, 82, 4, 4, 4, 83, 83, 83, 83, 83, 83, 85, 85, 28, 28, 28, 28, 28, 28, 67, 67, 67
		, 67, 67, 67, 67, 70, 70, 70, 8, 70, 70, 70, 87, 87, 87, 87, 87, 87, 87, 87, 13, 87, 87, 87, 5, 5, 6, 6
		, 6, 5, 23, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 6, 6, 5, 5, 50, 6, 67, 70, 48, 70, 48, 70
		, 70, 48, 67, 67, 67, 67, 23, 0, 52, 52, 52, 48, 52, 52, 12, 12, 12, 12, 12, 12, 73, 73, 73, 73, 73
		, 57, 57, 48, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 64, 64, 64, 64, 64, 64, 64, 64, 64
		, 92, 64, 64, 64, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89, 75, 75, 75, 75, 75, 75, 66, 66, 14
		, 66, 66, 71, 66, 66, 66, 66, 66, 66, 6, 66, 25, 25, 25, 25, 25, 25, 25, 25, 25, 58, 58, 58, 58
		, 71, 58, 14, 14, 14, 14, 66, 13, 13, 13, 13, 87, 97, 97, 97, 97, 97, 55, 55, 55, 55, 55, 56
		, 56, 56, 56, 56, 9, 9, 9, 9, 44, 26, 26, 26, 92, 26, 92, 26, 26, 99, 99, 99, 99, 99, 99, 99
		, 92, 99, 99, 17, 94, 94, 17, 17, 17, 17, 17, 40, 40, 40, 40, 40, 40, 40, 40, 40, 34, 34, 34
		, 34, 34, 34, 34, 41, 41, 41, 74, 74, 74, 16, 16, 16, 16, 16, 16, 16, 1, 41, 41, 41, 95, 95
		, 95, 36, 36, 36, 36, 36, 36, 36, 46, 46, 46, 46, 46, 46, 46, 7, 7, 7, 7, 7, 79, 45, 45, 45
		, 85, 85, 85, 85, 45, 45, 45, 45, 18, 18, 18, 18, 18, 18, 71, 18, 76, 76, 76, 76, 76, 76
		, 71, 76, 42, 42, 42, 71, 42, 42, 42, 42, 77, 77, 77, 77, 77, 77 };


	/*
	// NEH做初始解（初始排序-总加工时间；目标-总流水时间），GA得到的最优解
	chromPInit->code = { 4, 0, 22, 4, 31, 21, 41, 10, 88, 0, 17, 17, 16, 10, 16, 11, 31, 11, 81, 80, 39, 81, 99, 25, 59, 25, 3, 99, 3, 85
		, 26, 81, 80, 85, 90, 10, 10, 91, 24, 11, 10, 40, 3, 80, 10, 11, 40, 19, 5, 80, 81, 6, 11, 85, 24, 81, 85, 91, 78, 24, 11, 15, 24
		, 91, 90, 85, 91, 90, 0, 1, 24, 85, 66, 90, 80, 2, 87, 81, 24, 80, 91, 81, 88, 89, 94, 90, 91, 79, 78, 71, 78, 0, 83, 67, 83, 91
		, 90, 79, 0, 68, 91, 90, 17, 0, 5, 7, 17, 26, 64, 79, 78, 79, 17, 16, 65, 1, 17, 16, 70, 16, 17, 6, 16, 78, 25, 7, 78, 64, 25
		, 13, 60, 99, 66, 50, 16, 30, 99, 87, 99, 14, 23, 90, 99, 22, 78, 25, 29, 40, 88, 99, 89, 56, 25, 25, 40, 99, 21, 40, 38, 55
		, 25, 7, 25, 27, 40, 66, 58, 68, 67, 65, 68, 68, 1, 99, 66, 40, 2, 15, 40, 97, 15, 82, 51, 40, 15, 52, 15, 83, 15, 0, 94, 0
		, 94, 96, 65, 60, 53, 13, 60, 79, 13, 50, 0, 79, 94, 48, 13, 30, 28, 13, 29, 94, 8, 26, 5, 57, 79, 14, 94, 26, 50, 55, 5, 6
		, 30, 50, 29, 1, 26, 5, 71, 6, 4, 1, 31, 30, 87, 59, 59, 38, 89, 88, 46, 1, 68, 64, 89, 64, 88, 65, 88, 55, 18, 39, 14, 1
		, 67, 87, 84, 7, 94, 12, 66, 64, 87, 50, 42, 64, 66, 72, 39, 1, 75, 69, 88, 75, 92, 74, 1, 77, 88, 89, 14, 87, 88, 7, 35
		, 1, 76, 37, 14, 66, 93, 44, 34, 89, 73, 36, 68, 87, 88, 66, 87, 95, 1, 19, 87, 87, 89, 66, 20, 66, 64, 45, 88, 87, 47
		, 87, 7, 94, 66, 6, 66, 70, 1, 88, 23, 89, 89, 5, 42, 22, 5, 71, 74, 64, 89, 77, 92, 56, 76, 35, 6, 56, 64, 89, 65, 22
		, 6, 71, 23, 67, 75, 70, 67, 68, 67, 65, 71, 37, 44, 71, 64, 67, 71, 70, 64, 69, 71, 69, 34, 67, 1, 71, 75, 75, 54, 75
		, 67, 64, 65, 65, 68, 68, 67, 67, 68, 56, 67, 65, 56, 65, 70, 68, 23, 68, 57, 65, 38, 42, 57, 9, 38, 19, 21, 2, 42, 70
		, 57, 65, 23, 95, 70, 57, 74, 22, 70, 23, 9, 74, 20, 74, 22, 70, 42, 38, 42, 92, 47, 22, 70, 21, 42, 38, 69, 21, 38
		, 18, 92, 77, 70, 77, 45, 92, 93, 69, 41, 77, 21, 92, 92, 35, 77, 45, 43, 20, 77, 76, 35, 21, 76, 35, 46, 55, 30
		, 76, 36, 35, 35, 29, 37, 76, 30, 27, 76, 82, 97, 44, 37, 82, 97, 20, 55, 47, 47, 18, 44, 37, 96, 58, 44, 82, 29
		, 58, 27, 72, 34, 58, 96, 34, 36, 27, 34, 36, 2, 74, 36, 27, 96, 72, 27, 37, 44, 37, 34, 44, 96, 36, 34, 72, 12
		, 36, 96, 73, 12, 69, 51, 51, 12, 73, 51, 69, 58, 12, 19, 73, 52, 51, 12, 73, 19, 52, 52, 95, 45, 19, 19, 73
		, 80, 93, 51, 45, 69, 69, 45, 45, 20, 69, 47, 69, 97, 26, 97, 95, 41, 18, 82, 18, 52, 41, 82, 41, 20, 43, 47
		, 20, 72, 18, 47, 43, 41, 18, 54, 54, 83, 83, 46, 46, 43, 46, 46, 83, 43, 52, 46, 43, 9, 29, 50, 72, 43, 53
		, 54, 53, 54, 53, 54, 53, 48, 53, 48, 48, 48, 48, 28, 8, 28, 28, 8, 8, 28, 28, 8, 9, 8, 9, 84, 84, 84, 84, 84 };
	*/


	return;
};


// 用于测试
void getObjValsOfInitialChroms(Chromosome* chromPInit, Chromosome* chromPInit2, GeneticAlgorithm* gaP) {

	// 获取初始解的目标值
	pair<double, double> objectValues2 = getObjectValuesOfChromo(chromPInit2, gaP);
	pair<double, double> objectValues = getObjectValuesOfChromo(chromPInit, gaP);

	cout << "  test-chromPInit-bestObjectValues(makespan)=" << objectValues.second << ";  due time=" << objectValues.first << endl;
	cout << "  test-chromPInit2-bestObjectValues2(makespan)=" << objectValues2.second << ";  due time=" << objectValues2.first << endl;

	//for (int j = 0; j < chromPInit->tardinessOfjobs.size(); ++j) {
	//	cout << j << ":" << chromPInit->tardinessOfjobs[j] << "   ";
	//}
	//cout << endl;
	//for (int j = 0; j < chromPInit2->tardinessOfjobs.size(); ++j) {
	//	cout << j << ":" << chromPInit2->tardinessOfjobs[j] << "   ";
	//}

	//cout << endl; cout << endl;
	//for (int j = 0; j < chromPInit->code.size(); ++j) {
	//	cout << chromPInit->code[j] << " ";
	//}
	//cout << endl;
	//for (int j = 0; j < chromPInit2->code.size(); ++j) {
	//	cout << chromPInit2->code[j] << " ";
	//}


	char c; cin >> c;

	cout << "  tardinessOfjobs-chromPInit" << endl;
	for (int i = 0; i < chromPInit->tardinessOfjobs.size(); ++i) {
		cout << i << ":" << chromPInit->tardinessOfjobs[i] << endl;
	}
	cout << endl;

	cout << "  tardinessOfjobs-chromPInit2" << endl;
	for (int i = 0; i < chromPInit2->tardinessOfjobs.size(); ++i) {
		cout << i << ":" << chromPInit2->tardinessOfjobs[i] << endl;
	}
	cout << endl;

	cout << "tianmin:" << endl;
	for (int i = 0; i < chromPInit->codeLen; ++i) {
		cout << chromPInit->code[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < chromPInit2->codeLen; ++i) {
		cout << chromPInit2->code[i] << " ";
	}
	cout << endl;
	cin >> c;

	return;
}




