/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex12.cpp
**/
#include "ex12.h"

void main() {
	cout << "Multiple Inheritance Example..." << endl;
	Triangle t(10, 20);
	t.out();
}

//Printer Class
void Printer::print(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
}

//Poly Class
int Poly::fetchArea() {
	return width * height;
}

//Triangle Class
int Triangle::fetchArea() {
	return (int)(0.5 * width * height);
}

void Triangle::out() {
	print("The area of this triangle (%i, %i) is: %i.\n", width, height, fetchArea());
}