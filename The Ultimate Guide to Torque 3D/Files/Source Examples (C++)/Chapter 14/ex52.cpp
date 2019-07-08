/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex52.cpp
**/
#include <iostream>
using namespace std;

namespace MyNamespace {
	int getNumber() {
		return 27;
	}
};

void main() {
	int num = MyNamespace::getNumber();
	cout << "Number is: " << num << endl;
}