/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex46.cpp
**/
#include <iostream>
using namespace std;

struct testStruct {
	int num1;
	int num2;
	int num3;

	testStruct(int, int, int);
	~testStruct();
	void fetch(int *, int *, int *);
};

testStruct::testStruct(int n1, int n2, int n3) : num1(n1), num2(n2), num3(n3) {
	cout << "I have been created..." << endl;
}

testStruct::~testStruct() {
	cout << "I have been deleted..." << endl;
}

void testStruct::fetch(int *a, int *b, int *c) {
	*a = num1;
	*b = num2;
	*c = num3;
}

void main() {
	testStruct *a = new testStruct(10, 20, 30);
	int a1;
	int b1;
	int c1;
	a->fetch(&a1, &b1, &c1);
	cout << "Numbers: " << a1 << ", " << b1 << ", " << c1 << endl;
	delete a;
}