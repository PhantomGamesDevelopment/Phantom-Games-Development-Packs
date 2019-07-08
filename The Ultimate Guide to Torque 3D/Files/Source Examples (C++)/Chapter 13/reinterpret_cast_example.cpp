/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
reinterpret_cast_example.cpp
**/
#include <iostream>
using namespace std;
void main() {
	int* a = new int(27);
   void* b = reinterpret_cast<void*>(a);
   int* c = reinterpret_cast<int*>(b);
	cout << "A: " << (*a) << endl; 
	cout << "B: " << (b) << endl; 
	cout << "C: " << (*c) << endl;
}
