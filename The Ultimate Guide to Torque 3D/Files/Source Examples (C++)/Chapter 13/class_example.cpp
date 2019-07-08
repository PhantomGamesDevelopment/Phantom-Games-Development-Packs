/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
class_example.cpp
**/
#include <iostream>
using namespace std;
class myClass {
	public:
		void hi();
};
void myClass::hi() {
	cout << "Hi!" << endl;
}
void main() {
	myClass *c = new myClass();
	c->hi();
}
