#pragma once

#include<head.h>
#include<Chromosome.h>
#include<GA.h>
#include<IPG.h>

std::mutex g_mutex;

pair<double, pair<int, int>> Mach_BellFurnace::m_RuleForFurnWithWidth = make_pair(1950, make_pair(2, 3));



// --------������أ���������ͺϽ�����--------

set<string> rollingMachFrequ{ "BD-S003","BD-S005","BD-S025","BD-S009","BD-S010","BD-S011" };  // ������; getStatus()��
// BD-S003	��������ʽ������; BD-S005	������; BD-S025	�Ĺ�������; 
// BD-S009	ɭ����20��������; BD-S010	20��������; BD-S011	4��������

set<string> cut_StretchMachFrequ{ "BD-S018","BD-S015",
					"BD-S016","BD-S019","BD-S020","BD-S021" };  // ���ݼ��������; getStatus()��
// BD-S015	650�����; 
// BD-S016	1250�ݼ�; BD-S019	650���ݼ�; BD-S020	650���ݼ�; BD-S021	350�ݼ�

set<string> cutMachFrequ{ "BD-S018", "BD-S016","BD-S019","BD-S020","BD-S021" };  // ���ݼ�;
// BD-S016	1250�ݼ�; BD-S019	650���ݼ�; BD-S020	650���ݼ�; BD-S021	350�ݼ�

set<string> stretchMachFrequ{ "BD-S015"};  // �����;
// BD-S015	650�����; 

set<string> washMachFrequ{ "BD-S012","BD-S013","BD-S014" };     // BD-S012	1250��ϴ����; BD-S013	650��ϴ����; BD-S014	��650��ϴ����; getStatus()��

set<string> airFurnaceSet{ "BD-S024","BD-S007","BD-S008" };     //BD-S024  WSP����¯;    BD-S007  1250����ʽ�˻�¯��;    BD-S008  650����ʽ�˻�¯��

set<string> machsSet{ "BD-S003","BD-S005","BD-S025","BD-S009","BD-S010","BD-S011"  // ����
, "BD-S024","BD-S007","BD-S008", "BD-S006"                                         // ����¯
, "BD-S018", "BD-S016","BD-S019","BD-S020","BD-S021"                               // ����
, "BD-S015"                                                                        // �����
, "BD-S012","BD-S013","BD-S014"                                                    // ��ϴ
, "BD-S002"                                                                      // ����ʽ����¯
, "BD-S004"                                                                      // ˫��ϳ������
, "BD-S026"                                                                      // ����ĥ������
};

set<string> machsSubSet{ "BD-S003","BD-S005","BD-S025","BD-S009","BD-S010","BD-S011"  // ����
, "BD-S024","BD-S007","BD-S008", "BD-S006"                                         // ����¯
, "BD-S018", "BD-S016","BD-S019","BD-S020","BD-S021"                               // ����
, "BD-S015"                                                                        // �����
, "BD-S013"                                                                        // ��ϴ
};

set<string> alloyGradeSet{ "C1010","C1020","C1100","C1220","C2600"
,"C2680","C2720","C2740","C7025" ,"C7701"
,"H62","H65" ,"H70","T2","TU1","TU1"};     // �Ͻ��ƺ�


map<string, string> alloyMap{    // �ɺϽ��ƺţ�grade�����Ӧ�ĺϽ����ࣨtype��
	make_pair("C1010", "copper"),
	make_pair("C1020", "copper"),
	make_pair("C1100", "copper"),
	make_pair("C1220", "copper"),
	make_pair("C2600", "brass"),
	make_pair("C2680", "brass"),
	make_pair("C2720", "brass"),
	make_pair("C2740", "brass"),
	make_pair("C7025", "alloy"),
	make_pair("C7701", "alloy"),
	make_pair("H62", "brass"),
	make_pair("H65", "brass"),
	make_pair("H70", "brass"),
	make_pair("T2", "copper"),
	make_pair("TU1", "copper"),
	make_pair("TU2", "copper"), };

map<string, string> mapMachCodeToName{
	make_pair("BD-S002", "����ʽ����¯"),
	make_pair("BD-S003", "��������ʽ������"),
	make_pair("BD-S004", "˫��ϳ������"),

	make_pair("BD-S005", "������"),
	make_pair("BD-S025", "�Ĺ�������"),

	make_pair("BD-S024", "WSP����¯"),
	make_pair("BD-S007", "1250����ʽ�˻�¯��"),
	make_pair("BD-S008", "650����ʽ�˻�¯��"),
	make_pair("BD-S006", "����ʽ�˻�¯"),

	make_pair("BD-S012", "1250��ϴ����"),
	make_pair("BD-S013", "650��ϴ����"),
	make_pair("BD-S014", "��650��ϴ����"),

	make_pair("BD-S009", "sundwig20��������"),
	make_pair("BD-S010", "20��������"),
	make_pair("BD-S011", "4��������"),

	make_pair("BD-S015", "650�����"),

	make_pair("BD-S016", "1250�ݼ�"),
	make_pair("BD-S018", "1250�������"),
	make_pair("BD-S019", "650���ݼ�"),
	make_pair("BD-S020", "650���ݼ�"),
	make_pair("BD-S021", "350�ݼ�"),

	make_pair("BD-S022", "ȫ�Զ���װ��"),
	make_pair("BD-S023", "���װ�װ��"),

	make_pair("BD-S026", "����ĥ������"),
};


map<string, double> machsCapMap{
	make_pair("BD-S003", 20000)   // ����
	, make_pair("BD-S005", 12000)
	, make_pair("BD-S025", 10526)
	, make_pair("BD-S009", 10526)
	, make_pair("BD-S010", 6667)
	, make_pair("BD-S011", 6741)  // ����--����
	, make_pair("BD-S024", 6000)    // ����ʽ�˻�¯
	, make_pair("BD-S007", 10526)
	, make_pair("BD-S008", 6780)    // ����ʽ�˻�¯--����
	, make_pair("BD-S006", 5405)  // ����ʽ�˻�¯
	, make_pair("BD-S018", 15000)  // ���ݼ�
	, make_pair("BD-S016", 15000)
	, make_pair("BD-S019", 4500)
	, make_pair("BD-S020", 15000)
	, make_pair("BD-S021", 7500)  // ���ݼ�--����
	, make_pair("BD-S015", 6000)  // �����                                                                    
	, make_pair("BD-S013", 6780)  // ��ϴ

	, make_pair("BD-S002", 20000)
	, make_pair("BD-S012", 20000)
	, make_pair("BD-S004", 24000)
	, make_pair("BD-S024", 6000)
};


/*
BD-S012	1250��ϴ����
BD-S013	650��ϴ����
BD - S014	��650��ϴ����

BD - S018	1250�������
BD - S015	650�����

BD - S016	1250�ݼ�
BD - S019	650���ݼ�
BD - S020	650���ݼ�
BD - S021	350�ݼ�
*/

// --------END OF--������أ���������ͺϽ�����--------



// �趨��ʼ�Ų���ʱ�䣬��ת����ptime��ʽ
ptime getCurTime() {
	//ptime curTime = from_iso_string("20200610T000000");
	ptime curTime = from_iso_string("20220101T0000000");
	return curTime;
};


// �ɺϽ��ƺŻ�ȡ�Ͻ�����
string getTypeFromGrade(const string& alloyGrade)
{
	return alloyMap[alloyGrade];
}


// ��ȡ������ǰ�Ŀ�ȡ���ȣ����ȣ��������ھ�

double getCurWidth(Job* jobP, int machIndexOfJob)  // �򻯰�
{
	return jobP->m_initialInfos.m_targetWidth;
};

double getCurWidth(Job* jobP, string machCode, int reentry)  // �򻯰�
{
	return jobP->m_initialInfos.m_targetWidth;
};

double getCurWidth2(Job* jobP, int machIndexOfJob)  // ԭ��
{
	double curWidth(0); //job�ĵ�ǰ���
	for (int i = machIndexOfJob; i >= 0; --i)
	{
		if (jobP->m_proceTargets[i].second.m_targetWidth > 0.0)  // �ӵ�ǰ����ǰ��Ŀ���ȴ���0��ֵ 
		{
			curWidth = jobP->m_proceTargets[i].second.m_targetWidth;
			break;
		}
	}
	if (curWidth <= 0.0)   // ��Ŀ���ȶ���0 
		curWidth = jobP->m_initialInfos.m_targetWidth;
	return curWidth;
};

double getCurWidth2(Job* jobP, string machCode, int reentry)  // ԭ��
{
	int machIndexOfJob = -1;
	for (int i = 0; i < jobP->m_proceMachs.size(); ++i) {
		if (jobP->m_proceMachs[i].first == machCode && jobP->m_proceMachs[i].second == reentry) {
			machIndexOfJob = i;
			break;
		}
	}
	if (machIndexOfJob == -1) cout << "Error! The 'machCode' and 'reentry' is not right!" << endl;

	double curWidth(0); //job�ĵ�ǰ���
	for (int i = machIndexOfJob; i >= 0; --i)
	{
		if (jobP->m_proceTargets[i].second.m_targetWidth > 0.0)  // �ӵ�ǰ����ǰ��Ŀ���ȴ���0��ֵ 
		{
			curWidth = jobP->m_proceTargets[i].second.m_targetWidth;
			break;
		}
	}
	if (curWidth <= 0.0)   // ��Ŀ���ȶ���0 
		curWidth = jobP->m_initialInfos.m_targetWidth;
	return curWidth;
};


double getCurThick(Job* jobP, int machIndexOfJob)
{
	double curThick(0); //job�ĵ�ǰ���
	for (int i = machIndexOfJob; i >= 0; --i)
	{
		if (jobP->m_proceTargets[i].second.m_targetThick > 0.0)  // �ӵ�ǰ����ǰ��Ŀ���ȴ���0��ֵ 
		{
			curThick = jobP->m_proceTargets[i].second.m_targetThick;
			break;
		}
	}
	if (curThick <= 0.0)   // ��Ŀ���ȶ���0 
		curThick = jobP->m_initialInfos.m_targetThick;
	return curThick;
};

double getCurLength(Job* jobP, int machIndexOfJob)
{
	double curLength(0); //job�ĵ�ǰ���
	for (int i = machIndexOfJob; i >= 0; --i)
	{
		if (jobP->m_proceTargets[i].second.m_targetLength > 0.0)  // �ӵ�ǰ����ǰ��Ŀ���ȴ���0��ֵ 
		{
			curLength = jobP->m_proceTargets[i].second.m_targetLength;
			break;
		}
	}
	if (curLength <= 0.0)   // ��Ŀ���ȶ���0 
		curLength = jobP->m_initialInfos.m_targetLength;
	return curLength;
};

double getCurWeight(Job* jobP, int machIndexOfJob)
{
	double curWeight(0); //job�ĵ�ǰ���
	for (int i = machIndexOfJob; i >= 0; --i)
	{
		if (jobP->m_proceTargets[i].second.m_targetWeight > 0.0)  // �ӵ�ǰ����ǰ��Ŀ���ȴ���0��ֵ 
		{
			curWeight = jobP->m_proceTargets[i].second.m_targetWeight;
			break;
		}
	}
	if (curWeight <= 0.0)   // ��Ŀ���ȶ���0 
		curWeight = jobP->m_initialInfos.m_targetWeight;
	return curWeight;
};

double getCurInnerDia(Job* jobP, int machIndexOfJob)
{
	double curInnerDia(0); //job�ĵ�ǰ���
	for (int i = machIndexOfJob; i >= 0; --i)
	{
		if (jobP->m_proceTargets[i].second.m_targetWeight > 0.0)  // �ӵ�ǰ����ǰ��Ŀ���ȴ���0��ֵ 
		{
			curInnerDia = jobP->m_proceTargets[i].second.m_targetWeight;
			break;
		}
	}
	if (curInnerDia <= 0.0)   // ��Ŀ���ȶ���0 
		curInnerDia = jobP->m_initialInfos.m_targetInnerDia;
	return curInnerDia;
};




// --------MySQL���ݿ⽻��--------

// �����ݿ�������ݳ�ʼ��Jobs��Machs����Ϣ
void initialByDatabase(MYSQL* mysql, vector<string>& jobsCodeVec, vector<string>& machsCodeVec, map<string, Job*>& jobsMap, map<string, Mach*>& machsMap) {
	MYSQL_RES* res(NULL);   //����ṹ�������е�һ����ѯ�����  
	ConnectDatabase(mysql);

	char* sqla = "select * from tlorderinformation order by alloy_grade, deadline;"; //  select * from tlorderinformation
	res = QueryDatabase1(mysql, sqla);
	initializeJobs(res, jobsCodeVec, jobsMap);

	sqla = "select * from tlunit";
	res = QueryDatabase1(mysql, sqla);
	initialMachs(res, machsCodeVec, machsMap);

	sqla = "select * from tlcapacity"; //,  by tape_lable, order_id order_id
	res = QueryDatabase1(mysql, sqla);
	initializeCaps(res, machsCodeVec, machsMap);

	sqla = "select * from tlprocess"; //,  by tape_lable, order_id order_id
	res = QueryDatabase1(mysql, sqla);
	initializeJobs2(res, jobsCodeVec, jobsMap);

	// ��ʼ��job�Ľ���ʱ��Ϳ�ʼ����
	for (auto& jobInfo : jobsMap)  // ��������job
	{
		Job* curJobP = jobInfo.second;
		curJobP->m_dueDateOfOrder = from_iso_string((curJobP->m_dueDateOfOrderStr) + "T115959");  // ��ʼ��job�Ľ���ʱ��
		// ���job�����ڸ�ʽ��������!!??
		curJobP->m_startDateOfOrder = getCurTime();
	}
	

	std::default_random_engine randm(27);
	int numOfJob = 100;
	std::uniform_int_distribution<int> dis_weight(4000, 20000);  // ����������
	vector<double> widths{ 650, 850 };   // job�Ŀ�ȹ��
	// ���㹤���ʱ�䣬���㽻��ʱ��
	for (auto& ele : jobsMap)
	{
		Job* jobP = ele.second;
		cout << "job: " << jobP->m_jobCode << endl;
		jobP->m_initialInfos.m_targetWeight = dis_weight(randm);    // ����650,850
		jobP->m_initialInfos.m_targetWidth = widths[rand() % widths.size()];
	}

};

// ��ʼ�����ã�����mysql���ݿ�
bool ConnectDatabase(MYSQL* mysql)
{
	//��ʼ��mysql  
	mysql_init(mysql);
	char* host("127.0.0.1"); //�CMYSQL�������ĵ�ַ��
	char* user("root"); //�C��¼�û�����
	char* passwd("root"); //�C��¼���룻
	char* db("tongling"); //�CҪ���ӵ����ݿ⣻
	unsigned int port(3306); //�CMYSQL��������TCP����˿ڣ� ? ?

	//����false������ʧ�ܣ�����true�����ӳɹ�
	if (!(mysql_real_connect(mysql, host, user, passwd, db, port, NULL, 0))) //�м�ֱ����������û��������룬���ݿ������˿ںţ�����дĬ��0����3306�ȣ���������д�ɲ����ٴ���ȥ  
	{
		printf("Error connecting to database:%s\n", mysql_error(mysql));
		return false;
	}
	else
	{
		printf("Connected...\n");
		return true;
	}
	return true;
}

// ��ѯ���ݿⲢ���ز�ѯ���
MYSQL_RES* QueryDatabase1(MYSQL* mysql, char* sql)
{
	MYSQL_RES* res;
	//���ñ����ʽ��SET NAMES GBKҲ�У�������cmd����������
	mysql_query(mysql, "set names gbk");
	// ��ѯmysql���ݿ�
	if (mysql_real_query(mysql, sql, (unsigned long)strlen(sql)))
	{
		std::cout << "mysql_real_query failure! " << std::endl;
		return 0;
	}
	// �洢�����
	res = mysql_store_result(mysql);
	if (NULL == res) //  if( !(res = mysql_store_result(&mysql))
	{
		std::cout << "mysql_store_result failure! " << std::endl;
		return 0;
	}

	//��ӡ��������  
	//std::cout << "number of dataline returned: " << mysql_affected_rows(mysql) << std::endl;
	std::cout << "number of dataline returned: " << mysql_num_rows(res) << std::endl;
	return res;
}

// �����ݿ�������ݣ������ز����Ƿ�ɹ�
bool InsertDatabase1(MYSQL* mysql, char* sql)
{
	//���ñ����ʽ��SET NAMES GBKҲ�У�������cmd����������
	mysql_query(mysql, "set names gbk");

	//mysql_query(mysql, "START TRANSACTION");
	// ��ѯmysql���ݿ�
	cout << "iinsertting" << endl;
	if (mysql_query(mysql, sql))
	{
		std::cout << "mysql_query failure! " << std::endl;
		return 0;
	}
	//mysql_query(mysql, "COMMIT");
	return true;
}

// --------END OF--MySQL���ݿ⽻��--------




// --------���ܽ������--------

// �ж�_number�Ƿ���_aRange(��ʽΪ"[0.1,3.4]")�ķ�Χ��
bool isInRange(string _aRange, double _number)
{
	if (("null" == _aRange) || ("NULL" == _aRange)
		|| ("Null" == _aRange) || ("" == _aRange)
		|| (_number <= 0.0))
		return true;
	size_t site = _aRange.find(",", 0);
	size_t len = _aRange.length();

	if ((0 < site) && (site < len)
		&& (3 < len) && (len < 30))  // �������Ƴ��ȣ���Ȼ��ȷ��
	{
		char leftSign = _aRange[0];
		char rightSign = _aRange[len - 1];
		double lowBound = stod(_aRange.substr(1, site));
		double upperBound = stod(_aRange.substr(site + 1, len - 1));
		if (('(' == leftSign) && (')' == rightSign))
			return (lowBound < _number) && (_number < upperBound);
		if (('(' == leftSign) && (']' == rightSign))
			return (lowBound < _number) && (_number <= upperBound);
		if (('[' == leftSign) && (')' == rightSign))
			return (lowBound <= _number) && (_number < upperBound);
		if (('[' == leftSign) && (']' == rightSign))
			return (lowBound <= _number) && (_number <= upperBound);
		else
		{
			std::cout << "ERROR with '" << _aRange << "'�� ���ܽ����� " << std::endl;
			return false;
		}
	}
	else
	{
		std::cout << "ERROR with '" << _aRange << "'�� ���ܽ����� " << std::endl;
		return false;
	}
};

// ��ȡjob�ڵ�ǰmach�ϵ�status���Ա���Ҳ���
string getStatus(string machCode, Job* jobP, unsigned machIndex)
{
	pair<string, unsigned>& machs = (jobP->m_proceMachs)[machIndex];
	if ("BD-S002" == machCode)  //��BD-S002  ����ʽ����¯������Ĭ������??����
		return "ingotting";
	if ("BD-S007" == machCode) // ��BD-S007  1250����ʽ�˻�¯��??����
		return "semi_hard_state";

	if (rollingMachFrequ.count(machCode) > 0) //������
	{
		if ("BD-S003" == machCode) // ��BD-S003	  ��������ʽ������
			return "hot_rolling";
		if (("BD-S009" == machCode) || ("BD-S010" == machCode)) // ��BD-S009  ɭ����20��������
		{
			return "finished_product"; //�жϺ��滹��û��������û����Ϊ������Ʒ������Ϊ��Ʒ
			"intermediate_material";
		}
		// BD-S003	��������ʽ������, BD-S005	������; BD-S025	�Ĺ�������; 
		// BD-S009	ɭ����20��������; BD-S010	20��������; BD-S011	4��������
		return "";
	}

	if (cut_StretchMachFrequ.count(machCode) > 0) //�Ǽ��л�����
	{
		// �жϺ����ǲ���ֻ�а�װ��û�й�������Ϊ��Ʒ(finished_product)�����������¹����Ƿ��Ǻ��(cross_fedding);
		// ��װ��: BD-S022  ȫ�Զ���װ��, BD-S023  ���װ�װ��
		// ���¹����Ǻ��������Ϊ�������(deburring)
		if ((machIndex + 1) == jobP->m_proceMachs.size())  // BD-S016	1250�ݼ�; BD-S020	650���ݼ�;
			return "finished_product";
		if (("BD-S022" == jobP->m_proceMachs[machIndex + 1].first)
			|| ("BD-S023" == jobP->m_proceMachs[machIndex + 1].first))
			return "cross_fedding";
		else
			return "deburring";
		// BD-S018	1250�������; BD-S015	650�����; 
		// BD-S016	1250�ݼ�; BD-S019	650���ݼ�; BD-S020	650���ݼ�; BD-S021	350�ݼ�
		if ("BD-S019" == machCode) // ��BD-S019  650���ݼ�
			return "finished_product";
		if ("BD-S015" == machCode) // ��BD-S015  650�����
			return "";
		return ""; // BD-S021	350�ݼ�?,BD-S018	1250�������?	
	}

	if (washMachFrequ.count(machCode) > 0) //����ϴ
	{
		if ("BD-S012" == machCode) // ��BD-S012  1250��ϴ����
			return "finished_product";
		if ("BD-S013" == machCode) // ��BD-S013	650��ϴ����
			return "";
		if ("BD-S014" == machCode)
			return ""; // ?
	}

	return "";
}

// ��ȡ�ӹ�ʱ�䣻 ����mach��job�õ�job�ڸ�mach�ϵļӹ�ʱ��
time_duration getProcessTime2(Mach* machP, Job* jobP, unsigned machIndex)
{
	//return time_duration(2, 0, 0, 0);

	ProcessTargets const& processTargets = jobP->m_proceTargets[machIndex].second;  //??����
	double processT(0);
	// bool isRollingMach(rollingMachFrequ.count(machP->m_machCode) > 0);
	if (CodeOfBellFurn == machP->m_machCode)  // ���������¯
		return double2timeDuration(T_Bell_Furn);

	for (auto& cap : machP->m_capsOriginalInfo)
	{
		if (!isInRange(cap.m_width, processTargets.m_targetWeight))
			continue;
		if (!isInRange(cap.m_thick, processTargets.m_targetThick))
			continue;
		if ((cap.m_alloyType != jobP->m_alloyType)
			&& ("general_alloy" != cap.m_alloyType)
			&& ("null" != cap.m_alloyType)
			&& ("Null" != cap.m_alloyType)
			&& ("NULL" != cap.m_alloyType)
			&& ("" != cap.m_alloyType))
			continue;
		string status = getStatus(machP->m_machCode, jobP, machIndex);

		//std::cout << "status=" << status << "cap.m_status=" << cap.m_status << std::endl;

		if ((cap.m_status != status)
			&& ("general_status" != cap.m_status)
			&& ("null" != cap.m_status)
			&& ("Null" != cap.m_status)
			&& ("NULL" != cap.m_status)
			&& ("" != cap.m_status))
			continue;
		processT = mystod(cap.m_processT.c_str());
	}
	if (0.0 == processT)
		std::cout << "Error with capacity! Cannot find a matched capacity of job '"
		<< jobP->m_jobCode << "' on machine " << machP->m_machCode << "!" << std::endl;

	//processT = 0.0 == processT ? 0.5 : processT;


	if (0.0 == processT)
	{
		cout << "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee" << endl;
		cout << "    machCode=" <<machP->m_machCode << endl;
		cout << "    m_targetWeight=" <<processTargets.m_targetWeight << endl;
		cout << "    m_targetThick="<<processTargets.m_targetThick << endl;
		cout << "    m_targetWidth=" << processTargets.m_targetWidth << endl;
		cout << "    m_alloyType="<<jobP->m_alloyType << endl;
		cout << "    status="<<getStatus(machP->m_machCode, jobP, machIndex) << endl;
	}

	//std::cout << "machCode=" << machP->m_machCode << "; jobCode="<<jobP->m_jobCode <<"; cap="<<processT<< std::endl;
	return double2timeDuration(processT);
	// return double2timeDuration(processT*2);
};


// ��ȡ�ӹ�ʱ�䣻 ����mach��job�õ�job�ڸ�mach�ϵļӹ�ʱ�䣬�򻯰�
time_duration getProcessTime(Mach* machP, Job* jobP, unsigned machIndex)
// time_duration getProcessTime_Simplified(Mach* machP, Job* jobP, unsigned machIndex)
{
	//return time_duration(2, 0, 0, 0);
	double cap = machsCapMap[machP->m_machCode];
	double processT(0.0);
	double weigt = jobP->m_initialInfos.m_targetWeight;
	if (weigt <= 0.0){
		char c;
		cin >> c;
	}
	processT = weigt/cap;
	if (machP->m_machCode == CodeOfBellFurn)
		processT = 30.0;
	cout << " weigt=" << weigt << "  cap=" << cap << endl;

	if (0.0 == processT)
	{
		cout << "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee" << endl;
		cout << "    machCode=" << machP->m_machCode << endl;
		cout << "    m_alloyType=" << jobP->m_alloyType << endl;
		cout << "    status=" << getStatus(machP->m_machCode, jobP, machIndex) << endl;
	}

	//std::cout << "machCode=" << machP->m_machCode << "; jobCode="<<jobP->m_jobCode <<"; cap="<<processT<< std::endl;
	return double2timeDuration(processT);
	// return double2timeDuration(processT*2);
};



// batch����
void batch()
{


}

// �ж��ַ����Ƿ�������
bool isNum(string str)
{
	stringstream sin(str);
	double d;
	char c;
	if (!(sin >> d))  //sin >> t��ʾ��sinת����double�ı�������ʵ����int��float�͵Ķ�����գ������ת���ɹ�����ֵΪ��0�����ת�����ɹ��ͷ���Ϊ0
		return false;
	if (sin >> c)
		return false;
	return true;
};

// string to double
double mystod(const char* str)
{
	if (str == NULL)
	{
		std::cout << "Error with str-'" << str << "'! Str is NULL��" << std::endl;
		return -1;  // ֵΪ��
	}
	else if (isNum(str))
		return stod(str);
	else
	{
		std::cout << "Error with str-'" << str << "'! Str is NOT A NUMBER��" << std::endl;
		return -2;  // ��������
	}
}

// string to int
int mystoi(const char* str)
{
	if (str == NULL)
	{
		std::cout << "Error with str- " << str << "! Str is NULL��" << std::endl;
		return -1;  // ֵΪ��
	}
	else if (isNum(str))
		return stoi(str);
	else
	{
		std::cout << "Error with str- " << str << "! Str is NOT A NUMBER��" << std::endl;
		return -2;  // ��������
	}
}

// --------END OF--���ܽ������--------




// --------ʱ�����--------

// ����Ƚ�pair<Job*, ptime>
bool myCmpBy_ptime(const pair<Job*, ptime>& _a, const pair<Job*, ptime>& _b)
{
	return _b.second < _a.second;
};

// ����Ƚ�pair<Job*, time_duration>
bool myCmpBy_time_duration(const pair<Job*, time_duration>& _a, const pair<Job*, time_duration>& _b)
{
	return _a.second < _b.second;
};

// Сʱ����double��ת��Ϊtime_duration����
time_duration double2timeDuration(double _processT)
{
	int _n_Sec = int(_processT * 3600);//ת����

	int _hour = 0;
	int _minute = 0;
	int _second = 0;
	if (_n_Sec >= 3600)
	{
		_hour = _n_Sec / 3600;
		_n_Sec = _n_Sec % 3600;
	}
	if (_n_Sec >= 60)
	{
		_minute = _n_Sec / 60;
		_n_Sec = _n_Sec % 60;
	}
	_second = _n_Sec;
	return time_duration(_hour, _minute, _second, 0);
};

// time_duration����ת��ΪСʱ����double��
double timeDuration2Double(time_duration _timeDura)
{
	return double(_timeDura.total_seconds()) / 3600;
};

// --------END OF--ʱ�����--------




// --------�Ų�����ĳ��������ĳ�����Ŀ���ʱ��--------

