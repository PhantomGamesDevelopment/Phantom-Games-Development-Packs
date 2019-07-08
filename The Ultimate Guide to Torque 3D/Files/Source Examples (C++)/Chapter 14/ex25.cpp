/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex25.cpp
**/
#include <iostream>
using namespace std;

void copyStr(const char *, char *);

void main() {
	char theName[80];
	copyStr("Robert", theName);
	cout << "Name: " << theName << endl;
}

void copyStr(const char *src, char *trg) {
	int len = strlen(src);
	for(int i = 0; i < len; i++) {
		trg[i] = src[i];
	}
	trg[len] = '\0';
}