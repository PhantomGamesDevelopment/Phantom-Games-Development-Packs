/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex15.cpp
**/
#include <iostream>
using namespace std;

void printIt(char []);

void main() {
	char *myText = "Hello World";
	printIt(myText);
}

void printIt(char text[]) {
	int len = strlen(text);
	for(int i = 0; i < len; i++) {
		cout << "Char " << i+1 << ": " << text[i] << endl;
	}
}