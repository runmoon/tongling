#pragma once
#include<head.h>
#include<GA.h>

pair<double, pair<int, int>> Mach_BellFurnace::m_RuleForFurnWithWidth = make_pair(1950, make_pair(2, 3));



// --------集合相关，机器代码和合金种类--------

set<string> rollingmachFrequ{ "BD-S003","BD-S005","BD-S025","BD-S009","BD-S010","BD-S011" };  // 轧机组; getStatus()用
// BD-S003	二辊可逆式热轧机; BD-S005	粗轧机; BD-S025	四辊中轧机; 
// BD-S009	森德威20辊精轧机; BD-S010	20辊精轧机; BD-S011	4辊精轧机

set<string> cut_StretchmachFrequ{ "BD-S018","BD-S015",
					"BD-S016","BD-S019","BD-S020","BD-S021" };  // 横纵剪和拉弯矫; getStatus()用
// BD-S018	1250横剪机组; BD-S015	650拉弯矫; 
// BD-S016	1250纵剪; BD-S019	650薄纵剪; BD-S020	650厚纵剪; BD-S021	350纵剪

set<string> washmachFrequ{ "BD-S012","BD-S013","BD-S014" };     // BD-S012	1250清洗机列; BD-S013	650清洗机列; BD-S014	新650清洗机列; getStatus()用

set<string> airFurnaceSet{ "BD-S024","BD-S007","BD-S008" };     //BD-S024  WSP气垫炉;    BD-S007  1250气垫式退火炉组;    BD-S008  650气垫式退火炉组

map<string, string> alloyMap{    // 由合金牌号（grade）查对应的合金种类（type）
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
	make_pair("BD-S002", "步进式加热炉"),
	make_pair("BD-S003", "二辊可逆式热轧机"),
	make_pair("BD-S004", "双面铣削机组"),

	make_pair("BD-S005", "粗轧机"),
	make_pair("BD-S025", "四辊中轧机"),

	make_pair("BD-S024", "WSP气垫炉"),
	make_pair("BD-S007", "1250气垫式退火炉组"),
	make_pair("BD-S008", "650气垫式退火炉组"),
	make_pair("BD-S006", "钟罩式退火炉"),

	make_pair("BD-S012", "1250清洗机列"),
	make_pair("BD-S013", "650清洗机列"),
	make_pair("BD-S014", "新650清洗机列"),

	make_pair("BD-S009", "sundwig20辊精轧机"),
	make_pair("BD-S010", "20辊精轧机"),
	make_pair("BD-S011", "4辊精轧机"),

	make_pair("BD-S015", "650拉弯矫"),

	make_pair("BD-S016", "1250纵剪"),
	make_pair("BD-S018", "1250横剪机组"),
	make_pair("BD-S019", "650薄纵剪"),
	make_pair("BD-S020", "650厚纵剪"),
	make_pair("BD-S021", "350纵剪"),

	make_pair("BD-S022", "全自动包装线"),
	make_pair("BD-S023", "简易包装线"),

	make_pair("BD-S026", "新研磨研削机"),
};

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

// --------END OF--集合相关，机器代码和合金种类--------



// 设定开始排产的时间，并转化成ptime格式
ptime getCurTime() {
	ptime curTime = from_iso_string("20200610T000000");
	return curTime;
};


// 由合金牌号获取合金类型
string getTypeFromGrade(const string& alloyGrade)
{
	return alloyMap[alloyGrade];
}


// 获取工单当前的宽度、厚度，长度，重量，内径

double getCurWidth(Job* jobP, int machIndexOfJob)
{
	double curWidth(0); //job的当前宽度
	for (int i = machIndexOfJob; i >= 0; --i)
	{
		if (jobP->m_proceTargets[i].second.m_targetWidth > 0.0)  // 从当前，往前找目标宽度大于0的值 
		{
			curWidth = jobP->m_proceTargets[i].second.m_targetWidth;
			break;
		}
	}
	if (curWidth <= 0.0)   // 若目标宽度都≤0 
		curWidth = jobP->m_initialInfos.m_targetWidth;
	return curWidth;
};

double getCurWidth(Job* jobP, string machCode, int reentry)
{
	int machIndexOfJob = -1;
	for (int i = 0; i < jobP->m_proceMachs.size(); ++i) {
		if (jobP->m_proceMachs[i].first == machCode && jobP->m_proceMachs[i].second == reentry) {
			machIndexOfJob = i;
			break;
		}
	}
	if (machIndexOfJob == -1) cout << "Error! The 'machCode' and 'reentry' is not right!" << endl;

	double curWidth(0); //job的当前宽度
	for (int i = machIndexOfJob; i >= 0; --i)
	{
		if (jobP->m_proceTargets[i].second.m_targetWidth > 0.0)  // 从当前，往前找目标宽度大于0的值 
		{
			curWidth = jobP->m_proceTargets[i].second.m_targetWidth;
			break;
		}
	}
	if (curWidth <= 0.0)   // 若目标宽度都≤0 
		curWidth = jobP->m_initialInfos.m_targetWidth;
	return curWidth;
};

double getCurThick(Job* jobP, int machIndexOfJob)
{
	double curThick(0); //job的当前宽度
	for (int i = machIndexOfJob; i >= 0; --i)
	{
		if (jobP->m_proceTargets[i].second.m_targetThick > 0.0)  // 从当前，往前找目标宽度大于0的值 
		{
			curThick = jobP->m_proceTargets[i].second.m_targetThick;
			break;
		}
	}
	if (curThick <= 0.0)   // 若目标宽度都≤0 
		curThick = jobP->m_initialInfos.m_targetThick;
	return curThick;
};

double getCurLength(Job* jobP, int machIndexOfJob)
{
	double curLength(0); //job的当前宽度
	for (int i = machIndexOfJob; i >= 0; --i)
	{
		if (jobP->m_proceTargets[i].second.m_targetLength > 0.0)  // 从当前，往前找目标宽度大于0的值 
		{
			curLength = jobP->m_proceTargets[i].second.m_targetLength;
			break;
		}
	}
	if (curLength <= 0.0)   // 若目标宽度都≤0 
		curLength = jobP->m_initialInfos.m_targetLength;
	return curLength;
};

double getCurWeight(Job* jobP, int machIndexOfJob)
{
	double curWeight(0); //job的当前宽度
	for (int i = machIndexOfJob; i >= 0; --i)
	{
		if (jobP->m_proceTargets[i].second.m_targetWeight > 0.0)  // 从当前，往前找目标宽度大于0的值 
		{
			curWeight = jobP->m_proceTargets[i].second.m_targetWeight;
			break;
		}
	}
	if (curWeight <= 0.0)   // 若目标宽度都≤0 
		curWeight = jobP->m_initialInfos.m_targetWeight;
	return curWeight;
};

double getCurInnerDia(Job* jobP, int machIndexOfJob)
{
	double curInnerDia(0); //job的当前宽度
	for (int i = machIndexOfJob; i >= 0; --i)
	{
		if (jobP->m_proceTargets[i].second.m_targetWeight > 0.0)  // 从当前，往前找目标宽度大于0的值 
		{
			curInnerDia = jobP->m_proceTargets[i].second.m_targetWeight;
			break;
		}
	}
	if (curInnerDia <= 0.0)   // 若目标宽度都≤0 
		curInnerDia = jobP->m_initialInfos.m_targetInnerDia;
	return curInnerDia;
};



// --------MySQL数据库交互--------

// 初始化设置，连接mysql数据库
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
		std::cout << "mysql_real_query failure! " << std::endl;
		return 0;
	}
	// 存储结果集
	res = mysql_store_result(mysql);
	if (NULL == res) //  if( !(res = mysql_store_result(&mysql))
	{
		std::cout << "mysql_store_result failure! " << std::endl;
		return 0;
	}

	//打印数据行数  
	//std::cout << "number of dataline returned: " << mysql_affected_rows(mysql) << std::endl;
	std::cout << "number of dataline returned: " << mysql_num_rows(res) << std::endl;
	return res;
}

// 向数据库插入数据，并返回插入是否成功
bool InsertDatabase1(MYSQL* mysql, char* sql)
{
	//设置编码格式（SET NAMES GBK也行），否则cmd下中文乱码
	mysql_query(mysql, "set names gbk");

	//mysql_query(mysql, "START TRANSACTION");
	// 查询mysql数据库
	cout << "iinsertting" << endl;
	if (mysql_query(mysql, sql))
	{
		std::cout << "mysql_query failure! " << std::endl;
		return 0;
	}
	//mysql_query(mysql, "COMMIT");
	return true;
}

// --------END OF--MySQL数据库交互--------



// --------产能解析相关--------

// 判断_number是否在_aRange(格式为"[0.1,3.4]")的范围内
bool isInRange(string _aRange, double _number)
{
	if (("null" == _aRange) || ("NULL" == _aRange)
		|| ("Null" == _aRange) || ("" == _aRange)
		|| (_number <= 0.0))
		return true;
	size_t site = _aRange.find(",", 0);
	size_t len = _aRange.length();

	if ((0 < site) && (site < len)
		&& (3 < len) && (len < 30))  // 必须限制长度，不然不确定
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
			std::cout << "ERROR with '" << _aRange << "'！ 不能解析！ " << std::endl;
			return false;
		}
	}
	else
	{
		std::cout << "ERROR with '" << _aRange << "'！ 不能解析！ " << std::endl;
		return false;
	}
};

