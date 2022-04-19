#pragma once

#define BOOST_DATE_TIME_SOURCE

#define _CRT_SECURE_NO_WARNINGS

//�������ļ����������Ҫ��ͷ�ļ�
#include<fstream>
#include<streambuf>

#include<iostream>
#include<vector>
#include<list>
#include<utility>
#include<map>
#include<set>
#include<queue>
//#include<ctime>
#include <cstdlib> // Header file needed to use srand and rand
#include<algorithm>
#include<iostream>
#include<random>  // �����
#include<stdlib.h>

#include<sstream>

// mysql���
#include<Windows.h>
#include<WinSock.h>
#include<mysql.h>

#include <time.h>

// ���߳����
#include<thread>
#include<mutex>

// boostʱ�����
#include<boost/timer.hpp>
#include<boost/date_time.hpp>
#include<boost/date_time/gregorian/gregorian.hpp>
#include<boost/date_time/posix_time/posix_time.hpp>

#include<cstdlib>
#include<ctime>

#pragma comment(lib, "libmysql.lib")
#pragma comment(lib, "wsock32.lib")

using namespace std;
using namespace boost;
using namespace boost::gregorian;  //������ʽ
using namespace boost::posix_time;



extern set<string> rollingMachFrequ;  // ������;

extern set<string> cut_StretchMachFrequ;  // ���ݼ��������;

extern set<string> cutMachFrequ;  // ���ݼ�;

extern set<string> stretchMachFrequ;  // �����; 

extern set<string> washMachFrequ;     // ��ϴ����;

extern set<string> airFurnaceSet;     // ����¯;


// ����¯
const double T_Bell_Furn = 30.0;  // ��λ:Сʱ; ��Bell Furnace������¯ÿ¯30Сʱ
const int Num_Para_Bell = 5;  // ��λ:Сʱ; ����¯��5��ƽ�л�(Parallel machine)
const double T_waitOfFirstFurn = 0.0;  // ��λ:Сʱ; ��Bell Furnace������¯����һ¯�ĵȴ�ʱ�� 5.0
const double T_toleranceOfNewFurn =20.0;  // ��λ:Сʱ; ��Bell Furnace������¯����һ¯����ڲ����ѿ�¯������ʱ��

const string CodeOfBellFurn = "BD-S006";  // ����¯�Ĵ���MachCode



// �ӹ�Ŀ����
struct ProcessTargets
{
	double m_targetWidth = 0.0;    //�ӹ�Ŀ����
	double m_targetThick = 0.0;    //�ӹ�Ŀ����
	double m_targetWeight = 0.0;   //�ӹ�Ŀ������
	double m_targetLength = 0.0;   //�ӹ�Ŀ�곤��
	double m_targetInnerDia = 0.0;   //�ӹ�Ŀ���ھ���internal diameter
	ProcessTargets() {};

	ProcessTargets(double twid, double tthick) {
		this->m_targetWidth = twid;
		this->m_targetThick = tthick;
	};
	~ProcessTargets() {};
};

struct CapWithConditions
{
	string m_machCode;
	string m_width;
	string m_thick;
	string m_alloyType;
	string m_status;
	string m_processT;
	string m_cap;
};

// ������
class Job // ����
{
public:
	string m_jobCode;         // ������
	string m_orderCode;       // ������ţ�һ�����������������
	string m_initSpec;        // initial specification, ���Ϲ��
	string m_alloyGrade;       // �Ͻ��ƺţ�����ĳ�ֺϽ���ϵ�һ��
	string m_alloyType;        // bopper-��ͭ, brass-��ͭ, alloy-�Ͻ�
	string m_dueDateOfOrderStr;  // ����Ŀ�꽻���ڵ��ַ�����ʽ
	ptime m_dueDateOfOrder;      // ����Ŀ�꽻����

	ptime m_startDateOfOrder; // time point that can start processing 

	ProcessTargets m_initialInfos;  // ��ʼ��Ϣ
	ProcessTargets m_finalTargets;  // �ӹ�Ŀ��
	
