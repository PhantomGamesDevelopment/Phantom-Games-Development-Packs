/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
using_example.cpp
**/
#include <iostream>
using namespace std;
namespace MyCode {
	void HelloWorld() {
		cout << "Hello World!" << endl;
	}
};
void main() {
      using namespace MyCode;
	HelloWorld();
}