// 获取job在当前mach上的status，以便查找产能
string getStatus(string machCode, Job* jobP, unsigned machIndex)
{
	pair<string, unsigned>& machs = (jobP->m_proceMachs)[machIndex];
	if ("BD-S002" == machCode)  //是BD-S002  步进式加热炉，现在默认铸锭??？？
		return "ingotting";
	if ("BD-S007" == machCode) // 是BD-S007  1250气垫式退火炉组??？？
		return "semi_hard_state";

	if (rollingmachFrequ.count(machCode) > 0) //是轧机
	{
		if ("BD-S003" == machCode) // 是BD-S003	  二辊可逆式热轧机
			return "hot_rolling";
		if (("BD-S009" == machCode) || ("BD-S010" == machCode)) // 是BD-S009  森德威20辊精轧机
		{
			return "finished_product"; //判断后面还有没有轧机，没有则为轧机成品，有则为成品
			"intermediate_material";
		}
		// BD-S003	二辊可逆式热轧机, BD-S005	粗轧机; BD-S025	四辊中轧机; 
		// BD-S009	森德威20辊精轧机; BD-S010	20辊精轧机; BD-S011	4辊精轧机
		return "";
	}

	if (cut_StretchmachFrequ.count(machCode) > 0) //是剪切或拉矫
	{
		// 判断后面是不是只有包装或没有工序，是则为成品(finished_product)，不是则检查下工序是否是横剪(cross_fedding);
		// 包装线: BD-S022  全自动包装线, BD-S023  简易包装线
		// 若下工序不是横剪，则视为横剪供料(deburring)
		if ((machIndex + 1) == jobP->m_proceMachs.size())  // BD-S016	1250纵剪; BD-S020	650厚纵剪;
			return "finished_product";
		if (("BD-S022" == jobP->m_proceMachs[machIndex + 1].first)
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

	if (washmachFrequ.count(machCode) > 0) //是清洗
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
time_duration getProcessTime(Mach* machP, Job* jobP, unsigned machIndex)
{
	//return time_duration(2, 0, 0, 0);

	ProcessTargets const& processTargets = jobP->m_proceTargets[machIndex].second;  //??？？
	double processT(0);
	// bool isRollingMach(rollingmachFrequ.count(machP->m_machCode) > 0);
	if (CodeOfBellFurn == machP->m_machCode)  // 如果是钟罩炉
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
		cout << "    m_targetWeight="<<processTargets.m_targetThick << endl;
		cout << "    m_alloyType="<<jobP->m_alloyType << endl;
		cout << "    status="<<getStatus(machP->m_machCode, jobP, machIndex) << endl;
	}

	//std::cout << "machCode=" << machP->m_machCode << "; jobCode="<<jobP->m_jobCode <<"; cap="<<processT<< std::endl;
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
	if (!(sin >> d))  //sin >> t表示把sin转换成double的变量（其实对于int和float型的都会接收），如果转换成功，则值为非0，如果转换不成功就返回为0
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
		std::cout << "Error with str-'" << str << "'! Str is NULL！" << std::endl;
		return -1;  // 值为空
	}
	else if (isNum(str))
		return stod(str);
	else
	{
		std::cout << "Error with str-'" << str << "'! Str is NOT A NUMBER！" << std::endl;
		return -2;  // 不是数字
	}
}

// string to int
int mystoi(const char* str)
{
	if (str == NULL)
	{
		std::cout << "Error with str- " << str << "! Str is NULL！" << std::endl;
		return -1;  // 值为空
	}
	else if (isNum(str))
		return stoi(str);
	else
	{
		std::cout << "Error with str- " << str << "! Str is NOT A NUMBER！" << std::endl;
		return -2;  // 不是数字
	}
}

// --------END OF--产能解析相关--------



// --------时间相关--------

// 定义比较pair<Job*, ptime>
bool myCmpBy_ptime(pair<Job*, ptime> _a, pair<Job*, ptime> _b)
{
	return _b.second < _a.second;
};

// 定义比较pair<Job*, time_duration>
bool myCmpBy_time_duration(pair<Job*, time_duration> _a, pair<Job*, time_duration> _b)
{
	return _a.second < _b.second;
};

// 小时数（double）转化为time_duration类型
time_duration double2timeDuration(double _processT)
{
	int _n_Sec = int(_processT * 3600);//转整型

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

// --------END OF--时间相关--------



// --------排产，把某工单排入某机器的空闲时间--------

// 把某工单排入某机器
bool  insertJob(Job& curJob, Mach& curMach, unsigned machIndexOfJob)
{
	time_duration& processTime = curJob.m_proceTimes[machIndexOfJob].second;
	//time_duration& swithT = double2timeDuration(curMach.m_timeOfSwith);  // 切换时间
	//bool swith = isSwitchPre(curJob, curMach, machIndexOfJob);  // 是否需要切换

	bool isInserted(false);
	unsigned IndexOfTimeWin(0);  // machine的插入的索引

	ptime readyTimeForOrder;  // job可以在该机器上开始加工的时间
	time_period timeWinToInsert(curJob.m_startDateOfOrder, processTime);  //要插入的时间窗
	if (0 == machIndexOfJob)  // 如果是job的第一个machine
		readyTimeForOrder = curJob.m_startDateOfOrder;
	else
		readyTimeForOrder = (curJob.m_allocatedTimeWin.end() - 1)->second.end();

	if (0 == curMach.m_allocatedTimeWin.size())  // 如果是machine上的第一个job
	{
		timeWinToInsert = time_period(readyTimeForOrder, processTime);
		IndexOfTimeWin = 0;
		isInserted = true;
	}
	else
	{
		// 遍历curMach.m_allocatedTimeWin
		for (auto timeInfo_iter = curMach.m_allocatedTimeWin.begin(); timeInfo_iter != curMach.m_allocatedTimeWin.end(); ++timeInfo_iter, ++IndexOfTimeWin)
		{
			ptime left_TimeWin = timeInfo_iter->second.begin();
			ptime right_TimeWin = timeInfo_iter->second.last();
			if (left_TimeWin < readyTimeForOrder + processTime)  // 可加工时间和时间窗开始时间之间一定放不下
				continue;

			if (curMach.m_allocatedTimeWin.cbegin() == timeInfo_iter)  // 第一个time window
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
		}  // END OF 遍历curMach.m_allocatedTimeWin

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
	for (int i = 0; i < IndexOfTimeWin; ++i) ++iter;
	curMach.m_allocatedTimeWin.insert(iter,
		make_pair(make_pair(curJob.m_jobCode, curJob.m_proceMachs[machIndexOfJob].second), timeWinToInsert));
	return true;
};;

// 把某工单排入某个气垫炉
bool  insertJob(Job& curJob, Mach_AirFurnace& curMach, unsigned machIndexOfJob, map<string, Job*>& jobsMap)
{
	time_duration& processTime = curJob.m_proceTimes[machIndexOfJob].second;
	time_duration& switchT = double2timeDuration(curMach.m_timeOfSwith);

	bool isInserted(false);
	unsigned IndexOfTimeWin(0);  // machine的插入的索引

	ptime readyTimeForOrder;  // job可以在该机器上开始加工的时间
	time_period timeWinToInsert(curJob.m_startDateOfOrder, processTime);  //要插入的时间窗
	bool isSwitchPre;  // 考虑前面的机器，中间是否需要转换时间
	if (0 == machIndexOfJob)  // 如果是job的第一个machine
		readyTimeForOrder = curJob.m_startDateOfOrder;
	else
		readyTimeForOrder = (curJob.m_allocatedTimeWin.end() - 1)->second.end();
	//if ("BD-S008" == curMach.m_machCode) cout << "yyyyyyyyyyyyy" << endl;

	if (0 == curMach.m_allocatedTimeWin.size())  // 如果是machine上的第一个job
	{
		timeWinToInsert = time_period(readyTimeForOrder, processTime);
		IndexOfTimeWin = 0;
		isInserted = true;
		isSwitchPre = true;
	}
	else
	{
		// 遍历curMach.m_allocatedTimeWin
		for (auto timeInfo_iter = curMach.m_allocatedTimeWin.begin();
			timeInfo_iter != curMach.m_allocatedTimeWin.end(); ++timeInfo_iter, ++IndexOfTimeWin)
		{
			ptime left_TimeWin = timeInfo_iter->second.begin();
			ptime right_TimeWin = timeInfo_iter->second.last();
			//if ("BD-S008" == curMach.m_machCode) cout << "uppppppppppp" << endl;
			bool isSwitchNext = getIsSwitch(curJob, curMach, *jobsMap[timeInfo_iter->first.first]);    // 考虑后面的机器，是否需要转换时间

			if (left_TimeWin < readyTimeForOrder + processTime + (isSwitchNext ? switchT : double2timeDuration(0.0)))  // 可加工时间和时间窗开始时间之间一定放不下
				continue;

			if (curMach.m_allocatedTimeWin.cbegin() == timeInfo_iter)  // 第一个time window
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
		}  // END OF 遍历curMach.m_allocatedTimeWin

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
	for (int i = 0; i < IndexOfTimeWin; ++i) iter++;
	curMach.m_allocatedTimeWin.insert(iter,
		make_pair(make_pair(curJob.m_jobCode, curJob.m_proceMachs[machIndexOfJob].second), timeWinToInsert));
	return true;
};;

// 把某工单排入钟罩炉的某个平行机
bool  insertJob(Job& curJob, Mach_BellFurnace& curMach, unsigned machIndexOfJob, map<string, Job*> &jobsMap)
{
	time_duration& processTime = curJob.m_proceTimes[machIndexOfJob].second;
	pair<pair<int, int>, time_period> newFurnRes = (make_pair(pair<unsigned, unsigned>(make_pair(-1, -1))
		, time_period(curJob.m_startDateOfOrder, processTime))); // 记录新开一炉的情况; pair<pair< 第几个timeline, 插入位置>, 时间窗>
	pair<pair<int, int>, time_period> notNewFurnRes = (make_pair(pair<unsigned, unsigned>(make_pair(-1, -1))
		, time_period(curJob.m_startDateOfOrder, processTime))); // 记录插入已有炉的情况; pair<pair< 第几个timeline, 插入位置>, 时间窗>

	unsigned Index_parallelMach = 0;
	pair<int, bool> resultOfPreInsert(make_pair(-1, false));   // pair<TimeWin的位置, 是否要新开一炉>
	for (auto& timeline : curMach.m_timeLines)
	{
		time_period timeWinResult(curJob.m_startDateOfOrder, processTime);

		resultOfPreInsert = preInsertJobToMach(curJob, curMach, machIndexOfJob, timeline, timeWinResult, jobsMap);

		if (resultOfPreInsert.second) // 是新开1炉
		{
			if (newFurnRes.first.first < 0.0)
				newFurnRes = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
			else if (newFurnRes.second.begin() > timeWinResult.begin())
				newFurnRes = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
		}
		else // 不是新开1炉
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
	int insertPosition(-1);  // 插入位置
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
	if (isNew) {  // 新开一炉
		timeLine.insert(iterTmp, make_pair(jobToInsert, timeWinFinal));
	}
	else  // 加入已开的炉
		iterTmp->first.insert(make_pair(curJob.m_jobCode, num_reentry));

	return true;
};

// 把某工单排入钟罩炉的某个平行机
bool  insertJob(Job& curJob, Mach_BellFurnace& curMach, unsigned machIndexOfJob, bool yes, map<string, Job*> jobsMap)
{
	map<string, vector<Job*>> waitForBell;  //map<牌号, vector<Job*>>
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
			//组炉
		}
		else
		{
			//if ()  // 还有，则等待
			//else
				//
		}

	}


	time_duration& processTime = curJob.m_proceTimes[machIndexOfJob].second;
	pair<pair<int, int>, time_period> newFurnRes = (make_pair(pair<unsigned, unsigned>(make_pair(-1, -1))
		, time_period(curJob.m_startDateOfOrder, processTime))); // 记录新开一炉的情况; pair<pair< 第几个timeline, 插入位置>, 时间窗>
	pair<pair<int, int>, time_period> notNewFurnRes = (make_pair(pair<unsigned, unsigned>(make_pair(-1, -1))
		, time_period(curJob.m_startDateOfOrder, processTime))); // 记录插入已有炉的情况; pair<pair< 第几个timeline, 插入位置>, 时间窗>

	unsigned Index_parallelMach = 0;
	pair<int, bool> resultOfPreInsert(make_pair(-1, false));   // pair<TimeWin的位置, 是否要新开一炉>
	for (auto& timeline : curMach.m_timeLines)
	{
		time_period timeWinResult(curJob.m_startDateOfOrder, processTime);

		resultOfPreInsert = preInsertJobToMach(curJob, curMach, machIndexOfJob, timeline, timeWinResult, jobsMap);

		if (resultOfPreInsert.second) // 是新开1炉
		{
			if (newFurnRes.first.first < 0.0)
				newFurnRes = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
			else if (newFurnRes.second.begin() > timeWinResult.begin())
				newFurnRes = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
		}
		else // 不是新开1炉
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
	int insertPosition(-1);  // 插入位置
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

	if (isNew) // 新开一炉
		timeLine.insert(iterTmp, make_pair(jobToInsert, timeWinFinal));
	else  // 加入已开的炉
		iterTmp->first.insert(make_pair(curJob.m_jobCode, num_reentry));

	return true;
};

// 把某工单排入钟罩炉的某平行机，返回插入时间窗和插入索引
pair<unsigned, bool> preInsertJobToMachWithPreBatch(ptime readyTimeForOrder, Mach_BellFurnace& curMach, list<TimeWin>& timeline, time_period& timeWinResult)
{
	time_duration& processTime = double2timeDuration(curMach.m_proceTimePerFurn);  //钟罩炉时间为30小时
	//time_duration timeOfSwith = double2timeDuration(curMach.m_timeOfSwith);  // 开启新的一炉的转换时间
	//准备时间和上一炉的加工完成时间，是否要记入转换时间，需要搞明白？？??
	//   对job来讲，准备时间后不需要转换时间；对machine来讲，上一炉结束时间后需要转换时间！

	unsigned IndexOfTimeWin(0);  // machine的时间线插入的索引
	// using TimeWin = pair<vector<pair<Job*, unsigned>>, time_period>;  // pair< vector<pair<Job*, 该job第几次重入>>, job插入的时间窗>

	bool flag_NewFurn(false);  // 显示是否新开一炉
	bool isInserted(false);

	//cout << " timeline.size()=" << timeline.size()
	//	<< "  readyTimeForOrder="<<to_iso_extended_string(readyTimeForOrder)<<endl;

	if (0 == timeline.size())  // 如果是machine上的第一个job
	{
		timeWinResult = time_period(readyTimeForOrder + double2timeDuration(curMach.m_waitTofFirstFurn), processTime); //??？？!!!!!!!!!
		flag_NewFurn = true;  // 新开一炉
		IndexOfTimeWin = 0;
		isInserted = true;
	}
	else
	{

		IndexOfTimeWin = 0;
		// 遍历timeline
		for (list<TimeWin>::iterator timeInfo_iter = timeline.begin();
			timeInfo_iter != timeline.end(); ++timeInfo_iter, ++IndexOfTimeWin)
		{
			ptime left_TimeWin = timeInfo_iter->second.begin();
			ptime right_TimeWin = timeInfo_iter->second.last();

			if (left_TimeWin < readyTimeForOrder + processTime)  // 可加工时间和时间窗开始时间之间一定放不下新的一炉
				continue;

			if (timeline.cbegin() == timeInfo_iter)  // 第一个time window
			{
				//std::cout << "**************** " << std::endl;
				timeWinResult = time_period(readyTimeForOrder, processTime);
				flag_NewFurn = true;    // 新开一炉
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
					flag_NewFurn = true;    // 新开一炉
					isInserted = true;
					break;
				}
				if (right_Pre <= readyTimeForOrder)
				{
					timeWinResult = time_period(readyTimeForOrder, processTime);
					flag_NewFurn = true;    // 新开一炉
					isInserted = true;
					break;
				}
			}
		}  // END OF 遍历timeline

		if (false == isInserted)
		{
			//cout << "dsdssdsdsdsdsdsd"<< "flag_NewFurn=" << flag_NewFurn << "isInserted=" << isInserted << endl;
			auto iterTmp = timeline.end();
			ptime rightWin = (--iterTmp)->second.end();
			timeWinResult = time_period((readyTimeForOrder < rightWin) ? rightWin : readyTimeForOrder, processTime);
			flag_NewFurn = true;    // 新开一炉
			isInserted = true;
		}
	}
	//std::cout << "curJob=" << curJob.m_jobCode <<" 777777"<< std::endl;
	//std::cout << "      timeWinResult=" << to_iso_extended_string(timeWinResult.begin()) << endl;
	//<<"--" << to_iso_extended_string(timeWinResult.last()) << std::endl;
	return make_pair(IndexOfTimeWin, flag_NewFurn);
};

// 把某工单排入钟罩炉
bool  insertJobToBellFurnWithPreBatch(vector<pair<Job*, int>>& curBatch, ptime readyTimeForOrder, Mach_BellFurnace& curMach)
{

	time_duration& processTime = double2timeDuration(curMach.m_proceTimePerFurn);
	pair<pair<int, int>, time_period> newFurnRes = (make_pair(pair<unsigned, unsigned>(make_pair(-1, -1))
		, time_period(readyTimeForOrder, processTime))); // 记录新开一炉的情况; pair<pair< 第几个timeline, 插入位置>, 时间窗>

	unsigned Index_parallelMach = 0;
	pair<int, bool> resultOfPreInsert(make_pair(-1, false));   // pair<TimeWin的位置, 是否要新开一炉>
	for (auto& timeline : curMach.m_timeLines)
	{
		time_period timeWinResult(readyTimeForOrder, processTime);
		resultOfPreInsert = preInsertJobToMachWithPreBatch(readyTimeForOrder, curMach, timeline, timeWinResult);
		if (resultOfPreInsert.second) // 是新开1炉
		{
			if (newFurnRes.first.first < 0.0)
				newFurnRes = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
			else if (newFurnRes.second.begin() >= timeWinResult.begin())
				newFurnRes = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
		}
		// cout << "  Index_parallelMach=" << Index_parallelMach << endl;
		++Index_parallelMach;
	}

	unsigned Index_final = newFurnRes.first.first;  // 哪个时间线
	time_period timeWinFinal(newFurnRes.second.begin(), processTime);
	int insertPosition = newFurnRes.first.second;  // 插入位置

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
		it->second = 2;  // 标志位设置为"2"，表示已经排产了
	}

	return true;
};

// 把某工单排入钟罩炉的某个平行机,先组好钟罩炉的批次
int  insertJob(Job& curJob, Mach_BellFurnace& curMach, vector<vector<pair<Job*, int>>>& batchOfBell, map<string, Mach*>& machsMapLocal)
{
	string gradeOfJob = curJob.m_alloyGrade;
	int batchIndex = 0;
	int batchIndexFinal = -1;
	for (auto it = batchOfBell.begin(); it != batchOfBell.end(); ++it, ++batchIndex)
	{
		vector<pair<Job*, int>>& aBatch = *it;
		if (gradeOfJob == aBatch.begin()->first->m_alloyGrade)  // 牌号一样的那个批次
		{
			for (auto it2 = aBatch.begin(); it2 != aBatch.end(); ++it2)  // 遍历该批次，
			{
				if (curJob.m_jobCode == it2->first->m_jobCode) {  //如果在该批次中
					it2->second = 1;  // 把该带卷设置为正在钟罩炉前等待
					batchIndexFinal = batchIndex;
					break;
				}
			}
		}
		if (batchIndexFinal >= 0) break;
	}

	cout << " batchIndexFinal=" << batchIndexFinal << endl;  // 最后的插入时间


	vector<pair<Job*, int>>& curBatch = batchOfBell[batchIndexFinal];  // 含有当前job的批次
	ptime readyTimeForOrder = (curJob.m_allocatedTimeWin.end() - 1)->second.last();  // 钟罩炉工序的最早开始时间
	int isAssigned = 0;
	if (curBatch.size() == 1)  // 如果就只有这个job，直接排产
	{
		insertJobToBellFurnWithPreBatch(curBatch, readyTimeForOrder, curMach);
		isAssigned = 1;
	}
	else
	{
		int sumFlag = 0;
		for (auto it2 = curBatch.begin(); it2 != curBatch.end(); ++it2)  // 遍历该批次，
			sumFlag += it2->second;
		if (sumFlag == curBatch.size()) // 该批次中，带卷都在等待，直接排产
		{
			insertJobToBellFurnWithPreBatch(curBatch, readyTimeForOrder, curMach);
			isAssigned = 1;
		}
	}
	cout << " " << endl;
	if (isAssigned)  // 排后续工序
	{
		return batchIndexFinal;
	}
	return -1;
};

// 钟罩炉，是否可以加入钟罩炉的某个timeWin
bool canAddToTimeWin(Job* curJobP, unsigned machIndexOfJob, TimeWin& timeWin, map<string, Job*> jobsMap)
{
	double totalWidthOfBatch = getCurWidth(curJobP, machIndexOfJob);
	if (Mach_BellFurnace::m_RuleForFurnWithWidth.second.second == timeWin.first.size()) // 已经有3个了，不能插入
		return false;
	else if (Mach_BellFurnace::m_RuleForFurnWithWidth.second.first == timeWin.first.size())  // 已经有2个了
	{
		for (auto& jobInfo : timeWin.first)
		{
			Job* jobP = jobsMap[jobInfo.first];

			double curWidth = getCurWidth(jobP, CodeOfBellFurn, jobInfo.second); //job的当前宽度
			totalWidthOfBatch += curWidth;
			if ((totalWidthOfBatch > Mach_BellFurnace::m_RuleForFurnWithWidth.first)  // 不能超出宽度范围
				|| (curJobP->m_jobCode == jobP->m_jobCode))  return false;  // 不能是job自己
		}
	}
	return true;
}

// 预排入钟罩炉工序，把某工单排入钟罩炉的某平行机，返回插入时间窗和插入索引
pair<unsigned, bool> preInsertJobToMach(Job& curJob, Mach_BellFurnace& curMach, unsigned machIndexOfJob, list<TimeWin>& timeline, time_period& timeWinResult, map<string, Job*>& jobsMap)
{
	time_duration& processTime = double2timeDuration(curMach.m_proceTimePerFurn);  //钟罩炉时间为30小时
	//time_duration timeOfSwith = double2timeDuration(curMach.m_timeOfSwith);  // 开启新的一炉的转换时间
	//准备时间和上一炉的加工完成时间，是否要记入转换时间，需要搞明白？？??
	//   对job来讲，准备时间后不需要转换时间；对machine来讲，上一炉结束时间后需要转换时间！
	unsigned IndexOfTimeWin(0);  // machine的时间线插入的索引
	// using TimeWin = pair<vector<pair<Job*, unsigned>>, time_period>;  // pair< vector<pair<Job*, 该job第几次重入>>, job插入的时间窗>

	bool flag_NewFurn(false);  // 显示是否新开一炉
	bool isInserted(false);

	ptime readyTimeForOrder;  // job可以在该机器上开始加工的时间
	//time_period timeWinToInsert(curJob.m_startDateOfOrder, processTime);  //要插入的时间窗
	if (0 == machIndexOfJob)  // 如果是job的第一个machine
		readyTimeForOrder = curJob.m_startDateOfOrder;
	else
		readyTimeForOrder = (curJob.m_allocatedTimeWin.end() - 1)->second.end();

	//cout << " timeline.size()=" << timeline.size()
	//	<< "  readyTimeForOrder="<<to_iso_extended_string(readyTimeForOrder)<<endl;

	if (0 == timeline.size())  // 如果是machine上的第一个job
	{
		timeWinResult = time_period(readyTimeForOrder + double2timeDuration(curMach.m_waitTofFirstFurn), processTime); //??？？!!!!!!!!!
		flag_NewFurn = true;  // 新开一炉
		IndexOfTimeWin = 0;
		isInserted = true;
	}
	else
	{
		// 遍历timeline
		for (auto timeInfo_iter = timeline.begin();
			timeInfo_iter != timeline.end(); ++timeInfo_iter, ++IndexOfTimeWin)
		{
			ptime left_TimeWin = timeInfo_iter->second.begin();
			ptime right_TimeWin = timeInfo_iter->second.last();
			//cout << "    " <<to_iso_extended_string(left_TimeWin)<<"  IndexOfTimeWin="<< IndexOfTimeWin << endl;

			if (left_TimeWin < readyTimeForOrder + processTime)  // 可加工时间和时间窗开始时间之间一定放不下新的一炉
			{
				if ((readyTimeForOrder < left_TimeWin) && canAddToTimeWin(&curJob, machIndexOfJob, *timeInfo_iter, jobsMap))
				{
					timeWinResult = timeInfo_iter->second;
					flag_NewFurn = false; // 不新开一炉
					isInserted = true;  // 插入已有炉中
					break;
				}
				continue;
			}

			if (timeline.cbegin() != timeInfo_iter)  // 不是第一个time window
			{
				auto timeInfo_iter2 = timeInfo_iter;
				--timeInfo_iter2;
				ptime right_Pre = timeInfo_iter2->second.end();
				if ((readyTimeForOrder < right_Pre) && (canAddToTimeWin(&curJob, machIndexOfJob, *timeInfo_iter, jobsMap))
					&& (readyTimeForOrder < left_TimeWin))
				{
					timeWinResult = timeInfo_iter->second;
					flag_NewFurn = false;    // 不新开一炉
					isInserted = true;
					break;
				}

			}
		}  // END OF 遍历timeline

		if (!isInserted)
		{
			IndexOfTimeWin = 0;
			// 遍历timeline
			for (list<TimeWin>::iterator timeInfo_iter = timeline.begin();
				timeInfo_iter != timeline.end(); ++timeInfo_iter, ++IndexOfTimeWin)
			{
				ptime left_TimeWin = timeInfo_iter->second.begin();
				ptime right_TimeWin = timeInfo_iter->second.last();

				if (left_TimeWin < readyTimeForOrder + processTime)  // 可加工时间和时间窗开始时间之间一定放不下新的一炉
					continue;

				if (timeline.cbegin() == timeInfo_iter)  // 第一个time window
				{
					//std::cout << "**************** " << std::endl;
					timeWinResult = time_period(readyTimeForOrder, processTime);
					flag_NewFurn = true;    // 新开一炉
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
						flag_NewFurn = true;    // 新开一炉
						isInserted = true;
						break;
					}
					if (right_Pre <= readyTimeForOrder)
					{
						timeWinResult = time_period(readyTimeForOrder, processTime);
						flag_NewFurn = true;    // 新开一炉
						isInserted = true;
						break;
					}
				}
			}  // END OF 遍历timeline
		}

		if (false == isInserted)
		{
			//cout << "dsdssdsdsdsdsdsd"<< "flag_NewFurn=" << flag_NewFurn << "isInserted=" << isInserted << endl;
			auto iterTmp = timeline.end();
			ptime rightWin = (--iterTmp)->second.end();
			timeWinResult = time_period((readyTimeForOrder < rightWin) ? rightWin : readyTimeForOrder, processTime);
			flag_NewFurn = true;    // 新开一炉
			isInserted = true;
		}
	}
	//std::cout << "curJob=" << curJob.m_jobCode <<" 777777"<< std::endl;
	//std::cout << "      timeWinResult=" << to_iso_extended_string(timeWinResult.begin()) << endl;
	//<<"--" << to_iso_extended_string(timeWinResult.last()) << std::endl;
	return make_pair(IndexOfTimeWin, flag_NewFurn);
};

// 对于气垫炉，检查是否需要规格切换（根据前后工件的牌号是否相同来判断）airFurnaceSet
bool getIsSwitch(Job& curJob, Mach& curMach, Job& otherJob)  // 是否需要切换
{
	if (airFurnaceSet.find(curMach.m_machCode) != airFurnaceSet.end())  // 是气垫炉组的机器
	{
		//cout << otherJob.m_jobCode <<" "<< curJob.m_jobCode
			//<<" otherJob.m_alloyType=" << otherJob.m_alloyType<<"curJob.m_alloyType"<< curJob.m_alloyType << endl;
		if (otherJob.m_alloyGrade == curJob.m_alloyGrade) return false;
		else return true;
	}
	return false;
};

// --------END OF--排产，把某工单排入某机器的空闲时间--------



// --------初始化相关，初始化Job和Machine--------

//  测试--调试用的初始化
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

// 初始化产能——根据数据库读入的结果初始化机组产能信息
void initializeCaps(MYSQL_RES* res, vector<string>& machsCodeVec, map<string, Mach*>& machsMap)
{
	MYSQL_ROW row;  //一个行数据的类型安全(type-safe)的表示，表示一条数据
	MYSQL_FIELD* field = NULL;

	vector<string> fieldsNames_orders;
	//字段的个数
	int fieldcount = mysql_num_fields(res);
	std::cout << "Number of fields: " << fieldcount << std::endl;
	//显示所有字段名
	for (int i = 0; i < fieldcount; ++i)
	{
		field = mysql_fetch_field_direct(res, i);
		std::cout << field->name << "\t";
		fieldsNames_orders.push_back(field->name);
	}
	std::cout << std::endl;
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

// 读入工单信息——根据数据库读入的结果初始化工单信息
void initializeJobs(MYSQL_RES* res, vector<string>& jobsCodeVec, map<string, Job*>& jobsMap)
{
	MYSQL_ROW row;  //一个行数据的类型安全(type-safe)的表示，表示一条数据
	MYSQL_FIELD* field = NULL;

	vector<string> fieldsNames_orders;
	//字段的个数
	int fieldcount = mysql_num_fields(res);
	std::cout << "Number of fields: " << fieldcount << std::endl;
	//显示所有字段名
	for (int i = 0; i < fieldcount; i++)
	{
		field = mysql_fetch_field_direct(res, i);
		std::cout << field->name << "\t";
		fieldsNames_orders.push_back(field->name);
	}
	std::cout << std::endl;
	int i = 0;  // 行数
	while (row = mysql_fetch_row(res))
	{
		// strip_id, deadline, thick, width, note, length, weight, inner_diameter, alloy_grade
		// 重复读取行，并输出获取每行中字段的值，直到row为NULL
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

// 初始化工单——完善工单信息（加工目标和加工机器）
void initializeJobs2(MYSQL_RES* res, vector<string>& jobsCodeVec, map<string, Job*>& jobsMap)
{
	MYSQL_ROW row;  //一个行数据的类型安全(type-safe)的表示，表示一条数据
	MYSQL_FIELD* field = NULL;

	vector<string> fieldsNames_orders;
	//字段的个数
	int fieldcount = mysql_num_fields(res);
	std::cout << "Number of fields: " << fieldcount << std::endl;
	//显示所有字段名
	for (int i = 0; i < fieldcount; ++i)
	{
		field = mysql_fetch_field_direct(res, i);
		std::cout << field->name << "\t";
		fieldsNames_orders.push_back(field->name);
	}
	std::cout << std::endl;
	int i = 0;  // 行数
	while (row = mysql_fetch_row(res))
	{
		// strip_id, process_num, unit_id, thick, width, note, unit_name_orig
		// 重复读取行，并输出获取每行中字段的值，直到row为NULL
		string jobCode = row[0];
		Job* jobP;
		if (jobsMap.find(jobCode) == jobsMap.end()) // 如果jobCode不在jobsMap中，
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
		//cout<<"宽度a="<< mystod(row[4]) <<"   厚度b="<< mystod(row[3]) <<"\n"<<endl;
		jobP->m_proceTargets.push_back(make_pair(make_pair(machCode, num_reentry), ProcessTargets(mystod(row[4]), mystod(row[3]))));
		++i;
	}
	std::cout << "There are " << i << " notes in total." << std::endl;
};

// 初始化机器——根据数据库读入的结果初始化机器信息
void initialMachs(MYSQL_RES* res, vector<string>& machsCodeVec, map<string, Mach*>& machsMap)
{
	MYSQL_ROW row;  //一个行数据的类型安全(type-safe)的表示，表示一条数据
	MYSQL_FIELD* field = NULL;

	vector<string> fieldsNames_orders;
	//字段的个数
	int fieldcount = mysql_num_fields(res);
	std::cout << "Number of fields: " << fieldcount << std::endl;
	//显示所有字段名
	for (int i = 0; i < fieldcount; i++)
	{
		field = mysql_fetch_field_direct(res, i);
		std::cout << field->name << "\t";
		fieldsNames_orders.push_back(field->name);
	}
	std::cout << std::endl;
	int i = 0;  // 行数
	while (row = mysql_fetch_row(res))
	{
		// unit_id, name
		// 重复读取行，并输出获取每行中字段的值，直到row为NULL
		//std::cout << "machdCode" << row[0] << std::endl;
		if (CodeOfBellFurn == row[0])  // 是钟罩炉
		{
			Mach_BellFurnace* machP = new Mach_BellFurnace();
			machP->m_machCode = row[0];
			machsCodeVec.push_back(machP->m_machCode);
			machsMap[machP->m_machCode] = machP;
		}
		else if (airFurnaceSet.find(string(row[0])) != airFurnaceSet.end()) // 是气垫炉
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

// 初始化机器信息
void initialMachs2(MYSQL_RES* res, vector<string>& machsCodeVec, map<string, Mach*>& machsMap)
{
	//Mach *mach_bellFurnace = machsMap["BD-S006"];  // 钟罩炉

	MYSQL_ROW row;  //一个行数据的类型安全(type-safe)的表示，表示一条数据
	MYSQL_FIELD* field = NULL;

	vector<string> fieldsNames_orders;
	//字段的个数
	int fieldcount = mysql_num_fields(res);
	std::cout << "Number of fields: " << fieldcount << std::endl;
	//显示所有字段名
	for (int i = 0; i < fieldcount; i++)
	{
		field = mysql_fetch_field_direct(res, i);
		std::cout << field->name << "\t";
		fieldsNames_orders.push_back(field->name);
	}
	std::cout << std::endl;
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
	std::cout << "There are " << i << " machines in total." << std::endl;
}



// 查看制程
void printProcessLine(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap) {
	
	set<vector<pair<string, unsigned>>> processLineSet;  // 制程集合

	int numoerder = 0;
	map<string, unsigned> machFrequ;  //出现频率
	map<string, unsigned> machCount;  //工序统计出现频率
	//map<string, string> machFrequ;  // 制程，
	for (auto& jobInfo : jobsMap)  // 遍历所有job
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
	cout << "orderpro=" << numoerder << endl; // order个数

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

	// 打印
	for (auto& jobInfo : jobsMap)  // 遍历所有job
	{
		string curjobcode = jobInfo.first;
		Job* curJobP = jobInfo.second;
		curJobP->m_dueDateOfOrder = from_iso_string((curJobP->m_dueDateOfOrderStr) + "T115959");
		if (true)
		{
			std::cout << curjobcode << ": " << curJobP->m_dueDateOfOrderStr
				<< " " << curJobP->m_alloyGrade << std::endl;
			std::cout << curjobcode << ": " << to_iso_string(curJobP->m_dueDateOfOrder) << std::endl;
		}

		if (processLineSet.find(curJobP->m_proceMachs) != processLineSet.end())
		{
			processLineSet.insert(curJobP->m_proceMachs);
		}
		// 检查job的日期格式！！？？!!??
		curJobP->m_startDateOfOrder = getCurTime();
	}

	cout << "ordernum=" << jobsMap.size() << endl; // order个数
}

// 初始化不同的job vector，处理时间，松弛时间，截止时间
void initialJobVecs(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, vector<pair<Job*, ptime>>& jobsWithDueDate
	, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime) {

	for (auto& jobInfo : jobsMap)  // 遍历所有job，processTime，获取processTime
	{
		string curJobCode = jobInfo.first;

		Job* curJobP = jobInfo.second;
		jobsWithDueDate.push_back(make_pair(curJobP, curJobP->m_dueDateOfOrder));
		time_duration sumOfProcessTime;
		unsigned machIndex(0);
		if (curJobP->m_dueDateOfOrder <= getCurTime() && 0)
		{
			cout << "job '" << curJobP->m_jobCode
				<< "' has already missed its deadline!!! It will no long be scheduled." << endl;
			continue;
		}

		for (pair<string, unsigned>& machInfoOfCurJob : curJobP->m_proceMachs)   // 遍历某个job的所有machine
		{
			//std::cout << "machIndex=" << machIndex 
			//std::cout<< curJobP->m_proceTargets.size() << std::endl;
			ProcessTargets const& processTargets = (curJobP->m_proceTargets)[machIndex].second;
			time_duration processTime = getProcessTime(machsMap[machInfoOfCurJob.first], curJobP, machIndex);
			//time_duration processTime = curJobP->m_proceTimes[machIndex].second;
			std::cout << "2processtT=" << to_iso_string(processTime) << std::endl;
			if (curJobP->m_proceTimes.size() > machIndex)
				curJobP->m_proceTimes[machIndex] = make_pair((curJobP->m_proceTargets)[machIndex].first, processTime);
			else
				curJobP->m_proceTimes.insert(curJobP->m_proceTimes.begin() + machIndex,
					make_pair(curJobP->m_proceTargets[machIndex].first, processTime));

			sumOfProcessTime = sumOfProcessTime + processTime;
			++machIndex;
		}

		curJobP->m_totalProceTime = sumOfProcessTime;
		std::cout << "sumOfProcessTime = " << to_iso_string(sumOfProcessTime) << std::endl;
		time_duration slackTime = (curJobP->m_dueDateOfOrder - getCurTime()) - sumOfProcessTime;  //job的剩余时间（当前时点距离交货期的时间）与该job所需的加工时间之差
		jobsWithSlackTime.push_back(make_pair(curJobP, slackTime));
		jobsWithTotalProTime.push_back(make_pair(curJobP, sumOfProcessTime));
	}
}

void initialJobsBatch(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, vector<string>& jobsCodeVec
	, map<string, vector<Job*>>& jobsWithBell, map<string, pair<Job*, int>>& jobsWithBells, vector<BellBatch*>& jobsBatch) 
{
	int i = 0;
	string alloyGradePre = jobsMap[*jobsCodeVec.begin()]->m_alloyGrade;
	for (auto& jobInfo : jobsCodeVec)  // 遍历所有job，获取有钟罩炉的job
	{
		Job* jobP = jobsMap[jobInfo];
		string alloyGrade = jobP->m_alloyGrade;

		cout << "i=" << i << "  job code=" << jobP->m_jobCode << " alloy grade=" << jobP->m_alloyGrade << endl;
		cout << "  due time=" << to_iso_extended_string(jobP->m_dueDateOfOrder) << endl;
		for (pair<string, unsigned>& machInfoOfCurJob : jobP->m_proceMachs)   // 遍历某个job的所有machine
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

	for (auto& jobInfo : jobsCodeVec)  // 遍历所有job，获取有多个钟罩炉的job
	{
		Job* jobP = jobsMap[jobInfo];
		string alloyGrade = jobP->m_alloyGrade;

		cout << "i=" << i << "  job code=" << jobP->m_jobCode << " alloy grade=" << jobP->m_alloyGrade << endl;
		int countBell = 0;
		for (pair<string, unsigned>& machInfoOfCurJob : jobP->m_proceMachs)   // 遍历某个job的所有machine
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

	for (auto& jobInfo : jobsWithBell)  // 打印具有钟罩炉的job
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



	// 初始化jobsBatch
	string alloyGradePrev = "";
	BellBatch* pBellBatch;
	for (auto& jobInfo : jobsWithBell)  // 遍历所有job
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

	for (auto& pBatch : jobsBatch)  // 遍历所有job
	{
		cout << "  pBatch=" << pBatch->m_strAlloyGrade << endl;
		for (auto& jobInfo : pBatch->m_jobsWithWidth) {
			cout << "    joobcode" << jobInfo.first->m_jobCode << " width=" << jobInfo.second << endl;
		}
	}
}

// 打印钟罩炉工序统计个数
void printBellCount(map<string, Job*>& jobsMap) {
	map<string, int> bellCountRecord;
	for (auto& jobInfo : jobsMap)  // 遍历所有job
	{
		string curJobCode = jobInfo.first;
		Job* curJobP = jobInfo.second;
		bellCountRecord.insert(make_pair(curJobCode, 0));
		for (pair<string, unsigned>& machInfoOfCurJob : curJobP->m_proceMachs)   // 遍历某个job的所有machine
		{
			if (CodeOfBellFurn == machInfoOfCurJob.first)
				bellCountRecord[curJobCode] += 1;
		}
	}
	for (auto& recd : bellCountRecord)  // 遍历所有job
	{
		cout << "jobCode=" << recd.first << " num=" << recd.second << endl;
	}
}

// 打印交期、处理时间、松弛时间
void printDueProSlackTime(vector<pair<Job*, ptime>>& jobsWithDueDate
	, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime) 
{
	// 查看每个job的总加工时间和截止日期
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
	// 查看每个机器需要加工的时间，及空闲率

	for (auto& jobInfo : jobsWithSlackTime)  // 遍历所有job jobsWithDueDate
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

// --------END OF--初始化相关，初始化Job和Machine--------



// --------获取目标函数值相关--------

// 获取makespan目标函数值
double getMakespan(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap)  // get object value
{
	ptime timeOfStart;       // 开始排程的时间点
	ptime timeOfCompletion;  // 所有工件加工完的时间点
	double makeSpan(0);      // 加工所有工件所需的时间长度（小时）
	int i_numOfMach(0);

	for (auto& machInfo : machsMap)  // 遍历所有job
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
	//std::cout << "totalDueTime=　" << totalDueTime << std::endl;
	//std::cout << "makeSpan = " << makeSpan << std::endl;
	//std::cout << std::endl;
	return make_pair(totalDueTime, makeSpan);
};

// --------END OF--获取目标函数值相关--------



// --------求解方法相关--------

// NEH方法
void NEH_Method(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, MYSQL* mysql, 
	vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime)
{
	vector<pair<string, Job*>> jobOrder;
	//jobOrder.push_back(make_pair( (jobsWithTotalProTime.end() - 1)->first->m_jobCode, (jobsWithTotalProTime.end() - 1)->first) );
	//jobOrder.push_back(make_pair( (jobsWithTotalProTime.end() - 2)->first->m_jobCode, (jobsWithTotalProTime.end() - 2)->first) );

	jobOrder.push_back(make_pair((jobsWithSlackTime.begin()->first)->m_jobCode, jobsWithSlackTime.begin()->first));
	//jobOrder.push_back(make_pair((jobsWithSlackTime.begin() + 1)->first->m_jobCode, (jobsWithSlackTime.begin() + 1)->first));

	//for (unsigned i = 2; i < 30; ++i)  // 一个一个地放入job
	//unsigned i(-1);
	//for (auto& jobInfo: jobsMap)  // 一个一个地放入job
	for (int i = 1; i < jobsWithSlackTime.size(); ++i)  // 一个一个地放入job
	//for (int i = jobsWithTotalProTime.size() - 3; i >= 0; --i)
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
		for (unsigned j_insertPosition = 0; j_insertPosition <= jobOrder.size(); ++j_insertPosition) // 遍历所有插入位置
		{

			vector<pair<string, Job*>> jobOrderTemp;
			map<string, Job*> jobsMapInit;
			map<string, Mach*> machsMapTemp;
			//if (j_insertPosition >= 80 ) cout << "insertPostion=" << j_insertPosition << endl;

			cout << "  j_insertPosition=" << j_insertPosition << endl;
			initJobsTemp(jobsMapInit, jobOrderTemp, jobOrder);
			initMachsMapTemp(machsMapTemp, machsMap);

			
			//if (j_insertPosition >= 80)  cout << "insertPostion="<< j_insertPosition << endl;
			Job* curjobP_copy = new Job;
			*curjobP_copy = *curJobP; //0205782740-0-0
			jobOrderTemp.insert(jobOrderTemp.begin() + j_insertPosition, make_pair(curjobP_copy->m_jobCode, curjobP_copy));
			jobsMapInit.insert(make_pair(curjobP_copy->m_jobCode, curjobP_copy));

			int isp = 0;
			if (j_insertPosition >= 105) isp = 1;
			pair<double, double> objectVals = scheduleByJobOrder(jobOrderTemp, jobsMapInit, machsMapTemp, mysql, isp);
			//if (j_insertPosition >= 80) cout << "llllllllllll" << endl;
			if (0 == j_insertPosition) bestObjectVals = objectVals;
			else if ((objectVals.first) < (bestObjectVals.first))  // else if (objectVals.first < bestObjectVals.first)
			{
				bestPosition = j_insertPosition;
				bestObjectVals = objectVals;
			}
		}

		cout << "bestObjectVals(makespan)=" << bestObjectVals.second << ";  due time=" << bestObjectVals.first << endl;
		jobOrder.insert(jobOrder.begin() + bestPosition, make_pair(curJobP->m_jobCode, curJobP));
	}

	vector<pair<string, Job*>> jobOrderTemp;
	map<string, Job*> jobsMapInit;
	map<string, Mach*> machsMapTemp;

	initJobsTemp(jobsMapInit, jobOrderTemp, jobOrder);
	initMachsMapTemp(machsMapTemp, machsMap);

	pair<double, double> objectVals = scheduleByJobOrder(jobOrderTemp, jobsMapInit, machsMapTemp, mysql, true);
}

// GA（遗传）方法
void GA_Method(map<string, Mach*>& machsMap, MYSQL* mysql, 
	vector<pair<Job*, ptime>>& jobsWithDueDate, vector<pair<Job*, time_duration>>& jobsWithTotalProTime, vector<pair<Job*, time_duration>>& jobsWithSlackTime)
{
	vector<pair<string, Job*>> jobOrder;

	// for (int i = jobsWithTotalProTime.size() -1; i >= 0; --i) 
	// for (int i = 0; i < jobsWithSlackTime.size(); ++i)
	for (int i = 0; i < jobsWithDueDate.size(); ++i)
		//for (int i = 0; i < jobsWithTotalProTime.size(); ++i)
	{
		//Job* curJobP = jobsWithTotalProTime[i].first;
		Job* curJobP = jobsWithSlackTime[i].first;
		//Job* curJobP = jobsWithDueDate[i].first;
		//if(curJobP->)
		jobOrder.push_back(make_pair(curJobP->m_jobCode, curJobP));
	}

	// 用来生成初始排序结果
	vector<pair<string, Job*>> jobOrderInit(jobOrder.size());
	map<string, Job*> jobsMapInit;
	map<string, Mach*> machsMapInit;
	initJobsTemp(jobsMapInit, jobOrderInit, jobOrder);
	initMachsMapTemp(machsMapInit, machsMap);

	// 用来排气垫炉之前的工序
	vector<pair<string, Job*>> jobOrderBef(jobOrder.size());
	map<string, Job*> jobsMapBef;
	map<string, Mach*> machsMapBef;
	initJobsTemp(jobsMapBef, jobOrderBef, jobOrder);
	initMachsMapTemp(machsMapBef, machsMap);

	// 获取初始排序结果
	pair<double, double> objectVals = scheduleByJobOrderForGA(jobOrderInit, jobsMapInit, machsMapInit, mysql, true);  //delete !!!
	// 只排退火炉之前的工序

	//scheduleByJobOrderForGA_BefAir(jobOrderBef, jobsMapBef, machsMapBef, mysql, true);  //delete !!!



	// 初始化GA编码信息，放入codeInfoOfGA； 获取GA编码的长度，存入totalLenOfGACode中
	vector<pair<Job*, pair<int, int>>> codeInfoOfGA(jobOrder.size());  // 存放GA编码信息，vector<pair<某Job*, pair<GA编码的制程开始索引, GA编码的制程结束索引>>>
	for (int i = 0; i < jobOrder.size(); i++)
		codeInfoOfGA[i].first = jobOrderInit[i].second;
	//getCodeInfoOfGA_Air(jobOrderInit, codeInfoOfGA);                   // 获取GA编码信息
	getCodeInfoOfGA_All(jobOrderInit, codeInfoOfGA);                   // 获取GA编码信息
	int totalLenOfGACode = 0;                                          // 获取GA编码的长度，存入totalLenOfGACode中
	for (int i = 0; i < codeInfoOfGA.size(); ++i) {
		if (codeInfoOfGA[i].second.first >= 0) {
			totalLenOfGACode += (codeInfoOfGA[i].second.second - codeInfoOfGA[i].second.first + 1);
		}
	}

	cout << "totalLenOfGACode=" << totalLenOfGACode << endl;
	cout << "codeInfoOfGA=" << codeInfoOfGA.size() << endl;

	// 从初始初始排序结果得到染色体，存入jobChromPre中
	vector<pair<pair<Job*, int>, ptime>> jobChromPre(totalLenOfGACode);
	Chromosome* chromPInit = new Chromosome(totalLenOfGACode, jobOrder.size());  // 染色体
	Chromosome* chromPInit2 = new Chromosome(totalLenOfGACode, jobOrder.size());  // 染色体
	getJobChromoFromInitSolut(codeInfoOfGA, jobOrderInit, jobChromPre, chromPInit, chromPInit2);


	/*
	cout << " jobChromPre.size()=" << jobChromPre.size() << endl;
	for (int i = 0; i < jobChromPre.size(); ++i) {
		cout << "  " << i << endl;
		cout << jobChromPre[i].first.first->m_jobCode << "    " << jobChromPre[i].first.second << "    "
			<< to_iso_extended_string(jobChromPre[i].second) << endl;
	}
	*/


	// 初始化种群
	GeneticAlgorithm* populationP = new GeneticAlgorithm(jobOrder.size(), 150, 200, totalLenOfGACode);
	populationP->initializePopulation(chromPInit, chromPInit2);                             // 初始化种群
	populationP->initializeInfoOfBef(&jobOrderBef, &jobsMapBef, &machsMapBef);     // 初始化种群

	cout << "yyyyyyyyyyy" << endl;

	// 获取初始解的目标值
	pair<double, double> objectValues2 = getObjectValuesOfChromo(chromPInit2, populationP);
	pair<double, double> objectValues = getObjectValuesOfChromo(chromPInit, populationP);

	cout << "bestObjectVals(makespan)=" << objectVals.second << ";  due time=" << objectVals.first << endl;
	cout << "bestObjectValues(makespan)=" << objectValues.second << ";  due time=" << objectValues.first << endl;
	cout << "bestObjectValues2(makespan)=" << objectValues2.second << ";  due time=" << objectValues2.first << endl;

	//for (int j = 0; j < chromPInit->tardinessOfjobs.size(); ++j) {
	//	cout << j << ":" << chromPInit->tardinessOfjobs[j] << "   ";
	//}
	//cout << endl;
	//for (int j = 0; j < chromPInit2->tardinessOfjobs.size(); ++j) {
	//	cout << j << ":" << chromPInit2->tardinessOfjobs[j] << "   ";
	//}

	//cout << endl; cout << endl;
	//for (int j = 0; j < chromPInit->code.size(); ++j) {
	//	cout << chromPInit->code[j] << " ";
	//}
	//cout << endl;
	//for (int j = 0; j < chromPInit2->code.size(); ++j) {
	//	cout << chromPInit2->code[j] << " ";
	//}


	char c; cin >> c;

	cout << "why" << endl;
	for (int i = 0; i < chromPInit->tardinessOfjobs.size(); ++i) {
		cout << i << ":" << chromPInit->tardinessOfjobs[i] << endl;
	}
	cout << endl;

	cout << "why" << endl;
	for (int i = 0; i < chromPInit2->tardinessOfjobs.size(); ++i) {
		cout << i << ":" << chromPInit2->tardinessOfjobs[i] << endl;
	}
	cout << endl;

	// 初始化种群中的每个染色体的目标函数
	populationP->initObjValsOfGAPop();


	// 运行
	populationP->runGA();

	//crossoverOfPop(0.23, numOfPop, totalLenOfGACode, chromPInit, popPool);
	//mutationOfPop(0.23, populationP->numOfPop, populationP->totalLenOfGACode, chromPInit, popPool);


	for (int j = 0; j < populationP->m_popPool[10]->tardinessOfjobs.size(); ++j) {
		cout << j << ":" << populationP->m_popPool[10]->tardinessOfjobs[j] << "   ";
	}
	cout << endl; cout << endl;
	for (int j = 0; j < populationP->m_popPool[10]->code.size(); ++j) {
		cout << populationP->m_popPool[10]->code[j] << " ";
	}

	cout << endl; cout << endl;
	cout << " initialObjVal=" << populationP->m_popPool[10]->objectValues.second << endl;

	populationP->localSearchForCurChrome(populationP->m_popPool[10], 500);

	cout << endl; cout << endl;
	for (int j = 0; j < populationP->m_popPool[10]->code.size(); ++j) {
		cout << populationP->m_popPool[10]->code[j] << " ";
	}
	populationP->getNeighborByReverse(populationP->m_popPool[10], totalLenOfGACode);


	cout << endl;
	cout << " initialObjVal=" << populationP->m_popPool[10]->objectValues.second << endl;
	for (int j = 0; j < populationP->m_popPool[10]->tardinessOfjobs.size(); ++j) {
		cout << j << ":" << populationP->m_popPool[10]->tardinessOfjobs[j] << "   ";
	}
	cout << endl; cout << endl;
	for (int j = 0; j < populationP->m_popPool[10]->code.size(); ++j) {
		cout << populationP->m_popPool[10]->code[j] << " ";
	}


	for (auto& jobInfo : jobsMapInit) delete jobInfo.second;
	for (auto& machInfo : machsMapInit) delete machInfo.second;

	for (auto& jobInfo : jobsMapBef) delete jobInfo.second;
	for (auto& machInfo : machsMapBef) delete machInfo.second;

}


// --------END OF--求解方法相关--------




// --------GA获取初始解相关--------

// 按照已排的开始加工时间，对job进行排序
bool compareJobPPtime(pair<pair<Job*, int>, ptime> left, pair<pair<Job*, int>, ptime> right) {
	return right.second > left.second ;
};

// 排产，获取初始排产结果
pair<double, double> scheduleByJobOrderForGA(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, MYSQL* mysql, bool isPrint) {
	//if (jobOrder.size() >= 79)std::cout << "curJobsize: " << jobOrder.size() << std::endl;

	Job* curJobP;
	for (int i_job = 0; i_job < jobOrder.size(); ++i_job)
	{
		curJobP = jobOrder[i_job].second;

		cout << "curJobPcode = " << curJobP << endl;

		unsigned machIndex(0);  // job的 machine
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // 遍历某个job的所有machine
		{
			bool isSuccess;
			if (CodeOfBellFurn == machCodeOfCurJob.first)  // 是钟罩炉
			{
				Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // 是气垫炉组中的机器
			{
				Mach_AirFurnace* curMachP = static_cast<Mach_AirFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else {  //其他
				Mach* curMachP = machsMapTemp[machCodeOfCurJob.first];
				isSuccess = insertJob(*curJobP, *curMachP, machIndex);
			}
			if (isSuccess) curJobP->m_curMachIndex = machIndex + 1;

			//if ("0118741020-0-0"==curJobP->m_jobCode)
			//std::cout << "curJobP.code"<< curJobP->m_jobCode <<"curMach: " << machCodeOfCurJob.first
			//	<< "isSuccess: " << isSuccess << std::endl;
			//if (jobOrder.size() >= 80 && ("0118741020-0-0" == curJobP->m_jobCode)) std::cout << "curMach: " << machCodeOfCurJob.first << "qyqyqyqyqyqyqy " << machIndex << endl;
			++machIndex;
		}
	}

	// 打印最终排程结果
	if (isPrint) printFinalRes(jobsMapTemp, machsMapTemp);
	// 输出到CSV文件
	//if (isPrint) writeToCSV(jobsMapTemp, machsMapTemp);
	//if (isPrint) writeToCSV(jobsMapTemp, machsMapTemp, mysql);


	// 获取目标值
	pair<double, double> objectVals = getObjVals(jobsMapTemp, machsMapTemp);
	//std::cout << "总延期时间(小时): " << objectVals.first << "; 加工所有工件所需的时间长度(小时): " << objectVals.second << std::endl;

	//for (auto& jobInfo : jobsMapTemp) delete jobInfo.second;
	//for (auto& machInfo : machsMapTemp) delete machInfo.second;

	return objectVals;
};

// 排入气垫炉之前的工序，气垫炉中间的工序不排；最后一个工序是纵横剪的，则最后一个工序不排
pair<double, double> scheduleByJobOrderForGA_BefAir(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, MYSQL* mysql, bool isPrint) {
	//if (jobOrder.size() >= 79)std::cout << "curJobsize: " << jobOrder.size() << std::endl;

	Job* curJobP;
	for (int i_job = 0; i_job < jobOrder.size(); ++i_job)
	{
		curJobP = jobOrder[i_job].second;
		if (i_job == 0) cout << curJobP << endl;
		unsigned machIndex(0);  // job的 machine
		int leftPos = -1;
		int rightPos = -1;
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // 遍历某个job的所有machine
		{
			bool isSuccess;
			if (CodeOfBellFurn == machCodeOfCurJob.first)  // 是钟罩炉
			{
				Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // 是气垫炉组中的机器
				break;
			else {  //其他
				Mach* curMachP = machsMapTemp[machCodeOfCurJob.first];
				isSuccess = insertJob(*curJobP, *curMachP, machIndex);
			}
			if (isSuccess) curJobP->m_curMachIndex = machIndex + 1;
			++machIndex;
		}
	}

	// 打印最终排程结果
	if (isPrint) printFinalRes(jobsMapTemp, machsMapTemp);

	// 输出到CSV文件
	//if (isPrint) writeToCSV(jobsMapTemp, machsMapTemp);
	//if (isPrint) writeToCSV(jobsMapTemp, machsMapTemp, mysql);


	// 获取目标值
	pair<double, double> objectVals = getObjVals(jobsMapTemp, machsMapTemp);
	//std::cout << "总延期时间(小时): " << objectVals.first << "; 加工所有工件所需的时间长度(小时): " << objectVals.second << std::endl;

	//for (auto& jobInfo : jobsMapTemp) delete jobInfo.second;
	//for (auto& machInfo : machsMapTemp) delete machInfo.second;

	return make_pair(DBL_MAX, DBL_MAX);
	// return objectVals;
};

// 获取GA的编码信息
void getCodeInfoOfGA_Air(vector<pair<string, Job*>>& jobOrder, vector<pair<Job*, pair<int, int>>>& codeInfoOfGA)
{
	for (int i_job = 0; i_job < jobOrder.size(); ++i_job)
	{
		Job* curJobP = jobOrder[i_job].second;
		if (i_job == 0) cout << curJobP << endl;
		unsigned machIndex(0);  // job的 machine
		int leftPos = -1;
		int rightPos = -1;
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // 遍历某个job的所有machine
		{
			if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // 是气垫炉组中的机器
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

void getCodeInfoOfGA_All(vector<pair<string, Job*>>& jobOrder, vector<pair<Job*, pair<int, int>>>& codeInfoOfGA)
{
	for (int i_job = 0; i_job < jobOrder.size(); ++i_job)
	{
		Job* curJobP = jobOrder[i_job].second;
		if (i_job == 0) cout << curJobP << endl;
		unsigned machIndex(0);  // job的 machine
		int leftPos = -1;
		int rightPos = -1;

		if (curJobP->m_proceMachs.size() > 0) {
			leftPos = 0;
			rightPos = curJobP->m_proceMachs.size()-1;
		}

		codeInfoOfGA[i_job].second = make_pair(leftPos, rightPos);  // numOfEncode==0
	}
};

// --------END OF——GA获取初始解相关--------



// --------结果输出相关，输出到控制台和CSV文件--------

// 打印最终排程结果
void printFinalRes(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap)
{
	// 打印排程结果
	/*
	std::cout << std::endl;
	for (auto& machInfo : machsMap)  // 遍历所有machine
	{
		Mach& curMach = *(machInfo.second);
		std::cout << "MachCode---------" << curMach.m_machCode << std::endl;
		for (auto& timeInfo : curMach.m_allocatedTimeWin)  // 遍历所有time_period
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
	for (auto& machInfo : machsMap)  // 遍历所有machine
	{
		if (CodeOfBellFurn == machInfo.first)
		{
			Mach_BellFurnace* curMach = static_cast<Mach_BellFurnace*>(machInfo.second);
			std::cout << "MachCode---------" << curMach->m_machCode << std::endl;
			unsigned i_timeline(0);
			for (auto& timeline : curMach->m_timeLines)
			{
				std::cout << "   Number of time line: " << i_timeline << std::endl;
				for (auto& timeWinInfo : timeline)  // 遍历所有time_period
				{
					// using TimeWin = pair<map<Job*, unsigned>, time_period>;  // pair< map<Job*, 该job第几次重入>, job插入的时间窗>

					time_period timeWin = timeWinInfo.second;
					std::cout << "      " << to_iso_extended_string(timeWin.begin())
						<< "--" << to_iso_extended_string(timeWin.last()) << std::endl;
					unsigned i_jobs(0);
					for (auto& jobs : timeWinInfo.first)  // 遍历所有job
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
	for (auto& machInfo : machsMap)  // 遍历所有machine
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

	for (auto& machInfo : machsMap)  // 遍历所有machine
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
	for (auto& jobInfo : jobsMap)  // 遍历所有job
	{
		Job* curJobP = jobInfo.second;
		std::cout << "JobCode---------" << curJobP->m_jobCode << std::endl;
		for (auto& timeInfo : curJobP->m_allocatedTimeWin)  // 遍历所有time_period
		{
			string machCode = timeInfo.first.first;
			time_period timeWin = timeInfo.second;
			std::cout << "MachCode  " << machCode << ": " << to_iso_extended_string(timeWin.begin())
				<< "--" << to_iso_extended_string(timeWin.last()) << std::endl;
		}
		std::cout << std::endl;
	};
}

// 打印最终排程结果
void printFinalRes2(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap)
{
	// 打印排程结果
	/*
	std::cout << std::endl;
	for (auto& machInfo : machsMap)  // 遍历所有machine
	{
		Mach& curMach = *(machInfo.second);
		std::cout << "MachCode---------" << curMach.m_machCode << std::endl;
		for (auto& timeInfo : curMach.m_allocatedTimeWin)  // 遍历所有time_period
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
	for (auto& machInfo : machsMap)  // 遍历所有machine
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
	for (auto& jobInfo : jobsMap)  // 遍历所有job
	{
		Job* curJobP = jobInfo.second;
		std::cout << "JobCode---------" << curJobP->m_jobCode << std::endl;
		for (auto& timeInfo : curJobP->m_allocatedTimeWinPs)  // 遍历所有time_period
		{
			string machCode = timeInfo.first.first;
			std::cout << "MachCode  " << machCode << ": " << to_iso_extended_string(timeInfo.second->begin())
				<< "--" << to_iso_extended_string(timeInfo.second->last()) << std::endl;
		}
		std::cout << std::endl;
	};
}

// 打印最终排程结果
void writeToCSV(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap)
{
	ofstream opt;
	opt.open("ResultsOfMach_test.csv", ios::out | ios::trunc);
	cout << "Writing to csv..." << endl;
	for (auto& machInfo : machsMap)  // 遍历所有machine
	{
		if (CodeOfBellFurn == machInfo.first) {
			Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machInfo.second);
			opt << curMachP->m_machCode << endl;
			unsigned i_timeline(0);
			for (auto& timeline : curMachP->m_timeLines)
			{
				opt << "ParallelMachine=" << i_timeline << std::endl;
				for (auto& timeWinInfo : timeline)  // 遍历所有time_period
				{
					// using TimeWin = pair<map<Job*, unsigned>, time_period>;  // pair< map<Job*, 该job第几次重入>, job插入的时间窗>

					time_period timeWin = timeWinInfo.second;
					double beginT = timeDuration2Double(timeWin.begin() - getCurTime());
					double durationT = timeDuration2Double(timeWin.end() - timeWin.begin());

					opt << "," << beginT << ","
						<< durationT << std::endl;
					unsigned i_job(1);

					for (auto& jobs : timeWinInfo.first)  // 遍历所有job
					{
						Job* jobP = jobsMap[jobs.first];
						unsigned reEntry = jobs.second;

						//获取当前宽度
						unsigned machIndexOfJob(0);
						for (auto& proInfo : jobP->m_proceTargets)
						{
							if (make_pair(curMachP->m_machCode, reEntry) == proInfo.first)
								break;
							++machIndexOfJob;
						}

						double curWidth = getCurWidth(jobP, machIndexOfJob);
						// END OF 获取当前宽度

						opt << "," << "," << "," << "reentry=" << reEntry
							<< "," << jobP->m_jobCode
							<< "," << i_job
							<< "," << "当前宽度=" << curWidth << std::endl;
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
			for (auto& timeWinInfo : curMachP->m_allocatedTimeWin)  // 遍历所有time_period
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
	for (auto& jobInfo : jobsMap)  // 遍历所有job
	{
		Job* curJobP = jobInfo.second;
		opt << curJobP->m_jobCode << "," << "," << to_iso_extended_string(curJobP->m_dueDateOfOrder) << endl;
		unsigned i_mach(0);
		for (auto& timeInfo : curJobP->m_allocatedTimeWin)  // 遍历所有time_period
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

// 打印最终排程结果
void writeToCSV(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap, MYSQL* mysql)
{
	int coun = 0;
	for (auto& machInfo : machsMap)  // 遍历所有machine
	{
		cout << "machCodde=" << machInfo.first << endl;
		string jobCodeSeqs = "";

		if (CodeOfBellFurn == machInfo.first) {
			Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machInfo.second);
			unsigned i_timeline(0);
			for (auto& timeline : curMachP->m_timeLines)
			{
				for (auto& timeWinInfo : timeline)  // 遍历所有time_period
				{
					// using TimeWin = pair<map<Job*, unsigned>, time_period>;  // pair< map<Job*, 该job第几次重入>, job插入的时间窗>

					time_period timeWin = timeWinInfo.second;
					double beginT = timeDuration2Double(timeWin.begin() - getCurTime());
					double durationT = timeDuration2Double(timeWin.end() - timeWin.begin());
					unsigned i_job(1);

					for (auto& jobs : timeWinInfo.first)  // 遍历所有job
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
			for (auto& timeWinInfo : curMachP->m_allocatedTimeWin)  // 遍历所有time_period
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
	for (auto& jobInfo : jobsMap)  // 遍历所有job
	{
		Job* curJobP = jobInfo.second;
		unsigned i_mach(0);
		for (auto& timeInfo : curJobP->m_allocatedTimeWin)  // 遍历所有time_period
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

// --------END OF--结果输出相关，输出到控制台和CSV文件--------



// --------排产主函数相关，一个一个排入每个工序--------

pair<double, double> scheduleByJobOrder(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp,  MYSQL* mysql, bool isPrint) 
{
	Job* curJobP;
	for (auto& jobInfo : jobOrder)
	{
		curJobP = jobInfo.second;
		unsigned machIndex(0);  // job的 machine
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // 遍历某个job的所有machine
		{
			bool isSuccess;
			if (CodeOfBellFurn == machCodeOfCurJob.first)  // 是钟罩炉
			{
				Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // 是气垫炉组中的机器
			{
				Mach_AirFurnace* curMachP = static_cast<Mach_AirFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else  //其他
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

	// 打印最终排程结果
	if (isPrint) printFinalRes(jobsMapTemp, machsMapTemp);
	// 输出到CSV文件
	if (isPrint) writeToCSV(jobsMapTemp, machsMapTemp);

	//if (isPrint) writeToCSV(jobsMapTemp, machsMapTemp, mysql);
	//if (jobOrder.size() >= 80) std::cout << "mmmmmmmmmmmmmm" << endl;

	// 获取目标值
	pair<double, double> objectVals = getObjVals(jobsMapTemp, machsMapTemp);
	//std::cout << "总延期时间(小时): " << objectVals.first << "; 加工所有工件所需的时间长度(小时): " << objectVals.second << std::endl;

	//if (jobOrder.size() >= 80)  std::cout << "aaaaaaaaaaaaaaaaaaaaa" << endl;

	for (auto& jobInfo : jobsMapTemp) delete jobInfo.second;
	for (auto& machInfo : machsMapTemp) delete machInfo.second;

	return objectVals;
};

 // 先排钟罩炉的第一炉
pair<double, double> scheduleByJobOrder2(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, vector<pair<string, Job*>> jobWithBell, int lastJobWithBell, bool isPrint) {
	//if (jobOrder.size() >= 79)std::cout << "curJobsize: " << jobOrder.size() << std::endl;
	int i_job(0);
	vector<pair<Job*, ptime>> jobsTimeWithBellFurn;  //有钟罩炉的job集合vector<pair<Job*, ptime最早开始时间>>
	for (auto& jobInfo : jobOrder)
	{
		if (i_job > lastJobWithBell) break;
		Job* curJobP = jobInfo.second;
		cout << "curJob: " << curJobP->m_jobCode
			<< "  dd=" << curJobP->m_proceMachs.size() << std::endl;
		unsigned machIndex(0);  // job的 machine
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // 遍历某个job的所有machine
		{
			cout << "machIndex=" << machIndex << endl;
			// 对于钟罩炉，不再往下排
			if (CodeOfBellFurn == machCodeOfCurJob.first)
			{
				if (machIndex > 0)  // 记录前一个工序结束的时间
					jobsTimeWithBellFurn.push_back(make_pair(curJobP, curJobP->m_allocatedTimeWin[machIndex - 1].second.last()));
				else
					jobsTimeWithBellFurn.push_back(make_pair(curJobP, curJobP->m_startDateOfOrder));
				break;
			}

			bool isSuccess;
			if (CodeOfBellFurn == machCodeOfCurJob.first)  // 是钟罩炉
			{
				Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex);  //调用函数错了！！！
			}
			else if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // 是气垫炉组中的机器
			{
				Mach_AirFurnace* curMachP = static_cast<Mach_AirFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else  //其他
			{
				Mach* curMachP = machsMapTemp[machCodeOfCurJob.first];
				isSuccess = insertJob(*curJobP, *curMachP, machIndex);
			}
			++machIndex;
		}
		i_job++;
	}

	sort(jobsTimeWithBellFurn.begin(), jobsTimeWithBellFurn.end(), myCmpBy_ptime);  // 排序
	unsigned i_parallelMach(0); // 钟罩平行机  Num_Para_Bell
	Mach_BellFurnace* curMachPBell = static_cast<Mach_BellFurnace*>(machsMapTemp[CodeOfBellFurn]);

	vector<double> curWidths;
	for (auto& jobInfo : jobsTimeWithBellFurn)  //获取宽度
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
	vector<pair<unsigned, ptime>> batchPositions; // 获取组批位置
	bool bigger(false);
	unsigned num(0);  // 每炉中放入了多少
	for (int i = 0; i < curWidths.size(); ++i)
	{
		++num;
		cout << "num=" << num << " i=" << i << endl;
		cout << "curWidths[i]=" << curWidths[i] << endl;

		if (curWidths[i] > curMachPBell->m_RuleForFurnWithWidth.first)
			bigger = true;

		if (((curMachPBell->m_RuleForFurnWithWidth.second.first == num) && bigger)  // 2个带卷，且有大于650的
			|| (curMachPBell->m_RuleForFurnWithWidth.second.second == num))  // 3个带卷
		{
			cout << "num = " << num << "  bigger=" << bigger << "  i=" << i << endl;
			batchPositions.push_back(make_pair(i, jobsTimeWithBellFurn[i].second));
			num = 0;
			bigger = false;
		}
	}
	cout << "rrrrrrrrrrr batchPositions=" << batchPositions.size() << endl;
	// 排每个平行机的第一炉
	int index_position(0);
	int preIndex_Job(0);
	int Index_timeline(0);  //某炉的时间线
	for (int i = 0; i < jobsTimeWithBellFurn.size(); i++)
	{
		if (curMachPBell->m_numOfParallel == Index_timeline) break;  //超过5个平行机之后，跳出；即只排这5个平行机的第一炉
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
		unsigned machIndex(0);  // job的 machine
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // 遍历某个job的所有machine
		{
			cout << "machIndex=" << machIndex << endl;
			if (machIndex < curJobP->m_allocatedTimeWin.size())  // 排过的直接跳过
			{
				++machIndex;
				continue;
			}

			bool isSuccess;
			if (CodeOfBellFurn == machCodeOfCurJob.first)  // 是钟罩炉
			{
				Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex);  // 调用函数错了！！！
			}
			else if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // 是气垫炉组中的机器
			{
				Mach_AirFurnace* curMachP = static_cast<Mach_AirFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else  //其他
			{
				Mach* curMachP = machsMapTemp[machCodeOfCurJob.first];
				isSuccess = insertJob(*curJobP, *curMachP, machIndex);
			}
			++machIndex;
		}
		i_job++;
	}



	//if (jobOrder.size() >= 80) std::cout << "uiuiuiui" << endl;
	// 打印最终排程结果
	if (isPrint) printFinalRes(jobsMapTemp, machsMapTemp);
	// 输出到CSV文件
	if (isPrint) writeToCSV(jobsMapTemp, machsMapTemp);

	//if (jobOrder.size() >= 80) std::cout << "mmmmmmmmmmmmmm" << endl;

	// 获取目标值
	pair<double, double> objectVals = getObjVals(jobsMapTemp, machsMapTemp);
	//std::cout << "总延期时间(小时): " << objectVals.first << "; 加工所有工件所需的时间长度(小时): " << objectVals.second << std::endl;

	//if (jobOrder.size() >= 80)  std::cout << "aaaaaaaaaaaaaaaaaaaaa" << endl;

	for (auto& jobInfo : jobsMapTemp) delete jobInfo.second;
	for (auto& machInfo : machsMapTemp) delete machInfo.second;

	return objectVals;
};

// 每个工序遍历位置，进行插入
double scheduleByJobOrder3(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapLocal, map<string, Mach*>& machsMapLocal, MYSQL* mysql, bool isPrint)
{
	set<string> machSet;
	for (auto& jobInfo : jobOrder) // 遍历所有job
	{
		Job* curJobP = jobInfo.second;
		cout << "\n\n" << endl;
		cout << "curJob: " << curJobP->m_jobCode
			<< "  dd_m_proceMachs.size()=" << curJobP->m_proceMachs.size() << std::endl;
		for (auto mach : machSet)
		{
			cout << "mach=" << mach << endl;
		}

		unsigned machIndex(0);  // job的 machine
		ptime readyTimeForJob;  // job可以在某机器上可以开始加工的时间，上一个机器的结束时间或
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // 遍历某个job的所有machine
		{

			Mach* curMachP = machsMapLocal[machCodeOfCurJob.first];
			cout << "\n" << endl;
			cout << "                  curMach_=" << curMachP->m_machCode << endl;

			machSet.insert(curMachP->m_machCode);

			time_duration& processTime = curJobP->m_proceTimes[machIndex].second;
			time_period timeWinToInsertFinal(curJobP->m_startDateOfOrder, processTime);  //要插入的时间窗
			if (0 == machIndex)  // 如果是job的第一个machine
				readyTimeForJob = curJobP->m_startDateOfOrder;
			//else 
				//readyTimeForJob = (curJobP->m_allocatedTimeWinPs.end() - 1)->second->last();

			double insertPositionFinal;  // 最终的插入位置
			bool IsJustPutInFinal;  // 记录是直接放入还是插入并依次往后挪
			if (0 == curMachP->m_allocatedTimeWin.size())  // 如果是machine上的第一个job，则直接放入
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
			else // 如果不是machine上的第一个job，则遍历时间，依次插入，并比较makespan
			{
				bool isJustPutIn;  // 记录是直接放入还是插入并依次往后挪
				double finalMakespan = -100;
				bool isRenewInsertInfo;  // 记录下一轮是否要更新插入信息

				map<string, Mach*> machsMapTemp;
				initMachsMapTemp(machsMapTemp, machsMapLocal);  // 初始化machsMapTemp

				unsigned IndexOfTimeWin(0);  // 往该机器的时间窗里插入，插入的时间窗的索引

				int reentry = curJobP->m_proceMachs[machIndex].second;
				cout << "reentry=" << reentry << endl;
				if (reentry > 1)  // 如果是job重入到该机器
				{
					int reentryCount = 0;
					cout << "curMachP->m_allocatedTimeWin.size()=" << curMachP->m_allocatedTimeWin.size() << endl;
					// 遍历curMach.m_allocatedTimeWin每一个时间窗，找到先前插入的reentry=1的情况
					for (auto timeInfo_iter = curMachP->m_allocatedTimeWin.begin(); timeInfo_iter != curMachP->m_allocatedTimeWin.end();
						++timeInfo_iter, ++IndexOfTimeWin)
					{
						if (curJobP->m_jobCode == timeInfo_iter->first.first) ++reentryCount;
						if (reentryCount == reentry - 1) // 定位到job上一次在该机器上加工的时间窗的位置
						{
							cout << "IndexOfTimewwWin=" << IndexOfTimeWin << endl;
							IndexOfTimeWin += 1;  // 再往后挪一下，以便往前面插入
							break;
						}
					}
					cout << "IndexOfTimeWin=" << IndexOfTimeWin << endl;
				}
				unsigned IndexOfStart = IndexOfTimeWin;  // 往该机器的时间窗里插入，记录开始插入的时间窗的位置

				// 遍历curMach.m_allocatedTimeWin每一个时间窗，并插入
				for (; IndexOfTimeWin <= curMachP->m_allocatedTimeWin.size(); ++IndexOfTimeWin)
				{

					if (IndexOfStart != IndexOfTimeWin)  reInitMachsMapTemp(machsMapTemp, machsMapLocal);
					Mach* curMachPTemp = machsMapTemp[curMachP->m_machCode];
					time_period timeWinToInsert(curJobP->m_startDateOfOrder, processTime);  //要插入的时间窗

					if (curMachPTemp->m_allocatedTimeWin.size() == IndexOfTimeWin) // 放在所有时间窗之后
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
							isRenewInsertInfo = true;  // 下一轮一定更新插入信息
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

						if (0 == IndexOfTimeWin)  // 放在第一个时间窗之前
						{
							timeWinToInsert = time_period(readyTimeForJob, processTime);

							curMachPTemp->m_allocatedTimeWin.insert(timeWinIter,
								make_pair(make_pair(curJobP->m_jobCode, curJobP->m_proceMachs[machIndex].second), timeWinToInsert));
							if (readyTimeForJob + processTime <= left_TimeWin) // 直接放入即可
								isJustPutIn = true;
							else  // 否则，还是插入并挪后面的时间窗
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
						else  // 插入到时间窗之间
						{
							ptime right_TimeWinPro = (--timeWinIter)->second.last();
							readyTimeForJob = readyTimeForJob > right_TimeWinPro ? readyTimeForJob : right_TimeWinPro;

							timeWinToInsert = time_period(readyTimeForJob, processTime);
							curMachPTemp->m_allocatedTimeWin.insert(timeWinIter,
								make_pair(make_pair(curJobP->m_jobCode, curJobP->m_proceMachs[machIndex].second), timeWinToInsert));
							if (readyTimeForJob + processTime <= left_TimeWin)  // 直接放入即可
								isJustPutIn = true;
							else   // 否则，还是插入并挪后面的时间窗
							{
								isJustPutIn = false;
								moveTheFollowedTimeWin(jobsMapLocal, machsMapTemp, curMachPTemp, IndexOfTimeWin);
							}
						}
					}

					double curMachspan = getMakespan(jobsMapLocal, machsMapTemp);  // 获取makespan
					//double curMachspan = getMakespan();
					cout << "IndexOfStart=" << IndexOfStart << "  IndexOfTimeWin=" << IndexOfTimeWin << endl;

					if ((IndexOfStart == IndexOfTimeWin) || (curMachspan < finalMakespan) || isRenewInsertInfo)  // 更新插入信息
					{
						finalMakespan = curMachspan;
						insertPositionFinal = IndexOfTimeWin;
						IsJustPutInFinal = isJustPutIn;
						timeWinToInsertFinal = timeWinToInsert;
						//cout << "finalMakespan=" << finalMakespan << "  curMachspan=" << curMachspan << endl;
					}

					isRenewInsertInfo = false;  // 下一轮不一定更新插入信息

					cout << "  curMachspan=" << curMachspan << "  finalMakespan=" << finalMakespan << endl;
					cout << "   curIndex=" << IndexOfTimeWin << endl;
					cout << endl;
				}  // END OF 遍历curMach.m_allocatedTimeWin


				for (auto& machInfo : machsMapTemp) delete machInfo.second;
			}

			cout << "insertPositionFinal=" << insertPositionFinal << endl;

			time_period* timeWinPToInsert = new time_period(timeWinToInsertFinal);
			// 进行实际插入
			if (curMachP->m_allocatedTimeWin.size() == insertPositionFinal)  // 放到该机器的所有时间窗的后面
			{

				curMachP->m_allocatedTimeWin.push_back(make_pair(
					make_pair(curJobP->m_jobCode, curJobP->m_proceMachs[machIndex].second), timeWinToInsertFinal));

				curMachP->m_allocatedTimeWinPs.push_back(make_pair(
					make_pair(curJobP->m_jobCode, curJobP->m_proceMachs[machIndex].second), timeWinPToInsert));
				curJobP->m_allocatedTimeWinPs.push_back(make_pair(
					make_pair(curMachP->m_machCode, curJobP->m_proceMachs[machIndex].second), timeWinPToInsert));
			}
			else    // 插入到该机器的所有时间窗的中间
			{
				auto iter1 = curMachP->m_allocatedTimeWin.begin();
				for (int i = 0; i < insertPositionFinal; ++i) ++iter1;
				curMachP->m_allocatedTimeWin.insert(iter1, make_pair(
					make_pair(curJobP->m_jobCode, curJobP->m_proceMachs[machIndex].second), timeWinToInsertFinal));

				auto iter2 = curMachP->m_allocatedTimeWinPs.begin();
				for (int i = 0; i < insertPositionFinal; ++i) ++iter2;
				// 根据最终得出的插入位置insertPositionFinal，在某该机器上插入该工件;
				curMachP->m_allocatedTimeWinPs.insert(iter2, make_pair(
					make_pair(curJobP->m_jobCode, curJobP->m_proceMachs[machIndex].second), timeWinPToInsert));
				curJobP->m_allocatedTimeWinPs.push_back(make_pair(
					make_pair(curMachP->m_machCode, curJobP->m_proceMachs[machIndex].second), timeWinPToInsert));
				// 直接放入，还是插入并挪后面的时间窗
				if (!IsJustPutInFinal)// 需要挪动后面的时间窗
				{
					moveTheFollowedTimeWin(jobsMapLocal, machsMapLocal, curMachP, insertPositionFinal);
					syncTimeWinANDTimeWinPs(machsMapLocal);
				}

			}
			readyTimeForJob = timeWinToInsertFinal.last(); //下一个工序的最早开始时间

			++machIndex;
		} // END OF 遍历某个job的所有machine


		cout << "tyyutyuiuviiiiiinod" << endl;
		for (auto& machInfo : machsMapLocal)   // 遍历某个job的所有machine
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

	} // END OF 遍历所有job

	//syncTimeWinANDTimeWinPs(machsMapLocal);

	//if (jobOrder.size() >= 80) std::cout << "uiuiuiui" << endl;
	// 打印最终排程结果
	if (isPrint) printFinalRes2(jobsMapLocal, machsMapLocal);
	// 输出到CSV文件
	// if (isPrint) writeToCSV(jobsMapTemp, machsMapTemp, mysql);

	//if (jobOrder.size() >= 80) std::cout << "mmmmmmmmmmmmmm" << endl;

	// 获取目标值
	//pair<double, double> objectVals = getObjVals(jobsMapTemp, machsMapTemp);
	//std::cout << "总延期时间(小时): " << objectVals.first << "; 加工所有工件所需的时间长度(小时): " << objectVals.second << std::endl;

	//if (jobOrder.size() >= 80)  std::cout << "aaaaaaaaaaaaaaaaaaaaa" << endl;


	return getMakespan(jobsMapLocal, machsMapLocal);
};

// 钟罩炉先组好批次
pair<double, double> scheduleByJobOrder4(vector<pair<string, Job*>>& jobOrder,
	map<string, Job*>& jobsMapTemp, map<string, Mach*>& machsMapTemp, vector<vector<pair<Job*, int>>>& batchOfBell, MYSQL* mysql, bool isPrint) {
	//if (jobOrder.size() >= 79)std::cout << "curJobsize: " << jobOrder.size() << std::endl;
	int jobIndex = 0;
	for (auto& jobInfo : jobOrder)
	{
		Job* curJobP = jobInfo.second;
		//cout<< "curJob: " << curJobP->m_jobCode
			//<<"dd"<<curJobP->m_proceMachs.size()<< std::endl;
		unsigned machIndex(0);  // job的 machine
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // 遍历某个job的所有machine
		{
			if ("9800257701-0-0" == curJobP->m_jobCode)
			{
				std::cout << "         curMach: " << machCodeOfCurJob.first << endl;
				cout << "  " << curJobP->m_allocatedTimeWin.size() << endl;
			}

			bool isSuccess;
			if (CodeOfBellFurn == machCodeOfCurJob.first)  // 是钟罩炉
			{
				Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				int batchIndexFinal = insertJob(*curJobP, *curMachP, batchOfBell, machsMapTemp);
				if (batchIndexFinal >= 0)
				{
					vector<pair<Job*, int>>& curBatch = batchOfBell[batchIndexFinal];  // 含有当前job的批次
					vector<pair<Job*, int>> reentryBatch;  // 钟罩炉重入的批次
					for (auto it = curBatch.begin(); it != curBatch.end(); ++it) // 对该批次的job，排钟罩炉后续工序
					{
						Job* curJobP = it->first;
						int machIndex = curJobP->m_allocatedTimeWin.size();
						for (; machIndex < curJobP->m_proceMachs.size(); ++machIndex)     // 遍历job的后续工序，直到遇到钟罩炉
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
								reentryBatch.push_back(make_pair(curJobP, 1));  //放入可重入batch
								break;
							}
							insertJob(*curJobP, *curMachP, machIndex);
						}
					}
					if (reentryBatch.size() > 0)  // 对该批次的job，如果在钟罩炉处有重入
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
					for (auto it = reentryBatch.begin(); it != reentryBatch.end(); ++it) // 对该批次的job，排钟罩炉后续工序
					{
						Job* curJobP = it->first;
						int machIndex = curJobP->m_allocatedTimeWin.size();
						for (; machIndex < curJobP->m_proceMachs.size(); ++machIndex)     // 遍历job的后续工序，直到遇到钟罩炉
						{
							Mach* curMachP = machsMapTemp[curJobP->m_proceMachs[machIndex].first];
							if (curMachP->m_machCode == CodeOfBellFurn)
							{	break;}
							insertJob(*curJobP, *curMachP, machIndex);
						}
					}
					break;
				}
			}
			else if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // 是气垫炉组中的机器
			{
				Mach_AirFurnace* curMachP = static_cast<Mach_AirFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMapTemp);
			}
			else  //其他
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
		++jobIndex;
	}

	//if (jobOrder.size() >= 80) std::cout << "uiuiuiui" << endl;
	// 打印最终排程结果
	if (isPrint) printFinalRes(jobsMapTemp, machsMapTemp);
	// 输出到CSV文件
	// if (isPrint) writeToCSV(jobsMapTemp, machsMapTemp, mysql);

	//if (jobOrder.size() >= 80) std::cout << "mmmmmmmmmmmmmm" << endl;

	// 获取目标值
	pair<double, double> objectVals = getObjVals(jobsMapTemp, machsMapTemp);
	//std::cout << "总延期时间(小时): " << objectVals.first << "; 加工所有工件所需的时间长度(小时): " << objectVals.second << std::endl;

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


	queue<pair<Mach*, pair<int, int>>> changedTimeWins;  // queue<pair<Mach*, pair<int, int>> 队列，放入改变的时间窗和需要挪的时间窗索引
	IndexOfTimeWin += 1;
	int IndexOfTimeWin2 = IndexOfTimeWin;  // 


	auto timeInfo_iter = curMachP->m_allocatedTimeWin.begin();
	for (int i = 0; i < IndexOfTimeWin; ++i) ++timeInfo_iter;
	ptime earlistStartT = (--timeInfo_iter)->second.last();  // 开始时间

	// 检查当前位置之后的时间是否需要往后挪
	for (auto timeInfoMove_iter = timeInfo_iter; timeInfoMove_iter != curMachP->m_allocatedTimeWin.end();
		++timeInfoMove_iter, ++IndexOfTimeWin2)
	{

		ptime left_TimeWin2 = timeInfoMove_iter->second.begin();
		ptime right_TimeWin2 = timeInfoMove_iter->second.last();
		if (earlistStartT <= left_TimeWin2)
			break;
		else {
			// 往后挪
			time_duration processTimeWin = timeInfoMove_iter->second.last()
				- timeInfoMove_iter->second.begin();
			//timeInfoMove_iter->second = time_period( earlistStartT, earlistStartT + processTimeWin);
			timeInfoMove_iter->second.shift(earlistStartT- timeInfoMove_iter->second.begin());

			earlistStartT = timeInfoMove_iter->second.last();  // 更新开始时间

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


	// 对于有需要往后挪的时间窗的机组
	while (changedTimeWins.size())
	{
		
		pair<Mach*, pair<int, int>>& changedTimeWin = changedTimeWins.front();
		int indexFro = changedTimeWin.second.first;  // 需要挪的时间窗索引
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



		// 遍历某机组上的挪动的时间窗
		cout <<"  machCode="<< machPChanged->m_machCode << " indexFro=" << indexFro << " indexRear=" << indexRear << endl;
		for (int indexMachWin = indexFro; indexMachWin < indexRear; indexMachWin++)
		{
			//pair<pair<string, unsigned>, time_period>& timeWinInfo = machPChanged->m_allocatedTimeWin[indexMachWin];  // 这个是原版
			pair<pair<string, unsigned>, time_period>& timeWinInfo = *machPChanged->m_allocatedTimeWin.begin();
			
			Job* jobPChanged = jobsMapTemp[timeWinInfo.first.first];

			int i = 0; // job的工序索引
			for (; i < jobPChanged->m_proceMachs.size(); i++) // 对于该工序上的某个时间窗，有对应job，获取该工序在该job的所有工序中的索引
			{ 
				if (make_pair(curMachP->m_machCode, timeWinInfo.first.second) == jobPChanged->m_proceMachs[i])
					break;
			}
			if (jobPChanged->m_proceMachs.size() - 1 == i) continue;

			
			cout << "    jobPChanged=" << jobPChanged->m_jobCode << endl;
			cout << "     indexFro=" << indexFro << "  indexRear=" << indexRear << "  indexMachWin=" << indexMachWin << endl;
			cout << "    i_jobb ="<< i <<endl;
			
			ptime endTOfLastProcess = timeWinInfo.second.last();;  // job的上一个工序的时间窗的右侧
			for (int j = i+1; j < jobPChanged->m_proceMachs.size(); j++)  // 对于某时间窗对应的job，遍历该job的后续工序
			{
				Mach* machSuccessorP = machsMapTemp[jobPChanged->m_proceMachs[j].first];  // job的当前处理工序的后续工序
				pair<string, unsigned int>& jobProccessInfo = jobPChanged->m_proceMachs[j];

				int startMachIndex = 0;
				for (; startMachIndex < machSuccessorP->m_allocatedTimeWin.size(); startMachIndex++)
					// 对job的某后续工序的机组，定位到该job的时间窗
				{
					if (make_pair(jobPChanged->m_jobCode, jobProccessInfo.second)
						== machSuccessorP->m_allocatedTimeWin.begin()->first)
						//== machSuccessorP->m_allocatedTimeWin[startMachIndex].first)  //原版
					{
						//cout<<"ddddddgdgd"<<endl;
						break;
					}
						
				}
				cout << "      job的后续machCode=" << machSuccessorP->m_machCode <<"j="<<j<< "  startMachIndex=" << startMachIndex << endl;

				int IndexTimeWin;
				ptime endTOfLastTimeWin = endTOfLastProcess;  // 对于该工序，最早开始时间是上一个工序的结束时间
				for (IndexTimeWin = startMachIndex; IndexTimeWin < machSuccessorP->m_allocatedTimeWin.size(); IndexTimeWin++)
					// 对该job的某后续工序，遍历该机组后面的时间窗，判断是否需要往后挪
				{

					//cout << "           endTOfLastTimeWin=" << to_iso_extended_string(endTOfLastTimeWin) << endl;


					/* // 原版没注释
					if (endTOfLastTimeWin <= machSuccessorP->m_allocatedTimeWin[IndexTimeWin].second.begin())
						break;
					else {
						// 往后挪
						time_duration processTimeWin = machSuccessorP->m_allocatedTimeWin[IndexTimeWin].second.last()
							- machSuccessorP->m_allocatedTimeWin[IndexTimeWin].second.begin();
						machSuccessorP->m_allocatedTimeWin[IndexTimeWin].second.shift(endTOfLastTimeWin - machSuccessorP->m_allocatedTimeWin[IndexTimeWin].second.begin());
						endTOfLastTimeWin = machSuccessorP->m_allocatedTimeWin[IndexTimeWin].second.last();  // 更新开始时间
	
					//	cout << "          right_TimeWin__22=" << to_iso_extended_string(machSuccessorP->m_allocatedTimeWin[IndexTimeWin].second.last()) << endl;
					}
					*/
					if (IndexTimeWin == startMachIndex)  // 第一个时间窗
						endTOfLastProcess = endTOfLastTimeWin;
				}
				if(IndexTimeWin == startMachIndex) break;  // 如果对于（对job的某后续工序，遍历该机组后面的时间窗），如果后面的第一个时间窗就不需要挪，则不需要遍历job的后续的工序

				if (IndexTimeWin > startMachIndex + 1)    // 如果对于（对job的某后续工序，遍历该机组后面的时间窗），如果只有后面的第一个时间窗需要挪，则不需要处理该时间窗的后续时间窗
				{
					changedTimeWins.push(make_pair(machSuccessorP, make_pair(startMachIndex + 1, IndexTimeWin)));
					cout << "        machSuccessorP=" << machSuccessorP->m_machCode << " startMachIndex=" << startMachIndex
						<< " IndexTimeWin=" << IndexTimeWin << endl;
				
				}
			} // END OF 对于某时间窗对应的job，遍历该job的后续工序

		}// END OF 遍历某机组上的挪动的时间窗

		changedTimeWins.pop();
	}

};

// 同步 和
void syncTimeWinANDTimeWinPs(map<string, Mach*>& machsMapTemp)  // 同步m_allocatedTimeWinPs和m_allocatedTimeWin
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

// --------END OF--排产主函数相关，一个一个排入每个工--------



// --------迭代排产相关，拷贝job或machine--------

// 拷贝Job--把jobOrder中的所有job拷贝一下，放入到jobsMapTemp中
void initJobsTemp(map<string, Job*>& jobsMapTemp, vector<pair<string, Job*>>& jobOrderTemp, vector<pair<string, Job*>>& jobOrder) {

	for (auto& ele : jobOrderTemp) {
		Job* jobP = ele.second;
		delete jobP;
	}
	jobOrderTemp.clear();

	for(int i=0;i<jobOrder.size();++i)
	{
		Job* jobP = new Job;
		*jobP = *jobOrder[i].second;
		jobOrderTemp.push_back(make_pair(jobP->m_jobCode, jobP));
		jobOrderTemp[i].first = jobOrder[i].first;
		jobOrderTemp[i].second = jobP;
		jobsMapTemp.insert(make_pair(jobP->m_jobCode, jobP));
	}
};

// 拷贝machine--把machsMap中的所有machine拷贝一下，放入到machsMapTemp中
void initMachsMapTemp(map<string, Mach*>& machsMapTemp, map<string, Mach*>& machsMap)
{	
	for (auto& machInfo : machsMapTemp)
	{
		if (CodeOfBellFurn == machInfo.second->m_machCode)  // 是钟罩炉
		{
			Mach_BellFurnace* machP = new Mach_BellFurnace();
			*machP = *(static_cast<Mach_BellFurnace*> (machInfo.second));
			delete machP;
		}
		else if (airFurnaceSet.find(machInfo.second->m_machCode) != airFurnaceSet.end()) // 是气垫炉
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
	machsMapTemp.clear();

	for (auto& machInfo : machsMap)
	{

		if (CodeOfBellFurn == machInfo.second->m_machCode)  // 是钟罩炉
		{
			Mach_BellFurnace* machP = new Mach_BellFurnace();
			*machP = *(static_cast<Mach_BellFurnace*> (machInfo.second));
			machsMapTemp.insert(make_pair(machInfo.second->m_machCode, machP));
		}
		else if (airFurnaceSet.find(machInfo.second->m_machCode) != airFurnaceSet.end()) // 是气垫炉
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

// 拷贝排产时间窗--把jobOrderOrig中的job的排产时间窗拷贝给jobOrder中的job
void resetJobsTemp(vector<pair<string, Job*>>& jobOrder, vector<pair<string, Job*>>& jobOrderOrig) {
	for (int i = 0; i < jobOrderOrig.size(); ++i)
	{
		Job* jobP = jobOrder[i].second;
		Job* jobPOrig = jobOrderOrig[i].second;
		if (jobP->m_jobCode != jobP->m_jobCode)
			cout << "ERROR! the two jobs is different!";
		jobP->m_curMachIndex = jobPOrig->m_curMachIndex;
		jobP->m_allocatedTimeWin.assign(jobPOrig->m_allocatedTimeWin.begin(), jobPOrig->m_allocatedTimeWin.end());
	}
};

// 拷贝排产时间窗--把machsMapOrig中的job的排产时间窗拷贝给machsMap中的machine
void resetMachsMapTemp(map<string, Mach*>& machsMap, map<string, Mach*>& machsMapOrig) {
	for (auto& machInfo : machsMapOrig)
	{
		Mach* machPOrig = machInfo.second;
		Mach* machP = machsMap[machPOrig->m_machCode];
		if (CodeOfBellFurn == machInfo.second->m_machCode)  // 是钟罩炉
		{
			Mach_BellFurnace* machPOrigBell = (static_cast<Mach_BellFurnace*> (machPOrig));
			Mach_BellFurnace* machPBell = (static_cast<Mach_BellFurnace*> (machP));

			machPBell->m_timeLines.clear();
			machPBell->m_timeLines = vector<list<TimeWin>>(machPBell->m_numOfParallel);
			for (int i = 0; i < machPBell->m_numOfParallel; ++i) {
				machPBell->m_timeLines[i].assign(machPOrigBell->m_timeLines[i].begin(), machPOrigBell->m_timeLines[i].end());
			}
		}
		else{
			machP->m_allocatedTimeWin.assign(machPOrig->m_allocatedTimeWin.begin(), machPOrig->m_allocatedTimeWin.end());
		}
			
	}
};

// 简单拷贝machine
void reInitMachsMapTemp(map<string, Mach*>& machsMapTemp, map<string, Mach*>& machsMap) {
	for (auto& machInfo : machsMapTemp)
	{
		*(machInfo.second) = *machsMap[(machInfo.first)];
	}
};

// --------END OF--迭代排产相关，拷贝job或machine--------



// --------其他--------

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

// --------END OF--其他结束--------


void main()
{
	unsigned seed = 50;
	// unsigned seed = 500;

	srand(seed);

	vector<string> jobsCodeVec;     // vector<job code>
	vector<string> machsCodeVec;    // vector<machine code>
	map<string, Job*> jobsMap;      // map<jobCode, Job*>
	map<string, Mach*> machsMap;    // map<machCode, Mach*>
	//ptime curTime = from_iso_string("20200610T000000");  // 当前时间
	//ptime curTime = from_iso_string("20200501T0000000");

	MYSQL* mysql = new MYSQL;
	MYSQL_RES* res(NULL);   //这个结构代表返回行的一个查询结果集  
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


	// 打印制程
	printProcessLine(jobsMap, machsMap);
	

	// 打印jobCodeVec
	map<string, vector<Job*>> jobsWithBell;  // map<牌号, vector<Job*>>
	map<string, pair<Job*, int>> jobsWithBells;  // map<牌号, pair<Job*, int>> 

	vector<BellBatch*> jobsBatch;  // pair<牌号, vector<Job*>>
	initialJobsBatch(jobsMap, machsMap, jobsCodeVec, jobsWithBell, jobsWithBells, jobsBatch);

	// 打印钟罩炉工序统计个数
	printBellCount(jobsMap);


	// myInitialization(jobsCodeVec, machsCodeVec, jobsMap, machsMap);
	vector<pair<Job*, ptime>> jobsWithDueDate;               // vector<pair<Job*, 截止时间>>
	vector<pair<Job*, time_duration>> jobsWithTotalProTime;  // vector<pair<Job*, 总加工时间>>
	vector<pair<Job*, time_duration>> jobsWithSlackTime;  // vector<pair<Job*, 松弛时间>>   松弛时间：job的剩余时间（当前时点距离交货期的时间）与该job所需的加工时间之差
	// 初始化不同的job vector，处理时间，松弛时间，截止时间
	initialJobVecs(jobsMap, machsMap, jobsWithDueDate, jobsWithTotalProTime, jobsWithSlackTime);

	// 根据截止时间，总加工时间，松弛时间排序
	sort(jobsWithDueDate.begin(), jobsWithDueDate.end(), myCmpBy_ptime);
	sort(jobsWithTotalProTime.begin(), jobsWithTotalProTime.end(), myCmpBy_time_duration);
	sort(jobsWithSlackTime.begin(), jobsWithSlackTime.end(), myCmpBy_time_duration);

	// 打印交期、处理时间、松弛时间
	printDueProSlackTime(jobsWithDueDate, jobsWithTotalProTime, jobsWithSlackTime);


	

	/*
	vector<vector<pair<Job*, int>>> batchOfBell;  // vector<vector<pair<Job*, 标志状态>>, 牌号>  0:未到达; 1:在等待; 2:已组批
	vector<pair<string, Job*>> jobOrder;
	vector<pair<vector<pair<Job*, double>>, string>> jobsWithBellFurn;  // vector<pair<vector<pair<Job*, 宽度>>, 牌号>> 同一牌号的在一块
	for (int i = 0; i < jobsWithSlackTime.size(); ++i)  // 一个一个地放入job
	{
		Job* curJobP = jobsWithSlackTime[i].first;
		jobOrder.push_back(make_pair(curJobP->m_jobCode, curJobP));
		bool isIncludeBell = false;
		int indexOfMach=0;
		for (pair<string, unsigned>& machInfoOfCurJob : curJobP->m_proceMachs)   // 遍历某个job的所有machine
		{
			if (CodeOfBellFurn == machInfoOfCurJob.first)
			{
				isIncludeBell = true;
				break;
			}
			++indexOfMach;
		}
		bool isPutIn=false;  //是否已经放入
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
	cout << " 牌号个数=" <<jobsWithBellFurn.size() << endl;

	// 组批
	int batchCount = 0;
	for (auto &jobsOfSameAlloyGrade:jobsWithBellFurn)  // 对每一种牌号
	{
		string alloyGrade = jobsOfSameAlloyGrade.second;
		vector<pair<Job*, double>> tempBatch;
		cout << "batchCount=" << batchCount << endl;
		cout << " numOfSame=" << jobsOfSameAlloyGrade.first.size() << endl;
		batchCount++;
		int num = 0;
		for (auto it=jobsOfSameAlloyGrade.first.begin(); it != jobsOfSameAlloyGrade.first.end(); ++it, ++num)  // 一个一个地放入job
		{
			cout << "  num=" << num << endl;
			cout << "  tempBatchSize()=" <<tempBatch.size()<<endl;
			Job* curJobP = it->first;
			double widthOfCurJob = it->second;
			if(tempBatch.size()<=1) //1个，放入
				tempBatch.push_back(*it);
			else //2个
			{
				if ((widthOfCurJob <850))
					tempBatch.push_back(*it);  // 放入
				else
				{
					pair<Job*, double> tempJob;
					tempJob.first = tempBatch[1].first;
					tempJob.second = tempBatch[1].second;
					tempBatch.pop_back();
					tempBatch.push_back(make_pair(curJobP, widthOfCurJob));
					// 放入
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
	vector<pair<string, Job*>> jobWithBell;  // 有钟罩炉的job
	std::cout << "jobsWithSlackTime.size()" << jobsWithSlackTime.size() << std::endl;
	vector<pair<string, Job*>> jobOrder;

	for (int i = 0; i < 3; ++i)  // 一个一个地放入job //第7个死循环了
	//for (int i = 0; i < jobsWithSlackTime.size(); ++i)  // 一个一个地放入job //
	//for (int i = jobsWithTotalProTime.size()-1; i>=0; --i)
	//for (auto& jobInfo : jobsMap)
	{
		if (11 == i) continue;
		//Job* curJobP = jobInfo.second;
		Job* curJobP = jobsWithSlackTime[i].first;
		cout << curJobP->m_jobCode << endl;
		if (0 & ("0119331020-0-0" == curJobP->m_jobCode )) { // curJobP->m_jobCXode=0118381020-P-0-0
			// 0118381020-P-0-0前  // 0703061100-0-0   0118381020-P-0-0
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





	cout << "heae" << endl;
	char p; cin >> p;

	NEH_Method(jobsMap, machsMap, mysql, jobsWithDueDate, jobsWithTotalProTime, jobsWithSlackTime);

	//GA_Method(machsMap, mysql, jobsWithDueDate, jobsWithTotalProTime, jobsWithSlackTime);


	for (auto& jobInfo : jobsMap) delete jobInfo.second;
	for (auto& machInfo : machsMap) delete machInfo.second;





	/*
	vector<pair<string, Job*>> jobWithBell;  // 有钟罩炉的job
	int lastJobWithBell(0);
	map<string, int> bellCountRecorder;
	map<string, int> positionRecord;

	std::cout << "jobsWithSlackTime.size()" << jobsWithSlackTime.size() << std::endl;
	vector<pair<string, Job*>> jobOrder;

	for (int i = 0; i < jobsWithSlackTime.size(); ++i)  // 一个一个地放入job //
	//for (int i = jobsWithTotalProTime.size()-1; i>=0; --i)
	//for (auto& jobInfo : jobsMap)
	{
		//Job* curJobP = jobInfo.second;
		Job* curJobP = jobsWithSlackTime[i].first;

		
		//bellCountRecorder.insert(make_pair(curJobP->m_jobCode, 0));
		//int machIndex=0;
		//for (pair<string, unsigned>& machInfoOfCurJob : curJobP->m_proceMachs)   // 遍历某个job的所有machine
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




















