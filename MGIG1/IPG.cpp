#pragma once
#include<IPG.h>


extern std::mutex g_mutex;


void IPG::run(const int maxRound) {

	clock_t startTime, endTime;
	startTime = clock();


	list<pair<pair<double, double>, pair<double, double>>> objValsRecord;
	list<bool> isRenewRecord;
	list<Chromosome*> popPoolPre=this->popPool;
	list<list<int>> objValChanges;

	auto lastIter = popPool.end();
	--lastIter;
	objValsRecord.emplace_back(make_pair((*popPool.begin())->objectValues, (*lastIter)->objectValues));
	if (popPoolPre == popPool)
		isRenewRecord.emplace_back(false);
	else
		isRenewRecord.emplace_back(true);


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




		//if (i_round % 20) Sleep(4000);
		//char c; cin >> c;



		Chromosome* chosenChromo = this->selection(popPool);
		Chromosome* chosenChrom = new Chromosome(chosenChromo);
		//chosenChrom->code = vector<int>{ 0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,5,5,5,6,6,6,7,7,7,8 };
		//chosenChrom->codeLen = 25;

		vector<int> genesChosen;
		this->destruction(chosenChrom, genesChosen);

		cout << "codes:" << endl;
		for (int i = 0; i < chosenChrom->code.size(); ++i) {
			cout << " " << chosenChrom->code[i];
		}
		cout << endl;

		cout << "  Iter= " << i_round <<"  : " << chosenChrom->objectValues.first << "--" << chosenChrom->objectValues.second << endl;


		//const unsigned int num_thread = thread::hardware_concurrency();  // ��ȡӲ����֧�ֵ��̸߳���
		const int num_thread = 5;  // �����������Ϊ6������Ϊ5ʱ��CPU���ռ��85%
		vector<threadInfoOfLS*> threadInfos = initThreadsInfoOfLS(num_thread, this->edCodeP->m_jobOrderBefP, this->edCodeP->m_machsMapBefP);


		list<Chromosome*> partialSoluSet;
		//this->construction(partialSoluSet, chosenChrom, genesChosen);
		//this->construction_Parallel(partialSoluSet, chosenChrom, genesChosen, threadInfos);
		list<int> objValChange = this->construction_MultiParetos_Parallel(partialSoluSet, chosenChrom, genesChosen, threadInfos);

		objValChanges.emplace_back(objValChange);


		popPoolPre = popPool;

		cout <<"updating--------------" << endl;
		this->update(popPool, partialSoluSet);
		cout << "finashed of updating--------------" << endl;

		auto lastIter = popPool.end();
		--lastIter;
		objValsRecord.emplace_back(make_pair((*popPool.begin())->objectValues, (*lastIter)->objectValues));
		if (popPoolPre == popPool)
			isRenewRecord.emplace_back(false);
		else
			isRenewRecord.emplace_back(true);


		j = 0;
		for (auto iter = popPool.begin(); iter != popPool.end(); ++iter) {
			Chromosome* chromP = *iter;
			cout << "chrom " << j++ << ": obj1 = " << chromP->objectValues.first << "  obj2 = " << chromP->objectValues.second << endl;
		}
		cout << "end.." << endl;

		cout << endl;
		cout << "End of Iter " << i_round << " --------------------" << endl;
		cout << endl; cout << endl;

	}

	endTime = clock();
	double t_sec = (double)(endTime - startTime) / CLOCKS_PER_SEC;

	cout << "The run time is: " << t_sec << "s  " << t_sec / 60 << "min" << endl;


	cout << endl;
	int index = 0;
	auto isNewIter = isRenewRecord.begin();
	for (auto& ele : objValsRecord) {
		cout << "  index=" << ++index << "  " << ele.first.first << "--" << ele.first.second
			<< "  " << ele.second.first << "--" << ele.second.second 
			<<"    IsNew="<< *isNewIter << endl;
		++isNewIter;
	}
	cout << endl; cout << endl;


	isNewIter = isRenewRecord.begin();
	++isNewIter;
	index = 0;
	cout << "ÿ�ε������������" << endl;
	for (auto& ele : objValChanges) {
		cout << "  index=" << ++index << ":  ";
		for(auto ele2:ele) {
			cout << "  " << ele2; 
		}
		cout  <<"      IsNew=" << *isNewIter << endl;
		++isNewIter;
	}

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

	// �Ų����1���ɳ�ʼ��
	for (int i = 0; i < 1; ++i) {
		Chromosome* chromPInitnew = new Chromosome(totalLenOfChromCode, numOfJobRange);
		popPool.emplace_back(chromPInitnew);
		chromPInitnew->code.assign(chromPInit->code.begin(), chromPInit->code.end());
	}

	// �Ų����2���ɳ�ʼ��
	for (int i = 1; i < 2; ++i) {
		Chromosome* chromPInitnew = new Chromosome(totalLenOfChromCode, numOfJobRange);
		popPool.emplace_back(chromPInitnew);
		chromPInitnew->code.assign(chromPInit2->code.begin(), chromPInit2->code.end());
	}

	// ������ɳ�ʼ��
	for (int i_chrom = 2; i_chrom < numOfPop; ++i_chrom) {
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

	// �ɳ�ʼ�Ų����������������ɳ�ʼ��
	double rate = 0.1;  // ����������һ��
	int numOfMove = totalLenOfChromCode * rate;
	for (int i_chrom = numOfPop; i_chrom < numOfPop; ++i_chrom) {
		Chromosome* chromPInitnew = new Chromosome(totalLenOfChromCode, numOfJobRange);
		popPool.emplace_back(chromPInitnew);
		chromPInitnew->code.assign(chromPInit->code.begin(), chromPInit->code.end());
		for (int i = 0; i < numOfMove; ++i) {
			int left = totalLenOfChromCode * (1.0 * rand()) / (RAND_MAX + 1.0);
			int right = totalLenOfChromCode * (1.0 * rand()) / (RAND_MAX + 1.0);
			swap(chromPInitnew->code[left], chromPInitnew->code[right]);
		}
	}

	cout <<"popPoolnym" << endl;
	this->initObjValsOfChromos();
	cout << "   popPoolnym_initObjValsOfChromos" << endl;

	return;
};


