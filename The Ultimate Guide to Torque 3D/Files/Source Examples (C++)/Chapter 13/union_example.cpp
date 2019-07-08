/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
union_example.cpp
**/
#include <iostream>
using namespace std;
union myUnion {
	int a;
	unsigned short b;
	unsigned char c;
};
void main() {
	myUnion u = {0xFFFFFF};
	cout << "A is active: " << u.a << endl;
	u.b = 0x0011;
	cout << "B is active: " << u.b << endl;
	cout << "C? " << u.c << " That is: " << (short)u.c << endl;
}