	//vector<string>  m_proceMachs;     // ����(����)����  vector<mach����>
	//vector<pair<string, ProcessTargets>>  m_proceTargets;  // ��ÿ������(����)�ϵļӹ�Ŀ�꣬vector<pair<mach����, �ӹ�Ŀ��>>
	//vector<pair<string, time_duration>>  m_proceTimes;   // ��ÿ������(����)�ϵļӹ�ʱ����vector<pair<mach����, �ӹ�ʱ��>>

	vector<pair<string, unsigned>>  m_proceMachs;     // ����(����)����  vector<pair<mach����, ������ڼ�������>>
	vector<pair<pair<string, unsigned>, ProcessTargets>>  m_proceTargets;  // ��ÿ������(����)�ϵļӹ�Ŀ�꣬vector<pair<pair<mach����, ������ڼ�������>, �ӹ�Ŀ��>>
	vector<pair<pair<string, unsigned>, time_duration>>  m_proceTimes;     // ��ÿ������(����)�ϵļӹ�ʱ����vector<pair<pair<mach����, ������ڼ�������>, �ӹ�ʱ��>>

	 
	time_duration m_totalProceTime;   // ��Job���ܼӹ�ʱ��

	unsigned m_curMachIndex = 0;    // ��ǰҪ�ӹ���machine��m_proceMachs�е�index
	vector<pair<pair<string, unsigned>, time_period>> m_allocatedTimeWin;    // ��job�Ѿ��ӹ��˵Ĺ�����vector<pair<pair<mach����, �ڼ���>, �ӹ�ʱ�䴰>>
	// vector<pair<pair<string, unsigned>, time_period>*> m_allocatedTimeWinsP;    // ָ�룬��job�Ѿ��ӹ��˵Ĺ�����vector<pair<pair<mach����, �ڼ���>, �ӹ�ʱ�䴰>>
	vector<pair<pair<string, unsigned>, time_period*>> m_allocatedTimeWinPs;    // ָ�룬��job�Ѿ��ӹ��˵Ĺ�����vector<pair<pair<mach����, �ڼ���>, �ӹ�ʱ�䴰*>>
	//vector<pair<pair<Mach*, unsigned>, unsigned>> m_allocatedTimeWinIndexs;    // ��job�Ѿ��ӹ��˵Ĺ�����vector<pair<pair<machָ��, �ڼ����ӹ�ʱ�䴰>, �ڼ�������>>

};

// ������
struct Mach {
	string m_machCode;        // �������룬����ID������¯��Ӧ5��ƽ�л�������һ�������Ӧһ������
	pair<double, double>                   m_thickWindow;             //�ɼӹ��ĺ�ȷ�Χ
	double                                 m_assignedWeight;          //��������������

	vector<pair<double, double> >          m_thickWindowsOfMach;      //����ɼӹ��ĺ�ȷ�Χ����Ӧ����Сʱ����
	vector<double>                         m_hourCapacities;          //����Сʱ���ܣ�

	list<pair<pair<string, unsigned>, time_period>>  m_allocatedTimeWin;   // �����ϵ�ʱ����䣬vector<pair<pair<job����, �ڼ���>, �ӹ�ʱ�䴰>>
	list<pair<pair<string, unsigned>, time_period*>>  m_allocatedTimeWinPs;   // �����ϵ�ʱ����䣬vector<pair<pair<Jobָ��, �ڼ���>, �ӹ�ʱ�䴰*>>

	vector<CapWithConditions> m_capsOriginalInfo;  // ����Ĳ�����Ϣ

	double m_timeOfSwith{0.0};


	Mach() {};

	Mach(string machCode) {
		if (airFurnaceSet.find(machCode)!= airFurnaceSet.end()) return;
		if (rollingMachFrequ.find(machCode) != rollingMachFrequ.end()) { // ����
			this->m_timeOfSwith = 0.1;   // ��ͬ���ƺ���Ҫ���ת�������ת��ʱ��5Сʱ
		}
		else if (stretchMachFrequ.find(machCode) != stretchMachFrequ.end()) {  //�����
			this->m_timeOfSwith = 0.2;   // ��ͬ���ƺ���Ҫ���ת�������ת��ʱ��5Сʱ
		}
		else if (washMachFrequ.find(machCode) != washMachFrequ.end()) {  //��ϴ��
			this->m_timeOfSwith = 0.2;   // ��ͬ���ƺ���Ҫ���ת�������ת��ʱ��5Сʱ
		}
	};

