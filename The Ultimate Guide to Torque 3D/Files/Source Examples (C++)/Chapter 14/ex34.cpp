/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex34.cpp
**/
#include <iostream>
using namespace std;

template <typename T> void passRef(void *);
void printIt(char *);
void printIt(int *);

void main() {
	char *myName[] = {"Robert"};
	int nums = 10;	

	void *ptr;

	ptr = myName;
	passRef<char**>(ptr);
	ptr = &nums;
	passRef<int*>(ptr);
}

template <typename T> void passRef(void *ptr) {
	T v = static_cast<T>(ptr);
	cout << "Printing: " << *v << endl;
}