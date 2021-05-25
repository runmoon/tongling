#include<head.h>


set<string> rollingMachSet{ "BD-S003","BD-S005","BD-S025","BD-S009","BD-S010","BD-S011" };  // ������; getStatus()��
// BD-S003	��������ʽ������; BD-S005	������; BD-S025	�Ĺ�������; 
// BD-S009	ɭ����20��������; BD-S010	20��������; BD-S011	4��������

set<string> cut_StretchMachSet{ "BD-S018","BD-S015",
					"BD-S016","BD-S019","BD-S020","BD-S021" };  // ���ݼ��������; getStatus()��
// BD-S018	1250�������; BD-S015	650�����; 
// BD-S016	1250�ݼ�; BD-S019	650���ݼ�; BD-S020	650���ݼ�; BD-S021	350�ݼ�

set<string> washMachSet{ "BD-S012","BD-S013","BD-S014" };
// BD-S012	1250��ϴ����; BD-S013	650��ϴ����; BD-S014	��650��ϴ����; getStatus()��

set<string> airFurnaceSet{ "BD-S024","BD-S007","BD-S008" };
//BD-S024  WSP����¯;    BD-S007  1250����ʽ�˻�¯��;    BD-S008  650����ʽ�˻�¯��

map<string, string> alloyGrade2Type{    // �ɺϽ��ƺţ�grade�����Ӧ�ĺϽ����ࣨtype��
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
		cout << "mysql_real_query failure! " << endl;
		return 0;
	}
	// �洢�����
	res = mysql_store_result(mysql);
	if (NULL == res) //  if( !(res = mysql_store_result(&mysql))
	{
		cout << "mysql_store_result failure! " << endl;
		return 0;
	}

	//��ӡ��������  
	//cout << "number of dataline returned: " << mysql_affected_rows(mysql) << endl;
	cout << "number of dataline returned: " << mysql_num_rows(res) << endl;
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
			cout<<"ERROR with '" << _aRange <<"'�� ���ܽ����� "<<endl;
			return false;
		}
	}
	else
	{
		cout << "ERROR with '" << _aRange << "'�� ���ܽ����� " << endl;
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
	
	if (rollingMachSet.count(machCode) > 0) //������
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
		
	if (cut_StretchMachSet.count(machCode) > 0) //�Ǽ��л�����
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

	if (washMachSet.count(machCode) > 0) //����ϴ
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
	// bool isRollingMach(rollingMachSet.count(machP->m_machCode) > 0);
	if (CodeOfBellFurn == machP->m_machCode)  // ���������¯
		return double2timeDuration(T_Bell_Furn);

	for (auto& cap : machP->m_capsOriginalInfo)
	{
		cout << "cap.m_width=" << cap.m_width << "processTargets.m_targetWeight=" << processTargets.m_targetWeight << endl;

		if (!isInRange(cap.m_width, processTargets.m_targetWeight))
			continue;
		cout << "cap.m_thick=" << cap.m_thick << "processTargets.m_targetThick=" << processTargets.m_targetThick << endl;

		if (!isInRange(cap.m_thick, processTargets.m_targetThick))
			continue;
		if ((cap.m_alloyType != alloyGrade2Type[jobP->m_alloyGrade])
			&& ("general_alloy" != cap.m_alloyType)
			&& ("null" != cap.m_alloyType)
			&& ("Null" != cap.m_alloyType)
			&& ("NULL" != cap.m_alloyType)
			&& ("" != cap.m_alloyType))
			continue;
		string status = getStatus(machP->m_machCode, jobP, machIndex);

		cout << "status=" << status << "cap.m_status=" << cap.m_status << endl;

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
		cout<<"Error with capacity! Cannot find a matched capacity of job '"
		<<jobP->m_jobCode<<"' on machine "<< machP->m_machCode <<"!"<<endl;

	//cout << "machCode=" << machP->m_machCode << "; jobCode="<<jobP->m_jobCode <<"; cap="<<processT<< endl;
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
	if (!(sin >> d))
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
		cout<<"Error with str-'"<<str<< "'! Str is NULL��"<<endl;
		return -1;  // ֵΪ��
	}
	else if (isNum(str))
		return stod(str);
	else
	{
		cout << "Error with str-'" << str << "'! Str is NOT A NUMBER��" << endl;
		return -2;  // ��������
	}
}

