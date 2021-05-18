#pragma once

#define BOOST_DATE_TIME_SOURCE

#define _CRT_SECURE_NO_WARNINGS

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

#include<Windows.h>
#include<WinSock.h>
#include<mysql.h>

#include <time.h>

#include<boost/timer.hpp>
#include<boost/date_time.hpp>
#include<boost/date_time/gregorian/gregorian.hpp>
#include<boost/date_time/posix_time/posix_time.hpp>

#pragma comment(lib, "libmysql.lib")
#pragma comment(lib,"wsock32.lib")

using namespace std;
using namespace boost;
using namespace boost::gregorian;  //日期形式
using namespace boost::posix_time;


// 加工目标类
struct ProcessTargets
{
	double m_targetWeight;   //加工目标重量
	double m_targetWidth;    //加工目标宽度
	double m_targetThick;    //加工目标宽度
	double m_targetLength;   //加工目标长度
	double m_targetInnerDia;   //加工目标内径，internal diameter
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

	ProcessTargets m_finalTargets;  // 加工目标
	
	//vector<string>  m_proceMachs;     // 工序(机器)流程  vector<mach代码>
	//vector<pair<string, ProcessTargets>>  m_proceTargets;  // 在每个工序(机器)上的加工目标，vector<pair<mach代码, 加工目标>>
	//vector<pair<string, time_duration>>  m_proceTimes;   // 在每个工序(机器)上的加工时长，vector<pair<mach代码, 加工时长>>

	vector<pair<string, unsigned>>  m_proceMachs;     // 工序(机器)流程  vector<pair<mach代码, 本工序第几次重入>>
	vector<pair<pair<string, unsigned>, ProcessTargets>>  m_proceTargets;  // 在每个工序(机器)上的加工目标，vector<pair<pair<mach代码, 本工序第几次重入>, 加工目标>>
	vector<pair<pair<string, unsigned>, time_duration>>  m_proceTimes;   // 在每个工序(机器)上的加工时长，vector<pair<pair<mach代码, 第几重>, 本工序第几次重入>>


	time_duration m_totalProceTime;   // 该Job的总加工时长

	unsigned m_curMachIndex;    // 当前要加工的machine在m_proceMachs中的index
	vector<pair<pair<string, unsigned>, time_period>> m_allocatedTimeWin;    // 该job已经加工了的工序，在vector<pair<pair<mach代码, 第几重>, 加工时间窗>>
};

// 机器类
struct Mach {
	string m_machCode;        // 机器代码，机器ID；钟罩炉对应5个平行机，其他一个代码对应一个机器
	pair<double, double>                   m_thickWindow;             //可加工的厚度范围
	double                                 m_assignedWeight;          //排入机组的总重量

	vector<pair<double, double> >          m_thickWindowsOfMach;      //机组可加工的厚度范围；对应机组小时产能
	vector<double>                         m_hourCapacities;          //机组小时产能；

	vector<pair<pair<string, unsigned>, time_period>>  m_allocatedTimeWin;   // 机器上的时间分配，vector<pair<pair<job代码, 第几重>, 加工时间窗>>
	
	vector<CapWithConditions> m_capsOriginalInfo;  // 机组的产能信息
};


// 材料（牌号）
struct m_alloyGrade
{
	// 合金
	map<string, set<string>> type2grade; // map<合金种类, set<合金牌号>>
	map<string, string> grade2type;   // map<合金牌号, set<合金种类>>
};


// 机器类-粗轧机(粗轧机); 有1种, 每1种有1个机器
//    设备代码及名称: 
//         BD-S005	粗轧机; 
struct Mach_roughMill :public Mach {
	// 组批规则:
	//     1、工作辊遵循20卷更换一次原则。
	//     2、换辊之后遵循先轧成品，后开坯以及 先宽后窄 的顺序。
	//     3、开坯按照先合金、再紫铜最后黄铜的原则
	double m_timeOfSwith = 0;  //
	string m_machSubCode; // 机器代码，机器ID
	
};


// 机器类-步进炉(步进式加热炉); 有1种, 有1个机器
//    设备代码及名称: 
//         BD-S002 步进式加热炉
struct Mach_walkingBeamFurnace : public Mach {
	// 组批规则: 宽度，牌号
	// 不同的宽度规格需要规格转换, 规格转化时间4工时
	double m_timeOfSwith=4;  // 不同的宽度规格需要规格转换，规格转化时间4工时;

