/**
The Ultimate Guide To Torque 3D
Chapter 15
By: Robert C Fritzen
ex3.cpp
**/
#include <iostream>
using namespace std;

#define ARRAY_SIZE 10

void main() {
	int nums[ARRAY_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	#undef ARRAY_SIZE //<- ARRAY_SIZE no longer valid.
	int size = 10;
	for(int i = 0; i < size; i++) {
		cout << "Number " << i << ": " << nums[i] << endl;
	}
}