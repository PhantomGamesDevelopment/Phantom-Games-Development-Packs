/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex57.cpp
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
	int result = divideIt(10, 0);
	cout << "The division is: " << result << endl;
}