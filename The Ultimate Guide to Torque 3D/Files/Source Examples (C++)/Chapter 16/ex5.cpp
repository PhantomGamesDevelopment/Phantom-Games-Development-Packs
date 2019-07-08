/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex5.cpp
**/
#include "ex5.h"

void main() {
	Container<int> myNumber(20);
	Container<char> myLetter('q');

	cout << "Contents Are: " << myNumber.fetch() << ", " << myLetter.fetch() << endl;
	myNumber.decrement();
	myLetter.increment();
	cout << "Contents Are Now: " << myNumber.fetch() << ", " << myLetter.fetch() << endl;
}