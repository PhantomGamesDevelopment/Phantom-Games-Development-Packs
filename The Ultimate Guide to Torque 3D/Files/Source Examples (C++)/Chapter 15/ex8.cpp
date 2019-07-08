/**
The Ultimate Guide To Torque 3D
Chapter 15
By: Robert C Fritzen
ex8.cpp
**/
#include <iostream>
using namespace std;

#define USE_MY_CODE
#define REVERSE
#ifdef USE_MY_CODE
	#ifdef REVERSE
		int GetMaxNumbers(int a, int b) {
			return a > b ? b : a;
		}
	#else
		int GetMaxNumbers(int a, int b) {
			return a > b ? a : b;
		}
	#endif
#else
	#error "Cannot compile without code"
#endif

void main() {
	int a = 1, b = 2;
	cout << "Max is: " << GetMaxNumbers(a, b) << endl;
}