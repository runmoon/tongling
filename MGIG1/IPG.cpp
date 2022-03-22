




#pragma once
#include<IPG.h>


extern std::mutex g_mutex;



void IPG::run(const int maxRound) {

	clock_t startTime, endTime;
	startTime = clock();

	for (int i_round = 0; i_round < maxRound; ++i_round) {

		cout << endl; cout << endl;
		cout << "--------------------------------------" << endl;
		cout << endl;
		cout << "i_round = " << i_round << endl;
		cout << endl;


		popPool.sort(cmpByObjSecond());
		popPool.sort(cmpByObjFirst());

		cout << "pareto set:" << endl;
		int j = 0;
		for (auto iter = popPool.begin(); iter != popPool.end(); ++iter) {
			Chromosome* chromP = *iter;
			cout << "  chrom " << j++ << ": obj1 = " << chromP->objectValues.first << "  obj2 = " << chromP->objectValues.second << endl;
		}
		cout << "end" << endl;




		if (i_round % 20) {
			Sleep(4000);
		}
		//char c; cin >> c;



		Chromosome* chosenChromo = this->selection(popPool);
		Chromosome* chosenChrom = new Chromosome(chosenChromo);
		//chosenChrom->code = vector<int>{ 0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,5,5,5,6,6,6,7,7,7,8 };
		//chosenChrom->codeLen = 25;

		vector<int> genesChrosen;
		this->destruction(chosenChrom, genesChrosen);

		cout << "codes:" << endl;
		for (int i = 0; i < chosenChrom->code.size(); ++i) {
			cout << " " << chosenChrom->code[i];
		}
		cout << endl;

		cout << "  iter2:" << chosenChrom->objectValues.first << " " << chosenChrom->objectValues.second << endl;


		//const unsigned int num_thread = thread::hardware_concurrency();  // 获取硬件所支持的线程个数
		const int num_thread = 5;  // 这里最高设置为6；设置为5时，CPU最高占用85%
		vector<threadInfoOfLS*> threadInfos = initThreadsInfoOfLS(num_thread, this->edCodeP->m_jobOrderBefP, this->edCodeP->m_machsMapBefP);


		list<Chromosome*> partialSoluSet;
		//this->construction(partialSoluSet, chosenChrom, genesChrosen);
		this->construction_Parallel(partialSoluSet, chosenChrom, genesChrosen, threadInfos);


		this->update(popPool, partialSoluSet);




		j = 0;
		for (auto iter = popPool.begin(); iter != popPool.end(); ++iter) {
			Chromosome* chromP = *iter;
			cout << "chrom " << j++ << ": obj1 = " << chromP->objectValues.first << "  obj2 = " << chromP->objectValues.second << endl;
		}
		cout << "end.." << endl;

	}

	endTime = clock();
	double t_sec = (double)(endTime - startTime) / CLOCKS_PER_SEC;

	cout << "The run time is: " << t_sec << "s  " << t_sec / 60 << "min" << endl;


};


void IPG::randomlyCreateInitialSolution_test(list<Chromosome*>& popPool) {

	for (int i = 0; i < 50; ++i) {
		Chromosome* chromP = new Chromosome(25, 8);
		popPool.emplace_back(chromP);

		double obj1 = 0;
		double obj2 = 0;
		while (obj1 + obj2 < 15) {
			obj1 = (5.0 * rand()) / (RAND_MAX + 1.0) + (int)(chromP->code.size());
			obj2 = (5.0 * rand()) / (RAND_MAX + 1.0) + (int)(chromP->code.size());
			//obj1 = rand() % 40; 
			//obj2 = rand() % 40;
		}
		chromP->objectValues = make_pair(obj1, obj2);
	}

	popPool.sort(cmpByObjSecond());
	popPool.sort(cmpByObjFirst());

	/*
	int i = 0;
	for (auto iter = popPool.begin(); iter != popPool.end(); ++iter) {
		Chromosome* chromP = *iter;
		cout << "chrom " << i++ << ": obj1 = " << chromP->objectValues.first << "  obj2 = " << chromP->objectValues.second << endl;
	}
	*/

	cout << " " << endl;
	getParetoSet(popPool);
	cout << " " << endl;

	/*
	int j = 0;
	for (auto iter = popPool.begin(); iter != popPool.end(); ++iter) {
		Chromosome* chromP = *iter;
		cout <<"chrom "<< j++ << ": obj1 = " << chromP->objectValues.first << "  obj2 = " << chromP->objectValues.second << endl;
	}
	cout << "end.." << endl;
	char c; cin >> c;
	*/

	cout << "pareto set:" << endl;
	int j = 0;
	for (auto iter = popPool.begin(); iter != popPool.end(); ++iter) {
		Chromosome* chromP = *iter;
		cout << "  chrom " << j++ << ": obj1 = " << chromP->objectValues.first << "  obj2 = " << chromP->objectValues.second << endl;
	}
	cout << "end" << endl;
};


