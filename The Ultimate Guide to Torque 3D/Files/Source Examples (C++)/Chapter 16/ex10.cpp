/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex10.cpp
**/
#include "ex10.h"

void main() {
	int alpha[3] = {5, 5, 5};
	int bravo[3];
	Numerics a;
	Numbers n;

	a.set(10, 20, 30);
	a += alpha;
	a << &n;

	n.fetch(bravo);

	cout << "Numbers: " << bravo[0] << ", " << bravo[1] << ", " << bravo[2] << endl;
}

//Numbers Struct Definition
void Numbers::set(int n1, int n2, int n3) {
	a = n1;
	b = n2;
	c = n3;
}

void Numbers::fetch(int n[3]) {
	n[0] = a;
	n[1] = b;
	n[2] = c;
}

//Numerics Class Definition
void Numerics::set(int a, int b, int c) {
	one = a;
	two = b;
	three = c;
}

void Numerics::operator<<(Numbers *o) {
	o->set(one, two, three);
}

Numerics &Numerics::operator+=(int n[3]) {
	one += n[0];
	two += n[1];
	three += n[2];

	return *(this);
}