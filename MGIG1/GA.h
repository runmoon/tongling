#pragma once

#include<head.h>
#include<NDS.h>
#include<Chromosome.h>
#include<cstring>



class GeneticAlgorithm {
public:
	GeneticAlgorithm(int numOfJobRange, int numPop, int totalGenerati, int totalLen);

	friend pair<double, double> getObjectValuesOfChromo(Chromosome* chromP, GeneticAlgorithm* gaP, bool isPrint = 0);
	friend pair<double, double> getObjectValuesOfChromo_Parallel(Chromosome* chromP, GeneticAlgorithm* gaP, threadInfoOfLS* threadInfoP, bool isPrint = 0);


	void initializePopulation(Chromosome* chromPInit, Chromosome* chromPInit2);

	void initializePopulation2(Chromosome* chromPInit, Chromosome* chromPInit2, Chromosome* chromPInit3, Chromosome* chromPInit4);

	void getNewChrome(Chromosome* chromPInit, Chromosome* chromNew);

	// 初始化GA中用于对染色体解码（排产）信息
	void initializeInfoOfBef(vector<pair<string, Job*>>* jobOrderBefP, map<string, Job*>* jobsMapBefP, map<string, Mach*>* machsMapBefP);

	void initObjValsOfGAPop();
	  
	void resetChildChroms();

	void crossoverParentsByPOX(int sizeOfSet1, Chromosome* parent1, Chromosome* parent2, Chromosome* child1, Chromosome* child2);

	// 轮盘赌选择，不放回--只适合从集合中选择小批量个体
	void chosenChromsByRoulette(const int numOfChromsToChose, const int numOfPop, vector<Chromosome*>& popPool, vector<int>& chosenChroms);

	// 轮盘赌选择，放回--适合所有
	void chosenChromsByRoulette(const int numOfChromsToChose, const int numOfPop, vector<Chromosome*>& popPool, vector<int>& chosenChroms, int isPutBack);

	// 锦标赛选择，放回--适合所有
	void chosenChromsByTournament(const int numOfChromsToChose, const int numOfPop, vector<Chromosome*>& popPool, vector<int>& chosenChroms, int isPutBack);

	// 锦标赛选择，挑选不好的扔出来
	void chosenChromsByTournamentReject(const int numOfChromsToChose, const int numOfPop, vector<Chromosome*>& popPool, vector<int>& chosenChroms, const int numElites, int isPutBack);

	void crossoverOfPop();

	void mutationOfPop();

	pair<int, int> getPositionsForMutation(Chromosome* chromP);

	void mutationOfPop_JobTardiness();

	// 邻域方式--翻转部分片段
	static void getNeighborByReverse(Chromosome* chromP, const int totalLenOfGACode);

	// 邻域方式--交换部分片段获取新个体
	static void getNeighborByExchangeSub(Chromosome* chromP, const int totalLenOfGACode);

	// 邻域方式--截取一段，随机找一位置，再插入
	static void getNeighborByReinsertSub(Chromosome* chromP, const int totalLenOfGACode);

	// 邻域方式--交换两个位置
	static void getNeighborByExchangeTwo(Chromosome* chromP, const int totalLenOfGACode);

	// 邻域方式--抽取一个，再插入
	static void getNeighborByReinsertOne(Chromosome* chromP, const int totalLenOfGACode);


	void getNewChromeToNeighbor(Chromosome* chromP, const int totalLenOfGACode);

	// 对一个染色体做局部搜索
	void localSearchForCurChrome(Chromosome* chromP, GeneticAlgorithm* gaP, const int maxIter);
	//static void localSearchForCurChrome(Chromosome* chromP, const int maxIter);

	// 对一个染色体做局部搜索
	static void localSearchForCurChrome_Parallel(Chromosome* chromP, GeneticAlgorithm* gaP, const int maxIter, threadInfoOfLS* threadInfoP);
	

	// 一个线程对几个染色体做local search
	static void localSearch_AThread(vector<Chromosome*> &chromPVec, GeneticAlgorithm* gaP, const int maxIter, threadInfoOfLS* threadInfoP);

	void localSearch();

	void localSearch_Parallel(const int num_thread, vector<threadInfoOfLS*>& threadInfosP);


	void getObjValForChilds();

	static bool compareChrom(Chromosome* c1, Chromosome* c2) {
		return c1->objectValues.second < c2->objectValues.second;
	};

	// 通过 锦标赛选择法(放回) 或 轮盘赌选择法(放回) 选择下一代，同时保留精英
	void selectionForNextPop();

	// 通过“锦标赛剔除法”选择下一代，同时保留精英
	void selection_ByTournamentReject();

	// 检查是否重复太多，跳出局部最优
	void checkRepetitionsAndDisturbance();

