/**
The Ultimate Guide To Torque 3D
Chapter 15
By: Robert C Fritzen
ex4.cpp
**/
#include <iostream>
using namespace std;

#define maxima(a, b) a > b ? a : b

void main() {
	int one = 1;
	int two = 2;
	int max = maxima(one, two);
	cout << "The maxima of " << one << " and " << two << " is: " << max << endl;
}