/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex6.cpp
**/
#include <iostream>
using namespace std;
void main() {
	char names[][8] = {"Robert", "Tom"};
	cout << "Names: " << names[0] << ", " << names[1] << endl;
	cout << "Third letter of name 1: " << names[0][2] << endl;
}