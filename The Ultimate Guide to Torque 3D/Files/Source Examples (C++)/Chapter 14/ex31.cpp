/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex31.cpp
**/
#include <iostream>
using namespace std;

template <typename T> void printIt(T toPrint) {
	cout << "Printing: " << toPrint << endl;
}

void main() {
	int v1 = 10;
	double v2 = 5.5;
	const char *v3 = "Robert";

	printIt<int>(v1);
	printIt<double>(v2);
	printIt<const char *>(v3);
}