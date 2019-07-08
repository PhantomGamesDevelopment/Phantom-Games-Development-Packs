/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex35.cpp
**/
#include <iostream>
using namespace std;

char *getText();

void main() {
	char *name = getText();
	cout << "You typed: " << name << endl;
}

char *getText() {
	char *text = (char *)malloc(80);
	if(text != NULL) {
		cout << "Type your name: ";
		cin.getline(text, 80);
		return text;
	}
	return "NULL PTR";
}