/*
// ԭ��--��ĳ��������ĳ������������¯������¯�����������������л�ʱ��
bool  insertJob(Job& curJob, Mach& curMach, unsigned machIndexOfJob)
{
	time_duration& processTime = curJob.m_proceTimes[machIndexOfJob].second;
	//time_duration& swithT = double2timeDuration(curMach.m_timeOfSwith);  // �л�ʱ��
	//bool swith = isSwitchPre(curJob, curMach, machIndexOfJob);  // �Ƿ���Ҫ�л�

	bool isInserted(false);
	unsigned IndexOfTimeWin(0);  // machine�Ĳ��������

	ptime readyTimeForOrder;  // job�����ڸû����Ͽ�ʼ�ӹ���ʱ��
	time_period timeWinToInsert(curJob.m_startDateOfOrder, processTime);  //Ҫ�����ʱ�䴰
	if (0 == machIndexOfJob)  // �����job�ĵ�һ��machine
		readyTimeForOrder = curJob.m_startDateOfOrder;
	else
		readyTimeForOrder = (curJob.m_allocatedTimeWin.end() - 1)->second.end();

	if (0 == curMach.m_allocatedTimeWin.size())  // �����machine�ϵĵ�һ��job
	{
		timeWinToInsert = time_period(readyTimeForOrder, processTime);
		IndexOfTimeWin = 0;
		isInserted = true;
	}
	else
	{
		// ����curMach.m_allocatedTimeWin
		for (auto timeInfo_iter = curMach.m_allocatedTimeWin.begin(); timeInfo_iter != curMach.m_allocatedTimeWin.end(); ++timeInfo_iter, ++IndexOfTimeWin)
		{
			ptime left_TimeWin = timeInfo_iter->second.begin();
			ptime right_TimeWin = timeInfo_iter->second.last();
			if (left_TimeWin < readyTimeForOrder + processTime)  // �ɼӹ�ʱ���ʱ�䴰��ʼʱ��֮��һ���Ų���
				continue;

			if (curMach.m_allocatedTimeWin.cbegin() == timeInfo_iter)  // ��һ��time window
			{
				//std::cout << "**************** " << std::endl;
				timeWinToInsert = time_period(readyTimeForOrder, processTime);
				isInserted = true;
				break;
			}
			else
			{
				auto timeInfo_iter2 = timeInfo_iter;
				--timeInfo_iter2;
				ptime right_Pre = timeInfo_iter2->second.end();
				if ((readyTimeForOrder < right_Pre) && (right_Pre + processTime <= left_TimeWin))
				{
					timeWinToInsert = time_period(right_Pre, processTime);
					isInserted = true;
					break;
				}
				if (right_Pre <= readyTimeForOrder)
				{
					timeWinToInsert = time_period(readyTimeForOrder, processTime);
					isInserted = true;
					break;
				}
			}
		}  // END OF ����curMach.m_allocatedTimeWin

		if (false == isInserted)
		{
			auto iter = curMach.m_allocatedTimeWin.end();
			ptime right_Pre = (--iter)->second.end();
			timeWinToInsert = time_period(right_Pre > readyTimeForOrder ? right_Pre : readyTimeForOrder, processTime);
			isInserted = true;
		}
	}
	curJob.m_curMachIndex = machIndexOfJob;
	curJob.m_allocatedTimeWin.push_back(make_pair(curJob.m_proceMachs[machIndexOfJob], timeWinToInsert));

	auto iter = curMach.m_allocatedTimeWin.begin();
	std::advance(iter, IndexOfTimeWin);

	g_mutex.lock();
	curMach.m_allocatedTimeWin.insert(iter,
		make_pair(make_pair(curJob.m_jobCode, curJob.m_proceMachs[machIndexOfJob].second), timeWinToInsert));
	g_mutex.unlock();

	return true;
};
*/

// ��ĳ��������ĳ������������¯������¯��
bool  insertJob(Job& curJob, Mach& curMach, unsigned machIndexOfJob, map<string, Job*>& jobsMap)
{
	time_duration& processTime = curJob.m_proceTimes[machIndexOfJob].second;
	time_duration& switchT = double2timeDuration(curMach.m_timeOfSwith);

	bool isInserted(false);
	unsigned IndexOfTimeWin(0);  // machine�Ĳ��������

	ptime readyTimeForOrder;  // job�����ڸû����Ͽ�ʼ�ӹ���ʱ��
	time_period timeWinToInsert(curJob.m_startDateOfOrder, processTime);  //Ҫ�����ʱ�䴰
	bool isSwitchPre;  // ����ǰ��Ļ������м��Ƿ���Ҫת��ʱ��
	if (0 == machIndexOfJob)  // �����job�ĵ�һ��machine
		readyTimeForOrder = curJob.m_startDateOfOrder;
	else
		readyTimeForOrder = (curJob.m_allocatedTimeWin.end() - 1)->second.end();
	//if ("BD-S008" == curMach.m_machCode) cout << "yyyyyyyyyyyyy" << endl;

	if (0 == curMach.m_allocatedTimeWin.size())  // �����machine�ϵĵ�һ��job
	{
		timeWinToInsert = time_period(readyTimeForOrder, processTime);
		IndexOfTimeWin = 0;
		isInserted = true;
		isSwitchPre = true;
	}
	else
	{
		// ����curMach.m_allocatedTimeWin
		for (auto timeInfo_iter = curMach.m_allocatedTimeWin.begin();
			timeInfo_iter != curMach.m_allocatedTimeWin.end(); ++timeInfo_iter, ++IndexOfTimeWin)
		{
			ptime left_TimeWin = timeInfo_iter->second.begin();
			ptime right_TimeWin = timeInfo_iter->second.last();
			//if ("BD-S008" == curMach.m_machCode) cout << "uppppppppppp" << endl;
			bool isSwitchNext = getIsSwitch(curJob, curMach, *jobsMap[timeInfo_iter->first.first]);    // ���Ǻ���Ļ������Ƿ���Ҫת��ʱ��

			if (left_TimeWin < readyTimeForOrder + processTime + (isSwitchNext ? switchT : double2timeDuration(0.0)))  // �ɼӹ�ʱ���ʱ�䴰��ʼʱ��֮��һ���Ų���
				continue;

			if (curMach.m_allocatedTimeWin.cbegin() == timeInfo_iter)  // ��һ��time window
			{
				timeWinToInsert = time_period(readyTimeForOrder, processTime);
				isInserted = true;
				break;
			}
			else  // isSwitchPre*switchT
			{
				auto timeInfo_iter2 = timeInfo_iter;
				--timeInfo_iter2;
				ptime right_Pre = timeInfo_iter2->second.end();

				isSwitchPre = getIsSwitch(curJob, curMach, *jobsMap[timeInfo_iter2->first.first]);

				if ((readyTimeForOrder - (isSwitchPre ? switchT : double2timeDuration(0.0)) < right_Pre) &&
					(right_Pre + processTime + (isSwitchPre ? switchT : double2timeDuration(0.0))
						+ (isSwitchNext ? switchT : double2timeDuration(0.0)) <= left_TimeWin))
				{
					timeWinToInsert = time_period(right_Pre + (isSwitchPre ? switchT : double2timeDuration(0.0)), processTime);
					isInserted = true;
					break;
				}
				if (right_Pre <= readyTimeForOrder - (isSwitchPre ? switchT : double2timeDuration(0.0)))
				{
					timeWinToInsert = time_period(readyTimeForOrder, processTime);
					isInserted = true;
					break;
				}
			}
		}  // END OF ����curMach.m_allocatedTimeWin

		if (false == isInserted)
		{
			auto iter = curMach.m_allocatedTimeWin.end();
			ptime right_Pre = (--iter)->second.end();
			isSwitchPre = getIsSwitch(curJob, curMach, *jobsMap[iter->first.first]);
			timeWinToInsert = time_period(right_Pre > readyTimeForOrder - (isSwitchPre ? switchT : double2timeDuration(0.0)) ?
				right_Pre + (isSwitchPre ? switchT : double2timeDuration(0.0)) : readyTimeForOrder, processTime);
			isInserted = true;
		}
	}
	//if ("BD-S008" == curMach.m_machCode) cout << "uuuuuuuuuuuuu" << endl;
	curJob.m_curMachIndex = machIndexOfJob;
	curJob.m_allocatedTimeWin.push_back(make_pair(curJob.m_proceMachs[machIndexOfJob], timeWinToInsert));

	auto iter = curMach.m_allocatedTimeWin.begin();
	std::advance(iter, IndexOfTimeWin);

	g_mutex.lock();
	curMach.m_allocatedTimeWin.insert(iter,
		make_pair(make_pair(curJob.m_jobCode, curJob.m_proceMachs[machIndexOfJob].second), timeWinToInsert));
	g_mutex.unlock();

	return true;
};;

// ��ĳ��������ĳ������¯
bool  insertJob(Job& curJob, Mach_AirFurnace& curMach, unsigned machIndexOfJob, map<string, Job*>& jobsMap)
{
	time_duration& processTime = curJob.m_proceTimes[machIndexOfJob].second;
	time_duration& switchT = double2timeDuration(curMach.m_timeOfSwith);

	bool isInserted(false);
	unsigned IndexOfTimeWin(0);  // machine�Ĳ��������

	ptime readyTimeForOrder;  // job�����ڸû����Ͽ�ʼ�ӹ���ʱ��
	time_period timeWinToInsert(curJob.m_startDateOfOrder, processTime);  //Ҫ�����ʱ�䴰
	bool isSwitchPre;  // ����ǰ��Ļ������м��Ƿ���Ҫת��ʱ��
	if (0 == machIndexOfJob)  // �����job�ĵ�һ��machine
		readyTimeForOrder = curJob.m_startDateOfOrder;
	else
		readyTimeForOrder = (curJob.m_allocatedTimeWin.end() - 1)->second.end();
	//if ("BD-S008" == curMach.m_machCode) cout << "yyyyyyyyyyyyy" << endl;

	if (0 == curMach.m_allocatedTimeWin.size())  // �����machine�ϵĵ�һ��job
	{
		timeWinToInsert = time_period(readyTimeForOrder, processTime);
		IndexOfTimeWin = 0;
		isInserted = true;
		isSwitchPre = true;
	}
	else
	{
		// ����curMach.m_allocatedTimeWin
		for (auto timeInfo_iter = curMach.m_allocatedTimeWin.begin();
			timeInfo_iter != curMach.m_allocatedTimeWin.end(); ++timeInfo_iter, ++IndexOfTimeWin)
		{
			ptime left_TimeWin = timeInfo_iter->second.begin();
			ptime right_TimeWin = timeInfo_iter->second.last();
			//if ("BD-S008" == curMach.m_machCode) cout << "uppppppppppp" << endl;
			bool isSwitchNext = getIsSwitch(curJob, curMach, *jobsMap[timeInfo_iter->first.first]);    // ���Ǻ���Ļ������Ƿ���Ҫת��ʱ��

			if (left_TimeWin < readyTimeForOrder + processTime + (isSwitchNext ? switchT : double2timeDuration(0.0)))  // �ɼӹ�ʱ���ʱ�䴰��ʼʱ��֮��һ���Ų���
				continue;

			if (curMach.m_allocatedTimeWin.cbegin() == timeInfo_iter)  // ��һ��time window
			{
				timeWinToInsert = time_period(readyTimeForOrder, processTime);
				isInserted = true;
				break;
			}
			else  // isSwitchPre*switchT
			{
				auto timeInfo_iter2 = timeInfo_iter;
				--timeInfo_iter2;
				ptime right_Pre = timeInfo_iter2->second.end();

				isSwitchPre = getIsSwitch(curJob, curMach, *jobsMap[timeInfo_iter2->first.first]);

				if ((readyTimeForOrder - (isSwitchPre ? switchT : double2timeDuration(0.0)) < right_Pre) &&
					(right_Pre + processTime + (isSwitchPre ? switchT : double2timeDuration(0.0))
						+ (isSwitchNext ? switchT : double2timeDuration(0.0)) <= left_TimeWin))
				{
					timeWinToInsert = time_period(right_Pre + (isSwitchPre ? switchT : double2timeDuration(0.0)), processTime);
					isInserted = true;
					break;
				}
				if (right_Pre <= readyTimeForOrder - (isSwitchPre ? switchT : double2timeDuration(0.0)))
				{
					timeWinToInsert = time_period(readyTimeForOrder, processTime);
					isInserted = true;
					break;
				}
			}
		}  // END OF ����curMach.m_allocatedTimeWin

		if (false == isInserted)
		{
			auto iter = curMach.m_allocatedTimeWin.end();
			ptime right_Pre = (--iter)->second.end();
			isSwitchPre = getIsSwitch(curJob, curMach, *jobsMap[iter -> first.first]);
			timeWinToInsert = time_period(right_Pre > readyTimeForOrder - (isSwitchPre ? switchT : double2timeDuration(0.0)) ?
				right_Pre + (isSwitchPre ? switchT : double2timeDuration(0.0)) : readyTimeForOrder, processTime);
			isInserted = true;
		}
	}
	//if ("BD-S008" == curMach.m_machCode) cout << "uuuuuuuuuuuuu" << endl;
	curJob.m_curMachIndex = machIndexOfJob;
	curJob.m_allocatedTimeWin.push_back(make_pair(curJob.m_proceMachs[machIndexOfJob], timeWinToInsert));

	auto iter = curMach.m_allocatedTimeWin.begin();
	std::advance(iter, IndexOfTimeWin);
	
	g_mutex.lock();
	curMach.m_allocatedTimeWin.insert(iter,
		make_pair(make_pair(curJob.m_jobCode, curJob.m_proceMachs[machIndexOfJob].second), timeWinToInsert));
	g_mutex.unlock();

	return true;
};;

// ��ĳ������������¯��ĳ��ƽ�л�
bool  insertJob(Job& curJob, Mach_BellFurnace& curMach, unsigned machIndexOfJob, map<string, Job*> &jobsMap)
{
	time_duration& processTime = curJob.m_proceTimes[machIndexOfJob].second;


	pair<pair<int, int>, time_period> newFurnRes = (make_pair(pair<unsigned, unsigned>(make_pair(-1, -1))
		, time_period(curJob.m_startDateOfOrder, processTime))); // ��¼�¿�һ¯�����; pair<pair< �ڼ���timeline, ����λ��>, ʱ�䴰>
	pair<pair<int, int>, time_period> notNewFurnRes = (make_pair(pair<unsigned, unsigned>(make_pair(-1, -1))
		, time_period(curJob.m_startDateOfOrder, processTime))); // ��¼��������¯�����; pair<pair< �ڼ���timeline, ����λ��>, ʱ�䴰>

	unsigned Index_parallelMach = 0;
	pair<int, bool> resultOfPreInsert(make_pair(-1, false));   // pair<TimeWin��λ��, �Ƿ�Ҫ�¿�һ¯>
	for (auto& timeline : curMach.m_timeLines)
	{
		//cout << "  Index_parallelMach=" << Index_parallelMach <<"  processTime=" <<to_iso_string(processTime)<< endl;
		time_period timeWinResult(curJob.m_startDateOfOrder, processTime);

		resultOfPreInsert = preInsertJobToMach(curJob, curMach, machIndexOfJob, timeline, timeWinResult, jobsMap);
		//cout << "  endddddddddddd" << endl;

		if (resultOfPreInsert.second) // ���¿�1¯
		{
			if (newFurnRes.first.first < 0.0)
				newFurnRes = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
			else if (newFurnRes.second.begin() > timeWinResult.begin())
				newFurnRes = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
		}
		else // �����¿�1¯
		{
			if (notNewFurnRes.first.first < 0.0)
				notNewFurnRes = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
			else if (notNewFurnRes.second.begin() > timeWinResult.begin())
				notNewFurnRes = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
		}
		// cout << "  Index_parallelMach=" << Index_parallelMach << endl;
		++Index_parallelMach;
	}

	unsigned Index_final(0);
	time_period timeWinFinal(curJob.m_startDateOfOrder, processTime);
	int insertPosition(-1);  // ����λ��
	bool isNew;
	if ((newFurnRes.first.second >= 0) && (notNewFurnRes.first.second >= 0))
	{
		if (notNewFurnRes.second.begin() < newFurnRes.second.begin() + double2timeDuration(curMach.m_toleranceTofNewFurn))
		{
			Index_final = notNewFurnRes.first.first;
			insertPosition = notNewFurnRes.first.second;
			timeWinFinal = notNewFurnRes.second;
			isNew = false;
		}
		else
		{
			Index_final = newFurnRes.first.first;
			insertPosition = newFurnRes.first.second;
			timeWinFinal = newFurnRes.second;
			isNew = true;
		}

	}
	else if (newFurnRes.first.second >= 0)
	{
		Index_final = newFurnRes.first.first;
		insertPosition = newFurnRes.first.second;
		timeWinFinal = newFurnRes.second;
		isNew = true;
	}
	else
	{
		Index_final = notNewFurnRes.first.first;
		insertPosition = notNewFurnRes.first.second;
		timeWinFinal = notNewFurnRes.second;
		isNew = false;
	}

	curJob.m_curMachIndex = machIndexOfJob;
	curJob.m_allocatedTimeWin.push_back(make_pair(curJob.m_proceMachs[machIndexOfJob], timeWinFinal));

	list<TimeWin>& timeLine = curMach.m_timeLines[Index_final];
	unsigned num_reentry = curJob.m_proceMachs[machIndexOfJob].second;
	map<string, unsigned> jobToInsert{ make_pair(curJob.m_jobCode, num_reentry) };

	if (insertPosition < 0)  return false;

	auto iterTmp = timeLine.begin();
	for (int i = 0; i < insertPosition; ++i)  ++iterTmp;

	g_mutex.lock();
	if (isNew) {  // �¿�һ¯
		timeLine.insert(iterTmp, make_pair(jobToInsert, timeWinFinal));
	}
	else  // �����ѿ���¯
		iterTmp->first.insert(make_pair(curJob.m_jobCode, num_reentry));
	g_mutex.unlock();

	return true;
};

// ��ĳ������������¯��ĳ��ƽ�л�
bool  insertJob(Job& curJob, Mach_BellFurnace& curMach, unsigned machIndexOfJob, bool yes, map<string, Job*> jobsMap)
{
	map<string, vector<Job*>> waitForBell;  //map<�ƺ�, vector<Job*>>
	vector<Job*> jobSameGrade;
	for (auto& jobsInfo : waitForBell)
	{
		if (jobsInfo.first == curJob.m_alloyGrade)
		{
			jobSameGrade = jobsInfo.second;
		}
	}
	if (0 == jobSameGrade.size())
		waitForBell.insert(make_pair(curJob.m_alloyGrade, vector<Job*> {&curJob}));
	else if (1 == jobSameGrade.size()) {
		if ( (getCurWidth(&curJob, machIndexOfJob) >= 850) || (getCurWidth((*jobSameGrade.begin()), 0) >= 850) )
		{
			//��¯
		}
		else
		{
			//if ()  // ���У���ȴ�
			//else
				//
		}

	}


	time_duration& processTime = curJob.m_proceTimes[machIndexOfJob].second;
	pair<pair<int, int>, time_period> newFurnRes = (make_pair(pair<unsigned, unsigned>(make_pair(-1, -1))
		, time_period(curJob.m_startDateOfOrder, processTime))); // ��¼�¿�һ¯�����; pair<pair< �ڼ���timeline, ����λ��>, ʱ�䴰>
	pair<pair<int, int>, time_period> notNewFurnRes = (make_pair(pair<unsigned, unsigned>(make_pair(-1, -1))
		, time_period(curJob.m_startDateOfOrder, processTime))); // ��¼��������¯�����; pair<pair< �ڼ���timeline, ����λ��>, ʱ�䴰>

	unsigned Index_parallelMach = 0;
	pair<int, bool> resultOfPreInsert(make_pair(-1, false));   // pair<TimeWin��λ��, �Ƿ�Ҫ�¿�һ¯>
	for (auto& timeline : curMach.m_timeLines)
	{
		time_period timeWinResult(curJob.m_startDateOfOrder, processTime);

		resultOfPreInsert = preInsertJobToMach(curJob, curMach, machIndexOfJob, timeline, timeWinResult, jobsMap);

		if (resultOfPreInsert.second) // ���¿�1¯
		{
			if (newFurnRes.first.first < 0.0)
				newFurnRes = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
			else if (newFurnRes.second.begin() > timeWinResult.begin())
				newFurnRes = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
		}
		else // �����¿�1¯
		{
			if (notNewFurnRes.first.first < 0.0)
				notNewFurnRes = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
			else if (notNewFurnRes.second.begin() > timeWinResult.begin())
				notNewFurnRes = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
		}
		// cout << "  Index_parallelMach=" << Index_parallelMach << endl;
		++Index_parallelMach;
	}

	unsigned Index_final(0);
	time_period timeWinFinal(curJob.m_startDateOfOrder, processTime);
	int insertPosition(-1);  // ����λ��
	bool isNew;
	if ((newFurnRes.first.second >= 0) && (notNewFurnRes.first.second >= 0))
	{
		if (notNewFurnRes.second.begin() < newFurnRes.second.begin() + double2timeDuration(curMach.m_toleranceTofNewFurn))
		{
			Index_final = notNewFurnRes.first.first;
			insertPosition = notNewFurnRes.first.second;
			timeWinFinal = notNewFurnRes.second;
			isNew = false;
		}
		else
		{
			Index_final = newFurnRes.first.first;
			insertPosition = newFurnRes.first.second;
			timeWinFinal = newFurnRes.second;
			isNew = true;
		}

	}
	else if (newFurnRes.first.second >= 0)
	{
		Index_final = newFurnRes.first.first;
		insertPosition = newFurnRes.first.second;
		timeWinFinal = newFurnRes.second;
		isNew = true;
	}
	else
	{
		Index_final = notNewFurnRes.first.first;
		insertPosition = notNewFurnRes.first.second;
		timeWinFinal = notNewFurnRes.second;
		isNew = false;
	}

	curJob.m_curMachIndex = machIndexOfJob;
	curJob.m_allocatedTimeWin.push_back(make_pair(curJob.m_proceMachs[machIndexOfJob], timeWinFinal));

	list<TimeWin>& timeLine = curMach.m_timeLines[Index_final];
	unsigned num_reentry = curJob.m_proceMachs[machIndexOfJob].second;
	map<string, unsigned> jobToInsert{ make_pair(curJob.m_jobCode, num_reentry) };

	if (insertPosition < 0)
		return false;

	auto iterTmp = timeLine.begin();
	for (int i = 0; i < insertPosition; ++i)  ++iterTmp;

	if (isNew) // �¿�һ¯
		timeLine.insert(iterTmp, make_pair(jobToInsert, timeWinFinal));
	else  // �����ѿ���¯
		iterTmp->first.insert(make_pair(curJob.m_jobCode, num_reentry));

	return true;
};

// ��ĳ������������¯��ĳƽ�л������ز���ʱ�䴰�Ͳ�������
pair<unsigned, bool> preInsertJobToMachWithPreBatch(ptime readyTimeForOrder, Mach_BellFurnace& curMach, list<TimeWin>& timeline, time_period& timeWinResult)
{
	time_duration& processTime = double2timeDuration(curMach.m_proceTimePerFurn);  //����¯ʱ��Ϊ30Сʱ
	//time_duration timeOfSwith = double2timeDuration(curMach.m_timeOfSwith);  // �����µ�һ¯��ת��ʱ��
	//׼��ʱ�����һ¯�ļӹ����ʱ�䣬�Ƿ�Ҫ����ת��ʱ�䣬��Ҫ�����ף���??
	//   ��job������׼��ʱ�����Ҫת��ʱ�䣻��machine��������һ¯����ʱ�����Ҫת��ʱ�䣡

	unsigned IndexOfTimeWin(0);  // machine��ʱ���߲��������
	// using TimeWin = pair<vector<pair<Job*, unsigned>>, time_period>;  // pair< vector<pair<Job*, ��job�ڼ�������>>, job�����ʱ�䴰>

	bool flag_NewFurn(false);  // ��ʾ�Ƿ��¿�һ¯
	bool isInserted(false);

	//cout << " timeline.size()=" << timeline.size()
	//	<< "  readyTimeForOrder="<<to_iso_extended_string(readyTimeForOrder)<<endl;

	if (0 == timeline.size())  // �����machine�ϵĵ�һ��job
	{
		timeWinResult = time_period(readyTimeForOrder + double2timeDuration(curMach.m_waitTofFirstFurn), processTime); //??����!!!!!!!!!
		flag_NewFurn = true;  // �¿�һ¯
		IndexOfTimeWin = 0;
		isInserted = true;
	}
	else
	{

		IndexOfTimeWin = 0;
		// ����timeline
		for (list<TimeWin>::iterator timeInfo_iter = timeline.begin();
			timeInfo_iter != timeline.end(); ++timeInfo_iter, ++IndexOfTimeWin)
		{
			ptime left_TimeWin = timeInfo_iter->second.begin();
			ptime right_TimeWin = timeInfo_iter->second.last();

			if (left_TimeWin < readyTimeForOrder + processTime)  // �ɼӹ�ʱ���ʱ�䴰��ʼʱ��֮��һ���Ų����µ�һ¯
				continue;

			if (timeline.cbegin() == timeInfo_iter)  // ��һ��time window
			{
				//std::cout << "**************** " << std::endl;
				timeWinResult = time_period(readyTimeForOrder, processTime);
				flag_NewFurn = true;    // �¿�һ¯
				isInserted = true;
				break;
			}
			else
			{
				auto timeInfo_iter2 = timeInfo_iter;
				--timeInfo_iter2;
				ptime right_Pre = timeInfo_iter2->second.end();
				if ((readyTimeForOrder < right_Pre) && (right_Pre + processTime <= left_TimeWin))
				{
					timeWinResult = time_period(right_Pre, processTime);
					flag_NewFurn = true;    // �¿�һ¯
					isInserted = true;
					break;
				}
				if (right_Pre <= readyTimeForOrder)
				{
					timeWinResult = time_period(readyTimeForOrder, processTime);
					flag_NewFurn = true;    // �¿�һ¯
					isInserted = true;
					break;
				}
			}
		}  // END OF ����timeline

		if (false == isInserted)
		{
			//cout << "dsdssdsdsdsdsdsd"<< "flag_NewFurn=" << flag_NewFurn << "isInserted=" << isInserted << endl;
			auto iterTmp = timeline.end();
			ptime rightWin = (--iterTmp)->second.end();
			timeWinResult = time_period((readyTimeForOrder < rightWin) ? rightWin : readyTimeForOrder, processTime);
			flag_NewFurn = true;    // �¿�һ¯
			isInserted = true;
		}
	}
	//std::cout << "curJob=" << curJob.m_jobCode <<" 777777"<< std::endl;
	//std::cout << "      timeWinResult=" << to_iso_extended_string(timeWinResult.begin()) << endl;
	//<<"--" << to_iso_extended_string(timeWinResult.last()) << std::endl;
	return make_pair(IndexOfTimeWin, flag_NewFurn);
};

// ��ĳ������������¯
bool  insertJobToBellFurnWithPreBatch(vector<pair<Job*, int>>& curBatch, ptime readyTimeForOrder, Mach_BellFurnace& curMach)
{

	time_duration& processTime = double2timeDuration(curMach.m_proceTimePerFurn);
	pair<pair<int, int>, time_period> newFurnRes = (make_pair(pair<unsigned, unsigned>(make_pair(-1, -1))
		, time_period(readyTimeForOrder, processTime))); // ��¼�¿�һ¯�����; pair<pair< �ڼ���timeline, ����λ��>, ʱ�䴰>

	unsigned Index_parallelMach = 0;
	pair<int, bool> resultOfPreInsert(make_pair(-1, false));   // pair<TimeWin��λ��, �Ƿ�Ҫ�¿�һ¯>
	for (auto& timeline : curMach.m_timeLines)
	{
		time_period timeWinResult(readyTimeForOrder, processTime);
		resultOfPreInsert = preInsertJobToMachWithPreBatch(readyTimeForOrder, curMach, timeline, timeWinResult);
		if (resultOfPreInsert.second) // ���¿�1¯
		{
			if (newFurnRes.first.first < 0.0)
				newFurnRes = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
			else if (newFurnRes.second.begin() >= timeWinResult.begin())
				newFurnRes = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
		}
		// cout << "  Index_parallelMach=" << Index_parallelMach << endl;
		++Index_parallelMach;
	}

	unsigned Index_final = newFurnRes.first.first;  // �ĸ�ʱ����
	time_period timeWinFinal(newFurnRes.second.begin(), processTime);
	int insertPosition = newFurnRes.first.second;  // ����λ��

	for (auto it = curBatch.begin(); it != curBatch.end(); ++it)
	{
		Job* curJobP = it->first;
		list<TimeWin>& timeLine = curMach.m_timeLines[Index_final];
		unsigned num_reentry = curJobP->m_proceMachs[curJobP->m_allocatedTimeWin.size()].second;

		auto iterTmp = timeLine.begin();
		for (int i = 0; i < insertPosition; ++i)  ++iterTmp;
		if (curBatch.begin() == it)
		{
			map<string, unsigned> jobToInsert{ make_pair(curJobP->m_jobCode, num_reentry) };
			timeLine.insert(iterTmp, make_pair(jobToInsert, timeWinFinal));
		}
		else
			iterTmp->first.insert(make_pair(curJobP->m_jobCode, num_reentry));
		curJobP->m_allocatedTimeWin.push_back(make_pair(make_pair(curMach.m_machCode, num_reentry), timeWinFinal));
		it->second = 2;  // ��־λ����Ϊ"2"����ʾ�Ѿ��Ų���
	}

	return true;
};

