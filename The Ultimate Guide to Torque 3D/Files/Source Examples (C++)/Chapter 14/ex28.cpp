/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex28.cpp
**/
#include <iostream>
using namespace std;

void main() {
	char n[][4] = {"You", "Me"};
	cout << "0: " << *n << ", 1: " << *(n+1) << endl;
	cout << "Double-Dereference: " << **(n+1) << endl;
	cout << "Fetching Second Letter: " << (*(n))[1] << endl;
}