// string to int
int mystoi(const char* str)
{
	if (str == NULL)
	{
		cout << "Error with str- " << str << "! Str is NULL��" << endl;
		return -1;  // ֵΪ��
	}
	else if (isNum(str))
		return stoi(str);
	else
	{
		cout << "Error with str- " << str << "! Str is NOT A NUMBER��" << endl;
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
bool canAddToTimeWin(string jobCode, TimeWin &timeWin, Mach_BellFurnace& curMachBell)
{
	if (curMachBell.m_RuleForFurnWithWidth.second.second == timeWin.first.size()) // �Ѿ���3���ˣ����ܲ���
		return false;
	else if (curMachBell.m_RuleForFurnWithWidth.second.first == timeWin.first.size())  // �Ѿ���2����
	{
		for (auto& jobInfo: timeWin.first)
		{
			Job* jobP = jobInfo.first;
			if ((jobP->m_finalTargets.m_targetWidth >= curMachBell.m_RuleForFurnWithWidth.first)//??����job�Ŀ�ȹ���ĸ���
				||(jobCode ==jobP->m_jobCode))  // ������job�Լ�
				return false;
		}
	}
	return true;
}

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

	if (0 == timeline.size())  // �����machine�ϵĵ�һ��job
	{
		timeWinResult = time_period(readyTimeForOrder, timeDuration);
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

			if (left_TimeWin < readyTimeForOrder + timeDuration)  // �ɼӹ�ʱ���ʱ�䴰��ʼʱ��֮��һ���Ų����µ�һ¯
			{
				if ((readyTimeForOrder < left_TimeWin) && canAddToTimeWin(curJob.m_jobCode, *timeInfo_iter, curMach))
				{
					timeWinResult = timeInfo_iter->second;
					flag_NewFurn = false; // ���¿�һ¯
					isInserted = true;
					if (curJob.m_jobCode == "9800257701-0-0")
					{
						cout << "curJob=" << curJob.m_jobCode << endl;
						cout << "readyTimeForOrder=" << to_iso_extended_string(readyTimeForOrder) << endl;
						cout << "left_TimeWin=" << to_iso_extended_string(left_TimeWin) << endl;
					}

					break;
				}
				continue;
			}
			
			if (timeline.cbegin() == timeInfo_iter)  // ��һ��time window
			{
				cout << "**************** " << endl;
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
				}
				else if ((readyTimeForOrder < right_Pre)&&(canAddToTimeWin(curJob.m_jobCode, *timeInfo_iter, curMach))
					&&(readyTimeForOrder < left_TimeWin))
				{
					timeWinResult = timeInfo_iter->second;
					flag_NewFurn = false;    // ���¿�һ¯
					isInserted = true;
				}
				if (right_Pre <= readyTimeForOrder)
				{
					timeWinResult = time_period(readyTimeForOrder, timeDuration);
					flag_NewFurn = true;    // �¿�һ¯
					isInserted = true;
				}
			}
		}  // END OF ����timeline

		if (false == isInserted)
		{
			ptime rightWin = (timeline.end() - 1)->second.end();
			timeWinResult = time_period((readyTimeForOrder<rightWin) ? rightWin : readyTimeForOrder, timeDuration);
			flag_NewFurn = true;    // �¿�һ¯
			isInserted = true;
		}
	}

	cout << "curJob=" << curJob.m_jobCode <<"777777"<< endl;
	cout << "timeWinResult=" << to_iso_extended_string(timeWinResult.begin())
		<<"--" << to_iso_extended_string(timeWinResult.last()) << endl;
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
	pair<unsigned, bool> resultOfPreInsert(make_pair(-1,false));   // pair<TimeWin��λ��, �Ƿ�Ҫ�¿�һ¯>
	pair<unsigned, bool> resultOfFinal(make_pair(-1, false));   // pair<TimeWin��λ��, �Ƿ�Ҫ�¿�һ¯>

	cout << "size(m_numOfParallel) == "<< curMach.m_timeLines.size()<<endl;
	for(auto &timeline:curMach.m_timeLines)
	{ 
		time_period timeWinResult(curJob.m_startDateOfOrder, timeDuration);
		
		resultOfPreInsert = preInsertJobToMach(curJob, curMach, machIndexOfJob, timeline, timeWinResult);
		if (0 == Index_parallelMach)
		{
			timeWinFinal = timeWinResult;
			resultOfFinal = resultOfPreInsert;
		}	
		if (timeWinFinal.begin() > timeWinResult.begin()) // ����
		{
			timeWinFinal = timeWinResult;
			resultOfFinal = resultOfPreInsert;
			Index_final = Index_parallelMach;
		}
		++Index_parallelMach;
	}
	cout << "curJob=" << curJob.m_jobCode << "8888888" << endl;
	cout << "timeWinResult=" << to_iso_extended_string(timeWinFinal.begin()) << endl;

	curJob.m_curMachIndex = machIndexOfJob;
	curJob.m_allocatedTimeWin.push_back(make_pair(curJob.m_proceMachs[machIndexOfJob], timeWinFinal));

	vector<TimeWin> &timeLine = curMach.m_timeLines[Index_final];
	unsigned num_reentry = curJob.m_proceMachs[machIndexOfJob].second;
	map<Job*, unsigned> jobToInsert { make_pair(&curJob, num_reentry) };
	
	if (resultOfFinal.first < 0)
		return false;

	if (resultOfFinal.second) // �¿�һ¯
		timeLine.insert(timeLine.begin() + resultOfFinal.first, make_pair(jobToInsert, timeWinFinal ));
	else  // �����ѿ���¯
	{
		cout << "cccccccccsdsdsdsdsdsd" << endl;
		timeLine[resultOfFinal.first].first.insert(make_pair(&curJob, num_reentry));
	}
	cout << "ewwwwwwwwwwwww" << endl;
	//pair<map<Job*, unsigned>, time_period>;
	return true;
};

