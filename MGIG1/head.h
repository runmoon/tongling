#pragma once

#define BOOST_DATE_TIME_SOURCE

#define _CRT_SECURE_NO_WARNINGS

//以下是文件读入输出需要的头文件
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
#include<random>  // 随机数
#include<stdlib.h>

#include<sstream>

// mysql相关
#include<Windows.h>
#include<WinSock.h>
#include<mysql.h>

#include <time.h>

// 多线程相关
#include<thread>
#include<mutex>

// boost时间相关
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
using namespace boost::gregorian;  //日期形式
using namespace boost::posix_time;



extern set<string> rollingMachFrequ;  // 轧机组;

extern set<string> cut_StretchMachFrequ;  // 横纵剪和拉弯矫;

extern set<string> cutMachFrequ;  // 横纵剪;

extern set<string> stretchMachFrequ;  // 拉弯矫; 

extern set<string> washMachFrequ;     // 清洗机列;

extern set<string> airFurnaceSet;     // 气垫炉;


// 钟罩炉
const double T_Bell_Furn = 30.0;  // 单位:小时; （Bell Furnace）钟罩炉每炉30小时
const int Num_Para_Bell = 5;  // 单位:小时; 钟罩炉共5个平行机(Parallel machine)
const double T_waitOfFirstFurn = 0.0;  // 单位:小时; （Bell Furnace）钟罩炉开第一炉的等待时间 5.0
const double T_toleranceOfNewFurn =20.0;  // 单位:小时; （Bell Furnace）钟罩炉开第一炉相较于插入已开炉的容忍时间

const string CodeOfBellFurn = "BD-S006";  // 钟罩炉的代码MachCode



// 加工目标类
struct ProcessTargets
{
	double m_targetWidth = 0.0;    //加工目标宽度
	double m_targetThick = 0.0;    //加工目标厚度
	double m_targetWeight = 0.0;   //加工目标重量
	double m_targetLength = 0.0;   //加工目标长度
	double m_targetInnerDia = 0.0;   //加工目标内径，internal diameter
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

// 工单类
class Job // 带卷
{
public:
	string m_jobCode;         // 带卷编号
	string m_orderCode;       // 订单编号，一个定单包括多个带卷
	string m_initSpec;        // initial specification, 来料规格
	string m_alloyGrade;       // 合金牌号，代表某种合金材料的一种
	string m_alloyType;        // bopper-黄铜, brass-黄铜, alloy-合金
	string m_dueDateOfOrderStr;  // 带卷目标交货期的字符串形式
	ptime m_dueDateOfOrder;      // 带卷目标交货期

	ptime m_startDateOfOrder; // time point that can start processing 

	ProcessTargets m_initialInfos;  // 初始信息
	ProcessTargets m_finalTargets;  // 加工目标
	
	//vector<string>  m_proceMachs;     // 工序(机器)流程  vector<mach代码>
	//vector<pair<string, ProcessTargets>>  m_proceTargets;  // 在每个工序(机器)上的加工目标，vector<pair<mach代码, 加工目标>>
	//vector<pair<string, time_duration>>  m_proceTimes;   // 在每个工序(机器)上的加工时长，vector<pair<mach代码, 加工时长>>

	vector<pair<string, unsigned>>  m_proceMachs;     // 工序(机器)流程  vector<pair<mach代码, 本工序第几次重入>>
	vector<pair<pair<string, unsigned>, ProcessTargets>>  m_proceTargets;  // 在每个工序(机器)上的加工目标，vector<pair<pair<mach代码, 本工序第几次重入>, 加工目标>>
	vector<pair<pair<string, unsigned>, time_duration>>  m_proceTimes;     // 在每个工序(机器)上的加工时长，vector<pair<pair<mach代码, 本工序第几次重入>, 加工时长>>

	 
	time_duration m_totalProceTime;   // 该Job的总加工时长

