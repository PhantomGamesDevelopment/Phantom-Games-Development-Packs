/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex6.cpp
**/
#include "ex6.h"

void main() {
	Container<int> myNumber(20);
	Container<char> myLetter('q');
	Container<char *> myWord("Hello");

	cout << "Contents Are: " << myNumber.fetch() << ", " << myLetter.fetch() << ", " << myWord.fetch() << endl;
	myNumber.decrement();
	myLetter.increment();
	myWord.increment();
	cout << "Contents Are Now: " << myNumber.fetch() << ", " << myLetter.fetch() << ", " << myWord.fetch() << endl;
}