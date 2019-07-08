/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
extern_example.cpp

NOTE: DO NOT COMPILE THIS FILE... THIS IS FOR DEMONSTRATION
 PURPOSES ONLY
**/
//FILE 1
#include <iostream>
using namespace std;
int gInt = 12;
//FILE 2
#include <iostream>
using namespace std;
void main() {
	extern int gInt;
	cout << "The Number Is: " << gInt;
}