	void printBestObjVal();

	void runGA();





	// 对一个染色体做局部搜索
	static void localSearchForCurChrome_Parallel_DueObjs(Chromosome* chromP, GeneticAlgorithm* gaP, const int maxIter, threadInfoOfLS* threadInfoP);

	//建立多层pareto前沿面，按照帕雷多层数和挤度选择下一代；
	void selectForNextGeneration_byParetoPlanes();

	void runNSGA2();





	int m_numOfPop;                   // 种群大小，染色体个数
	int m_totalLenOfGACode;           // 每个染色体的长度
	int m_numOfJobRangeOfGA;          // job的范围0-100 
	int m_totalGeneration;            // 总代数 
	int m_curGeneration = 1;          // 第几代
	double m_pressureBaseNum = 0.96;      // 轮盘赌选择压力系数的基数 0.985
	double m_coeff_pressure = 1;          // 系数,轮盘赌选择压力
	double m_coeff_additionalNum = 1000;  // 轮盘赌系数
	vector<Chromosome*> m_popPool;
	vector<pair<Job*, pair<int, int>>> m_codeInfoOfGA;  // vector<pair<某Job*, pair<GA编码的制程开始索引, GA编码的制程结束索引>>>
	list<pair<double, double>> m_recordOfBestObjVals;

	double m_eliteRate = 0.015;  // 精英比例
	double m_mutatRate = 0.03;   // 变异率
	double m_crossRate = 0.08;   // 交叉率
	int m_numForMutation;
	int m_numForCrossOver;
	int m_numElites;
	vector<Chromosome*> m_childsForCroOver;
	vector<Chromosome*> m_childsForMutation;
	vector<Chromosome*> m_popPoolNew;          // 用于选择下一代
	set<Chromosome*> m_totalChromsSet;
	vector<Chromosome*> m_eliteChroms;

	// 用来排气垫炉之前的工序
	vector<pair<string, Job*>>* m_jobOrderBefP;
	map<string, Job*>* m_jobsMapBefP;
	map<string, Mach*>* m_machsMapBefP;

	// 用来解码，计算染色体适值
	vector<pair<string, Job*>>     m_jobOrderTmp2;
	map<string, Job*>              m_jobsMapTmp2;
	map<string, Mach*>             m_machsMapTmp2;

	virtual ~GeneticAlgorithm();
};



// --------GA获取目标函数值相关--------

// GA解码用，可以获取job的拖期时长————获取目标函数值 <总延期时间(小时), 加工所有工件所需的时间长度(小时)>
//pair<double, double> getObjValsForChrom(vector<pair<string, Job*>>& jobOrder, map<string, Mach*>& machsMap, Chromosome* chromP);
pair<double, double> getObjValsForChrom(vector<pair<string, Job*>>& jobOrder, map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, Chromosome* chromP);

// 获取染色体的目标函数值
pair<double, double> getObjectValuesOfChromo(Chromosome* chromP, GeneticAlgorithm* gaP, bool isPrint);

// 获取染色体的目标函数值
pair<double, double> getObjectValuesOfChromo_Parallel(Chromosome* chromP, GeneticAlgorithm* gaP, threadInfoOfLS* threadInfo, bool isPrint);

// --------GA获取目标函数值相关--------



// 由初始排产结果获取染色体编码：排产结果须存入jobOrderScheduled中；编码信息须存入encodeInfoOfGA中
void initChromCodesByInitSedul(vector<pair<string, Job*>>& jobOrderScheduled, const map<string, pair<int, pair<int, int>>>& encodeInfoOfGA
	, const int totalLenOfGACode, Chromosome* chromPInit, Chromosome* chromPInit2);

// 由初始排产结果获取染色体编码：排产结果须存入jobOrderScheduled中；编码信息须存入encodeInfoOfGA中
// 由初始排产结果获取染色体编码：排产结果须存入jobOrderScheduled中；编码信息须存入encodeInfoOfGA中
void initChromCodesByInitSedul2(vector<pair<string, Job*>>& jobOrderScheduled, vector<pair<string, Job*>>& jobOrderScheduled2
	, const map<string, pair<int, pair<int, int>>>& encodeInfoOfGA, const int totalLenOfGACode
	, Chromosome* chromPInit, Chromosome* chromPInit2, Chromosome* chromPInit3, Chromosome* chromPInit4);

// 由预先代码获取染色体编码
void initChromCodesByPreCode(const int totalLenOfGACode, Chromosome* chromPInit, Chromosome* chromPInit2);

// 用于测试
void getObjValsOfInitialChroms(Chromosome* chromPInit, Chromosome* chromPInit2, GeneticAlgorithm* gaP);


