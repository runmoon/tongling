#pragma once
#include<NDS.h>


bool cmpByCrowdingDistance(const pair<Chromosome*, double>& ele, const pair<Chromosome*, double>& ele2) {
	return ele.second > ele2.second;
};


void fastNondominationSort(list<Chromosome*>& solutionSet, vector<set<Chromosome*>>& paretoPlanes) {
	map<Chromosome*, pair<int, set<Chromosome*>>> infosForSort;
	for (auto iter = solutionSet.begin(); iter != solutionSet.end(); ++iter)
		infosForSort.insert(pair<Chromosome*, pair<int, set<Chromosome*>>>(*iter, make_pair(0, set<Chromosome*>{})));

	int i = 1;
	int i_count = 0;
	for (auto iter1 = solutionSet.begin(); iter1 != solutionSet.end(); ++iter1, i_count++) {

		Chromosome* chromP = *iter1;
		int j_count = 0;
		for (auto iter2 = solutionSet.begin(); iter2 != solutionSet.end(); ++iter2, j_count++) {
			Chromosome* chromP2 = *iter2;
			if (i_count == j_count) break;
			if (chromP->objectValues.first == chromP2->objectValues.first && chromP->objectValues.second == chromP2->objectValues.second)
				continue;

			
			//cout << i++ <<" i_count="<<i_count<<" j_count="<<j_count << endl;
			//cout <<"  "<< chromP->objectValues.first<<" " << chromP->objectValues.second << endl;
			//cout << "  " << chromP2->objectValues.first << " " << chromP2->objectValues.second << endl;
			

			if ((chromP->objectValues.first <= chromP2->objectValues.first && chromP->objectValues.second < chromP2->objectValues.second)
				|| (chromP->objectValues.first < chromP2->objectValues.first && chromP->objectValues.second <= chromP2->objectValues.second)) {
				pair<int, set<Chromosome*>>& values = infosForSort[chromP];
				infosForSort[chromP2].first += 1;              // 支配chromP2的个数加一
				infosForSort[chromP].second.insert(chromP2);   // 种群中被个体chromP支配的个体
			}
			if ((chromP2->objectValues.first <= chromP->objectValues.first && chromP2->objectValues.second < chromP->objectValues.second)
				|| (chromP2->objectValues.first < chromP->objectValues.first && chromP2->objectValues.second <= chromP->objectValues.second)) {
				pair<int, set<Chromosome*>>& values = infosForSort[chromP2];
				infosForSort[chromP].first += 1;               // 支配chromP的个数加一
				infosForSort[chromP2].second.insert(chromP);   // 种群中被个体chromP2支配的个体
			}
		}
	}

	/*
	// 打印每个染色体支配的染色体的值
	cout << endl;
	for (auto& ele :infosForSort) {
		cout << "num " << ele.first->objectValues.first << " " << ele.first->objectValues.second << endl;;
		cout << " " << (ele.second.first);
		for (auto ele2 : ele.second.second)
			cout << "    " << ele2->objectValues.first<<" "<<ele2->objectValues.second;
		cout << endl; cout << endl;
	}*/


	while (solutionSet.size() != 0) {
		paretoPlanes.emplace_back(set<Chromosome*>{});
		set<Chromosome*>& curPlane = *(paretoPlanes.end() - 1);  // 初始化一个pareto平面
		//cout << "solutionSet.size()=" << solutionSet.size() << endl;

		for (auto iter = solutionSet.begin(); iter != solutionSet.end();) {
			Chromosome* chromP = *iter;
			//cout << "  infosForSort[chromP].first=" << infosForSort[chromP].first <<"  " << infosForSort[chromP].second.size() << endl;
			if (infosForSort[chromP].first == 0) {
				curPlane.insert(chromP);
				iter = solutionSet.erase(iter);
				if (iter == solutionSet.end()) break;
			}
			else
				++iter;
		}
		//cout << "curPlane.size()=" << curPlane.size() << endl;
		for (auto& chromP : curPlane) {   // 对于该层的pareto面
			//cout << "   curPlane=" << infosForSort[chromP].first << "  " << infosForSort[chromP].second.size() << endl;
			for (auto& chromP2 : infosForSort[chromP].second)   // 对于每个被chromP所支配的个体chromP2
				infosForSort[chromP2].first -= 1;               // 支配chromP2的个数减1
		}
	}

	/*
	// 去重
	for (auto iter = paretoPlanes.begin(); iter != paretoPlanes.end(); ++iter) {
		set<Chromosome*>& curPlane = *iter;
		list<Chromosome*> paretoList;

		paretoList.assign(curPlane.begin(), curPlane.end());
		auto iter2 = paretoList.begin();
		set<pair<double, double>> objsSet;
		for (; iter2!=paretoList.end(); ) {
			Chromosome* chromP = *iter2;
			if (objsSet.find(chromP->objectValues) != objsSet.end()) {
				iter2 = paretoList.erase(iter2);
				if (iter2 == paretoList.end()) break;
			}
			else {
				objsSet.emplace(chromP);
				++iter2;
			}
		}
		curPlane.
		curPlane.assign(paretoList.begin(), paretoList.end());
	}
	*/


	//打印各层pareto平面
	cout << endl;
	cout << "Pareto Plans:" << endl;
	int ji = 1;
	for (auto iter = paretoPlanes.begin(); iter != paretoPlanes.end(); ++iter) {
		set<Chromosome*>& curPlane = *iter;
		cout << "  plane " << ji++ << " :";
		for (auto chromP : curPlane) {
			cout << "   " << chromP->objectValues.first << " " << chromP->objectValues.second;
		}
		cout << endl; cout << endl;
	}
	cout << endl;


	//vector<pair<Chromosome*, double>> crowdingDistances;
	//getDensityEstimation(paretoPlanes[4], crowdingDistances);

	return;
};


