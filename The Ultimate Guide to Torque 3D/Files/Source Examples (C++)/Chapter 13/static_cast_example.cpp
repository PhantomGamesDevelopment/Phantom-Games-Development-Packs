/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
static_cast_example.cpp
**/
#include <iostream>
using namespace std;
void main() {
	int number = 43;
	char c = static_cast<char>(number);
	cout << number << " is now: " << c << endl;
}
