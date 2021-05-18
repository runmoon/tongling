#include<head.h>


set<string> rollingMachSet{ "BD-S003","BD-S005","BD-S025","BD-S009","BD-S010","BD-S011" };  // ������
// BD-S003	��������ʽ������; BD-S005	������; BD-S025	�Ĺ�������; 
// BD-S009	ɭ����20��������; BD-S010	20��������; BD-S011	4��������

set<string> cut_StretchMachSet{ "BD-S018","BD-S015",
					"BD-S016","BD-S019","BD-S020","BD-S021" };  // ���ݼ��������
// BD-S018	1250�������; BD-S015	650�����; 
// BD-S016	1250�ݼ�; BD-S019	650���ݼ�; BD-S020	650���ݼ�; BD-S021	350�ݼ�

set<string> washMachSet{ "BD-S012","BD-S013","BD-S014" };
// BD-S012	1250��ϴ����; BD-S013	650��ϴ����; BD-S014	��650��ϴ����; 

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

map<string, string> alloyGrade2Type{
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

struct capacityWithConditions
{
	string m_machCode;
	string m_width;
	string m_thick;
	string m_alloyType;
	string m_status;
	string m_processT;
};

// �ж�_number�Ƿ���_aRange(��ʽΪ"[0.1,3.4]")�ķ�Χ��
bool isInRange(string _aRange, double _number)
{
	if (("null" == _aRange) ||("NULL" == _aRange) 
		|| ("Null" == _aRange) || ("" == _aRange))
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
	if ("BD-S002" == machCode)  //��BD-S002  ����ʽ����¯������Ĭ������
		return "ingotting";
	if ("BD-S007" == machCode) // ��BD-S007  1250����ʽ�˻�¯��
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

	for (auto& cap : machP->m_capsOriginalInfo)
	{
		if (!isInRange(cap.m_width, processTargets.m_targetWeight))
			continue;
		if (!isInRange(cap.m_thick, processTargets.m_targetWeight))
			continue;
		if ((cap.m_alloyType != alloyGrade2Type[jobP->m_alloyGrade])
			&& ("general_alloy" != cap.m_alloyType)
			&& ("null" != cap.m_alloyType)
			&& ("Null" != cap.m_alloyType)
			&& ("NULL" != cap.m_alloyType)
			&& ("" != cap.m_alloyType))
			continue;
		string status = getStatus(machP->m_machCode, jobP, machIndex);
		if ((cap.m_status != status)
			&& ("general_status" != cap.m_status)
			&& ("null" != cap.m_status)
			&& ("Null" != cap.m_status)
			&& ("NULL" != cap.m_status)
			&& ("" != cap.m_status))
			continue;
		processT = stod(cap.m_processT);
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
double mystod(char* str)
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
int mystoi(char* str)
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
	int _n_Sec = int(_processT * 3600);//�ַ���ת����

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



// ��ĳ��������ĳ����
bool  insertJobToMach(Job& curJob, Mach& curMach, unsigned machIndexOfJob)
{
	time_duration& timeDuration = curJob.m_proceTimes[machIndexOfJob].second;
	bool isInserted(false);
	unsigned insertIndexOfMach(0);  // machine�Ĳ��������

	ptime readyTimeForOrder;  // job�����ڸû����Ͽ�ʼ�ӹ���ʱ��
	time_period timeWinToInsert(curJob.m_startDateOfOrder, timeDuration);  //Ҫ�����ʱ�䴰
	if (0 == machIndexOfJob)  // �����job�ĵ�һ��machine
		readyTimeForOrder = curJob.m_startDateOfOrder;
	else
		readyTimeForOrder = (curJob.m_allocatedTimeWin.end()-1)->second.end();

	if (0 == curMach.m_allocatedTimeWin.size())  // �����machine�ϵĵ�һ��job
	{
		timeWinToInsert = time_period(readyTimeForOrder, timeDuration);
		insertIndexOfMach = 0;
		isInserted = true;
	}
	else
	{
		// ����curMach.m_allocatedTimeWin
		for (auto timeInfo_iter = curMach.m_allocatedTimeWin.begin(); timeInfo_iter != curMach.m_allocatedTimeWin.end(); ++timeInfo_iter, ++insertIndexOfMach)
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
	
	curMach.m_allocatedTimeWin.insert(curMach.m_allocatedTimeWin.begin() + insertIndexOfMach, 
		make_pair(make_pair(curJob.m_jobCode, curJob.m_proceMachs[machIndexOfJob].second), timeWinToInsert));
	return true;
};

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
		int orderOfMach = mystoi(row[1]);
		string machCode = row[2];
		
		double processT = mystod(row[3]);
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

		unsigned num_reentry = 1;
		//machCode
		for (pair<string, unsigned>& machInfo : jobP->m_proceMachs)
			num_reentry = (machCode ==machInfo.first)? (num_reentry+1): num_reentry;
		//cout<< jobP->m_jobCode<<", "<< machCode << endl;
		jobP->m_proceMachs.push_back(make_pair(machCode, num_reentry));
		jobP->m_proceTargets.push_back(make_pair(make_pair(machCode, num_reentry), ProcessTargets()));
		time_duration timeDura= double2timeDuration(processT);
		jobP->m_proceTimes.push_back(make_pair(make_pair(machCode, num_reentry), timeDura));
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
		Mach* machP = new Mach;
		machP->m_machCode = row[0];
		machsCodeVec.push_back(machP->m_machCode);
		machsMap[machP->m_machCode] = machP;
		++i;
	}
	cout << "There are " << i << " machines in total." << endl;
}

// ��ʼ��������Ϣ
void initialMachs2(MYSQL_RES* res, vector<string>& machsCodeVec, map<string, Mach*>& machsMap)
{

	Mach *mach_bellFurnace = machsMap["BD-S006"];  // ����¯


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
	
	
	// ��ӡ
	for (auto& jobInfo : jobsMap)  // ��������job
	{
		string curjobcode = jobInfo.first;
		Job* curJobP = jobInfo.second;
		curJobP->m_dueDateOfOrder = from_iso_string((curJobP-> m_dueDateOfOrderStr)+"T115959");
		if (false)
		{
			cout << curjobcode << ": " << curJobP->m_dueDateOfOrderStr << endl;
			cout << curjobcode << ": " << to_iso_string(curJobP->m_dueDateOfOrder) << endl;
		}
		// ���job�����ڸ�ʽ��������!!??
		curJobP->m_startDateOfOrder = curTime;
	}
	
	
	//myInitialization(jobsCodeVec, machsCodeVec, jobsMap, machsMap);
	vector<pair<Job*, ptime>> jobsWithDueDate;               // vector<pair<Job*, ��ֹʱ��>>
	vector<pair<Job*, time_duration>> jobsWithTotalProTime;  // vector<pair<Job*, �ܼӹ�ʱ��>>
	vector<pair<Job*, time_duration>> jobsWithSlackTime;  // vector<pair<Job*, �ɳ�ʱ��>>   �ɳ�ʱ�䣺job��ʣ��ʱ�䣨��ǰʱ����뽻���ڵ�ʱ�䣩���job����ļӹ�ʱ��֮��
	for (auto& jobInfo : jobsMap)  // ��������job��
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
			
			curJobP->m_proceTimes.push_back(make_pair(machInfoOfCurJob, processTime));
			sumOfProcessTime = sumOfProcessTime+ processTime;
			machIndex++;
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

	for (auto& jobInfo : jobsWithSlackTime)  // ��������job
	//for (auto& jobInfo : jobsWithDueDate)  // ��������job
	{
		Job* curJobP = jobInfo.first;
		if(false)
			cout << "Job code: " << curJobP->m_jobCode << "  SlackTime: " << timeDuration2Double(jobInfo.second)
			<< " hours" << endl;
		//cout << "Job code: " << curJobP->m_jobCode << "  DueTime: " << to_iso_extended_string(jobInfo.second)
			//<< endl;
	};

	cout << endl;
	for (auto &jobInfo : jobsWithDueDate)
	{
		Job* curJobP = jobInfo.first;
		// cout << "curJob: " << curJobP->m_jobCode << endl;
		unsigned machIndex(0);  // job�� machine
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // ����ĳ��job������machine
		{
			Mach& curMach = *machsMap[machCodeOfCurJob.first];
			bool isSuccess = insertJobToMach(*curJobP, curMach, machIndex);
			//cout << "curMach: " << curMach.m_machCode <<"\n"<< "isSuccess: " << isSuccess << endl;
			machIndex++;
		}
	}

	// ��ӡ�����ų̽��
	printFinalRes(jobsMap, machsMap);
	
	// ��ȡĿ��ֵ
	pair<double, double> objectVals = getObjVals(jobsMap, machsMap);
	cout << "<������ʱ��(Сʱ): " << objectVals.first<< "; �ӹ����й��������ʱ�䳤��(Сʱ): " <<objectVals.second << endl;
	
}
















