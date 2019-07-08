/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex12.cpp
**/
#include <iostream>
using namespace std;

void doubleMe(int &);

void main() {
	int number = 5;
	cout << "The Number is: " << number << endl;
	doubleMe(number);
	cout << "The Number is now: " << number << endl;
	cout << "We read and wrote to: 0x" << &number << endl;
}

void doubleMe(int &num) {
	num *= 2;
}