	virtual ~Mach() {};
};


// ��ȡ������ǰ�Ŀ�ȡ���ȣ����ȣ��������ھ�
double getCurWidth(Job* jobP, int machIndexOfJob);

double getCurWidth(Job* jobP, string machCode, int reentry);

double getCurThick(Job* jobP, int machIndexOfJob);

double getCurLength(Job* jobP, int machIndexOfJob);

double getCurWeight(Job* jobP, int machIndexOfJob);

double getCurInnerDia(Job* jobP, int machIndexOfJob);



// ����¯������
struct BellBatch {
	string m_strAlloyGrade;
	vector<pair<Job*, double>> m_jobsWithWidth;  // vector<pair<Job*, ���>>
	bool isFull=false;
};

using TimeWin = pair<map<string, unsigned>, time_period>;  // pair< map<job����, ��job�ڼ�������>, job�����ʱ�䴰>


// ������-������(������); ��1��, ÿ1����1������
//    �豸���뼰����: 
//         BD-S005	������; 
struct Mach_roughMill :public Mach {
	// ��������:
	//     1����������ѭ20�����һ��ԭ��
	//     2������֮����ѭ������Ʒ�������Լ� �ȿ��խ ��˳��
	//     3�����������ȺϽ�����ͭ����ͭ��ԭ��
	string m_machSubCode; // �������룬����ID

	virtual ~Mach_roughMill() {};
};


// ������-����¯(����ʽ�˻�¯); ��1��, 5��ƽ�л�����Ӧͬ1���豸���룬��ͬʱ����;
//    �豸���뼰����: 
//         BD-S006	����ʽ�˻�¯
struct Mach_BellFurnace:public Mach{
	// ��������: ���
	//		850����2��ÿ¯��650ϵ�У�850���£����3��/¯����װҲ��2��ÿ¯
	//      ÿ¯��ʱԼ30Сʱ����
	string m_machSubCode;      // �������룬����ID; һ�������Ӧһ��ƽ�л�����5��ƽ�л�
	unsigned m_numOfParallel = Num_Para_Bell;
	
	static pair<double, pair<int, int>> m_RuleForFurnWithWidth; // pair<���850, pair<ÿ¯����2, ÿ¯����3>> ��¯����; -- 850����2��ÿ¯��650ϵ�У�850���£����3��/¯����װҲ��2��ÿ¯
	double m_proceTimePerFurn = T_Bell_Furn;  // 30
	double m_waitTofFirstFurn = T_waitOfFirstFurn; // �¿�һ¯�ĵȴ�ʱ��
	double m_toleranceTofNewFurn = T_toleranceOfNewFurn; // �¿�һ¯������ʱ��

	vector<list<TimeWin>> m_timeLines;

	Mach_BellFurnace() {
		this->m_timeOfSwith = 0;
		for (unsigned i = 0; i < this->m_numOfParallel; i++)
			this->m_timeLines.push_back(list<TimeWin>());
	};

	virtual ~Mach_BellFurnace() {};
};


// ������-����¯��(����ʽ�˻�¯); ��3��, ÿ1����1������
//    �豸���뼰����: 
//         BD-S024 WSP����¯; 
//         BD-S007 1250����ʽ�˻�¯��; 
//         BD-S008 650����ʽ�˻�¯��
struct Mach_AirFurnace :public Mach {
	// ��������: �ƺţ�����ת��5��Сʱ
	string m_machSubCode; // �������룬����ID

	pair<double, pair<int, int>> m_cap; // ����ת��5��Сʱ
	Mach_AirFurnace() {
		this->m_timeOfSwith = 5;   // ��ͬ���ƺ���Ҫ���ת�������ת��ʱ��5Сʱ
	}

	virtual ~Mach_AirFurnace() {};
};
 

