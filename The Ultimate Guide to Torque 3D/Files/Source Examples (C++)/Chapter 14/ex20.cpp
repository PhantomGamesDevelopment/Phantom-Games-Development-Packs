/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex20.cpp
**/
#include <iostream>
using namespace std;

void main() {
	int *num;
	int myArray[] = {10, 20, 30, 40, 50};

	num = myArray;
	for(int i = 0; i < 5; i++) {
		cout << "Accessing memory at 0x" << &(*(num + i))
			<< ": " << *(num + i) << endl;
	}
}