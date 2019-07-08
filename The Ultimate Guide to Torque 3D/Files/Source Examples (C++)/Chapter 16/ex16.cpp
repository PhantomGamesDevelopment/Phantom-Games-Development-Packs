/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex16.cpp
**/
#include "ex16.h"

void main() {
	cout << "Abstract Class Example..." << endl;
	//Pointer-to-base
	Poly *p1 = new Triangle();
	Poly *p2 = new Rect();
	//Poly *p3 = new Poly(); //<- Illegal
	p1->set(10, 20);
	p2->set(10, 20);
	cout << "Fetching Area..." << endl;
	p1->printArea();
	p2->printArea();
}

//Poly Class
void Poly::set(int w, int h) {
	width = w;
	height = h;
}

void Poly::printArea() {
	cout << "Area of our instance is: " << this->fetchArea() << endl;
}

//Triangle Class
int Triangle::fetchArea() {
	return (int)(0.5 * width * height);
}

//Rect Class
int Rect::fetchArea() {
	return width * height;
}