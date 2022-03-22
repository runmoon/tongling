#pragma once
#include<EDCode.h>
extern set<string> airFurnaceSet;



// 由初始排产结果获取染色体编码：排产结果须存入jobOrderScheduled中；编码信息须存入encodeInfoOfGA中
void EDCode::initChromCodesByInitSedul(vector<pair<string, Job*>>& jobOrderScheduled, const map<string, pair<int, pair<int, int>>>& encodeInfoOfGA
	, const int totalLenOfChromCode, Chromosome* chromPInit, Chromosome* chromPInit2)
{
	vector<pair<pair<Job*, int>, ptime>> jobChromPre(totalLenOfChromCode);
	int j = 0;

	//cout << "chromPInit2->code" << chromPInit2->code.size() << endl;
	//cout << "jobChromPre->code" << jobChromPre.size() << endl;

	for (int i = 0; i < jobOrderScheduled.size(); ++i) {
		Job* jobP = jobOrderScheduled[i].second;
		const pair<int, pair<int, int>>& encodeInfo = encodeInfoOfGA.at(jobP->m_jobCode);
		int codeNum = encodeInfo.first;

		if (encodeInfo.second.first < 0) continue;
		//cout << "  jobPCode=" << jobP->m_jobCode << "  codeNum="<< codeNum << " "<< encodeInfo.second.first  <<"-" << encodeInfo.second.second << endl;
		//cout << "  jobP->m_allocatedTimeWin.szie()" << jobP->m_allocatedTimeWin.size() << endl;
		if (jobP->m_allocatedTimeWin.size() != (encodeInfo.second.second - encodeInfo.second.first + 1)) {
			cout << "error with job squence about GA!" << endl;
			char cn; cin >> cn;
		}
		for (int i_pro = encodeInfo.second.first; i_pro <= encodeInfo.second.second; ++i_pro) {
			jobChromPre[j] = make_pair(make_pair(jobP, codeNum), jobP->m_allocatedTimeWin[i_pro].second.begin());
			chromPInit2->code[j] = codeNum;
			++j;
		}
	}
	sort(jobChromPre.begin(), jobChromPre.end(), compareJobPPtime); sort(jobChromPre.begin(), jobChromPre.end(), compareJobPPtime);

	for (int i = 0; i < jobChromPre.size(); ++i) {
		chromPInit->code[i] = jobChromPre[i].first.second;
	}

	// 打印
	if (0) {
		cout << " jobChromPre.size()=" << jobChromPre.size() << endl;
		for (int i = 0; i < jobChromPre.size(); ++i) {
			cout << "  " << i << endl;
			cout << jobChromPre[i].first.first->m_jobCode << "    " << jobChromPre[i].first.second << "    "
				<< to_iso_extended_string(jobChromPre[i].second) << endl;
		}

		cout << " 染色体1：" << endl;
		for (auto iter = chromPInit->code.begin(); iter != chromPInit->code.end(); ++iter) {
			cout << *iter << ", ";
		}
		cout << endl;
		cout << " 染色体2：" << endl;
		for (auto iter = chromPInit2->code.begin(); iter != chromPInit2->code.end(); ++iter) {
			cout << *iter << ", ";
		}
		char c4; cin >> c4;
	}

	return;
};