void IPG::randomlyCreateInitialSolution(const int numOfPop, Chromosome* chromPInit, Chromosome* chromPInit2) {

	const int totalLenOfChromCode = this->m_totalLenOfChromCode;
	const int numOfJobRange = this->m_numOfJobRange;

	list<Chromosome*>& popPool = this->popPool;

	//popPool[0] = chromPInit;
	//popPool[1] = chromPInit2;

	// 排产结果1构成初始解
	for (int i = 0; i < 0; ++i) {
		Chromosome* chromPInitnew = new Chromosome(totalLenOfChromCode, numOfJobRange);
		popPool.emplace_back(chromPInitnew);
		chromPInitnew->code.assign(chromPInit->code.begin(), chromPInit->code.end());
	}

	// 排产结果2构成初始解
	for (int i = 1; i < 0; ++i) {
		Chromosome* chromPInitnew = new Chromosome(totalLenOfChromCode, numOfJobRange);
		popPool.emplace_back(chromPInitnew);
		chromPInitnew->code.assign(chromPInit2->code.begin(), chromPInit2->code.end());
	}

	// 随机生成初始解
	for (int i_chrom = 0; i_chrom < 70; ++i_chrom) {
		Chromosome* chromPInitnew = new Chromosome(totalLenOfChromCode, numOfJobRange);
		popPool.emplace_back(chromPInitnew);
		for (int i = 0; i < totalLenOfChromCode; ++i) {
			int indexOfUndefined = rand() % (totalLenOfChromCode - i) + 1;
			int index = 0;
			int j = 0;
			for (; index < indexOfUndefined; ++index, ++j) {
				if (chromPInitnew->code[j] >= 0) --index;
			}
			chromPInitnew->code[--j] = chromPInit->code[i];
		}
	}

	// 由初始排产结果，随机交换生成初始解
	double rate = 0.1;  // 交换比例的一般
	int numOfMove = totalLenOfChromCode * rate;
	for (int i_chrom = 70; i_chrom < numOfPop; ++i_chrom) {
		Chromosome* chromPInitnew = new Chromosome(totalLenOfChromCode, numOfJobRange);
		popPool.emplace_back(chromPInitnew);
		chromPInitnew->code.assign(chromPInit->code.begin(), chromPInit->code.end());
		for (int i = 0; i < numOfMove; ++i) {
			int left = totalLenOfChromCode * (1.0 * rand()) / (RAND_MAX + 1.0);
			int right = totalLenOfChromCode * (1.0 * rand()) / (RAND_MAX + 1.0);
			swap(chromPInitnew->code[left], chromPInitnew->code[right]);
		}
	}

	this->initObjValsOfChromos();

	return;
};


void IPG::initObjValsOfChromos() {
	list<Chromosome*>& popPool = this->popPool;
	for (auto& chromP : popPool) {
		this->edCodeP->getObjectValuesOfChromo(chromP, false);
	}
};


