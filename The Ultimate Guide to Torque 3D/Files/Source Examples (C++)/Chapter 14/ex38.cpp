/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex38.cpp
**/
#include <iostream>
#include <stdlib.h>
using namespace std;

void main() {
	cout << "Allocate Array" << endl;
	int myNums[] = {10, 20, 30, 40, 50};
	int newCount = 6;
	int *nums = NULL;
	int *newPtr = (int *)realloc(nums, newCount * sizeof(int));
	for(int i = 0; i < 5; i++) {
		*(newPtr + i) = myNums[i];
	}
	*(newPtr + 5) = 60;
	cout << "Print" << endl;
	for(int i = 0; i < 6; i++) {
		cout << i+1 << ": " << *(newPtr + i) << endl;
	}
}