// 由预先代码获取染色体编码
void EDCode::initChromCodesByPreCode(Chromosome* chromPInit, Chromosome* chromPInit2)
{
	/*
	// NEH--初始排序-总加工时间；目标-makespan
	chromPInit->code = { 3, 0, 22, 3, 43, 38, 21, 43, 79, 3, 79, 31, 91, 31, 47, 79, 44, 47, 79, 31, 44, 8, 15, 79, 84, 72, 79, 96, 91, 10, 91
		, 11, 91, 79, 80, 81, 78, 91, 35, 35, 50, 0, 91, 8, 94, 8, 90, 91, 8, 37, 59, 37, 91, 23, 52, 8, 12, 15, 73, 8, 15, 15, 24, 22, 15, 38
		, 15, 21, 72, 54, 84, 53, 51, 84, 80, 96, 84, 72, 19, 81, 2, 96, 78, 19, 43, 20, 80, 84, 72, 81, 43, 50, 78, 43, 69, 84, 0, 72, 96, 43
		, 74, 2, 69, 47, 94, 50, 43, 20, 74, 0, 72, 47, 96, 80, 59, 0, 94, 47, 16, 1, 47, 44, 96, 0, 81, 47, 44, 0, 44, 30, 78, 29, 44, 0, 39
		, 44, 59, 27, 50, 60, 10, 0, 82, 94, 50, 90, 4, 10, 11, 1, 35, 16, 83, 10, 23, 90, 50, 85, 90, 35, 11, 24, 28, 23, 10, 11, 35, 24, 22
		, 48, 35, 70, 38, 35, 22, 37, 10, 11, 23, 70, 87, 21, 38, 37, 21, 5, 23, 24, 37, 80, 11, 37, 23, 65, 37, 81, 22, 6, 80, 38, 78, 67
		, 88, 21, 94, 81, 80, 52, 93, 94, 52, 12, 57, 52, 68, 73, 12, 78, 81, 12, 94, 64, 73, 52, 68, 88, 19, 89, 94, 73, 12, 78, 19, 75
		, 90, 66, 73, 52, 87, 5, 90, 25, 19, 65, 6, 58, 2, 19, 14, 19, 90, 69, 2, 13, 20, 24, 97, 90, 55, 20, 74, 22, 69, 56, 74, 20, 69
		, 38, 9, 20, 12, 74, 26, 21, 20, 69, 71, 74, 99, 69, 73, 89, 64, 16, 17, 69, 40, 1, 16, 16, 24, 1, 92, 69, 1, 54, 34, 69, 41, 54
		, 53, 22, 25, 66, 70, 95, 54, 69, 53, 36, 70, 46, 53, 54, 7, 38, 45, 70, 53, 18, 51, 76, 21, 1, 51, 16, 42, 51, 77, 48, 1, 54
		, 48, 51, 48, 1, 53, 1, 88, 1, 68, 88, 51, 88, 68, 1, 67, 30, 68, 87, 5, 29, 30, 16, 30, 65, 39, 29, 5, 6, 39, 27, 29, 30, 75
		, 5, 60, 6, 30, 82, 29, 27, 6, 26, 60, 27, 4, 83, 29, 27, 85, 82, 4, 82, 27, 64, 71, 99, 82, 89, 83, 85, 83, 82, 87, 65, 87
		, 64, 85, 83, 28, 17, 40, 66, 89, 65, 28, 25, 83, 66, 85, 28, 64, 25, 89, 85, 66, 48, 48, 28, 28, 87, 65, 87, 1, 67, 92, 34
		, 68, 65, 67, 70, 64, 1, 89, 70, 66, 67, 70, 5, 41, 95, 5, 75, 67, 70, 36, 46, 75, 6, 67, 70, 75, 6, 68, 26, 70, 26, 64, 26
		, 75, 45, 7, 26, 71, 88, 99, 88, 71, 71, 99, 87, 65, 87, 76, 18, 17, 40, 17, 87, 17, 65, 67, 40, 93, 40, 57, 93, 87, 65, 67
		, 57, 87, 65, 67, 89, 57, 57, 65, 66, 67, 99, 88, 67, 88, 25, 88, 25, 25, 88, 42, 77, 88, 25, 68, 92, 25, 92, 34, 68, 64
		, 58, 92, 34, 58, 68, 64, 89, 92, 34, 58, 92, 41, 68, 64, 14, 58, 66, 89, 41, 95, 14, 13, 68, 64, 89, 66, 14, 97, 13, 14
		, 13, 66, 97, 41, 64, 97, 36, 66, 55, 13, 46, 36, 55, 89, 99, 36, 55, 46, 46, 97, 99, 7, 66, 99, 45, 7, 55, 45, 7, 99
		, 45, 18, 56, 40, 34, 36, 34, 18, 76, 56, 46, 36, 40, 56, 76, 45, 18, 46, 76, 9, 56, 45, 40, 9, 9, 40, 9, 41, 42, 42
		, 77, 71, 95, 77, 42, 71, 77, 71, 7, 71, 18, 17, 18, 76, 42, 76, 17, 77, 42, 77 };

	chromPInit2->code = { 3, 3, 3, 43, 43, 43, 43, 43, 43, 43, 79, 79, 79, 79, 79, 79, 79, 31, 31, 31, 91, 91, 91, 91, 91, 91, 91, 91, 47, 47
		, 47, 47, 47, 47, 47, 44, 44, 44, 44, 44, 44, 44, 8, 8, 8, 8, 8, 8, 15, 15, 15, 15, 15, 15, 84, 84, 84, 84, 84, 84, 72, 72, 72, 72
		, 72, 72, 96, 96, 96, 96, 96, 96, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 80, 80, 80, 80, 80, 80, 80, 81, 81, 81, 81, 81
		, 81, 81, 78, 78, 78, 78, 78, 78, 78, 35, 35, 35, 35, 35, 35, 35, 50, 50, 50, 50, 50, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 94, 94, 94
		, 94, 94, 94, 94, 94, 90, 90, 90, 90, 90, 90, 90, 90, 37, 37, 37, 37, 37, 37, 37, 59, 59, 59, 23, 23, 23, 23, 23, 23, 52, 52
		, 52, 52, 52, 52, 12, 12, 12, 12, 12, 12, 73, 73, 73, 73, 73, 73, 24, 24, 24, 24, 24, 24, 22, 22, 22, 22, 22, 22, 22, 38, 38
		, 38, 38, 38, 38, 38, 21, 21, 21, 21, 21, 21, 21, 54, 54, 54, 54, 54, 54, 53, 53, 53, 53, 53, 53, 51, 51, 51, 51, 51, 51, 19
		, 19, 19, 19, 19, 19, 19, 2, 2, 2, 2, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 69, 20, 20, 20, 20, 20, 20, 20, 74, 74, 74, 74
		, 74, 74, 16, 16, 16, 16, 16, 16, 16, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 30, 30, 30, 30, 30, 30, 29, 29, 29, 29, 29, 29
		, 39, 39, 39, 27, 27, 27, 27, 27, 27, 60, 60, 60, 82, 82, 82, 82, 82, 82, 4, 4, 4, 83, 83, 83, 83, 83, 83, 85, 85, 85, 85
		, 85, 85, 28, 28, 28, 28, 28, 28, 48, 48, 48, 48, 48, 48, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 87, 87, 87, 87, 87
		, 87, 87, 87, 87, 87, 87, 87, 5, 5, 5, 5, 5, 5, 5, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 6, 6, 6, 6, 6, 6, 6
		, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 93, 93, 93, 57, 57
		, 57, 57, 57, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 89
		, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89, 75, 75, 75, 75, 75, 75, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66
		, 25, 25, 25, 25, 25, 25, 25, 25, 25, 58, 58, 58, 58, 58, 14, 14, 14, 14, 14, 13, 13, 13, 13, 13, 97, 97, 97, 97
		, 97, 55, 55, 55, 55, 55, 56, 56, 56, 56, 56, 9, 9, 9, 9, 9, 26, 26, 26, 26, 26, 26, 71, 71, 71, 71, 71, 71, 71
		, 71, 71, 99, 99, 99, 99, 99, 99, 99, 99, 99, 17, 17, 17, 17, 17, 17, 17, 40, 40, 40, 40, 40, 40, 40, 40, 40
		, 92, 92, 92, 92, 92, 92, 92, 34, 34, 34, 34, 34, 34, 34, 41, 41, 41, 41, 41, 41, 95, 95, 95, 95, 36, 36, 36
		, 36, 36, 36, 36, 46, 46, 46, 46, 46, 46, 46, 7, 7, 7, 7, 7, 7, 45, 45, 45, 45, 45, 45, 45, 18, 18, 18, 18
		, 18, 18, 18, 76, 76, 76, 76, 76, 76, 76, 42, 42, 42, 42, 42, 42, 42, 77, 77, 77, 77, 77, 77, 77 };
	*/


	// NEH--初始排序-总加工时间；目标-总流水时间
	chromPInit->code = { 4, 0, 22, 4, 31, 38, 21, 39, 4, 31, 59, 39, 17, 17, 10, 59, 31, 16, 10, 16, 11, 80, 11, 81, 80, 39, 25, 81, 25, 99
		, 59, 3, 99, 3, 85, 24, 85, 91, 10, 10, 11, 40, 80, 81, 80, 5, 40, 3, 10, 11, 26, 81, 80, 6, 10, 11, 85, 90, 24, 85, 81, 78, 91, 24
		, 15, 11, 24, 85, 91, 90, 0, 91, 90, 1, 24, 85, 80, 66, 90, 87, 24, 80, 81, 88, 89, 91, 81, 79, 78, 94, 90, 91, 78, 0, 71, 79, 0
		, 67, 91, 90, 0, 79, 68, 91, 90, 78, 7, 5, 17, 26, 64, 79, 17, 1, 6, 16, 17, 65, 78, 16, 17, 70, 16, 78, 17, 16, 60, 25, 7, 64
		, 13, 25, 50, 87, 99, 66, 78, 16, 30, 99, 29, 99, 14, 23, 90, 99, 22, 25, 88, 40, 89, 56, 99, 38, 25, 25, 40, 21, 99, 40, 55
		, 27, 25, 58, 25, 12, 40, 73, 15, 99, 15, 40, 51, 15, 40, 97, 15, 82, 15, 40, 52, 83, 0, 96, 69, 94, 0, 60, 94, 54, 94, 0
		, 53, 13, 60, 79, 13, 50, 0, 94, 79, 48, 13, 30, 28, 13, 29, 8, 94, 26, 5, 57, 79, 14, 26, 94, 50, 55, 5, 6, 26, 30, 50
		, 29, 1, 26, 5, 71, 6, 1, 30, 50, 1, 7, 67, 14, 6, 94, 7, 1, 66, 70, 65, 55, 29, 9, 66, 87, 1, 84, 66, 7, 87, 50, 72
		, 66, 64, 1, 75, 42, 64, 66, 69, 88, 75, 74, 1, 88, 89, 92, 87, 88, 77, 1, 89, 14, 7, 93, 87, 35, 1, 68, 87, 76, 37
		, 14, 66, 89, 44, 34, 87, 66, 36, 2, 66, 88, 87, 19, 45, 88, 87, 66, 95, 20, 66, 64, 47, 87, 18, 41, 87, 88, 43, 89
		, 46, 88, 89, 64, 88, 64, 88, 88, 89, 23, 64, 89, 5, 42, 74, 22, 5, 71, 89, 64, 56, 77, 92, 6, 76, 35, 67, 56, 89
		, 64, 6, 65, 64, 70, 71, 64, 71, 67, 44, 37, 69, 67, 65, 71, 70, 75, 23, 71, 67, 75, 22, 69, 71, 67, 68, 34, 36
		, 1, 68, 75, 65, 67, 71, 1, 75, 65, 67, 67, 68, 68, 67, 68, 67, 65, 68, 68, 65, 68, 56, 65, 56, 38, 65, 70, 68
		, 21, 23, 68, 57, 65, 38, 21, 57, 70, 23, 65, 57, 70, 42, 2, 19, 9, 57, 42, 74, 22, 70, 23, 9, 93, 95, 45, 42
		, 74, 47, 20, 38, 22, 70, 42, 74, 42, 92, 74, 22, 70, 21, 38, 92, 77, 70, 77, 21, 69, 92, 38, 18, 41, 77, 92
		, 92, 35, 77, 93, 77, 76, 35, 21, 76, 35, 46, 43, 30, 76, 35, 35, 55, 37, 76, 30, 29, 76, 44, 37, 27, 55, 44
		, 37, 96, 58, 29, 44, 58, 27, 72, 58, 34, 96, 27, 34, 36, 58, 34, 36, 27, 96, 36, 72, 27, 96, 72, 37, 44, 37
		, 34, 44, 12, 36, 34, 96, 12, 73, 36, 12, 73, 51, 51, 12, 73, 51, 69, 52, 12, 73, 52, 51, 52, 2, 73, 2, 19
		, 69, 45, 19, 51, 45, 69, 19, 45, 19, 19, 69, 97, 45, 95, 45, 69, 97, 20, 97, 47, 20, 69, 97, 82, 47, 20
		, 47, 18, 82, 41, 18, 82, 95, 18, 41, 82, 41, 20, 52, 43, 47, 20, 72, 82, 18, 47, 43, 41, 18, 43, 72, 46
		, 52, 46, 43, 43, 46, 46, 83, 46, 83, 83, 83, 83, 54, 54, 53, 54, 53, 54, 53, 54, 53, 48, 53, 48, 48, 48
		, 48, 28, 8, 28, 28, 8, 8, 28, 28, 8, 9, 8, 9, 84, 84, 84, 84, 84 };

	chromPInit2->code = { 4, 4, 4, 31, 31, 31, 39, 39, 39, 59, 59, 59, 17, 17, 17, 17, 17, 17, 17, 10, 10, 10, 10, 10, 10, 16, 16, 16, 16
		, 16, 16, 16, 11, 11, 11, 11, 11, 11, 80, 80, 80, 80, 80, 80, 80, 81, 81, 81, 81, 81, 81, 81, 25, 25, 25, 25, 25, 25, 25, 25, 25
		, 99, 99, 99, 99, 99, 99, 99, 99, 99, 3, 3, 3, 85, 85, 85, 85, 85, 85, 24, 24, 24, 24, 24, 24, 91, 91, 91, 91, 91, 91, 91, 91
		, 40, 40, 40, 40, 40, 40, 40, 40, 40, 5, 5, 5, 5, 5, 5, 5, 26, 26, 26, 26, 26, 26, 6, 6, 6, 6, 6, 6, 6, 90, 90, 90, 90, 90
		, 90, 90, 90, 78, 78, 78, 78, 78, 78, 78, 15, 15, 15, 15, 15, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
		, 1, 1, 1, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 87, 88, 88, 88, 88
		, 88, 88, 88, 88, 88, 88, 88, 88, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89, 79, 79, 79, 79, 79, 79, 79, 94, 94, 94, 94
		, 94, 94, 94, 94, 71, 71, 71, 71, 71, 71, 71, 71, 71, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 67, 68, 68, 68, 68, 68
		, 68, 68, 68, 68, 68, 68, 68, 7, 7, 7, 7, 7, 7, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 65, 65, 65, 65, 65, 65
		, 65, 65, 65, 65, 65, 65, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 70, 60, 60, 60, 13, 13, 13, 13, 13, 50, 50, 50, 50
		, 50, 50, 30, 30, 30, 30, 30, 30, 29, 29, 29, 29, 29, 29, 14, 14, 14, 14, 14, 23, 23, 23, 23, 23, 23, 22, 22, 22, 22
		, 22, 22, 22, 56, 56, 56, 56, 56, 38, 38, 38, 38, 38, 38, 38, 21, 21, 21, 21, 21, 21, 21, 55, 55, 55, 55, 55, 27, 27
		, 27, 27, 27, 27, 58, 58, 58, 58, 58, 12, 12, 12, 12, 12, 12, 73, 73, 73, 73, 73, 73, 51, 51, 51, 51, 51, 51, 97, 97
		, 97, 97, 97, 82, 82, 82, 82, 82, 82, 52, 52, 52, 52, 52, 52, 83, 83, 83, 83, 83, 83, 96, 96, 96, 96, 96, 96, 69, 69
		, 69, 69, 69, 69, 69, 69, 69, 69, 69, 54, 54, 54, 54, 54, 54, 53, 53, 53, 53, 53, 53, 48, 48, 48, 48, 48, 48, 28, 28
		, 28, 28, 28, 28, 8, 8, 8, 8, 8, 8, 57, 57, 57, 57, 57, 9, 9, 9, 9, 9, 84, 84, 84, 84, 84, 84, 72, 72, 72, 72, 72
		, 72, 75, 75, 75, 75, 75, 75, 42, 42, 42, 42, 42, 42, 42, 74, 74, 74, 74, 74, 74, 92, 92, 92, 92, 92, 92, 92, 77
		, 77, 77, 77, 77, 77, 77, 93, 93, 93, 35, 35, 35, 35, 35, 35, 35, 76, 76, 76, 76, 76, 76, 76, 37, 37, 37, 37, 37
		, 37, 37, 44, 44, 44, 44, 44, 44, 44, 34, 34, 34, 34, 34, 34, 34, 36, 36, 36, 36, 36, 36, 36, 2, 2, 2, 2, 19, 19
		, 19, 19, 19, 19, 19, 45, 45, 45, 45, 45, 45, 45, 95, 95, 95, 95, 20, 20, 20, 20, 20, 20, 20, 47, 47, 47, 47, 47
		, 47, 47, 18, 18, 18, 18, 18, 18, 18, 41, 41, 41, 41, 41, 41, 43, 43, 43, 43, 43, 43, 43, 46, 46, 46, 46, 46, 46, 46 };


	// NEH做初始解（初始排序-总加工时间；目标-makespan），GA得到的最优解
	chromPInit->code = { 3, 3, 3, 43, 43, 43, 43, 43, 43, 43, 79, 79, 7, 79, 79, 79, 79, 31, 31, 31, 91, 91, 91, 91, 91, 44, 47, 47, 47
		, 1, 47, 47, 47, 15, 15, 15, 15, 8, 70, 8, 8, 8, 8, 9, 44, 44, 44, 44, 44, 91, 91, 91, 15, 15, 84, 84, 84, 84, 84, 84, 72, 77
		, 72, 71, 72, 72, 72, 72, 96, 96, 92, 96, 96, 96, 1, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 80, 80, 78, 78, 95, 78
		, 81, 81, 81, 81, 81, 81, 81, 80, 80, 80, 80, 80, 78, 78, 78, 78, 35, 35, 35, 35, 35, 35, 35, 50, 50, 50, 50, 5, 50, 0, 0
		, 0, 0, 0, 0, 0, 52, 71, 0, 94, 92, 17, 94, 94, 94, 94, 94, 90, 90, 90, 90, 90, 90, 90, 90, 37, 37, 37, 37, 37, 37, 37
		, 59, 59, 59, 23, 23, 5, 23, 20, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 88, 93, 93, 93, 48, 57, 57, 57, 73, 24
		, 24, 24, 24, 24, 24, 22, 22, 22, 22, 22, 22, 22, 38, 38, 38, 38, 38, 38, 38, 21, 21, 21, 21, 21, 21, 21, 54, 54
		, 54, 54, 54, 54, 53, 53, 53, 53, 53, 53, 51, 51, 51, 51, 51, 51, 19, 19, 19, 19, 19, 19, 19, 2, 2, 2, 2, 69, 69
		, 69, 69, 69, 69, 69, 69, 69, 69, 69, 20, 23, 20, 20, 20, 20, 20, 74, 74, 74, 1, 1, 71, 1, 1, 1, 1, 1, 1, 1, 1
		, 47, 96, 30, 30, 30, 30, 30, 30, 29, 29, 29, 29, 29, 29, 39, 39, 39, 27, 92, 27, 27, 71, 27, 27, 27, 60, 60
		, 60, 82, 82, 82, 82, 82, 82, 4, 4, 4, 83, 83, 83, 83, 83, 83, 85, 85, 28, 28, 28, 28, 28, 28, 67, 67, 67
		, 67, 67, 67, 67, 70, 70, 70, 8, 70, 70, 70, 87, 87, 87, 87, 87, 87, 87, 87, 13, 87, 87, 87, 5, 5, 6, 6
		, 6, 5, 23, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 6, 6, 5, 5, 50, 6, 67, 70, 48, 70, 48, 70
		, 70, 48, 67, 67, 67, 67, 23, 0, 52, 52, 52, 48, 52, 52, 12, 12, 12, 12, 12, 12, 73, 73, 73, 73, 73
		, 57, 57, 48, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 68, 64, 64, 64, 64, 64, 64, 64, 64, 64
		, 92, 64, 64, 64, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89, 89, 75, 75, 75, 75, 75, 75, 66, 66, 14
		, 66, 66, 71, 66, 66, 66, 66, 66, 66, 6, 66, 25, 25, 25, 25, 25, 25, 25, 25, 25, 58, 58, 58, 58
		, 71, 58, 14, 14, 14, 14, 66, 13, 13, 13, 13, 87, 97, 97, 97, 97, 97, 55, 55, 55, 55, 55, 56
		, 56, 56, 56, 56, 9, 9, 9, 9, 44, 26, 26, 26, 92, 26, 92, 26, 26, 99, 99, 99, 99, 99, 99, 99
		, 92, 99, 99, 17, 94, 94, 17, 17, 17, 17, 17, 40, 40, 40, 40, 40, 40, 40, 40, 40, 34, 34, 34
		, 34, 34, 34, 34, 41, 41, 41, 74, 74, 74, 16, 16, 16, 16, 16, 16, 16, 1, 41, 41, 41, 95, 95
		, 95, 36, 36, 36, 36, 36, 36, 36, 46, 46, 46, 46, 46, 46, 46, 7, 7, 7, 7, 7, 79, 45, 45, 45
		, 85, 85, 85, 85, 45, 45, 45, 45, 18, 18, 18, 18, 18, 18, 71, 18, 76, 76, 76, 76, 76, 76
		, 71, 76, 42, 42, 42, 71, 42, 42, 42, 42, 77, 77, 77, 77, 77, 77 };


	/*
	// NEH做初始解（初始排序-总加工时间；目标-总流水时间），GA得到的最优解
	chromPInit->code = { 4, 0, 22, 4, 31, 21, 41, 10, 88, 0, 17, 17, 16, 10, 16, 11, 31, 11, 81, 80, 39, 81, 99, 25, 59, 25, 3, 99, 3, 85
		, 26, 81, 80, 85, 90, 10, 10, 91, 24, 11, 10, 40, 3, 80, 10, 11, 40, 19, 5, 80, 81, 6, 11, 85, 24, 81, 85, 91, 78, 24, 11, 15, 24
		, 91, 90, 85, 91, 90, 0, 1, 24, 85, 66, 90, 80, 2, 87, 81, 24, 80, 91, 81, 88, 89, 94, 90, 91, 79, 78, 71, 78, 0, 83, 67, 83, 91
		, 90, 79, 0, 68, 91, 90, 17, 0, 5, 7, 17, 26, 64, 79, 78, 79, 17, 16, 65, 1, 17, 16, 70, 16, 17, 6, 16, 78, 25, 7, 78, 64, 25
		, 13, 60, 99, 66, 50, 16, 30, 99, 87, 99, 14, 23, 90, 99, 22, 78, 25, 29, 40, 88, 99, 89, 56, 25, 25, 40, 99, 21, 40, 38, 55
		, 25, 7, 25, 27, 40, 66, 58, 68, 67, 65, 68, 68, 1, 99, 66, 40, 2, 15, 40, 97, 15, 82, 51, 40, 15, 52, 15, 83, 15, 0, 94, 0
		, 94, 96, 65, 60, 53, 13, 60, 79, 13, 50, 0, 79, 94, 48, 13, 30, 28, 13, 29, 94, 8, 26, 5, 57, 79, 14, 94, 26, 50, 55, 5, 6
		, 30, 50, 29, 1, 26, 5, 71, 6, 4, 1, 31, 30, 87, 59, 59, 38, 89, 88, 46, 1, 68, 64, 89, 64, 88, 65, 88, 55, 18, 39, 14, 1
		, 67, 87, 84, 7, 94, 12, 66, 64, 87, 50, 42, 64, 66, 72, 39, 1, 75, 69, 88, 75, 92, 74, 1, 77, 88, 89, 14, 87, 88, 7, 35
		, 1, 76, 37, 14, 66, 93, 44, 34, 89, 73, 36, 68, 87, 88, 66, 87, 95, 1, 19, 87, 87, 89, 66, 20, 66, 64, 45, 88, 87, 47
		, 87, 7, 94, 66, 6, 66, 70, 1, 88, 23, 89, 89, 5, 42, 22, 5, 71, 74, 64, 89, 77, 92, 56, 76, 35, 6, 56, 64, 89, 65, 22
		, 6, 71, 23, 67, 75, 70, 67, 68, 67, 65, 71, 37, 44, 71, 64, 67, 71, 70, 64, 69, 71, 69, 34, 67, 1, 71, 75, 75, 54, 75
		, 67, 64, 65, 65, 68, 68, 67, 67, 68, 56, 67, 65, 56, 65, 70, 68, 23, 68, 57, 65, 38, 42, 57, 9, 38, 19, 21, 2, 42, 70
		, 57, 65, 23, 95, 70, 57, 74, 22, 70, 23, 9, 74, 20, 74, 22, 70, 42, 38, 42, 92, 47, 22, 70, 21, 42, 38, 69, 21, 38
		, 18, 92, 77, 70, 77, 45, 92, 93, 69, 41, 77, 21, 92, 92, 35, 77, 45, 43, 20, 77, 76, 35, 21, 76, 35, 46, 55, 30
		, 76, 36, 35, 35, 29, 37, 76, 30, 27, 76, 82, 97, 44, 37, 82, 97, 20, 55, 47, 47, 18, 44, 37, 96, 58, 44, 82, 29
		, 58, 27, 72, 34, 58, 96, 34, 36, 27, 34, 36, 2, 74, 36, 27, 96, 72, 27, 37, 44, 37, 34, 44, 96, 36, 34, 72, 12
		, 36, 96, 73, 12, 69, 51, 51, 12, 73, 51, 69, 58, 12, 19, 73, 52, 51, 12, 73, 19, 52, 52, 95, 45, 19, 19, 73
		, 80, 93, 51, 45, 69, 69, 45, 45, 20, 69, 47, 69, 97, 26, 97, 95, 41, 18, 82, 18, 52, 41, 82, 41, 20, 43, 47
		, 20, 72, 18, 47, 43, 41, 18, 54, 54, 83, 83, 46, 46, 43, 46, 46, 83, 43, 52, 46, 43, 9, 29, 50, 72, 43, 53
		, 54, 53, 54, 53, 54, 53, 48, 53, 48, 48, 48, 48, 28, 8, 28, 28, 8, 8, 28, 28, 8, 9, 8, 9, 84, 84, 84, 84, 84 };
	*/


	return;
};




