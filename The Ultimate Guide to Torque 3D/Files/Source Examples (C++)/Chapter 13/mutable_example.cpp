/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
mutable_example.cpp
**/
#include <iostream>
using namespace std;
class MyClass {
	public:
		const struct vals {
			int x;
			mutable int y;
			vals() : x(1), y(1) {}
		};
		vals x;
		MyClass() {}
};
void main() {
	MyClass c;
	c.x.y = 4;
	cout << "Values of class: " << c.x.x << ", " << c.x.y << endl;
}
