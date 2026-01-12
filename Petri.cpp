#include<iostream>
#include<list>
#include<vector>
#include<queue>
#include<map>
#include<set>
#include<stack>
#include<algorithm>
using namespace std;

const int NUM = 128;

enum Type{
	//用于表示节点类型
	SS,PP,TT,EN,NONE
};

class Next {
private:
	int index;
	bool visited = false;
public:
	Next(int index) {
		this->index = index;
	}

	int getIndex() {
		return this->index;
	}

	void setVisited() {
		this->visited = true;
	}

	void clearVisited() {
		this->visited = false;
	}

	bool getVisited() {
		return this->visited;
	}
};

class Node {
public:
	Type type ;//节点类型
	int index;//节点总序列
	int offsetIndex;//节点内部序列
	double rc;
	double rl;
	double rt;
	list<Next> next;

	Node() {
		type = NONE;
		index = -1;
		offsetIndex = -1;
		rc = -1;
		rl = -1;
		rt = -1;
	}

	Node(Type type, int position) {
		this->type = type;
		index = position;
		offsetIndex = -1;
		rc = -1;
		rl = -1;
		rt = -1;
	}

	Node(Type type, int offsetIndex, double rc, int count) {
		this->type = type;
		index = offsetIndex + count;
		this->offsetIndex = offsetIndex;
		this->rc = rc;
		rl = -1;
		rt = -1;
	}

	Node(Type type, int offsetIndex, double rl, double rt, int count) {
		this->type = type;
		index = offsetIndex + count;
		this->offsetIndex = offsetIndex;
		rc = -1;
		this->rl = rl;
		this->rt = rt;
	}
};

class Petri {
public:
	int num;//总结点数
	int pnum;//P类节点数
	int tnum;//T类节点数
	int lnum;//边数目
	Node node[NUM];

	//初始化节点
	void start() {
		//输入各类节点数
		cin >> pnum;
		cin >> tnum;
		num = pnum + tnum + 2;

		//输入起点
		int a;
		int count = 0;
		cin >> a;
		if (a == 0) {
			Node linNode(SS, 1);
			node[0] = linNode;
			count++;
		}
		else {
			cout << "input 1 error";
		}

		//输入P类节点
		for (size_t i = 0; i < pnum; i++)
		{
			int a, b;
			double c;
			cin >> a >> b >> c;
			if (a == 1) {
				Node linNode(PP, b, c, count);
				node[i + count] = linNode;
			}
			else {
				cout<< "input 2 error";
			}
		}
		count += pnum;

		//输入T类节点
		for (size_t i = 0; i < tnum; i++)
		{
			int a, b;
			double c, d;
			cin >> a >> b >> c >>d;
			if (a == 2) {
				Node linNode(TT, b, c, d, count);
				node[i + count] = linNode;
			}
			else {
				cout << "input 3 error";
			}
		}
		count += tnum;

		//输入终点
		a = 0;
		cin >> a;
		if (a == 3) {
			Node linNode(EN, count + 1);
			node[count] = linNode;
		}
		else {
			cout << "input 4 error";
		}
		count++;
	}

	//初始化边
	void startLine() {
		cin >> lnum;
		for (size_t i = 0; i < lnum; i++)
		{
			int a, b;
			cin >> a >> b;
			Next linLine(b);
			node[a-1].next.push_back(linLine);
		}
	}

