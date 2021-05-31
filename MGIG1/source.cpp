#include<head.h>
ptime curTime = from_iso_string("20200610T000000");

set<string> rollingmachFrequ{ "BD-S003","BD-S005","BD-S025","BD-S009","BD-S010","BD-S011" };  // ������; getStatus()��
// BD-S003	��������ʽ������; BD-S005	������; BD-S025	�Ĺ�������; 
// BD-S009	ɭ����20��������; BD-S010	20��������; BD-S011	4��������

set<string> cut_StretchmachFrequ{ "BD-S018","BD-S015",
					"BD-S016","BD-S019","BD-S020","BD-S021" };  // ���ݼ��������; getStatus()��
// BD-S018	1250�������; BD-S015	650�����; 
// BD-S016	1250�ݼ�; BD-S019	650���ݼ�; BD-S020	650���ݼ�; BD-S021	350�ݼ�

set<string> washmachFrequ{ "BD-S012","BD-S013","BD-S014" };
// BD-S012	1250��ϴ����; BD-S013	650��ϴ����; BD-S014	��650��ϴ����; getStatus()��

set<string> airFurnaceSet{ "BD-S024","BD-S007","BD-S008" };
//BD-S024  WSP����¯;    BD-S007  1250����ʽ�˻�¯��;    BD-S008  650����ʽ�˻�¯��

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
	make_pair("TU2", "copper"),};

string getTypeFromGrade(string alloyGrade)
{
	return alloyMap[alloyGrade];
}

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


// ����mysql���ݿ�
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

