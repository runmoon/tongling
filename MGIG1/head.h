#pragma once

#define BOOST_DATE_TIME_SOURCE

#define _CRT_SECURE_NO_WARNINGS

//�������ļ����������Ҫ��ͷ�ļ�
#include<fstream>
#include<streambuf>

#include<iostream>
#include<vector>
#include<utility>
#include<map>
#include<set>
//#include<ctime>
#include <cstdlib> // Header file needed to use srand and rand
#include<algorithm>
#include<iostream>
#include<stdlib.h>

#include<sstream>
// mysql���
#include<Windows.h>
#include<WinSock.h>
#include<mysql.h>

#include <time.h>
// boostʱ�����
#include<boost/timer.hpp>
#include<boost/date_time.hpp>
#include<boost/date_time/gregorian/gregorian.hpp>
#include<boost/date_time/posix_time/posix_time.hpp>

#pragma comment(lib, "libmysql.lib")
#pragma comment(lib,"wsock32.lib")

using namespace std;
using namespace boost;
using namespace boost::gregorian;  //������ʽ
using namespace boost::posix_time;
const double T_Bell_Furn = 30.0;  // ��λ:Сʱ; ��Bell Furnace������¯ÿ¯30Сʱ
const int Num_Para_Bell = 5;  // ��λ:Сʱ; ����¯��5��ƽ�л�(Parallel machine)

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

	ProcessTargets m_initialInfos;  // �ӹ�Ŀ��
	ProcessTargets m_finalTargets;  // �ӹ�Ŀ��
	
	//vector<string>  m_proceMachs;     // ����(����)����  vector<mach����>
	//vector<pair<string, ProcessTargets>>  m_proceTargets;  // ��ÿ������(����)�ϵļӹ�Ŀ�꣬vector<pair<mach����, �ӹ�Ŀ��>>
	//vector<pair<string, time_duration>>  m_proceTimes;   // ��ÿ������(����)�ϵļӹ�ʱ����vector<pair<mach����, �ӹ�ʱ��>>

	vector<pair<string, unsigned>>  m_proceMachs;     // ����(����)����  vector<pair<mach����, ������ڼ�������>>
	vector<pair<pair<string, unsigned>, ProcessTargets>>  m_proceTargets;  // ��ÿ������(����)�ϵļӹ�Ŀ�꣬vector<pair<pair<mach����, ������ڼ�������>, �ӹ�Ŀ��>>
	vector<pair<pair<string, unsigned>, time_duration>>  m_proceTimes;     // ��ÿ������(����)�ϵļӹ�ʱ����vector<pair<pair<mach����, ������ڼ�������>, �ӹ�ʱ��>>


	time_duration m_totalProceTime;   // ��Job���ܼӹ�ʱ��

	unsigned m_curMachIndex;    // ��ǰҪ�ӹ���machine��m_proceMachs�е�index
	vector<pair<pair<string, unsigned>, time_period>> m_allocatedTimeWin;    // ��job�Ѿ��ӹ��˵Ĺ�����vector<pair<pair<mach����, �ڼ���>, �ӹ�ʱ�䴰>>
};

// ������
struct Mach {
	string m_machCode;        // �������룬����ID������¯��Ӧ5��ƽ�л�������һ�������Ӧһ������
	pair<double, double>                   m_thickWindow;             //�ɼӹ��ĺ�ȷ�Χ
	double                                 m_assignedWeight;          //��������������

	vector<pair<double, double> >          m_thickWindowsOfMach;      //����ɼӹ��ĺ�ȷ�Χ����Ӧ����Сʱ����
	vector<double>                         m_hourCapacities;          //����Сʱ���ܣ�

	vector<pair<pair<string, unsigned>, time_period>>  m_allocatedTimeWin;   // �����ϵ�ʱ����䣬vector<pair<pair<job����, �ڼ���>, �ӹ�ʱ�䴰>>
	
	vector<CapWithConditions> m_capsOriginalInfo;  // ����Ĳ�����Ϣ

	double m_timeOfSwith;


	virtual ~Mach() {};
};

using TimeWin = pair<map<Job*, unsigned>, time_period>;  // pair< map<Job*, ��job�ڼ�������>, job�����ʱ�䴰>

// ���ϣ��ƺţ�
struct m_alloyGrade
{
	// �Ͻ�
	map<string, set<string>> type2grade; // map<�Ͻ�����, set<�Ͻ��ƺ�>>
	map<string, string> grade2type;   // map<�Ͻ��ƺ�, set<�Ͻ�����>>
};


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


