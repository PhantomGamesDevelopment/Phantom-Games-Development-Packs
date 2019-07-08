/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex30.cpp
**/
#include <iostream>
using namespace std;

void main() {
	int v1 = 10;
	double v2 = static_cast<double>(v1);
	double v3 = (double)v1;

	cout << "v1: " << v1 << ", v2: " << v2 << ", v3: " << v3 << endl;
}