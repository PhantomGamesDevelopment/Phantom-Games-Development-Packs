/**
The Ultimate Guide To Torque 3D
Chapter 15
By: Robert C Fritzen
ex2.cpp
**/
#include <iostream>
using namespace std;

#define ARRAY_SIZE 10

void main() {
	int nums[ARRAY_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	for(int i = 0; i < ARRAY_SIZE; i++) {
		cout << "Number " << i << ": " << nums[i] << endl;
	}
}