	unsigned m_curMachIndex = 0;    // 当前要加工的machine在m_proceMachs中的index
	vector<pair<pair<string, unsigned>, time_period>> m_allocatedTimeWin;    // 该job已经加工了的工序，在vector<pair<pair<mach代码, 第几重>, 加工时间窗>>
	// vector<pair<pair<string, unsigned>, time_period>*> m_allocatedTimeWinsP;    // 指针，该job已经加工了的工序，在vector<pair<pair<mach代码, 第几重>, 加工时间窗>>
	vector<pair<pair<string, unsigned>, time_period*>> m_allocatedTimeWinPs;    // 指针，该job已经加工了的工序，在vector<pair<pair<mach代码, 第几重>, 加工时间窗*>>
	//vector<pair<pair<Mach*, unsigned>, unsigned>> m_allocatedTimeWinIndexs;    // 该job已经加工了的工序，在vector<pair<pair<mach指针, 第几个加工时间窗>, 第几次重入>>

};

// 机器类
struct Mach {
	string m_machCode;        // 机器代码，机器ID；钟罩炉对应5个平行机，其他一个代码对应一个机器
	pair<double, double>                   m_thickWindow;             //可加工的厚度范围
	double                                 m_assignedWeight;          //排入机组的总重量

	vector<pair<double, double> >          m_thickWindowsOfMach;      //机组可加工的厚度范围；对应机组小时产能
	vector<double>                         m_hourCapacities;          //机组小时产能；

	list<pair<pair<string, unsigned>, time_period>>  m_allocatedTimeWin;   // 机器上的时间分配，vector<pair<pair<job代码, 第几重>, 加工时间窗>>
	list<pair<pair<string, unsigned>, time_period*>>  m_allocatedTimeWinPs;   // 机器上的时间分配，vector<pair<pair<Job指针, 第几重>, 加工时间窗*>>

	vector<CapWithConditions> m_capsOriginalInfo;  // 机组的产能信息

	double m_timeOfSwith{0.0};


	Mach() {};

	Mach(string machCode) {
		if (airFurnaceSet.find(machCode)!= airFurnaceSet.end()) return;
		if (rollingMachFrequ.find(machCode) != rollingMachFrequ.end()) { // 轧机
			this->m_timeOfSwith = 0.1;   // 不同的牌号需要规格转换，规格转化时间5小时
		}
		else if (stretchMachFrequ.find(machCode) != stretchMachFrequ.end()) {  //拉弯矫
			this->m_timeOfSwith = 0.2;   // 不同的牌号需要规格转换，规格转化时间5小时
		}
		else if (washMachFrequ.find(machCode) != washMachFrequ.end()) {  //清洗机
			this->m_timeOfSwith = 0.2;   // 不同的牌号需要规格转换，规格转化时间5小时
		}
	};

	virtual ~Mach() {};
};


// 获取工单当前的宽度、厚度，长度，重量，内径
double getCurWidth(Job* jobP, int machIndexOfJob);

double getCurWidth(Job* jobP, string machCode, int reentry);

double getCurThick(Job* jobP, int machIndexOfJob);

double getCurLength(Job* jobP, int machIndexOfJob);

double getCurWeight(Job* jobP, int machIndexOfJob);

double getCurInnerDia(Job* jobP, int machIndexOfJob);



// 钟罩炉的组批
struct BellBatch {
	string m_strAlloyGrade;
	vector<pair<Job*, double>> m_jobsWithWidth;  // vector<pair<Job*, 宽度>>
	bool isFull=false;
};

using TimeWin = pair<map<string, unsigned>, time_period>;  // pair< map<job代码, 该job第几次重入>, job插入的时间窗>


// 机器类-粗轧机(粗轧机); 有1种, 每1种有1个机器
//    设备代码及名称: 
//         BD-S005	粗轧机; 
struct Mach_roughMill :public Mach {
	// 组批规则:
	//     1、工作辊遵循20卷更换一次原则。
	//     2、换辊之后遵循先轧成品，后开坯以及 先宽后窄 的顺序。
	//     3、开坯按照先合金、再紫铜最后黄铜的原则
	string m_machSubCode; // 机器代码，机器ID

	virtual ~Mach_roughMill() {};
};


// 机器类-钟罩炉(钟罩式退火炉); 有1种, 5个平行机，对应同1个设备代码，可同时生产;
//    设备代码及名称: 
//         BD-S006	钟罩式退火炉
struct Mach_BellFurnace:public Mach{
	// 组批规则: 宽度
	//		850以上2卷每炉，650系列（850以下）最多3卷/炉；混装也是2卷每炉
	//      每炉工时约30小时左右
	string m_machSubCode;      // 机器代码，机器ID; 一个代码对应一个平行机，共5个平行机
	unsigned m_numOfParallel = Num_Para_Bell;
	