void IPG::initObjValsOfChromos() {
	list<Chromosome*>& popPool = this->popPool;
	int i = 0;
	for (auto& chromP : popPool) {
		//cout << "chrom_i="<<i << endl;
		this->edCodeP->getObjectValuesOfChromo(chromP, false);
	}
};


Chromosome* IPG::selection(list<Chromosome*>& paretoSoluSet) {
	Chromosome* chosenChrom;
	cout << "paretoSoluSet.size()=" << paretoSoluSet.size() << endl;

	// ���paretoSoluSet�н�ĸ���С��6���������ѡ
	if (paretoSoluSet.size() < 6) {
		auto iter = paretoSoluSet.begin();
		int randNum = rand() % paretoSoluSet.size();
		std::advance(iter, randNum);
		chosenChrom = *iter;
		return chosenChrom;
	}

	// �Ҽ��˽⣨ĳ��Ŀ����С�Ľ⣩������paretoSoluSet�������
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
		swap(*iterOfminObj, *iter);    // ���˽����paretoSoluSet�������
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

	// 0.2�ĸ���ѡ���˽⣬0.8�ĸ���ѡ������
	iter = paretoSoluSet.end();
	--iter;
	long numOfChroms = paretoSoluSet.size();
	double num = (1.0 * rand()) / (RAND_MAX + 1.0);  // [0, 1)
	cout << "num=" << num << endl;
	if (num < 0.1)               // 0.1�ĸ���
		chosenChrom = (*iter);
	else if (num < 0.2) {        // 0.1�ĸ���
		--iter;
		chosenChrom = *(iter);
	}
	else {                       // 0.8�ĸ���
		long index = (long)(numOfChroms - 2.0) * (1.0 * rand()) / (RAND_MAX + 1.0);
		cout << "    index=" << index << endl;
		list<Chromosome*>::iterator iter = paretoSoluSet.begin();
		std::advance(iter, index);
		chosenChrom = *iter;
	}


	return chosenChrom;
};


