/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex11.cpp
**/
#include "ex11.h"

void main() {
	cout << "Inheritance Example..." << endl;
	Poly p(10, 20);
	p.out();
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

void Poly::out() {
	print("The area of %i*%i is: %i\n", width, height, fetchArea());
}