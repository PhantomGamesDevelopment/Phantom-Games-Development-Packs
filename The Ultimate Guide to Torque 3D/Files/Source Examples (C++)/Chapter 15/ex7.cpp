/**
The Ultimate Guide To Torque 3D
Chapter 15
By: Robert C Fritzen
ex7.cpp
**/
#include <iostream>
using namespace std;

#define USE_MY_CODE
#define REVERSE
#if defined(USE_MY_CODE) && !defined(REVERSE)
	int GetMaxNumbers(int a, int b) {
		return a > b ? a : b;
	}
#elif defined(USE_MY_CODE) && defined(REVERSE)
	int GetMaxNumbers(int a, int b) {
		return a > b ? b : a;
	}
#else
	#error "Cannot compile without code"
#endif

void main() {
	int a = 1, b = 2;
	cout << "Max is: " << GetMaxNumbers(a, b) << endl;
}