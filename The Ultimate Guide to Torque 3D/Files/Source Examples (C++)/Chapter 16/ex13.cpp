/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex13.cpp
**/
#include "ex13.h"

void main() {
	cout << "Polymorphism Example..." << endl;
	Triangle t;
	Rect r;
	//Pointer-to-base
	Poly *p1 = &t;
	Poly *p2 = &r;
	p1->set(10, 20);
	p2->set(10, 20);
	cout << "Fetching Area..." << endl;
	cout << "Area of Triangle is: " << t.fetchArea() << endl;
	cout << "Area of Rectangle is: " << r.fetchArea() << endl;
}

//Poly Class
void Poly::set(int w, int h) {
	width = w;
	height = h;
}

//Triangle Class
int Triangle::fetchArea() {
	return (int)(0.5 * width * height);
}

//Rect Class
int Rect::fetchArea() {
	return width * height;
}