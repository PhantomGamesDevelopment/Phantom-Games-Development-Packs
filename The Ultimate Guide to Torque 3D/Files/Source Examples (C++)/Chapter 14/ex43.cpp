/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex43.cpp
**/
#include <iostream>
using namespace std;

struct testStruct {
	int num1;
	int num2;
	int num3;

	void setThemUp();
};

void testStruct::setThemUp() {
	num1 = 10;
	num2 = 20;
	num3 = 30;
}

void main() {
	testStruct a;
	a.setThemUp();
	cout << "Numbers: " << a.num1 << ", " << a.num2 << ", " << a.num3 << endl;
}