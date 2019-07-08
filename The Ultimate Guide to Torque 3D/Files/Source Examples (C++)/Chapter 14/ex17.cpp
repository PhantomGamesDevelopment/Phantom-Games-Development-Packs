/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex17.cpp
**/
#include <iostream>
using namespace std;

void main() {
	int *num;
	int myNumber = 27;
	cout << "Created variable myNumber (" << myNumber << ") at: 0x" << &myNumber << endl;

	num = &myNumber;
	cout << "Pointed num (0x" << num << ") to myNumber" << endl;
	int val = *num;
	cout << "Created variable val at: 0x" << &val << " and dereferenced num: " << val << endl;
}