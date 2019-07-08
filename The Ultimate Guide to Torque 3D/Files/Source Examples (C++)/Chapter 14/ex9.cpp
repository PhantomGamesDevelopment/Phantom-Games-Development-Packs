/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex9.cpp
**/
#include <iostream>
using namespace std;

int addEmUp(int [], int);

void main() {
	const int SIZE = 5;
	int numbers[SIZE] = {10, 20, 30, 40, 50};
	int result = addEmUp(numbers, SIZE);
	cout << "Added result is: " << result << endl;
}

int addEmUp(int nums[], int elements) {
	int total = 0;
	for(int i = 0; i < elements; i++) {
		total += nums[i];
	}
	return total;
}