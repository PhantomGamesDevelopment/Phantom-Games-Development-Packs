/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
public_example.cpp
**/
#include <iostream>
using namespace std;
class MyClass {
	public:
		int a;
};
void main() {
	MyClass mC;
   mC.a = 5;
	cout << "Value: " << mC.a << endl;
}
