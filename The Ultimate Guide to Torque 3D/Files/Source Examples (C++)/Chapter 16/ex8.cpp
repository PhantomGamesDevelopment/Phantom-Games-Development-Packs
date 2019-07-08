/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex8.cpp
**/
#include "ex8.h"

void main() {
	cout << "Init Templates..." << endl;
	Container<int, int, int, int> *c1 
		= new Container<int, int, int, int>(10, 20, 30, 40);
	Container<char *, double, int, char> *c2
		= new Container<char *, double, int, char>("Hello", 2.0, 10, 'r');
	cout << "Container 1: " << endl;
	cout << c1->fetch1() << ", " << c1->fetch2() << ", " << c1->fetch3() << ", " << c1->fetch4() << endl;
	cout << "Container 2: " << endl;
	cout << c2->fetch1() << ", " << c2->fetch2() << ", " << c2->fetch3() << ", " << c2->fetch4() << endl;
	delete c1;
	delete c2;
}

//Container functions
template <typename T1, typename T2> 
	Container<char *, T1, int, T2>::Container(char *v1, T1 v2, int v3, T2 v4) {
	e1 = v1;
	e2 = v2;
	e3 = v3;
	e4 = v4;
	cout << "Initialized using partial specialization template" << endl;
}

template <typename T1, typename T2> char *
	Container<char *, T1, int, T2>::fetch1() {
	return e1;
}

template <typename T1, typename T2> T1
	Container<char *, T1, int, T2>::fetch2() {
	return e2;
}

template <typename T1, typename T2> int
	Container<char *, T1, int, T2>::fetch3() {
	return e3;
}

template <typename T1, typename T2> T2
	Container<char *, T1, int, T2>::fetch4() {
	return e4;
}