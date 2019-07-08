/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex40.cpp
**/
#include <iostream>
#include <stdlib.h>
using namespace std;

void main() {
	int *v1 = new int(27);
	int *v2 = new int[3];
	for(int i = 0; i < 3; i++) {
		*(v2 + i) = i * 10;
	}
	int *v3 = (int *)malloc(sizeof(int));
	*v3 = 10;
	cout << "Using the variables..." << endl;
	cout << "v1: " << *v1 << endl << "v2: ";
	for(int i = 0; i < 3; i++) {
	   cout << *(v2 + i) << " ";
	}
	cout << endl << "v3: " << *v3 << endl;
	cout << "Deleting/Freeing" << endl;
	delete v1;
	delete[] v2;
	free(v3);
	cout << "Complete..." << endl;
}