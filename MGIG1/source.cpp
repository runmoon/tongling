#include<head.h>


set<string> rollingMachSet{ "BD-S003","BD-S005","BD-S025","BD-S009","BD-S010","BD-S011" };  // 轧机组
// BD-S003	二辊可逆式热轧机; BD-S005	粗轧机; BD-S025	四辊中轧机; 
// BD-S009	森德威20辊精轧机; BD-S010	20辊精轧机; BD-S011	4辊精轧机

set<string> cut_StretchMachSet{ "BD-S018","BD-S015",
					"BD-S016","BD-S019","BD-S020","BD-S021" };  // 横纵剪和拉弯矫
// BD-S018	1250横剪机组; BD-S015	650拉弯矫; 
// BD-S016	1250纵剪; BD-S019	650薄纵剪; BD-S020	650厚纵剪; BD-S021	350纵剪

set<string> washMachSet{ "BD-S012","BD-S013","BD-S014" };
// BD-S012	1250清洗机列; BD-S013	650清洗机列; BD-S014	新650清洗机列; 

/*
BD-S012	1250清洗机列
BD-S013	650清洗机列
BD - S014	新650清洗机列

BD - S018	1250横剪机组
BD - S015	650拉弯矫

BD - S016	1250纵剪
BD - S019	650薄纵剪
BD - S020	650厚纵剪
BD - S021	350纵剪
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


// 连接mysql数据库
bool ConnectDatabase(MYSQL* mysql)
{
	//初始化mysql  
	mysql_init(mysql);
	char* host("127.0.0.1"); //–MYSQL服务器的地址；
	char* user("root"); //–登录用户名；
	char* passwd("root"); //–登录密码；
	char* db("tongling"); //–要连接的数据库；
	unsigned int port(3306); //–MYSQL服务器的TCP服务端口； ? ?

	//返回false则连接失败，返回true则连接成功
	if (!(mysql_real_connect(mysql, host, user, passwd, db, port, NULL, 0))) //中间分别是主机，用户名，密码，数据库名，端口号（可以写默认0或者3306等），可以先写成参数再传进去  
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

// 查询数据库并返回查询结果
MYSQL_RES* QueryDatabase1(MYSQL* mysql, char* sql)
{
	MYSQL_RES* res;
	//设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码
	mysql_query(mysql, "set names gbk");
	// 查询mysql数据库
	if (mysql_real_query(mysql, sql, (unsigned long)strlen(sql)))
	{
		cout << "mysql_real_query failure! " << endl;
		return 0;
	}
	// 存储结果集
	res = mysql_store_result(mysql);
	if (NULL == res) //  if( !(res = mysql_store_result(&mysql))
	{
		cout << "mysql_store_result failure! " << endl;
		return 0;
	}

	//打印数据行数  
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

// 判断_number是否在_aRange(格式为"[0.1,3.4]")的范围内
bool isInRange(string _aRange, double _number)
{
	if (("null" == _aRange) ||("NULL" == _aRange) 
		|| ("Null" == _aRange) || ("" == _aRange))
		return true;
	size_t site = _aRange.find(",", 0);
	size_t len = _aRange.length();

	if ((0<site)&&(site<len)
		&&(3<len)&&(len<30))  // 必须限制长度，不然不确定
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
			cout<<"ERROR with '" << _aRange <<"'！ 不能解析！ "<<endl;
			return false;
		}
	}
	else
	{
		cout << "ERROR with '" << _aRange << "'！ 不能解析！ " << endl;
		return false;
	}
}; 

// 获取job在当前mach上的status，以便查找产能
string getStatus(string machCode, Job* jobP, unsigned machIndex)
{
	pair<string, unsigned>& machs = (jobP->m_proceMachs)[machIndex];
	if ("BD-S002" == machCode)  //是BD-S002  步进式加热炉，现在默认铸锭
		return "ingotting";
	if ("BD-S007" == machCode) // 是BD-S007  1250气垫式退火炉组
		return "semi_hard_state";
	if (rollingMachSet.count(machCode) > 0) //是轧机
	{
		if ("BD-S003" == machCode) // 是BD-S003	  二辊可逆式热轧机
			return "hot_rolling";
		if (("BD-S009" == machCode)||("BD-S010" == machCode)) // 是BD-S009  森德威20辊精轧机
		{
			return "finished_product"; //判断后面还有没有轧机，没有则为轧机成品，有则为成品
			"intermediate_material";
		}
		// BD-S003	二辊可逆式热轧机, BD-S005	粗轧机; BD-S025	四辊中轧机; 
		// BD-S009	森德威20辊精轧机; BD-S010	20辊精轧机; BD-S011	4辊精轧机
		return "";
		
	}
		
	if (cut_StretchMachSet.count(machCode) > 0) //是剪切或拉矫
	{
		// 判断后面是不是只有包装或没有工序，是则为成品(finished_product)，不是则检查下工序是否是横剪(cross_fedding);
		// 包装线: BD-S022  全自动包装线, BD-S023  简易包装线
		// 若下工序不是横剪，则视为横剪供料(deburring)
		if ((machIndex + 1) == jobP->m_proceMachs.size())  // BD-S016	1250纵剪; BD-S020	650厚纵剪;
			return "finished_product";
		if(("BD-S022" ==jobP->m_proceMachs[machIndex + 1].first)
			|| ("BD-S023" == jobP->m_proceMachs[machIndex + 1].first))
			return "cross_fedding";
		else
			return "deburring";
		// BD-S018	1250横剪机组; BD-S015	650拉弯矫; 
		// BD-S016	1250纵剪; BD-S019	650薄纵剪; BD-S020	650厚纵剪; BD-S021	350纵剪
		if ("BD-S019" == machCode) // 是BD-S019  650薄纵剪
			return "finished_product";
		if ("BD-S015" == machCode) // 是BD-S015  650拉弯矫
			return "";
		return ""; // BD-S021	350纵剪?,BD-S018	1250横剪机组?	
	}
	if (washMachSet.count(machCode) > 0) //是清洗
	{
		if ("BD-S012" == machCode) // 是BD-S012  1250清洗机列
			return "finished_product";
		if ("BD-S013" == machCode) // 是BD-S013	650清洗机列
			return "";
		if ("BD-S014" == machCode)
			return ""; // ?
	}
	return "";
}

// 获取加工时间； 根据mach和job得到job在该mach上的加工时间
time_duration getProcessTime(Mach* machP, Job * jobP, unsigned machIndex)
{
	//return time_duration(2, 0, 0, 0);

	ProcessTargets const& processTargets = jobP->m_proceTargets[machIndex].second;  //??？？
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


// batch组批
void batch()
{


}

// 判断字符串是否是数字
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
		cout<<"Error with str-'"<<str<< "'! Str is NULL！"<<endl;
		return -1;  // 值为空
	}
	else if (isNum(str))
		return stod(str);
	else
	{
		cout << "Error with str-'" << str << "'! Str is NOT A NUMBER！" << endl;
		return -2;  // 不是数字
	}
}

// string to int
int mystoi(char* str)
{
	if (str == NULL)
	{
		cout << "Error with str- " << str << "! Str is NULL！" << endl;
		return -1;  // 值为空
	}
	else if (isNum(str))
		return stoi(str);
	else
	{
		cout << "Error with str- " << str << "! Str is NOT A NUMBER！" << endl;
		return -2;  // 不是数字
	}
}

// 定义比较pair<Job*, ptime>
bool myCmpBy_ptime(pair<Job*, ptime> _a, pair<Job*, ptime> _b)
{
	return _a.second < _b.second;
};

// 定义比较pair<Job*, time_duration>
bool myCmpBy_time_duration(pair<Job*, time_duration> _a, pair<Job*, time_duration> _b)
{
	return _a.second < _b.second;
};

// 小时数（double）转化为time_duration类型
time_duration double2timeDuration(double _processT)
{
	int _n_Sec = int(_processT * 3600);//字符串转整型

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

// time_duration类型转化为小时数（double）
double timeDuration2Double(time_duration _timeDura)
{
	return double(_timeDura.total_seconds()) / 3600;
};



// 把某工单排入某机器
bool  insertJobToMach(Job& curJob, Mach& curMach, unsigned machIndexOfJob)
{
	time_duration& timeDuration = curJob.m_proceTimes[machIndexOfJob].second;
	bool isInserted(false);
	unsigned insertIndexOfMach(0);  // machine的插入的索引

	ptime readyTimeForOrder;  // job可以在该机器上开始加工的时间
	time_period timeWinToInsert(curJob.m_startDateOfOrder, timeDuration);  //要插入的时间窗
	if (0 == machIndexOfJob)  // 如果是job的第一个machine
		readyTimeForOrder = curJob.m_startDateOfOrder;
	else
		readyTimeForOrder = (curJob.m_allocatedTimeWin.end()-1)->second.end();

	if (0 == curMach.m_allocatedTimeWin.size())  // 如果是machine上的第一个job
	{
		timeWinToInsert = time_period(readyTimeForOrder, timeDuration);
		insertIndexOfMach = 0;
		isInserted = true;
	}
	else
	{
		// 遍历curMach.m_allocatedTimeWin
		for (auto timeInfo_iter = curMach.m_allocatedTimeWin.begin(); timeInfo_iter != curMach.m_allocatedTimeWin.end(); ++timeInfo_iter, ++insertIndexOfMach)
		{
			ptime left_TimeWin = timeInfo_iter->second.begin();
			ptime right_TimeWin = timeInfo_iter->second.last();
			if (left_TimeWin < readyTimeForOrder + timeDuration)  // 可加工时间和时间窗开始时间之间一定放不下
				continue;

			if (curMach.m_allocatedTimeWin.cbegin() == timeInfo_iter)  // 第一个time window
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
		}  // END OF 遍历curMach.m_allocatedTimeWin

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

//  调试用的初始化
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
	MYSQL_ROW row;  //一个行数据的类型安全(type-safe)的表示，表示一条数据
	MYSQL_FIELD* field = NULL;

	vector<string> fieldsNames_orders;
	//字段的个数
	int fieldcount = mysql_num_fields(res);
	cout << "Number of fields: " << fieldcount << endl;
	//显示所有字段名
	for (int i = 0; i < fieldcount; ++i)
	{
		field = mysql_fetch_field_direct(res, i);
		cout << field->name << "\t";
		fieldsNames_orders.push_back(field->name);
	}
	cout << endl;
	int i = 0;  // 行数
	while (row = mysql_fetch_row(res))
	{
		// unit_id, width, thick, alloy_name, status, process_time, capacity, explanation
		// 重复读取行，并输出获取每行中字段的值，直到row为NULL

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


// 数据库读取工单信息并初始化
void initializeJobs(MYSQL_RES* res, vector<string>& jobsCodeVec, map<string, Job*>& jobsMap)
{
	MYSQL_ROW row;  //一个行数据的类型安全(type-safe)的表示，表示一条数据
	MYSQL_FIELD* field = NULL;

	vector<string> fieldsNames_orders;
	//字段的个数
	int fieldcount = mysql_num_fields(res);
	cout << "Number of fields: " << fieldcount << endl;
	//显示所有字段名
	for (int i = 0; i < fieldcount; i++)
	{
		field = mysql_fetch_field_direct(res, i);
		cout << field->name << "\t";
		fieldsNames_orders.push_back(field->name);
	}
	cout << endl;
	int i = 0;  // 行数
	while (row = mysql_fetch_row(res))
	{
		// strip_id, deadline, thick, width, note, length, weight, inner_diameter, alloy_grade
		// 重复读取行，并输出获取每行中字段的值，直到row为NULL
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

// 初始化工单——完善工单信息（加工目标和加工机器）
void initializeJobs2(MYSQL_RES* res, vector<string>& jobsCodeVec, map<string, Job*>& jobsMap)
{
	MYSQL_ROW row;  //一个行数据的类型安全(type-safe)的表示，表示一条数据
	MYSQL_FIELD* field = NULL;

	vector<string> fieldsNames_orders;
	//字段的个数
	int fieldcount = mysql_num_fields(res);
	cout << "Number of fields: " << fieldcount << endl;
	//显示所有字段名
	for (int i = 0; i < fieldcount; ++i)
	{
		field = mysql_fetch_field_direct(res, i);
		cout << field->name << "\t";
		fieldsNames_orders.push_back(field->name);
	}
	cout << endl;
	int i = 0;  // 行数
	while (row = mysql_fetch_row(res))
	{
		// strip_id, process_num, unit_id, thick, width, note, unit_name_orig
		// 重复读取行，并输出获取每行中字段的值，直到row为NULL
		string jobCode = row[0];
		int orderOfMach = mystoi(row[1]);
		string machCode = row[2];
		
		double processT = mystod(row[3]);
		Job* jobP;
		if (jobsMap.find(jobCode) == jobsMap.end()) // 如果jobCode不在jobsMap中，
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

// 数据库读取机器信息并初始化
void initialMachs(MYSQL_RES* res, vector<string>& machsCodeVec, map<string, Mach*>& machsMap)
{
	MYSQL_ROW row;  //一个行数据的类型安全(type-safe)的表示，表示一条数据
	MYSQL_FIELD* field = NULL;

	vector<string> fieldsNames_orders;
	//字段的个数
	int fieldcount = mysql_num_fields(res);
	cout << "Number of fields: " << fieldcount << endl;
	//显示所有字段名
	for (int i = 0; i < fieldcount; i++)
	{
		field = mysql_fetch_field_direct(res, i);
		cout << field->name << "\t";
		fieldsNames_orders.push_back(field->name);
	}
	cout << endl;
	int i = 0;  // 行数
	while (row = mysql_fetch_row(res))
	{
		// unit_id, name
		// 重复读取行，并输出获取每行中字段的值，直到row为NULL
		Mach* machP = new Mach;
		machP->m_machCode = row[0];
		machsCodeVec.push_back(machP->m_machCode);
		machsMap[machP->m_machCode] = machP;
		++i;
	}
	cout << "There are " << i << " machines in total." << endl;
}

// 初始化机器信息
void initialMachs2(MYSQL_RES* res, vector<string>& machsCodeVec, map<string, Mach*>& machsMap)
{

	Mach *mach_bellFurnace = machsMap["BD-S006"];  // 钟罩炉


	MYSQL_ROW row;  //一个行数据的类型安全(type-safe)的表示，表示一条数据
	MYSQL_FIELD* field = NULL;

	vector<string> fieldsNames_orders;
	//字段的个数
	int fieldcount = mysql_num_fields(res);
	cout << "Number of fields: " << fieldcount << endl;
	//显示所有字段名
	for (int i = 0; i < fieldcount; i++)
	{
		field = mysql_fetch_field_direct(res, i);
		cout << field->name << "\t";
		fieldsNames_orders.push_back(field->name);
	}
	cout << endl;
	int i = 0;  // 行数
	while (row = mysql_fetch_row(res))
	{
		// unit_id, name
		// 重复读取行，并输出获取每行中字段的值，直到row为NULL
		Mach* machP = new Mach;
		machP->m_machCode = row[0];
		machsCodeVec.push_back(machP->m_machCode);
		machsMap[machP->m_machCode] = machP;
		++i;
	}
	cout << "There are " << i << " machines in total." << endl;
}


// 获取目标函数值 <总延期时间(小时), 加工所有工件所需的时间长度(小时)>
pair<double, double> getObjVals(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap)  // get object value
{
	double totalDueTime(0);  // 总延期时间（小时）
	ptime timeOfStart;       // 开始排程的时间点
	ptime timeOfCompletion;  // 所有工件加工完的时间点
	double makeSpan(0);      // 加工所有工件所需的时间长度（小时）
	int i_numOfJob(0);
	for (auto& jobInfo : jobsMap)  // 遍历所有job
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
	cout << "totalDueTime=　" << totalDueTime << endl;
	cout << "makeSpan = " << makeSpan << endl;
	cout << endl;
	return make_pair(totalDueTime, makeSpan);
};


// 打印最终排程结果
void printFinalRes(map<string, Job*> &jobsMap, map<string, Mach*> &machsMap)
{
	// 打印排程结果
	cout << endl;
	for (auto& machInfo : machsMap)  // 遍历所有machine
	{
		Mach& curMach = *(machInfo.second);
		cout << "MachCode---------" << curMach.m_machCode << endl;
		for (auto& timeInfo : curMach.m_allocatedTimeWin)  // 遍历所有time_period
		{
			string jobCode = timeInfo.first.first;
			time_period timeWin = timeInfo.second;
			cout << "JobCode  " << jobCode << ": " << to_iso_extended_string(timeWin.begin())
				<< "--" << to_iso_extended_string(timeWin.last()) << endl;
		}
		cout << endl;
	};
	cout << "\n\n" << endl;
	for (auto& jobInfo : jobsMap)  // 遍历所有job
	{
		Job* curJobP = jobInfo.second;
		cout << "JobCode---------" << curJobP->m_jobCode << endl;
		for (auto& timeInfo : curJobP->m_allocatedTimeWin)  // 遍历所有time_period
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
	ptime curTime = from_iso_string("20200607T000000");  // 当前时间
	//ptime curTime = from_iso_string("20200501T0000000");

	
	MYSQL* mysql = new MYSQL;
	MYSQL_RES* res(NULL);   //这个结构代表返回行的一个查询结果集  
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
	
	
	// 打印
	for (auto& jobInfo : jobsMap)  // 遍历所有job
	{
		string curjobcode = jobInfo.first;
		Job* curJobP = jobInfo.second;
		curJobP->m_dueDateOfOrder = from_iso_string((curJobP-> m_dueDateOfOrderStr)+"T115959");
		if (false)
		{
			cout << curjobcode << ": " << curJobP->m_dueDateOfOrderStr << endl;
			cout << curjobcode << ": " << to_iso_string(curJobP->m_dueDateOfOrder) << endl;
		}
		// 检查job的日期格式！！？？!!??
		curJobP->m_startDateOfOrder = curTime;
	}
	
	
	//myInitialization(jobsCodeVec, machsCodeVec, jobsMap, machsMap);
	vector<pair<Job*, ptime>> jobsWithDueDate;               // vector<pair<Job*, 截止时间>>
	vector<pair<Job*, time_duration>> jobsWithTotalProTime;  // vector<pair<Job*, 总加工时间>>
	vector<pair<Job*, time_duration>> jobsWithSlackTime;  // vector<pair<Job*, 松弛时间>>   松弛时间：job的剩余时间（当前时点距离交货期的时间）与该job所需的加工时间之差
	for (auto& jobInfo : jobsMap)  // 遍历所有job，
	{
		string curJobCode = jobInfo.first;
		
		Job *curJobP = jobInfo.second;
		jobsWithDueDate.push_back(make_pair(curJobP, curJobP->m_dueDateOfOrder));
		time_duration sumOfProcessTime;
		unsigned machIndex(0);

		for (pair<string, unsigned>& machInfoOfCurJob : curJobP->m_proceMachs)   // 遍历某个job的所有machine
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
		time_duration slackTime = (curJobP->m_dueDateOfOrder-curTime) - sumOfProcessTime;  //job的剩余时间（当前时点距离交货期的时间）与该job所需的加工时间之差
		jobsWithSlackTime.push_back(make_pair(curJobP, slackTime));
		jobsWithTotalProTime.push_back(make_pair(curJobP, sumOfProcessTime));
	}

	// 根据截止时间，总加工时间，松弛时间排序
	sort(jobsWithDueDate.begin(), jobsWithDueDate.end(), myCmpBy_ptime);
	sort(jobsWithTotalProTime.begin(), jobsWithTotalProTime.end(), myCmpBy_time_duration);
	sort(jobsWithSlackTime.begin(), jobsWithSlackTime.end(), myCmpBy_time_duration);

	for (auto& jobInfo : jobsWithSlackTime)  // 遍历所有job
	//for (auto& jobInfo : jobsWithDueDate)  // 遍历所有job
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
		unsigned machIndex(0);  // job的 machine
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // 遍历某个job的所有machine
		{
			Mach& curMach = *machsMap[machCodeOfCurJob.first];
			bool isSuccess = insertJobToMach(*curJobP, curMach, machIndex);
			//cout << "curMach: " << curMach.m_machCode <<"\n"<< "isSuccess: " << isSuccess << endl;
			machIndex++;
		}
	}

	// 打印最终排程结果
	printFinalRes(jobsMap, machsMap);
	
	// 获取目标值
	pair<double, double> objectVals = getObjVals(jobsMap, machsMap);
	cout << "<总延期时间(小时): " << objectVals.first<< "; 加工所有工件所需的时间长度(小时): " <<objectVals.second << endl;
	
}
















