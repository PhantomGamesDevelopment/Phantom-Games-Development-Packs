/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex1.cpp
**/
#include "ex2.h"
#include <iostream>
using namespace std;

void main() {
	myClass m;
	m.set(10, 20, 30);
	int *a = new int();
	int *b = new int(); 
	int *c = new int();
	m.fetch(a, b, c);
	cout << "Numbers: " << *a << ", " << *b << ", " << *c << endl;
}

myClass::myClass() : num1(0), num2(0), num3(0) {
}

myClass::~myClass() {
}

void myClass::set(int a, int b, int c) {
	num1 = a;
	num2 = b;
	num3 = c;
}

void myClass::fetch(int *a, int *b, int *c) {
	*a = num1;
	*b = num2;
	*c = num3;
}