	//遍历petri网
	void SPetri(vector<vector<int>>& vc) {
		queue<int>que;
		int count = 0;
		que.push(1);
		multimap<int, int> mp;//前为孩子节点，后为父节点
		set<int>sSet;//储存断节点
		while (!que.empty()) {
			int a = que.front();
			que.pop();
			count++;
			if (count == lnum) {
				sSet.insert(a);
				break;
			}
			auto it1 = node[a - 1].next.begin();
			while (it1 != node[a - 1].next.end()) {
				if (!it1->getVisited()) {
					it1->setVisited();
					mp.insert(pair<int, int>((*it1).getIndex(), a));
					que.push((*it1).getIndex());
				}
				else {
					//跳过
					//sSet.insert(it1->getIndex());
				}
				it1++;
			}
		}

		//得到PW路径
		stack<int>sta;
		stack<int>sta1;
		sta.push(21);
		while (!sta.empty()) {
			int a = sta.top();
			sta.pop();
			sta1.push(a);
			if (a == 1) {
				vector<int> pw;
				while (!sta1.empty()) {
					pw.push_back(sta1.top());
					sta1.pop();
				}
				vc.push_back(pw);
				if (sta.empty()) {
					break;
				}
				int c = sta.top();
				int d = node[c - 1].next.begin()->getIndex();
				reverse(pw.begin(),pw.end());
				for (auto it4 = pw.begin(); it4 != pw.end(); it4++) {
					if (*it4 != d) {
						sta1.push(*it4);
					}
					else {
						sta1.push(*it4);
						break;
					}
				}
			}
			if (mp.contains(a)) {
				auto it1 = mp.equal_range(a);
				for (auto it2 = it1.first; it2 != it1.second; it2++)
				{
					int b = it2->second;
					sta.push(b);
				}
			}
		}

		//处理断节点
		for (auto it1 = sSet.begin(); it1 != sSet.end(); it1++) {
			sta.push(*it1);
			while (!sta.empty()) {
				int a = sta.top();
				sta.pop();
				sta1.push(a);
				if (a == 1) {
					vector<int> pw;
					while (!sta1.empty()) {
						pw.push_back(sta1.top());
						sta1.pop();
					}
					vector<vector<int>>vc1 = vc;
					for (auto it2 = vc1.begin(); it2 != vc1.end(); it2++) {
						vector<int>pw1 = pw;
						pw1.insert(pw1.end(), it2->begin(), it2->end());
						vc.push_back(pw1);
					}
				}
				if (mp.contains(a)) {
					auto it1 = mp.equal_range(a);
					for (auto it2 = it1.first; it2 != it1.second; it2++)
					{
						int b = it2->second;
						sta.push(b);
					}
				}
			}
		}
	}

	int changeIToO(Type type, int index) {
		if (type == PP) {
			return index - 1;
		}
		else if (type == TT) {
			return index - pnum - 1;
		}
		else {
			cout << "change index error";
			return-1;
		}
	}
};

void ACCUPpw(vector<vector<int>>& vc, vector<double>& Ppw, Petri pe, double pt[]) {
	for (auto& i : vc)
	{
		double a = 0;
		for (auto it1 = i.begin(); it1 != i.end(); it1++) {
			if ((*it1 > pe.pnum + 1) && (*it1 <= pe.pnum + pe.tnum + 1)) {
				if (a == 0) {
					a = pt[pe.changeIToO(TT, *it1) - 1];
				}
				else {
					a*= pt[pe.changeIToO(TT, *it1) - 1];
				}
			}
		}
		Ppw.push_back(a);
	}
}

void ACCURpw(vector<vector<int>>& vc ,Petri pe, vector<double>& Rpw) {
	for (auto& i : vc)
	{
		double rc = 1, rl = 1, rt = 1, result = 1;
		for (auto it1 = i.begin(); it1 != i.end(); it1++) {
			if ((*it1 >= 2) && (*it1 <= pe.pnum + 1)) {
				rc *= pe.node[*it1 - 1].rc;
			}
			else if ((*it1 > pe.pnum + 1) && (*it1 <= pe.pnum + pe.tnum + 1)) {
				rl *= pe.node[*it1 - 1].rl;
				rt *= pe.node[*it1 - 1].rt;
			}
		}
		result = rc * rl * rt;
		Rpw.push_back(result);
	}
}

double ACCURsa(vector<double> Ppw, vector<double> Rpw) {
	double sum1 = 0;
	for (auto& i : Ppw)
	{
		sum1 += i;
	}
	double sum2 = 0;
	for (auto it1 = Ppw.begin(), it2 = Rpw.begin(); it1 != Ppw.end(); it1++, it2++) {
		sum2 += (*it1) * (*it2);
	}
	return sum2 / sum1;
}