// ��ĳ������������¯
bool  insertJob(Job& curJob, Mach_AirFurnace& curMach, unsigned machIndexOfJob)

{
	time_duration& timeDuration = curJob.m_proceTimes[machIndexOfJob].second;

	bool isInserted(false);
	unsigned IndexOfTimeWin(0);  // machine�Ĳ��������

	ptime readyTimeForOrder;  // job�����ڸû����Ͽ�ʼ�ӹ���ʱ��
	time_period timeWinToInsert(curJob.m_startDateOfOrder, timeDuration);  //Ҫ�����ʱ�䴰
	if (0 == machIndexOfJob)  // �����job�ĵ�һ��machine
		readyTimeForOrder = curJob.m_startDateOfOrder;
	else
		readyTimeForOrder = (curJob.m_allocatedTimeWin.end() - 1)->second.end();

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
				cout << "**************** " << endl;
				timeWinToInsert = time_period(readyTimeForOrder, timeDuration);
				isInserted = true;
				break;
			}
			else
			{
				auto timeInfo_iter2 = timeInfo_iter - 1;
				ptime right_Pre = timeInfo_iter2->second.end();
				if ((readyTimeForOrder < right_Pre) && (right_Pre + timeDuration <= left_TimeWin))
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
			ptime right_Pre = (curMach.m_allocatedTimeWin.end() - 1)->second.end();
			timeWinToInsert = time_period(right_Pre > readyTimeForOrder ? right_Pre : readyTimeForOrder, timeDuration);
			isInserted = true;
		}
	}
	curJob.m_curMachIndex = machIndexOfJob;
	curJob.m_allocatedTimeWin.push_back(make_pair(curJob.m_proceMachs[machIndexOfJob], timeWinToInsert));

	curMach.m_allocatedTimeWin.insert(curMach.m_allocatedTimeWin.begin() + IndexOfTimeWin,
		make_pair(make_pair(curJob.m_jobCode, curJob.m_proceMachs[machIndexOfJob].second), timeWinToInsert));
	return true;
};;

