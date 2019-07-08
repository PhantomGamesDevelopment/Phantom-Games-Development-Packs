/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
virtual_example.cpp
**/
#include <iostream>
using namespace std;
class Base {
	public:
		virtual void printme() {
			cout << "Hello!" << endl;
		}
};
class Derived : public Base {
	/* Uncomment for override effect.
	public:
		void printme() {
			cout << "Hello From Two!" << endl;
		}
	*/
};
void main() {
	Base b;
	b.printme();
	Derived d;
	d.printme();
}
