#include<head.h>
ptime curTime = from_iso_string("20200610T000000");

set<string> rollingmachFrequ{ "BD-S003","BD-S005","BD-S025","BD-S009","BD-S010","BD-S011" };  // 轧机组; getStatus()用
// BD-S003	二辊可逆式热轧机; BD-S005	粗轧机; BD-S025	四辊中轧机; 
// BD-S009	森德威20辊精轧机; BD-S010	20辊精轧机; BD-S011	4辊精轧机

set<string> cut_StretchmachFrequ{ "BD-S018","BD-S015",
					"BD-S016","BD-S019","BD-S020","BD-S021" };  // 横纵剪和拉弯矫; getStatus()用
// BD-S018	1250横剪机组; BD-S015	650拉弯矫; 
// BD-S016	1250纵剪; BD-S019	650薄纵剪; BD-S020	650厚纵剪; BD-S021	350纵剪

set<string> washmachFrequ{ "BD-S012","BD-S013","BD-S014" };
// BD-S012	1250清洗机列; BD-S013	650清洗机列; BD-S014	新650清洗机列; getStatus()用

set<string> airFurnaceSet{ "BD-S024","BD-S007","BD-S008" };
//BD-S024  WSP气垫炉;    BD-S007  1250气垫式退火炉组;    BD-S008  650气垫式退火炉组

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
	make_pair("TU2", "copper"),};

string getTypeFromGrade(string alloyGrade)
{
	return alloyMap[alloyGrade];
}

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

