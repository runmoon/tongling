#pragma once
#include<head.h>
#include<cstring>


class Chromosome {
public:
	Chromosome(int len, int rangeOfJob) :codeLen(len), rangeOfJob(rangeOfJob) {
		code = vector<int>(len, -1);
		objectValues = make_pair(DBL_MAX, DBL_MAX);
		tardinessOfjobs = vector<double>(rangeOfJob, 0.0);
	};
	int codeLen;
	int rangeOfJob;  // job�ı��뷶ΧΪ��0-rangeOfJob
	vector<int> code;
	pair<double, double> objectValues;

	vector<double> tardinessOfjobs;  // ÿ��job������ʱ�� time_duration

	map<int, double>  delayJobs;  // ���ڵ�Job

	Chromosome(const Chromosome* c1) {
		this->code = c1->code;
		this->rangeOfJob = c1->rangeOfJob;
		this->code.assign(c1->code.begin(), c1->code.end());
		this->objectValues = c1->objectValues;
		this->tardinessOfjobs.assign(c1->tardinessOfjobs.begin(), c1->tardinessOfjobs.end());
		for (auto& ele : c1->delayJobs) {
			this->delayJobs.insert(ele);
		}
	}
};


class GeneticAlgorithm {
public:
	GeneticAlgorithm(int numOfJobRange, int numPop, int totalGenerati, int totalLen);

	friend pair<double, double> getObjectValuesOfChromo(Chromosome* chromP, GeneticAlgorithm* populationP, bool isPrint = 0);
	friend pair<double, double> getObjectValuesOfChromo_Parallel(Chromosome* chromP, GeneticAlgorithm* populationP, bool isPrint = 0);


	void initializePopulation(Chromosome* chromPInit, Chromosome* chromPInit2);

	void getNewChrome(Chromosome* chromPInit, Chromosome* chromNew);

	void initializeInfoOfBef(vector<pair<string, Job*>>* jobOrderBefAirP, map<string, Job*>* jobsMapBefAirP, map<string, Mach*>* machsMapBefAirP);

	void initObjValsOfGAPop();
	  
	void resetChildChroms();

	void crossoverParentsByPOX(int sizeOfSet1, Chromosome* parent1, Chromosome* parent2, Chromosome* child1, Chromosome* child2);

	// ���̶�ѡ�񣬲��Ż�--ֻ�ʺϴӼ�����ѡ��С��������
	void chosenChromsByRoulette(const int numOfChromsToChose, const int numOfPop, vector<Chromosome*>& popPool, vector<int>& chosenChroms);

	// ���̶�ѡ�񣬷Ż�--�ʺ�����
	void chosenChromsByRoulette(const int numOfChromsToChose, const int numOfPop, vector<Chromosome*>& popPool, vector<int>& chosenChroms, int isPutBack);

	// ������ѡ�񣬷Ż�--�ʺ�����
	void chosenChromsByTournament(const int numOfChromsToChose, const int numOfPop, vector<Chromosome*>& popPool, vector<int>& chosenChroms, int isPutBack);

	// ������ѡ����ѡ���õ��ӳ���
	void chosenChromsByTournament2(const int numOfChromsToChose, const int numOfPop, vector<Chromosome*>& popPool, vector<int>& chosenChroms, int isPutBack);

	void crossoverOfPop();

	void mutationOfPop();

	pair<int, int> getPositionsForMutation(Chromosome* chromP);

	void mutationOfPop_JobTardiness();

	// ����ʽ--��ת����Ƭ��
	static void getNeighborByReverse(Chromosome* chromP, const int totalLenOfGACode);

	// ����ʽ--��������Ƭ�λ�ȡ�¸���
	static void getNeighborByExchangeSub(Chromosome* chromP, const int totalLenOfGACode);

	// ����ʽ--��ȡһ�Σ������һλ�ã��ٲ���
	static void getNeighborByReinsertSub(Chromosome* chromP, const int totalLenOfGACode);

	// ����ʽ--��������λ��
	static void getNeighborByExchangeTwo(Chromosome* chromP, const int totalLenOfGACode);

	// ����ʽ--��ȡһ�����ٲ���
	static void getNeighborByReinsertOne(Chromosome* chromP, const int totalLenOfGACode);


