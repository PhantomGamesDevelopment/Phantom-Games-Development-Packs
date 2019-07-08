/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex21.cpp
**/
#include <iostream>
using namespace std;

void main() {
	int *num1, *num2;
	int myArray1[] = {10, 20, 30, 40, 50};
	int myArray2[] = {10, 20, 30, 40, 50};

	num1 = myArray1;
	num2 = myArray2;
	cout << "Are they equal?" << endl;
	cout << "num1: 0x" << num1 << ", num2: 0x" << num2 << endl;
	if(num1 == num2) {
		cout << "Yes!" << endl;
	}
	else {
		cout << "No!" << endl;
	}
}