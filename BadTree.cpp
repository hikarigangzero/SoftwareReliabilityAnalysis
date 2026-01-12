#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<cmath>
#include<bitset>
#include<stack>
#include<array>
#include<queue>
using namespace std;

int num = 0;
int Num = 0;
double ti = 0;

class Node {
private:
	string name;
	int index;
	int priority = 0;
	double value = 0;

public:
	Node(int index, string name) {
		this->index = index;
		this->name = name;
	}

	Node(string name) {
		this->index = 0;
		this->name = name;
	}

	Node(int index, string name, int priority) {
		this->index = index;
		this->name = name;
		this->priority = priority;
	}

	Node(string name, int priority) {
		this->name = name;
		this->priority = priority;
	}

	Node(string name, int priority, double value) {
		this->name = name;
		this->priority = priority;
		this->value = value;
	}

	string getName() {
		return this->name;
	}

	int getIndex() {
		return this->index;
	}

	int getPriority() {
		return this->priority;
	}

	double getValue() {
		return this->value;
	}

	bool operator<(const Node& other) const {
		// 比较逻辑：先按 `name` 排序，如果相等，再按 `index` 排序
		if (name != other.name)
			return name < other.name;
		return index < other.index;
	}
};

//构建故障树
void construct(map<Node,map<Node,vector<Node>>> &mp, string &str, vector<Node>&vcm) {
	Node root("T");
	map<Node, vector<Node>> mp1;
	vector<string> vString;//储存割集
	
	//将割集进行分类开
	/*
	输入格式为 X1X2+X3X4+X5X4X1
	*/
	auto it1 = str.begin();
	while (it1!=str.end()) {
		//auto itHead = it1;
		string strBlock;
		while (it1!=str.end() && *it1 != '+') {
			//cout << *it1;
			strBlock.push_back(*it1);
			it1++;
		}
		vString.push_back(strBlock);
		if (it1 == str.end()) {
			break;
		}
		it1++;
		
	}

	//构建第一块
	auto it2 = vString.begin();
	int count = 1;
	for (; it2 != vString.end(); it2++)
	{
		auto i = 0;
		string a = *it2;
		vector<Node> linVec;
		while (true) {
			string firstValue = string(1, a[i]);
			i++;
			int secondValue = stoi(string(1, a[i]));
			firstValue.push_back(a[i]);
			Node linNode(secondValue, firstValue);
			linVec.push_back(linNode);
			i++;
			if (a[i] == '\0') {
				break;
			}
		}
		string str1 = "M";
		string str2 = to_string(count);
		str1.append(str2);
		vcm.push_back(Node(str1));
		mp1.insert(pair<Node, vector<Node>>(Node(str1), linVec));
		count++;
	}
	mp.insert(pair<Node,map<Node,vector<Node>>>(root, mp1));
}

//输出等效故障树
void print(map<Node, map<Node, vector<Node>>>& mp, vector<Node>& vcm) {
	cout << "等效故障树输出：" << endl;
	cout << "|--root+" << endl;
	auto mp1 = mp.at(Node("T"));
	auto it1 = vcm.begin();
	while (it1!=vcm.end()) {
		auto vcNode = mp1.at(*it1);
		cout << "|----" << (*it1).getName() << "*" << endl;
		auto it2 = vcNode.begin();
		for (; it2 != vcNode.end(); it2++) {
			cout << "|------" << (*it2).getName() << endl;
		}
		it1++;
	}
}

//储存表达式
void saveT(string str,vector<Node>&vc) {
	auto it1 = str.begin();
	while (it1 != str.end()) {
		while (it1 != str.end() && *it1 != '+') {
			string linstr;
			linstr.push_back(*it1);
			it1++;
			linstr.push_back(*it1);
			string linstr2;
			linstr2.push_back(*it1);
			int a = stoi(linstr2);
			it1++;
			Node linNode(a,linstr,0);
			vc.push_back(linNode);
			if (it1 == str.end() || *it1 == '+') {
				break;
			}
			Node linNode2("*", 2);
			vc.push_back(linNode2);
			/*if (it1 == str.end() || *it1 == '\0') {
				break;
			}*/
		}
		if (it1 == str.end()) {
			break;
		}
		if (*it1 == '+') {
			Node linNode3("+", 1);
			vc.push_back(linNode3);
			it1++;
		}
	}
}