	void getNewChromeToNeighbor(Chromosome* chromP, const int totalLenOfGACode);

	// ��һ��Ⱦɫ�����ֲ�����
	static void localSearchForCurChrome_Parallel(Chromosome* chromP, GeneticAlgorithm* populationP, const int maxIter);
	
	// ��һ��Ⱦɫ�����ֲ�����
	void localSearchForCurChrome(Chromosome* chromP, GeneticAlgorithm* populationP, const int maxIter);
	//static void localSearchForCurChrome(Chromosome* chromP, const int maxIter);

	void localSearch();

	void localSearch_Parallel();


	void getObjValForChilds();

	static bool compareChrom(Chromosome* c1, Chromosome* c2) {
		return c1->objectValues.second < c2->objectValues.second;
	};

	//ѡ����һ����ͬʱ������Ӣ
	void selectForNextGeneration();

	void printBestObjVal();

	void runGA();

	int m_numOfPop;                   // ��Ⱥ��С��Ⱦɫ�����
	int m_totalLenOfGACode;           // ÿ��Ⱦɫ��ĳ���
	int m_numOfJobRangeOfGA;          // job�ķ�Χ0-100 
	int m_totalGeneration;            // �ܴ��� 
	int m_curGeneration = 1;          // �ڼ���
	double m_pressureBaseNum = 0.96;      // ���̶�ѡ��ѹ��ϵ���Ļ��� 0.985
	double m_coeff_pressure = 1;          // ϵ��,���̶�ѡ��ѹ��
	double m_coeff_additionalNum = 1000;  // ���̶�ϵ��
	vector<Chromosome*> m_popPool;
	vector<pair<Job*, pair<int, int>>> m_codeInfoOfGA;  // vector<pair<ĳJob*, pair<GA������Ƴ̿�ʼ����, GA������Ƴ̽�������>>>
	list<pair<double, double>> m_recordOfBestObjVals;

	double m_eliteRate = 0.00;
	double m_mutatRate = 0.03;
	double m_crossRate = 0.08;
	int m_numForMutation;
	int m_numForCrossOver;
	int m_numElites;
	vector<Chromosome*> m_childsForCroOver;
	vector<Chromosome*> m_childsForMutation;
	vector<Chromosome*> m_popPoolNew;          // ����ѡ����һ��
	set<Chromosome*> m_totalChromsSet;
	vector<Chromosome*> m_eliteChroms;

	// ����������¯֮ǰ�Ĺ���
	vector<pair<string, Job*>>* m_jobOrderBefAirP;
	map<string, Job*>* m_jobsMapBefAirP;
	map<string, Mach*>* m_machsMapBefAirP;

	// �������룬����Ⱦɫ����ֵ
	vector<pair<string, Job*>>     m_jobOrderTmp2;
	map<string, Job*>              m_jobsMapTmp2;
	map<string, Mach*>             m_machsMapTmp2;

	virtual ~GeneticAlgorithm();
};




// --------GA��ȡĿ�꺯��ֵ���--------

// GA�����ã����Ի�ȡjob������ʱ������������ȡĿ�꺯��ֵ <������ʱ��(Сʱ), �ӹ����й��������ʱ�䳤��(Сʱ)>
pair<double, double> getObjValsForChrom(vector<pair<string, Job*>>& jobOrder, map<string, Mach*>& machsMap, Chromosome* chromP);

// ��ȡȾɫ���Ŀ�꺯��ֵ
pair<double, double> getObjectValuesOfChromo(Chromosome* chromP, GeneticAlgorithm* populationP, bool isPrint);

// ��ȡȾɫ���Ŀ�꺯��ֵ
pair<double, double> getObjectValuesOfChromo_Parallel(Chromosome* chromP, GeneticAlgorithm* populationP, bool isPrint);

// --------GA��ȡĿ�꺯��ֵ���--------


// �ɱ�����Ϣ��ȡ��ʼ�⣨Ⱦɫ�壩
void getJobChromoFromInitSolut(vector<pair<Job*, pair<int, int>>>& codeInfoOfGA, vector<pair<string, Job*>>& jobOrder
	, vector<pair<pair<Job*, int>, ptime>>& jobChromPre, Chromosome* chromPInit, Chromosome* chromPInit2);