// GA解码用，可以获取job的拖期时长————获取目标函数值 <总延期时间(小时), 加工所有工件所需的时间长度(小时)>
pair<double, double> EDCode::getObjValsForChrom(vector<pair<string, Job*>>& jobOrder, map<string, Mach*>& machsMap, Chromosome* chromP)  // get object value
{
	chromP->delayJobs.clear();
	double totalDueTime(0.0);   // 总延期时间（小时）
	ptime timeOfStart;          // 开始排程的时间点
	ptime timeOfCompletion;     // 所有工件加工完的时间点
	double makeSpan(0.0);       // 加工所有工件所需的时间长度（小时）
	int i_numOfJob(0);

	double totalRuntime(0.0);   // 总流水时间

	for (auto& jobInfo : jobOrder)  // 遍历所有job
	{
		Job* curJobP = jobInfo.second;
		if (0 == curJobP->m_allocatedTimeWin.size())
			continue;
		const time_period& lastTimeWin = (curJobP->m_allocatedTimeWin.end() - 1)->second;
		ptime dueDate = curJobP->m_dueDateOfOrder;
		double delayTime = timeDuration2Double(lastTimeWin.last() - dueDate);
		delayTime = delayTime >= 0 ? delayTime : 0;
		//cout<<"delayTime="<< delayTime <<endl;
		totalDueTime += delayTime;

		//计算流水时间
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
		chromP->tardinessOfjobs[i_numOfJob] = delayTime;
		if (delayTime > 0.0) chromP->delayJobs.emplace_back(i_numOfJob);
		i_numOfJob++;
		//std::cout << "delay time of "<<curJobP->m_jobCode << " is: " << delayTime<<std::endl;
	}

	//cout<< "timeOfCompletion=" <<to_iso_extended_string(timeOfCompletion)<<endl;
	//cout<< "timeOfStart=" << to_iso_extended_string(timeOfStart) << endl;


	makeSpan = timeDuration2Double(timeOfCompletion - timeOfStart);
	//std::cout << "totalDueTime=　" << totalDueTime << std::endl;
	//std::cout << "makeSpan = " << makeSpan << std::endl;
	//std::cout << std::endl;

	//return make_pair(totalDueTime, makeSpan);
	return make_pair(makeSpan, totalRuntime);
};

