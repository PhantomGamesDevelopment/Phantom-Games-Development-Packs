/**
The Ultimate Guide To Torque 3D
Chapter 15
By: Robert C Fritzen
ex5.cpp
**/
#include <iostream>
using namespace std;

#define DefineTypedMax(type, name) \
	type GetMax##name(type a, type b) { \
	if(a >= b) { \
	   return a; \
	}\
	return b;\
}

DefineTypedMax(int, Numbers);

void main() {
	int a = 1, b = 2;
	cout << "Max is: " << GetMaxNumbers(a, b) << endl;
}