/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex29.cpp
**/
#include <iostream>
using namespace std;

void main() {
	int v1;
	double v2;
	char v3;

	void *voidPtr;
	voidPtr = &v1;
	cout << "Address 1: 0x" << voidPtr << endl;
	voidPtr = &v2;
	cout << "Address 2: 0x" << voidPtr << endl;
	voidPtr = &v3;
	cout << "Address 3: 0x" << voidPtr << endl;
}