//输出各可靠度与概率
void print(vector<double> Ppw, vector<double> Rpw, double rsa) {
	//测试路径的迁移概率
	cout << "==========各路径迁移概率==========" << endl;
	auto it2 = Ppw.begin();
	for (size_t i = 0; i < Ppw.size(); i++)
	{
		cout << "pw" << i + 1 << "的迁移概率为：" << *(it2 + i) << endl;
	}
	//测试路径的可靠度
	cout << "==========各路径可靠度==========" << endl;
	auto it1 = Rpw.begin();
	for (size_t i = 0; i < Rpw.size(); i++)
	{
		cout << "pw" << i + 1 << "的可靠度为：" << *(it1 + i) << endl;
	}
	//输出系统可靠度
	cout << "==========系统可靠度==========" << endl;
	cout << "系统SA的可靠度为：" << rsa << endl;
}

int main() {
	Petri petri;
	vector<vector<int>> vc;
	petri.start();
	petri.startLine();
	petri.SPetri(vc);

	vector<double>Ppw;
	double pt[NUM];
	for (size_t i = 0; i < petri.tnum; i++)
	{
		cin >> pt[i];
	}

	int cccc = 1;
	cout << "输出pw路径：" << endl;
	for (auto& i : vc)
	{
		cout << "pw" << cccc++ << ":";
		for (auto it = i.begin(); it != i.end(); it++) {
			if (*it == 1) {
				cout << "S";
			}
			else if ((*it == petri.num)) {
				cout << "->EN" << endl;
			}
			else if ((*it >= 2) && (*it <= petri.pnum + 1)) {
				cout << "->C" << petri.changeIToO(PP, *it);
			}
			else if ((*it > petri.pnum + 1) && (*it <= petri.pnum + petri.tnum + 1)) {
				cout << "->T" << petri.changeIToO(TT, *it);
			}
			else {
				cout << "pw output error";
			}
		}
	}
	
	//计算迁移概率
	ACCUPpw(vc, Ppw, petri, pt);
	
	//计算路径可靠性
	vector<double> Rpw;
	ACCURpw(vc, petri, Rpw);

	//计算系统可靠性
	double Rsa = 0;
	Rsa = ACCURsa(Ppw, Rpw);
	print(Ppw, Rpw, Rsa);
	return 0;
}
/*
总P类节点数 总T类节点数
9 10
类型 序列 rc rl rt
0
1 1 1
1 2 0.99
1 3 0.98
1 4 1
1 5 0.99
1 6 0.99
1 7 1
1 8 0.98
1 9 1
2 1 0.99 1
2 2 1 0.99
2 3 1 1
2 4 0.98 0.98
2 5 1 0.99
2 6 0.99 1
2 7 0.99 0.98
2 8 1 0.98
2 9 0.98 0.99
2 10 1 1
3
边数目
22
节点 边
1 2
2 11
3 12
4 13
5 14
5 20
6 15
7 16
8 17
8 19
9 18
10 21
11 3
12 4
13 5
14 6
15 7
16 8
17 9
18 3
19 10
20 10
路径概率
1 0.99 0.98 0.80 1 1 0.3 0.98 0.98 0.20



9 10
0
1 1 1
1 2 0.99
1 3 0.98
1 4 1
1 5 0.99
1 6 0.99
1 7 1
1 8 0.98
1 9 1
2 1 0.99 1
2 2 1 0.99
2 3 1 1
2 4 0.98 0.98
2 5 1 0.99
2 6 0.99 1
2 7 0.99 0.98
2 8 1 0.98
2 9 0.98 0.99
2 10 1 1
3
22
1 2
2 11
3 12
4 13
5 14
5 20
6 15
7 16
8 17
8 19
9 18
10 21
11 3
12 4
13 5
14 6
15 7
16 8
17 9
18 3
19 10
20 10
1 0.99 0.98 0.80 1 1 0.3 0.98 0.98 0.20
*/