// ��ĳ������������¯��ĳ��ƽ�л�,���������¯������
int  insertJob(Job& curJob, Mach_BellFurnace& curMach, vector<vector<pair<Job*, int>>>& batchOfBell, map<string, Mach*>& machsMapLocal)
{
	string gradeOfJob = curJob.m_alloyGrade;
	int batchIndex = 0;
	int batchIndexFinal = -1;
	for (auto it = batchOfBell.begin(); it != batchOfBell.end(); ++it, ++batchIndex)
	{
		vector<pair<Job*, int>>& aBatch = *it;
		if (gradeOfJob == aBatch.begin()->first->m_alloyGrade)  // �ƺ�һ�����Ǹ�����
		{
			for (auto it2 = aBatch.begin(); it2 != aBatch.end(); ++it2)  // ���������Σ�
			{
				if (curJob.m_jobCode == it2->first->m_jobCode) {  //����ڸ�������
					it2->second = 1;  // �Ѹô�������Ϊ��������¯ǰ�ȴ�
					batchIndexFinal = batchIndex;
					break;
				}
			}
		}
		if (batchIndexFinal >= 0) break;
	}

	cout << " batchIndexFinal=" << batchIndexFinal << endl;  // ���Ĳ���ʱ��


	vector<pair<Job*, int>>& curBatch = batchOfBell[batchIndexFinal];  // ���е�ǰjob������
	ptime readyTimeForOrder = (curJob.m_allocatedTimeWin.end() - 1)->second.last();  // ����¯��������翪ʼʱ��
	int isAssigned = 0;
	if (curBatch.size() == 1)  // �����ֻ�����job��ֱ���Ų�
	{
		insertJobToBellFurnWithPreBatch(curBatch, readyTimeForOrder, curMach);
		isAssigned = 1;
	}
	else
	{
		int sumFlag = 0;
		for (auto it2 = curBatch.begin(); it2 != curBatch.end(); ++it2)  // ���������Σ�
			sumFlag += it2->second;
		if (sumFlag == curBatch.size()) // �������У������ڵȴ���ֱ���Ų�
		{
			insertJobToBellFurnWithPreBatch(curBatch, readyTimeForOrder, curMach);
			isAssigned = 1;
		}
	}
	cout << " " << endl;
	if (isAssigned)  // �ź�������
	{
		return batchIndexFinal;
	}
	return -1;
};

// ����¯���Ƿ���Լ�������¯��ĳ��timeWin
bool canAddToTimeWin(Job* curJobP, unsigned machIndexOfJob, TimeWin& timeWin, map<string, Job*> jobsMap)
{
	double totalWidthOfBatch = getCurWidth(curJobP, machIndexOfJob);
	if (Mach_BellFurnace::m_RuleForFurnWithWidth.second.second == timeWin.first.size()) // �Ѿ���3���ˣ����ܲ���
		return false;
	else if (Mach_BellFurnace::m_RuleForFurnWithWidth.second.first == timeWin.first.size())  // �Ѿ���2����
	{
		for (auto& jobInfo : timeWin.first)
		{
			Job* jobP = jobsMap[jobInfo.first];

			double curWidth = getCurWidth(jobP, CodeOfBellFurn, jobInfo.second); //job�ĵ�ǰ���
			totalWidthOfBatch += curWidth;
			if ((totalWidthOfBatch > Mach_BellFurnace::m_RuleForFurnWithWidth.first)  // ���ܳ�����ȷ�Χ
				|| (curJobP->m_jobCode == jobP->m_jobCode))  return false;  // ������job�Լ�
		}
	}
	return true;
}

// Ԥ��������¯���򣬰�ĳ������������¯��ĳƽ�л������ز���ʱ�䴰�Ͳ�������
pair<unsigned, bool> preInsertJobToMach(Job& curJob, Mach_BellFurnace& curMach, unsigned machIndexOfJob, list<TimeWin>& timeline, time_period& timeWinResult, map<string, Job*>& jobsMap)
{
	time_duration& processTime = double2timeDuration(curMach.m_proceTimePerFurn);  //����¯ʱ��Ϊ30Сʱ
	//time_duration timeOfSwith = double2timeDuration(curMach.m_timeOfSwith);  // �����µ�һ¯��ת��ʱ��
	//׼��ʱ�����һ¯�ļӹ����ʱ�䣬�Ƿ�Ҫ����ת��ʱ�䣬��Ҫ�����ף���??
	//   ��job������׼��ʱ�����Ҫת��ʱ�䣻��machine��������һ¯����ʱ�����Ҫת��ʱ�䣡
	unsigned IndexOfTimeWin(0);  // machine��ʱ���߲��������
	// using TimeWin = pair<vector<pair<Job*, unsigned>>, time_period>;  // pair< vector<pair<Job*, ��job�ڼ�������>>, job�����ʱ�䴰>

	bool flag_NewFurn(false);  // ��ʾ�Ƿ��¿�һ¯
	bool isInserted(false);

	ptime readyTimeForOrder;  // job�����ڸû����Ͽ�ʼ�ӹ���ʱ��
	//time_period timeWinToInsert(curJob.m_startDateOfOrder, processTime);  //Ҫ�����ʱ�䴰
	if (0 == machIndexOfJob)  // �����job�ĵ�һ��machine
		readyTimeForOrder = curJob.m_startDateOfOrder;
	else
		readyTimeForOrder = (curJob.m_allocatedTimeWin.end() - 1)->second.end();

	//cout << " timeline.size()=" << timeline.size()
	//	<< "  readyTimeForOrder="<<to_iso_extended_string(readyTimeForOrder)<<endl;

	if (0 == timeline.size())  // �����machine�ϵĵ�һ��job
	{
		timeWinResult = time_period(readyTimeForOrder + double2timeDuration(curMach.m_waitTofFirstFurn), processTime); //??����!!!!!!!!!
		flag_NewFurn = true;  // �¿�һ¯
		IndexOfTimeWin = 0;
		isInserted = true;
	}
	else
	{
		// ����timeline
		for (auto timeInfo_iter = timeline.begin(); timeInfo_iter != timeline.end(); ++timeInfo_iter, ++IndexOfTimeWin)
		{
			ptime left_TimeWin = timeInfo_iter->second.begin();
			ptime right_TimeWin = timeInfo_iter->second.last();
			//cout << "    " <<to_iso_extended_string(left_TimeWin)<<"  IndexOfTimeWin="<< IndexOfTimeWin << endl;

			//cout << "hhhhhhhhh" << endl;
			if (left_TimeWin < readyTimeForOrder + processTime)  // �ɼӹ�ʱ���ʱ�䴰��ʼʱ��֮��һ���Ų����µ�һ¯
			{
				if ((readyTimeForOrder < left_TimeWin) && canAddToTimeWin(&curJob, machIndexOfJob, *timeInfo_iter, jobsMap))
				{
					timeWinResult = timeInfo_iter->second;
					flag_NewFurn = false; // ���¿�һ¯
					isInserted = true;    // ��������¯��
					break;
				}
				continue;
			}

			//cout << "xxxxxxxxxxxxx" << endl;
			if (timeline.cbegin() != timeInfo_iter)  // ���ǵ�һ��time window
			{
				auto timeInfo_iter2 = timeInfo_iter;
				--timeInfo_iter2;
				ptime right_Pre = timeInfo_iter2->second.end();
				if ((readyTimeForOrder < right_Pre) && (canAddToTimeWin(&curJob, machIndexOfJob, *timeInfo_iter, jobsMap))
					&& (readyTimeForOrder < left_TimeWin))
				{
					timeWinResult = timeInfo_iter->second;
					flag_NewFurn = false;    // ���¿�һ¯
					isInserted = true;
					break;
				}
			}
		}  // END OF ����timeline

		//cout << "  isInserted=" << isInserted << endl;

		if (!isInserted)
		{
			IndexOfTimeWin = 0;
			// ����timeline
			for (list<TimeWin>::iterator timeInfo_iter = timeline.begin(); timeInfo_iter != timeline.end(); ++timeInfo_iter, ++IndexOfTimeWin)
			{
				ptime left_TimeWin = timeInfo_iter->second.begin();
				ptime right_TimeWin = timeInfo_iter->second.last();

				if (left_TimeWin < readyTimeForOrder + processTime)  // �ɼӹ�ʱ���ʱ�䴰��ʼʱ��֮��һ���Ų����µ�һ¯
					continue;

				if (timeline.cbegin() == timeInfo_iter)  // ��һ��time window
				{
					//std::cout << "**************** " << std::endl;
					timeWinResult = time_period(readyTimeForOrder, processTime);
					flag_NewFurn = true;    // �¿�һ¯
					isInserted = true;
					break;
				}
				else
				{
					auto timeInfo_iter2 = timeInfo_iter;
					--timeInfo_iter2;
					ptime right_Pre = timeInfo_iter2->second.end();
					if ((readyTimeForOrder < right_Pre) && (right_Pre + processTime <= left_TimeWin))
					{
						timeWinResult = time_period(right_Pre, processTime);
						flag_NewFurn = true;    // �¿�һ¯
						isInserted = true;
						break;
					}
					if (right_Pre <= readyTimeForOrder)
					{
						timeWinResult = time_period(readyTimeForOrder, processTime);
						flag_NewFurn = true;    // �¿�һ¯
						isInserted = true;
						break;
					}
				}
			}  // END OF ����timeline
		}

		if (false == isInserted)
		{
			//cout << "dsdssdsdsdsdsdsd"<< "flag_NewFurn=" << flag_NewFurn << "isInserted=" << isInserted << endl;
			auto iterTmp = timeline.end();
			ptime rightWin = (--iterTmp)->second.end();
			timeWinResult = time_period((readyTimeForOrder < rightWin) ? rightWin : readyTimeForOrder, processTime);
			flag_NewFurn = true;    // �¿�һ¯
			isInserted = true;
		}
	}
	//std::cout << "curJob=" << curJob.m_jobCode <<" 777777"<< std::endl;
	//std::cout << "      timeWinResult=" << to_iso_extended_string(timeWinResult.begin()) << endl;
	//<<"--" << to_iso_extended_string(timeWinResult.last()) << std::endl;
	return make_pair(IndexOfTimeWin, flag_NewFurn);
};

// ��������¯������Ƿ���Ҫ����л�������ǰ�󹤼����ƺ��Ƿ���ͬ���жϣ�airFurnaceSet
bool getIsSwitch(Job& curJob, Mach_AirFurnace& curMach, Job& otherJob)  // �Ƿ���Ҫ�л�
{
	if (airFurnaceSet.find(curMach.m_machCode) != airFurnaceSet.end())  // ������¯��Ļ���
	{
		//cout << otherJob.m_jobCode <<" "<< curJob.m_jobCode
			//<<" otherJob.m_alloyType=" << otherJob.m_alloyType<<"curJob.m_alloyType"<< curJob.m_alloyType << endl;
		if (otherJob.m_alloyGrade == curJob.m_alloyGrade) return false;
		else return true;
	}
	return false;
};

// ���ڷ�����¯����������������Ƿ���Ҫ����л�������ǰ�󹤼����ƺ��Ƿ���ͬ���жϣ�
bool getIsSwitch(Job& curJob, Mach& curMach, Job& otherJob)  // �Ƿ���Ҫ�л�
{
	// ������ ����������ã���ϴ �Ļ��ж���Ҫ�л�ʱ��
	if(rollingMachFrequ.find(curMach.m_machCode)!= rollingMachFrequ.end()       // ������
		|| stretchMachFrequ.find(curMach.m_machCode) != stretchMachFrequ.end()  // �����
		|| washMachFrequ.find(curMach.m_machCode) != washMachFrequ.end())       // ��ϴ����
	{
		//cout << otherJob.m_jobCode <<" "<< curJob.m_jobCode
			//<<" otherJob.m_alloyType=" << otherJob.m_alloyType<<"curJob.m_alloyType"<< curJob.m_alloyType << endl;
		if (otherJob.m_alloyGrade == curJob.m_alloyGrade) return false;
		else return true;
	}
	return false;
};


// --------END OF--�Ų�����ĳ��������ĳ�����Ŀ���ʱ��--------




// --------��ʼ����أ���ʼ��Job��Machine--------

//  ����--�����õĳ�ʼ��
void myInitialization(vector<string>& jobsCodeVec, vector<string>& machsCodeVec, map<string, Job*>& jobsMap, map<string, Mach*>& machsMap)
{
	jobsCodeVec = vector<string>();
	machsCodeVec = vector<string>();
	jobsMap = map<string, Job*>();
	machsMap = map<string, Mach*>();

	string machCode = "C101";
	machsCodeVec.push_back(machCode);
	Mach* machIter = new Mach;
	machsMap.insert(make_pair(machCode, machIter));
	machIter->m_machCode = machCode;

	machCode = "C102";
	machsCodeVec.push_back(machCode);
	machIter = new Mach;
	machsMap.insert(make_pair(machCode, machIter));
	machIter->m_machCode = machCode;

	machCode = "C103";
	machsCodeVec.push_back(machCode);
	machIter = new Mach;
	machsMap.insert(make_pair(machCode, machIter));
	machIter->m_machCode = machCode;


	string jobCode = "j1";
	jobsCodeVec.push_back(jobCode);
	Job* jobIter = new Job;
	jobsMap.insert(make_pair(jobCode, jobIter));
	jobIter->m_jobCode = jobCode;
	jobIter->m_startDateOfOrder = time_from_string("2020-5-1 00:00:00");
	jobIter->m_dueDateOfOrder = time_from_string("2020-5-1 20:00:00");
	jobIter->m_proceMachs = vector<pair<string, unsigned>>({ make_pair("C101",1), make_pair("C102",1) });
	jobIter->m_proceTargets = vector<pair<pair<string, unsigned>, ProcessTargets>>({ make_pair(make_pair("C101",1), ProcessTargets()),
		make_pair(make_pair("C101",1), ProcessTargets()) });

	jobCode = "j2";
	jobsCodeVec.push_back(jobCode);
	jobIter = new Job;
	jobsMap.insert(make_pair(jobCode, jobIter));
	jobIter->m_jobCode = jobCode;
	jobIter->m_startDateOfOrder = time_from_string("2020-5-1 00:00:00");
	jobIter->m_dueDateOfOrder = time_from_string("2020-5-1 18:00:00");
	jobIter->m_proceMachs = vector<pair<string, unsigned>>({ make_pair("C102",1), make_pair("C103",1) });
	jobIter->m_proceTargets = vector<pair<pair<string, unsigned>, ProcessTargets>>({ make_pair(make_pair("C102",1), ProcessTargets()),
		make_pair(make_pair("C103",1), ProcessTargets()) });

	jobCode = "j3";
	jobsCodeVec.push_back(jobCode);
	jobIter = new Job;
	jobsMap.insert(make_pair(jobCode, jobIter));
	jobIter->m_jobCode = jobCode;
	jobIter->m_startDateOfOrder = time_from_string("2020-5-1 00:00:00");
	jobIter->m_dueDateOfOrder = time_from_string("2020-5-1 21:00:00");
	jobIter->m_proceMachs = vector<pair<string, unsigned>>({ make_pair("C101",1), make_pair("C102",1), make_pair("C103",1) });
	jobIter->m_proceTargets = vector<pair<pair<string, unsigned>, ProcessTargets>>({ make_pair(make_pair("C101",1), ProcessTargets()),
		make_pair(make_pair("C102",1), ProcessTargets()),
		make_pair(make_pair("C103",1), ProcessTargets()) });

};

// ��ʼ�����ܡ����������ݿ����Ľ����ʼ�����������Ϣ
void initializeCaps(MYSQL_RES* res, vector<string>& machsCodeVec, map<string, Mach*>& machsMap)
{
	MYSQL_ROW row;  //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾһ������
	MYSQL_FIELD* field = NULL;

	vector<string> fieldsNames_orders;
	//�ֶεĸ���
	int fieldcount = mysql_num_fields(res);
	std::cout << "Number of fields: " << fieldcount << std::endl;
	//��ʾ�����ֶ���
	for (int i = 0; i < fieldcount; ++i)
	{
		field = mysql_fetch_field_direct(res, i);
		std::cout << field->name << "\t";
		fieldsNames_orders.push_back(field->name);
	}
	std::cout << std::endl;
	int i = 0;  // ����
	while (row = mysql_fetch_row(res))
	{
		// unit_id, width, thick, alloy_name, status, process_time, capacity, explanation
		// �ظ���ȡ�У��������ȡÿ�����ֶε�ֵ��ֱ��rowΪNULL

		CapWithConditions cap;

		cap.m_width = (NULL == row[1]) ? "NULL" : row[1];
		cap.m_thick = (NULL == row[2]) ? "NULL" : row[2];
		cap.m_alloyType = (NULL == row[3]) ? "NULL" : row[3];;
		cap.m_status = (NULL == row[4]) ? "NULL" : row[4];;
		cap.m_processT = (NULL == row[5]) ? "NULL" : row[5];;
		cap.m_cap = (NULL == row[6]) ? "NULL" : row[6];;
		string machCode = (NULL == row[0]) ? "NULL" : row[0];;
		Mach* machP = machsMap[machCode];

		machP->m_capsOriginalInfo.push_back(cap);

		/*
		std::cout << "row[0] = " << row[0] << std::endl;
		std::cout << "row[1] = " << row[1] << std::endl;
		std::cout << "row[2] = " << row[2] << std::endl;
		std::cout << "row[3] = " << row[3] << std::endl;
		std::cout << "row[4] = " << row[4] << std::endl;
		std::cout << "row[5] = " << row[5] << std::endl;
		std::cout << "row[6] = " << row[6] << std::endl;
		*/
		++i;

	}
	std::cout << "There are " << i << " notes in total." << std::endl;
};

// ���빤����Ϣ�����������ݿ����Ľ����ʼ��������Ϣ
void initializeJobs(MYSQL_RES* res, vector<string>& jobsCodeVec, map<string, Job*>& jobsMap)
{
	MYSQL_ROW row;  //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾһ������
	MYSQL_FIELD* field = NULL;

	vector<string> fieldsNames_orders;
	//�ֶεĸ���
	int fieldcount = mysql_num_fields(res);
	std::cout << "Number of fields: " << fieldcount << std::endl;
	//��ʾ�����ֶ���
	for (int i = 0; i < fieldcount; i++)
	{
		field = mysql_fetch_field_direct(res, i);
		std::cout << field->name << "\t";
		fieldsNames_orders.push_back(field->name);
	}
	std::cout << std::endl;
	int i = 0;  // ����
	while (row = mysql_fetch_row(res))
	{
		// strip_id, deadline, thick, width, note, length, weight, inner_diameter, alloy_grade
		// �ظ���ȡ�У��������ȡÿ�����ֶε�ֵ��ֱ��rowΪNULL
		Job* jobP = new Job;
		jobP->m_jobCode = row[0];
		jobP->m_dueDateOfOrderStr = row[1];
		jobP->m_alloyGrade = row[8];
		jobP->m_alloyType = getTypeFromGrade(jobP->m_alloyGrade);
		jobP->m_initialInfos.m_targetThick = mystod(row[2]);
		jobP->m_initialInfos.m_targetWidth = mystod(row[3]);
		jobP->m_initialInfos.m_targetLength = mystod(row[5]);
		jobP->m_initialInfos.m_targetWeight = mystod(row[6]);
		jobP->m_initialInfos.m_targetInnerDia = mystod(row[7]);
		if (jobP->m_dueDateOfOrderStr<"20200600") continue;
		jobsCodeVec.push_back(jobP->m_jobCode);
		jobsMap[jobP->m_jobCode] = jobP;
		if (i == 99) break;
		++i;
	}
	std::cout << "There are " << i << " jobs in total." << std::endl;
};

// ��ʼ�������������ƹ�����Ϣ���ӹ�Ŀ��ͼӹ�������
void initializeJobs2(MYSQL_RES* res, vector<string>& jobsCodeVec, map<string, Job*>& jobsMap)
{
	MYSQL_ROW row;  //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾһ������
	MYSQL_FIELD* field = NULL;

	vector<string> fieldsNames_orders;
	//�ֶεĸ���
	int fieldcount = mysql_num_fields(res);
	std::cout << "Number of fields: " << fieldcount << std::endl;
	//��ʾ�����ֶ���
	for (int i = 0; i < fieldcount; ++i)
	{
		field = mysql_fetch_field_direct(res, i);
		std::cout << field->name << "\t";
		fieldsNames_orders.push_back(field->name);
	}
	std::cout << std::endl;
	int i = 0;  // ����
	while (row = mysql_fetch_row(res))
	{
		// strip_id, process_num, unit_id, thick, width, note, unit_name_orig
		// �ظ���ȡ�У��������ȡÿ�����ֶε�ֵ��ֱ��rowΪNULL
		string jobCode = row[0];
		Job* jobP;
		if (jobsMap.find(jobCode) == jobsMap.end()) // ���jobCode����jobsMap�У�
		{
			std::cout << "Warning: there are no info in table 'tlorderinformation' for the strip_id of '"
				<< jobCode << "', but '" << jobCode << "' exists in table 'tlprocess'!" << std::endl;
			jobP = new Job;
			jobP->m_jobCode = jobCode;
			//continue;
		}

		else
			jobP = jobsMap[jobCode];
		/*
		if(jobsMap.find(jobCode) == jobsMap.end())
		{
			jobP = new Job;
			jobsCodeVec.push_back(jobCode);
			jobsMap[jobCode] = jobP;
		}
		else
			jobP = jobsMap[jobCode];
		*/
		int orderOfMach = mystoi(row[1]);
		string machCode = row[2];
		unsigned num_reentry = 1;
		for (pair<string, unsigned>& machInfo : jobP->m_proceMachs)
			num_reentry = (machCode == machInfo.first) ? (num_reentry + 1) : num_reentry;
		//std::cout<< jobP->m_jobCode<<", "<< machCode << std::endl;
		jobP->m_proceMachs.push_back(make_pair(machCode, num_reentry));
		//ProcessTargets* processTags= new ProcessTargets(mystod(row[3]), mystod(row[4]));
		//cout<<"���a="<< mystod(row[4]) <<"   ���b="<< mystod(row[3]) <<"\n"<<endl;
		jobP->m_proceTargets.push_back(make_pair(make_pair(machCode, num_reentry), ProcessTargets(mystod(row[4]), mystod(row[3]))));
		++i;
	}
	std::cout << "There are " << i << " notes in total." << std::endl;
};

// ��ʼ�����������������ݿ����Ľ����ʼ��������Ϣ
void initialMachs(MYSQL_RES* res, vector<string>& machsCodeVec, map<string, Mach*>& machsMap)
{
	MYSQL_ROW row;  //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾһ������
	MYSQL_FIELD* field = NULL;

	vector<string> fieldsNames_orders;
	//�ֶεĸ���
	int fieldcount = mysql_num_fields(res);
	std::cout << "Number of fields: " << fieldcount << std::endl;
	//��ʾ�����ֶ���
	for (int i = 0; i < fieldcount; i++)
	{
		field = mysql_fetch_field_direct(res, i);
		std::cout << field->name << "\t";
		fieldsNames_orders.push_back(field->name);
	}
	std::cout << std::endl;
	int i = 0;  // ����
	while (row = mysql_fetch_row(res))
	{
		// unit_id, name
		// �ظ���ȡ�У��������ȡÿ�����ֶε�ֵ��ֱ��rowΪNULL
		//std::cout << "machdCode" << row[0] << std::endl;
		if (CodeOfBellFurn == row[0])  // ������¯
		{
			Mach_BellFurnace* machP = new Mach_BellFurnace();
			machP->m_machCode = row[0];
			machsCodeVec.push_back(machP->m_machCode);
			machsMap[machP->m_machCode] = machP;
		}
		else if (airFurnaceSet.find(string(row[0])) != airFurnaceSet.end()) // ������¯
		{
			Mach_AirFurnace* machP = new Mach_AirFurnace();
			machP->m_machCode = row[0];
			machsCodeVec.push_back(machP->m_machCode);
			machsMap[machP->m_machCode] = machP;
			std::cout << "machCode" << row[0] << std::endl;
			std::cout << "mach time" << machP->m_timeOfSwith << std::endl;

		}
		else
		{
			Mach* machP = new Mach(row[0]);
			machP->m_machCode = row[0];
			machsCodeVec.push_back(machP->m_machCode);
			machsMap[machP->m_machCode] = machP;
			std::cout << "machCode" << row[0] << std::endl;
			std::cout << "mach time" << machP->m_timeOfSwith << std::endl;
		}
		++i;
	}
	std::cout << "There are " << i << " machines in total." << std::endl;
}

// ��ʼ��������Ϣ
void initialMachs2(MYSQL_RES* res, vector<string>& machsCodeVec, map<string, Mach*>& machsMap)
{
	//Mach *mach_bellFurnace = machsMap["BD-S006"];  // ����¯

	MYSQL_ROW row;  //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾһ������
	MYSQL_FIELD* field = NULL;

	vector<string> fieldsNames_orders;
	//�ֶεĸ���
	int fieldcount = mysql_num_fields(res);
	std::cout << "Number of fields: " << fieldcount << std::endl;
	//��ʾ�����ֶ���
	for (int i = 0; i < fieldcount; i++)
	{
		field = mysql_fetch_field_direct(res, i);
		std::cout << field->name << "\t";
		fieldsNames_orders.push_back(field->name);
	}
	std::cout << std::endl;
	int i = 0;  // ����
	while (row = mysql_fetch_row(res))
	{
		// unit_id, name
		// �ظ���ȡ�У��������ȡÿ�����ֶε�ֵ��ֱ��rowΪNULL
		Mach* machP = new Mach(row[0]);
		machP->m_machCode = row[0];
		machsCodeVec.push_back(machP->m_machCode);
		machsMap[machP->m_machCode] = machP;
		++i;
	}
	std::cout << "There are " << i << " machines in total." << std::endl;
}