// �ж�_number�Ƿ���_aRange(��ʽΪ"[0.1,3.4]")�ķ�Χ��
bool isInRange(string _aRange, double _number)
{
	if (("null" == _aRange) ||("NULL" == _aRange) 
		|| ("Null" == _aRange) || ("" == _aRange)
		|| (_number<=0.0))
		return true;
	size_t site = _aRange.find(",", 0);
	size_t len = _aRange.length();

	if ((0<site)&&(site<len)
		&&(3<len)&&(len<30))  // �������Ƴ��ȣ���Ȼ��ȷ��
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
			std::cout<<"ERROR with '" << _aRange <<"'�� ���ܽ����� "<<std::endl;
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
	
	if (rollingmachFrequ.count(machCode) > 0) //������
	{
		if ("BD-S003" == machCode) // ��BD-S003	  ��������ʽ������
			return "hot_rolling";
		if (("BD-S009" == machCode)||("BD-S010" == machCode)) // ��BD-S009  ɭ����20��������
		{
			return "finished_product"; //�жϺ��滹��û��������û����Ϊ������Ʒ������Ϊ��Ʒ
			"intermediate_material";
		}
		// BD-S003	��������ʽ������, BD-S005	������; BD-S025	�Ĺ�������; 
		// BD-S009	ɭ����20��������; BD-S010	20��������; BD-S011	4��������
		return "";
	}
		
	if (cut_StretchmachFrequ.count(machCode) > 0) //�Ǽ��л�����
	{
		// �жϺ����ǲ���ֻ�а�װ��û�й�������Ϊ��Ʒ(finished_product)�����������¹����Ƿ��Ǻ��(cross_fedding);
		// ��װ��: BD-S022  ȫ�Զ���װ��, BD-S023  ���װ�װ��
		// ���¹����Ǻ��������Ϊ�������(deburring)
		if ((machIndex + 1) == jobP->m_proceMachs.size())  // BD-S016	1250�ݼ�; BD-S020	650���ݼ�;
			return "finished_product";
		if(("BD-S022" ==jobP->m_proceMachs[machIndex + 1].first)
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

	if (washmachFrequ.count(machCode) > 0) //����ϴ
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
time_duration getProcessTime(Mach* machP, Job * jobP, unsigned machIndex)
{
	//return time_duration(2, 0, 0, 0);

	ProcessTargets const& processTargets = jobP->m_proceTargets[machIndex].second;  //??����
	double processT(0);
	// bool isRollingMach(rollingmachFrequ.count(machP->m_machCode) > 0);
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
	if (0 == processT)
		std::cout<<"Error with capacity! Cannot find a matched capacity of job '"
		<<jobP->m_jobCode<<"' on machine "<< machP->m_machCode <<"!"<<std::endl;

	//std::cout << "machCode=" << machP->m_machCode << "; jobCode="<<jobP->m_jobCode <<"; cap="<<processT<< std::endl;
	return double2timeDuration(processT);
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
		std::cout<<"Error with str-'"<<str<< "'! Str is NULL��"<<std::endl;
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

// ����Ƚ�pair<Job*, ptime>
bool myCmpBy_ptime(pair<Job*, ptime> _a, pair<Job*, ptime> _b)
{
	return _a.second < _b.second;
};

// ����Ƚ�pair<Job*, time_duration>
bool myCmpBy_time_duration(pair<Job*, time_duration> _a, pair<Job*, time_duration> _b)
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

// ����¯���Ƿ���Լ�������¯��ĳ��timeWin
bool canAddToTimeWin(string jobCode, unsigned machIndexOfJob, TimeWin &timeWin, Mach_BellFurnace& curMachBell)
{
	if (curMachBell.m_RuleForFurnWithWidth.second.second == timeWin.first.size()) // �Ѿ���3���ˣ����ܲ���
		return false;
	else if (curMachBell.m_RuleForFurnWithWidth.second.first == timeWin.first.size())  // �Ѿ���2����
	{
		for (auto& jobInfo: timeWin.first)
		{
			Job* jobP = jobInfo.first;
			double curWidth(0); //job�ĵ�ǰ���

			for (int i = machIndexOfJob; i >= 0; --i)
			{
				if (jobP->m_proceTargets[i].second.m_targetWidth > 0.0) // �ӵ�ǰ����ǰ��Ŀ���ȴ���0��ֵ 
				{
					curWidth = jobP->m_proceTargets[i].second.m_targetWidth;
					break;
				}
			}
			if (curWidth <= 0.0)   // ��Ŀ���ȶ���0 
				curWidth = jobP->m_initialInfos.m_targetWidth;
			if ((curWidth >= curMachBell.m_RuleForFurnWithWidth.first)//??����job�Ŀ�ȹ���ĸ���
				||(jobCode ==jobP->m_jobCode))  // ������job�Լ�
				return false;
		}
	}
	return true;
}

// airFurnaceSet
bool getIsSwitch(Job& curJob, Mach& curMach, Job& otherJob)  // �Ƿ���Ҫ�л�
{
	if (airFurnaceSet.find(curMach.m_machCode) != airFurnaceSet.end())  // ������¯��Ļ���
	{
		//cout << otherJob.m_jobCode <<" "<< curJob.m_jobCode
			//<<" otherJob.m_alloyType=" << otherJob.m_alloyType<<"curJob.m_alloyType"<< curJob.m_alloyType << endl;
		if (otherJob.m_alloyType == curJob.m_alloyType) return false;
		else return true;
	}
	return false;
};

// ��ĳ��������ĳ������¯
bool  insertJob(Job& curJob, Mach_AirFurnace& curMach, unsigned machIndexOfJob, map<string, Job*> &jobsMap)
{
	time_duration& timeDuration = curJob.m_proceTimes[machIndexOfJob].second;
	time_duration& switchT = double2timeDuration(curMach.m_timeOfSwith);
	
	bool isInserted(false);
	unsigned IndexOfTimeWin(0);  // machine�Ĳ��������

	ptime readyTimeForOrder;  // job�����ڸû����Ͽ�ʼ�ӹ���ʱ��
	time_period timeWinToInsert(curJob.m_startDateOfOrder, timeDuration);  //Ҫ�����ʱ�䴰
	bool isSwitchPre;  // ����ǰ��Ļ������м��Ƿ���Ҫת��ʱ��
	if (0 == machIndexOfJob)  // �����job�ĵ�һ��machine
		readyTimeForOrder = curJob.m_startDateOfOrder;
	else
		readyTimeForOrder = (curJob.m_allocatedTimeWin.end() - 1)->second.end();
	//if ("BD-S008" == curMach.m_machCode) cout << "yyyyyyyyyyyyy" << endl;

	if (0 == curMach.m_allocatedTimeWin.size())  // �����machine�ϵĵ�һ��job
	{
		timeWinToInsert = time_period(readyTimeForOrder, timeDuration);
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
			//cout << "isSwitchNext=" << isSwitchNext << endl;
			if (left_TimeWin < readyTimeForOrder + timeDuration + (isSwitchNext ? switchT : double2timeDuration(0.0)))  // �ɼӹ�ʱ���ʱ�䴰��ʼʱ��֮��һ���Ų���
				continue;
			
			if (curMach.m_allocatedTimeWin.cbegin() == timeInfo_iter)  // ��һ��time window
			{
				timeWinToInsert = time_period(readyTimeForOrder, timeDuration);
				isInserted = true;
				break;
			}
			else  // isSwitchPre*switchT
			{
				auto timeInfo_iter2 = timeInfo_iter - 1;
				ptime right_Pre = timeInfo_iter2->second.end();

				isSwitchPre = getIsSwitch(curJob, curMach, *jobsMap[timeInfo_iter2->first.first]);

				if ((readyTimeForOrder - (isSwitchPre ? switchT : double2timeDuration(0.0)) < right_Pre) &&
					(right_Pre + timeDuration + (isSwitchPre?switchT: double2timeDuration(0.0))
						+ (isSwitchNext ? switchT : double2timeDuration(0.0)) <= left_TimeWin))
				{
					timeWinToInsert = time_period(right_Pre + (isSwitchPre ? switchT : double2timeDuration(0.0)), timeDuration);
					isInserted = true;
					break;
				}
				if (right_Pre <= readyTimeForOrder - (isSwitchPre ? switchT : double2timeDuration(0.0)))
				{
					timeWinToInsert = time_period(readyTimeForOrder, timeDuration);
					isInserted = true;
					break;
				}
			}
		}  // END OF ����curMach.m_allocatedTimeWin

		if (false == isInserted)
		{
			ptime right_Pre = (curMach.m_allocatedTimeWin.end() - 1)->second.end();
			isSwitchPre = getIsSwitch(curJob, curMach, *jobsMap[(curMach.m_allocatedTimeWin.end() - 1)->first.first]);
			timeWinToInsert = time_period(right_Pre > readyTimeForOrder - (isSwitchPre ? switchT : double2timeDuration(0.0)) ? 
				right_Pre+(isSwitchPre ? switchT : double2timeDuration(0.0)) : readyTimeForOrder, timeDuration);
			isInserted = true;
		}
	}
	//if ("BD-S008" == curMach.m_machCode) cout << "uuuuuuuuuuuuu" << endl;
	curJob.m_curMachIndex = machIndexOfJob;
	curJob.m_allocatedTimeWin.push_back(make_pair(curJob.m_proceMachs[machIndexOfJob], timeWinToInsert));

	curMach.m_allocatedTimeWin.insert(curMach.m_allocatedTimeWin.begin() + IndexOfTimeWin,
		make_pair(make_pair(curJob.m_jobCode, curJob.m_proceMachs[machIndexOfJob].second), timeWinToInsert));
	return true;
};;


// ��ĳ��������ĳ���������ز���ʱ�䴰�Ͳ�������
pair<unsigned, bool> preInsertJobToMach(Job& curJob, Mach_BellFurnace& curMach, unsigned machIndexOfJob, vector<TimeWin>& timeline, time_period& timeWinResult)
{
	time_duration& timeDuration = double2timeDuration(curMach.m_proceTimePerFurn);  //����¯ʱ��Ϊ30Сʱ
	//time_duration timeOfSwith = double2timeDuration(curMach.m_timeOfSwith);  // �����µ�һ¯��ת��ʱ��
	//׼��ʱ�����һ¯�ļӹ����ʱ�䣬�Ƿ�Ҫ����ת��ʱ�䣬��Ҫ�����ף���??
	//   ��job������׼��ʱ�����Ҫת��ʱ�䣻��machine��������һ¯����ʱ�����Ҫת��ʱ�䣡

	unsigned IndexOfTimeWin(0);  // machine��ʱ���߲��������
	// using TimeWin = pair<vector<pair<Job*, unsigned>>, time_period>;  // pair< vector<pair<Job*, ��job�ڼ�������>>, job�����ʱ�䴰>

	bool flag_NewFurn(false);  // ��ʾ�Ƿ��¿�һ¯
	bool isInserted(false);

	ptime readyTimeForOrder;  // job�����ڸû����Ͽ�ʼ�ӹ���ʱ��
	//time_period timeWinToInsert(curJob.m_startDateOfOrder, timeDuration);  //Ҫ�����ʱ�䴰
	if (0 == machIndexOfJob)  // �����job�ĵ�һ��machine
		readyTimeForOrder = curJob.m_startDateOfOrder;
	else
		readyTimeForOrder = (curJob.m_allocatedTimeWin.end() - 1)->second.end();

	//cout << " timeline.size()=" << timeline.size()
	//	<< "  readyTimeForOrder="<<to_iso_extended_string(readyTimeForOrder)<<endl;

	if (0 == timeline.size())  // �����machine�ϵĵ�һ��job
	{
		timeWinResult = time_period(readyTimeForOrder+double2timeDuration(5.0), timeDuration); //??����!!!!!!!!!
		flag_NewFurn = true;  // �¿�һ¯
		IndexOfTimeWin = 0;
		isInserted = true;
	}
	else
	{
		// ����timeline
		for (vector<TimeWin>::iterator timeInfo_iter = timeline.begin();
			timeInfo_iter != timeline.end(); ++timeInfo_iter, ++IndexOfTimeWin)
		{
			ptime left_TimeWin = timeInfo_iter->second.begin();
			ptime right_TimeWin = timeInfo_iter->second.last();
			//cout << to_iso_extended_string(left_TimeWin)
				//<<"  IndexOfTimeWin="<< IndexOfTimeWin << endl;

			if (left_TimeWin < readyTimeForOrder + timeDuration)  // �ɼӹ�ʱ���ʱ�䴰��ʼʱ��֮��һ���Ų����µ�һ¯
			{
				if ((readyTimeForOrder < left_TimeWin) && canAddToTimeWin(curJob.m_jobCode, machIndexOfJob, *timeInfo_iter, curMach))
				{
					timeWinResult = timeInfo_iter->second;
					flag_NewFurn = false; // ���¿�һ¯
					isInserted = true;
					break;
				}
				continue;
			}

			if (timeline.cbegin() != timeInfo_iter)  // ��һ��time window
			{
				auto timeInfo_iter2 = timeInfo_iter - 1;
				ptime right_Pre = timeInfo_iter2->second.end();
				if ((readyTimeForOrder < right_Pre) && (canAddToTimeWin(curJob.m_jobCode, machIndexOfJob, *timeInfo_iter, curMach))
					&& (readyTimeForOrder < left_TimeWin))
				{
					timeWinResult = timeInfo_iter->second;
					flag_NewFurn = false;    // ���¿�һ¯
					isInserted = true;
					break;
				}

			}
		}  // END OF ����timeline

		if (!isInserted)
		{
			IndexOfTimeWin = 0;
			// ����timeline
			for (vector<TimeWin>::iterator timeInfo_iter = timeline.begin();
				timeInfo_iter != timeline.end(); ++timeInfo_iter, ++IndexOfTimeWin)
			{
				ptime left_TimeWin = timeInfo_iter->second.begin();
				ptime right_TimeWin = timeInfo_iter->second.last();

				if (left_TimeWin < readyTimeForOrder + timeDuration)  // �ɼӹ�ʱ���ʱ�䴰��ʼʱ��֮��һ���Ų����µ�һ¯
					continue;

				if (timeline.cbegin() == timeInfo_iter)  // ��һ��time window
				{
					//std::cout << "**************** " << std::endl;
					timeWinResult = time_period(readyTimeForOrder, timeDuration);
					flag_NewFurn = true;    // �¿�һ¯
					isInserted = true;
					break;
				}
				else
				{
					auto timeInfo_iter2 = timeInfo_iter - 1;
					ptime right_Pre = timeInfo_iter2->second.end();
					if ((readyTimeForOrder < right_Pre) && (right_Pre + timeDuration <= left_TimeWin))
					{
						timeWinResult = time_period(right_Pre, timeDuration);
						flag_NewFurn = true;    // �¿�һ¯
						isInserted = true;
						break;
					}
					if (right_Pre <= readyTimeForOrder)
					{
						timeWinResult = time_period(readyTimeForOrder, timeDuration);
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
			ptime rightWin = (timeline.end() - 1)->second.end();
			timeWinResult = time_period((readyTimeForOrder < rightWin) ? rightWin : readyTimeForOrder, timeDuration);
			flag_NewFurn = true;    // �¿�һ¯
			isInserted = true;
		}
	}
	//std::cout << "curJob=" << curJob.m_jobCode <<" 777777"<< std::endl;
	//std::cout << "      timeWinResult=" << to_iso_extended_string(timeWinResult.begin()) << endl;
	//<<"--" << to_iso_extended_string(timeWinResult.last()) << std::endl;
	return make_pair(IndexOfTimeWin, flag_NewFurn);
};

// ��ĳ������������¯��ĳ��ƽ�л�
bool  insertJob(Job& curJob, Mach_BellFurnace& curMach, unsigned machIndexOfJob)
{
	time_duration& timeDuration = curJob.m_proceTimes[machIndexOfJob].second;

	pair<unsigned, string> SchedulInfo;  // pair<ʱ�䴰λ��, ��ʾ�Ƿ��¿�һ¯>  "newFurnace":�¿�һ¯; "addToExist":�¿�һ¯

	unsigned Index_parallelMach = 0;
	unsigned Index_final = Index_parallelMach;
	time_period timeWinFinal(curJob.m_startDateOfOrder, timeDuration);
	pair<int, bool> resultOfPreInsert(make_pair(-1, false));   // pair<TimeWin��λ��, �Ƿ�Ҫ�¿�һ¯>
	pair<int, bool> resultOfFinal(make_pair(-1, false));   // pair<TimeWin��λ��, �Ƿ�Ҫ�¿�һ¯>
	//if ("0118741020-0-0" == curJob.m_jobCode) cout << "eeeeeeeeeeeeee" << endl;
	//std::cout << "size(m_numOfParallel) == "<< curMach.m_timeLines.size()<<std::endl;

	pair<pair<int, int>, time_period> newFurn = (make_pair(pair<unsigned, unsigned>(make_pair(-1, -1)), time_period(curJob.m_startDateOfOrder, timeDuration))); // pair<pair< �ڼ���timeline, ����λ��>, ʱ�䴰>
	pair<pair<int, int>, time_period> notNewFurn = (make_pair(pair<unsigned, unsigned>(make_pair(-1, -1)), time_period(curJob.m_startDateOfOrder, timeDuration))); // pair<pair< �ڼ���timeline, ����λ��>, ʱ�䴰>

	for (auto& timeline : curMach.m_timeLines)
	{
		time_period timeWinResult(curJob.m_startDateOfOrder, timeDuration);

		resultOfPreInsert = preInsertJobToMach(curJob, curMach, machIndexOfJob, timeline, timeWinResult);

		if (resultOfPreInsert.second) // ���¿�1¯
		{
			if (newFurn.first.first < 0.0)
				newFurn = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
			else if (newFurn.second.begin() > timeWinResult.begin())
				newFurn = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
		}
		else // �����¿�1¯
		{
			if (notNewFurn.first.first < 0.0)
				notNewFurn = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
			else if (notNewFurn.second.begin() > timeWinResult.begin())
				notNewFurn = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
		}
		cout << "  Index_parallelMach=" << Index_parallelMach << endl;
		++Index_parallelMach;
	}

	int insertPosition(-1);  // ����λ��
	bool isNew;
	if ((newFurn.first.second >= 0) && (notNewFurn.first.second >= 0))
	{
		if (notNewFurn.second.begin() < newFurn.second.begin() + double2timeDuration(20.0))
		{
			Index_final = notNewFurn.first.first;
			insertPosition = notNewFurn.first.second;
			timeWinFinal = notNewFurn.second;
			isNew = false;
		}
		else
		{
			Index_final = newFurn.first.first;
			insertPosition = newFurn.first.second;
			timeWinFinal = newFurn.second;
			isNew = true;
		}

	}
	else if (newFurn.first.second >= 0)
	{
		Index_final = newFurn.first.first;
		insertPosition = newFurn.first.second;
		timeWinFinal = newFurn.second;
		isNew = true;
	}
	else
	{
		Index_final = notNewFurn.first.first;
		insertPosition = notNewFurn.first.second;
		timeWinFinal = notNewFurn.second;
		isNew = false;
	}

	curJob.m_curMachIndex = machIndexOfJob;
	curJob.m_allocatedTimeWin.push_back(make_pair(curJob.m_proceMachs[machIndexOfJob], timeWinFinal));

	vector<TimeWin>& timeLine = curMach.m_timeLines[Index_final];
	unsigned num_reentry = curJob.m_proceMachs[machIndexOfJob].second;
	map<Job*, unsigned> jobToInsert{ make_pair(&curJob, num_reentry) };

	if (insertPosition < 0)
		return false;

	if (isNew) // �¿�һ¯
		timeLine.insert(timeLine.begin() + insertPosition, make_pair(jobToInsert, timeWinFinal));
	else  // �����ѿ���¯
		timeLine[insertPosition].first.insert(make_pair(&curJob, num_reentry));

	return true;
};


// ��ĳ��������ĳ����
bool  insertJob(Job& curJob, Mach& curMach, unsigned machIndexOfJob)
{
	time_duration& timeDuration = curJob.m_proceTimes[machIndexOfJob].second;
	//time_duration& swithT = double2timeDuration(curMach.m_timeOfSwith);  // �л�ʱ��
	//bool swith = isSwitchPre(curJob, curMach, machIndexOfJob);  // �Ƿ���Ҫ�л�
	
	int a = true * 3;
	bool isInserted(false);
	unsigned IndexOfTimeWin(0);  // machine�Ĳ��������

	ptime readyTimeForOrder;  // job�����ڸû����Ͽ�ʼ�ӹ���ʱ��
	time_period timeWinToInsert(curJob.m_startDateOfOrder, timeDuration);  //Ҫ�����ʱ�䴰
	if (0 == machIndexOfJob)  // �����job�ĵ�һ��machine
		readyTimeForOrder = curJob.m_startDateOfOrder;
	else
		readyTimeForOrder = (curJob.m_allocatedTimeWin.end()-1)->second.end();

	if (0 == curMach.m_allocatedTimeWin.size())  // �����machine�ϵĵ�һ��job
	{
		timeWinToInsert = time_period(readyTimeForOrder, timeDuration);
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
			if (left_TimeWin < readyTimeForOrder + timeDuration)  // �ɼӹ�ʱ���ʱ�䴰��ʼʱ��֮��һ���Ų���
				continue;

			if (curMach.m_allocatedTimeWin.cbegin() == timeInfo_iter)  // ��һ��time window
			{
				//std::cout << "**************** " << std::endl;
				timeWinToInsert = time_period(readyTimeForOrder, timeDuration);
				isInserted = true;
				break;
			}
			else
			{
				auto timeInfo_iter2 = timeInfo_iter-1;
				ptime right_Pre = timeInfo_iter2->second.end();
				if ((readyTimeForOrder < right_Pre)&&(right_Pre + timeDuration <= left_TimeWin))
				{
					timeWinToInsert = time_period(right_Pre, timeDuration);
					isInserted = true;
					break;
				}
				if (right_Pre <= readyTimeForOrder)
				{
					timeWinToInsert = time_period(readyTimeForOrder, timeDuration);
					isInserted = true;
					break;
				}
			}
		}  // END OF ����curMach.m_allocatedTimeWin

		if (false == isInserted)
		{
			ptime right_Pre = (curMach.m_allocatedTimeWin.end()-1)->second.end();
			timeWinToInsert = time_period(right_Pre>readyTimeForOrder ? right_Pre : readyTimeForOrder, timeDuration);
			isInserted = true;
		}
	}
	curJob.m_curMachIndex = machIndexOfJob;
	curJob.m_allocatedTimeWin.push_back(make_pair(curJob.m_proceMachs[machIndexOfJob], timeWinToInsert));
	

	curMach.m_allocatedTimeWin.insert(curMach.m_allocatedTimeWin.begin() + IndexOfTimeWin, 
		make_pair(make_pair(curJob.m_jobCode, curJob.m_proceMachs[machIndexOfJob].second), timeWinToInsert));
	return true;
};;

//  �����õĳ�ʼ��
void myInitialization(vector<string> &jobsCodeVec, vector<string> &machsCodeVec, map<string, Job*> &jobsMap, map<string, Mach*> &machsMap)
{
	jobsCodeVec = vector<string>();
	machsCodeVec = vector<string>();
	jobsMap = map<string, Job*>();
	machsMap = map<string, Mach*>();

	string machCode = "C101";
	machsCodeVec.push_back(machCode);
	Mach * machIter = new Mach;
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
	Job*  jobIter = new Job;
	jobsMap.insert(make_pair(jobCode, jobIter));
	jobIter->m_jobCode = jobCode;
	jobIter->m_startDateOfOrder = time_from_string("2020-5-1 00:00:00");
	jobIter->m_dueDateOfOrder = time_from_string("2020-5-1 20:00:00");
	jobIter->m_proceMachs = vector<pair<string, unsigned>>({make_pair("C101",1), make_pair("C102",1)});
	jobIter->m_proceTargets = vector<pair<pair<string,unsigned>, ProcessTargets>>({ make_pair(make_pair("C101",1), ProcessTargets()),
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

//  ��ʼ������
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


// ���ݿ��ȡ������Ϣ����ʼ��
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
		jobsCodeVec.push_back(jobP->m_jobCode);
		jobsMap[jobP->m_jobCode] = jobP;
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
			std::cout<< "Warning: there are no info in table 'tlorderinformation' for the strip_id of '"
				<< jobCode<<"', but '"<< jobCode<< "' exists in table 'tlprocess'!" <<std::endl;
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
			num_reentry = (machCode ==machInfo.first)? (num_reentry+1): num_reentry;
		//std::cout<< jobP->m_jobCode<<", "<< machCode << std::endl;
		jobP->m_proceMachs.push_back(make_pair(machCode, num_reentry));
		//ProcessTargets* processTags= new ProcessTargets(mystod(row[3]), mystod(row[4]));
		//cout<<"���a="<< mystod(row[4]) <<"   ���b="<< mystod(row[3]) <<"\n"<<endl;
		jobP->m_proceTargets.push_back(make_pair(make_pair(machCode, num_reentry), ProcessTargets(mystod(row[4]), mystod(row[3])) ));
		++i;
	}
	std::cout << "There are " << i << " notes in total." << std::endl;
};

// ���ݿ��ȡ������Ϣ����ʼ��
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
		std::cout << "machdCode" << row[0] << std::endl;
		if (CodeOfBellFurn == row[0])  // ������¯
		{
			Mach_BellFurnace* machP = new Mach_BellFurnace();
			machP->m_machCode = row[0];
			machsCodeVec.push_back(machP->m_machCode);
			machsMap[machP->m_machCode] = machP;
		}
		else if(airFurnaceSet.find(string(row[0]))!= airFurnaceSet.end()) // ������¯
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
			Mach* machP = new Mach;
			machP->m_machCode = row[0];
			machsCodeVec.push_back(machP->m_machCode);
			machsMap[machP->m_machCode] = machP;
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
		Mach* machP = new Mach;
		machP->m_machCode = row[0];
		machsCodeVec.push_back(machP->m_machCode);
		machsMap[machP->m_machCode] = machP;
		++i;
	}
	std::cout << "There are " << i << " machines in total." << std::endl;
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


// ��ӡ�����ų̽��
void printFinalRes(map<string, Job*> &jobsMap, map<string, Mach*> &machsMap)
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
				std::cout << "   Number of time line: " << i_timeline<< std::endl;
				for (auto& timeWinInfo : timeline)  // ��������time_period
				{
					// using TimeWin = pair<map<Job*, unsigned>, time_period>;  // pair< map<Job*, ��job�ڼ�������>, job�����ʱ�䴰>

					time_period timeWin = timeWinInfo.second;
					std::cout << "      "<<to_iso_extended_string(timeWin.begin())
						<< "--" << to_iso_extended_string(timeWin.last())<<std::endl;
					unsigned i_jobs(0);
					for (auto& jobs : timeWinInfo.first)  // ��������job
					{
						std::cout <<"                                      Number of jobs this period="<< i_jobs 
							<< "  jobCode = " << (jobs.first)->m_jobCode << "; reentry = " << jobs.second << std::endl;;
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
					<< " "<< to_iso_extended_string(timeWin.begin())
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
void writeToCSV(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap)
{
	ofstream opt;
	opt.open("ResultsOfMach_spt.csv", ios::out | ios::trunc);
	cout << "Writing to csv..." << endl;
	for (auto& machInfo : machsMap)  // ��������machine
	{
		if (CodeOfBellFurn == machInfo.first){
			Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machInfo.second);
			opt << curMachP->m_machCode << endl;
			unsigned i_timeline(0);
			for (auto& timeline : curMachP->m_timeLines)
			{
				opt << "ParallelMachine="<< i_timeline << std::endl;
				for (auto& timeWinInfo : timeline)  // ��������time_period
				{
					// using TimeWin = pair<map<Job*, unsigned>, time_period>;  // pair< map<Job*, ��job�ڼ�������>, job�����ʱ�䴰>

					time_period timeWin = timeWinInfo.second;
					double beginT = timeDuration2Double(timeWin.begin() - curTime);
					double durationT = timeDuration2Double(timeWin.end() - timeWin.begin());

					opt <<","<< beginT <<","
						<< durationT << std::endl;
					unsigned i_job(1);
					
					for (auto& jobs : timeWinInfo.first)  // ��������job
					{
						Job* jobP = jobs.first;
						unsigned reEntry = jobs.second;

						//��ȡ��ǰ���
						double curWidth(0);   //job�ĵ�ǰ���
						unsigned machIndexOfJob(0);
						for (auto& proInfo : jobP->m_proceTargets)
						{
							if (make_pair(curMachP->m_machCode, reEntry) == proInfo.first)
							{ 
								curWidth = proInfo.second.m_targetWidth;
								break;
							}
							++machIndexOfJob;
						}
						for (int i = machIndexOfJob; i >= 0; --i)
						{
							if (jobP->m_proceTargets[i].second.m_targetWidth > 0.0) // �ӵ�ǰ����ǰ��Ŀ���ȴ���0��ֵ 
							{
								curWidth = jobP->m_proceTargets[i].second.m_targetWidth;
								break;
							}
						}
						//cout<<" curWidth="<< curWidth <<endl;
						if (curWidth <= 0.0)   // ��Ŀ���ȶ���0 
							curWidth = jobP->m_initialInfos.m_targetWidth;
						// END OF ��ȡ��ǰ���

						opt <<"," << "," << "," << "reentry=" << reEntry
							<<","<< jobP->m_jobCode
							<< i_job << "," 
							<< "��ǰ���=" << curWidth << std::endl;
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
				double beginT = timeDuration2Double(timeWin.begin() - curTime);
				double durationT = timeDuration2Double(timeWin.end() - timeWin.begin());

				opt << i_job << "," << "reentry=" << timeWinInfo.first.second
					<< "," << jobCode
					<< "," << beginT
					<< "," << durationT
					<< "," << jobsMap[jobCode]->m_alloyType << std::endl;
				++i_job;
			}
		}
		else{
			Mach* curMachP = machInfo.second;
			opt << curMachP->m_machCode << endl;
			unsigned i_job(1);
			for (auto& timeWinInfo : curMachP->m_allocatedTimeWin)  // ��������time_period
			{
				string jobCode = timeWinInfo.first.first;
				time_period timeWin = timeWinInfo.second;
				double beginT = timeDuration2Double(timeWin.begin()-curTime);
				double durationT = timeDuration2Double(timeWin.end() - timeWin.begin());
				opt << i_job << "," << "reentry=" << timeWinInfo.first.second
					<< "," << jobCode 
					<<  timeWinInfo.first.second
					<< "," << beginT
					<< "," << durationT  << std::endl;
				++i_job;
			}
		}
	};
	cout << "Done of writing to csv..." << endl;

	opt.close();
	opt.open("ResultsOfJob_spt.csv", ios::out|ios::trunc);

	cout << "Writing to csv..." << endl;
	for (auto& jobInfo : jobsMap)  // ��������job
	{
		Job* curJobP = jobInfo.second;
		opt << curJobP->m_jobCode <<","<<","<< to_iso_extended_string(curJobP->m_dueDateOfOrder)<< endl;
		unsigned i_mach(0);
		for (auto& timeInfo : curJobP->m_allocatedTimeWin)  // ��������time_period
		{
			string machCode = timeInfo.first.first;
			time_period timeWin = timeInfo.second;
			double beginT = timeDuration2Double(timeWin.begin() - curTime);
			double durationT = timeDuration2Double(timeWin.end() - timeWin.begin());

			opt<< i_mach << "," << machCode <<","<< beginT
				<<","<< durationT << std::endl;
			++i_mach;
		}
	};
	cout << "Done of writing to csv..." << endl;
}


pair<double, double> scheduleByJobOrder(vector<pair<string, Job*>>& jobOrder, 
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, bool isPrint=0) {
	//if (jobOrder.size() >= 79)std::cout << "curJobsize: " << jobOrder.size() << std::endl;
	for (auto& jobInfo : jobOrder)
	{
		Job* curJobP = jobInfo.second;
		cout<< "curJob: " << curJobP->m_jobCode
			<<"dd"<<curJobP->m_proceMachs.size()<< std::endl;
		unsigned machIndex(0);  // job�� machine
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // ����ĳ��job������machine
		{
			
			std::cout << "         curMach: " << machCodeOfCurJob.first << endl;

			bool isSuccess;
			if (CodeOfBellFurn == machCodeOfCurJob.first)  // ������¯
			{
				Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex);
			}
			else if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // ������¯���еĻ���
			{
				Mach_AirFurnace* curMachP = static_cast<Mach_AirFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else  //����
			{
				Mach* curMachP = machsMapTemp[machCodeOfCurJob.first];
				isSuccess = insertJob(*curJobP, *curMachP, machIndex);
			}

			//if ("0118741020-0-0"==curJobP->m_jobCode)
			//std::cout << "curJobP.code"<< curJobP->m_jobCode <<"curMach: " << machCodeOfCurJob.first
			//	<< "isSuccess: " << isSuccess << std::endl;
			//if (jobOrder.size() >= 80 && ("0118741020-0-0" == curJobP->m_jobCode)) std::cout << "curMach: " << machCodeOfCurJob.first << "qyqyqyqyqyqyqy " << machIndex << endl;
			++machIndex;
		}
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

void initJobsTemp(map<string, Job*>& jobsMapTemp, vector<pair<string, Job*>>& jobOrderTemp, vector<pair<string, Job*>>& jobOrder) {
	for (auto& jobInfo : jobOrder)
	{
		Job* jobP = new Job;
		*jobP = *jobInfo.second;
		jobsMapTemp.insert(make_pair(jobP->m_jobCode, jobP));
		jobOrderTemp.push_back(make_pair(jobP->m_jobCode, jobP));
	}
};

void initMachsMapTemp(map<string, Mach*>& machsMapTemp, map<string, Mach*>& machsMap) {
	for (auto& machInfo : machsMap)
	{

		if (CodeOfBellFurn == machInfo.second->m_machCode)  // ������¯
		{
			Mach_BellFurnace* machP = new Mach_BellFurnace();
			*machP = *(static_cast<Mach_BellFurnace*> (machInfo.second));
			machsMapTemp.insert(make_pair(machInfo.second->m_machCode, machP));
		}
		else if (airFurnaceSet.find(machInfo.second->m_machCode) != airFurnaceSet.end()) // ������¯
		{
			Mach_AirFurnace* machP = new Mach_AirFurnace();
			*machP = *(static_cast<Mach_AirFurnace*> (machInfo.second));
			machsMapTemp.insert(make_pair(machInfo.second->m_machCode, machP));
		}
		else
		{
			Mach* machP = new Mach;
			*machP = *(machInfo.second);
			machsMapTemp.insert(make_pair(machInfo.second->m_machCode, machP));
		}
	}
};


void main()
{
	vector<string> jobsCodeVec;     // vector<job code>
	vector<string> machsCodeVec;    // vector<machine code>
	map<string, Job*> jobsMap;      // map<jobCode, Job*>
	map<string, Mach*> machsMap;    // map<machCode, Mach*>
	//ptime curTime = from_iso_string("20200610T000000");  // ��ǰʱ��
	//ptime curTime = from_iso_string("20200501T0000000");
	
	MYSQL* mysql = new MYSQL;
	MYSQL_RES* res(NULL);   //����ṹ�������е�һ����ѯ�����  
	ConnectDatabase(mysql);

	char* sqla = "select * from tlorderinformation";
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
	
	set<vector<pair<string, unsigned>>> processLineSet;
	map<string, unsigned> machFrequ;
	for (auto& jobInfo : jobsMap)  // ��������job
	{
		string curjobcode = jobInfo.first;
		Job* curJobP = jobInfo.second;
		if (processLineSet.find(curJobP->m_proceMachs) == processLineSet.end())
		{
			processLineSet.insert(curJobP->m_proceMachs);
			for (auto& oneProcess : curJobP->m_proceMachs)
			{
				if (machFrequ.find(oneProcess.first) == machFrequ.end())
					machFrequ.insert(make_pair(oneProcess.first, 1));
				if (1==oneProcess.second)
					machFrequ[oneProcess.first] += 1;
			}
		}
	}
	std::cout << processLineSet.size() <<std::endl; 
	for (auto& process : processLineSet)
	{
		for (auto& oneProcess : process)  std::cout << oneProcess.first << " " << oneProcess.second << std::endl;
		std::cout << std::endl;
	}
	std::cout << std::endl;

	for (auto& mach : machFrequ) std::cout << mach.first << " " << mach.second << std::endl;

	
	// ��ӡ
	for (auto& jobInfo : jobsMap)  // ��������job
	{
		string curjobcode = jobInfo.first;
		Job* curJobP = jobInfo.second;
		curJobP->m_dueDateOfOrder = from_iso_string((curJobP-> m_dueDateOfOrderStr)+"T115959");
		if (true)
		{
			std::cout << curjobcode << ": " << curJobP->m_dueDateOfOrderStr 
				<<" "<< curJobP->m_alloyGrade << std::endl;
			//std::cout << curjobcode << ": " << to_iso_string(curJobP->m_dueDateOfOrder) <<std::endl;
		}

		if (processLineSet.find(curJobP->m_proceMachs)!= processLineSet.end())
		{
			processLineSet.insert(curJobP->m_proceMachs);
		}
		// ���job�����ڸ�ʽ��������!!??
		curJobP->m_startDateOfOrder = curTime;
	}

	
	//myInitialization(jobsCodeVec, machsCodeVec, jobsMap, machsMap);
	vector<pair<Job*, ptime>> jobsWithDueDate;               // vector<pair<Job*, ��ֹʱ��>>
	vector<pair<Job*, time_duration>> jobsWithTotalProTime;  // vector<pair<Job*, �ܼӹ�ʱ��>>
	vector<pair<Job*, time_duration>> jobsWithSlackTime;  // vector<pair<Job*, �ɳ�ʱ��>>   �ɳ�ʱ�䣺job��ʣ��ʱ�䣨��ǰʱ����뽻���ڵ�ʱ�䣩���job����ļӹ�ʱ��֮��
	for (auto& jobInfo : jobsMap)  // ��������job��processTime����ȡprocessTime
	{
		string curJobCode = jobInfo.first;
		
		Job *curJobP = jobInfo.second;
		jobsWithDueDate.push_back(make_pair(curJobP, curJobP->m_dueDateOfOrder));
		time_duration sumOfProcessTime;
		unsigned machIndex(0);

		for (pair<string, unsigned>& machInfoOfCurJob : curJobP->m_proceMachs)   // ����ĳ��job������machine
		{
			//std::cout << "machIndex=" << machIndex 
			//std::cout<< curJobP->m_proceTargets.size() << std::endl;
			ProcessTargets const& processTargets = (curJobP->m_proceTargets)[machIndex].second;
			time_duration processTime = getProcessTime(machsMap[machInfoOfCurJob.first],curJobP, machIndex);
			//time_duration processTime = curJobP->m_proceTimes[machIndex].second;
			std::cout <<"2processtT="<<to_iso_string(processTime)<< std::endl;
			if(curJobP->m_proceTimes.size()> machIndex)
				curJobP->m_proceTimes[machIndex]= make_pair((curJobP->m_proceTargets)[machIndex].first, processTime);
			else
				curJobP->m_proceTimes.insert(curJobP->m_proceTimes.begin() + machIndex,
					make_pair(curJobP->m_proceTargets[machIndex].first, processTime));

			sumOfProcessTime = sumOfProcessTime + processTime;
			++machIndex;
		}
		
		curJobP->m_totalProceTime = sumOfProcessTime;
		std::cout << "sumOfProcessTime = "<< to_iso_string(sumOfProcessTime)<< std::endl;
		time_duration slackTime = (curJobP->m_dueDateOfOrder-curTime) - sumOfProcessTime;  //job��ʣ��ʱ�䣨��ǰʱ����뽻���ڵ�ʱ�䣩���job����ļӹ�ʱ��֮��
		jobsWithSlackTime.push_back(make_pair(curJobP, slackTime));
		jobsWithTotalProTime.push_back(make_pair(curJobP, sumOfProcessTime));
	}

	
	// ���ݽ�ֹʱ�䣬�ܼӹ�ʱ�䣬�ɳ�ʱ������
	sort(jobsWithDueDate.begin(), jobsWithDueDate.end(), myCmpBy_ptime);
	sort(jobsWithTotalProTime.begin(), jobsWithTotalProTime.end(), myCmpBy_time_duration);
	sort(jobsWithSlackTime.begin(), jobsWithSlackTime.end(), myCmpBy_time_duration);
	

	for (auto& jobInfo : jobsWithSlackTime)  // ��������job
	//for (auto& jobInfo : jobsWithDueDate)  // ��������job
	{
		Job* curJobP = jobInfo.first;
		if(1)
		{
			//std::cout << "Job code: " << curJobP->m_jobCode << "  SlackTime: " << to_iso_string(jobInfo.second)
			//<< " hours" << std::endl;
			//std::cout << "Job code: " << curJobP->m_jobCode << "  DueTime: " << to_iso_extended_string(jobInfo.second)
			//<< " hours"<< std::endl;
			//std::cout << "  " << curJobP->m_alloyGrade << std::endl;

			std::cout << "Job code: " << curJobP->m_jobCode << std::endl;
			for (unsigned i=0; i< curJobP->m_proceTargets.size() ; ++i)
				cout << "    mubiao " << curJobP->m_proceTargets[i].first.first 
				<< "  Width="<<curJobP->m_proceTargets[i].second.m_targetWidth << endl;
		}
	};
	
	cout << "jobsWithSlackTime.size()="<< jobsWithSlackTime.size() <<endl;

	/*
	vector<pair<string, Job*>> jobOrder;
	jobOrder.push_back(make_pair(jobsWithTotalProTime[0].first->m_jobCode, jobsWithTotalProTime[0].first));
	//for (unsigned i = 2; i < 30; ++i)  // һ��һ���ط���job
	//unsigned i(-1);
	//for (auto& jobInfo: jobsMap)  // һ��һ���ط���job
	for (unsigned i = 1; i < jobsWithTotalProTime.size(); ++i)  // һ��һ���ط���job
	{
		
	    //Job* curJobP = jobInfo.second;
		//cout << "i=" << i++ << endl;
		//if (0 == i)
		//{
		//	jobOrder.push_back( make_pair(curJobP->m_jobCode, curJobP));
		//	continue;
		//}

		Job* curJobP = jobsWithTotalProTime[i].first;
		cout << "curJobP->m_jobCode=" << curJobP->m_jobCode << "  i="<< i << endl;

		unsigned bestPosition(0);
		pair<double, double> bestObjectVals;
		//cout<<"=========jobnum="<< jobOrder.size() <<endl;
		for (unsigned j_insertPosition = 0; j_insertPosition <= jobOrder.size(); ++j_insertPosition) // �������в���λ��
		{
			vector<pair<string, Job*>> jobOrderTemp;
			map<string, Job*> jobsMapTemp;
			map<string, Mach*> machsMapTemp;
			//if (j_insertPosition >= 80 ) cout << "insertPostion=" << j_insertPosition << endl;
			
			initJobsTemp(jobsMapTemp, jobOrderTemp, jobOrder);
			initMachsMapTemp(machsMapTemp, machsMap);

			//if (j_insertPosition >= 80)  cout << "insertPostion="<< j_insertPosition << endl;
			Job* curjobP_copy = new Job;
			*curjobP_copy = *curJobP; //0205782740-0-0
			jobOrderTemp.insert(jobOrderTemp.begin()+j_insertPosition, make_pair(curjobP_copy->m_jobCode, curjobP_copy));
			jobsMapTemp.insert(make_pair(curjobP_copy->m_jobCode, curjobP_copy));
			
			//if (j_insertPosition >= 80) cout << "xxxxxxxxxxxxxxx" << endl;
			pair<double, double> objectVals = scheduleByJobOrder(jobOrderTemp, jobsMapTemp, machsMapTemp);
			//if (j_insertPosition >= 80) cout << "llllllllllll" << endl;
			if (0 == j_insertPosition) bestObjectVals = objectVals;
			else if (objectVals.second < bestObjectVals.second)
			{
				bestPosition = j_insertPosition;
				bestObjectVals = objectVals;
			}
		}

		cout << "bestObjectVals(makespan)=" << bestObjectVals.second <<";  due time=" << bestObjectVals.first << endl;
		jobOrder.insert(jobOrder.begin()+bestPosition, make_pair(curJobP->m_jobCode, curJobP));
	}

	vector<pair<string, Job*>> jobOrderTemp;
	map<string, Job*> jobsMapTemp;
	map<string, Mach*> machsMapTemp;

	initJobsTemp(jobsMapTemp, jobOrderTemp, jobOrder);
	initMachsMapTemp(machsMapTemp, machsMap);

	pair<double, double> objectVals = scheduleByJobOrder(jobOrderTemp, jobsMapTemp, machsMapTemp,true);
	*/
	
	std::cout <<"jobsWithSlackTime.size()" << jobsWithSlackTime.size()<<std::endl;
	vector<pair<string, Job*>> jobOrder;
	for (unsigned i = 0; i < jobsWithSlackTime.size(); ++i)  // һ��һ���ط���job
	//for (auto& jobInfo: jobsMap)  // һ��һ���ط���job
	{
		//Job* curJobP = jobInfo.second;
		Job* curJobP = jobsWithSlackTime[i].first;
		jobOrder.push_back( make_pair(curJobP->m_jobCode, curJobP));
	}

	pair<double, double> objectVals = scheduleByJobOrder(jobOrder, jobsMap, machsMap, true);
	cout << "objectVals(makespan)=" << objectVals.second << ";  due time=" << objectVals.first << endl;
	
	


	/*
	std::cout  <<std::endl;
	for (auto &jobInfo : jobsWithSlackTime)
	{
		Job* curJobP = jobInfo.first;
		 std::cout << "curJob: " << curJobP->m_jobCode << std::endl;
		unsigned machIndex(0);  // job�� machine
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // ����ĳ��job������machine
		{
			std::cout << "curMach: "<< machCodeOfCurJob.first << std::endl;
			if (CodeOfBellFurn == machCodeOfCurJob.first)  // ������¯
			{
				Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMap[machCodeOfCurJob.first]);
				bool isSuccess = insertJob(*curJobP, *curMachP, machIndex);
			}
			else if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // ������¯���еĻ���
			{
				Mach_AirFurnace* curMachP = static_cast<Mach_AirFurnace*>(machsMap[machCodeOfCurJob.first]);
				bool isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMap);
			}
			else  //����
			{
				Mach* curMachP = machsMap[machCodeOfCurJob.first];
				bool isSuccess = insertJob(*curJobP, *curMachP, machIndex);
			}

			//std::cout << "curMach: " << curMach.m_machCode <<"\n"<< "isSuccess: " << isSuccess << std::endl;
			machIndex++;
		}
	}

	// ��ӡ�����ų̽��
	printFinalRes(jobsMap, machsMap);
	
	// ��ȡĿ��ֵ
	pair<double, double> objectVals = getObjVals(jobsMap, machsMap);
	std::cout << "������ʱ��(Сʱ): " << objectVals.first<< "; �ӹ����й��������ʱ�䳤��(Сʱ): " <<objectVals.second << std::endl;
	*/

}
