Chromosome* IPG::selection(list<Chromosome*>& paretoSoluSet) {
	Chromosome* chosenChrom;
	cout << "paretoSoluSet.size()=" << paretoSoluSet.size() << endl;

	// 如果paretoSoluSet中解的个数小于6个，就随机选
	if (paretoSoluSet.size() < 6) {
		auto iter = paretoSoluSet.begin();
		int randNum = rand() % paretoSoluSet.size();
		std::advance(iter, randNum);
		chosenChrom = *iter;
		return chosenChrom;
	}

	// 找极端解（某个目标最小的解），放在paretoSoluSet的最后面
	list<Chromosome*>::iterator iterOfminObj = paretoSoluSet.begin();
	list<Chromosome*>::iterator iterOfminObj2 = paretoSoluSet.begin();

	auto iter = paretoSoluSet.begin();
	++iter;
	for (; iter != paretoSoluSet.end(); ++iter) {
		if ((*iter)->objectValues.first < (*iterOfminObj)->objectValues.first)
			iterOfminObj = iter;
		if ((*iter)->objectValues.second < (*iterOfminObj2)->objectValues.second)
			iterOfminObj2 = iter;
	}
	/*
	cout << "  ui=" << endl;
	cout << "  iter1:" << (*iterOfminObj)->objectValues.first << " " << (*iterOfminObj)->objectValues.second << endl;
	cout << "  iter2:" << (*iterOfminObj2)->objectValues.first << " " << (*iterOfminObj2)->objectValues.second << endl;
	*/

	iter = paretoSoluSet.end();
	--iter;

	if (iterOfminObj == iter) {
		--iter;
		swap(*iterOfminObj2, *iter);
	}
	else if (iterOfminObj2 == iter) {
		--iter;
		swap(*iterOfminObj, *iter);
	}
	else {
		swap(*iterOfminObj, *iter);    // 极端解放在paretoSoluSet的最后面
		--iter;
		swap(*iterOfminObj2, *iter);
	}

	/*
	int j = 0;
	for (auto iter = paretoSoluSet.begin(); iter != paretoSoluSet.end(); ++iter) {
		Chromosome* chromP = *iter;
		cout << "  chrom " << j++ << ": obj1 = " << chromP->objectValues.first << "  obj2 = " << chromP->objectValues.second << endl;
	}
	cout << "end.." << endl;
	*/

	// 0.2的概率选极端解，0.8的概率选其他解
	iter = paretoSoluSet.end();
	--iter;
	long numOfChroms = paretoSoluSet.size();
	double num = (1.0 * rand()) / (RAND_MAX + 1.0);  // [0, 1)
	cout << "num=" << num << endl;
	if (num < 0.1)               // 0.1的概率
		chosenChrom = (*iter);
	else if (num < 0.2) {        // 0.1的概率
		--iter;
		chosenChrom = *(iter);
	}
	else {                       // 0.8的概率
		long index = (long)(numOfChroms - 2.0) * (1.0 * rand()) / (RAND_MAX + 1.0);
		cout << "    index=" << index << endl;
		list<Chromosome*>::iterator iter = paretoSoluSet.begin();
		std::advance(iter, index);
		chosenChrom = *iter;
	}


	return chosenChrom;
};


void IPG::destruction(Chromosome* chromChosen, vector<int>& genesChrosen) {
	//const int nummin = 10;
	//const int nummax = 15;

	int num = (int)(nummax - nummin) * (1.0 * rand()) / (RAND_MAX + 1.0) + nummin;  // [0, 1)
	cout << "  num Of gene: " << num << endl;

	genesChrosen.reserve(num);
	set<int> genesSet;

	for (int i = 0; i < num; i++) {
		while (genesChrosen.size() <= i) {
			const size_t& codeLen = chromChosen->code.size();
			int index = (int)(codeLen * (1.0 * rand()) / (RAND_MAX + 1.0));  // [0, 1)
			int gene = chromChosen->code[index];
			if (genesSet.insert(gene).second) {  // 插入成功
				genesChrosen.emplace_back(gene);

				chromChosen->code.erase(chromChosen->code.begin() + index);
				break;
			}
		}
	}

	cout << "  chosen genes:";
	for (int i = 0; i < num; i++) {
		cout << " " << i << ":gene=" << genesChrosen[i] << "  ";
	}
	cout << endl;

	return;
};


