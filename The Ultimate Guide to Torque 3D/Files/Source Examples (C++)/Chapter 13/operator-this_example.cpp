/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
operator-this_example.cpp
**/
#include <iostream>
using namespace std;
struct MyStruct {
	int num;
	MyStruct() : num(0) {}
	MyStruct& operator++() {
		num++;
		cout << "Number is now :" << num << endl;
		return *this;
	}
};
void main() {
	MyStruct s;
	for(int i = 0; i < 10; i++) {
		s++;
	}
}
