/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex26.cpp
**/
#include <iostream>
using namespace std;

char *getText(char *);

void main() {
	char name[80];
	getText(name);
	cout << "You typed: " << name << endl;
}

char *getText(char *text) {
	cout << "Type your name: ";
	cin.getline(text, 80);
	return text;
}