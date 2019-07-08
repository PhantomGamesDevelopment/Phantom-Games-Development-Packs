/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
namespace_example.cpp
**/
#include <iostream>
using namespace std;
namespace MyCode {
	void HelloWorld() {
		cout << "Hello World!" << endl;
	}
};
void main() {
	MyCode::HelloWorld();
}
