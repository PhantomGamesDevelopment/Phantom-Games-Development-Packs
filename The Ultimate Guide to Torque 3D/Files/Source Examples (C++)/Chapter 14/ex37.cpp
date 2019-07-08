/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex37.cpp
**/
#include <iostream>
using namespace std;

int *fetch();

void main() {
	int *v = fetch();
	if(v != NULL) {
		cout << "Got a pointer at 0x" << v << ", it has: " << *v << endl;
	}
}

int *fetch() {
	int *value = new int(27);
	if(value != NULL) {
		return value;
	}
	return NULL;
}