// �鿴�Ƴ�
void printProcessLine(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap) {
	
	set<vector<pair<string, unsigned>>> processLineSet;  // �Ƴ̼���

	int numoerder = 0;
	map<string, unsigned> machFrequ;  //����Ƶ��
	map<string, unsigned> machCount;  //����ͳ�Ƴ���Ƶ��
	//map<string, string> machFrequ;  // �Ƴ̣�
	for (auto& jobInfo : jobsMap)  // ��������job
	{
		string curjobcode = jobInfo.first;
		Job* curJobP = jobInfo.second;
		numoerder += curJobP->m_proceMachs.size();
		for (auto& oneProcess : curJobP->m_proceMachs) {
			if (machCount.find(oneProcess.first) == machCount.end())
				machCount.insert(make_pair(oneProcess.first, 1));
			if (1 == oneProcess.second)
				machCount[oneProcess.first] += 1;
		}

		if (processLineSet.find(curJobP->m_proceMachs) == processLineSet.end())
		{
			processLineSet.insert(curJobP->m_proceMachs);
			for (auto& oneProcess : curJobP->m_proceMachs)
			{
				if (machFrequ.find(oneProcess.first) == machFrequ.end())
					machFrequ.insert(make_pair(oneProcess.first, 1));
				if (1 == oneProcess.second)
					machFrequ[oneProcess.first] += 1;
			}
		}
	}
	cout << "orderpro=" << numoerder << endl; // order����

	std::cout << "processLineSet.size()=" << processLineSet.size() << std::endl;
	for (auto& process : processLineSet)
	{
		for (auto& oneProcess : process)  std::cout << mapMachCodeToName[oneProcess.first] << " " << oneProcess.second << std::endl;
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for (auto& mach : machFrequ) std::cout << mapMachCodeToName[mach.first] << " " << mach.second << std::endl;

	cout << endl;
	cout << "machCount=" << machCount.size() << endl;
	for (auto& mach : machCount) std::cout << mapMachCodeToName[mach.first] << " " << mach.second << std::endl;

	// ��ӡ
	for (auto& jobInfo : jobsMap)  // ��������job
	{
		string curjobcode = jobInfo.first;
		Job* curJobP = jobInfo.second;
		if (true)
		{
			std::cout << curjobcode << ": " << curJobP->m_dueDateOfOrderStr
				<< " " << curJobP->m_alloyGrade << std::endl;
			std::cout << curjobcode << ": " << to_iso_string(curJobP->m_dueDateOfOrder) << std::endl;
		}

		if (processLineSet.find(curJobP->m_proceMachs) != processLineSet.end())
			processLineSet.insert(curJobP->m_proceMachs);
	}

	cout << "ordernum=" << jobsMap.size() << endl; // order����
}

// ��ȡjob����������ʱ�䣬����ʼ����ͬ��job vector��ÿ������Ĵ���ʱ�䣬�ܼӹ�ʱ�䣬�ɳ�ʱ�䣬��ֹʱ��
void initialJobVecs(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, vector<pair<Job*, ptime>>& jobsWithDueDate
	, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime) 
{
	int i = 0;
	for (auto& jobInfo : jobsMap)  // ��������job��processTime����ȡprocessTime
	{
		cout << "  i="<<i++ <<" jobCode=" << jobInfo.first << endl;
		string curJobCode = jobInfo.first;

		cout << "  pppppppp" << endl;
		Job* curJobP = jobInfo.second;
		jobsWithDueDate.emplace_back(make_pair(curJobP, curJobP->m_dueDateOfOrder));
		time_duration sumOfProcessTime=double2timeDuration(0.0);
		unsigned machIndex(0);

		cout << "  vvvvvvvvv" << endl;
		if (curJobP->m_dueDateOfOrder <= getCurTime() && 0)
		{
			cout << "job '" << curJobP->m_jobCode
				<< "' has already missed its deadline!!! It will no long be scheduled." << endl;
			continue;
		}

		cout << "  xxxxxxxxxx" << endl;
		cout << "  curJobP->m_proceMachs.size()="<< curJobP->m_proceMachs.size() << endl;
		for (pair<string, unsigned>& machInfoOfCurJob : curJobP->m_proceMachs)   // ����ĳ��job������machine
		{
			//std::cout << "machIndex=" << machIndex 
			//std::cout<< curJobP->m_proceTargets.size() << std::endl;

			cout << "machsMap[machInfoOfCurJob.first=" << machInfoOfCurJob.first << endl;
			time_duration processTime = getProcessTime(machsMap[machInfoOfCurJob.first], curJobP, machIndex);
			cout<< "machsMap[machInfoOfCurJob.first=" << machInfoOfCurJob.first << endl;
			//time_duration processTime = curJobP->m_proceTimes[machIndex].second;
			std::cout << "    2processtT=" << to_iso_string(processTime) << std::endl;
			std::cout << "    curJobP->m_proceTimes.size()=" << curJobP->m_proceTimes.size() << std::endl;
			if (curJobP->m_proceTimes.size() > machIndex)
				curJobP->m_proceTimes[machIndex] = make_pair(curJobP->m_proceMachs[machIndex], processTime);
			else
				curJobP->m_proceTimes.insert(curJobP->m_proceTimes.begin() + machIndex,
					make_pair(curJobP->m_proceMachs[machIndex], processTime));

			sumOfProcessTime = sumOfProcessTime + processTime;
			++machIndex;
		}
		std::cout << "  sumOfProcessTime=" << to_iso_string(sumOfProcessTime) << std::endl;

		curJobP->m_totalProceTime = sumOfProcessTime;
		time_duration slackTime = (curJobP->m_dueDateOfOrder - getCurTime()) - sumOfProcessTime;  //job��ʣ��ʱ�䣨��ǰʱ����뽻���ڵ�ʱ�䣩���job����ļӹ�ʱ��֮��
		jobsWithSlackTime.emplace_back(make_pair(curJobP, slackTime));
		jobsWithTotalProTime.emplace_back(make_pair(curJobP, sumOfProcessTime));
	}

	// ���ݽ�ֹʱ�䣬�ܼӹ�ʱ�䣬�ɳ�ʱ������
	sort(jobsWithDueDate.begin(), jobsWithDueDate.end(), myCmpBy_ptime);
	sort(jobsWithTotalProTime.begin(), jobsWithTotalProTime.end(), myCmpBy_time_duration);
	sort(jobsWithSlackTime.begin(), jobsWithSlackTime.end(), myCmpBy_time_duration);

	return;
}

void initialJobsBatch(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, vector<string>& jobsCodeVec
	, map<string, vector<Job*>>& jobsWithBell, map<string, pair<Job*, int>>& jobsWithBells, vector<BellBatch*>& jobsBatch) 
{
	int i = 0;
	string alloyGradePre = jobsMap[*jobsCodeVec.begin()]->m_alloyGrade;
	for (auto& jobInfo : jobsCodeVec)  // ��������job����ȡ������¯��job
	{
		Job* jobP = jobsMap[jobInfo];
		string alloyGrade = jobP->m_alloyGrade;

		cout << "i=" << i << "  job code=" << jobP->m_jobCode << " alloy grade=" << jobP->m_alloyGrade << endl;
		cout << "  due time=" << to_iso_extended_string(jobP->m_dueDateOfOrder) << endl;
		for (pair<string, unsigned>& machInfoOfCurJob : jobP->m_proceMachs)   // ����ĳ��job������machine
		{
			if (CodeOfBellFurn == machInfoOfCurJob.first)
			{
				cout << "    " << CodeOfBellFurn << endl;
				if (alloyGradePre == jobP->m_alloyGrade) 
					jobsWithBell[alloyGradePre].push_back(jobP);
				else
				{
					jobsWithBell.insert(make_pair(jobP->m_alloyGrade, vector<Job*>{ jobP }));
					alloyGradePre = jobP->m_alloyGrade;
				}
				break;
			}
		}
		++i;
	}

	i = 0;
	for (auto& jobInfo : jobsCodeVec)  // ��������job����ȡ�ж������¯��job
	{
		Job* jobP = jobsMap[jobInfo];
		string alloyGrade = jobP->m_alloyGrade;

		cout << "i=" << i << "  job code=" << jobP->m_jobCode << " alloy grade=" << jobP->m_alloyGrade << endl;
		int countBell = 0;
		for (pair<string, unsigned>& machInfoOfCurJob : jobP->m_proceMachs)   // ����ĳ��job������machine
		{
			if (CodeOfBellFurn == machInfoOfCurJob.first)
			{
				++countBell;
				if (2 == countBell)
					jobsWithBells.insert(make_pair(jobP->m_jobCode, make_pair(jobP, countBell)));
				else if (countBell > 2)
					jobsWithBells[jobP->m_jobCode] = make_pair(jobP, countBell);
			}
		}
		++i;
	}

	for (auto& jobInfo : jobsWithBell)  // ��ӡ��������¯��job
	{
		cout << " _alloyGrade=" << jobInfo.first << endl;
		for (auto& jobP : jobInfo.second)
		{
			cout << " jobCodee=" << jobP->m_jobCode << "  due time=" << to_iso_extended_string(jobP->m_dueDateOfOrder);
			if (jobsWithBells.find(jobP->m_jobCode) != jobsWithBells.end())
				cout << "      countBell=" << jobsWithBells[jobP->m_jobCode].second;
			cout << endl;
		}
	}



	// ��ʼ��jobsBatch
	string alloyGradePrev = "";
	BellBatch* pBellBatch;
	for (auto& jobInfo : jobsWithBell)  // ��������job
	{
		if (alloyGradePrev != jobInfo.first) {
			pBellBatch = new BellBatch;
			alloyGradePrev = jobInfo.first;
			pBellBatch->m_strAlloyGrade = jobInfo.first;
			jobsBatch.push_back(pBellBatch);
		}
		else {
			if ("" == alloyGradePrev)
				pBellBatch = new BellBatch;
			else
				pBellBatch = new BellBatch;
		}

		cout << " _alloyGrade=" << jobInfo.first << endl;
		for (auto& jobP : jobInfo.second)
		{
			vector<pair<Job*, double>>& jobsWithWidth = pBellBatch->m_jobsWithWidth;
			if (1 >= jobsWithWidth.size())
			{
				jobsWithWidth.push_back(make_pair(jobP, jobP->m_initialInfos.m_targetWidth));
				if ((2 == jobsWithWidth.size()) &&
					((jobsWithWidth.begin()->second > 850) || (jobP->m_initialInfos.m_targetWidth > 850)))
				{
					pBellBatch->isFull = true;
					pBellBatch->m_strAlloyGrade = jobInfo.first;
				}

			}
			else if (((pBellBatch->isFull))
				|| (((2 == jobsWithWidth.size()) && (jobP->m_initialInfos.m_targetWidth) > 850)))
			{

				pBellBatch = new BellBatch;
				jobsBatch.push_back(pBellBatch);
				pBellBatch->m_jobsWithWidth.push_back(make_pair(jobP, jobP->m_initialInfos.m_targetWidth));
				pBellBatch->m_strAlloyGrade = jobInfo.first;
			}
			else if ((2 == jobsWithWidth.size()) && (jobP->m_initialInfos.m_targetWidth < 850))
			{
				jobsWithWidth.push_back(make_pair(jobP, jobP->m_initialInfos.m_targetWidth));
				pBellBatch->isFull = true;
			}


		}
	}

	for (auto& pBatch : jobsBatch)  // ��������job
	{
		cout << "  pBatch=" << pBatch->m_strAlloyGrade << endl;
		for (auto& jobInfo : pBatch->m_jobsWithWidth) {
			cout << "    joobcode" << jobInfo.first->m_jobCode << " width=" << jobInfo.second << endl;
		}
	}
}

// Ϊjob��machԤ�ȷ���ʱ�䴰�ռ�
void allocateTimeWinSpace(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap) {
	//map<string, int> numOfTimeWin;  // ʱ�䴰�ĸ���

	for (auto& jobInfo : jobsMap)  // ��������job
	{
		string curJobCode = jobInfo.first;
		Job* curJobP = jobInfo.second;
		/*
		for (pair<string, unsigned>& machInfoOfCurJob : curJobP->m_proceMachs)   // ����ĳ��job������machine
		{
			if (numOfTimeWin.find(machInfoOfCurJob.first) == numOfTimeWin.end())
				numOfTimeWin.insert(make_pair(machInfoOfCurJob.first, 1));
			else
				numOfTimeWin[machInfoOfCurJob.first] += 1;
		}
		*/

		g_mutex.lock();
		curJobP->m_allocatedTimeWin.reserve(curJobP->m_proceMachs.size());
		g_mutex.unlock();
	}

	/*
	// ע�⣺������ͨ������curMachP->m_allocatedTimeWin��std::list������std::vector����std::listû��reserve
	//       Ӧ�ü�����mutex��ʵ���̰߳�ȫ��
	for (auto& machInfo : machsMap)  // ��������job
	{
		string curMachCode = machInfo.first;
		Mach* curMachP = machInfo.second;
		int numToReserve = 0;
		if (numOfTimeWin.find(curMachCode) != numOfTimeWin.end())
			numToReserve = numOfTimeWin[curMachCode];
		curMachP->m_allocatedTimeWin.reserve(numToReserve);  // curMachP->m_allocatedTimeWin��std::list��û��reserve
	}
	*/

	// ����¯��ʱ�䴰������vector����Ҫ��reserve����Ԥ���ռ�
	for (auto& machInfo : machsMap)
	{
		Mach* machP = machInfo.second;
		if (CodeOfBellFurn == machP->m_machCode)  // ������¯
		{
			Mach_BellFurnace* machPBell = (static_cast<Mach_BellFurnace*> (machP));

			g_mutex.lock();
			machPBell->m_timeLines = vector<list<TimeWin>>(machPBell->m_numOfParallel);
			//machPBell->m_timeLines.reserve(machPBell->m_numOfParallel);
			g_mutex.unlock();
		}
	}
};


// ��ӡ����¯����ͳ�Ƹ���
void printBellCount(map<string, Job*>& jobsMap) {
	map<string, int> bellCountRecord;
	for (auto& jobInfo : jobsMap)  // ��������job
	{
		string curJobCode = jobInfo.first;
		Job* curJobP = jobInfo.second;
		bellCountRecord.insert(make_pair(curJobCode, 0));
		for (pair<string, unsigned>& machInfoOfCurJob : curJobP->m_proceMachs)   // ����ĳ��job������machine
		{
			if (CodeOfBellFurn == machInfoOfCurJob.first)
				bellCountRecord[curJobCode] += 1;
		}
	}
	for (auto& recd : bellCountRecord)  // ��������job
	{
		cout << "jobCode=" << recd.first << " num=" << recd.second << endl;
	}
}

// ��ӡ���ڡ�����ʱ�䡢�ɳ�ʱ��
void printDueProSlackTime(vector<pair<Job*, ptime>>& jobsWithDueDate
	, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime) 
{
	// �鿴ÿ��job���ܼӹ�ʱ��ͽ�ֹ����
	for (int i = 0; i < jobsWithDueDate.size(); ++i)
	{
		Job* curJobP = jobsWithDueDate[i].first;
		if (1)
		{
			string s = to_iso_string(jobsWithTotalProTime[i].second);
			std::cout << "Job code: " << curJobP->m_jobCode << "  DueTime: " << to_iso_extended_string(jobsWithDueDate[i].second) << " hours"
				<< "  TotalProTime: " << s.substr(0, 2) << "h" << s.substr(2, 2) << "m" << s.substr(4, 2) << " s" << std::endl;
			//std::cout << "  " << curJobP->m_alloyGrade << std::endl;
		}
	};
	// �鿴ÿ��������Ҫ�ӹ���ʱ�䣬��������

	for (auto& jobInfo : jobsWithSlackTime)  // ��������job jobsWithDueDate
	{
		Job* curJobP = jobInfo.first;
		if (1)
		{
			std::cout << "Job code: " << curJobP->m_jobCode << "  SlackTime: " << to_iso_string(jobInfo.second)
				<< " hours" << std::endl;
			//std::cout << "Job code: " << curJobP->m_jobCode << "  DueTime: " << to_iso_extended_string(jobInfo.second)
			//<< " hours"<< std::endl;
			//std::cout << "  " << curJobP->m_alloyGrade << std::endl;

		}
	};
	cout << "jobsWithSlackTime.size()=" << jobsWithSlackTime.size() << endl;
}

// --------END OF--��ʼ����أ���ʼ��Job��Machine--------




// --------��ȡĿ�꺯��ֵ���--------

// ��ȡmakespanĿ�꺯��ֵ
double getMakespan(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap)  // get object value
{
	ptime timeOfStart;       // ��ʼ�ų̵�ʱ���
	ptime timeOfCompletion;  // ���й����ӹ����ʱ���
	double makeSpan(0);      // �ӹ����й��������ʱ�䳤�ȣ�Сʱ��
	int i_numOfMach(0);

	for (auto& machInfo : machsMap)  // ��������job
	{
		Mach* machP = machInfo.second;
		if (0 == machP->m_allocatedTimeWin.size())
			continue;

		time_period& fistTimeWin = (machP->m_allocatedTimeWin.begin())->second;
		auto iter = machP->m_allocatedTimeWin.end();
		time_period& lastTimeWin = (--iter)->second;

		if (0 == i_numOfMach)
		{
			timeOfStart = fistTimeWin.begin();
			timeOfCompletion = lastTimeWin.last();
		}
		else
		{
			timeOfCompletion = timeOfCompletion > lastTimeWin.last() ? timeOfCompletion: lastTimeWin.last();
			timeOfStart =  timeOfStart < fistTimeWin.begin() ? timeOfStart: fistTimeWin.begin();
		}
		i_numOfMach++;
		//std::cout << "delay time of "<<curJobP->m_jobCode << " is: " << delayTime<<std::endl;
	}
	

	makeSpan = timeDuration2Double(timeOfCompletion - timeOfStart);
	return  makeSpan;
}

// ��ȡĿ�꺯��ֵ <������ʱ��(Сʱ), �ӹ����й��������ʱ�䳤��(Сʱ)>
pair<double, double> getObjVals(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap)  // get object value
{
	double totalDueTime(0);  // ������ʱ�䣨Сʱ��
	ptime timeOfStart;       // ��ʼ�ų̵�ʱ���
	ptime timeOfCompletion;  // ���й����ӹ����ʱ���
	double makeSpan(0);      // �ӹ����й��������ʱ�䳤�ȣ�Сʱ��
	int i_numOfJob(0);
	for (auto& jobInfo : jobsMap)  // ��������job
	{
		Job* curJobP = jobInfo.second;
		if (0 == curJobP->m_allocatedTimeWin.size())
			continue;
		time_period lastTimeWin = (curJobP->m_allocatedTimeWin.end() - 1)->second;
		ptime dueDate = curJobP->m_dueDateOfOrder;
		double delayTime = timeDuration2Double(lastTimeWin.last() - dueDate);
		delayTime = delayTime >= 0 ? delayTime : 0;
		//cout<<"delayTime="<< delayTime <<endl;
		totalDueTime += delayTime;
		if (0 == i_numOfJob)
		{
			timeOfStart = curJobP->m_startDateOfOrder;
			timeOfCompletion = lastTimeWin.last();
			//cout << "timeOfCompletion_or=" << to_iso_extended_string(timeOfCompletion) << endl;
			//cout << "timeOfStart_or=" << to_iso_extended_string(timeOfStart) << endl;
		}
		else
		{
			if (timeOfCompletion < lastTimeWin.last())
				timeOfCompletion = lastTimeWin.last();
			if ((curJobP->m_startDateOfOrder) < timeOfStart)
				timeOfStart = curJobP->m_startDateOfOrder;
		}
		i_numOfJob++;
		//std::cout << "delay time of "<<curJobP->m_jobCode << " is: " << delayTime<<std::endl;
	}

	//cout<< "timeOfCompletion=" <<to_iso_extended_string(timeOfCompletion)<<endl;
	//cout<< "timeOfStart=" << to_iso_extended_string(timeOfStart) << endl;

	makeSpan = timeDuration2Double(timeOfCompletion - timeOfStart);
	//std::cout << "totalDueTime=��" << totalDueTime << std::endl;
	//std::cout << "makeSpan = " << makeSpan << std::endl;
	//std::cout << std::endl;
	return make_pair(totalDueTime, makeSpan);
};

// ��ȡĿ�꺯��ֵ <������ʱ��(Сʱ), �ӹ����й��������ʱ�䳤��(Сʱ)>������ÿ��job������ʱ��
pair<double, double> getObjValsWithTardiness(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, map<Job*, double>& jobTardiness)  // get object value
{
	double totalDueTime(0);  // ������ʱ�䣨Сʱ��
	ptime timeOfStart;       // ��ʼ�ų̵�ʱ���
	ptime timeOfCompletion;  // ���й����ӹ����ʱ���
	double makeSpan(0);      // �ӹ����й��������ʱ�䳤�ȣ�Сʱ��
	int i_numOfJob(0);

	double totalRuntime(0.0);   // ����ˮʱ��

	for (auto& jobInfo : jobsMap)  // ��������job
	{
		Job* curJobP = jobInfo.second;
		if (0 == curJobP->m_allocatedTimeWin.size())
			continue;
		time_period lastTimeWin = (curJobP->m_allocatedTimeWin.end() - 1)->second;
		ptime dueDate = curJobP->m_dueDateOfOrder;
		double delayTime = timeDuration2Double(lastTimeWin.last() - dueDate);
		delayTime = delayTime >= 0 ? delayTime : 0;
		//cout<<"delayTime="<< delayTime <<endl;
		totalDueTime += delayTime;

		// ������ˮʱ��
		double runingTime = timeDuration2Double(lastTimeWin.last() - (curJobP->m_allocatedTimeWin.begin())->second.begin());
		runingTime = runingTime >= 0 ? runingTime : 0;
		totalRuntime += runingTime;

		if (0 == i_numOfJob)
		{
			timeOfStart = curJobP->m_startDateOfOrder;
			timeOfCompletion = lastTimeWin.last();
			//cout << "timeOfCompletion_or=" << to_iso_extended_string(timeOfCompletion) << endl;
			//cout << "timeOfStart_or=" << to_iso_extended_string(timeOfStart) << endl;
		}
		else
		{
			if (timeOfCompletion < lastTimeWin.last())
				timeOfCompletion = lastTimeWin.last();
			if ((curJobP->m_startDateOfOrder) < timeOfStart)
				timeOfStart = curJobP->m_startDateOfOrder;
		}
		jobTardiness.emplace(make_pair(curJobP, delayTime));
		i_numOfJob++;
		//std::cout << "delay time of "<<curJobP->m_jobCode << " is: " << delayTime<<std::endl;
	}

	// �������л�ʱ��
	double totalSwitchT = 0.0;
	for (auto& machInfo : machsMap) {  // ��������mach
		string machCode = machInfo.first;

		if (CodeOfBellFurn == machCode)  // ������¯
		{
			Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machInfo.second);
			;
		}
		else {  //����
			Mach* curMachP = machInfo.second;
			double switchT = curMachP->m_timeOfSwith;
			if (switchT == 0.0 || curMachP->m_allocatedTimeWin.size() == 0) continue;

			list<pair<pair<string, unsigned>, time_period>>::iterator iter = curMachP->m_allocatedTimeWin.begin();
			list<pair<pair<string, unsigned>, time_period>>::iterator iterNext = iter;
			++iterNext;
			for (; iterNext != curMachP->m_allocatedTimeWin.end(); ++iter, ++iterNext) {
				if (jobsMap[iter->first.first]->m_alloyGrade != jobsMap[iterNext->first.first]->m_alloyGrade)
					totalSwitchT += switchT;
			}
		}

	}

	//cout<< "timeOfCompletion=" <<to_iso_extended_string(timeOfCompletion)<<endl;
	//cout<< "timeOfStart=" << to_iso_extended_string(timeOfStart) << endl;

	makeSpan = timeDuration2Double(timeOfCompletion - timeOfStart);
	//std::cout << "totalDueTime=��" << totalDueTime << std::endl;
	//std::cout << "makeSpan = " << makeSpan << std::endl;
	//std::cout << std::endl;

	std::cout << "      makeSpan = " << makeSpan << "  totalDueTime = " << totalDueTime
		<< "  totalSwitchT = " << totalSwitchT << std::endl;

	//return make_pair(totalDueTime, makeSpan);
	//return make_pair(totalRuntime, makeSpan);
	return make_pair(totalSwitchT, makeSpan);
};

// --------END OF--��ȡĿ�꺯��ֵ���--------




// --------��ⷽ�����--------

// ����������
void rule_Method(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, MYSQL* mysql,
	vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime) 
{
	cout << "Ruler Method..." << endl;
	cout << endl;

	vector<pair<string, Job*>> jobOrder;

	//for (int i = jobsWithDueDate.size() - 1; i >= 0; --i)

	//for (int i = jobsWithTotalProTime.size() -1; i >= 0; --i) 
	//for (int i = 0; i < jobsWithSlackTime.size(); ++i)
	//for (int i =0; i < jobsWithDueDate.size(); ++i)
	for (int i = 0; i < jobsWithTotalProTime.size(); ++i)
	{
		Job* curJobP = jobsWithTotalProTime[i].first;
		//Job* curJobP = jobsWithSlackTime[i].first;
		//Job* curJobP = jobsWithDueDate[i].first;
		//if(curJobP->)
		jobOrder.push_back(make_pair(curJobP->m_jobCode, curJobP));
	}

	cout << "  startSheduling..." << endl;
	pair<double, double> objectVals = scheduleByJobOrder(jobOrder, jobsMap, machsMap, mysql, false);

	//cout << "objectVals(makespan)=" << objectVals.second << ";  due time=" << objectVals.first << endl;
	cout << "totalSwitchT=" << objectVals.first << ";  makeSpan=" << objectVals.second << endl;
}

// NEH�����������㣬jobOrder��Ҫ�ǿյģ�job����Ϣ�������jobsMap�У�
void NEH_MethodCore(vector<pair<string, Job*>>& jobOrder, map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, MYSQL* mysql,
	vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime)
{

	Job* jobP_chosen = (jobsWithTotalProTime.end() - 1)->first;
	//Job* jobP_chosen = jobsWithSlackTime.begin()->first;
	
	Job* curJobP = jobsMap[jobP_chosen->m_jobCode];
	jobOrder.push_back(make_pair(curJobP->m_jobCode, curJobP));

	//for (unsigned i = 2; i < 30; ++i)  // һ��һ���ط���job
	//unsigned i(-1);
	//for (auto& jobInfo: jobsMap)  // һ��һ���ط���job
	//for (int i = 1; i < jobsWithSlackTime.size(); ++i)  // һ��һ���ط���job
	for (int i = jobsWithTotalProTime.size() - 2; i >= 0; --i)
	{
		//Job* curJobP = jobInfo.second;
		//cout << "i=" << i++ << endl;
		//if (0 == i)
		//{
		//	jobOrder.push_back( make_pair(curJobP->m_jobCode, curJobP));
		//	continue;
		//}

		//Job* jobP_chosen = jobsWithSlackTime[i].first;
		Job * jobP_chosen = jobsWithTotalProTime[i].first;
		Job* curJobP = jobsMap[jobP_chosen->m_jobCode];

		cout << "    i=" << i << "  curJobP->m_jobCode=" << curJobP->m_jobCode << endl;

		unsigned bestPosition(0);
		pair<double, double> bestObjectVals;
		for (unsigned j_insertPosition = 0; j_insertPosition <= jobOrder.size(); ++j_insertPosition) // �������в���λ��
		{
			jobOrder.insert(jobOrder.begin() + j_insertPosition, make_pair(curJobP->m_jobCode, curJobP));
			pair<double, double> objectVals = scheduleByJobOrder(jobOrder, jobsMap, machsMap, mysql, false);

			if (0 == j_insertPosition) 
				bestObjectVals = objectVals;
			//else if (objectVals.first < bestObjectVals.first)
			else if ((objectVals.second) < (bestObjectVals.second))
			{
				bestPosition = j_insertPosition;
				bestObjectVals = objectVals;
			}
			resetJobsToOrigin(jobOrder);      // ��jobOrderTemp�е��Ų�ʱ�䴰��0
			resetMachsMapToOrigin(machsMap);  // ��machsMapTemp�е��Ų�ʱ�䴰��0
			jobOrder.erase(jobOrder.begin() + j_insertPosition);
		}
		cout << "            NEH-Running-bestObjectVals(makespan)=" << bestObjectVals.second << ";  due time=" << bestObjectVals.first << endl;

		jobOrder.insert(jobOrder.begin() + bestPosition, make_pair(curJobP->m_jobCode, curJobP));
	}
	return;
};

// NEH����
void NEH_Method(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, MYSQL* mysql, 
	vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime)
{
	cout <<"NEH Method..." << endl;
	cout << endl;
	Sleep(2000);

	vector<pair<string, Job*>> jobOrder;
	jobOrder.reserve(jobsWithDueDate.size());
	NEH_MethodCore(jobOrder, jobsMap, machsMap,  mysql, jobsWithDueDate, jobsWithTotalProTime, jobsWithSlackTime);

	pair<double, double> objectVals = scheduleByJobOrder(jobOrder, jobsMap, machsMap, mysql, false);

	cout << "  NEH--objectVals(makespan)=" << objectVals.second << ";  due time=" << objectVals.first << endl;
}

