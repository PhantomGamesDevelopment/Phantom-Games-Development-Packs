/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
explicit_example.cpp
**/
#include <iostream>
using namespace std;
struct Base {
	explicit Base(int a) {
		num = a;
	}

	int num;
};
void main() {
	Base b(1);
	cout << "Number: " << b.num;
}