void IPG::construction(list<Chromosome*>& partialSoluSet, Chromosome* chromChosen, vector<int>& genesChrosen) {
	int codeLen = chromChosen->codeLen;
	int num = (int)genesChrosen.size();
	cout << "    num=" << num << endl;

	int subLen = codeLen - num;

	cout << "    subLen=" << subLen << endl;


	list<Chromosome*> partialSoluSetNew;
	partialSoluSet.emplace_back(chromChosen);

	for (int i = 0; i < num; i++) {
		int gene = genesChrosen[i];
		cout << "  i=" << i << "  gene:" << gene << endl;
		int c = 0;
		// 对每一个染色体都插入
		for (auto& curChrom : partialSoluSet)
		{
			cout << "   chrom " << c++ << endl;
			// 对某染色体的所有可插入位置，都插入
			for (int pos = 0; pos < subLen; ++pos) {
				Chromosome* chromNew = new Chromosome(curChrom);
				chromNew->code.insert(chromNew->code.begin() + pos, gene);

				// 计算目标值，决定能否插入
				//getObjVals_test(chromNew);
				this->edCodeP->getObjectValuesOfChromo(chromNew, false);

				bool result = insertToParetoSet(partialSoluSetNew, chromNew);
				cout << "    pos=" << pos << "  chrom " << pos
					<< ": obj1 = " << chromNew->objectValues.first << "  obj2 = " << chromNew->objectValues.second
					<< "    result=" << result << endl;

				if (result == false)
					delete chromNew;

				int indexJump = rand() % 20;
				pos += indexJump;
			}
			cout << endl;
		}
		// 保留非支配解
		partialSoluSet.swap(partialSoluSetNew);
		for (auto curChrom : partialSoluSetNew)
			delete curChrom;
		partialSoluSetNew.clear();

		partialSoluSet.sort(cmpByObjFirst());
		int j = 0;
		cout << "    i=" << i << " objs:" << endl;
		for (auto iter = partialSoluSet.begin(); iter != partialSoluSet.end(); ++iter) {
			Chromosome* chromP = *iter;
			cout << "      chrom " << j++ << ": obj1 = " << chromP->objectValues.first << "  obj2 = " << chromP->objectValues.second << endl;
		}
		cout << endl;
	}
	// 保留非支配解
	//getParetoSet(partialSoluSet);

	return;
};


void IPG::construction_Parallel(list<Chromosome*>& partialSoluSet, Chromosome* chromChosen, vector<int>& genesChrosen, vector<threadInfoOfLS*>& threadInfos) {
	int codeLen = chromChosen->codeLen;
	int num = (int)genesChrosen.size();
	cout << "    num=" << num << endl;

	int subLen = codeLen - num;
	cout << "    subLen=" << subLen << endl;


	list<Chromosome*> partialSoluSetNew;
	partialSoluSet.emplace_back(chromChosen);


	for (int i = 0; i < num; i++) {
		cout << " num of Chroms in partialSoluSet: " << partialSoluSet.size() << endl;

		int gene = genesChrosen[i];
		cout << "  i=" << i << "  gene:" << gene << endl;
		int c = 0;
		// 对每一个染色体都插入
		for (auto& curChrom : partialSoluSet) {
			cout << "   chromToInsert-" << c++ << endl;
			// 对某染色体的所有可插入位置，都插入
			this->insertToAChrom_Parellel(partialSoluSetNew, curChrom, gene, threadInfos);
			cout << "   after-Solution-size=" << partialSoluSetNew.size() << endl;
		}
		// 保留非支配解
		partialSoluSet.swap(partialSoluSetNew);
		for (auto curChrom : partialSoluSetNew)
			delete curChrom;
		partialSoluSetNew.clear();

		partialSoluSet.sort(cmpByObjFirst());

	}
	// 保留非支配解
	//getParetoSet(partialSoluSet);

	return;
};


void IPG::insertToAChrom_Parellel(list<Chromosome*>& partialSoluSetNew, Chromosome* curChrom, int gene, vector<threadInfoOfLS*>& threadInfos) {


	int subLen = curChrom->code.size();

	const int numOfThread = threadInfos.size();
	cout << "    subLen=" << subLen << endl;
	vector<vector<Chromosome*>> chromsForThreads(numOfThread);

	int t = 0;
	// 对某染色体的所有可插入位置，都插入
	for (int pos = 0; pos < subLen; ++pos) {
		Chromosome* chromNew = new Chromosome(curChrom);
		chromNew->code.insert(chromNew->code.begin() + pos, gene);

		chromsForThreads[t % numOfThread].emplace_back(chromNew);
		++t;

		int indexJump = rand() % 20;
		pos += indexJump;
	}

	vector<thread> threads(numOfThread);

	for (int t = 0; t < numOfThread; ++t) {
		cout << "    numsOfChromoOfAthread=" << chromsForThreads[t].size() << endl;
		threads[t] = thread(IPG::calculateTempChroms_thread, chromsForThreads[t], threadInfos[t]);
	}

	for (int t = 0; t < numOfThread; ++t) {
		threads[t].join();
		cout << "    after-Solution-size=" << partialSoluSetNew.size() << endl;
	}

	for (int i = 0; i < chromsForThreads.size(); ++i) {
		for (int j = 0; j < chromsForThreads[i].size(); ++j) {
			Chromosome* chromTmp = chromsForThreads[i][j];

			bool result = IPG::insertToParetoSet(partialSoluSetNew, chromTmp);
			cout << "  i=" << i << "  j=" << j
				<< "  objs:" << chromTmp->objectValues.first << "__" << chromTmp->objectValues.second
				<< "  result=" << result << "  soluSize=" << partialSoluSetNew.size() << endl;

			if (result == false)
				delete chromTmp;
		}
	}


	cout << endl;
	return;

};

