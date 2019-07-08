/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
enum_example.cpp
**/
#include <iostream>
using namespace std;
struct Base {
	enum Info {
		One = 1,
		Two = 2,
	};
};
void main() {
	cout << "Base One: " << Base::Info::One;
}
