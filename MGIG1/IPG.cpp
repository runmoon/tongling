#pragma once
#include<IPG.h>


void randomlyCreateInitialSolution_test(list<Chromosome*>& popPool) {

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


Chromosome* selection(list<Chromosome*>& paretoSoluSet) {
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


void destruction(Chromosome* chromChosen, vector<int>& genesChrosen) {
	const int nummin = 2;
	const int nummax = 5;

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


void construction(list<Chromosome*>& partialSoluSet, Chromosome* chromChosen, vector<int>& genesChrosen) {
	int codeLen = chromChosen->codeLen;
	int num = (int)genesChrosen.size();
	cout << "    num=" << num << endl;

	int subLen = codeLen - num;


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
				getObjValsTest(chromNew);
				bool result = insertToParetoSet(partialSoluSetNew, chromNew);
				cout << "    pos=" << pos << "  chrom " << pos
					<< ": obj1 = " << chromNew->objectValues.first << "  obj2 = " << chromNew->objectValues.second
					<< "    result=" << result << endl;

				if (result == false)
					delete chromNew;
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


void update(list<Chromosome*>& preParetoSet, list<Chromosome*>& partialSoluSet) {
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


bool insertToParetoSet(list<Chromosome*>& partialSoluSetNew, Chromosome* chromNew) {
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
	for (auto iter = partialSoluSetNew.begin(); iter != partialSoluSetNew.end(); ++iter) {
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
			if (iter == partialSoluSetNew.end()) break;
			if (iter != partialSoluSetNew.begin())
				--iter;
		}
		else {
			isInsert = true;
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


void getParetoSet(list<Chromosome*>& solutionSet) {
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


void getObjValsTest(Chromosome* chromNew) {

	double obj = (1.0 * rand()) / (RAND_MAX + 1.0) + (int)(chromNew->code.size());
	double obj2 = (1.0 * rand()) / (RAND_MAX + 1.0) + (int)(chromNew->code.size());

	chromNew->objectValues.first = obj;
	chromNew->objectValues.second = obj2;

};





