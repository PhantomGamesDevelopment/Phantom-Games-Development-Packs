/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
catch-throw-try_example.cpp
**/
#include <iostream>
using namespace std;
void main() {
	try {
		throw 15;
	}
	catch (int eNum) {
		cout << "Caught Error: " << eNum;
	}
}
