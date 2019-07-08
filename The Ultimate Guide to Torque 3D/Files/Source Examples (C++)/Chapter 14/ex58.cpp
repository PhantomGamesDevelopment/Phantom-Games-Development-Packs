/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex58.cpp
**/
#include <iostream>
using namespace std;

int divideIt(int num, int den) {
	if(den == 0) {
		throw "Divide By Zero";
	}
	return num / den;
}

void main() {
	try {
		int result = divideIt(10, 0);
		cout << "The division is: " << result << endl;
	}
	catch(char *exception) {
		cout << "The above operation failed: " << exception << endl;
	}
}