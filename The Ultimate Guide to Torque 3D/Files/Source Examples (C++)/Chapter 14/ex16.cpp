/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex16.cpp
**/
#include <iostream>
using namespace std;

void main() {
	int *num;
	int myNumber = 27;

	num = &myNumber;
	int val = *num;
	cout << "Our pointer is at: 0x" << num << ", and the value is: " << val << endl;
}