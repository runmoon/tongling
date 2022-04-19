#pragma once

#include<Chromosome.h>

#include<map>
#include<list>
#include<vector>
#include<set>
#include<string>
#include<utility>
#include<iostream>
#include<algorithm>
using namespace std;




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
		this->codeLen = c1->codeLen;
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




class cmpByObjFirst_NDS {
public:
	bool operator()(Chromosome* c1, Chromosome* c2) {
		return c1->objectValues.first < c2->objectValues.first;
	}
};


class cmpByObjSecond_NDS {
public:
	bool operator()(Chromosome* c1, Chromosome* c2) {
		return c1->objectValues.second > c2->objectValues.second;
	}
};


bool cmpByCrowdingDistance(const pair<Chromosome*, double>& ele, const pair<Chromosome*, double>& ele2);


void fastNondominationSort(list<Chromosome*>& solutionSet, vector<set<Chromosome*>>& paretoPlanes);


void getDensityEstimation(set<Chromosome*>& paretoPlane, vector<pair<Chromosome*, double>>& crowdingDistances);




