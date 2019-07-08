/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex19.cpp
**/
#include <iostream>
using namespace std;

void main() {
	int *num;
	int myArray[] = {10, 20, 30, 40, 50};

	num = myArray;
	cout << "Dereference num: " << *num << endl;
	cout << "Defeference myArray: " << *myArray << endl;
}