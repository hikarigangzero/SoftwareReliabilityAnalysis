#include<iostream>
using namespace std;

//用于求解参数a1
double f1(int arr[], int n) {
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

double f2(int arr[], int n, double a1, double N) {
	double a = 0;
	for (size_t i = 0; i < n; i++)
	{
		a += 1 / (N - i);
	}
	a -= n / (N - a1);
	return a;
}

double JM(int arr[], int n, double a1 , double ex, double ey) {
	double left;
	double right;
	double root = 0;
	if (a1 > (n - 1) / 2) {
		left = n - 1;
		right = n;
		double f2right = 0;
		do {
			f2right = f2(arr, n, a1, right);
			if (fabs(f2right) <= ey) {
				root = right;
				return root;
			}
			else if (f2right > ey) {
				left = right;
				right++;
			}
			else {
				//不作操作
			}
		} while (f2right > ey);

		//下一步
		double abs2RL = 0;
		double f2Root = 0;
		do {
			abs2RL = fabs(right - left);
			if (abs2RL <= ex) {
				root = (right + left) / 2;
				return root;
			}
			else {
				root = (right + left) / 2;
				f2Root = f2(arr, n, a1, root);
				if (f2Root > ey) {
					left = root;
				}
				else if (fabs(f2Root) <= ey) {
					return root;
				}
				else {
					right = root;
				}
			}
		} while (true);
	}
	else {
		return root;
	}
}

double f3(int arr[], int n, double N0 ) {
	double a = 0;
	double b = 0;
	double c = 0;
	for (size_t i = 0; i < n; i++)
	{
		a += arr[i];
		b += i * arr[i];
	}
	c = n / (N0 * a - b);
	return c;
}

int main() {
	double Ex = 0.0001;
	double Ey = 0.0001;
	//待求解参数
	double N0;
	double coefficient;
	//失效数据集
	int n = 34;
	int arr[34] = { 9,12,11,4,7,2,5,8,5,7,1,6,1,9,4,1,3,3,6,1,11,33,7,91,2,1,87,47,12,9,135,258,16,35 };
	//int arr[26] = { 9,12,11,4,7,2,5,8,5,7,1,6,1,9,4,1,3,3,6,1,11,33,7,91,2,1 };
	//中间参数
	double a1 = f1(arr, n);
	//求解N0
	N0 = JM(arr, n, a1, Ex, Ey);
	coefficient = f3(arr, n, N0);
	cout << N0 << endl;
	cout << coefficient << endl;
	return 0;
}