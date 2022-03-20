#pragma once
#include<head.h>
#include<GA.h>

#include<set>
#include<list>
#include<vector>
#include<utility>
#include<algorithm>
#include<iostream>

using namespace std;
const int nummin = 3;  // ����
const int nummax = 5;  // ������

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
	int rangeOfJob;  // job�ı��뷶ΧΪ��0-rangeOfJob
	vector<int> code;
	pair<double, double> objectValues;  // pair<������ʱ�䣬makespan>

	vector<double> tardinessOfjobs;  // ÿ��job������ʱ�� time_duration
	vector<int> delayJobs;        // ���ڵ�Job���Ķ���
	// map<int, double>  delayJobs;  // ���ڵ�Job��ԭ��




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
		return c1->objectValues.second < c2->objectValues.second;
	}
};


void randomlyCreateInitialSolution_test(list<Chromosome*>& popPool);


Chromosome* selection(list<Chromosome*>& paretoSoluSet);


void destruction(Chromosome* chromChosen, vector<int>& genesChrosen);


void construction(list<Chromosome*>& partialSoluSet, Chromosome* chromChosen, vector<int>& genesChrosen);


void update(list<Chromosome*>& partialSoluSet, list<Chromosome*>& preParetoSet);


bool insertToParetoSet(list<Chromosome*>& partialSoluSetNew, Chromosome* chromNew);


void getParetoSet(list<Chromosome*>& solutionSet);


void getObjValsTest(Chromosome* chromNew);





