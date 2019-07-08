/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex24.cpp
**/
#include <iostream>
#include <stdlib.h> 
#include <time.h>
using namespace std;

void generateNumber(int *);

void main() {
	int myArray[10];
	cout << "Fetching an array from memory..." << endl;
	cout << "What did we get?" << endl;
	generateNumber(myArray);
	for(int i = 0; i < 10; i++) {
		cout << "Value #" << i+1 << ": " << myArray[i]
			<< " (" << &myArray[i] << ")" << endl;
	}
}

void generateNumber(int *arrayMe) {
	srand(time(0));
	for(int i = 0; i < 10; i++) {
		*(arrayMe + i) = rand() % 100 + 1;
	}
}