//结构重要度相关
//初始化十进制真值表
void StartKey(int arr[], bool brr[]) {
	for (int i = 0; i < Num; i++)
	{
		arr[i] = i;
	}
	for (size_t i = 0; i < Num; i++)
	{
		brr[i] = false;
	}
}

//得到后缀表达式
void GetLater(vector<Node>& vc, vector<Node>& vcLater) {
	auto it1 = vc.begin();
	stack<Node> sta;
	for (auto it1 = vc.begin(); it1!=vc.end(); it1++)
	{
		if ((*it1).getPriority() == 0) {
			vcLater.push_back(*it1);
		}
		else {
			if (sta.empty() || sta.top().getPriority() < (*it1).getPriority()) {
				sta.push(*it1);
			}
			else {
				while (!sta.empty()) {
					if (sta.top().getPriority() >= (*it1).getPriority()) {
						vcLater.push_back(sta.top());
						sta.pop();
					}
					else {
						break;
					}
				}
				sta.push(*it1);
			}
		}
	}
	while (!sta.empty()) {
		vcLater.push_back(sta.top());
		sta.pop();
	}
}

//计算真值表输出并初始化二进制表
void OutputValue(int arr[], bool brr[], vector<string>&str1, vector<Node> vcLater) {
	//宽度为num
	for (int i = 0; i < Num; i++)
	{
		bitset<16> bs(arr[i]);
		string linBsStr = bs.to_string();
		string bsStr = linBsStr.substr(16-num, num);
		str1.push_back(bsStr);
		bool output = false;
		int result = 0;
		
		stack<int> sta;
		auto it1 = vcLater.begin();

		while (it1 != vcLater.end()) {
			if ((*it1).getPriority() == 0) {
				int a = (*it1).getIndex();
				a--;
				if (bsStr[a] == '0') {
					sta.push(0);
				}
				else {
					sta.push(1);
				}
			}
			else {
				int a = sta.top();
				sta.pop();
				int b = sta.top();
				sta.pop();
				if ((*it1).getPriority() == 1) {
					//+
					a = b + a;
					sta.push(a);
				}
				else if((*it1).getPriority() == 2){
					//*
					a = a * b;
					sta.push(a);
				}
				else {
					cout << "error" << endl;
				}
			}
			it1++;
		}
		result = sta.top();
		sta.pop();
		if (result != 0) {
			output = true;
		}
		brr[i] = output;
	}
}

int BASToInt(bool a, bool b) {
	if (a && !b) {
		return 1;
	}
	else if(a && b){
		return 0;
	}
	else if (!a && !b) {
		return 0;
	}
	else if (!a && b) {
		return -1;
	}
	else {
		cout << "BASToInt Error";
		return -2;
	}
}

//1.算结构重要度
void Important1(bool brr[], vector<string> str, double no1[]) {
	for (int i = 0; i < num; i++)
	{
		double result = 0;
		for (int j = 0; j < Num; j++) {
			string str1 = str.at(j);
			char ch1 = str1[i];
			if (ch1 == '1') {
				bool a = brr[j];
				int c = j - (int)pow(2, num - (i + 1));
				bool b = brr[c];
				result += BASToInt(a, b);
			}
			else {
				//不执行
			}
		}
		no1[i] = (1 / pow(2, num - 1)) * result;
	}
}

