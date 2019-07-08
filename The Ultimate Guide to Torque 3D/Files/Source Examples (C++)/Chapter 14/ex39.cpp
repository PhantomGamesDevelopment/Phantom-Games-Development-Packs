/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex39.cpp
**/
#include <iostream>
#include <stdlib.h>
using namespace std;

void main() {
	cout << "Allocate Array" << endl;
	int myNums[] = {10, 20, 30, 40, 50};
	int newSize = 10 * sizeof(int);
	int *nums = (int *)malloc(5 * sizeof(int));
	for(int i = 0; i < 5; i++) {
		*(nums + i) = myNums[i];
	}
	void **arrayPtr = (void **)(&nums);
	cout << "Safely Re-Allocating Array" << endl;
	*arrayPtr = *arrayPtr ? realloc(*arrayPtr, newSize) : malloc(newSize);
	nums = (int *)(*(arrayPtr));
	for(int i = 5; i < 10; i++) {
		*(nums + i) = (i+1)*10;
	}
	cout << "Print...." << endl;
	for(int i = 0; i < 10; i++) {
		cout << "Number " << i+1 << ": " << *(nums + i) << endl;
	}
}