// ������-�����; ��1��, ÿ1����1������
//     �豸���뼰����: 
//     BD-S018	1250�������
struct Mach_cross :public Mach {
	// ��������: ���Ϲ��
	double m_timeOfSwith = 0;  //
	string m_machSubCode; // �������룬����ID
	Mach_cross() {
		this->m_timeOfSwith = 0;
	}

	virtual ~Mach_cross() {};
};



// �趨��ʼ�Ų���ʱ�䣬��ת����ptime��ʽ
ptime getCurTime();


// �ɺϽ��ƺŻ�ȡ�Ͻ�����
string getTypeFromGrade(const string& alloyGrade);


// ��ȡ������ǰ�Ŀ�ȡ���ȣ����ȣ��������ھ�

double getCurWidth(Job* jobP, int machIndexOfJob);

double getCurWidth(Job* jobP, string machCode, int reentry);

double getCurThick(Job* jobP, int machIndexOfJob);

double getCurLength(Job* jobP, int machIndexOfJob);

double getCurWeight(Job* jobP, int machIndexOfJob);

double getCurInnerDia(Job* jobP, int machIndexOfJob);



// ���߳����
struct threadInfoOfLS {
	vector<pair<string, Job*>>     m_jobOrderTmp;
	map<string, Job*>              m_jobsMapTmp;
	map<string, Mach*>             m_machsMapTmp;
};




// --------MySQL���ݿ⽻��--------

// �����ݿ�������ݳ�ʼ��Jobs��Machs����Ϣ
void initialByDatabase(MYSQL* mysql, vector<string> jobsCodeVec, vector<string> machsCodeVec, map<string, Job*> jobsMap, map<string, Mach*> machsMap);

// ��ʼ�����ã�����mysql���ݿ�
bool ConnectDatabase(MYSQL* mysql);

// ��ѯ���ݿⲢ���ز�ѯ���
MYSQL_RES* QueryDatabase1(MYSQL* mysql, char* sql);

// �����ݿ�������ݣ������ز����Ƿ�ɹ�
bool InsertDatabase1(MYSQL* mysql, char* sql);

// --------END OF--MySQL���ݿ⽻��--------




// --------���ܽ������--------

// �ж�_number�Ƿ���_aRange(��ʽΪ"[0.1,3.4]")�ķ�Χ��
bool isInRange(string _aRange, double _number);

// ��ȡjob�ڵ�ǰmach�ϵ�status���Ա���Ҳ���
string getStatus(string machCode, Job* jobP, unsigned machIndex);

// ��ȡ�ӹ�ʱ�䣻 ����mach��job�õ�job�ڸ�mach�ϵļӹ�ʱ��
time_duration getProcessTime2(Mach* machP, Job* jobP, unsigned machIndex);

// ��ȡ�ӹ�ʱ�䣻 ����mach��job�õ�job�ڸ�mach�ϵļӹ�ʱ��
time_duration getProcessTime(Mach* machP, Job* jobP, unsigned machIndex);
// time_duration getProcessTime_Simplified(Mach* machP, Job* jobP, unsigned machIndex)


// batch����
void batch();

// �ж��ַ����Ƿ�������
bool isNum(string str);

// string to double
double mystod(const char* str);

// string to int
int mystoi(const char* str);

// --------END OF--���ܽ������--------




// --------ʱ�����--------

// ����Ƚ�pair<Job*, ptime>
bool myCmpBy_ptime(const pair<Job*, ptime>& _a, const pair<Job*, ptime>& _b);

// ����Ƚ�pair<Job*, time_duration>
bool myCmpBy_time_duration(const pair<Job*, time_duration>& _a, const pair<Job*, time_duration>& _b);

// Сʱ����double��ת��Ϊtime_duration����
time_duration double2timeDuration(double _processT);

// time_duration����ת��ΪСʱ����double��
double timeDuration2Double(time_duration _timeDura);

// --------END OF--ʱ�����--------




// --------�Ų�����ĳ��������ĳ�����Ŀ���ʱ��--------

// ԭ��--��ĳ��������ĳ������������¯������¯��
//bool  insertJob(Job& curJob, Mach& curMach, unsigned machIndexOfJob);

// ��ĳ��������ĳ������������¯������¯��
bool  insertJob(Job& curJob, Mach& curMach, unsigned machIndexOfJob, map<string, Job*>& jobsMap);