// 获取染色体的目标函数值
pair<double, double> EDCode::getObjectValuesOfChromo(Chromosome* chromP, bool isPrint)
{
	const int totalLenOfChromCode = this->m_totalLenOfChromCode;
	const int lenOfChromCode = chromP->code.size();

	vector<pair<string, Job*>>& jobOrder = this->m_jobOrderTmp2;
	map<string, Job*>& jobsMapTemp = this->m_jobsMapTmp2;
	map<string, Mach*>& machsMapTemp = this->m_machsMapTmp2;
	// 用来排气垫炉之前的工序
	vector<pair<string, Job*>>* jobOrderBefP = this->m_jobOrderBefP;
	map<string, Job*>* jobsMapBefP = this->m_jobsMapBefP;
	map<string, Mach*>* machsMapBefP = this->m_machsMapBefP;

	resetJobsTemp(jobOrder, *jobOrderBefP);
	resetMachsMapTemp(machsMapTemp, *machsMapBefP);

	// 排入气垫炉中间的工序
	for (int i_pro = 0; i_pro < lenOfChromCode; ++i_pro) {

		int jobIndex = chromP->code[i_pro];
		Job* curJobP = jobOrder[jobIndex].second;
		int machIndex = curJobP->m_curMachIndex;

		pair<string, unsigned>& machCodeOfCurJob = curJobP->m_proceMachs[machIndex];

		bool isSuccess = false;
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
	}

	// 排入剩余工序
	Job* curJobP;
	for (auto& jobInfo : jobOrder)
	{
		curJobP = jobInfo.second;
		for (int machIndex = curJobP->m_curMachIndex; machIndex < curJobP->m_proceMachs.size(); ++machIndex)   // 遍历某个job的剩余工序
		{
			pair<string, unsigned>& machCodeOfCurJob = curJobP->m_proceMachs[machIndex];
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
			if (isSuccess) curJobP->m_curMachIndex = machIndex + 1;
		}
	}

	// 获取目标值
	pair<double, double> objVals = getObjValsForChrom(jobOrder, machsMapTemp, chromP);
	chromP->objectValues = objVals;

	// 打印最终排程结果
	if (isPrint) {
		cout << "testPrint!!!" << endl;  printFinalRes(jobsMapTemp, machsMapTemp);
	}
	return objVals;
};