	static pair<double, pair<int, int>> m_RuleForFurnWithWidth; // pair<宽度850, pair<每炉个数2, 每炉个数3>> 组炉规则; -- 850以上2卷每炉，650系列（850以下）最多3卷/炉；混装也是2卷每炉
	double m_proceTimePerFurn = T_Bell_Furn;  // 30
	double m_waitTofFirstFurn = T_waitOfFirstFurn; // 新开一炉的等待时间
	double m_toleranceTofNewFurn = T_toleranceOfNewFurn; // 新开一炉的容忍时间

	vector<list<TimeWin>> m_timeLines;

	Mach_BellFurnace() {
		this->m_timeOfSwith = 0;
		for (unsigned i = 0; i < this->m_numOfParallel; i++)
			this->m_timeLines.push_back(list<TimeWin>());
	};

	virtual ~Mach_BellFurnace() {};
};


// 机器类-气垫炉组(气垫式退火炉); 有3种, 每1种有1个机器
//    设备代码及名称: 
//         BD-S024 WSP气垫炉; 
//         BD-S007 1250气垫式退火炉组; 
//         BD-S008 650气垫式退火炉组
struct Mach_AirFurnace :public Mach {
	// 组批规则: 牌号，规则转换5个小时
	string m_machSubCode; // 机器代码，机器ID

	pair<double, pair<int, int>> m_cap; // 规则转化5个小时
	Mach_AirFurnace() {
		this->m_timeOfSwith = 5;   // 不同的牌号需要规格转换，规格转化时间5小时
	}

	virtual ~Mach_AirFurnace() {};
};
 

// 机器类-横剪组; 有1种, 每1种有1个机器
//     设备代码及名称: 
//     BD-S018	1250横剪机组
struct Mach_cross :public Mach {
	// 组批规则: 出料规格
	double m_timeOfSwith = 0;  //
	string m_machSubCode; // 机器代码，机器ID
	Mach_cross() {
		this->m_timeOfSwith = 0;
	}