// ������-����¯(����ʽ����¯); ��1��, ��1������
//    �豸���뼰����: 
//         BD-S002 ����ʽ����¯
struct Mach_walkingBeamFurnace : public Mach {
	// ��������: ��ȣ��ƺ�
	// ��ͬ�Ŀ�ȹ����Ҫ���ת��, ���ת��ʱ��4��ʱ
	Mach_walkingBeamFurnace(){
		this->m_timeOfSwith = 4;   // ��ͬ�Ŀ�ȹ����Ҫ���ת�������ת��ʱ��4��ʱ;
	}

	//map<,>;  // ��� 640,850,1040,640,850,1250

	virtual ~Mach_walkingBeamFurnace() {};
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
	
	pair<double, pair<int, int>> m_RuleForFurnWithWidth = make_pair(850, make_pair(2, 3)); // pair<���850, pair<ÿ¯����, ÿ¯����>> ��¯����; -- 850����2��ÿ¯��650ϵ�У�850���£����3��/¯����װҲ��2��ÿ¯
	double m_proceTimePerFurn = T_Bell_Furn;  // 30
	vector<vector<TimeWin>> m_timeLines;

	Mach_BellFurnace() {
		this->m_timeOfSwith = 0;
		for (unsigned i = 0; i < this->m_numOfParallel; i++)
			this->m_timeLines.push_back(vector<TimeWin>());
	};

	virtual ~Mach_BellFurnace() {};
};


struct timeWinForBell
{
	Job* m_jobP;
	unsigned m_reentry;  // ��job�ڸ�mach���ǵڼ�������
	
	unsigned m_numOfjobAllcated; // ����job
	time_period m_timewide;
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


// ������-�ݼ���; ��4��, ÿ1����1������
//     �豸���뼰����: 
//     BD-S016	1250�ݼ�; BD-S019 650���ݼ�
//     BD-S020	650���ݼ�; BD-S021 350�ݼ�
struct Mach_slit :public Mach {
	// ��������: ���Ͽ�ȣ������ھ�
	//     �����Ĵ�Ҫԭ��: ��ȣ�״̬����?��
	  //
	string m_machSubCode; // �������룬����ID
	Mach_slit() {
		this->m_timeOfSwith = 0;
	};

	virtual ~Mach_slit() {};
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


// �ж��ַ����Ƿ�������
bool isNum(string str);

// string to double
double mystod(const char* str);

// Сʱ����double��ת��Ϊtime_duration����
time_duration double2timeDuration(double _processT);

// time_duration����ת��ΪСʱ����double��
double timeDuration2Double(time_duration _timeDura);

// ����Ƚ�pair<Job*, ptime>
bool myCmpBy_ptime(pair<Job*, ptime> _a, pair<Job*, ptime> _b);

// ����Ƚ�pair<Job*, time_duration>
bool myCmpBy_time_duration(pair<Job*, time_duration> _a, pair<Job*, time_duration> _b);



// ����mysql���ݿ�
bool ConnectDatabase(MYSQL* mysql);

// ��ѯ���ݿⲢ���ز�ѯ���
MYSQL_RES* QueryDatabase1(MYSQL* mysql, char* sql);

// �����õĳ�ʼ��
void myInitialization(vector<string>& jobsCodeVec, vector<string>& machsCodeVec, map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);

// ���ݿ��ȡ������Ϣ����ʼ��
void initializeJobs(MYSQL_RES* res, vector<string>& jobsCodeVec, map<string, Job*>& jobsMap);

// ��ʼ�������������ƹ�����Ϣ���ӹ�Ŀ��ͼӹ�������
void initializeJobs2(MYSQL_RES* res, vector<string>& jobsCodeVec, map<string, Job*>& jobsMap);

// ���ݿ��ȡ������Ϣ����ʼ��
void initialMachs(MYSQL_RES* res, vector<string>& machsCodeVec, map<string, Mach*>& machsMap);

// ��ʼ��������Ϣ
void initialMachs2(MYSQL_RES* res, vector<string>& machsCodeVec, map<string, Mach*>& machsMap);

// ��ĳ��������ĳ����
bool  insertJob(Job& curJob, Mach& curMach, unsigned machIndexOfJob);

// ��ȡ�ӹ�ʱ�䣻���ݻ������ͼӹ�Ŀ��õ��ڸû����ϵļӹ�ʱ��
time_duration getProcessTime(Mach* machP, Job* jobP, ProcessTargets const& processTargets, unsigned machIndex);

// ��ȡĿ�꺯��ֵ
pair<double, double> getObjVals(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);

// ��ӡ�����ų̽��
void printFinalRes(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);




