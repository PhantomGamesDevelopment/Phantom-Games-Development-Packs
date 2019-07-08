/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex14.cpp
**/
#include "ex14.h"

void main() {
	cout << "Polymorphism Example..." << endl;
	//Pointer-to-base
	Poly *p1 = new Triangle();
	Poly *p2 = new Rect();
	Poly *p3 = new Poly();
	p1->set(10, 20);
	p2->set(10, 20);
	p3->set(10, 20);
	cout << "Fetching Area..." << endl;
	cout << "Area of Triangle is: " << p1->fetchArea() << endl;
	cout << "Area of Rectangle is: " << p2->fetchArea() << endl;
	cout << "Area of Poly P3 is: " << p3->fetchArea() << endl;
}

//Poly Class
void Poly::set(int w, int h) {
	width = w;
	height = h;
}

int Poly::fetchArea() {
	return 0;
}

//Triangle Class
int Triangle::fetchArea() {
	return (int)(0.5 * width * height);
}

//Rect Class
int Rect::fetchArea() {
	return width * height;
}