//一阶导数实现
void dNumf(vector<Node> str1, vector<Node>& vdNumc, int index1) {
	auto it1 = str1.begin();
	while (it1 != str1.end()) {
		queue<Node> que1;
		int a = 0;
		while ((*it1).getPriority() != 1) {
			if ((*it1).getPriority() == 0) {
				if ((*it1).getIndex() == index1) {
					if (a != 0) {
						que1.push(*it1);
					}
					a++;
				}
				else {
					que1.push(*it1);
				}
			}
			else if ((*it1).getPriority() == 2) {
				//跳过
			}
			else {
				cout << "一阶导数求解部分 error";
			}
			it1++;
			if (it1 == str1.end()) {
				break;
			}
		}
		Node linNode("number", 0, a);
		if (a != 0) {
			vdNumc.push_back(linNode);
			while (!que1.empty()) {
				vdNumc.push_back(Node("*", 2));
				vdNumc.push_back(que1.front());
				que1.pop();
			}
		}
		else {
			vdNumc.push_back(linNode);
		}
		if (it1 != str1.end()) {
			vdNumc.push_back(Node("+", 1));
			it1++;
		}
	}
}

void startdNum(vector<Node> vc, vector<Node> vdNumc[], vector<Node> vdNumcLater[]) {
	for (size_t i = 0; i < num; i++)
	{
		dNumf(vc, vdNumc[i], i + 1);
		GetLater(vdNumc[i], vdNumcLater[i]);
	}
}

//2.概率重要度
void Important2(double lam[], vector<Node> vdNumcLater[], double sanGi[]) {
	for (size_t i = 0; i < num; i++)
	{
		double result = 0;
		auto it1 = vdNumcLater[i].begin();
		stack<double>sta;
		while (it1 != vdNumcLater[i].end()) {
			if ((*it1).getPriority() == 0) {
				if ((*it1).getName() == "number") {
					sta.push((double)(*it1).getValue());
				}
				else {
					int a = (*it1).getIndex();
					a--;
					double b = lam[a] * ti;
					b = (double)0 - b;
					sta.push(1 - exp(b));
				}
			}
			else if ((*it1).getPriority() == 2) {
				//*
				double a = sta.top();
				sta.pop();
				double b = sta.top();
				sta.pop();
				a = b * a;
				sta.push(a);
			}
			else if ((*it1).getPriority() == 1) {
				//+
				double a = sta.top();
				sta.pop();
				double b = sta.top();
				sta.pop();
				a = b + a;
				sta.push(a);
			}
			else {
				cout << "概率重要度 error" << endl;
			}
			it1++;
		}
		result = sta.top();
		sta.pop();
		sanGi[i] = result;
	}
}

double TopEventP(vector<Node> vdLater, double lam[]) {
	double result = 0;
	auto it1 = vdLater.begin();
	stack<double>sta;
	while (it1 != vdLater.end()) {
		if ((*it1).getPriority() == 0) {
			int a = (*it1).getIndex();
			a--;
			double b = lam[a] * ti;
			b = (double)0 - b;
			sta.push(1 - exp(b));
		}
		else if ((*it1).getPriority() == 2) {
			//*
			double a = sta.top();
			sta.pop();
			double b = sta.top();
			sta.pop();
			a = b * a;
			sta.push(a);
		}
		else if ((*it1).getPriority() == 1) {
			//+
			double a = sta.top();
			sta.pop();
			double b = sta.top();
			sta.pop();
			a = b + a;
			sta.push(a);
		}
		else {
			cout << "顶事件计算 error" << endl;
		}
		it1++;
	}
	result = sta.top();
	sta.pop();
	return result;
}

//3.关键重要度
void Important3(double IiCR[], double tEvent, double lam[], double sanGi[]) {
	for (size_t i = 0; i < num; i++)
	{
		double result = 0;
		double Fi = 1 - exp(-lam[i] * ti);
		result = (Fi / tEvent) * sanGi[i];
		IiCR[i] = result;
	}
}

