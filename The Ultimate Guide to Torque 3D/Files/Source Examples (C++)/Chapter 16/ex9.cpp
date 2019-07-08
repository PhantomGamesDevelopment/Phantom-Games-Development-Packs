/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex9.cpp
**/
#include "ex9.h"

void main() {
	int a1, b1, c1;
	Numerics a;
	Numerics b;

	a.set(10, 20, 30);
	b.set(40, 50, 60);

	a.fetch(&a1, &b1, &c1);
	cout << "Current A: " << a1 << ", " << b1 << ", " << c1 << endl;
	b.fetch(&a1, &b1, &c1);
	cout << "Current B: " << a1 << ", " << b1 << ", " << c1 << endl;
	//Make A = A + B
	a = a + b;
	a.fetch(&a1, &b1, &c1);
	cout << "Now A: " << a1 << ", " << b1 << ", " << c1 << endl;
	//Revert.
	a = a - b;
	a.fetch(&a1, &b1, &c1);
	cout << "Now A: " << a1 << ", " << b1 << ", " << c1 << endl;
}

//Numerics Class Definition
void Numerics::set(int a, int b, int c) {
	one = a;
	two = b;
	three = c;
}

void Numerics::fetch(int *a, int *b, int *c) {
	*a = one;
	*b = two;
	*c = three;
}

Numerics &Numerics::operator+(const Numerics &r) {
	one += r.one;
	two += r.two;
	three += r.three;

	return *(this);
}

Numerics &Numerics::operator-(const Numerics &r) {
	one -= r.one;
	two -= r.two;
	three -= r.three;

	return *(this);
}