// ��ĳ��������ĳ������¯
bool  insertJob(Job& curJob, Mach_AirFurnace& curMach, unsigned machIndexOfJob, map<string, Job*>& jobsMap);

// ��ĳ������������¯��ĳ��ƽ�л�
bool  insertJob(Job& curJob, Mach_BellFurnace& curMach, unsigned machIndexOfJob, map<string, Job*>& jobsMap);

// ��ĳ������������¯��ĳ��ƽ�л�
bool  insertJob(Job& curJob, Mach_BellFurnace& curMach, unsigned machIndexOfJob, bool yes, map<string, Job*> jobsMap);

// ��ĳ������������¯��ĳƽ�л������ز���ʱ�䴰�Ͳ�������
pair<unsigned, bool> preInsertJobToMachWithPreBatch(ptime readyTimeForOrder, Mach_BellFurnace& curMach, list<TimeWin>& timeline, time_period& timeWinResult);

// ��ĳ������������¯
bool  insertJobToBellFurnWithPreBatch(vector<pair<Job*, int>>& curBatch, ptime readyTimeForOrder, Mach_BellFurnace& curMach);

// ��ĳ������������¯��ĳ��ƽ�л�,���������¯������
int  insertJob(Job& curJob, Mach_BellFurnace& curMach, vector<vector<pair<Job*, int>>>& batchOfBell, map<string, Mach*>& machsMapLocal);

// ����¯���Ƿ���Լ�������¯��ĳ��timeWin
bool canAddToTimeWin(Job* curJobP, unsigned machIndexOfJob, TimeWin& timeWin, map<string, Job*> jobsMap);

// Ԥ��������¯���򣬰�ĳ������������¯��ĳƽ�л������ز���ʱ�䴰�Ͳ�������
pair<unsigned, bool> preInsertJobToMach(Job& curJob, Mach_BellFurnace& curMach, unsigned machIndexOfJob, list<TimeWin>& timeline, time_period& timeWinResult, map<string, Job*>& jobsMap);

// ��������¯������Ƿ���Ҫ����л�������ǰ�󹤼����ƺ��Ƿ���ͬ���жϣ�airFurnaceSet
bool getIsSwitch(Job& curJob, Mach& curMach, Job& otherJob);

// ��������¯������Ƿ���Ҫ����л�������ǰ�󹤼����ƺ��Ƿ���ͬ���жϣ�airFurnaceSet
bool getIsSwitch(Job& curJob, Mach_AirFurnace& curMach, Job& otherJob);  // �Ƿ���Ҫ�л�

// ���ڷ�����¯����������������Ƿ���Ҫ����л�������ǰ�󹤼����ƺ��Ƿ���ͬ���жϣ�
bool getIsSwitch(Job& curJob, Mach& curMach, Job& otherJob);  // �Ƿ���Ҫ�л�

// --------END OF--�Ų�����ĳ��������ĳ�����Ŀ���ʱ��--------




// --------��ʼ����أ���ʼ��Job��Machine--------

//  ����--�����õĳ�ʼ��
void myInitialization(vector<string>& jobsCodeVec, vector<string>& machsCodeVec, map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);

// ��ʼ�����ܡ����������ݿ����Ľ����ʼ�����������Ϣ
void initializeCaps(MYSQL_RES* res, vector<string>& machsCodeVec, map<string, Mach*>& machsMap);

// ���빤����Ϣ�����������ݿ����Ľ����ʼ��������Ϣ
void initializeJobs(MYSQL_RES* res, vector<string>& jobsCodeVec, map<string, Job*>& jobsMap);

// ��ʼ�������������ƹ�����Ϣ���ӹ�Ŀ��ͼӹ�������
void initializeJobs2(MYSQL_RES* res, vector<string>& jobsCodeVec, map<string, Job*>& jobsMap);

// ��ʼ�����������������ݿ����Ľ����ʼ��������Ϣ
void initialMachs(MYSQL_RES* res, vector<string>& machsCodeVec, map<string, Mach*>& machsMap);

// ��ʼ��������Ϣ
void initialMachs2(MYSQL_RES* res, vector<string>& machsCodeVec, map<string, Mach*>& machsMap);