	//map<,>;  // 宽度 640,850,1040,640,850,1250
};




// 机器类-钟罩炉(钟罩式退火炉); 有1种, 5个平行机，对应同1个设备代码，可同时生产;
//    设备代码及名称: 
//         BD-S006	钟罩式退火炉
struct Mach_BellFurnace:public Mach{
	// 组批规则: 宽度
	//		850以上2卷每炉，650系列（850以下）最多3卷/炉；混装也是2卷每炉
	//      每炉工时约30小时左右
	double m_timeOfSwith = 0;
	string m_machSubCode; // 机器代码，机器ID; 一个代码对应一个平行机，共5个平行机

	pair<double, pair<int, int>> m_cap = make_pair(850, make_pair(2, 3)); // pair<850, pair<2, 3>> -- 850以上2卷每炉，650系列（850以下）最多3卷/炉；混装也是3卷每炉
	double m_proceTimePerFurnace;  // 30
};


// 机器类-气垫炉组(气垫式退火炉); 有3种, 每1种有1个机器
//    设备代码及名称: 
//         BD-S024 WSP气垫炉; 
//         BD-S007 1250气垫式退火炉组; 
//         BD-S008 650气垫式退火炉组
struct Mach_airFurnace :public Mach {
	// 组批规则: 牌号，规则转换5个小时
	double m_timeOfSwith = 5;  // 不同的牌号需要规格转换，规格转化时间5小时
	string m_machSubCode; // 机器代码，机器ID

	pair<double, pair<int, int>> m_cap; // 规则转化5个小时
};


// 机器类-纵剪组; 有4种, 每1种有1个机器
//     设备代码及名称: 
//     BD-S016	1250纵剪; BD-S019 650薄纵剪
//     BD-S020	650厚纵剪; BD-S021 350纵剪
struct Mach_slit :public Mach {
	// 组批规则: 出料宽度，出料内径
	//     组批的次要原则: 厚度，状态（？?）
	double m_timeOfSwith = 0;  //
	string m_machSubCode; // 机器代码，机器ID


};


// 机器类-横剪组; 有1种, 每1种有1个机器
//     设备代码及名称: 
//     BD-S018	1250横剪机组
struct Mach_cross :public Mach {
	// 组批规则: 出料规格
	double m_timeOfSwith = 0;  //
	string m_machSubCode; // 机器代码，机器ID

};


// 判断字符串是否是数字
bool isNum(string str);

// string to double
double mystod(char* str);

// 小时数（double）转化为time_duration类型
time_duration double2timeDuration(double _processT);

// time_duration类型转化为小时数（double）
double timeDuration2Double(time_duration _timeDura);

// 定义比较pair<Job*, ptime>
bool myCmpBy_ptime(pair<Job*, ptime> _a, pair<Job*, ptime> _b);

// 定义比较pair<Job*, time_duration>
bool myCmpBy_time_duration(pair<Job*, time_duration> _a, pair<Job*, time_duration> _b);



// 连接mysql数据库
bool ConnectDatabase(MYSQL* mysql);

// 查询数据库并返回查询结果
MYSQL_RES* QueryDatabase1(MYSQL* mysql, char* sql);

// 调试用的初始化
void myInitialization(vector<string>& jobsCodeVec, vector<string>& machsCodeVec, map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);

// 数据库读取工单信息并初始化
void initializeJobs(MYSQL_RES* res, vector<string>& jobsCodeVec, map<string, Job*>& jobsMap);

// 初始化工单——完善工单信息（加工目标和加工机器）
void initializeJobs2(MYSQL_RES* res, vector<string>& jobsCodeVec, map<string, Job*>& jobsMap);

// 数据库读取机器信息并初始化
void initialMachs(MYSQL_RES* res, vector<string>& machsCodeVec, map<string, Mach*>& machsMap);

// 初始化机器信息
void initialMachs2(MYSQL_RES* res, vector<string>& machsCodeVec, map<string, Mach*>& machsMap);

// 把某工单排入某机器
bool  insertJobToMach(Job& curJob, Mach& curMach, unsigned machIndexOfJob);

// 获取加工时间；根据机组代码和加工目标得到在该机器上的加工时间
time_duration getProcessTime(Mach* machP, Job* jobP, ProcessTargets const& processTargets, unsigned machIndex);

// 获取目标函数值
pair<double, double> getObjVals(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);

// 打印最终排程结果
void printFinalRes(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap);




