#include<iostream>
#include<cmath>
using namespace std;

#define num 34

//初始化tN
int starttN(int arr[]) {
	int a = 0;
	for (size_t i = 0; i < num; i++)
	{
		a += arr[i];
	}
	return a;
}

//初始化tSum
int starttSum(int arr[]) {
	int a = 0;
	for (size_t i = 0; i < num; i++)
	{
		int b = 0;
		for (size_t j = 0; j <= i; j++) {
			b += arr[j];
		}
		a += b;
	}
	return a;
}

double GO(double D, double ox, double oy) {
	if ((D > 0) && (D < 0.5)) {
		double xl = (1 - 2 * D) / 2;
		double xr = 1 / D;
		while (true) {
			double xm = (xl + xr) / 2;
			if (fabs(xr - xl) <= ox) {
				return xm;
			}
			else {
				double f = (1 - D * xm) * exp(xm) + (D - 1) * xm - 1;
				if (f > oy) {
					xl = xm;
				}
				else if (f < -oy) {
					xr = xm;
				}
				else {
					return xm;
				}
			}
		}
		
	}
	else if ((D >= 0.5)) {
		cout << "None";
		return 0;
	}
	else {
		cout << "GO Error";
		return 0;
	}
}

int main() {
	//声明
	int T[num] = { 9,12,11,4,7,2,5,8,5,7,1,6,1,9,4,1,3,3,6,1,11,33,7,91,2,1,87,47,12,9,135,258,16,35 };
	double tSum = 0;
	double tN = 0;
	double N = 34;
	double a = 0, b = 0;
	double ox = 0.000001;
	double oy = 0.000001;

	//初始化
	tN = starttN(T);
	tSum = starttSum(T);
	cout << tN << " " << tSum << endl;
	double D = tSum / (N * tN);
	int xm = GO(D, ox, oy);
	b = xm / tN;
	a = N / (1 - exp(-b * tN));
	cout << a << endl;
	cout << b << endl;
	return 0;
}