// �鿴�Ƴ�
void printProcessLine(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);

// ��ȡjob����������ʱ�䣬����ʼ����ͬ��job vector��ÿ������Ĵ���ʱ�䣬�ܼӹ�ʱ�䣬�ɳ�ʱ�䣬��ֹʱ��
void initialJobVecs(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, vector<pair<Job*, ptime>>& jobsWithDueDate
	, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime);

// ��ʼ��
void initialJobsBatch(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, vector<string>& jobsCodeVec
	, map<string, vector<Job*>>& jobsWithBell, map<string, pair<Job*, int>>& jobsWithBells, vector<BellBatch*>& jobsBatch);

// Ϊjob��machԤ�ȷ���ʱ�䴰�ռ�
void allocateTimeWinSpace(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);

// ��ӡ����¯����ͳ�Ƹ���
void printBellCount(map<string, Job*>& jobsMap);

// ��ӡ���ڡ�����ʱ�䡢�ɳ�ʱ��
void printDueProSlackTime(vector<pair<Job*, ptime>>& jobsWithDueDate
	, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime);

// --------END OF--��ʼ����أ���ʼ��Job��Machine--------




// --------��ȡĿ�꺯��ֵ���--------

// ��ȡmakespanĿ�꺯��ֵ
double getMakespan(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);

// ��ȡĿ�꺯��ֵ <������ʱ��(Сʱ), �ӹ����й��������ʱ�䳤��(Сʱ)>
pair<double, double> getObjVals(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);

// ��ȡĿ�꺯��ֵ <������ʱ��(Сʱ), �ӹ����й��������ʱ�䳤��(Сʱ)>������ÿ��job������ʱ��
pair<double, double> getObjValsWithTardiness(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, map<Job*, double>& jobTardiness);

// --------END OF--��ȡĿ�꺯��ֵ���--------




// --------������--------

// ����������
void rule_Method(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, MYSQL* mysql,
	vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime);

// NEH�����������㣬jobOrder��Ҫ�ǿյģ�job����Ϣ�������jobsMap�У�
void NEH_MethodCore(vector<pair<string, Job*>>& jobOrder, map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, MYSQL* mysql,
	vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime);

// NEH����
void NEH_Method(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, MYSQL* mysql,
	vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime);

// GA���Ŵ�������
void GA_Method(map<string, Mach*>& machsMap, MYSQL* mysql,
	vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime);

// IPG���������׶ࣩ����
void IPG_Method(map<string, Mach*>& machsMap, MYSQL* mysql,
	vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime);

// --------END OF--������--------




// --------GA��ȡ��ʼ�����--------

// �������ŵĿ�ʼ�ӹ�ʱ�䣬��job��������
bool compareJobPPtime(pair<pair<Job*, int>, ptime> left, pair<pair<Job*, int>, ptime> right);

// ��jobOrder˳���Ų�����ȡ��ʼ�Ų����
pair<double, double> scheduleForGAByJobOrder(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, MYSQL* mysql, bool isPrint = true);

// ʹ��NEH������ȡ��ʼ������
pair<double, double> scheduleForGAByNEH(vector<pair<string, Job*>>& jobOrder, map<string, Mach*>& machsMap, MYSQL* mysql,
	vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime);


// ��������¯֮ǰ�Ĺ�������¯�м�Ĺ����ţ����һ���������ݺ���ģ������һ��������
pair<double, double> scheduleForGAByJobOrder_BefAir(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, MYSQL* mysql, bool isPrint = true);


// ��ȡGA�ı�����Ϣ
void getCodeInfoOfGA_Air(vector<pair<string, Job*>>& jobOrder, vector<pair<Job*, pair<int, int>>>& codeInfoOfGA);

// ��ȡGA��Ⱦɫ�������Ϣ������jobOrder��˳�����Ϊ0,1,2,3...
int getCodeInfoOfGA_All(vector<pair<string, Job*>>& jobOrder, map<string, pair<int, pair<int, int>>>& encodeInfoOfGA);

// --------END OF����GA��ȡ��ʼ�����--------