// GA���Ŵ�������
void GA_Method(map<string, Mach*>& machsMap, MYSQL* mysql, 
	vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime)
{
	cout << "GA Method..." << endl; cout << endl;
	Sleep(2000);

	vector<pair<string, Job*>> jobOrder;
	jobOrder.reserve(jobsWithDueDate.size());

	// for (int i = jobsWithTotalProTime.size() -1; i >= 0; --i) 
	for (int i = 0; i < jobsWithSlackTime.size(); ++i)
	//for (int i = 0; i < jobsWithDueDate.size(); ++i)
	//for (int i = 0; i < jobsWithTotalProTime.size(); ++i)
	{
		//Job* curJobP = jobsWithTotalProTime[i].first;
		Job* curJobP = jobsWithSlackTime[i].first;
		//Job* curJobP = jobsWithDueDate[i].first;
		jobOrder.emplace_back(make_pair(curJobP->m_jobCode, curJobP));
	}

	// �������ɳ�ʼ��������jobOrderInit��machsMapInit
	vector<pair<string, Job*>> jobOrderInit(jobOrder.size());
	map<string, Job*> jobsMapInit;
	map<string, Mach*> machsMapInit;
	initJobsTemp(jobsMapInit, jobOrderInit, jobOrder);
	initMachsMapTemp(machsMapInit, machsMap);

	// 1����jobOrderBef��Job˳��������Ⱦɫ����룻2���洢����ǰ�ĳ�ʼ��Job��Mach��Ϣ
	vector<pair<string, Job*>> jobOrderBef(jobOrder.size());
	map<string, Job*> jobsMapBef;
	map<string, Mach*> machsMapBef;
	initJobsTemp(jobsMapBef, jobOrderBef, jobOrder);
	initMachsMapTemp(machsMapBef, machsMap);

	// ��jobOrderBef��ȡGAȾɫ�������Ϣ������encodeInfoOfGA
	map<string, pair<int, pair<int, int>>> encodeInfoOfGA;                    // GA������Ϣ����jobOrderBef�е�Job˳�����
	int totalLenOfChromCode = getCodeInfoOfGA_All(jobOrderBef, encodeInfoOfGA);  // GA����ĳ���


	


	// ʹ��NEH����������ȡ��ʼ����job Order
	jobOrderInit.clear();
	NEH_MethodCore(jobOrderInit, jobsMapInit, machsMapInit, mysql, jobsWithDueDate, jobsWithTotalProTime, jobsWithSlackTime);
	// ��ȡ��ʼ��������ȡ��ʼ������
	pair<double, double> objectVals = scheduleByJobOrder(jobOrderInit, jobsMapInit, machsMapInit, mysql, false);
	cout << "��ʼ���� jobOrderInit.size()=" << jobOrderInit.size() << endl;
	cout << "         bestObjectVals(makespan)=" << objectVals.second << ";  due time=" << objectVals.first << endl;



	/*
	// �ӳ�ʼ�������õ�Ⱦɫ��
	Chromosome* chromPInit = new Chromosome(totalLenOfChromCode, jobOrderInit.size());   // Ⱦɫ��
	Chromosome* chromPInit2 = new Chromosome(totalLenOfChromCode, jobOrderInit.size());  // Ⱦɫ��
	initChromCodesByInitSedul(jobOrderInit, encodeInfoOfGA, totalLenOfChromCode, chromPInit, chromPInit2);   // �ɳ�ʼ�Ų������ȡȾɫ��ı���
	*/



	vector<pair<string, Job*>> jobOrderInit2;
	jobOrderInit2.reserve(jobOrder.size());
	map<string, Job*> jobsMapInit2;
	map<string, Mach*> machsMapInit2;
	iniByJobsWithDueDate(jobsWithDueDate, jobOrderInit2, jobsMapInit2, machsMap, machsMapInit2);
	pair<double, double> objectVals2 = scheduleByJobOrder(jobOrderInit2, jobsMapInit2, machsMapInit2, mysql, false);  // ��ȡ��ʼ��������ȡ��ʼ������

	cout << "��ʼ���� jobOrderInit2.size()=" << jobOrderInit2.size() << endl;
	cout << "         bestObjectVals2(makespan)=" << objectVals2.second << ";  due time=" << objectVals2.first << endl;

	// �ӳ�ʼ�������õ�Ⱦɫ��
	Chromosome* chromPInit = new Chromosome(totalLenOfChromCode, jobOrderInit.size());   // Ⱦɫ��
	Chromosome* chromPInit2 = new Chromosome(totalLenOfChromCode, jobOrderInit.size());  // Ⱦɫ��
	Chromosome* chromPInit3 = new Chromosome(totalLenOfChromCode, jobOrderInit.size());   // Ⱦɫ��
	Chromosome* chromPInit4 = new Chromosome(totalLenOfChromCode, jobOrderInit.size());  // Ⱦɫ��
	initChromCodesByInitSedul2(jobOrderInit, jobOrderInit2, encodeInfoOfGA, totalLenOfChromCode, chromPInit, chromPInit2, chromPInit3, chromPInit4);   // �ɳ�ʼ�Ų������ȡȾɫ��ı���






	/*
	// ��initChromCodesByPreCode��Ԥ���趨�Ĵ���õ�Ⱦɫ��
	Chromosome* chromPInit = new Chromosome(totalLenOfChromCode, jobOrderInit.size());   // Ⱦɫ��
	Chromosome* chromPInit2 = new Chromosome(totalLenOfChromCode, jobOrderInit.size());  // Ⱦɫ��
	initChromCodesByPreCode(totalLenOfChromCode, chromPInit, chromPInit2);   // �ɳ�ʼ�Ų������ȡȾɫ��ı���
	*/




	// GA�ĳ�ʼ��
	GeneticAlgorithm* gaP = new GeneticAlgorithm(jobOrder.size(), 150, 200, totalLenOfChromCode);
	// gaP->initializePopulation(chromPInit, chromPInit2);                            // ��ʼ����Ⱥ��Ⱦɫ�弯�ϣ�
	gaP->initializePopulation2(chromPInit, chromPInit2, chromPInit3, chromPInit4);  // ��ʼ����Ⱥ��Ⱦɫ�弯�ϣ�
	gaP->initializeInfoOfBef(&jobOrderBef, &jobsMapBef, &machsMapBef);              // ��ʼ��GA�����ڶ�Ⱦɫ����루�Ų�����Ϣ

	//���ڲ���
	//getObjValsOfInitialChroms(chromPInit, chromPInit2, gaP);
	//getObjValsOfInitialChroms(chromPInit3, chromPInit4, gaP);
	//char cr; cin >> cr;


	gaP->initObjValsOfGAPop();                                              // ��ʼ����Ⱥ�е�ÿ��Ⱦɫ���Ŀ�꺯��
	gaP->runGA();                                                          	// ����
	//gaP->runNSGA2();                                                          	// ����



	//crossoverOfPop(0.23, numOfPop, totalLenOfChromCode, chromPInit, popPool);
	//mutationOfPop(0.23, gaP->numOfPop, gaP->totalLenOfChromCode, chromPInit, popPool);


	/*
	for (int j = 0; j < gaP->m_popPool[10]->tardinessOfjobs.size(); ++j) {
		cout << j << ":" << gaP->m_popPool[10]->tardinessOfjobs[j] << "   ";
	}
	cout << endl; cout << endl;
	for (int j = 0; j < gaP->m_popPool[10]->code.size(); ++j) {
		cout << gaP->m_popPool[10]->code[j] << " ";
	}

	cout << endl; cout << endl;
	cout << " initialObjVal=" << gaP->m_popPool[10]->objectValues.second << endl;

	gaP->localSearchForCurChrome(gaP->m_popPool[10], gaP, 500);

	cout << endl; cout << endl;
	for (int j = 0; j < gaP->m_popPool[10]->code.size(); ++j) {
		cout << gaP->m_popPool[10]->code[j] << " ";
	}
	gaP->getNeighborByReverse(gaP->m_popPool[10], totalLenOfChromCode);


	cout << endl;
	cout << " initialObjVal=" << gaP->m_popPool[10]->objectValues.second << endl;
	for (int j = 0; j < gaP->m_popPool[10]->tardinessOfjobs.size(); ++j) {
		cout << j << ":" << gaP->m_popPool[10]->tardinessOfjobs[j] << "   ";
	}
	cout << endl; cout << endl;
	for (int j = 0; j < gaP->m_popPool[10]->code.size(); ++j) {
		cout << gaP->m_popPool[10]->code[j] << " ";
	}
	*/


	for (auto& jobInfo : jobsMapInit) delete jobInfo.second;
	for (auto& machInfo : machsMapInit) delete machInfo.second;

	for (auto& jobInfo : jobsMapBef) delete jobInfo.second;
	for (auto& machInfo : machsMapBef) delete machInfo.second;

};

// IPG���������׶ࣩ����
void IPG_Method(map<string, Mach*>& machsMap, MYSQL* mysql,
	vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime)
{
	cout << "IPG Method..." << endl; cout << endl;
	Sleep(2000);

	vector<pair<string, Job*>> jobOrder;
	jobOrder.reserve(jobsWithDueDate.size());

	// for (int i = jobsWithTotalProTime.size() -1; i >= 0; --i) 
	for (int i = 0; i < jobsWithSlackTime.size(); ++i)
		//for (int i = 0; i < jobsWithDueDate.size(); ++i)
		//for (int i = 0; i < jobsWithTotalProTime.size(); ++i)
	{
		//Job* curJobP = jobsWithTotalProTime[i].first;
		Job* curJobP = jobsWithSlackTime[i].first;
		//Job* curJobP = jobsWithDueDate[i].first;
		jobOrder.emplace_back(make_pair(curJobP->m_jobCode, curJobP));
	}

	// �������ɳ�ʼ��������jobOrderInit��machsMapInit
	vector<pair<string, Job*>> jobOrderInit(jobOrder.size());
	map<string, Job*> jobsMapInit;
	map<string, Mach*> machsMapInit;
	initJobsTemp(jobsMapInit, jobOrderInit, jobOrder);
	initMachsMapTemp(machsMapInit, machsMap);

	// 1����jobOrderBef��Job˳��������Ⱦɫ����룻2���洢����ǰ�ĳ�ʼ��Job��Mach��Ϣ
	vector<pair<string, Job*>> jobOrderBef(jobOrder.size());
	map<string, Job*> jobsMapBef;
	map<string, Mach*> machsMapBef;
	initJobsTemp(jobsMapBef, jobOrderBef, jobOrder);
	initMachsMapTemp(machsMapBef, machsMap);

	// ��jobOrderBef��ȡGAȾɫ�������Ϣ������encodeInfoOfGA
	map<string, pair<int, pair<int, int>>> encodeInfoOfGA;                    // GA������Ϣ����jobOrderBef�е�Job˳�����
	int totalLenOfChromCode = getCodeInfoOfGA_All(jobOrderBef, encodeInfoOfGA);  // GA����ĳ���
	cout << "ssssssssssssss" << endl;

	/*
	// ʹ��NEH����������ȡ��ʼ����job Order
	jobOrderInit.clear();
	NEH_MethodCore(jobOrderInit, jobsMapInit, machsMapInit, mysql, jobsWithDueDate, jobsWithTotalProTime, jobsWithSlackTime);
	// ��ȡ��ʼ��������ȡ��ʼ������
	pair<double, double> objectVals = scheduleByJobOrder(jobOrderInit, jobsMapInit, machsMapInit, mysql, false);
	cout << "��ʼ���� jobOrderInit.size()=" << jobOrderInit.size() << endl;
	cout << "         bestObjectVals(makespan)=" << objectVals.second << ";  due time=" << objectVals.first << endl;

	// �ӳ�ʼ�������õ�Ⱦɫ��
	Chromosome* chromPInit = new Chromosome(totalLenOfChromCode, jobOrderInit.size());   // Ⱦɫ��
	Chromosome* chromPInit2 = new Chromosome(totalLenOfChromCode, jobOrderInit.size());  // Ⱦɫ��
	*/


	// ��initChromCodesByPreCode��Ԥ���趨�Ĵ���õ�Ⱦɫ��
	Chromosome* chromPInit = new Chromosome(totalLenOfChromCode, jobOrderInit.size());   // Ⱦɫ��
	Chromosome* chromPInit2 = new Chromosome(totalLenOfChromCode, jobOrderInit.size());  // Ⱦɫ��

	EDCode* edCodeP = new EDCode(totalLenOfChromCode, &jobOrderBef, &jobsMapBef, &machsMapBef);
	//edCodeP->initChromCodesByInitSedul(jobOrderInit, encodeInfoOfGA, totalLenOfChromCode, chromPInit, chromPInit2);
	edCodeP->initChromCodesByPreCode(chromPInit, chromPInit2);

	cout << "ttttttttttttttt" << endl;
	cout<< totalLenOfChromCode <<endl;

	IPG* ipgP = new IPG(edCodeP);
	list<Chromosome*> popPool;
	//ipgP->randomlyCreateInitialSolution_test(popPool);

	char c; cin >> c;

	ipgP->randomlyCreateInitialSolution(100, chromPInit, chromPInit2);
	popPool = ipgP->popPool;
	cout << "  codelen=" << ipgP->m_totalLenOfChromCode<< endl;



	cout << "Non_pareto set:" << endl;
	int j = 0;
	for (auto iter = popPool.begin(); iter != popPool.end(); ++iter) {
		Chromosome* chromP = *iter;
		cout << "  chrom " << j++ << ": obj1 = " << chromP->objectValues.first << "  obj2 = " << chromP->objectValues.second << endl;
	}
	cout << "end" << endl;


	list<Chromosome*> popPool2;
	Chromosome* ch1 = new Chromosome(totalLenOfChromCode, jobOrderInit.size());
	ch1->objectValues = make_pair(9.7, 25.0);
	popPool2.emplace_back(ch1);

	Chromosome* ch2 = new Chromosome(totalLenOfChromCode, jobOrderInit.size());
	ch2->objectValues = make_pair(9.8, 25.3);
	popPool2.emplace_back(ch2);

	Chromosome* ch3 = new Chromosome(totalLenOfChromCode, jobOrderInit.size());
	ch3->objectValues = make_pair(9.9, 25.5);
	popPool2.emplace_back(ch3);

	Chromosome* ch4 = new Chromosome(totalLenOfChromCode, jobOrderInit.size());
	ch4->objectValues = make_pair(10, 25.1);
	popPool2.emplace_back(ch4);

	/*
	Chromosome* ch5 = new Chromosome(totalLenOfChromCode, jobOrderInit.size());
	ch5->objectValues = make_pair(10.4, 28);
	popPool2.emplace_back(ch5);

	Chromosome* ch6 = new Chromosome(totalLenOfChromCode, jobOrderInit.size());
	ch6->objectValues = make_pair(10.4, 27.1);
	popPool2.emplace_back(ch6);

	Chromosome* ch7 = new Chromosome(totalLenOfChromCode, jobOrderInit.size());
	ch7->objectValues = make_pair(12, 27);
	popPool2.emplace_back(ch7);

	Chromosome* ch8 = new Chromosome(totalLenOfChromCode, jobOrderInit.size());
	ch8->objectValues = make_pair(11.7, 28.2);
	popPool2.emplace_back(ch8);

	Chromosome* ch9 = new Chromosome(totalLenOfChromCode, jobOrderInit.size());
	ch9->objectValues = make_pair(11, 26.5);
	popPool2.emplace_back(ch9);

	Chromosome* ch10 = new Chromosome(totalLenOfChromCode, jobOrderInit.size());
	ch10->objectValues = make_pair(13, 26.7);
	popPool2.emplace_back(ch10);
	popPool2;
	*/

	vector<set<Chromosome*>> paretoPlanes;
	fastNondominationSort(popPool2, paretoPlanes);

	vector<pair<Chromosome*, double>> crowdingDistances;
	getDensityEstimation(paretoPlanes[0], crowdingDistances);




	char c2; cin >> c2;


	ipgP->getParetoSet(ipgP->popPool);


	ipgP->run(50);

};



// --------END OF--��ⷽ�����--------




// --------GA��ȡ��ʼ�����--------

// �������ŵĿ�ʼ�ӹ�ʱ�䣬��job��������
bool compareJobPPtime(pair<pair<Job*, int>, ptime> left, pair<pair<Job*, int>, ptime> right) {
	return right.second > left.second;
};