// airFurnaceSet
bool isSwitch(Job& curJob, Mach& curMach, unsigned machIndexOfJob)  // �Ƿ���Ҫ�л�
{
	if (airFurnaceSet.find(curJob.m_jobCode) != airFurnaceSet.end())  // ������¯��Ļ���
	{


	}
	return false;
};

// ��ĳ��������ĳ����
bool  insertJob(Job& curJob, Mach& curMach, unsigned machIndexOfJob)
{
	time_duration& timeDuration = curJob.m_proceTimes[machIndexOfJob].second;
	//time_duration& swithT = double2timeDuration(curMach.m_timeOfSwith);  // �л�ʱ��
	//bool swith = isSwitch(curJob, curMach, machIndexOfJob);  // �Ƿ���Ҫ�л�
	
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
				cout << "**************** " << endl;
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
	cout << "Number of fields: " << fieldcount << endl;
	//��ʾ�����ֶ���
	for (int i = 0; i < fieldcount; ++i)
	{
		field = mysql_fetch_field_direct(res, i);
		cout << field->name << "\t";
		fieldsNames_orders.push_back(field->name);
	}
	cout << endl;
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
		cout << "row[0] = " << row[0] << endl;
		cout << "row[1] = " << row[1] << endl;
		cout << "row[2] = " << row[2] << endl;
		cout << "row[3] = " << row[3] << endl;
		cout << "row[4] = " << row[4] << endl;
		cout << "row[5] = " << row[5] << endl;
		cout << "row[6] = " << row[6] << endl;
		*/
		++i;
		
	}
	cout << "There are " << i << " notes in total." << endl;
};


// ���ݿ��ȡ������Ϣ����ʼ��
void initializeJobs(MYSQL_RES* res, vector<string>& jobsCodeVec, map<string, Job*>& jobsMap)
{
	MYSQL_ROW row;  //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾһ������
	MYSQL_FIELD* field = NULL;

	vector<string> fieldsNames_orders;
	//�ֶεĸ���
	int fieldcount = mysql_num_fields(res);
	cout << "Number of fields: " << fieldcount << endl;
	//��ʾ�����ֶ���
	for (int i = 0; i < fieldcount; i++)
	{
		field = mysql_fetch_field_direct(res, i);
		cout << field->name << "\t";
		fieldsNames_orders.push_back(field->name);
	}
	cout << endl;
	int i = 0;  // ����
	while (row = mysql_fetch_row(res))
	{
		// strip_id, deadline, thick, width, note, length, weight, inner_diameter, alloy_grade
		// �ظ���ȡ�У��������ȡÿ�����ֶε�ֵ��ֱ��rowΪNULL
		Job* jobP = new Job;
		jobP->m_jobCode = row[0];
		jobP->m_dueDateOfOrderStr = row[1];
		jobP->m_alloyGrade = row[8];
		jobP->m_finalTargets.m_targetThick = mystod(row[2]);
		jobP->m_finalTargets.m_targetWeight = mystod(row[3]);
		jobP->m_finalTargets.m_targetLength = mystod(row[5]);
		jobP->m_finalTargets.m_targetWeight = mystod(row[6]);
		jobP->m_finalTargets.m_targetInnerDia = mystod(row[7]);
		jobsCodeVec.push_back(jobP->m_jobCode);
		jobsMap[jobP->m_jobCode] = jobP;
		++i;
	}
	cout << "There are " << i << " jobs in total." << endl;
};

