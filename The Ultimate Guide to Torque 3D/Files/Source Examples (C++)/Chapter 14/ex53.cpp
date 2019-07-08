/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex53.cpp
**/
#include <iostream>
using namespace std;

namespace MyNamespace {
	int getNumber() {
		return 27;
	}
};
using namespace MyNamespace;

void main() {
	int num = getNumber();
	cout << "Number is: " << num << endl;
}