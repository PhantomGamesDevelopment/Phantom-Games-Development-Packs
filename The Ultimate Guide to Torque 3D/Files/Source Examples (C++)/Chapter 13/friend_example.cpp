/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
friend_example.cpp
**/
#include <iostream>
using namespace std;
class B;
class A {
   friend B;
	private:
		int a;
	public:
		A() : a(1) {}
};
class B {	
	private:
		int a;
	public:
		B(A x) : a(x.a) { cout << "Stored: " << a; }
};
void main() {
	A a;
	B b(a);
}
