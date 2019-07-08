/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
const_cast_example.cpp
**/
#include <iostream>
using namespace std;
void main() {
	const char *text = "Hello";
	char *newText;
	newText = const_cast<char*>(text);
	cout << newText << endl;
}
