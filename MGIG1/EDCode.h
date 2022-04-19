#pragma once
#include<head.h>
#include<Chromosome.h>

// Encode and Decode
class EDCode {
public:

	EDCode(const int totalLenOfChromCode, vector<pair<string, Job*>>* jobOrderBefP, map<string, Job*>* jobsMapBefP, map<string, Mach*>* machsMapBefP){
		this->m_totalLenOfChromCode = totalLenOfChromCode;
		this->m_numOfJobRange = jobOrderBefP->size();

		// 1����jobOrderBef��Job˳��������Ⱦɫ����룻2���洢����ǰ�ĳ�ʼ��Job��Mach��Ϣ
		this->m_jobOrderBefP = jobOrderBefP;
		this->m_jobsMapBefP = jobsMapBefP;
		this->m_machsMapBefP = machsMapBefP;

		// ����ʱ�õģ�����Ⱦɫ����ֵ
		this->m_jobOrderTmp2 = { vector<pair<string, Job*>>(this->m_jobOrderBefP->size()) };
		initJobsTemp(this->m_jobsMapTmp2, this->m_jobOrderTmp2, *(this->m_jobOrderBefP));
		initMachsMapTemp(this->m_machsMapTmp2, *(this->m_machsMapBefP));
	};


	// �ɳ�ʼ�Ų������ȡȾɫ����룺�Ų���������jobOrderScheduled�У�������Ϣ�����encodeInfoOfGA��
	void initChromCodesByInitSedul(vector<pair<string, Job*>>& jobOrderScheduled, const map<string, pair<int, pair<int, int>>>& encodeInfoOfGA
		, const int totalLenOfGACode, Chromosome* chromPInit, Chromosome* chromPInit2);

	// ��Ԥ�ȴ����ȡȾɫ�����
	void initChromCodesByPreCode(Chromosome* chromPInit, Chromosome* chromPInit2);




	// GA�����ã����Ի�ȡjob������ʱ������������ȡĿ�꺯��ֵ <������ʱ��(Сʱ), �ӹ����й��������ʱ�䳤��(Сʱ)>
	//static pair<double, double> getObjValsForChrom(vector<pair<string, Job*>>& jobOrder, map<string, Mach*>& machsMap, Chromosome* chromP);
	static pair<double, double> getObjValsForChrom(vector<pair<string, Job*>>& jobOrder, map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, Chromosome* chromP);

	// ��ȡȾɫ���Ŀ�꺯��ֵ
	pair<double, double> getObjectValuesOfChromo(Chromosome* chromP, bool isPrint);

	// ��ȡȾɫ���Ŀ�꺯��ֵ
	static pair<double, double> EDCode::getObjectValuesOfChromo_Parallel(Chromosome* chromP, threadInfoOfLS* threadInfoP, bool isPrint);



	int m_totalLenOfChromCode;       // ÿ��Ⱦɫ��ĳ���
	int m_numOfJobRange;              // job�ķ�Χ��0-100 
	map<string, pair<int, pair<int, int>>> m_encodeInfoOfGA;

	// ����������¯֮ǰ�Ĺ���
	vector<pair<string, Job*>>*    m_jobOrderBefP;
	map<string, Job*>*             m_jobsMapBefP;
	map<string, Mach*>*            m_machsMapBefP;

	// �������룬����Ⱦɫ����ֵ
	vector<pair<string, Job*>>     m_jobOrderTmp2;
	map<string, Job*>              m_jobsMapTmp2;
	map<string, Mach*>             m_machsMapTmp2;

};



