/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex3.cpp
**/
#include "ex3.h"

void main() {
	cout << "Area & Volume Example" << endl;
	Area a;
	Volume v;
	a.assignFrom(10, 10);
	cout << "Area of " << a.fetchL() << "x" << a.fetchW() << " is: " << a.calculate() << endl;
	v.assignFrom(a, 10);
	cout << "Volume of " << v.fetchL() << "x" << v.fetchW() << "x" << v.fetchH() << " is: " << v.calculate() << endl;
	cout << "Adjust Volume..." << endl;
	v.assignFrom(5, 7, 9);
	cout << "Volume of " << v.fetchL() << "x" << v.fetchW() << "x" << v.fetchH() << " is: " << v.calculate() << endl;
	a.assignFrom(v);
	cout << "Area of " << a.fetchL() << "x" << a.fetchW() << " is: " << a.calculate() << endl;
}

// AREA
Area::Area() : length(0), width(0) { }

void Area::assignFrom(int l, int w) {
	length = l;
	width = w;
}

void Area::assignFrom(Volume v) {
	length = v.fetchL();
	width = v.fetchW();
}

int Area::calculate() {
	return length * width;
}

// VOLUME
Volume::Volume() : length(0), width(0), height(0) { }

void Volume::assignFrom(int l, int w, int h) {
	length = l;
	width = w;
	height = h;
}

void Volume::assignFrom(Area a, int h) {
	length = a.fetchL();
	width = a.fetchW();
	height = h;
}

int Volume::calculate() {
	return length * width * height;
}