void IPG::calculateTempChroms_thread(vector<Chromosome*>& chromsForThread, threadInfoOfLS* threadInfoP) {

	for (int i = 0; i < chromsForThread.size(); ++i) {
		Chromosome* chromTmp = chromsForThread[i];

		// 计算目标值，决定能否插入
		EDCode::getObjectValuesOfChromo_Parallel(chromTmp, threadInfoP, false);
	}
};



void IPG::update(list<Chromosome*>& preParetoSet, list<Chromosome*>& partialSoluSet) {
	for (auto curChrom : partialSoluSet) {
		bool result = insertToParetoSet(preParetoSet, curChrom);
		if (result == false)
			delete curChrom;
	}
	partialSoluSet.clear();
	preParetoSet.sort(cmpByObjSecond());
	preParetoSet.sort(cmpByObjFirst());

	return;
};


bool IPG::insertToParetoSet(list<Chromosome*>& partialSoluSetNew, Chromosome* chromNew) {
	/*
	for (auto it = partialSoluSetNew.begin(); it != partialSoluSetNew.end(); ++it) {
		cout << "    " << (*it)->objectValues.first << " " << (*it)->objectValues.second << endl;
	}
	cout << "" << endl;
	*/


	if (partialSoluSetNew.size() == 0) {
		partialSoluSetNew.emplace_back(chromNew);
		return true;
	}

	bool isInsert = false;
	//list<Chromosome*> paretoSet;
	for (auto iter = partialSoluSetNew.begin(); iter != partialSoluSetNew.end();) {
		Chromosome* curChrom = *iter;
		pair<double, double>& objValsNew = chromNew->objectValues;
		pair<double, double>& objVals = curChrom->objectValues;
		if ((objValsNew.first >= objVals.first) && (objValsNew.second >= objVals.second)) {
			isInsert = false;
			break;
		}
		else if ((objValsNew.first <= objVals.first) && (objValsNew.second <= objVals.second)) {
			isInsert = true;
			iter = partialSoluSetNew.erase(iter);
			if (iter == partialSoluSetNew.end())
				break;
		}
		else {
			isInsert = true;
			++iter;
		}
	}

	if (isInsert == true) {
		partialSoluSetNew.emplace_back(chromNew);

		/*
		for (auto it = partialSoluSetNew.begin(); it != partialSoluSetNew.end(); ++it) {
			cout << "    " << (*it)->objectValues.first << " " << (*it)->objectValues.second << endl;
		}
		cout << "-----------" << endl;
		cout << "" << endl;
		*/

		return true;
	}
	return false;
};


void IPG::getParetoSet(list<Chromosome*>& solutionSet) {
	list<Chromosome*> subParetoSet;
	int i = 0;
	for (auto& curChrom : solutionSet) {
		//cout<< "  i=" << i++ <<": " << curChrom->objectValues.first<<" "<< curChrom->objectValues.second << endl;
		bool result = insertToParetoSet(subParetoSet, curChrom);

		if (result == false)
			delete curChrom;
	}
	solutionSet.swap(subParetoSet);
	return;
};


void IPG::getObjVals_test(Chromosome* chromNew) {

	double obj = (1.0 * rand()) / (RAND_MAX + 1.0) + (int)(chromNew->code.size());
	double obj2 = (1.0 * rand()) / (RAND_MAX + 1.0) + (int)(chromNew->code.size());

	chromNew->objectValues.first = obj;
	chromNew->objectValues.second = obj2;

};