void printAll(map<Node, map<Node, vector<Node>>> mp, vector<Node> node,
	vector<Node> vc, vector<Node> vcLater, int mappingKey[],
	vector<string> mappingBKey, bool mappingValue[], double no1[],
	double sanGi[], double IiCR[], double tEvent) {
	cout << "==========故障树可视化部分==========" << endl;
	print(mp, node);
	cout << endl;

	cout << "==========表达式部分==========" << endl;
	cout << "分割得到的表达式：" << endl;
	for (auto it1 = vc.begin(); it1 != vc.end(); it1++)
	{
		cout << (*it1).getName() << " ";
	}cout << endl;
	cout << "表达式的后缀表达式形式：" << endl;
	for (auto it1 = vcLater.begin(); it1 != vcLater.end(); it1++)
	{
		cout << (*it1).getName() << " ";
	}cout << endl;
	cout << endl;

	cout << "==========真值表部分==========" << endl;
	cout << "真值表输出：" << endl;
	for (size_t i = 0; i < Num; i++)
	{
		cout << mappingKey[i] << " ";
		cout << mappingBKey.at(i) << " ";
		cout << mappingValue[i] << endl;
	}
	cout << endl;

	cout << "==========结构重要度部分==========" << endl;
	cout << "结构重要度输出:" << endl;
	for (size_t i = 0; i < num; i++)
	{
		cout << "第" << i + 1 << "个组件的结构重要度为：" << no1[i] << endl;
	}
	cout << endl;

	cout << "==========概率重要度部分==========" << endl;
	cout << "概率重要度输出:" << endl;
	for (size_t i = 0; i < num; i++)
	{
		cout << "第" << i + 1 << "个组件的概率重要度为：" << sanGi[i] << endl;
	}
	cout << endl;

	cout << "==========关键重要度部分==========" << endl;
	cout << "顶事件概率：" << tEvent << endl;
	cout << "关键重要度输出:" << endl;
	for (size_t i = 0; i < num; i++)
	{
		cout << "第" << i + 1 << "个组件的关键重要度为：" << IiCR[i] << endl;
	}
}

int main() {
	//输入表达式
	string str;
	cin >> str;
	cin >> num;

	//输入各组件参数oi
	double lam[16];
	for (size_t i = 0; i < num; i++)
	{
		cin >> lam[i];
	}
	cin >> ti;
	
	//故障树结构
	vector<Node> node;
	map<Node, map<Node, vector<Node>>> mp;
	//表达式结构
	vector<Node> vc;
	//后缀表达式
	vector<Node> vcLater;
	int NodeNum = 0;

	//建故障树
	construct(mp, str, node);

	//分割表达式并储存
	saveT(str, vc);
	
	//得到表达式十进制真值表对照表
	Num = (int)pow(2, num);
	int mappingKey[65535];//十进制
	vector<string> mappingBKey;//二进制
	bool mappingValue[65535];//真值表输出结果
	StartKey(mappingKey, mappingValue);

	//得到后缀表达式
	GetLater(vc, vcLater);
	OutputValue(mappingKey, mappingValue, mappingBKey, vcLater);

	//计算得出结构重要度
	double no1[16];
	Important1(mappingValue, mappingBKey, no1);
	

	//计算概率重要度
	//此处的顶事件概率采用近视计算
	//一阶导数表达式
	vector<Node> vdNumc[16];

	//一阶导数后缀表达式
	vector<Node> vdNumcLater[16];
	double sanGi[16];
	startdNum(vc, vdNumc, vdNumcLater);
	Important2(lam, vdNumcLater, sanGi);

	//计算关键重要度
	//顶事件概率
	double tEvent = 0;
	tEvent = TopEventP(vcLater, lam);
	
	//关键重要度
	double IiCR[16];
	Important3(IiCR, tEvent, lam, sanGi);

	printAll(mp, node, vc, vcLater, mappingKey, mappingBKey, mappingValue, no1, sanGi, IiCR, tEvent);
	return 0;
}
/*测试数据
*输入表达式：
* 	X1X2+X3X4+X5X4X1
*输入组件数目：
*	5
*输入各组件参数oi
*	0.001 0.002 0.003 0.004 0.005
*输入时间t
*	100
*/

/*
X1X2+X3X4+X5X4X1
5
0.001 0.002 0.003 0.004 0.005
100

X1+X2X3
3
0.001 0.002 0.003
100
*/