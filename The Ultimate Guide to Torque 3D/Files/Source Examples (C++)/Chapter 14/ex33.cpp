/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex33.cpp
**/
#include <iostream>
using namespace std;

template <typename T1, typename T2, typename T3> T1 printIt(T1 v1, T2 *v2, T3 v3) {
	cout << "Printing: " << v1 << ", " << *v2 << ", " << v3 << endl;
	return v1;
}

void main() {
	int v1 = 10;
	double v2 = 5.5;
	const char *v3 = "Robert";

	int result = printIt<int, double, const char *>(v1, &v2, v3);
	cout << "Result: " << result << endl;
}