#pragma once

#include<head.h>
#include<Chromosome.h>

#include<NDS.h>
#include<EDCode.h>

#include<set>
#include<list>
#include<vector>
#include<utility>
#include<algorithm>
#include<iostream>
#include<time.h>

#include<thread>
#include<mutex>



using namespace std;
const int nummin = 10;  // 包括
const int nummax = 15;  // 不包括

/*
class Chromosome {
public:
	Chromosome(int len, int rangeOfJob) :codeLen(len), rangeOfJob(rangeOfJob) {
		this->code = vector<int>(len, -1);
		this->objectValues = make_pair(DBL_MAX, DBL_MAX);
		this->tardinessOfjobs.reserve(rangeOfJob);
		this->tardinessOfjobs = vector<double>(rangeOfJob, 0.0);
		this->delayJobs.reserve(rangeOfJob);
	};
	int codeLen;
	int rangeOfJob;  // job的编码范围为：0-rangeOfJob
	vector<int> code;
	pair<double, double> objectValues;  // pair<总脱期时间，makespan>

	vector<double> tardinessOfjobs;  // 每个job的脱期时长 time_duration
	vector<int> delayJobs;        // 脱期的Job，改动后
	// map<int, double>  delayJobs;  // 脱期的Job，原来




	Chromosome(const Chromosome* c1) {
		this->code = c1->code;
		this->rangeOfJob = c1->rangeOfJob;

		this->code.assign(c1->code.begin(), c1->code.end());
		this->objectValues = c1->objectValues;
		this->tardinessOfjobs.assign(c1->tardinessOfjobs.begin(), c1->tardinessOfjobs.end());
		this->delayJobs.clear();

		//for (auto& ele : c1->delayJobs) { this->delayJobs.emplace_back(ele); }
		this->delayJobs = c1->delayJobs;
	}
};
*/

class cmpByObjFirst {
public:
	bool operator()(Chromosome* c1, Chromosome* c2) {
		return c1->objectValues.first < c2->objectValues.first;
	}
};

class cmpByObjSecond {
public:
	bool operator()(Chromosome* c1, Chromosome* c2) {
		return c1->objectValues.second > c2->objectValues.second;
	}
};




class IPG {
public:
	IPG(EDCode *edCodePEx):edCodeP(edCodePEx) {
		this->m_totalLenOfChromCode = edCodePEx->m_totalLenOfChromCode;
		this->m_numOfJobRange = edCodePEx->m_numOfJobRange;
	};
	EDCode* edCodeP;

	list<Chromosome*> popPool;

	void run(const int maxRound);

	void randomlyCreateInitialSolution_test(list<Chromosome*>& popPool);

	void randomlyCreateInitialSolution(const int numOfPop, Chromosome* chromPInit, Chromosome* chromPInit2);

	void initObjValsOfChromos();

	Chromosome* selection(list<Chromosome*>& paretoSoluSet);

	void destruction(Chromosome* chromChosen, vector<int>& genesChrosen);

	void construction(list<Chromosome*>& partialSoluSet, Chromosome* chromChosen, vector<int>& genesChrosen);



	void construction_Parallel(list<Chromosome*>& partialSoluSet, Chromosome* chromChosen, vector<int>& genesChrosen, vector<threadInfoOfLS*>& threadInfos);
	
	void insertToAChrom_Parellel(list<Chromosome*>& partialSoluSetNew, Chromosome* curChrom, int gene, vector<threadInfoOfLS*>& threadInfos);

	static void calculateTempChroms_thread(vector<Chromosome*>& chromsForThread, threadInfoOfLS* threadInfoP);



	list<int> construction_MultiParetos_Parallel(list<Chromosome*>& partialSoluSet, Chromosome* chromChosen, vector<int>& genesChrosen, vector<threadInfoOfLS*>& threadInfos);

	void insertToAChrom__MultiParetos_Parellel(list<Chromosome*>& partialSoluSetNew, Chromosome* curChrom, int gene, vector<threadInfoOfLS*>& threadInfos);

	void choseByMultiParetos(list<Chromosome*>& partialSoluSet, list<Chromosome*>& partialSoluSetNew, const int maxNumOfPop);

	void choseByPareto(list<Chromosome*>& partialSoluSet, list<Chromosome*>& partialSoluSetNew);




	void update(list<Chromosome*>& partialSoluSet, list<Chromosome*>& preParetoSet);

	static bool insertToParetoSet(list<Chromosome*>& partialSoluSetNew, Chromosome* chromNew);

	void getParetoSet(list<Chromosome*>& solutionSet);

	void getObjVals_test(Chromosome* chromNew);


	int m_totalLenOfChromCode;
	int m_numOfJobRange;
};