// ��ʼ�������������ƹ�����Ϣ���ӹ�Ŀ��ͼӹ�������
void initializeJobs2(MYSQL_RES* res, vector<string>& jobsCodeVec, map<string, Job*>& jobsMap)
{
	MYSQL_ROW row;  //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾһ������
	MYSQL_FIELD* field = NULL;

	vector<string> fieldsNames_orders;
	//�ֶεĸ���
	int fieldcount = mysql_num_fields(res);
	cout << "Number of fields: " << fieldcount << endl;
	//��ʾ�����ֶ���
	for (int i = 0; i < fieldcount; ++i)
	{
		field = mysql_fetch_field_direct(res, i);
		cout << field->name << "\t";
		fieldsNames_orders.push_back(field->name);
	}
	cout << endl;
	int i = 0;  // ����
	while (row = mysql_fetch_row(res))
	{
		// strip_id, process_num, unit_id, thick, width, note, unit_name_orig
		// �ظ���ȡ�У��������ȡÿ�����ֶε�ֵ��ֱ��rowΪNULL
		string jobCode = row[0];
		Job* jobP;
		if (jobsMap.find(jobCode) == jobsMap.end()) // ���jobCode����jobsMap�У�
		{
			cout<< "Warning: there are no info in table 'tlorderinformation' for the strip_id of '"
				<< jobCode<<"', but '"<< jobCode<< "' exists in table 'tlprocess'!" <<endl;
			continue;
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
		//cout<< jobP->m_jobCode<<", "<< machCode << endl;
		jobP->m_proceMachs.push_back(make_pair(machCode, num_reentry));
		jobP->m_proceTargets.push_back(make_pair(make_pair(machCode, num_reentry), ProcessTargets()));
		++i;
	}
	cout << "There are " << i << " notes in total." << endl;
};

// ���ݿ��ȡ������Ϣ����ʼ��
void initialMachs(MYSQL_RES* res, vector<string>& machsCodeVec, map<string, Mach*>& machsMap)
{
	MYSQL_ROW row;  //һ�������ݵ����Ͱ�ȫ(type-safe)�ı�ʾ����ʾһ������
	MYSQL_FIELD* field = NULL;

	vector<string> fieldsNames_orders;
	//�ֶεĸ���
	int fieldcount = mysql_num_fields(res);
	cout << "Number of fields: " << fieldcount << endl;
	//��ʾ�����ֶ���
	for (int i = 0; i < fieldcount; i++)
	{
		field = mysql_fetch_field_direct(res, i);
		cout << field->name << "\t";
		fieldsNames_orders.push_back(field->name);
	}
	cout << endl;
	int i = 0;  // ����
	while (row = mysql_fetch_row(res))
	{
		// unit_id, name
		// �ظ���ȡ�У��������ȡÿ�����ֶε�ֵ��ֱ��rowΪNULL
		if (CodeOfBellFurn == row[0])  // ������¯
		{
			Mach_BellFurnace* machP = new Mach_BellFurnace();
			machP->m_machCode = row[0];
			machsCodeVec.push_back(machP->m_machCode);
			machsMap[machP->m_machCode] = machP;
		}
		else if(airFurnaceSet.find(row[0])!= airFurnaceSet.end()) // ������¯
		{
			Mach_AirFurnace* machP = new Mach_AirFurnace();
			machP->m_machCode = row[0];
			machsCodeVec.push_back(machP->m_machCode);
			machsMap[machP->m_machCode] = machP;
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
	cout << "There are " << i << " machines in total." << endl;
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
	cout << "Number of fields: " << fieldcount << endl;
	//��ʾ�����ֶ���
	for (int i = 0; i < fieldcount; i++)
	{
		field = mysql_fetch_field_direct(res, i);
		cout << field->name << "\t";
		fieldsNames_orders.push_back(field->name);
	}
	cout << endl;
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
	cout << "There are " << i << " machines in total." << endl;
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
		totalDueTime += delayTime;
		if (0 == i_numOfJob)
		{
			timeOfStart = curJobP->m_startDateOfOrder;
			timeOfCompletion = lastTimeWin.last();
		}
		else
		{
			if (timeOfCompletion < lastTimeWin.last())
				timeOfCompletion = lastTimeWin.last();
			if ((curJobP->m_startDateOfOrder) < timeOfStart)
				timeOfStart = curJobP->m_startDateOfOrder;
		}
		i_numOfJob++;
		cout << "delay time of "<<curJobP->m_jobCode << " is: " << delayTime<<endl;
	}
	
	makeSpan = timeDuration2Double(timeOfCompletion - timeOfStart);
	cout << "totalDueTime=��" << totalDueTime << endl;
	cout << "makeSpan = " << makeSpan << endl;
	cout << endl;
	return make_pair(totalDueTime, makeSpan);
};


// ��ӡ�����ų̽��
void printFinalRes(map<string, Job*> &jobsMap, map<string, Mach*> &machsMap)
{
	// ��ӡ�ų̽��
	/*
	cout << endl;
	for (auto& machInfo : machsMap)  // ��������machine
	{
		Mach& curMach = *(machInfo.second);
		cout << "MachCode---------" << curMach.m_machCode << endl;
		for (auto& timeInfo : curMach.m_allocatedTimeWin)  // ��������time_period
		{
			string jobCode = timeInfo.first.first;
			time_period timeWin = timeInfo.second;
			cout << "JobCode  " << jobCode << ": " << to_iso_extended_string(timeWin.begin())
				<< "--" << to_iso_extended_string(timeWin.last()) << endl;
		}
		cout << endl;
	};
	*/

	cout << endl;
	for (auto& machInfo : machsMap)  // ��������machine
	{
		if (CodeOfBellFurn == machInfo.first)
		{
			Mach_BellFurnace* curMach = static_cast<Mach_BellFurnace*>(machInfo.second);
			cout << "MachCode---------" << curMach->m_machCode << endl;
			unsigned i_timeline(0);
			for (auto& timeline : curMach->m_timeLines)
			{
				cout << "   Number of time line: " << i_timeline<< endl;
				for (auto& timeWinInfo : timeline)  // ��������time_period
				{
					// using TimeWin = pair<map<Job*, unsigned>, time_period>;  // pair< map<Job*, ��job�ڼ�������>, job�����ʱ�䴰>

					time_period timeWin = timeWinInfo.second;
					cout << "      "<<to_iso_extended_string(timeWin.begin())
						<< "--" << to_iso_extended_string(timeWin.last())<<endl;
					unsigned i_jobs(0);
					for (auto& jobs : timeWinInfo.first)  // ��������job
					{
						cout <<"                                      Number of jobs this period="<< i_jobs 
							<< "  jobCode = " << (jobs.first)->m_jobCode << "; reentry = " << jobs.second << endl;;
						++i_jobs;
					}
				}
				++i_timeline;
			}

			cout << endl;
		}

	};
	cout << "\n\n" << endl;
	for (auto& jobInfo : jobsMap)  // ��������job
	{
		Job* curJobP = jobInfo.second;
		cout << "JobCode---------" << curJobP->m_jobCode << endl;
		for (auto& timeInfo : curJobP->m_allocatedTimeWin)  // ��������time_period
		{
			string machCode = timeInfo.first.first;
			time_period timeWin = timeInfo.second;
			cout << "MachCode  " << machCode << ": " << to_iso_extended_string(timeWin.begin())
				<< "--" << to_iso_extended_string(timeWin.last()) << endl;
		}
		cout << endl;
	};
}


void main()
{
	vector<string> jobsCodeVec;     // vector<job code>
	vector<string> machsCodeVec;    // vector<machine code>
	map<string, Job*> jobsMap;      // map<jobCode, Job*>
	map<string, Mach*> machsMap;    // map<machCode, Mach*>
	ptime curTime = from_iso_string("20200607T000000");  // ��ǰʱ��
	//ptime curTime = from_iso_string("20200501T0000000");

	
	MYSQL* mysql = new MYSQL;
	MYSQL_RES* res(NULL);   //����ṹ�������е�һ����ѯ�����  
	ConnectDatabase(mysql);

	char* sqla = "select * from tlorderinformation order by alloy_grade";
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
	
	
	// ��ӡ
	for (auto& jobInfo : jobsMap)  // ��������job
	{
		string curjobcode = jobInfo.first;
		Job* curJobP = jobInfo.second;
		curJobP->m_dueDateOfOrder = from_iso_string((curJobP-> m_dueDateOfOrderStr)+"T115959");
		if (false)
		{
			cout << curjobcode << ": " << curJobP->m_dueDateOfOrderStr << endl;
			cout << curjobcode << ": " << to_iso_string(curJobP->m_dueDateOfOrder) <<endl;
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
			//cout << "machIndex=" << machIndex 
			//cout<< curJobP->m_proceTargets.size() << endl;
			ProcessTargets const& processTargets = (curJobP->m_proceTargets)[machIndex].second;
			time_duration processTime = getProcessTime(machsMap[machInfoOfCurJob.first],curJobP, machIndex);
			//time_duration processTime = curJobP->m_proceTimes[machIndex].second;
			cout <<"2processtT="<<to_iso_string(processTime)<< endl;
			if(curJobP->m_proceTimes.size()> machIndex)
				curJobP->m_proceTimes[machIndex]= make_pair((curJobP->m_proceTargets)[machIndex].first, processTime);
			else
				curJobP->m_proceTimes.insert(curJobP->m_proceTimes.begin() + machIndex,
					make_pair(curJobP->m_proceTargets[machIndex].first, processTime));

			sumOfProcessTime = sumOfProcessTime + processTime;
			++machIndex;
		}
		
		curJobP->m_totalProceTime = sumOfProcessTime;
		cout << "sumOfProcessTime = "<< to_iso_string(sumOfProcessTime)<< endl;
		time_duration slackTime = (curJobP->m_dueDateOfOrder-curTime) - sumOfProcessTime;  //job��ʣ��ʱ�䣨��ǰʱ����뽻���ڵ�ʱ�䣩���job����ļӹ�ʱ��֮��
		jobsWithSlackTime.push_back(make_pair(curJobP, slackTime));
		jobsWithTotalProTime.push_back(make_pair(curJobP, sumOfProcessTime));
	}

	
	// ���ݽ�ֹʱ�䣬�ܼӹ�ʱ�䣬�ɳ�ʱ������
	sort(jobsWithDueDate.begin(), jobsWithDueDate.end(), myCmpBy_ptime);
	sort(jobsWithTotalProTime.begin(), jobsWithTotalProTime.end(), myCmpBy_time_duration);
	sort(jobsWithSlackTime.begin(), jobsWithSlackTime.end(), myCmpBy_time_duration);
	
	for (auto& jobInfo : jobsWithDueDate)  // ��������job
	//for (auto& jobInfo : jobsWithDueDate)  // ��������job
	{
		Job* curJobP = jobInfo.first;
		if(true)
		{
			//cout << "Job code: " << curJobP->m_jobCode << "  SlackTime: " << to_iso_extended_string(jobInfo.second)
			//<< " hours" << endl;
			cout << "Job code: " << curJobP->m_jobCode << "  DueTime: " << to_iso_extended_string(jobInfo.second)
			<< " hours"<< endl;
			cout << "  " << curJobP->m_alloyGrade << endl;
		}
	};

	/*
	cout << endl;
	for (auto &jobInfo : jobsWithDueDate)
	{
		Job* curJobP = jobInfo.first;
		 cout << "curJob: " << curJobP->m_jobCode << endl;
		unsigned machIndex(0);  // job�� machine
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // ����ĳ��job������machine
		{
			cout << "curMach: "<< machCodeOfCurJob.first << endl;
			if (CodeOfBellFurn == machCodeOfCurJob.first)  // ������¯
			{
				Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMap[machCodeOfCurJob.first]);
				bool isSuccess = insertJob(*curJobP, *curMachP, machIndex);
			}
			else if (airFurnaceSet.find(machCodeOfCurJob.first) == airFurnaceSet.end()) // ������¯���еĻ���
			{
				Mach_AirFurnace* curMachP = static_cast<Mach_AirFurnace*>(machsMap[machCodeOfCurJob.first]);
				bool isSuccess = insertJob(*curJobP, *curMachP, machIndex);
			}
			else  //����
			{
				Mach* curMachP = machsMap[machCodeOfCurJob.first];
				bool isSuccess = insertJob(*curJobP, *curMachP, machIndex);
			}

			//cout << "curMach: " << curMach.m_machCode <<"\n"<< "isSuccess: " << isSuccess << endl;
			machIndex++;
		}
	}

	// ��ӡ�����ų̽��
	printFinalRes(jobsMap, machsMap);
	
	// ��ȡĿ��ֵ
	pair<double, double> objectVals = getObjVals(jobsMap, machsMap);
	cout << "<������ʱ��(Сʱ): " << objectVals.first<< "; �ӹ����й��������ʱ�䳤��(Сʱ): " <<objectVals.second << endl;
	*/
}
