// 获取染色体的目标函数值
pair<double, double> EDCode::getObjectValuesOfChromo_Parallel(Chromosome* chromP, threadInfoOfLS* threadInfoP, bool isPrint)
{
	const int lenOfChromCode = chromP->code.size();

	/*
	vector<pair<string, Job*>> jobOrder;  // 引用变拷贝
	map<string, Job*> jobsMapTemp;        // 引用变拷贝
	map<string, Mach*> machsMapTemp;      // 引用变拷贝
	// 须初始化job和mach对象
	initJobsTemp(jobsMapTemp, jobOrder, gaP->m_jobOrderTmp2);  // 拷贝Job--把jobOrder中的所有job拷贝一下，放入到jobsMapTemp中
	initMachsMapTemp(machsMapTemp, gaP->m_machsMapTmp2);       // 拷贝machine--把machsMap中的所有machine拷贝一下，放入到machsMapTemp中

	// 为job和mach预先分配时间窗空间
	allocateTimeWinSpace(jobsMapTemp, machsMapTemp);
	*/

	vector<pair<string, Job*>>& jobOrder = threadInfoP->m_jobOrderTmp;
	map<string, Job*>& jobsMapTemp = threadInfoP->m_jobsMapTmp;
	map<string, Mach*>& machsMapTemp = threadInfoP->m_machsMapTmp;

	/*
	// 用来排气垫炉之前的工序
	vector<pair<string, Job*>>* jobOrderBefP = gaP->m_jobOrderBefP;
	map<string, Job*>* jobsMapBefP = gaP->m_jobsMapBefP;
	map<string, Mach*>* machsMapBefP = gaP->m_machsMapBefP;
	resetJobsTemp(jobOrder, *jobOrderBefP);
	resetMachsMapTemp(machsMapTemp, *machsMapBefP);
	*/

	resetJobsToOrigin(jobOrder);  	// 无预先排产时--把jobOrder中的job的排产时间窗清0
	resetMachsMapToOrigin(machsMapTemp);  	// 无预先排产时--把machsMap中的排产时间窗清0

	// 排入气垫炉中间的工序
	for (int i_pro = 0; i_pro < lenOfChromCode; ++i_pro) {
		int jobIndex = chromP->code[i_pro];
		Job* curJobP = jobOrder[jobIndex].second;
		int machIndex = curJobP->m_curMachIndex;

		pair<string, unsigned>& machCodeOfCurJob = curJobP->m_proceMachs[machIndex];

		bool isSuccess = false;
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
	}

	/*
	// 排入剩余工序
	Job* curJobP;
	for (auto& jobInfo : jobOrder)
	{
		curJobP = jobInfo.second;
		for (int machIndex = curJobP->m_curMachIndex; machIndex < curJobP->m_proceMachs.size(); ++machIndex)   // 遍历某个job的剩余工序
		{
			pair<string, unsigned>& machCodeOfCurJob = curJobP->m_proceMachs[machIndex];
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
			if (isSuccess) curJobP->m_curMachIndex = machIndex + 1;
		}
	}
	*/

	// 获取目标值
	pair<double, double> objVals = EDCode::getObjValsForChrom(jobOrder, machsMapTemp, chromP);
	chromP->objectValues = objVals;


	// 释放内存，释放new出的job和mach-多线程用到
	//releaseMemoryOfTmpJobsMachs(jobOrder, machsMapTemp);


	// 打印最终排程结果
	if (isPrint) {
		cout << "testPrint!!!" << endl;  printFinalRes(jobsMapTemp, machsMapTemp);
	}
	return objVals;
};