	virtual ~Mach_cross() {};
};



// 设定开始排产的时间，并转化成ptime格式
ptime getCurTime();


// 由合金牌号获取合金类型
string getTypeFromGrade(const string& alloyGrade);


// 获取工单当前的宽度、厚度，长度，重量，内径

double getCurWidth(Job* jobP, int machIndexOfJob);

double getCurWidth(Job* jobP, string machCode, int reentry);

double getCurThick(Job* jobP, int machIndexOfJob);

double getCurLength(Job* jobP, int machIndexOfJob);

double getCurWeight(Job* jobP, int machIndexOfJob);

double getCurInnerDia(Job* jobP, int machIndexOfJob);



// 多线程相关
struct threadInfoOfLS {
	vector<pair<string, Job*>>     m_jobOrderTmp;
	map<string, Job*>              m_jobsMapTmp;
	map<string, Mach*>             m_machsMapTmp;
};




// --------MySQL数据库交互--------

// 从数据库读入数据初始化Jobs和Machs的信息
void initialByDatabase(MYSQL* mysql, vector<string> jobsCodeVec, vector<string> machsCodeVec, map<string, Job*> jobsMap, map<string, Mach*> machsMap);

// 初始化设置，连接mysql数据库
bool ConnectDatabase(MYSQL* mysql);

// 查询数据库并返回查询结果
MYSQL_RES* QueryDatabase1(MYSQL* mysql, char* sql);

// 向数据库插入数据，并返回插入是否成功
bool InsertDatabase1(MYSQL* mysql, char* sql);

// --------END OF--MySQL数据库交互--------




// --------产能解析相关--------

// 判断_number是否在_aRange(格式为"[0.1,3.4]")的范围内
bool isInRange(string _aRange, double _number);

// 获取job在当前mach上的status，以便查找产能
string getStatus(string machCode, Job* jobP, unsigned machIndex);

// 获取加工时间； 根据mach和job得到job在该mach上的加工时间
time_duration getProcessTime2(Mach* machP, Job* jobP, unsigned machIndex);

// 获取加工时间； 根据mach和job得到job在该mach上的加工时间
time_duration getProcessTime(Mach* machP, Job* jobP, unsigned machIndex);
// time_duration getProcessTime_Simplified(Mach* machP, Job* jobP, unsigned machIndex)


// batch组批
void batch();

// 判断字符串是否是数字
bool isNum(string str);

// string to double
double mystod(const char* str);

// string to int
int mystoi(const char* str);

// --------END OF--产能解析相关--------




// --------时间相关--------

// 定义比较pair<Job*, ptime>
bool myCmpBy_ptime(const pair<Job*, ptime>& _a, const pair<Job*, ptime>& _b);

// 定义比较pair<Job*, time_duration>
bool myCmpBy_time_duration(const pair<Job*, time_duration>& _a, const pair<Job*, time_duration>& _b);

// 小时数（double）转化为time_duration类型
time_duration double2timeDuration(double _processT);

// time_duration类型转化为小时数（double）
double timeDuration2Double(time_duration _timeDura);

// --------END OF--时间相关--------




// --------排产，把某工单排入某机器的空闲时间--------

// 原版--把某工单排入某机器（非气垫炉、钟罩炉）
//bool  insertJob(Job& curJob, Mach& curMach, unsigned machIndexOfJob);

// 把某工单排入某机器（非气垫炉、钟罩炉）
bool  insertJob(Job& curJob, Mach& curMach, unsigned machIndexOfJob, map<string, Job*>& jobsMap);

// 把某工单排入某个气垫炉
bool  insertJob(Job& curJob, Mach_AirFurnace& curMach, unsigned machIndexOfJob, map<string, Job*>& jobsMap);

// 把某工单排入钟罩炉的某个平行机
bool  insertJob(Job& curJob, Mach_BellFurnace& curMach, unsigned machIndexOfJob, map<string, Job*>& jobsMap);

// 把某工单排入钟罩炉的某个平行机
bool  insertJob(Job& curJob, Mach_BellFurnace& curMach, unsigned machIndexOfJob, bool yes, map<string, Job*> jobsMap);

// 把某工单排入钟罩炉的某平行机，返回插入时间窗和插入索引
pair<unsigned, bool> preInsertJobToMachWithPreBatch(ptime readyTimeForOrder, Mach_BellFurnace& curMach, list<TimeWin>& timeline, time_period& timeWinResult);

// 把某工单排入钟罩炉
bool  insertJobToBellFurnWithPreBatch(vector<pair<Job*, int>>& curBatch, ptime readyTimeForOrder, Mach_BellFurnace& curMach);

// 把某工单排入钟罩炉的某个平行机,先组好钟罩炉的批次
int  insertJob(Job& curJob, Mach_BellFurnace& curMach, vector<vector<pair<Job*, int>>>& batchOfBell, map<string, Mach*>& machsMapLocal);

// 钟罩炉，是否可以加入钟罩炉的某个timeWin
bool canAddToTimeWin(Job* curJobP, unsigned machIndexOfJob, TimeWin& timeWin, map<string, Job*> jobsMap);

// 预排入钟罩炉工序，把某工单排入钟罩炉的某平行机，返回插入时间窗和插入索引
pair<unsigned, bool> preInsertJobToMach(Job& curJob, Mach_BellFurnace& curMach, unsigned machIndexOfJob, list<TimeWin>& timeline, time_period& timeWinResult, map<string, Job*>& jobsMap);

// 对于气垫炉，检查是否需要规格切换（根据前后工件的牌号是否相同来判断）airFurnaceSet
bool getIsSwitch(Job& curJob, Mach& curMach, Job& otherJob);

// 对于气垫炉，检查是否需要规格切换（根据前后工件的牌号是否相同来判断）airFurnaceSet
bool getIsSwitch(Job& curJob, Mach_AirFurnace& curMach, Job& otherJob);  // 是否需要切换

// 对于非气垫炉的其他机器，检查是否需要规格切换（根据前后工件的牌号是否相同来判断）
bool getIsSwitch(Job& curJob, Mach& curMach, Job& otherJob);  // 是否需要切换

// --------END OF--排产，把某工单排入某机器的空闲时间--------




// --------初始化相关，初始化Job和Machine--------

//  测试--调试用的初始化
void myInitialization(vector<string>& jobsCodeVec, vector<string>& machsCodeVec, map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);

// 初始化产能——根据数据库读入的结果初始化机组产能信息
void initializeCaps(MYSQL_RES* res, vector<string>& machsCodeVec, map<string, Mach*>& machsMap);

// 读入工单信息——根据数据库读入的结果初始化工单信息
void initializeJobs(MYSQL_RES* res, vector<string>& jobsCodeVec, map<string, Job*>& jobsMap);

// 初始化工单——完善工单信息（加工目标和加工机器）
void initializeJobs2(MYSQL_RES* res, vector<string>& jobsCodeVec, map<string, Job*>& jobsMap);

// 初始化机器——根据数据库读入的结果初始化机器信息
void initialMachs(MYSQL_RES* res, vector<string>& machsCodeVec, map<string, Mach*>& machsMap);

// 初始化机器信息
void initialMachs2(MYSQL_RES* res, vector<string>& machsCodeVec, map<string, Mach*>& machsMap);



// 查看制程
void printProcessLine(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);

// 获取job各个工序处理时间，并初始化不同的job vector，每个工序的处理时间，总加工时间，松弛时间，截止时间
void initialJobVecs(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, vector<pair<Job*, ptime>>& jobsWithDueDate
	, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime);

// 初始化
void initialJobsBatch(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, vector<string>& jobsCodeVec
	, map<string, vector<Job*>>& jobsWithBell, map<string, pair<Job*, int>>& jobsWithBells, vector<BellBatch*>& jobsBatch);

// 为job和mach预先分配时间窗空间
void allocateTimeWinSpace(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);

// 打印钟罩炉工序统计个数
void printBellCount(map<string, Job*>& jobsMap);

// 打印交期、处理时间、松弛时间
void printDueProSlackTime(vector<pair<Job*, ptime>>& jobsWithDueDate
	, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime);

// --------END OF--初始化相关，初始化Job和Machine--------




// --------获取目标函数值相关--------

// 获取makespan目标函数值
double getMakespan(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);

// 获取目标函数值 <总延期时间(小时), 加工所有工件所需的时间长度(小时)>
pair<double, double> getObjVals(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);

// 获取目标函数值 <总延期时间(小时), 加工所有工件所需的时间长度(小时)>，还有每个job的脱期时长
pair<double, double> getObjValsWithTardiness(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, map<Job*, double>& jobTardiness);

// --------END OF--获取目标函数值相关--------




// --------求解相关--------

// 按规则排序
void rule_Method(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, MYSQL* mysql,
	vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime);

// NEH方法核心运算，jobOrder需要是空的，job的信息都需存在jobsMap中！
void NEH_MethodCore(vector<pair<string, Job*>>& jobOrder, map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, MYSQL* mysql,
	vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime);

// NEH方法
void NEH_Method(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, MYSQL* mysql,
	vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime);

// GA（遗传）方法
void GA_Method(map<string, Mach*>& machsMap, MYSQL* mysql,
	vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime);

// IPG（迭代帕雷多）方法
void IPG_Method(map<string, Mach*>& machsMap, MYSQL* mysql,
	vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime);

// --------END OF--求解相关--------




// --------GA获取初始解相关--------

// 按照已排的开始加工时间，对job进行排序
bool compareJobPPtime(pair<pair<Job*, int>, ptime> left, pair<pair<Job*, int>, ptime> right);

// 按jobOrder顺序排产，获取初始排产结果
pair<double, double> scheduleForGAByJobOrder(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, MYSQL* mysql, bool isPrint = true);

// 使用NEH，来获取初始排序结果
pair<double, double> scheduleForGAByNEH(vector<pair<string, Job*>>& jobOrder, map<string, Mach*>& machsMap, MYSQL* mysql,
	vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime);


// 排入气垫炉之前的工序，气垫炉中间的工序不排；最后一个工序是纵横剪的，则最后一个工序不排
pair<double, double> scheduleForGAByJobOrder_BefAir(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, MYSQL* mysql, bool isPrint = true);


// 获取GA的编码信息
void getCodeInfoOfGA_Air(vector<pair<string, Job*>>& jobOrder, vector<pair<Job*, pair<int, int>>>& codeInfoOfGA);

// 获取GA的染色体编码信息：按照jobOrder的顺序编码为0,1,2,3...
int getCodeInfoOfGA_All(vector<pair<string, Job*>>& jobOrder, map<string, pair<int, pair<int, int>>>& encodeInfoOfGA);

// --------END OF——GA获取初始解相关--------




// --------结果输出相关，输出到控制台和CSV文件--------

// 打印最终排程结果
void printFinalRes(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);

// 打印最终排程结果
void printFinalRes2(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);

// 打印最终排程结果
void writeToCSV(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);

// 打印最终排程结果
void writeToCSV(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, MYSQL* mysql);

// --------END OF--结果输出相关，输出到控制台和CSV文件--------




// --------排产主函数相关，一个一个排入每个工序--------

pair<double, double> scheduleByJobOrder(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, MYSQL* mysql, bool isPrint = true);

// 先排钟罩炉的第一炉
pair<double, double> scheduleByJobOrder2(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, vector<pair<string, Job*>> jobWithBell, int lastJobWithBell, bool isPrint);

// 每个工序遍历位置，进行插入
double scheduleByJobOrder3(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapLocal, map<string, Mach*>& machsMapLocal, MYSQL* mysql, bool isPrint);

// 钟罩炉先组好批次
pair<double, double> scheduleByJobOrder4(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, vector<vector<pair<Job*, int>>>& batchOfBell, MYSQL* mysql, bool isPrint);

void moveTheFollowedTimeWin(map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, Mach* curMachP, int IndexOfTimeWin);

// 同步m_allocatedTimeWinPs和m_allocatedTimeWin
void syncTimeWinANDTimeWinPs(map<string, Mach*>& machsMapTemp);

// --------END OF--排产主函数相关，一个一个排入每个工--------




// --------迭代排产相关，拷贝job或machine--------

// 拷贝Job--把jobOrder中的所有job拷贝一下，放入到jobsMapTemp中
void initJobsTemp(map<string, Job*>& jobsMapTemp, vector<pair<string, Job*>>& jobOrderTemp, vector<pair<string, Job*>>& jobOrder);

// 拷贝machine--把machsMap中的所有machine拷贝一下，放入到machsMapTemp中
void initMachsMapTemp(map<string, Mach*>& machsMapTemp, map<string, Mach*>& machsMap);

// 有预先排产时--拷贝排产时间窗--把jobOrderOrig中的job的排产时间窗拷贝给jobOrder中的job
void resetJobsTemp(vector<pair<string, Job*>>& jobOrder, vector<pair<string, Job*>>& jobOrderOrig);

// 有预先排产时--拷贝排产时间窗--把machsMapOrig中的job的排产时间窗拷贝给machsMap中的machine
void resetMachsMapTemp(map<string, Mach*>& machsMap, map<string, Mach*>& machsMapOrig);

// 无预先排产时--把jobOrder中的job的排产时间窗清0
void resetJobsToOrigin(vector<pair<string, Job*>>& jobOrder);

// 无预先排产时--把machsMap中的排产时间窗清0
void resetMachsMapToOrigin(map<string, Mach*>& machsMap);

// 简单拷贝machine
void reInitMachsMapTemp(map<string, Mach*>& machsMapTemp, map<string, Mach*>& machsMap);

// 释放内存，释放new出的job和mach-多线程用到
void releaseMemoryOfTmpJobsMachs(vector<pair<string, Job*>>& jobOrderTemp, map<string, Mach*>& machsMapTemp);

// 初始化线程信息
vector<threadInfoOfLS*> initThreadsInfoOfLS(const int num_thread, vector<pair<string, Job*>>* jobOrderBefP, map<string, Mach*>* machsMapBefP);

// 释放线程信息
void releaseThreadsInfoOfLS(const int num_thread, vector<threadInfoOfLS*>& threadInfos);

// --------END OF--迭代排产相关，拷贝job或machine--------






// --------案例生成--------

void initJobsInfo(vector<string> jobsCodeVec, vector<string> machsCodeVec, map<string, Job*> jobsMap, map<string, Mach*> machsMap);


// --------END OF--案例生成--------






// --------其他--------

pair<Job*, Job*> splitJob(Job* jobP, int splitPosition);

// --------其他结束--------



