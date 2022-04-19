#pragma once
#include<head.h>
#include<Chromosome.h>

// Encode and Decode
class EDCode {
public:

	EDCode(const int totalLenOfChromCode, vector<pair<string, Job*>>* jobOrderBefP, map<string, Job*>* jobsMapBefP, map<string, Mach*>* machsMapBefP){
		this->m_totalLenOfChromCode = totalLenOfChromCode;
		this->m_numOfJobRange = jobOrderBefP->size();

		// 1、用jobOrderBef的Job顺序来进行染色体编码；2、存储解码前的初始的Job和Mach信息
		this->m_jobOrderBefP = jobOrderBefP;
		this->m_jobsMapBefP = jobsMapBefP;
		this->m_machsMapBefP = machsMapBefP;

		// 解码时用的，计算染色体适值
		this->m_jobOrderTmp2 = { vector<pair<string, Job*>>(this->m_jobOrderBefP->size()) };
		initJobsTemp(this->m_jobsMapTmp2, this->m_jobOrderTmp2, *(this->m_jobOrderBefP));
		initMachsMapTemp(this->m_machsMapTmp2, *(this->m_machsMapBefP));
	};


	// 由初始排产结果获取染色体编码：排产结果须存入jobOrderScheduled中；编码信息须存入encodeInfoOfGA中
	void initChromCodesByInitSedul(vector<pair<string, Job*>>& jobOrderScheduled, const map<string, pair<int, pair<int, int>>>& encodeInfoOfGA
		, const int totalLenOfGACode, Chromosome* chromPInit, Chromosome* chromPInit2);

	// 由预先代码获取染色体编码
	void initChromCodesByPreCode(Chromosome* chromPInit, Chromosome* chromPInit2);




	// GA解码用，可以获取job的拖期时长————获取目标函数值 <总延期时间(小时), 加工所有工件所需的时间长度(小时)>
	//static pair<double, double> getObjValsForChrom(vector<pair<string, Job*>>& jobOrder, map<string, Mach*>& machsMap, Chromosome* chromP);
	static pair<double, double> getObjValsForChrom(vector<pair<string, Job*>>& jobOrder, map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, Chromosome* chromP);

	// 获取染色体的目标函数值
	pair<double, double> getObjectValuesOfChromo(Chromosome* chromP, bool isPrint);

	// 获取染色体的目标函数值
	static pair<double, double> EDCode::getObjectValuesOfChromo_Parallel(Chromosome* chromP, threadInfoOfLS* threadInfoP, bool isPrint);



	int m_totalLenOfChromCode;       // 每个染色体的长度
	int m_numOfJobRange;              // job的范围：0-100 
	map<string, pair<int, pair<int, int>>> m_encodeInfoOfGA;

	// 用来排气垫炉之前的工序
	vector<pair<string, Job*>>*    m_jobOrderBefP;
	map<string, Job*>*             m_jobsMapBefP;
	map<string, Mach*>*            m_machsMapBefP;

	// 用来解码，计算染色体适值
	vector<pair<string, Job*>>     m_jobOrderTmp2;
	map<string, Job*>              m_jobsMapTmp2;
	map<string, Mach*>             m_machsMapTmp2;

};



