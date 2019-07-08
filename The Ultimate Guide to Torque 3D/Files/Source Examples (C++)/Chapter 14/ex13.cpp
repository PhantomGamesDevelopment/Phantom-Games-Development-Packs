/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex13.cpp
**/
#include <iostream>
using namespace std;

void main() {
	int *num;
	int myNumber = 27;

	num = &myNumber;
	cout << "Number is: " << myNumber << " and it's stored at: 0x" << num << endl;
}