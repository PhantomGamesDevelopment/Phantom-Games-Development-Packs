/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex60.cpp
**/
#include <iostream>
using namespace std;

struct MyException {
	int errorCode;
	char *message;

	MyException(int a, char *c) : errorCode(a), message(c) { }
	const char *fetch() {
		char *lMsg = new char[256];
		sprintf(lMsg, "The program failed with error code %i (%s)", errorCode, message);
		return lMsg;
	}
};

int divideIt(int num, int den) {
	if(den == 0) {
		throw MyException(0, "Divide By Zero");	
	}
	return num / den;
}

void main() {
	try {
		int result = divideIt(10, 0);
		cout << "The division is: " << result << endl;
	}
	catch(MyException e) {
		cout << e.fetch() << endl;
	}
}