// --------��������أ����������̨��CSV�ļ�--------

// ��ӡ�����ų̽��
void printFinalRes(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);

// ��ӡ�����ų̽��
void printFinalRes2(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);

// ��ӡ�����ų̽��
void writeToCSV(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);

// ��ӡ�����ų̽��
void writeToCSV(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, MYSQL* mysql);

// --------END OF--��������أ����������̨��CSV�ļ�--------




// --------�Ų���������أ�һ��һ������ÿ������--------

pair<double, double> scheduleByJobOrder(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, MYSQL* mysql, bool isPrint = true);

// ��������¯�ĵ�һ¯
pair<double, double> scheduleByJobOrder2(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, vector<pair<string, Job*>> jobWithBell, int lastJobWithBell, bool isPrint);

// ÿ���������λ�ã����в���
double scheduleByJobOrder3(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapLocal, map<string, Mach*>& machsMapLocal, MYSQL* mysql, bool isPrint);

// ����¯���������
pair<double, double> scheduleByJobOrder4(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, vector<vector<pair<Job*, int>>>& batchOfBell, MYSQL* mysql, bool isPrint);

void moveTheFollowedTimeWin(map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, Mach* curMachP, int IndexOfTimeWin);

// ͬ��m_allocatedTimeWinPs��m_allocatedTimeWin
void syncTimeWinANDTimeWinPs(map<string, Mach*>& machsMapTemp);

// --------END OF--�Ų���������أ�һ��һ������ÿ����--------




// --------�����Ų���أ�����job��machine--------

// ����Job--��jobOrder�е�����job����һ�£����뵽jobsMapTemp��
void initJobsTemp(map<string, Job*>& jobsMapTemp, vector<pair<string, Job*>>& jobOrderTemp, vector<pair<string, Job*>>& jobOrder);

// ����machine--��machsMap�е�����machine����һ�£����뵽machsMapTemp��
void initMachsMapTemp(map<string, Mach*>& machsMapTemp, map<string, Mach*>& machsMap);

// ��Ԥ���Ų�ʱ--�����Ų�ʱ�䴰--��jobOrderOrig�е�job���Ų�ʱ�䴰������jobOrder�е�job
void resetJobsTemp(vector<pair<string, Job*>>& jobOrder, vector<pair<string, Job*>>& jobOrderOrig);

// ��Ԥ���Ų�ʱ--�����Ų�ʱ�䴰--��machsMapOrig�е�job���Ų�ʱ�䴰������machsMap�е�machine
void resetMachsMapTemp(map<string, Mach*>& machsMap, map<string, Mach*>& machsMapOrig);

// ��Ԥ���Ų�ʱ--��jobOrder�е�job���Ų�ʱ�䴰��0
void resetJobsToOrigin(vector<pair<string, Job*>>& jobOrder);

// ��Ԥ���Ų�ʱ--��machsMap�е��Ų�ʱ�䴰��0
void resetMachsMapToOrigin(map<string, Mach*>& machsMap);

// �򵥿���machine
void reInitMachsMapTemp(map<string, Mach*>& machsMapTemp, map<string, Mach*>& machsMap);

// �ͷ��ڴ棬�ͷ�new����job��mach-���߳��õ�
void releaseMemoryOfTmpJobsMachs(vector<pair<string, Job*>>& jobOrderTemp, map<string, Mach*>& machsMapTemp);

// ��ʼ���߳���Ϣ
vector<threadInfoOfLS*> initThreadsInfoOfLS(const int num_thread, vector<pair<string, Job*>>* jobOrderBefP, map<string, Mach*>* machsMapBefP);

// �ͷ��߳���Ϣ
void releaseThreadsInfoOfLS(const int num_thread, vector<threadInfoOfLS*>& threadInfos);

// --------END OF--�����Ų���أ�����job��machine--------






// --------��������--------

void initJobsInfo(vector<string> jobsCodeVec, vector<string> machsCodeVec, map<string, Job*> jobsMap, map<string, Mach*> machsMap);


// --------END OF--��������--------






// --------����--------

pair<Job*, Job*> splitJob(Job* jobP, int splitPosition);

// --------��������--------



