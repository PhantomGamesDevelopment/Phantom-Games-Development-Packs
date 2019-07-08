/**
The Ultimate Guide To Torque 3D
Chapter 15
By: Robert C Fritzen
ex6.cpp
**/
#include <iostream>
using namespace std;

#define USE_MY_CODE
#ifdef USE_MY_CODE
	int GetMaxNumbers(int a, int b) {
		return a > b ? a : b;
	}
#else
	#error "Cannot compile without code"
#endif

void main() {
	int a = 1, b = 2;
	cout << "Max is: " << GetMaxNumbers(a, b) << endl;
}