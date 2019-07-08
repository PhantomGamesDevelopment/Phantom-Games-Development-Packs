/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
struct_example.cpp
**/
#include <iostream>
using namespace std;
struct myClass {
	void hi();
};
void myClass::hi() {
	cout << "Hi!" << endl;
}
void main() {
	myClass *c = new myClass();
	c->hi();
}
