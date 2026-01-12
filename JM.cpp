#include<iostream>
#include<vector>
#include<array>
#include<cmath>
#include <iomanip>
using namespace std;

//用于求解参数a1
double f1(int arr[],int n) {
	double a = 0;
	double b = 0;
	double c = 0;
	for (size_t i = 0; i < n; i++)
	{
		a += arr[i];
		b += i * arr[i];
	}
	c = (1 / a) * b;
	return c;
}

//用于求解参数N0
int f2(int arr[], int n, double a1, double e0, vector<double>& vd) {
	double a;
	double n0;
	for (size_t i = 1; i < 2 * n; i++)
	{
		a = 0;
		n0 = i + 0.7734;
		for (size_t j = 0; j < n; j++)
		{
			a += 1 / (n0 - j);
		}
		a -= n / (n0 - a1);
		vd.push_back(a);
		if (fabs(a) <= e0) {
			return i;
		}
	}
	return 0;
}

int main() {
	//待求解参数
	int N0;
	double coefficient;
	//失效数据集
	int n = 34;
	int arr[34] = { 9,12,11,4,7,2,5,8,5,7,1,6,1,9,4,1,3,3,6,1,11,33,7,91,2,1,87,47,12,9,135,258,16,35 };
	vector<double> vd;
	//中间参数
	double a1 = f1(arr, n);
	//设置N0的误差
	double e0 = 0.02738;
	N0 = f2(arr, n, a1, e0, vd);
	cout << N0 << endl;
	vector<double>::iterator it = vd.begin();
	for (size_t i = 1; it != vd.end(); i++, it++) {
		cout << "N0为" << i << "时：" << *it << endl;
	}
	return 0;
}