void getDensityEstimation(set<Chromosome*>& paretoPlane, vector<pair<Chromosome*, double>>& crowdingDistances) {
	crowdingDistances.clear();



	if (paretoPlane.size() <= 0) return;

	list<Chromosome*> popPool;
	popPool.assign(paretoPlane.begin(), paretoPlane.end());
	popPool.sort(cmpByObjFirst_NDS());
	popPool.sort(cmpByObjSecond_NDS());

	//vector<double> crowdingDistances;
	crowdingDistances.reserve(popPool.size());

	crowdingDistances.emplace_back(make_pair(*popPool.begin(), DBL_MAX));
	if (paretoPlane.size() == 1) return;
	auto iterLastOne = popPool.end();
	--iterLastOne;
	crowdingDistances.emplace_back(make_pair(*iterLastOne, DBL_MAX));
	if (paretoPlane.size() == 2) return;



	double minObj = (*popPool.begin())->objectValues.first;
	double maxObj = 0.0;
	double minObj2 = 0.0;
	double maxObj2 = (*popPool.begin())->objectValues.second;;

	auto iter = popPool.begin();

	++iter;
	auto iterLast = popPool.end();
	--iterLast;
	maxObj = (*iterLast)->objectValues.first;
	minObj2 = (*iterLast)->objectValues.second;

	auto iterPre = iter;
	auto iterRare = iter;
	--iterPre;
	++iterRare;

	for (; iter != iterLast; ++iter, ++iterPre, ++iterRare) {
		Chromosome* chromP = *iter;
		//if (chromP->objectValues.first == minObj || chromP->objectValues.first == maxObj) continue;
		double crowdingVal;
		if ((chromP->objectValues.first == minObj || chromP->objectValues.second == maxObj2)
			&& (chromP->objectValues.first == minObj2 || chromP->objectValues.second == maxObj))
			crowdingVal = 0.0;
		else
			crowdingVal = ((*iterRare)->objectValues.first - (*iterPre)->objectValues.first) / (maxObj - minObj)
			+ ((*iterPre)->objectValues.second - (*iterRare)->objectValues.second) / (maxObj2 - minObj2);
		crowdingDistances.emplace_back(make_pair(chromP, crowdingVal));

	}

	cout << "crowdingDistances.size()=" << crowdingDistances.size() << endl;
	for (int i = 0; i < crowdingDistances.size(); ++i) {
		cout << "  i=" << i << "  crowdingDistances:" << crowdingDistances[i].second
			<< "  " << crowdingDistances[i].first 
			<<"   objs="<< crowdingDistances[i].first->objectValues.first<< "--" <<crowdingDistances[i].first->objectValues.second << endl;
	}
	sort(crowdingDistances.begin(), crowdingDistances.end(), cmpByCrowdingDistance);

	cout << endl;
	for (int i = 0; i < crowdingDistances.size(); ++i) {
		cout << "  i=" << i << "  crowdingDistances:" << crowdingDistances[i].second << endl;
	}
	return;

}







