/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex41.cpp
**/
#include <iostream>
using namespace std;

struct testStruct {
	int num1;
	int num2;
	int num3;
};

void main() {
	testStruct a;
	a.num1 = 10;
	a.num2 = 20;
	a.num3 = 30;

	cout << "The numbers are: " << a.num1 << ", " << a.num2 << ", " << a.num3 << endl;
}