// ��jobOrder˳���Ų�����ȡ��ʼ�Ų����
pair<double, double> scheduleForGAByJobOrder(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, MYSQL* mysql, bool isPrint) {
	//if (jobOrder.size() >= 79)std::cout << "curJobsize: " << jobOrder.size() << std::endl;

	Job* curJobP;
	for (int i_job = 0; i_job < jobOrder.size(); ++i_job)
	{
		curJobP = jobOrder[i_job].second;

		cout << "curJobPcode = " << curJobP << endl;

		unsigned machIndex(0);  // job�� machine
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // ����ĳ��job������machine
		{
			bool isSuccess;
			if (CodeOfBellFurn == machCodeOfCurJob.first)  // ������¯
			{
				Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // ������¯���еĻ���
			{
				Mach_AirFurnace* curMachP = static_cast<Mach_AirFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else {  //����
				Mach* curMachP = machsMapTemp[machCodeOfCurJob.first];
				//isSuccess = insertJob(*curJobP, *curMachP, machIndex);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			if (isSuccess) curJobP->m_curMachIndex = machIndex + 1;

			//if ("0118741020-0-0"==curJobP->m_jobCode)
			//std::cout << "curJobP.code"<< curJobP->m_jobCode <<"curMach: " << machCodeOfCurJob.first
			//	<< "isSuccess: " << isSuccess << std::endl;
			//if (jobOrder.size() >= 80 && ("0118741020-0-0" == curJobP->m_jobCode)) std::cout << "curMach: " << machCodeOfCurJob.first << "qyqyqyqyqyqyqy " << machIndex << endl;
			++machIndex;
		}
	}

	// ��ӡ�����ų̽��
	if (isPrint) printFinalRes(jobsMapTemp, machsMapTemp);
	// �����CSV�ļ�
	//if (isPrint) writeToCSV(jobsMapTemp, machsMapTemp);
	//if (isPrint) writeToCSV(jobsMapTemp, machsMapTemp, mysql);


	// ��ȡĿ��ֵ
	pair<double, double> objectVals = getObjVals(jobsMapTemp, machsMapTemp);
	//std::cout << "������ʱ��(Сʱ): " << objectVals.first << "; �ӹ����й��������ʱ�䳤��(Сʱ): " << objectVals.second << std::endl;

	//for (auto& jobInfo : jobsMapTemp) delete jobInfo.second;
	//for (auto& machInfo : machsMapTemp) delete machInfo.second;

	return objectVals;
};

// ʹ��NEH������ȡ��ʼ������
pair<double, double> scheduleForGAByNEH(vector<pair<string, Job*>>& jobOrder, map<string, Mach*>& machsMap, MYSQL* mysql,
	vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime)
{
	cout << "  By NEH Method..." << endl;
	cout << endl;
	Sleep(2000);

	for (auto& ele:jobOrder) {
		Job* JobP = ele.second;
		delete JobP;
	}
	jobOrder.clear();

	jobOrder.push_back(make_pair((jobsWithTotalProTime.end() - 1)->first->m_jobCode, (jobsWithTotalProTime.end() - 1)->first));
	//jobOrder.push_back(make_pair((jobsWithSlackTime.begin()->first)->m_jobCode, jobsWithSlackTime.begin()->first));

	//for (unsigned i = 2; i < 30; ++i)  // һ��һ���ط���job
	//unsigned i(-1);
	//for (auto& jobInfo: jobsMap)  // һ��һ���ط���job
	//for (int i = 1; i < jobsWithSlackTime.size(); ++i)  // һ��һ���ط���job
	for (int i = jobsWithTotalProTime.size() - 2; i >= 0; --i)
	{

		//Job* curJobP = jobInfo.second;
		//cout << "i=" << i++ << endl;
		//if (0 == i)
		//{
		//	jobOrder.push_back( make_pair(curJobP->m_jobCode, curJobP));
		//	continue;
		//}

		cout << "i=" << i << endl;
		//Job* curJobP = jobsWithSlackTime[i].first;
		Job* curJobP = jobsWithTotalProTime[i].first;
		cout << "curJobP->m_jobCode=" << curJobP->m_jobCode << "  i=" << i << endl;

		unsigned bestPosition(0);
		pair<double, double> bestObjectVals;
		//cout<<"=========jobnum="<< jobOrder.size() <<endl;
		for (unsigned j_insertPosition = 0; j_insertPosition <= jobOrder.size(); ++j_insertPosition) // �������в���λ��
		{

			vector<pair<string, Job*>> jobOrderTemp;
			map<string, Job*> jobsMapInit;
			map<string, Mach*> machsMapTemp;
			//if (j_insertPosition >= 80 ) cout << "insertPostion=" << j_insertPosition << endl;

			initJobsTemp(jobsMapInit, jobOrderTemp, jobOrder);
			initMachsMapTemp(machsMapTemp, machsMap);


			//if (j_insertPosition >= 80)  cout << "insertPostion="<< j_insertPosition << endl;
			Job* curjobP_copy = new Job;
			*curjobP_copy = *curJobP; //0205782740-0-0
			jobOrderTemp.insert(jobOrderTemp.begin() + j_insertPosition, make_pair(curjobP_copy->m_jobCode, curjobP_copy));
			jobsMapInit.insert(make_pair(curjobP_copy->m_jobCode, curjobP_copy));

			pair<double, double> objectVals = scheduleByJobOrder(jobOrderTemp, jobsMapInit, machsMapTemp, mysql, false);

			if (0 == j_insertPosition) bestObjectVals = objectVals;
			else if ((objectVals.second) < (bestObjectVals.second))  // else if (objectVals.first < bestObjectVals.first)
			{
				bestPosition = j_insertPosition;
				bestObjectVals = objectVals;
			}
		}

		cout << "bestObjectVals(makespan)=" << bestObjectVals.second << ";  due time=" << bestObjectVals.first << endl;
		jobOrder.insert(jobOrder.begin() + bestPosition, make_pair(curJobP->m_jobCode, curJobP));
	}

	pair<double, double> objectVals;

	return objectVals;
};


// �ý���ʱ������ʼ��jobOrderInit2
void iniByJobsWithDueDate(vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<string, Job*>>& jobOrderInit2, map<string, Job*>& jobsMapInit2
	, map<string, Mach*>& machsMap, map<string, Mach*>& machsMapInit2) 
{
	for (int i = 0; i < jobsWithDueDate.size(); ++i) {
		Job* jobP = new Job;
		Job* jobPOrign = jobsWithDueDate[i].first;
		*jobP = *jobPOrign;
		//cout << "  jobP->m_proceMachs.size()=" << jobP->m_proceMachs.size() << endl;
		jobOrderInit2.emplace_back(make_pair(jobP->m_jobCode, jobP));
		jobsMapInit2.emplace(make_pair(jobP->m_jobCode, jobP));
		jobP->m_allocatedTimeWin.reserve(jobP->m_proceMachs.size());  // Ԥ��job�Ĺ������ռ�
	}
	initMachsMapTemp(machsMapInit2, machsMap);

	return;
}



// ��������¯֮ǰ�Ĺ�������¯�м�Ĺ����ţ����һ���������ݺ���ģ������һ��������
pair<double, double> scheduleForGAByJobOrder_BefAir(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, MYSQL* mysql, bool isPrint) {
	//if (jobOrder.size() >= 79)std::cout << "curJobsize: " << jobOrder.size() << std::endl;

	Job* curJobP;
	for (int i_job = 0; i_job < jobOrder.size(); ++i_job)
	{
		curJobP = jobOrder[i_job].second;
		if (i_job == 0) cout << curJobP << endl;
		unsigned machIndex(0);  // job�� machine
		int leftPos = -1;
		int rightPos = -1;
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // ����ĳ��job������machine
		{
			bool isSuccess;
			if (CodeOfBellFurn == machCodeOfCurJob.first)  // ������¯
			{
				Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // ������¯���еĻ���
				break;
			else {  //����
				Mach* curMachP = machsMapTemp[machCodeOfCurJob.first];
				//isSuccess = insertJob(*curJobP, *curMachP, machIndex);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			if (isSuccess) curJobP->m_curMachIndex = machIndex + 1;
			++machIndex;
		}
	}

	// ��ӡ�����ų̽��
	if (isPrint) printFinalRes(jobsMapTemp, machsMapTemp);

	// �����CSV�ļ�
	//if (isPrint) writeToCSV(jobsMapTemp, machsMapTemp);
	//if (isPrint) writeToCSV(jobsMapTemp, machsMapTemp, mysql);


	// ��ȡĿ��ֵ
	pair<double, double> objectVals = getObjVals(jobsMapTemp, machsMapTemp);
	//std::cout << "������ʱ��(Сʱ): " << objectVals.first << "; �ӹ����й��������ʱ�䳤��(Сʱ): " << objectVals.second << std::endl;

	//for (auto& jobInfo : jobsMapTemp) delete jobInfo.second;
	//for (auto& machInfo : machsMapTemp) delete machInfo.second;

	return make_pair(DBL_MAX, DBL_MAX);
	// return objectVals;
};

// ��ȡGA�ı�����Ϣ
void getCodeInfoOfGA_Air(vector<pair<string, Job*>>& jobOrder, vector<pair<Job*, pair<int, int>>>& codeInfoOfGA)
{
	for (int i_job = 0; i_job < jobOrder.size(); ++i_job)
	{
		Job* curJobP = jobOrder[i_job].second;
		if (i_job == 0) cout << curJobP << endl;
		unsigned machIndex(0);  // job�� machine
		int leftPos = -1;
		int rightPos = -1;
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // ����ĳ��job������machine
		{
			if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // ������¯���еĻ���
			{
				cout << "    machCodeOfCurJob.first=" << machCodeOfCurJob.first << endl;
				cout << "  curJobP=" << curJobP->m_jobCode << endl;
				leftPos = machIndex;   rightPos = machIndex;
				for (int i = machIndex + 1; i < curJobP->m_proceMachs.size(); i++) {
					if (airFurnaceSet.find(curJobP->m_proceMachs[i].first) != airFurnaceSet.end()) rightPos = i;
				}
				cout << "leftPos=" << leftPos << endl; cout << "rightPos=" << rightPos << endl;
				break;
			}
			++machIndex;
		}
		codeInfoOfGA[i_job].second = make_pair(leftPos, rightPos);  // numOfEncode==0
	}
};

// ��ȡGA��Ⱦɫ�������Ϣ������jobOrder��˳�����Ϊ0,1,2,3...
int getCodeInfoOfGA_All(vector<pair<string, Job*>>& jobOrder, map<string, pair<int, pair<int, int>>>& encodeInfoOfGA)
{
	for (int i_job = 0; i_job < jobOrder.size(); ++i_job)
	{
		Job* curJobP = jobOrder[i_job].second;
		if (i_job == 0) cout << curJobP << endl;
		unsigned machIndex(0);  // job�� machine
		int leftPos = -1;
		int rightPos = -1;

		cout << "  i=" << i_job << "  jobCode=" << curJobP->m_jobCode<< endl;
		cout<< "    curJobP->m_proceMachs.size()=" << curJobP->m_proceMachs.size() << endl;

		if (curJobP->m_proceMachs.size() > 0) {
			leftPos = 0;
			rightPos = curJobP->m_proceMachs.size()-1;
		}

		encodeInfoOfGA.emplace(make_pair(curJobP->m_jobCode, make_pair(i_job, make_pair(leftPos, rightPos))));
	}

	// ��������ܳ���
	int totalLenOfChromCode = 0;
	for (auto& ele : encodeInfoOfGA) {
		pair<int, int>& codeRange = ele.second.second;
		if (codeRange.first >= 0)
			totalLenOfChromCode += (codeRange.second - codeRange.first + 1);
	}

	cout << "totalLenOfChromCode=" << totalLenOfChromCode << endl;
	cout << "codeInfoOfGA=" << encodeInfoOfGA.size() << endl;

	return totalLenOfChromCode;
};

// --------END OF����GA��ȡ��ʼ�����--------




// --------��������أ����������̨��CSV�ļ�--------

// ��ӡ�����ų̽��
void printFinalRes(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap)
{
	// ��ӡ�ų̽��
	/*
	std::cout << std::endl;
	for (auto& machInfo : machsMap)  // ��������machine
	{
		Mach& curMach = *(machInfo.second);
		std::cout << "MachCode---------" << curMach.m_machCode << std::endl;
		for (auto& timeInfo : curMach.m_allocatedTimeWin)  // ��������time_period
		{
			string jobCode = timeInfo.first.first;
			time_period timeWin = timeInfo.second;
			std::cout << "JobCode  " << jobCode << ": " << to_iso_extended_string(timeWin.begin())
				<< "--" << to_iso_extended_string(timeWin.last()) << std::endl;
		}
		std::cout << std::endl;
	};
	*/

	std::cout << std::endl;
	for (auto& machInfo : machsMap)  // ��������machine
	{
		if (CodeOfBellFurn == machInfo.first)
		{
			Mach_BellFurnace* curMach = static_cast<Mach_BellFurnace*>(machInfo.second);
			std::cout << "MachCode---------" << curMach->m_machCode << std::endl;
			unsigned i_timeline(0);
			for (auto& timeline : curMach->m_timeLines)
			{
				std::cout << "   Number of time line: " << i_timeline << std::endl;
				for (auto& timeWinInfo : timeline)  // ��������time_period
				{
					// using TimeWin = pair<map<Job*, unsigned>, time_period>;  // pair< map<Job*, ��job�ڼ�������>, job�����ʱ�䴰>

					time_period timeWin = timeWinInfo.second;
					std::cout << "      " << to_iso_extended_string(timeWin.begin())
						<< "--" << to_iso_extended_string(timeWin.last()) << std::endl;
					unsigned i_jobs(0);
					for (auto& jobs : timeWinInfo.first)  // ��������job
					{
						Job* jobP = jobsMap[jobs.first];
						std::cout << "                                      Number of jobs this period=" << i_jobs
							<< "  jobCode = " << jobP->m_jobCode << "; reentry = " << jobs.second
							<< "  job_AlloyGrade="<< jobP->m_alloyGrade<< std::endl;;
						++i_jobs;
					}
				}
				++i_timeline;
			}
			std::cout << std::endl;
		}
	};

	std::cout << std::endl;
	for (auto& machInfo : machsMap)  // ��������machine
	{
		if (airFurnaceSet.find(machInfo.first) != airFurnaceSet.end())
		{
			Mach_AirFurnace* curMach = static_cast<Mach_AirFurnace*>(machInfo.second);
			std::cout << "MachCode---------" << curMach->m_machCode << std::endl;
			for (pair<pair<string, unsigned>, time_period>& timeWinInfo : curMach->m_allocatedTimeWin)
			{// vector<pair<pair<string, unsigned>, time_period>> m_allocatedTimeWin;
				string jobCode = timeWinInfo.first.first;
				time_period timeWin = timeWinInfo.second;
				std::cout << "jobCode  " << jobCode << ": " << "reentry=" << timeWinInfo.first.second
					<< " " << to_iso_extended_string(timeWin.begin())
					<< "--" << to_iso_extended_string(timeWin.last())
					<< jobsMap[jobCode]->m_alloyType << std::endl;
			}
			std::cout << std::endl;
		}
	};

	for (auto& machInfo : machsMap)  // ��������machine
	{
		if (airFurnaceSet.find(machInfo.first) == airFurnaceSet.end() && CodeOfBellFurn != machInfo.first)
		{
			Mach_AirFurnace* curMach = static_cast<Mach_AirFurnace*>(machInfo.second);
			std::cout << "MachCode---------" << curMach->m_machCode << std::endl;
			for (pair<pair<string, unsigned>, time_period>& timeWinInfo : curMach->m_allocatedTimeWin)
			{// vector<pair<pair<string, unsigned>, time_period>> m_allocatedTimeWin;
				string jobCode = timeWinInfo.first.first;
				time_period timeWin = timeWinInfo.second;
				std::cout << "jobCode  " << jobCode << ": " << "reentry=" << timeWinInfo.first.second
					<< " " << to_iso_extended_string(timeWin.begin())
					<< "--" << to_iso_extended_string(timeWin.last())
					<< jobsMap[jobCode]->m_alloyType << std::endl;
			}
			std::cout << std::endl;
		}
	};

	std::cout << "\n\n" << std::endl;
	for (auto& jobInfo : jobsMap)  // ��������job
	{
		Job* curJobP = jobInfo.second;
		std::cout << "JobCode---------" << curJobP->m_jobCode << std::endl;
		for (auto& timeInfo : curJobP->m_allocatedTimeWin)  // ��������time_period
		{
			string machCode = timeInfo.first.first;
			time_period timeWin = timeInfo.second;
			std::cout << "MachCode  " << machCode << ": " << to_iso_extended_string(timeWin.begin())
				<< "--" << to_iso_extended_string(timeWin.last()) << std::endl;
		}
		std::cout << std::endl;
	};
}

// ��ӡ�����ų̽��
void printFinalRes2(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap)
{
	// ��ӡ�ų̽��
	/*
	std::cout << std::endl;
	for (auto& machInfo : machsMap)  // ��������machine
	{
		Mach& curMach = *(machInfo.second);
		std::cout << "MachCode---------" << curMach.m_machCode << std::endl;
		for (auto& timeInfo : curMach.m_allocatedTimeWin)  // ��������time_period
		{
			string jobCode = timeInfo.first.first;
			time_period timeWin = timeInfo.second;
			std::cout << "JobCode  " << jobCode << ": " << to_iso_extended_string(timeWin.begin())
				<< "--" << to_iso_extended_string(timeWin.last()) << std::endl;
		}
		std::cout << std::endl;
	};
	*/


	std::cout << std::endl;
	for (auto& machInfo : machsMap)  // ��������machine
	{
		 Mach* curMach = machInfo.second;
		std::cout << "MachCode---------" << curMach->m_machCode << std::endl;
		for (pair<pair<string, unsigned>, time_period>& timeWinInfo : curMach->m_allocatedTimeWin)
		{// vector<pair<pair<string, unsigned>, time_period>> m_allocatedTimeWin;
			string jobCode = timeWinInfo.first.first;
			time_period timeWin = timeWinInfo.second;
			std::cout << "jobCode  " << jobCode << ": " << "reentry=" << timeWinInfo.first.second
				<< " " << to_iso_extended_string(timeWin.begin())
				<< "--" << to_iso_extended_string(timeWin.last())
				<< jobsMap[jobCode]->m_alloyType << std::endl;
		}
		std::cout << std::endl;
	}

	std::cout << "\n\n" << std::endl;
	for (auto& jobInfo : jobsMap)  // ��������job
	{
		Job* curJobP = jobInfo.second;
		std::cout << "JobCode---------" << curJobP->m_jobCode << std::endl;
		for (auto& timeInfo : curJobP->m_allocatedTimeWinPs)  // ��������time_period
		{
			string machCode = timeInfo.first.first;
			std::cout << "MachCode  " << machCode << ": " << to_iso_extended_string(timeInfo.second->begin())
				<< "--" << to_iso_extended_string(timeInfo.second->last()) << std::endl;
		}
		std::cout << std::endl;
	};
}

// ��ӡ�����ų̽��
void writeToCSV(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap)
{
	ofstream opt;
	opt.open("ResultsOfMach_test.csv", ios::out | ios::trunc);
	cout << "Writing to csv..." << endl;
	for (auto& machInfo : machsMap)  // ��������machine
	{
		if (CodeOfBellFurn == machInfo.first) {
			Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machInfo.second);
			opt << curMachP->m_machCode << endl;
			unsigned i_timeline(0);
			for (auto& timeline : curMachP->m_timeLines)
			{
				opt << "ParallelMachine=" << i_timeline << std::endl;
				for (auto& timeWinInfo : timeline)  // ��������time_period
				{
					// using TimeWin = pair<map<Job*, unsigned>, time_period>;  // pair< map<Job*, ��job�ڼ�������>, job�����ʱ�䴰>

					time_period timeWin = timeWinInfo.second;
					double beginT = timeDuration2Double(timeWin.begin() - getCurTime());
					double durationT = timeDuration2Double(timeWin.end() - timeWin.begin());

					opt << "," << beginT << ","
						<< durationT << std::endl;
					unsigned i_job(1);

					for (auto& jobs : timeWinInfo.first)  // ��������job
					{
						Job* jobP = jobsMap[jobs.first];
						unsigned reEntry = jobs.second;

						//��ȡ��ǰ���
						unsigned machIndexOfJob(0);
						for (auto& proInfo : jobP->m_proceTargets)
						{
							if (make_pair(curMachP->m_machCode, reEntry) == proInfo.first)
								break;
							++machIndexOfJob;
						}

						double curWidth = getCurWidth(jobP, machIndexOfJob);
						// END OF ��ȡ��ǰ���

						opt << "," << "," << "," << "reentry=" << reEntry
							<< "," << jobP->m_jobCode
							<< "," << i_job
							<< "," << "��ǰ���=" << curWidth << std::endl;
						++i_job;
					}
				}
				++i_timeline;
			}
		}
		else if (airFurnaceSet.find(machInfo.first) != airFurnaceSet.end()) {
			Mach_AirFurnace* curMachP = static_cast<Mach_AirFurnace*>(machInfo.second);
			opt << curMachP->m_machCode << endl;
			unsigned i_job(1);
			for (pair<pair<string, unsigned>, time_period>& timeWinInfo : curMachP->m_allocatedTimeWin)
			{// vector<pair<pair<string, unsigned>, time_period>> m_allocatedTimeWin;

				string jobCode = timeWinInfo.first.first;
				time_period timeWin = timeWinInfo.second;
				double beginT = timeDuration2Double(timeWin.begin() - getCurTime());
				double durationT = timeDuration2Double(timeWin.end() - timeWin.begin());

				opt << i_job << "," << "reentry=" << timeWinInfo.first.second
					<< "," << jobCode
					<< "," << beginT
					<< "," << durationT
					<< "," << jobsMap[jobCode]->m_alloyGrade<< std::endl;
				++i_job;
			}
		}
		else {
			Mach* curMachP = machInfo.second;
			opt << curMachP->m_machCode << endl;
			unsigned i_job(1);
			for (auto& timeWinInfo : curMachP->m_allocatedTimeWin)  // ��������time_period
			{
				string jobCode = timeWinInfo.first.first;
				time_period timeWin = timeWinInfo.second;
				double beginT = timeDuration2Double(timeWin.begin() - getCurTime());
				double durationT = timeDuration2Double(timeWin.end() - timeWin.begin());
				opt << i_job << "," << "reentry=" << timeWinInfo.first.second
					<< "," << jobCode
					<< timeWinInfo.first.second
					<< "," << beginT
					<< "," << durationT << std::endl;
				++i_job;
			}
		}
	};
	cout << "Done of writing to csv..." << endl;

	opt.close();
	opt.open("ResultsOfJob_test.csv", ios::out | ios::trunc);

	cout << "Writing to csv..." << endl;
	for (auto& jobInfo : jobsMap)  // ��������job
	{
		Job* curJobP = jobInfo.second;
		opt << curJobP->m_jobCode << "," << "," << to_iso_extended_string(curJobP->m_dueDateOfOrder) << endl;
		unsigned i_mach(0);
		for (auto& timeInfo : curJobP->m_allocatedTimeWin)  // ��������time_period
		{
			string machCode = timeInfo.first.first;
			time_period timeWin = timeInfo.second;
			double beginT = timeDuration2Double(timeWin.begin() - getCurTime());
			double durationT = timeDuration2Double(timeWin.end() - timeWin.begin());

			opt << i_mach << "," << machCode << "," << beginT
				<< "," << durationT << std::endl;

			++i_mach;
		}
	};
	cout << "Done of writing to csv..." << endl;
}

// ��ӡ�����ų̽��
void writeToCSV(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, MYSQL* mysql)
{
	int coun = 0;
	for (auto& machInfo : machsMap)  // ��������machine
	{
		cout << "machCodde=" << machInfo.first << endl;
		string jobCodeSeqs = "";

		if (CodeOfBellFurn == machInfo.first) {
			Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machInfo.second);
			unsigned i_timeline(0);
			for (auto& timeline : curMachP->m_timeLines)
			{
				for (auto& timeWinInfo : timeline)  // ��������time_period
				{
					// using TimeWin = pair<map<Job*, unsigned>, time_period>;  // pair< map<Job*, ��job�ڼ�������>, job�����ʱ�䴰>

					time_period timeWin = timeWinInfo.second;
					double beginT = timeDuration2Double(timeWin.begin() - getCurTime());
					double durationT = timeDuration2Double(timeWin.end() - timeWin.begin());
					unsigned i_job(1);

					for (auto& jobs : timeWinInfo.first)  // ��������job
					{
						Job* jobP = jobsMap[jobs.first];
						jobCodeSeqs = jobCodeSeqs + "," + jobP->m_jobCode;
					}
				}
				++i_timeline;
			}
		}
		else if (airFurnaceSet.find(machInfo.first) != airFurnaceSet.end()) {
			Mach_AirFurnace* curMachP = static_cast<Mach_AirFurnace*>(machInfo.second);
			unsigned i_job(1);
			for (pair<pair<string, unsigned>, time_period>& timeWinInfo : curMachP->m_allocatedTimeWin)
			{// vector<pair<pair<string, unsigned>, time_period>> m_allocatedTimeWin;

				string jobCode = timeWinInfo.first.first;
				jobCodeSeqs = jobCodeSeqs + "," + jobCode;
			}
		}
		else {
			Mach* curMachP = machInfo.second;
			unsigned i_job(1);
			for (auto& timeWinInfo : curMachP->m_allocatedTimeWin)  // ��������time_period
			{
				string jobCode = timeWinInfo.first.first;
				jobCodeSeqs = jobCodeSeqs + "," + jobCode;
			}
		}
		getCurTime();

		string strCurTime = to_iso_extended_string(getCurTime());
		strCurTime = strCurTime.substr(0, 4) + strCurTime.substr(5, 2) +
			strCurTime.substr(8, 2) + strCurTime.substr(11, 2) + strCurTime.substr(14, 2);

		string sqla = "insert into tlresultset (execution_time, sol_id, unit_id, pro_id) values('"
			+ strCurTime + "','" + "0" + "','"
			+ machInfo.first + "','" + jobCodeSeqs + "')";
		cout << sqla << " coun =" << coun << endl;
		char* sql = (char*)sqla.c_str();
		InsertDatabase1(mysql, sql);
		coun++;
	};



	int count = 0;
	for (auto& jobInfo : jobsMap)  // ��������job
	{
		Job* curJobP = jobInfo.second;
		unsigned i_mach(0);
		for (auto& timeInfo : curJobP->m_allocatedTimeWin)  // ��������time_period
		{
			string machCode = timeInfo.first.first;
			time_period timeWin = timeInfo.second;
			double beginT = timeDuration2Double(timeWin.begin() - getCurTime());
			double durationT = timeDuration2Double(timeWin.end() - timeWin.begin());

			string sStartT = to_iso_extended_string(timeWin.begin());
			string sEndT = to_iso_extended_string(timeWin.last());
			sStartT = sStartT.substr(0, 4) + sStartT.substr(5, 2) +
				sStartT.substr(8, 2) + sStartT.substr(11, 2) + sStartT.substr(14, 2);
			sEndT = sEndT.substr(0, 4) + sEndT.substr(5, 2) +
				sEndT.substr(8, 2) + sEndT.substr(11, 2) + sEndT.substr(14, 2);

			string sqla = "insert into tlresult (unit_id, tape_label, start_time, end_time) values('"
				+ machCode + "','" + curJobP->m_jobCode + "','"
				+ sStartT + "','" + sEndT + "')";
			//cout << sqla << " count ="<<count<< endl;
			char* sql = (char*)sqla.c_str();
			InsertDatabase1(mysql, sql);

			++i_mach;
			++count;
		}
	};
	cout << "Done of writing to csv..." << endl;
	cout << "count=" << count << endl;
}

// --------END OF--��������أ����������̨��CSV�ļ�--------




// --------�Ų���������أ�һ��һ������ÿ������--------

pair<double, double> scheduleByJobOrder(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp,  MYSQL* mysql, bool isPrint) 
{

	int i = 0;
	Job* curJobP;
	for (auto& jobInfo : jobOrder)
	{
		//cout << "  i=" << i++ <<"  jobCode=" << jobInfo.second->m_jobCode<< endl;
		curJobP = jobInfo.second;
		unsigned machIndex(0);  // job�� machine

		//cout << "  ui=" << curJobP<< "  curJobP->jobcode=" << curJobP->m_jobCode << endl;
		//cout<<"  curJobP->m_proceMachs.size()="<< curJobP->m_proceMachs.size() << endl;

		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // ����ĳ��job������machine
		{
			//cout << "    machIndex=" << machIndex << "  machCodeOfCurJob=" << machCodeOfCurJob.first << endl;
			bool isSuccess;
			if (CodeOfBellFurn == machCodeOfCurJob.first)  // ������¯
			{
				Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				//cout << "    curMachP=" << curMachP->m_machCode << endl;
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);

			}
			else if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // ������¯���еĻ���
			{
				Mach_AirFurnace* curMachP = static_cast<Mach_AirFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else  //����
			{
				Mach* curMachP = machsMapTemp[machCodeOfCurJob.first];
				//isSuccess = insertJob(*curJobP, *curMachP, machIndex); 
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}

			//if ("0118741020-0-0"==curJobP->m_jobCode)
			//std::cout << "    curJobP.code"<< curJobP->m_jobCode <<"  curMach: " << machCodeOfCurJob.first
			//	<< "  isSuccess: " << isSuccess << std::endl;
			//if (jobOrder.size() >= 80 && ("0118741020-0-0" == curJobP->m_jobCode)) std::cout << "curMach: " << machCodeOfCurJob.first << "qyqyqyqyqyqyqy " << machIndex << endl;
			if (isSuccess) curJobP->m_curMachIndex = machIndex + 1;
			++machIndex;
		}
	}


	isPrint = false;
	// ��ӡ�����ų̽��
	if (isPrint) printFinalRes(jobsMapTemp, machsMapTemp);
	// �����CSV�ļ�
	if (false) writeToCSV(jobsMapTemp, machsMapTemp);

	//if (isPrint) writeToCSV(jobsMapTemp, machsMapTemp, mysql);
	//if (jobOrder.size() >= 80) std::cout << "mmmmmmmmmmmmmm" << endl;

	// ��ȡĿ��ֵ
	//pair<double, double> objectVals = getObjVals(jobsMapTemp, machsMapTemp);
	map<Job*, double> jobTardiness;
	pair<double, double> objectVals = getObjValsWithTardiness(jobsMapTemp, machsMapTemp, jobTardiness);
	
	//cout<<" Tardiness of jobs:"<<endl;
	//for (auto& ele:jobTardiness) {
	//	Job* jobP = ele.first;
	//	cout << "  "<<jobP->m_jobCode << "  tardiness = " << ele.second<< endl;
	//}

	//std::cout << "������ʱ��(Сʱ): " << objectVals.first << "; �ӹ����й��������ʱ�䳤��(Сʱ): " << objectVals.second << std::endl;

	return objectVals;
};

 // ��������¯�ĵ�һ¯
pair<double, double> scheduleByJobOrder2(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, vector<pair<string, Job*>> jobWithBell, int lastJobWithBell, bool isPrint) {
	//if (jobOrder.size() >= 79)std::cout << "curJobsize: " << jobOrder.size() << std::endl;
	int i_job(0);
	vector<pair<Job*, ptime>> jobsTimeWithBellFurn;  //������¯��job����vector<pair<Job*, ptime���翪ʼʱ��>>
	for (auto& jobInfo : jobOrder)
	{
		if (i_job > lastJobWithBell) break;
		Job* curJobP = jobInfo.second;
		cout << "curJob: " << curJobP->m_jobCode
			<< "  dd=" << curJobP->m_proceMachs.size() << std::endl;
		unsigned machIndex(0);  // job�� machine
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // ����ĳ��job������machine
		{
			cout << "machIndex=" << machIndex << endl;
			// ��������¯������������
			if (CodeOfBellFurn == machCodeOfCurJob.first)
			{
				if (machIndex > 0)  // ��¼ǰһ�����������ʱ��
					jobsTimeWithBellFurn.push_back(make_pair(curJobP, curJobP->m_allocatedTimeWin[machIndex - 1].second.last()));
				else
					jobsTimeWithBellFurn.push_back(make_pair(curJobP, curJobP->m_startDateOfOrder));
				break;
			}

			bool isSuccess;
			if (CodeOfBellFurn == machCodeOfCurJob.first)  // ������¯
			{
				Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				//isSuccess = insertJob(*curJobP, *curMachP, machIndex);  //���ú������ˣ�����
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);

			}
			else if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // ������¯���еĻ���
			{
				Mach_AirFurnace* curMachP = static_cast<Mach_AirFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else  //����
			{
				Mach* curMachP = machsMapTemp[machCodeOfCurJob.first];
				//isSuccess = insertJob(*curJobP, *curMachP, machIndex);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			++machIndex;
		}
		i_job++;
	}

	sort(jobsTimeWithBellFurn.begin(), jobsTimeWithBellFurn.end(), myCmpBy_ptime);  // ����
	unsigned i_parallelMach(0); // ����ƽ�л�  Num_Para_Bell
	Mach_BellFurnace* curMachPBell = static_cast<Mach_BellFurnace*>(machsMapTemp[CodeOfBellFurn]);

	vector<double> curWidths;
	for (auto& jobInfo : jobsTimeWithBellFurn)  //��ȡ���
	{
		Job* jobP = jobInfo.first;
		unsigned machIndexOfJob(0);
		for (auto& proMach : jobP->m_proceMachs)
		{
			if (CodeOfBellFurn == proMach.first) break;
			++machIndexOfJob;
		}
		curWidths.push_back(getCurWidth(jobP, machIndexOfJob));
	}
	cout << "ssssssss curWidths=" << curWidths.size() << endl;
	vector<pair<unsigned, ptime>> batchPositions; // ��ȡ����λ��
	bool bigger(false);
	unsigned num(0);  // ÿ¯�з����˶���
	for (int i = 0; i < curWidths.size(); ++i)
	{
		++num;
		cout << "num=" << num << " i=" << i << endl;
		cout << "curWidths[i]=" << curWidths[i] << endl;

		if (curWidths[i] > curMachPBell->m_RuleForFurnWithWidth.first)
			bigger = true;

		if (((curMachPBell->m_RuleForFurnWithWidth.second.first == num) && bigger)  // 2���������д���650��
			|| (curMachPBell->m_RuleForFurnWithWidth.second.second == num))  // 3������
		{
			cout << "num = " << num << "  bigger=" << bigger << "  i=" << i << endl;
			batchPositions.push_back(make_pair(i, jobsTimeWithBellFurn[i].second));
			num = 0;
			bigger = false;
		}
	}
	cout << "rrrrrrrrrrr batchPositions=" << batchPositions.size() << endl;
	// ��ÿ��ƽ�л��ĵ�һ¯
	int index_position(0);
	int preIndex_Job(0);
	int Index_timeline(0);  //ĳ¯��ʱ����
	for (int i = 0; i < jobsTimeWithBellFurn.size(); i++)
	{
		if (curMachPBell->m_numOfParallel == Index_timeline) break;  //����5��ƽ�л�֮����������ֻ����5��ƽ�л��ĵ�һ¯
		if (index_position >= batchPositions.size()) break;
		if (batchPositions[index_position].first == i)
		{
			int curIndex_job = batchPositions[index_position].first;
			cout << "i=" << i << " batchPositions[curIndex_job].first=" << batchPositions[index_position].first << endl;
			cout << "curIndex_job=" << curIndex_job << endl;
			time_period timeWin(batchPositions[index_position].second, double2timeDuration(curMachPBell->m_proceTimePerFurn));
			list<TimeWin>& timeline = curMachPBell->m_timeLines[Index_timeline];
			timeline.push_back(make_pair(map<string, unsigned>({}), timeWin));
			cout << "preIndex_Job=" << preIndex_Job << "  curIndex_job=" << curIndex_job
				<< " timeline=" << Index_timeline << endl;

			for (int index = preIndex_Job; index <= curIndex_job; ++index)
			{
				Job* jobP = jobsTimeWithBellFurn[index].first;
				jobP->m_allocatedTimeWin.push_back(make_pair(make_pair(CodeOfBellFurn, 1), timeWin));
				timeline.begin()->first.insert(make_pair(jobP->m_jobCode, 1));
				cout << "index=" << index << endl;
			}
			preIndex_Job = batchPositions[index_position].first + 1;
			++index_position;
			++Index_timeline;

			cout << "rtrtrtrtrtr" << endl;
		}
	}
	cout << "ttttttttttttt" << endl;

	for (auto& jobInfo : jobOrder)
	{
		Job* curJobP = jobInfo.second;
		cout << "curJob: " << curJobP->m_jobCode
			<< "  dd=" << curJobP->m_proceMachs.size() << std::endl;
		unsigned machIndex(0);  // job�� machine
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // ����ĳ��job������machine
		{
			cout << "machIndex=" << machIndex << endl;
			if (machIndex < curJobP->m_allocatedTimeWin.size())  // �Ź���ֱ������
			{
				++machIndex;
				continue;
			}

			bool isSuccess;
			if (CodeOfBellFurn == machCodeOfCurJob.first)  // ������¯
			{
				Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				//isSuccess = insertJob(*curJobP, *curMachP, machIndex);  // ���ú������ˣ�����
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // ������¯���еĻ���
			{
				Mach_AirFurnace* curMachP = static_cast<Mach_AirFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else  //����
			{
				Mach* curMachP = machsMapTemp[machCodeOfCurJob.first];
				//isSuccess = insertJob(*curJobP, *curMachP, machIndex);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			++machIndex;
		}
		i_job++;
	}



	//if (jobOrder.size() >= 80) std::cout << "uiuiuiui" << endl;
	// ��ӡ�����ų̽��
	if (isPrint) printFinalRes(jobsMapTemp, machsMapTemp);
	// �����CSV�ļ�
	if (isPrint) writeToCSV(jobsMapTemp, machsMapTemp);

	//if (jobOrder.size() >= 80) std::cout << "mmmmmmmmmmmmmm" << endl;

	// ��ȡĿ��ֵ
	pair<double, double> objectVals = getObjVals(jobsMapTemp, machsMapTemp);
	//std::cout << "������ʱ��(Сʱ): " << objectVals.first << "; �ӹ����й��������ʱ�䳤��(Сʱ): " << objectVals.second << std::endl;

	//if (jobOrder.size() >= 80)  std::cout << "aaaaaaaaaaaaaaaaaaaaa" << endl;

	for (auto& jobInfo : jobsMapTemp) delete jobInfo.second;
	for (auto& machInfo : machsMapTemp) delete machInfo.second;

	return objectVals;
};

// ÿ���������λ�ã����в���
double scheduleByJobOrder3(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapLocal, map<string, Mach*>& machsMapLocal, MYSQL* mysql, bool isPrint)
{
	set<string> machSet;
	for (auto& jobInfo : jobOrder) // ��������job
	{
		Job* curJobP = jobInfo.second;
		cout << "\n\n" << endl;
		cout << "curJob: " << curJobP->m_jobCode
			<< "  dd_m_proceMachs.size()=" << curJobP->m_proceMachs.size() << std::endl;
		for (auto mach : machSet)
		{
			cout << "mach=" << mach << endl;
		}

		unsigned machIndex(0);  // job�� machine
		ptime readyTimeForJob;  // job������ĳ�����Ͽ��Կ�ʼ�ӹ���ʱ�䣬��һ�������Ľ���ʱ���
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // ����ĳ��job������machine
		{

			Mach* curMachP = machsMapLocal[machCodeOfCurJob.first];
			cout << "\n" << endl;
			cout << "                  curMach_=" << curMachP->m_machCode << endl;

			machSet.insert(curMachP->m_machCode);

			time_duration& processTime = curJobP->m_proceTimes[machIndex].second;
			time_period timeWinToInsertFinal(curJobP->m_startDateOfOrder, processTime);  //Ҫ�����ʱ�䴰
			if (0 == machIndex)  // �����job�ĵ�һ��machine
				readyTimeForJob = curJobP->m_startDateOfOrder;
			//else 
				//readyTimeForJob = (curJobP->m_allocatedTimeWinPs.end() - 1)->second->last();

			double insertPositionFinal;  // ���յĲ���λ��
			bool IsJustPutInFinal;  // ��¼��ֱ�ӷ��뻹�ǲ��벢��������Ų
			if (0 == curMachP->m_allocatedTimeWin.size())  // �����machine�ϵĵ�һ��job����ֱ�ӷ���
			{
				timeWinToInsertFinal = time_period(readyTimeForJob, processTime);
				insertPositionFinal = 0;
				IsJustPutInFinal = true;
				cout << "insertDerict" << endl;



				if (0 & ("0118381020-P-0-0" == curJobP->m_jobCode)) // && ("BD-S005" == curMachP->m_machCode) && (0 == IndexOfTimeWin)
				{
					cout << " startTime=" << to_iso_extended_string(curJobP->m_startDateOfOrder) << endl;
					cout << " readyTimeForJob=" << to_iso_extended_string(readyTimeForJob) << endl;

					cout << "noye" << endl;

					double a;
					scanf("%d", &a);
					printf("a=%d", a);
					scanf("%d", &a);
					//break;
				}

			}
			else // �������machine�ϵĵ�һ��job�������ʱ�䣬���β��룬���Ƚ�makespan
			{
				bool isJustPutIn;  // ��¼��ֱ�ӷ��뻹�ǲ��벢��������Ų
				double finalMakespan = -100;
				bool isRenewInsertInfo;  // ��¼��һ���Ƿ�Ҫ���²�����Ϣ

				map<string, Mach*> machsMapTemp;
				initMachsMapTemp(machsMapTemp, machsMapLocal);  // ��ʼ��machsMapTemp

				unsigned IndexOfTimeWin(0);  // ���û�����ʱ�䴰����룬�����ʱ�䴰������

				int reentry = curJobP->m_proceMachs[machIndex].second;
				cout << "reentry=" << reentry << endl;
				if (reentry > 1)  // �����job���뵽�û���
				{
					int reentryCount = 0;
					cout << "curMachP->m_allocatedTimeWin.size()=" << curMachP->m_allocatedTimeWin.size() << endl;
					// ����curMach.m_allocatedTimeWinÿһ��ʱ�䴰���ҵ���ǰ�����reentry=1�����
					for (auto timeInfo_iter = curMachP->m_allocatedTimeWin.begin(); timeInfo_iter != curMachP->m_allocatedTimeWin.end();
						++timeInfo_iter, ++IndexOfTimeWin)
					{
						if (curJobP->m_jobCode == timeInfo_iter->first.first) ++reentryCount;
						if (reentryCount == reentry - 1) // ��λ��job��һ���ڸû����ϼӹ���ʱ�䴰��λ��
						{
							cout << "IndexOfTimewwWin=" << IndexOfTimeWin << endl;
							IndexOfTimeWin += 1;  // ������Ųһ�£��Ա���ǰ�����
							break;
						}
					}
					cout << "IndexOfTimeWin=" << IndexOfTimeWin << endl;
				}
				unsigned IndexOfStart = IndexOfTimeWin;  // ���û�����ʱ�䴰����룬��¼��ʼ�����ʱ�䴰��λ��

				// ����curMach.m_allocatedTimeWinÿһ��ʱ�䴰��������
				for (; IndexOfTimeWin <= curMachP->m_allocatedTimeWin.size(); ++IndexOfTimeWin)
				{

					if (IndexOfStart != IndexOfTimeWin)  reInitMachsMapTemp(machsMapTemp, machsMapLocal);
					Mach* curMachPTemp = machsMapTemp[curMachP->m_machCode];
					time_period timeWinToInsert(curJobP->m_startDateOfOrder, processTime);  //Ҫ�����ʱ�䴰

					if (curMachPTemp->m_allocatedTimeWin.size() == IndexOfTimeWin) // ��������ʱ�䴰֮��
					{
						auto iter = curMachPTemp->m_allocatedTimeWin.end();
						ptime right_TimeWinPro = (--iter)->second.last();
						readyTimeForJob = readyTimeForJob > right_TimeWinPro ? readyTimeForJob : right_TimeWinPro;

						timeWinToInsert = time_period(readyTimeForJob, processTime);
						curMachPTemp->m_allocatedTimeWin.push_back(make_pair(make_pair(curJobP->m_jobCode, curJobP->m_proceMachs[machIndex].second)
							, timeWinToInsert));
						isJustPutIn = true;


						if (0 & ("0118381020-P-0-0" == curJobP->m_jobCode) && ("BD-S005" == curMachP->m_machCode) && (0 == IndexOfTimeWin))
						{
							cout << " startTime=" << to_iso_extended_string(curJobP->m_startDateOfOrder) << endl;
							cout << " readyTimeForJob=" << to_iso_extended_string(readyTimeForJob) << endl;

							cout << "noye" << endl;

							double a;
							scanf("%d", &a);
							printf("a=%d", a);
							scanf("%d", &a);
							//break;
						}

					}
					else
					{
						//preInsertTimeWin(, unsigned IndexOfTimeWin);

						auto timeWinIter = curMachPTemp->m_allocatedTimeWin.begin();
						for (int i = 0; i < IndexOfTimeWin; ++i) ++timeWinIter;
						ptime left_TimeWin = timeWinIter->second.begin();

						if (left_TimeWin < readyTimeForJob)  // 
						{
							isRenewInsertInfo = true;  // ��һ��һ�����²�����Ϣ
							continue;
						}


						//0118381020-P-0-0 0118651020-0-0
						if (0 & ("0118381020-P-0-0" == curJobP->m_jobCode)
							&& ("BD-S005" == curMachP->m_machCode) && (0 == IndexOfTimeWin))

						{
							cout << " startTime=" << to_iso_extended_string(curJobP->m_startDateOfOrder) << endl;
							cout << " readyTimeForJob=" << to_iso_extended_string(readyTimeForJob) << endl;
							cout << " left_TimeWin=" << to_iso_extended_string(left_TimeWin) << endl;
							cout << "noye" << endl;
							cout << "IndexOfTimeWin=" << IndexOfTimeWin << endl;

							double a;
							scanf("%d", &a);
							printf("a=%d", a);
							scanf("%d", &a);
							//break;
						}

						if (0 == IndexOfTimeWin)  // ���ڵ�һ��ʱ�䴰֮ǰ
						{
							timeWinToInsert = time_period(readyTimeForJob, processTime);

							curMachPTemp->m_allocatedTimeWin.insert(timeWinIter,
								make_pair(make_pair(curJobP->m_jobCode, curJobP->m_proceMachs[machIndex].second), timeWinToInsert));
							if (readyTimeForJob + processTime <= left_TimeWin) // ֱ�ӷ��뼴��
								isJustPutIn = true;
							else  // ���򣬻��ǲ��벢Ų�����ʱ�䴰
							{

								//0118381020-P-0-0 0118651020-0-0
								if (0 & ("0118381020-P-0-0" == curJobP->m_jobCode)) // && ("BD-S005" == curMachP->m_machCode) && (0 == IndexOfTimeWin)
								{
									cout << " startTime=" << to_iso_extended_string(curJobP->m_startDateOfOrder) << endl;
									cout << " readyTimeForJob=" << to_iso_extended_string(readyTimeForJob) << endl;
									cout << " left_TimeWin=" << to_iso_extended_string(left_TimeWin) << endl;
									cout << "noye" << endl;

									double a;
									scanf("%d", &a);
									printf("a=%d", a);
									scanf("%d", &a);
									//break;
								}


								isJustPutIn = false;
								moveTheFollowedTimeWin(jobsMapLocal, machsMapTemp, curMachPTemp, IndexOfTimeWin);


							}
						}
						else  // ���뵽ʱ�䴰֮��
						{
							ptime right_TimeWinPro = (--timeWinIter)->second.last();
							readyTimeForJob = readyTimeForJob > right_TimeWinPro ? readyTimeForJob : right_TimeWinPro;

							timeWinToInsert = time_period(readyTimeForJob, processTime);
							curMachPTemp->m_allocatedTimeWin.insert(timeWinIter,
								make_pair(make_pair(curJobP->m_jobCode, curJobP->m_proceMachs[machIndex].second), timeWinToInsert));
							if (readyTimeForJob + processTime <= left_TimeWin)  // ֱ�ӷ��뼴��
								isJustPutIn = true;
							else   // ���򣬻��ǲ��벢Ų�����ʱ�䴰
							{
								isJustPutIn = false;
								moveTheFollowedTimeWin(jobsMapLocal, machsMapTemp, curMachPTemp, IndexOfTimeWin);
							}
						}
					}

					double curMachspan = getMakespan(jobsMapLocal, machsMapTemp);  // ��ȡmakespan
					//double curMachspan = getMakespan();
					cout << "IndexOfStart=" << IndexOfStart << "  IndexOfTimeWin=" << IndexOfTimeWin << endl;

					if ((IndexOfStart == IndexOfTimeWin) || (curMachspan < finalMakespan) || isRenewInsertInfo)  // ���²�����Ϣ
					{
						finalMakespan = curMachspan;
						insertPositionFinal = IndexOfTimeWin;
						IsJustPutInFinal = isJustPutIn;
						timeWinToInsertFinal = timeWinToInsert;
						//cout << "finalMakespan=" << finalMakespan << "  curMachspan=" << curMachspan << endl;
					}

					isRenewInsertInfo = false;  // ��һ�ֲ�һ�����²�����Ϣ

					cout << "  curMachspan=" << curMachspan << "  finalMakespan=" << finalMakespan << endl;
					cout << "   curIndex=" << IndexOfTimeWin << endl;
					cout << endl;
				}  // END OF ����curMach.m_allocatedTimeWin


				for (auto& machInfo : machsMapTemp) delete machInfo.second;
			}

			cout << "insertPositionFinal=" << insertPositionFinal << endl;

			time_period* timeWinPToInsert = new time_period(timeWinToInsertFinal);
			// ����ʵ�ʲ���
			if (curMachP->m_allocatedTimeWin.size() == insertPositionFinal)  // �ŵ��û���������ʱ�䴰�ĺ���
			{

				curMachP->m_allocatedTimeWin.push_back(make_pair(
					make_pair(curJobP->m_jobCode, curJobP->m_proceMachs[machIndex].second), timeWinToInsertFinal));

				curMachP->m_allocatedTimeWinPs.push_back(make_pair(
					make_pair(curJobP->m_jobCode, curJobP->m_proceMachs[machIndex].second), timeWinPToInsert));
				curJobP->m_allocatedTimeWinPs.push_back(make_pair(
					make_pair(curMachP->m_machCode, curJobP->m_proceMachs[machIndex].second), timeWinPToInsert));
			}
			else    // ���뵽�û���������ʱ�䴰���м�
			{
				auto iter1 = curMachP->m_allocatedTimeWin.begin();
				for (int i = 0; i < insertPositionFinal; ++i) ++iter1;
				curMachP->m_allocatedTimeWin.insert(iter1, make_pair(
					make_pair(curJobP->m_jobCode, curJobP->m_proceMachs[machIndex].second), timeWinToInsertFinal));

				auto iter2 = curMachP->m_allocatedTimeWinPs.begin();
				for (int i = 0; i < insertPositionFinal; ++i) ++iter2;
				// �������յó��Ĳ���λ��insertPositionFinal����ĳ�û����ϲ���ù���;
				curMachP->m_allocatedTimeWinPs.insert(iter2, make_pair(
					make_pair(curJobP->m_jobCode, curJobP->m_proceMachs[machIndex].second), timeWinPToInsert));
				curJobP->m_allocatedTimeWinPs.push_back(make_pair(
					make_pair(curMachP->m_machCode, curJobP->m_proceMachs[machIndex].second), timeWinPToInsert));
				// ֱ�ӷ��룬���ǲ��벢Ų�����ʱ�䴰
				if (!IsJustPutInFinal)// ��ҪŲ�������ʱ�䴰
				{
					moveTheFollowedTimeWin(jobsMapLocal, machsMapLocal, curMachP, insertPositionFinal);
					syncTimeWinANDTimeWinPs(machsMapLocal);
				}

			}
			readyTimeForJob = timeWinToInsertFinal.last(); //��һ����������翪ʼʱ��

			++machIndex;
		} // END OF ����ĳ��job������machine


		cout << "tyyutyuiuviiiiiinod" << endl;
		for (auto& machInfo : machsMapLocal)   // ����ĳ��job������machine
		{

			Mach* curMachP = machInfo.second;
			std::cout << "MachCode---------" << curMachP->m_machCode << std::endl;
			for (pair<pair<string, unsigned>, time_period>& timeWinInfo : curMachP->m_allocatedTimeWin)
			{// vector<pair<pair<string, unsigned>, time_period>> m_allocatedTimeWin;
				string jobCode = timeWinInfo.first.first;
				time_period timeWin = timeWinInfo.second;
				std::cout << "jobCode  " << jobCode << ": " << "reentry=" << timeWinInfo.first.second
					<< " " << to_iso_extended_string(timeWin.begin())
					<< "--" << to_iso_extended_string(timeWin.last())
					<< jobsMapLocal[jobCode]->m_alloyType << std::endl;
			}
			//std::cout << std::endl;
		}

	} // END OF ��������job

	//syncTimeWinANDTimeWinPs(machsMapLocal);

	//if (jobOrder.size() >= 80) std::cout << "uiuiuiui" << endl;
	// ��ӡ�����ų̽��
	if (isPrint) printFinalRes2(jobsMapLocal, machsMapLocal);
	// �����CSV�ļ�
	// if (isPrint) writeToCSV(jobsMapTemp, machsMapTemp, mysql);

	//if (jobOrder.size() >= 80) std::cout << "mmmmmmmmmmmmmm" << endl;

	// ��ȡĿ��ֵ
	//pair<double, double> objectVals = getObjVals(jobsMapTemp, machsMapTemp);
	//std::cout << "������ʱ��(Сʱ): " << objectVals.first << "; �ӹ����й��������ʱ�䳤��(Сʱ): " << objectVals.second << std::endl;

	//if (jobOrder.size() >= 80)  std::cout << "aaaaaaaaaaaaaaaaaaaaa" << endl;


	return getMakespan(jobsMapLocal, machsMapLocal);
};

// ����¯���������
pair<double, double> scheduleByJobOrder4(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, vector<vector<pair<Job*, int>>>& batchOfBell, MYSQL* mysql, bool isPrint) {
	//if (jobOrder.size() >= 79)std::cout << "curJobsize: " << jobOrder.size() << std::endl;
	int jobIndex = 0;
	for (auto& jobInfo : jobOrder)
	{
		Job* curJobP = jobInfo.second;
		//cout<< "curJob: " << curJobP->m_jobCode
			//<<"dd"<<curJobP->m_proceMachs.size()<< std::endl;
		unsigned machIndex(0);  // job�� machine
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // ����ĳ��job������machine
		{
			if ("9800257701-0-0" == curJobP->m_jobCode)
			{
				std::cout << "         curMach: " << machCodeOfCurJob.first << endl;
				cout << "  " << curJobP->m_allocatedTimeWin.size() << endl;
			}

			bool isSuccess;
			if (CodeOfBellFurn == machCodeOfCurJob.first)  // ������¯
			{
				Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				int batchIndexFinal = insertJob(*curJobP, *curMachP, batchOfBell, machsMapTemp);
				if (batchIndexFinal >= 0)
				{
					vector<pair<Job*, int>>& curBatch = batchOfBell[batchIndexFinal];  // ���е�ǰjob������
					vector<pair<Job*, int>> reentryBatch;  // ����¯���������
					for (auto it = curBatch.begin(); it != curBatch.end(); ++it) // �Ը����ε�job��������¯��������
					{
						Job* curJobP = it->first;
						int machIndex = curJobP->m_allocatedTimeWin.size();
						for (; machIndex < curJobP->m_proceMachs.size(); ++machIndex)     // ����job�ĺ�������ֱ����������¯
						{
							Mach* curMachP = machsMapTemp[curJobP->m_proceMachs[machIndex].first];
							if ("9800257701-0-0" == curJobP->m_jobCode)
							{
								std::cout << "         curMach: " << curMachP->m_machCode << endl;
								cout << "  " << curJobP->m_allocatedTimeWin.size() << endl;
							}
							//cout << "\n" << endl;
							//cout << "                  curMach_=" << curMachP->m_machCode << endl;
							if (curMachP->m_machCode == CodeOfBellFurn) 
							{
								reentryBatch.push_back(make_pair(curJobP, 1));  //���������batch
								break;
							}
							//insertJob(*curJobP, *curMachP, machIndex);
							isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
						}
					}
					if (reentryBatch.size() > 0)  // �Ը����ε�job�����������¯��������
					{
						ptime readyTimeForOrders;
						for (auto& jobInfo : reentryBatch)
						{
							Job* curJobP = jobInfo.first;
							ptime readyTime=(curJobP->m_allocatedTimeWin.end() - 1)->second.last();
							readyTimeForOrders = readyTimeForOrders > readyTime ? readyTimeForOrders: readyTime;
						}
						insertJobToBellFurnWithPreBatch(reentryBatch, readyTimeForOrders, *curMachP);
					}
					for (auto it = reentryBatch.begin(); it != reentryBatch.end(); ++it) // �Ը����ε�job��������¯��������
					{
						Job* curJobP = it->first;
						int machIndex = curJobP->m_allocatedTimeWin.size();
						for (; machIndex < curJobP->m_proceMachs.size(); ++machIndex)     // ����job�ĺ�������ֱ����������¯
						{
							Mach* curMachP = machsMapTemp[curJobP->m_proceMachs[machIndex].first];
							if (curMachP->m_machCode == CodeOfBellFurn)
							{	break;}
							//insertJob(*curJobP, *curMachP, machIndex);
							isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
						}
					}
					break;
				}
			}
			else if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // ������¯���еĻ���
			{
				Mach_AirFurnace* curMachP = static_cast<Mach_AirFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else  //����
			{
				Mach* curMachP = machsMapTemp[machCodeOfCurJob.first];
				//isSuccess = insertJob(*curJobP, *curMachP, machIndex);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			//if ("0118741020-0-0"==curJobP->m_jobCode)
			//std::cout << "curJobP.code"<< curJobP->m_jobCode <<"curMach: " << machCodeOfCurJob.first
			//	<< "isSuccess: " << isSuccess << std::endl;
			//if (jobOrder.size() >= 80 && ("0118741020-0-0" == curJobP->m_jobCode)) std::cout << "curMach: " << machCodeOfCurJob.first << "qyqyqyqyqyqyqy " << machIndex << endl;
			++machIndex;
		}
		++jobIndex;
	}

	//if (jobOrder.size() >= 80) std::cout << "uiuiuiui" << endl;
	// ��ӡ�����ų̽��
	if (isPrint) printFinalRes(jobsMapTemp, machsMapTemp);
	// �����CSV�ļ�
	// if (isPrint) writeToCSV(jobsMapTemp, machsMapTemp, mysql);

	//if (jobOrder.size() >= 80) std::cout << "mmmmmmmmmmmmmm" << endl;

	// ��ȡĿ��ֵ
	pair<double, double> objectVals = getObjVals(jobsMapTemp, machsMapTemp);
	//std::cout << "������ʱ��(Сʱ): " << objectVals.first << "; �ӹ����й��������ʱ�䳤��(Сʱ): " << objectVals.second << std::endl;

	//if (jobOrder.size() >= 80)  std::cout << "aaaaaaaaaaaaaaaaaaaaa" << endl;

	for (auto& jobInfo : jobsMapTemp) delete jobInfo.second;
	for (auto& machInfo : machsMapTemp) delete machInfo.second;

	return objectVals;
};

void moveTheFollowedTimeWin(map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, Mach* curMachP, int IndexOfTimeWin)
{
	
	std::cout << "MachCode---------" << curMachP->m_machCode << std::endl;
	for (pair<pair<string, unsigned>, time_period>& timeWinInfo : curMachP->m_allocatedTimeWin)
	{// vector<pair<pair<string, unsigned>, time_period>> m_allocatedTimeWin;
		string jobCode = timeWinInfo.first.first;
		time_period timeWin = timeWinInfo.second;
		std::cout << "jobCode  " << jobCode << ": " << "reentry=" << timeWinInfo.first.second
			<< " " << to_iso_extended_string(timeWin.begin())
			<< "--" << to_iso_extended_string(timeWin.last())
			<< std::endl;
	}


	queue<pair<Mach*, pair<int, int>>> changedTimeWins;  // queue<pair<Mach*, pair<int, int>> ���У�����ı��ʱ�䴰����ҪŲ��ʱ�䴰����
	IndexOfTimeWin += 1;
	int IndexOfTimeWin2 = IndexOfTimeWin;  // 


	auto timeInfo_iter = curMachP->m_allocatedTimeWin.begin();
	for (int i = 0; i < IndexOfTimeWin; ++i) ++timeInfo_iter;
	ptime earlistStartT = (--timeInfo_iter)->second.last();  // ��ʼʱ��

	// ��鵱ǰλ��֮���ʱ���Ƿ���Ҫ����Ų
	for (auto timeInfoMove_iter = timeInfo_iter; timeInfoMove_iter != curMachP->m_allocatedTimeWin.end();
		++timeInfoMove_iter, ++IndexOfTimeWin2)
	{

		ptime left_TimeWin2 = timeInfoMove_iter->second.begin();
		ptime right_TimeWin2 = timeInfoMove_iter->second.last();
		if (earlistStartT <= left_TimeWin2)
			break;
		else {
			// ����Ų
			time_duration processTimeWin = timeInfoMove_iter->second.last()
				- timeInfoMove_iter->second.begin();
			//timeInfoMove_iter->second = time_period( earlistStartT, earlistStartT + processTimeWin);
			timeInfoMove_iter->second.shift(earlistStartT- timeInfoMove_iter->second.begin());

			earlistStartT = timeInfoMove_iter->second.last();  // ���¿�ʼʱ��

		}

	}

	std::cout << "MachCode---------" << curMachP->m_machCode << std::endl;
	for (pair<pair<string, unsigned>, time_period>& timeWinInfo : curMachP->m_allocatedTimeWin)
	{// vector<pair<pair<string, unsigned>, time_period>> m_allocatedTimeWin;
		string jobCode = timeWinInfo.first.first;
		time_period timeWin = timeWinInfo.second;
		std::cout << "jobCode  " << jobCode << ": " << "reentry=" << timeWinInfo.first.second
			<< " " << to_iso_extended_string(timeWin.begin())
			<< "--" << to_iso_extended_string(timeWin.last())
			<< std::endl;
	}

	if (curMachP->m_machCode == "BD-S005"|| curMachP->m_machCode == "BD-S006")
	{
		cout << "IndexOfTimeWin=" << IndexOfTimeWin << "            IndexOfTimeWin2=" << IndexOfTimeWin2 << endl;
	}

	Job* curJobP = jobsMapTemp[curMachP->m_allocatedTimeWin.begin()->first.first];
	
	if (0&("0118381020-P-0-0" == curJobP->m_jobCode)
		&& ("BD-S005" == curMachP->m_machCode) && (0 == IndexOfTimeWin))
	{
		cout << " startTime=" << to_iso_extended_string(curJobP->m_startDateOfOrder) << endl;
		cout << "yenoooo" << endl;

		double a;
		scanf("%d", &a);
		printf("a=%d", a);
		scanf("%d", &a);
		//break;
	}


	changedTimeWins.push(make_pair(curMachP, make_pair(IndexOfTimeWin, IndexOfTimeWin2)));


	// ��������Ҫ����Ų��ʱ�䴰�Ļ���
	while (changedTimeWins.size())
	{
		
		pair<Mach*, pair<int, int>>& changedTimeWin = changedTimeWins.front();
		int indexFro = changedTimeWin.second.first;  // ��ҪŲ��ʱ�䴰����
		int indexRear = changedTimeWin.second.second;
		Mach* machPChanged = changedTimeWin.first;
	

		if (0 & ("0118381020-P-0-0" == curJobP->m_jobCode))
		{
			cout << "\n" << endl;
			cout << " machCodeer=" << machPChanged->m_machCode << endl;
			cout << "yhhhhoo" << endl;

			double a = 0.0;
			scanf("%d", &a);
			printf("a=%d", a);
			scanf("%d", &a);
			//break;
		}



		// ����ĳ�����ϵ�Ų����ʱ�䴰
		cout <<"  machCode="<< machPChanged->m_machCode << " indexFro=" << indexFro << " indexRear=" << indexRear << endl;
		for (int indexMachWin = indexFro; indexMachWin < indexRear; indexMachWin++)
		{
			//pair<pair<string, unsigned>, time_period>& timeWinInfo = machPChanged->m_allocatedTimeWin[indexMachWin];  // �����ԭ��
			pair<pair<string, unsigned>, time_period>& timeWinInfo = *machPChanged->m_allocatedTimeWin.begin();
			
			Job* jobPChanged = jobsMapTemp[timeWinInfo.first.first];

			int i = 0; // job�Ĺ�������
			for (; i < jobPChanged->m_proceMachs.size(); i++) // ���ڸù����ϵ�ĳ��ʱ�䴰���ж�Ӧjob����ȡ�ù����ڸ�job�����й����е�����
			{ 
				if (make_pair(curMachP->m_machCode, timeWinInfo.first.second) == jobPChanged->m_proceMachs[i])
					break;
			}
			if (jobPChanged->m_proceMachs.size() - 1 == i) continue;

			
			cout << "    jobPChanged=" << jobPChanged->m_jobCode << endl;
			cout << "     indexFro=" << indexFro << "  indexRear=" << indexRear << "  indexMachWin=" << indexMachWin << endl;
			cout << "    i_jobb ="<< i <<endl;
			
			ptime endTOfLastProcess = timeWinInfo.second.last();;  // job����һ�������ʱ�䴰���Ҳ�
			for (int j = i+1; j < jobPChanged->m_proceMachs.size(); j++)  // ����ĳʱ�䴰��Ӧ��job��������job�ĺ�������
			{
				Mach* machSuccessorP = machsMapTemp[jobPChanged->m_proceMachs[j].first];  // job�ĵ�ǰ������ĺ�������
				pair<string, unsigned int>& jobProccessInfo = jobPChanged->m_proceMachs[j];

				int startMachIndex = 0;
				for (; startMachIndex < machSuccessorP->m_allocatedTimeWin.size(); startMachIndex++)
					// ��job��ĳ��������Ļ��飬��λ����job��ʱ�䴰
				{
					if (make_pair(jobPChanged->m_jobCode, jobProccessInfo.second)
						== machSuccessorP->m_allocatedTimeWin.begin()->first)
						//== machSuccessorP->m_allocatedTimeWin[startMachIndex].first)  //ԭ��
					{
						//cout<<"ddddddgdgd"<<endl;
						break;
					}
						
				}
				cout << "      job�ĺ���machCode=" << machSuccessorP->m_machCode <<"j="<<j<< "  startMachIndex=" << startMachIndex << endl;

				int IndexTimeWin;
				ptime endTOfLastTimeWin = endTOfLastProcess;  // ���ڸù������翪ʼʱ������һ������Ľ���ʱ��
				for (IndexTimeWin = startMachIndex; IndexTimeWin < machSuccessorP->m_allocatedTimeWin.size(); IndexTimeWin++)
					// �Ը�job��ĳ�������򣬱����û�������ʱ�䴰���ж��Ƿ���Ҫ����Ų
				{

					//cout << "           endTOfLastTimeWin=" << to_iso_extended_string(endTOfLastTimeWin) << endl;


					/* // ԭ��ûע��
					if (endTOfLastTimeWin <= machSuccessorP->m_allocatedTimeWin[IndexTimeWin].second.begin())
						break;
					else {
						// ����Ų
						time_duration processTimeWin = machSuccessorP->m_allocatedTimeWin[IndexTimeWin].second.last()
							- machSuccessorP->m_allocatedTimeWin[IndexTimeWin].second.begin();
						machSuccessorP->m_allocatedTimeWin[IndexTimeWin].second.shift(endTOfLastTimeWin - machSuccessorP->m_allocatedTimeWin[IndexTimeWin].second.begin());
						endTOfLastTimeWin = machSuccessorP->m_allocatedTimeWin[IndexTimeWin].second.last();  // ���¿�ʼʱ��
	
					//	cout << "          right_TimeWin__22=" << to_iso_extended_string(machSuccessorP->m_allocatedTimeWin[IndexTimeWin].second.last()) << endl;
					}
					*/
					if (IndexTimeWin == startMachIndex)  // ��һ��ʱ�䴰
						endTOfLastProcess = endTOfLastTimeWin;
				}
				if(IndexTimeWin == startMachIndex) break;  // ������ڣ���job��ĳ�������򣬱����û�������ʱ�䴰�����������ĵ�һ��ʱ�䴰�Ͳ���ҪŲ������Ҫ����job�ĺ����Ĺ���

				if (IndexTimeWin > startMachIndex + 1)    // ������ڣ���job��ĳ�������򣬱����û�������ʱ�䴰�������ֻ�к���ĵ�һ��ʱ�䴰��ҪŲ������Ҫ�����ʱ�䴰�ĺ���ʱ�䴰
				{
					changedTimeWins.push(make_pair(machSuccessorP, make_pair(startMachIndex + 1, IndexTimeWin)));
					cout << "        machSuccessorP=" << machSuccessorP->m_machCode << " startMachIndex=" << startMachIndex
						<< " IndexTimeWin=" << IndexTimeWin << endl;
				
				}
			} // END OF ����ĳʱ�䴰��Ӧ��job��������job�ĺ�������

		}// END OF ����ĳ�����ϵ�Ų����ʱ�䴰

		changedTimeWins.pop();
	}

};

// ͬ�� ��
void syncTimeWinANDTimeWinPs(map<string, Mach*>& machsMapTemp)  // ͬ��m_allocatedTimeWinPs��m_allocatedTimeWin
{
	for (auto& machInfo : machsMapTemp)
	{
		Mach* machP = machInfo.second;
		int index = 0;
		for (auto& timeWin: machP->m_allocatedTimeWin)
		{
			auto iter = machP->m_allocatedTimeWinPs.begin();
			for (int i = 0; i < index; ++i) ++iter;
			(iter)->second = &timeWin.second;
			++index;
		}
	}
};

// --------END OF--�Ų���������أ�һ��һ������ÿ����--------




// --------�����Ų���أ�����job��machine--------

// ����Job--��jobOrder�е�����job����һ�ݣ����뵽jobsMapTemp��
void initJobsTemp(map<string, Job*>& jobsMapNew, vector<pair<string, Job*>>& jobsOrderNew, vector<pair<string, Job*>>& jobOrder) {

	g_mutex.lock();
	jobsOrderNew.reserve(jobOrder.size());
	g_mutex.unlock();

	//cout << jobsOrderNew.size() << endl;
	for (auto& ele : jobsOrderNew) {
		Job* jobP = ele.second;
		delete jobP;
	}
	jobsOrderNew.clear();

	for (int i = 0; i < jobOrder.size(); ++i) {
		Job* jobP = new Job;
		Job* jobPOrig = jobOrder[i].second;
		*jobP = *jobPOrig;
		jobP->m_allocatedTimeWin.reserve(jobP->m_proceMachs.size());  // Ԥ��job�Ĺ������ռ�
		jobsOrderNew.emplace_back(make_pair(jobP->m_jobCode, jobP));
	}

	g_mutex.lock();
	for (int i = 0; i < jobsOrderNew.size(); ++i)
	{
		Job* jobP = jobsOrderNew[i].second;
		jobsMapNew.insert(make_pair(jobP->m_jobCode, jobP));
	}
	g_mutex.unlock();
};

// ����machine--��machsMap�е�����machine����һ�ݣ����뵽machsMapTemp��
void initMachsMapTemp(map<string, Mach*>& machsMapNew, map<string, Mach*>& machsMap)
{	
	for (auto& machInfo : machsMapNew)
	{
		if (CodeOfBellFurn == machInfo.second->m_machCode)  // ������¯
		{
			Mach_BellFurnace* machP = new Mach_BellFurnace();
			*machP = *(static_cast<Mach_BellFurnace*> (machInfo.second));
			delete machP;
		}
		else if (airFurnaceSet.find(machInfo.second->m_machCode) != airFurnaceSet.end()) // ������¯
		{
			Mach_AirFurnace* machP = new Mach_AirFurnace();
			*machP = *(static_cast<Mach_AirFurnace*> (machInfo.second));
			delete machP;
		}
		else
		{
			Mach* machP = new Mach;
			*machP = *(machInfo.second);
			delete machP;
		}
	}
	machsMapNew.clear();

	for (auto& machInfo : machsMap)
	{

		if (CodeOfBellFurn == machInfo.second->m_machCode)  // ������¯
		{
			Mach_BellFurnace* machP = new Mach_BellFurnace();
			*machP = *(static_cast<Mach_BellFurnace*> (machInfo.second));
			machsMapNew.insert(make_pair(machInfo.second->m_machCode, machP));
		}
		else if (airFurnaceSet.find(machInfo.second->m_machCode) != airFurnaceSet.end()) // ������¯
		{
			Mach_AirFurnace* machP = new Mach_AirFurnace();
			*machP = *(static_cast<Mach_AirFurnace*> (machInfo.second));
			machsMapNew.insert(make_pair(machInfo.second->m_machCode, machP));
		}
		else
		{
			Mach* machP = new Mach;
			*machP = *(machInfo.second);
			machsMapNew.insert(make_pair(machInfo.second->m_machCode, machP));
		}
	}
	return;
};

// ��Ԥ���Ų�ʱ--�����Ų�ʱ�䴰--��jobOrderOrig�е�job���Ų�ʱ�䴰������jobOrder�е�job
void resetJobsTemp(vector<pair<string, Job*>>& jobOrder, vector<pair<string, Job*>>& jobOrderOrig) {
	for (int i = 0; i < jobOrderOrig.size(); ++i)
	{
		/*
		cout << "thread-" << std::this_thread::get_id << "-" << jobOrder.size() << endl;
		cout << "thread-" << std::this_thread::get_id << "-" << jobOrderOrig.size() << endl;
		*/
		
		Job* jobP = jobOrder[i].second;
		Job* jobPOrig = jobOrderOrig[i].second;
		if (jobP->m_jobCode != jobP->m_jobCode)
			cout << "ERROR! the two jobs is different!";
		jobP->m_curMachIndex = jobPOrig->m_curMachIndex;
		jobP->m_allocatedTimeWin.assign(jobPOrig->m_allocatedTimeWin.begin(), jobPOrig->m_allocatedTimeWin.end());
	}
	return;
};

// ��Ԥ���Ų�ʱ--�����Ų�ʱ�䴰--��machsMapOrig�е�job���Ų�ʱ�䴰������machsMap�е�machine
void resetMachsMapTemp(map<string, Mach*>& machsMap, map<string, Mach*>& machsMapOrig) {
	for (auto& machInfo : machsMapOrig)
	{
		Mach* machPOrig = machInfo.second;
		Mach* machP = machsMap[machPOrig->m_machCode];
		if (CodeOfBellFurn == machInfo.second->m_machCode)  // ������¯
		{
			Mach_BellFurnace* machPOrigBell = (static_cast<Mach_BellFurnace*> (machPOrig));
			Mach_BellFurnace* machPBell = (static_cast<Mach_BellFurnace*> (machP));

			machPBell->m_timeLines.clear();
			machPBell->m_timeLines = vector<list<TimeWin>>(machPBell->m_numOfParallel);
			for (int i = 0; i < machPBell->m_numOfParallel; ++i) {
				g_mutex.lock();
				machPBell->m_timeLines[i].assign(machPOrigBell->m_timeLines[i].begin(), machPOrigBell->m_timeLines[i].end());
				g_mutex.unlock();
			}
		}
		else{
			g_mutex.lock();
			machP->m_allocatedTimeWin.assign(machPOrig->m_allocatedTimeWin.begin(), machPOrig->m_allocatedTimeWin.end());
			g_mutex.unlock();
		}
	}
	return;
};

// ��Ԥ���Ų�ʱ--��jobOrder�е�job���Ų�ʱ�䴰��0
void resetJobsToOrigin(vector<pair<string, Job*>>& jobOrder) {
	for (int i = 0; i < jobOrder.size(); ++i)
	{
		Job* jobP = jobOrder[i].second;
		jobP->m_curMachIndex = 0;
		jobP->m_allocatedTimeWin.clear();
	}
	return;
};

// ��Ԥ���Ų�ʱ--��machsMap�е��Ų�ʱ�䴰��0
void resetMachsMapToOrigin(map<string, Mach*>& machsMap) {
	for (auto& machInfo : machsMap)
	{
		Mach* machP = machInfo.second;
		if (CodeOfBellFurn == machInfo.second->m_machCode)  // ������¯
		{
			Mach_BellFurnace* machPBell = (static_cast<Mach_BellFurnace*> (machP));
			for (int i = 0; i < machPBell->m_numOfParallel; ++i)
				machPBell->m_timeLines[i].clear();
		}
		else {
			machP->m_allocatedTimeWin.clear();
		}
	}
	return;
};


// �򵥿���machine
void reInitMachsMapTemp(map<string, Mach*>& machsMapTemp, map<string, Mach*>& machsMap) {
	for (auto& machInfo : machsMapTemp)
	{
		*(machInfo.second) = *machsMap[(machInfo.first)];
	}
};

// �ͷ��ڴ棬�ͷ�new����job��mach-���߳��õ�
void releaseMemoryOfTmpJobsMachs(vector<pair<string, Job*>>& jobOrderTemp, map<string, Mach*>& machsMapTemp)
{
	// �ͷ�job
	for (auto& ele : jobOrderTemp) {
		Job* jobP = ele.second;
		delete jobP;
	}
	// �ͷ�mach
	for (auto& machInfo : machsMapTemp)
	{
		if (CodeOfBellFurn == machInfo.second->m_machCode)  // ������¯
		{
			Mach_BellFurnace* machP = new Mach_BellFurnace();
			*machP = *(static_cast<Mach_BellFurnace*> (machInfo.second));
			delete machP;
		}
		else if (airFurnaceSet.find(machInfo.second->m_machCode) != airFurnaceSet.end()) // ������¯
		{
			Mach_AirFurnace* machP = new Mach_AirFurnace();
			*machP = *(static_cast<Mach_AirFurnace*> (machInfo.second));
			delete machP;
		}
		else
		{
			Mach* machP = new Mach;
			*machP = *(machInfo.second);
			delete machP;
		}
	}
}

// ��ʼ���߳���Ϣ
vector<threadInfoOfLS*> initThreadsInfoOfLS(const int num_thread, vector<pair<string, Job*>>* jobOrderBefP, map<string, Mach*>* machsMapBefP) {
	vector<threadInfoOfLS*> threadInfos;
	for (int t = 0; t < num_thread; ++t) {
		threadInfoOfLS* threadInfoP = new threadInfoOfLS;
		threadInfos.emplace_back(threadInfoP);
		// �������룬����Ⱦɫ����ֵ
		threadInfoP->m_jobOrderTmp = { vector<pair<string, Job*>>(jobOrderBefP->size()) };
		initJobsTemp(threadInfoP->m_jobsMapTmp, threadInfoP->m_jobOrderTmp, (*jobOrderBefP));
		initMachsMapTemp(threadInfoP->m_machsMapTmp, (*machsMapBefP));
	}
	return threadInfos;
}

// �ͷ��߳���Ϣ
void releaseThreadsInfoOfLS(const int num_thread, vector<threadInfoOfLS*>& threadInfos){
	for (int t = 0; t < num_thread; ++t) {
		// �ͷ��ڴ棬�ͷ�new����job��mach-���߳��õ�
		threadInfoOfLS* threadInfoP = threadInfos[t];
		releaseMemoryOfTmpJobsMachs(threadInfoP->m_jobOrderTmp, threadInfoP->m_machsMapTmp);
		delete threadInfoP;
	}
}


// --------END OF--�����Ų���أ�����job��machine--------





// --------��������--------

void initJobsInfo(vector<string>& jobsCodeVec, vector<string>& machsCodeVec, map<string, Job*>& jobsMap, map<string, Mach*>& machsMap) {
	
	std::default_random_engine randm(7);  // 1
	//std::default_random_engine randm(27);  // 2

	int numOfJob = 40;


	std::uniform_int_distribution<int> dis_operation(5, 13);     // �����Ĺ���ĸ���
	std::uniform_int_distribution<int> dis_weight(4000, 20000);  // ����������
	std::uniform_real_distribution<double> dis2(0.0, 1.0);       // 


	ptime curTime = getCurTime();

	vector<double> widths{650, 850};   // job�Ŀ�ȹ��
	vector<string> alloyGrades(alloyGradeSet.begin(), alloyGradeSet.end());
	list<string> processLines;

	for(auto ele: processLines)
		cout << ele<< " " << endl;
	cout << endl;


	for (int i = 1; i <= numOfJob; ++i)
	{
		cout << "i=" << i << endl;
		processLines.clear();
		processLines.assign(machsSubSet.begin(), machsSubSet.end());
		for (auto& ele : machsSubSet) {
			processLines.emplace_back(ele);
		}
		processLines.emplace_back("BD-S006"); 
		//processLines.emplace_back("BD-S006");
		auto iterBegin = processLines.begin();
		auto iter = processLines.begin();

		Job* jobP = new Job;
		jobP->m_jobCode = "Job"+to_string(i);
		cout << "  jobCode=" << jobP->m_jobCode << endl;
		int numOfProcess = dis_operation(randm);

		if (numOfProcess > 10) {
			for (auto& ele : machsSubSet) {
				processLines.emplace_back(ele);
			}
			processLines.emplace_back("BD-S006");
		};

		cout << "  numOfProcess=" << numOfProcess << endl;
		for (int i = 0; i < numOfProcess; ++i) {
			int index2 = rand() % processLines.size();
			iter = processLines.begin();
			std::advance(iter, index2);
			string machCode = *iter;
			processLines.erase(iter);
			unsigned num_reentry = 1;
			for (pair<string, unsigned>& machInfo : jobP->m_proceMachs)
				num_reentry = (machCode == machInfo.first) ? (num_reentry + 1) : num_reentry;
			//std::cout<< jobP->m_jobCode<<", "<< machCode << std::endl;
			jobP->m_proceMachs.push_back(make_pair(machCode, num_reentry));
			cout << "    machCode=" << machCode <<"  num_reentry=" << num_reentry << endl;
		}

		int index = rand() % alloyGrades.size();  // alloyGradeSet
		jobP->m_alloyGrade = alloyGrades[index];                    // �Ͻ��ƺ�
		jobP->m_alloyType = getTypeFromGrade(jobP->m_alloyGrade);   // �Ͻ�����
		jobP->m_initialInfos.m_targetWeight = dis_weight(randm);    // ����650,850
		jobP->m_initialInfos.m_targetWidth = widths[rand() % widths.size()];

		jobP->m_initialInfos.m_targetThick = 0.0;
		jobP->m_initialInfos.m_targetLength = 0.0;
		jobP->m_initialInfos.m_targetInnerDia = 0.0;

		jobsCodeVec.push_back(jobP->m_jobCode);
		jobsMap[jobP->m_jobCode] = jobP;
	}
	//char ch; cin >> ch;

	for (string machCode:machsSubSet) 
	{
		if (CodeOfBellFurn == machCode)  // ������¯
		{
			Mach_BellFurnace* machP = new Mach_BellFurnace();
			machP->m_machCode = machCode;
			machsCodeVec.push_back(machP->m_machCode);
			machsMap[machP->m_machCode] = machP;
		}
		else if (airFurnaceSet.find(string(machCode)) != airFurnaceSet.end()) // ������¯
		{
			Mach_AirFurnace* machP = new Mach_AirFurnace();
			machP->m_machCode = machCode;
			machsCodeVec.push_back(machP->m_machCode);
			machsMap[machP->m_machCode] = machP;
			std::cout << "machCode" << machCode << std::endl;
			std::cout << "mach time" << machP->m_timeOfSwith << std::endl;
		}
		else
		{
			Mach* machP = new Mach(machCode);
			machP->m_machCode = machCode;
			machsCodeVec.push_back(machP->m_machCode);
			machsMap[machP->m_machCode] = machP;
			std::cout << "machCode" << machCode << std::endl;
			std::cout << "mach time" << machP->m_timeOfSwith << std::endl;
		}
	}

	// ���㹤���ʱ�䣬���㽻��ʱ��
	for (auto& ele: jobsMap) 
	{
		Job* jobP = ele.second;

		cout << "job: " << jobP->m_jobCode << endl;
		time_duration sumOfProcessT = double2timeDuration(0.0);
		for (int i = 0; i < jobP->m_proceMachs.size(); ++i) {
			Mach* machP = machsMap[jobP->m_proceMachs[i].first];
			time_duration processT = getProcessTime(machP, jobP, i);
			sumOfProcessT += processT;
			jobP->m_proceTimes.push_back(make_pair(jobP->m_proceMachs[i], processT));
			cout<<"  machCode= "<< machP->m_machCode << "  processT= " << processT << "  " << timeDuration2Double(processT) << endl;
		}
		//sumOfProcessT = double2timeDuration(timeDuration2Double(sumOfProcessT) * 3 / 2);

		jobP->m_startDateOfOrder = curTime;  // ��������ʱ��

		std::uniform_real_distribution<double> dis_dueTime(3/2*timeDuration2Double(sumOfProcessT), 2*timeDuration2Double(sumOfProcessT));   // �����Ľ���
		jobP->m_dueDateOfOrder = jobP->m_startDateOfOrder + double2timeDuration(dis_dueTime(randm));
		jobP->m_dueDateOfOrderStr = to_iso_extended_string(jobP->m_dueDateOfOrder);

		cout<< "job: "<< jobP->m_jobCode << " jobDueDate= " << jobP->m_dueDateOfOrderStr <<"  " << timeDuration2Double(sumOfProcessT) << endl;
		cout << endl;
	}
}

// --------END OF--��������--------





// --------����--------

pair<Job*, Job*> splitJob(Job* jobP, int splitPosition) {
	Job* jobP_front = new Job;
	Job* jobP_rear = new Job;

	jobP_front->m_jobCode = jobP->m_jobCode + "_front";
	jobP_front->m_alloyGrade = jobP->m_alloyGrade;
	jobP_front->m_alloyType = jobP->m_alloyType;

	jobP_rear->m_jobCode = jobP->m_jobCode + "_rear";
	jobP_rear->m_alloyGrade = jobP->m_alloyGrade;
	jobP_rear->m_alloyType = jobP->m_alloyType;

	jobP_front->m_initialInfos.m_targetThick = jobP->m_initialInfos.m_targetThick;
	jobP_front->m_initialInfos.m_targetWidth = jobP->m_initialInfos.m_targetThick;
	jobP_front->m_initialInfos.m_targetLength = jobP->m_initialInfos.m_targetThick;
	jobP_front->m_initialInfos.m_targetWeight = jobP->m_initialInfos.m_targetThick;
	jobP_front->m_initialInfos.m_targetInnerDia = jobP->m_initialInfos.m_targetThick;

	jobP_rear->m_initialInfos.m_targetThick = getCurThick(jobP, splitPosition);
	jobP_rear->m_initialInfos.m_targetWidth = getCurWidth(jobP, splitPosition);
	jobP_rear->m_initialInfos.m_targetLength = getCurLength(jobP, splitPosition);
	jobP_rear->m_initialInfos.m_targetWeight = getCurWeight(jobP, splitPosition);
	jobP_rear->m_initialInfos.m_targetInnerDia = getCurInnerDia(jobP, splitPosition);

	for (int indexMach = 0; indexMach <= jobP->m_proceMachs.size(); ++indexMach)
	{
		if (indexMach < splitPosition)
		{
			jobP_front->m_proceMachs.push_back(jobP->m_proceMachs[indexMach]);
			jobP_front->m_proceTargets.push_back(jobP->m_proceTargets[indexMach]);
			jobP_front->m_proceTimes.push_back(jobP->m_proceTimes[indexMach]);
		}
		else
		{
			jobP_rear->m_proceMachs.push_back(jobP->m_proceMachs[indexMach]);
			jobP_rear->m_proceTargets.push_back(jobP->m_proceTargets[indexMach]);
			jobP_rear->m_proceTimes.push_back(jobP->m_proceTimes[indexMach]);
		}
	}

	//curJobP->m_startDateOfOrder;

	return(make_pair(jobP_front, jobP_rear));
};

// --------END OF--��������--------



void main()
{
	//unsigned seed = 50;
	//unsigned seed = 500;
	unsigned seed = 5088;

	srand(seed);


	vector<string> jobsCodeVec;     // vector<job code>
	vector<string> machsCodeVec;    // vector<machine code>
	map<string, Job*> jobsMap;      // map<jobCode, Job*>
	map<string, Mach*> machsMap;    // map<machCode, Mach*>
	//ptime curTime = from_iso_string("20200610T000000");  // ��ǰʱ��
	//ptime curTime = from_iso_string("20200501T0000000");
	//ptime curTime = from_iso_string("20220101T0000000");

	MYSQL* mysql = new MYSQL;
	if(true)
		initJobsInfo(jobsCodeVec, machsCodeVec, jobsMap, machsMap);              // ���ɰ���
	else 
		initialByDatabase(mysql, jobsCodeVec, machsCodeVec, jobsMap, machsMap);  // �����ݿ�������ݳ�ʼ����Ϣ

	// ��ӡ�Ƴ�
	printProcessLine(jobsMap, machsMap);
	

	// ��ӡjobCodeVec
	map<string, vector<Job*>> jobsWithBell;  // map< �ƺ�, vector<Job*> >
	map<string, pair<Job*, int>> jobsWithBells;  // map< �ƺ�, pair<Job*, int> > 
	vector<BellBatch*> jobsBatch;  // pair<�ƺ�, vector<Job*>>
	initialJobsBatch(jobsMap, machsMap, jobsCodeVec, jobsWithBell, jobsWithBells, jobsBatch);

	// ��ӡ����¯����ͳ�Ƹ���
	printBellCount(jobsMap);

	// Ϊjob��machԤ�ȷ���ʱ�䴰�ռ�
	allocateTimeWinSpace(jobsMap, machsMap);


	// myInitialization(jobsCodeVec, machsCodeVec, jobsMap, machsMap);
	vector<pair<Job*, ptime>> jobsWithDueDate;               // vector<pair<Job*, ��ֹʱ��>>
	vector<pair<Job*, time_duration>> jobsWithTotalProTime;  // vector<pair<Job*, �ܼӹ�ʱ��>>
	vector<pair<Job*, time_duration>> jobsWithSlackTime;  // vector<pair<Job*, �ɳ�ʱ��>>   �ɳ�ʱ�䣺job��ʣ��ʱ�䣨��ǰʱ����뽻���ڵ�ʱ�䣩���job����ļӹ�ʱ��֮��
	jobsWithDueDate.reserve(jobsMap.size());
	jobsWithTotalProTime.reserve(jobsMap.size());
	jobsWithSlackTime.reserve(jobsMap.size());

	cout <<"  jobsWithDueDate.size()=" << jobsWithDueDate.size() << endl;
	cout << "  jobsMap.size()=" << jobsMap.size() << endl;

	// ��ʼ����ͬ��job vector������ʱ�䣬�ɳ�ʱ�䣬��ֹʱ��
	initialJobVecs(jobsMap, machsMap, jobsWithDueDate, jobsWithTotalProTime, jobsWithSlackTime);

	//cout << "aaaaaaaaaaaaa" << endl;
	// ��ӡ���ڡ�����ʱ�䡢�ɳ�ʱ��
	printDueProSlackTime(jobsWithDueDate, jobsWithTotalProTime, jobsWithSlackTime);


	

	/*
	vector<vector<pair<Job*, int>>> batchOfBell;  // vector<vector<pair<Job*, ��־״̬>>, �ƺ�>  0:δ����; 1:�ڵȴ�; 2:������
	vector<pair<string, Job*>> jobOrder;
	vector<pair<vector<pair<Job*, double>>, string>> jobsWithBellFurn;  // vector<pair<vector<pair<Job*, ���>>, �ƺ�>> ͬһ�ƺŵ���һ��
	for (int i = 0; i < jobsWithSlackTime.size(); ++i)  // һ��һ���ط���job
	{
		Job* curJobP = jobsWithSlackTime[i].first;
		jobOrder.push_back(make_pair(curJobP->m_jobCode, curJobP));
		bool isIncludeBell = false;
		int indexOfMach=0;
		for (pair<string, unsigned>& machInfoOfCurJob : curJobP->m_proceMachs)   // ����ĳ��job������machine
		{
			if (CodeOfBellFurn == machInfoOfCurJob.first)
			{
				isIncludeBell = true;
				break;
			}
			++indexOfMach;
		}
		bool isPutIn=false;  //�Ƿ��Ѿ�����
		if (isIncludeBell)
		{
			for (auto& jobsSameAlloyGrade : jobsWithBellFurn)
			{
				if (jobsSameAlloyGrade.second == curJobP->m_alloyGrade) {
					jobsSameAlloyGrade.first.push_back(make_pair(curJobP, curJobP->m_getCurWidth(indexOfMach)));
					isPutIn = true;
				}
			}
			if (!isPutIn)
			{
				jobsWithBellFurn.push_back(make_pair(vector<pair<Job*, double>> {make_pair(curJobP, curJobP->m_getCurWidth(indexOfMach))}, 
					curJobP->m_alloyGrade));
			}
		}
	}

	cout <<"yuyuyu" <<endl;
	cout << " �ƺŸ���=" <<jobsWithBellFurn.size() << endl;

	// ����
	int batchCount = 0;
	for (auto &jobsOfSameAlloyGrade:jobsWithBellFurn)  // ��ÿһ���ƺ�
	{
		string alloyGrade = jobsOfSameAlloyGrade.second;
		vector<pair<Job*, double>> tempBatch;
		cout << "batchCount=" << batchCount << endl;
		cout << " numOfSame=" << jobsOfSameAlloyGrade.first.size() << endl;
		batchCount++;
		int num = 0;
		for (auto it=jobsOfSameAlloyGrade.first.begin(); it != jobsOfSameAlloyGrade.first.end(); ++it, ++num)  // һ��һ���ط���job
		{
			cout << "  num=" << num << endl;
			cout << "  tempBatchSize()=" <<tempBatch.size()<<endl;
			Job* curJobP = it->first;
			double widthOfCurJob = it->second;
			if(tempBatch.size()<=1) //1��������
				tempBatch.push_back(*it);
			else //2��
			{
				if ((widthOfCurJob <850))
					tempBatch.push_back(*it);  // ����
				else
				{
					pair<Job*, double> tempJob;
					tempJob.first = tempBatch[1].first;
					tempJob.second = tempBatch[1].second;
					tempBatch.pop_back();
					tempBatch.push_back(make_pair(curJobP, widthOfCurJob));
					// ����
					vector<pair<Job*, int>> aBatch;
					aBatch.push_back(make_pair(tempBatch[0].first, 0));
					aBatch.push_back(make_pair(tempBatch[1].first, 0));
					batchOfBell.push_back(aBatch);
					tempBatch.clear();

					tempBatch.push_back(tempJob);
				}
			}
			if ((1 == tempBatch.size()) ||
				((2 == tempBatch.size()) && (tempBatch[0].second < 850) && (tempBatch[1].second < 850)))
					continue;
			vector<pair<Job*, int>> aBatch;
			for(auto& batch:tempBatch) aBatch.push_back(make_pair(batch.first, 0));
			batchOfBell.push_back(aBatch);
			tempBatch.clear();
		}
		cout << num << endl;
		if (tempBatch.size() != 0)
		{
			vector<pair<Job*, int>> aBatch;
			for (auto& batch : tempBatch) aBatch.push_back(make_pair(batch.first, 0));
			batchOfBell.push_back(aBatch);
		}
	}

	for (auto& sameGrade:batchOfBell)
	{
		for (auto& jobInfo:sameGrade)
		{
			cout << "  grade=" <<jobInfo.first->m_alloyGrade << 
				"  jobCode="<<jobInfo.first->m_jobCode <<
				"  flag="<<jobInfo.second<< endl;
		}
		cout << endl;
	}


	scheduleByJobOrder4(jobOrder, jobsMap, machsMap, batchOfBell, mysql, 1);
	*/





	/*
	vector<pair<string, Job*>> jobWithBell;  // ������¯��job
	std::cout << "jobsWithSlackTime.size()" << jobsWithSlackTime.size() << std::endl;
	vector<pair<string, Job*>> jobOrder;

	for (int i = 0; i < 3; ++i)  // һ��һ���ط���job //��7����ѭ����
	//for (int i = 0; i < jobsWithSlackTime.size(); ++i)  // һ��һ���ط���job //
	//for (int i = jobsWithTotalProTime.size()-1; i>=0; --i)
	//for (auto& jobInfo : jobsMap)
	{
		if (11 == i) continue;
		//Job* curJobP = jobInfo.second;
		Job* curJobP = jobsWithSlackTime[i].first;
		cout << curJobP->m_jobCode << endl;
		if (0 & ("0119331020-0-0" == curJobP->m_jobCode )) { // curJobP->m_jobCXode=0118381020-P-0-0
			// 0118381020-P-0-0ǰ  // 0703061100-0-0   0118381020-P-0-0
			double a;
			cout << "curJobP->m_jobCXode=" <<curJobP->m_jobCode << endl;
			cout << "i="<<i << endl;
			cout << "\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
			scanf("%d", &a);
			printf("a=%d", a);
			scanf("%d", &a);
		}
		double a;
		//scanf("%d", &a);
		//printf("a=%d", a);
		//scanf("%d", &a);
		jobOrder.push_back(make_pair(curJobP->m_jobCode, curJobP));

	}

	//std::cout << "lastJobWithBell=" << lastJobWithBell << std::endl;
	cout << "uiuiui--" << endl;
	double objectVals = scheduleByJobOrder3(jobOrder, jobsMap, machsMap, mysql, true);
	//	pair<double, double> objectVals = scheduleByJobOrder2(jobOrder, jobsMap, machsMap, jobWithBell, lastJobWithBell, true);
	*/





	cout << "Solving..." << endl;
	Sleep(2);

	//rule_Method(jobsMap, machsMap, mysql, jobsWithDueDate, jobsWithTotalProTime, jobsWithSlackTime);

	//NEH_Method(jobsMap, machsMap, mysql, jobsWithDueDate, jobsWithTotalProTime, jobsWithSlackTime);

	//GA_Method(machsMap, mysql, jobsWithDueDate, jobsWithTotalProTime, jobsWithSlackTime);

	IPG_Method(machsMap, mysql, jobsWithDueDate, jobsWithTotalProTime, jobsWithSlackTime);


	for (auto& jobInfo : jobsMap) delete jobInfo.second;
	for (auto& machInfo : machsMap) delete machInfo.second;





	/*
	vector<pair<string, Job*>> jobWithBell;  // ������¯��job
	int lastJobWithBell(0);
	map<string, int> bellCountRecorder;
	map<string, int> positionRecord;

	std::cout << "jobsWithSlackTime.size()" << jobsWithSlackTime.size() << std::endl;
	vector<pair<string, Job*>> jobOrder;

	for (int i = 0; i < jobsWithSlackTime.size(); ++i)  // һ��һ���ط���job //
	//for (int i = jobsWithTotalProTime.size()-1; i>=0; --i)
	//for (auto& jobInfo : jobsMap)
	{
		//Job* curJobP = jobInfo.second;
		Job* curJobP = jobsWithSlackTime[i].first;

		
		//bellCountRecorder.insert(make_pair(curJobP->m_jobCode, 0));
		//int machIndex=0;
		//for (pair<string, unsigned>& machInfoOfCurJob : curJobP->m_proceMachs)   // ����ĳ��job������machine
		//{
		//	if (CodeOfBellFurn == machInfoOfCurJob.first)
		//		bellCountRecorder[curJobP->m_jobCode] += 1;
		//	if (bellCountRecorder[curJobP->m_jobCode] == 1)
		//		positionRecord.insert(make_pair(curJobP->m_jobCode, machIndex));
		//	++machIndex;
		//}
		//if (positionRecord.find(curJobP->m_jobCode) != positionRecord.end())
		//{
		//	pair<Job*, Job*> splitedJobs = splitJob(curJobP, positionRecord[curJobP->m_jobCode]);
		//	jobOrder.push_back(make_pair(splitedJobs.first->m_jobCode, splitedJobs.first));
		//	jobOrder.push_back(make_pair(splitedJobs.second->m_jobCode, splitedJobs.second));
		//}
		//else
		//{
		//	jobOrder.push_back(make_pair(curJobP->m_jobCode, curJobP));
		//}


		//for (auto& machInfo : curJobP->m_proceMachs)
		//{
		//	if (CodeOfBellFurn == machInfo.first)
		//	{
		//		jobWithBell.push_back(make_pair(curJobP->m_jobCode, curJobP));
		//		lastJobWithBell = i;
		//		std::cout << "lastJobWithBell=" << lastJobWithBell << machInfo.first << std::endl;
		//		break;
		//	}
		//}
		//

		jobOrder.push_back(make_pair(curJobP->m_jobCode, curJobP));

	}

	//std::cout << "lastJobWithBell=" << lastJobWithBell << std::endl;
	pair<double, double> objectVals = scheduleByJobOrder(jobOrder, jobsMap, machsMap, mysql, true);
	//	pair<double, double> objectVals = scheduleByJobOrder2(jobOrder, jobsMap, machsMap, jobWithBell, lastJobWithBell, true);

	cout << "objectVals(makespan)=" << objectVals.second << ";  due time=" << objectVals.first << endl;
	*/
	
	
	
}












