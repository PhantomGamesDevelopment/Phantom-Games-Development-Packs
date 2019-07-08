/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex59.cpp
**/
#include <iostream>
using namespace std;

int divideIt(int num, int den) {
	if(den == 0) {
		throw -1;
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
	catch(int eCode) {
		cout << "The above operation failed with error code: " << eCode << endl;
	}
}