// 判断_number是否在_aRange(格式为"[0.1,3.4]")的范围内
bool isInRange(string _aRange, double _number)
{
	if (("null" == _aRange) ||("NULL" == _aRange) 
		|| ("Null" == _aRange) || ("" == _aRange)
		|| (_number<=0.0))
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
			std::cout<<"ERROR with '" << _aRange <<"'！ 不能解析！ "<<std::endl;
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
		if (("BD-S009" == machCode)||("BD-S010" == machCode)) // 是BD-S009  森德威20辊精轧机
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
time_duration getProcessTime(Mach* machP, Job * jobP, unsigned machIndex)
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
	if (0 == processT)
		std::cout<<"Error with capacity! Cannot find a matched capacity of job '"
		<<jobP->m_jobCode<<"' on machine "<< machP->m_machCode <<"!"<<std::endl;

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
		std::cout<<"Error with str-'"<<str<< "'! Str is NULL！"<<std::endl;
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

// 钟罩炉，是否可以加入钟罩炉的某个timeWin
bool canAddToTimeWin(string jobCode, unsigned machIndexOfJob, TimeWin &timeWin, Mach_BellFurnace& curMachBell)
{
	if (curMachBell.m_RuleForFurnWithWidth.second.second == timeWin.first.size()) // 已经有3个了，不能插入
		return false;
	else if (curMachBell.m_RuleForFurnWithWidth.second.first == timeWin.first.size())  // 已经有2个了
	{
		for (auto& jobInfo: timeWin.first)
		{
			Job* jobP = jobInfo.first;
			double curWidth(0); //job的当前宽度

			for (int i = machIndexOfJob; i >= 0; --i)
			{
				if (jobP->m_proceTargets[i].second.m_targetWidth > 0.0) // 从当前，往前找目标宽度大于0的值 
				{
					curWidth = jobP->m_proceTargets[i].second.m_targetWidth;
					break;
				}
			}
			if (curWidth <= 0.0)   // 若目标宽度都≤0 
				curWidth = jobP->m_initialInfos.m_targetWidth;
			if ((curWidth >= curMachBell.m_RuleForFurnWithWidth.first)//??？？job的宽度规格按哪个？
				||(jobCode ==jobP->m_jobCode))  // 不能是job自己
				return false;
		}
	}
	return true;
}

// airFurnaceSet
bool getIsSwitch(Job& curJob, Mach& curMach, Job& otherJob)  // 是否需要切换
{
	if (airFurnaceSet.find(curMach.m_machCode) != airFurnaceSet.end())  // 是气垫炉组的机器
	{
		//cout << otherJob.m_jobCode <<" "<< curJob.m_jobCode
			//<<" otherJob.m_alloyType=" << otherJob.m_alloyType<<"curJob.m_alloyType"<< curJob.m_alloyType << endl;
		if (otherJob.m_alloyType == curJob.m_alloyType) return false;
		else return true;
	}
	return false;
};

// 把某工单排入某个气垫炉
bool  insertJob(Job& curJob, Mach_AirFurnace& curMach, unsigned machIndexOfJob, map<string, Job*> &jobsMap)
{
	time_duration& timeDuration = curJob.m_proceTimes[machIndexOfJob].second;
	time_duration& switchT = double2timeDuration(curMach.m_timeOfSwith);
	
	bool isInserted(false);
	unsigned IndexOfTimeWin(0);  // machine的插入的索引

	ptime readyTimeForOrder;  // job可以在该机器上开始加工的时间
	time_period timeWinToInsert(curJob.m_startDateOfOrder, timeDuration);  //要插入的时间窗
	bool isSwitchPre;  // 考虑前面的机器，中间是否需要转换时间
	if (0 == machIndexOfJob)  // 如果是job的第一个machine
		readyTimeForOrder = curJob.m_startDateOfOrder;
	else
		readyTimeForOrder = (curJob.m_allocatedTimeWin.end() - 1)->second.end();
	//if ("BD-S008" == curMach.m_machCode) cout << "yyyyyyyyyyyyy" << endl;

	if (0 == curMach.m_allocatedTimeWin.size())  // 如果是machine上的第一个job
	{
		timeWinToInsert = time_period(readyTimeForOrder, timeDuration);
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
			//cout << "isSwitchNext=" << isSwitchNext << endl;
			if (left_TimeWin < readyTimeForOrder + timeDuration + (isSwitchNext ? switchT : double2timeDuration(0.0)))  // 可加工时间和时间窗开始时间之间一定放不下
				continue;
			
			if (curMach.m_allocatedTimeWin.cbegin() == timeInfo_iter)  // 第一个time window
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
		}  // END OF 遍历curMach.m_allocatedTimeWin

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


// 把某工单排入某机器，返回插入时间窗和插入索引
pair<unsigned, bool> preInsertJobToMach(Job& curJob, Mach_BellFurnace& curMach, unsigned machIndexOfJob, vector<TimeWin>& timeline, time_period& timeWinResult)
{
	time_duration& timeDuration = double2timeDuration(curMach.m_proceTimePerFurn);  //钟罩炉时间为30小时
	//time_duration timeOfSwith = double2timeDuration(curMach.m_timeOfSwith);  // 开启新的一炉的转换时间
	//准备时间和上一炉的加工完成时间，是否要记入转换时间，需要搞明白？？??
	//   对job来讲，准备时间后不需要转换时间；对machine来讲，上一炉结束时间后需要转换时间！

	unsigned IndexOfTimeWin(0);  // machine的时间线插入的索引
	// using TimeWin = pair<vector<pair<Job*, unsigned>>, time_period>;  // pair< vector<pair<Job*, 该job第几次重入>>, job插入的时间窗>

	bool flag_NewFurn(false);  // 显示是否新开一炉
	bool isInserted(false);

	ptime readyTimeForOrder;  // job可以在该机器上开始加工的时间
	//time_period timeWinToInsert(curJob.m_startDateOfOrder, timeDuration);  //要插入的时间窗
	if (0 == machIndexOfJob)  // 如果是job的第一个machine
		readyTimeForOrder = curJob.m_startDateOfOrder;
	else
		readyTimeForOrder = (curJob.m_allocatedTimeWin.end() - 1)->second.end();

	//cout << " timeline.size()=" << timeline.size()
	//	<< "  readyTimeForOrder="<<to_iso_extended_string(readyTimeForOrder)<<endl;

	if (0 == timeline.size())  // 如果是machine上的第一个job
	{
		timeWinResult = time_period(readyTimeForOrder+double2timeDuration(5.0), timeDuration); //??？？!!!!!!!!!
		flag_NewFurn = true;  // 新开一炉
		IndexOfTimeWin = 0;
		isInserted = true;
	}
	else
	{
		// 遍历timeline
		for (vector<TimeWin>::iterator timeInfo_iter = timeline.begin();
			timeInfo_iter != timeline.end(); ++timeInfo_iter, ++IndexOfTimeWin)
		{
			ptime left_TimeWin = timeInfo_iter->second.begin();
			ptime right_TimeWin = timeInfo_iter->second.last();
			//cout << to_iso_extended_string(left_TimeWin)
				//<<"  IndexOfTimeWin="<< IndexOfTimeWin << endl;

			if (left_TimeWin < readyTimeForOrder + timeDuration)  // 可加工时间和时间窗开始时间之间一定放不下新的一炉
			{
				if ((readyTimeForOrder < left_TimeWin) && canAddToTimeWin(curJob.m_jobCode, machIndexOfJob, *timeInfo_iter, curMach))
				{
					timeWinResult = timeInfo_iter->second;
					flag_NewFurn = false; // 不新开一炉
					isInserted = true;
					break;
				}
				continue;
			}

			if (timeline.cbegin() != timeInfo_iter)  // 第一个time window
			{
				auto timeInfo_iter2 = timeInfo_iter - 1;
				ptime right_Pre = timeInfo_iter2->second.end();
				if ((readyTimeForOrder < right_Pre) && (canAddToTimeWin(curJob.m_jobCode, machIndexOfJob, *timeInfo_iter, curMach))
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
			for (vector<TimeWin>::iterator timeInfo_iter = timeline.begin();
				timeInfo_iter != timeline.end(); ++timeInfo_iter, ++IndexOfTimeWin)
			{
				ptime left_TimeWin = timeInfo_iter->second.begin();
				ptime right_TimeWin = timeInfo_iter->second.last();

				if (left_TimeWin < readyTimeForOrder + timeDuration)  // 可加工时间和时间窗开始时间之间一定放不下新的一炉
					continue;

				if (timeline.cbegin() == timeInfo_iter)  // 第一个time window
				{
					//std::cout << "**************** " << std::endl;
					timeWinResult = time_period(readyTimeForOrder, timeDuration);
					flag_NewFurn = true;    // 新开一炉
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
						flag_NewFurn = true;    // 新开一炉
						isInserted = true;
						break;
					}
					if (right_Pre <= readyTimeForOrder)
					{
						timeWinResult = time_period(readyTimeForOrder, timeDuration);
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
			ptime rightWin = (timeline.end() - 1)->second.end();
			timeWinResult = time_period((readyTimeForOrder < rightWin) ? rightWin : readyTimeForOrder, timeDuration);
			flag_NewFurn = true;    // 新开一炉
			isInserted = true;
		}
	}
	//std::cout << "curJob=" << curJob.m_jobCode <<" 777777"<< std::endl;
	//std::cout << "      timeWinResult=" << to_iso_extended_string(timeWinResult.begin()) << endl;
	//<<"--" << to_iso_extended_string(timeWinResult.last()) << std::endl;
	return make_pair(IndexOfTimeWin, flag_NewFurn);
};

// 把某工单排入钟罩炉的某个平行机
bool  insertJob(Job& curJob, Mach_BellFurnace& curMach, unsigned machIndexOfJob)
{
	time_duration& timeDuration = curJob.m_proceTimes[machIndexOfJob].second;

	pair<unsigned, string> SchedulInfo;  // pair<时间窗位置, 显示是否新开一炉>  "newFurnace":新开一炉; "addToExist":新开一炉

	unsigned Index_parallelMach = 0;
	unsigned Index_final = Index_parallelMach;
	time_period timeWinFinal(curJob.m_startDateOfOrder, timeDuration);
	pair<int, bool> resultOfPreInsert(make_pair(-1, false));   // pair<TimeWin的位置, 是否要新开一炉>
	pair<int, bool> resultOfFinal(make_pair(-1, false));   // pair<TimeWin的位置, 是否要新开一炉>
	//if ("0118741020-0-0" == curJob.m_jobCode) cout << "eeeeeeeeeeeeee" << endl;
	//std::cout << "size(m_numOfParallel) == "<< curMach.m_timeLines.size()<<std::endl;

	pair<pair<int, int>, time_period> newFurn = (make_pair(pair<unsigned, unsigned>(make_pair(-1, -1)), time_period(curJob.m_startDateOfOrder, timeDuration))); // pair<pair< 第几个timeline, 插入位置>, 时间窗>
	pair<pair<int, int>, time_period> notNewFurn = (make_pair(pair<unsigned, unsigned>(make_pair(-1, -1)), time_period(curJob.m_startDateOfOrder, timeDuration))); // pair<pair< 第几个timeline, 插入位置>, 时间窗>

	for (auto& timeline : curMach.m_timeLines)
	{
		time_period timeWinResult(curJob.m_startDateOfOrder, timeDuration);

		resultOfPreInsert = preInsertJobToMach(curJob, curMach, machIndexOfJob, timeline, timeWinResult);

		if (resultOfPreInsert.second) // 是新开1炉
		{
			if (newFurn.first.first < 0.0)
				newFurn = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
			else if (newFurn.second.begin() > timeWinResult.begin())
				newFurn = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
		}
		else // 不是新开1炉
		{
			if (notNewFurn.first.first < 0.0)
				notNewFurn = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
			else if (notNewFurn.second.begin() > timeWinResult.begin())
				notNewFurn = make_pair(make_pair(Index_parallelMach, resultOfPreInsert.first), timeWinResult);
		}
		cout << "  Index_parallelMach=" << Index_parallelMach << endl;
		++Index_parallelMach;
	}

	int insertPosition(-1);  // 插入位置
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

	if (isNew) // 新开一炉
		timeLine.insert(timeLine.begin() + insertPosition, make_pair(jobToInsert, timeWinFinal));
	else  // 加入已开的炉
		timeLine[insertPosition].first.insert(make_pair(&curJob, num_reentry));

	return true;
};


// 把某工单排入某机器
bool  insertJob(Job& curJob, Mach& curMach, unsigned machIndexOfJob)
{
	time_duration& timeDuration = curJob.m_proceTimes[machIndexOfJob].second;
	//time_duration& swithT = double2timeDuration(curMach.m_timeOfSwith);  // 切换时间
	//bool swith = isSwitchPre(curJob, curMach, machIndexOfJob);  // 是否需要切换
	
	int a = true * 3;
	bool isInserted(false);
	unsigned IndexOfTimeWin(0);  // machine的插入的索引

	ptime readyTimeForOrder;  // job可以在该机器上开始加工的时间
	time_period timeWinToInsert(curJob.m_startDateOfOrder, timeDuration);  //要插入的时间窗
	if (0 == machIndexOfJob)  // 如果是job的第一个machine
		readyTimeForOrder = curJob.m_startDateOfOrder;
	else
		readyTimeForOrder = (curJob.m_allocatedTimeWin.end()-1)->second.end();

	if (0 == curMach.m_allocatedTimeWin.size())  // 如果是machine上的第一个job
	{
		timeWinToInsert = time_period(readyTimeForOrder, timeDuration);
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
			if (left_TimeWin < readyTimeForOrder + timeDuration)  // 可加工时间和时间窗开始时间之间一定放不下
				continue;

			if (curMach.m_allocatedTimeWin.cbegin() == timeInfo_iter)  // 第一个time window
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
	

	curMach.m_allocatedTimeWin.insert(curMach.m_allocatedTimeWin.begin() + IndexOfTimeWin, 
		make_pair(make_pair(curJob.m_jobCode, curJob.m_proceMachs[machIndexOfJob].second), timeWinToInsert));
	return true;
};;

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

//  初始化产能
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


// 数据库读取工单信息并初始化
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
		jobsCodeVec.push_back(jobP->m_jobCode);
		jobsMap[jobP->m_jobCode] = jobP;
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
		//cout<<"宽度a="<< mystod(row[4]) <<"   厚度b="<< mystod(row[3]) <<"\n"<<endl;
		jobP->m_proceTargets.push_back(make_pair(make_pair(machCode, num_reentry), ProcessTargets(mystod(row[4]), mystod(row[3])) ));
		++i;
	}
	std::cout << "There are " << i << " notes in total." << std::endl;
};

// 数据库读取机器信息并初始化
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
		std::cout << "machdCode" << row[0] << std::endl;
		if (CodeOfBellFurn == row[0])  // 是钟罩炉
		{
			Mach_BellFurnace* machP = new Mach_BellFurnace();
			machP->m_machCode = row[0];
			machsCodeVec.push_back(machP->m_machCode);
			machsMap[machP->m_machCode] = machP;
		}
		else if(airFurnaceSet.find(string(row[0]))!= airFurnaceSet.end()) // 是气垫炉
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


// 打印最终排程结果
void printFinalRes(map<string, Job*> &jobsMap, map<string, Mach*> &machsMap)
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
				std::cout << "   Number of time line: " << i_timeline<< std::endl;
				for (auto& timeWinInfo : timeline)  // 遍历所有time_period
				{
					// using TimeWin = pair<map<Job*, unsigned>, time_period>;  // pair< map<Job*, 该job第几次重入>, job插入的时间窗>

					time_period timeWin = timeWinInfo.second;
					std::cout << "      "<<to_iso_extended_string(timeWin.begin())
						<< "--" << to_iso_extended_string(timeWin.last())<<std::endl;
					unsigned i_jobs(0);
					for (auto& jobs : timeWinInfo.first)  // 遍历所有job
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
					<< " "<< to_iso_extended_string(timeWin.begin())
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
void writeToCSV(map<string, Job*>& jobsMap, map<string, Mach*>& machsMap)
{
	ofstream opt;
	opt.open("ResultsOfMach_spt.csv", ios::out | ios::trunc);
	cout << "Writing to csv..." << endl;
	for (auto& machInfo : machsMap)  // 遍历所有machine
	{
		if (CodeOfBellFurn == machInfo.first){
			Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machInfo.second);
			opt << curMachP->m_machCode << endl;
			unsigned i_timeline(0);
			for (auto& timeline : curMachP->m_timeLines)
			{
				opt << "ParallelMachine="<< i_timeline << std::endl;
				for (auto& timeWinInfo : timeline)  // 遍历所有time_period
				{
					// using TimeWin = pair<map<Job*, unsigned>, time_period>;  // pair< map<Job*, 该job第几次重入>, job插入的时间窗>

					time_period timeWin = timeWinInfo.second;
					double beginT = timeDuration2Double(timeWin.begin() - curTime);
					double durationT = timeDuration2Double(timeWin.end() - timeWin.begin());

					opt <<","<< beginT <<","
						<< durationT << std::endl;
					unsigned i_job(1);
					
					for (auto& jobs : timeWinInfo.first)  // 遍历所有job
					{
						Job* jobP = jobs.first;
						unsigned reEntry = jobs.second;

						//获取当前宽度
						double curWidth(0);   //job的当前宽度
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
							if (jobP->m_proceTargets[i].second.m_targetWidth > 0.0) // 从当前，往前找目标宽度大于0的值 
							{
								curWidth = jobP->m_proceTargets[i].second.m_targetWidth;
								break;
							}
						}
						//cout<<" curWidth="<< curWidth <<endl;
						if (curWidth <= 0.0)   // 若目标宽度都≤0 
							curWidth = jobP->m_initialInfos.m_targetWidth;
						// END OF 获取当前宽度

						opt <<"," << "," << "," << "reentry=" << reEntry
							<<","<< jobP->m_jobCode
							<< i_job << "," 
							<< "当前宽度=" << curWidth << std::endl;
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
			for (auto& timeWinInfo : curMachP->m_allocatedTimeWin)  // 遍历所有time_period
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
	for (auto& jobInfo : jobsMap)  // 遍历所有job
	{
		Job* curJobP = jobInfo.second;
		opt << curJobP->m_jobCode <<","<<","<< to_iso_extended_string(curJobP->m_dueDateOfOrder)<< endl;
		unsigned i_mach(0);
		for (auto& timeInfo : curJobP->m_allocatedTimeWin)  // 遍历所有time_period
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
		unsigned machIndex(0);  // job的 machine
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // 遍历某个job的所有machine
		{
			
			std::cout << "         curMach: " << machCodeOfCurJob.first << endl;

			bool isSuccess;
			if (CodeOfBellFurn == machCodeOfCurJob.first)  // 是钟罩炉
			{
				Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMapTemp[machCodeOfCurJob.first]);
				isSuccess = insertJob(*curJobP, *curMachP, machIndex);
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


void main()
{
	vector<string> jobsCodeVec;     // vector<job code>
	vector<string> machsCodeVec;    // vector<machine code>
	map<string, Job*> jobsMap;      // map<jobCode, Job*>
	map<string, Mach*> machsMap;    // map<machCode, Mach*>
	//ptime curTime = from_iso_string("20200610T000000");  // 当前时间
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
	
	set<vector<pair<string, unsigned>>> processLineSet;
	map<string, unsigned> machFrequ;
	for (auto& jobInfo : jobsMap)  // 遍历所有job
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

	
	// 打印
	for (auto& jobInfo : jobsMap)  // 遍历所有job
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
		// 检查job的日期格式！！？？!!??
		curJobP->m_startDateOfOrder = curTime;
	}

	
	//myInitialization(jobsCodeVec, machsCodeVec, jobsMap, machsMap);
	vector<pair<Job*, ptime>> jobsWithDueDate;               // vector<pair<Job*, 截止时间>>
	vector<pair<Job*, time_duration>> jobsWithTotalProTime;  // vector<pair<Job*, 总加工时间>>
	vector<pair<Job*, time_duration>> jobsWithSlackTime;  // vector<pair<Job*, 松弛时间>>   松弛时间：job的剩余时间（当前时点距离交货期的时间）与该job所需的加工时间之差
	for (auto& jobInfo : jobsMap)  // 遍历所有job，processTime，获取processTime
	{
		string curJobCode = jobInfo.first;
		
		Job *curJobP = jobInfo.second;
		jobsWithDueDate.push_back(make_pair(curJobP, curJobP->m_dueDateOfOrder));
		time_duration sumOfProcessTime;
		unsigned machIndex(0);

		for (pair<string, unsigned>& machInfoOfCurJob : curJobP->m_proceMachs)   // 遍历某个job的所有machine
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
	//for (unsigned i = 2; i < 30; ++i)  // 一个一个地放入job
	//unsigned i(-1);
	//for (auto& jobInfo: jobsMap)  // 一个一个地放入job
	for (unsigned i = 1; i < jobsWithTotalProTime.size(); ++i)  // 一个一个地放入job
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
		for (unsigned j_insertPosition = 0; j_insertPosition <= jobOrder.size(); ++j_insertPosition) // 遍历所有插入位置
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
	for (unsigned i = 0; i < jobsWithSlackTime.size(); ++i)  // 一个一个地放入job
	//for (auto& jobInfo: jobsMap)  // 一个一个地放入job
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
		unsigned machIndex(0);  // job的 machine
		for (pair<string, unsigned>& machCodeOfCurJob : curJobP->m_proceMachs)   // 遍历某个job的所有machine
		{
			std::cout << "curMach: "<< machCodeOfCurJob.first << std::endl;
			if (CodeOfBellFurn == machCodeOfCurJob.first)  // 是钟罩炉
			{
				Mach_BellFurnace* curMachP = static_cast<Mach_BellFurnace*>(machsMap[machCodeOfCurJob.first]);
				bool isSuccess = insertJob(*curJobP, *curMachP, machIndex);
			}
			else if (airFurnaceSet.find(machCodeOfCurJob.first) != airFurnaceSet.end()) // 是气垫炉组中的机器
			{
				Mach_AirFurnace* curMachP = static_cast<Mach_AirFurnace*>(machsMap[machCodeOfCurJob.first]);
				bool isSuccess = insertJob(*curJobP, *curMachP, machIndex, jobsMap);
			}
			else  //其他
			{
				Mach* curMachP = machsMap[machCodeOfCurJob.first];
				bool isSuccess = insertJob(*curJobP, *curMachP, machIndex);
			}

			//std::cout << "curMach: " << curMach.m_machCode <<"\n"<< "isSuccess: " << isSuccess << std::endl;
			machIndex++;
		}
	}

	// 打印最终排程结果
	printFinalRes(jobsMap, machsMap);
	
	// 获取目标值
	pair<double, double> objectVals = getObjVals(jobsMap, machsMap);
	std::cout << "总延期时间(小时): " << objectVals.first<< "; 加工所有工件所需的时间长度(小时): " <<objectVals.second << std::endl;
	*/

}
















