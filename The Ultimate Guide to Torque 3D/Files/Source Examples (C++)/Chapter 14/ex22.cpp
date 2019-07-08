/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex22.cpp
**/
#include <iostream>
using namespace std;

void main() {
	bool fullMatch = true;
	int *num1, *num2;
	int myArray1[] = {10, 20, 30, 40, 50};
	int myArray2[] = {10, 20, 30, 40, 50};

	num1 = myArray1;
	num2 = myArray2;
	cout << "Are they equal?" << endl;
	for(int i = 0; i < 5; i++) {
		if(*(num1 + i) == *(num2 + i)) {
			continue;
		}
		else {
			fullMatch = false;
			break;
		}
	}
	if(fullMatch) {
		cout << "Yes!" << endl;
	}
	else {
		cout << "No!" << endl;
	}
}