void IPG::destruction(Chromosome* chromChosen, vector<int>& genesChosen) {
	//const int nummin = 10;
	//const int nummax = 15;

	int num = (int)(nummax - nummin) * (1.0 * rand()) / (RAND_MAX + 1.0) + nummin;  // [0, 1)
	cout << "  num Of gene: " << num << endl;

	genesChosen.reserve(num);
	set<int> genesSet;

	for (int i = 0; i < num; i++) {
		while (genesChosen.size() <= i) {
			const size_t& codeLen = chromChosen->code.size();
			int index = (int)(codeLen * (1.0 * rand()) / (RAND_MAX + 1.0));  // [0, 1)
			int gene = chromChosen->code[index];
			if (genesSet.insert(gene).second) {  // ����ɹ�
				genesChosen.emplace_back(gene);

				chromChosen->code.erase(chromChosen->code.begin() + index);
				break;
			}
		}
	}

	cout << "  chosen genes:";
	for (int i = 0; i < num; i++) {
		cout << " " << i << ":gene=" << genesChosen[i] << "  ";
	}
	cout << endl;

	return;
};


void IPG::construction(list<Chromosome*>& partialSoluSet, Chromosome* chromChosen, vector<int>& genesChosen) {
	int codeLen = chromChosen->codeLen;
	int num = (int)genesChosen.size();
	cout << "    num=" << num << endl;

	int subLen = codeLen - num;

	cout << "    subLen=" << subLen << endl;


	list<Chromosome*> partialSoluSetNew;
	partialSoluSet.emplace_back(chromChosen);

	for (int i = 0; i < num; i++) {
		int gene = genesChosen[i];
		cout << "  i=" << i << "  gene:" << gene << endl;
		int c = 0;
		// ��ÿһ��Ⱦɫ�嶼����
		for (auto& curChrom : partialSoluSet)
		{
			cout << "   chrom " << c++ << endl;
			// ��ĳȾɫ������пɲ���λ�ã�������
			for (int pos = 0; pos < subLen; ++pos) {
				Chromosome* chromNew = new Chromosome(curChrom);
				chromNew->code.insert(chromNew->code.begin() + pos, gene);

				// ����Ŀ��ֵ�������ܷ����
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
		// ������֧���
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
	// ������֧���
	//getParetoSet(partialSoluSet);

	return;
};






void IPG::construction_Parallel(list<Chromosome*>& partialSoluSet, Chromosome* chromChosen, vector<int>& genesChosen, vector<threadInfoOfLS*>& threadInfos) {
	int codeLen = chromChosen->codeLen;
	int num = (int)genesChosen.size();
	cout << "  NUM Of chosen genes=" << num << endl;

	int subLen = codeLen - num;
	cout << "  SubLen=" << subLen << endl;


	list<Chromosome*> partialSoluSetNew;
	partialSoluSet.emplace_back(chromChosen);


	for (int i = 0; i < num; i++) {
		int gene = genesChosen[i];
		cout << "    i_index Of chosen gene: " << i+1 << "  gene:" << gene << endl;
		cout << "    num of Chroms in partialSoluSet: " << partialSoluSet.size() << endl;
		int beforeSizeP = partialSoluSet.size();
		
		int c = 0;
		// ��ÿһ��Ⱦɫ�嶼����
		for (auto& curChrom : partialSoluSet) {
			cout << "      index Of chrom in partialSoluSet: " << ++c << ";  totol size of partialSoluSet=" << partialSoluSet.size()<< endl;
			int beforeSize = partialSoluSetNew.size();
			// ��ĳȾɫ������пɲ���λ�ã�������
			this->insertToAChrom_Parellel(partialSoluSetNew, curChrom, gene, threadInfos);
			cout << "      size of partialSoluSetNew after insert to partial Chrom "<< i+1 <<" :" << partialSoluSetNew.size()
				<<"    before size=" << beforeSize << endl;
			cout << "      num of partial Chroms left to insert: "<< partialSoluSet.size() -c << endl;
			cout <<"      End of this partial Chroms' insertion!" << endl;
			cout << endl;
		}
		// ������֧���
		partialSoluSet.swap(partialSoluSetNew);
		for (auto curChrom : partialSoluSetNew)
			delete curChrom;
		partialSoluSetNew.clear();

		partialSoluSet.sort(cmpByObjFirst());
		cout << endl;
		cout << "    Before size Of partialSoluSet=" << beforeSizeP << "  After size=" << partialSoluSet.size() << endl;
		cout << "    Num of Genes left to insert: " << num - i - 1 << endl;
		cout << "    End of gene '"<<gene<<"' insertation; =========================================" << endl;

		cout << endl;
	}
	// ������֧���
	//getParetoSet(partialSoluSet);

	return;
};


void IPG::insertToAChrom_Parellel(list<Chromosome*>& partialSoluSetNew, Chromosome* curChrom, int gene, vector<threadInfoOfLS*>& threadInfos) {


	int subLen = curChrom->code.size();

	const int numOfThread = threadInfos.size();
	cout << "        subLen=" << subLen << endl;
	vector<vector<Chromosome*>> chromsForThreads(numOfThread);

	int t = 0;
	// ��ĳȾɫ������пɲ���λ�ã������룬�õ������µĲ��ֽ��Ӧ��Ⱦɫ��
	for (int pos = 0; pos < subLen; ++pos) {
		Chromosome* chromNew = new Chromosome(curChrom);
		chromNew->code.insert(chromNew->code.begin() + pos, gene);

		chromsForThreads[t % numOfThread].emplace_back(chromNew);
		++t;

		int indexJump = rand() % 20;
		pos += indexJump;
	}

	vector<thread> threads(numOfThread);

	// ��Ⱦɫ�����������̣߳��ֱ����Ŀ��ֵ
	for (int t = 0; t < numOfThread; ++t) {
		cout << "        numsOfChromoOfAthread=" << chromsForThreads[t].size() << endl;
		threads[t] = thread(IPG::calculateTempChroms_thread, chromsForThreads[t], threadInfos[t]);
	}

	for (int t = 0; t < numOfThread; ++t)
		threads[t].join();

	// ����ÿһ��Ⱦɫ�壬���Ŀ��ֵ�����ܷ���벿�ֽ�����׶�⼯
	for (int i = 0; i < chromsForThreads.size(); ++i) {
		cout << "        thread: " << i << endl;
		for (int j = 0; j < chromsForThreads[i].size(); ++j) {
			Chromosome* chromTmp = chromsForThreads[i][j];
			bool result = IPG::insertToParetoSet(partialSoluSetNew, chromTmp);
			cout << "          i=" << i << "  j=" << j
				<< "    objs:" << chromTmp->objectValues.first << "__" << chromTmp->objectValues.second
				<< "    result=" << result << "  soluSize=" << partialSoluSetNew.size() << endl;

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

		// ����Ŀ��ֵ�������ܷ����
		EDCode::getObjectValuesOfChromo_Parallel(chromTmp, threadInfoP, false);
	}
};







list<int> IPG::construction_MultiParetos_Parallel(list<Chromosome*>& partialSoluSet, Chromosome* chromChosen, vector<int>& genesChosen, vector<threadInfoOfLS*>& threadInfos) {
	int codeLen = chromChosen->codeLen;
	int numOfChosenGene = (int)genesChosen.size();
	cout << "  NUM Of chosen genes=" << numOfChosenGene << endl;

	int subLen = codeLen - numOfChosenGene;
	cout << "  SubLen=" << subLen << endl;


	list<Chromosome*> partialSoluSetNew;
	partialSoluSet.emplace_back(chromChosen);

	list<int> objValChange;


	for (int i = 0; i < numOfChosenGene; i++) {
		int gene = genesChosen[i];
		cout << "    i_index Of chosen gene: " << i + 1 << "  gene:" << gene << endl;
		cout << "    num of Chroms in partialSoluSet: " << partialSoluSet.size() << endl;
		int beforeSizeP = partialSoluSet.size();

		int c = 0;
		// ��ÿһ��Ⱦɫ�嶼����
		for (auto& curChrom : partialSoluSet) {
			cout << "      index Of chrom in partialSoluSet: " << ++c << ";  totol size of partialSoluSet=" << partialSoluSet.size() << endl;
			int beforeSize = partialSoluSetNew.size();
			// ��ĳȾɫ������пɲ���λ�ã�������
			this->insertToAChrom__MultiParetos_Parellel(partialSoluSetNew, curChrom, gene, threadInfos);
			cout << "      size of partialSoluSetNew after insert to partial Chrom " << i + 1 << " :" << partialSoluSetNew.size()
				<< "    before size=" << beforeSize << endl;
			cout << "      num of partial Chroms left to insert: " << partialSoluSet.size() - c << endl;
			cout << "      End of this partial Chroms' insertion!" << endl;
			cout << endl;
		}

		cout<<" Ŀ��ֵ��" << endl;
		for (Chromosome* chromP : partialSoluSetNew)
			cout << "  " << chromP->objectValues.first << "--" << chromP->objectValues.second << endl;

		// ȥ��--Ŀ��ֵ��ͬ��Ⱦɫ��ֻ����һ��
		set<pair<double, double>> objValsSet;
		auto iter = partialSoluSetNew.begin();
		for (;iter!= partialSoluSetNew.end();) {
			Chromosome* chromP = *iter;
			if (objValsSet.find(chromP->objectValues) != objValsSet.end()) {  // ��������ȥ�ز���--ֱ��ɾ���������滻
				iter = partialSoluSetNew.erase(iter);   
				delete chromP;
				if (iter == partialSoluSetNew.end())  break;
			}
			else {
				objValsSet.emplace(chromP->objectValues);
				++iter;
			}
		}
		cout << " Ŀ��ֵ��ȥ��֮�󣩣�" << endl;
		for (Chromosome* chromP : partialSoluSetNew)
			cout << "  " << chromP->objectValues.first << "--" << chromP->objectValues.second << endl;
		cout << "finised ȥ��" << endl;


		const int maxNumOfPop = 5;
		if (numOfChosenGene - i > 5)
			choseByPareto(partialSoluSet, partialSoluSetNew);                      // ѡ��-ͨ�����׶������ӵ������ѡ
		else
			choseByMultiParetos(partialSoluSet, partialSoluSetNew, maxNumOfPop);   // ѡ��-ͨ�����׶������ӵ������ѡ


		cout << "finised ���׶�" << endl;

		partialSoluSet.sort(cmpByObjFirst());
		partialSoluSet.sort(cmpByObjSecond());

		for (Chromosome* chromP : partialSoluSet)
			cout << "  " << chromP->objectValues.first << "--" << chromP->objectValues.second << endl;
		
		cout << endl;
		cout << "    Before size Of partialSoluSet=" << beforeSizeP << "  After size=" << partialSoluSet.size() << endl;
		cout << "    Num of Genes left to insert: " << numOfChosenGene - i - 1 << endl;
		cout << "    End of gene '" << gene << "' insertation; =========================================" << endl;

		cout << endl;



		auto lastIter = partialSoluSet.end();
		auto lastIter2 = popPool.end();
		--lastIter; --lastIter2;
		if ( (*partialSoluSet.begin())->objectValues.first < (*popPool.begin())->objectValues.first
			|| (*lastIter)->objectValues.second < (*lastIter2)->objectValues.second) {
			objValChange.emplace_back(1);
		}
		else {
			objValChange.emplace_back(0);
		}
	}
	// ������֧���
	getParetoSet(partialSoluSet);
	cout << "9999999999999999999999999999999999999999999999999999999" << endl;
	//char c4; cin >> c4;
	return objValChange;
};


// ��һ��Ⱦɫ����в��룬�õ������µģ����ֽ��Ӧ�ģ�Ⱦɫ��
void IPG::insertToAChrom__MultiParetos_Parellel(list<Chromosome*>& partialSoluSetNew, Chromosome* curChrom, int gene, vector<threadInfoOfLS*>& threadInfos) {


	int subLen = curChrom->code.size();

	const int numOfThread = threadInfos.size();
	cout << "      subLen=" << subLen << endl;
	vector<vector<Chromosome*>> chromsForThreads(numOfThread);

	int increment = rand() % 10;
	int t = 0;
	// ��ĳȾɫ������пɲ���λ�ã������룬�õ������µĲ��ֽ��Ӧ��Ⱦɫ��
	for (int pos = 0; pos < subLen; ++pos) {
		Chromosome* chromNew = new Chromosome(curChrom);
		chromNew->code.insert(chromNew->code.begin() + pos, gene);

		partialSoluSetNew.emplace_back(chromNew);  // ���벿�ֽ�Ľ⼯��

		chromsForThreads[t % numOfThread].emplace_back(chromNew);
		++t;

		//int indexJump = rand() % 20;
		int indexJump = rand() % 40 + increment;
		pos += indexJump;
	}

	vector<thread> threads(numOfThread);

	// ��Ⱦɫ�����������̣߳��ֱ����Ŀ��ֵ
	for (int t = 0; t < numOfThread; ++t) {
		cout << "        numsOfChromoOfAthread=" << chromsForThreads[t].size() << endl;
		threads[t] = thread(IPG::calculateTempChroms_thread, chromsForThreads[t], threadInfos[t]);
	}

	for (int t = 0; t < numOfThread; ++t)
		threads[t].join();

	cout << endl;
	return;

};


void IPG::choseByMultiParetos(list<Chromosome*>& partialSoluSet, list<Chromosome*>& partialSoluSetNew, const int maxNumOfPop) {
	
	cout << "  partialSoluSet.size()=" << partialSoluSet.size() << endl;

	for (auto& ele:partialSoluSet) {
		cout<<" partialsolut: " << ele->objectValues.first << " " << ele->objectValues.second << endl;
		Chromosome* chrom = ele;
		cout<< chrom <<endl;
	}

	for (auto& chromP : partialSoluSet)	delete chromP;
		
	cout << "  cvvvvvvvvvvv" << endl;
	partialSoluSet.clear();

	cout << "  ioooooooo" << endl;
	cout << "  partialSoluSetNew.size()=" << partialSoluSetNew.size() << endl;
	if (partialSoluSetNew.size() <= maxNumOfPop) {
		cout << "  tyyyyyyyyyyy" << endl;
		// ������֧���
		partialSoluSet.swap(partialSoluSetNew);
		return;
	}
	vector<set<Chromosome*>> paretoPlanes;

	cout << "  ��ʼ���ٷ�֧������......" << endl;
	fastNondominationSort(partialSoluSetNew, paretoPlanes);

	int index_plane=1;
	int curSize = 0;
	vector<pair<Chromosome*, double>> crowdingDistances;  // ӵ����
	for (auto iter = paretoPlanes.begin(); iter != paretoPlanes.end(); ++iter) {
		crowdingDistances.clear();

		set<Chromosome*>& curPlane = *iter;

		cout << "    plane "<< index_plane++ <<endl;
		if (curSize == maxNumOfPop) {    // ���ڶ��ڵ�Ⱦɫ�壬��������
			for (auto curChrom : curPlane)  
				delete curChrom;
			continue;
		}
		/*
		if (planeSize + curSize <= maxNumOfPop) {  // ȫ������
			for (auto chromP : curPlane)
				partialSoluSet.emplace_back(chromP);
			curSize += planeSize;
		}
		*/
		getDensityEstimation(curPlane, crowdingDistances);    // ����ӵ����

		for (auto& chromInfo : crowdingDistances){
			Chromosome* chromP = chromInfo.first;
			if (curSize < maxNumOfPop && chromInfo.second != 0.0) {  // ����
				partialSoluSet.emplace_back(chromP);
				cout << "      ������һ���⼯��Ⱦɫ�壺" << chromP << endl;
				++curSize;
			}
			else                          // ɾ��
				delete chromP;
		}
	}
	partialSoluSetNew.clear();
	return;
}


void IPG::choseByPareto(list<Chromosome*>& partialSoluSet, list<Chromosome*>& partialSoluSetNew) {

	cout << "  partialSoluSet.size()=" << partialSoluSet.size() << endl;

	for (auto& ele : partialSoluSet) {
		cout << " partialsolut: " << ele->objectValues.first << " " << ele->objectValues.second << endl;
		Chromosome* chrom = ele;
		cout << chrom << endl;
	}

	for (auto& chromP : partialSoluSet)	delete chromP;

	partialSoluSet.clear();

	this->getParetoSet(partialSoluSetNew);

	partialSoluSet.swap(partialSoluSetNew);
	partialSoluSetNew.clear();

	return;
}







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







