/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex18.cpp
**/
#include <iostream>
using namespace std;

void main() {
	int *num;
	int myArray[] = {10, 20, 30, 40, 50};

	num = myArray;
	cout << "The array is stored at: 0x" << num << endl;
	cout << "We're dereferencing value 2: " << num[2] << endl;
	cout << "Or we can do this: " << myArray[2] << " which is at: 0x" << &myArray[2] << endl;
}