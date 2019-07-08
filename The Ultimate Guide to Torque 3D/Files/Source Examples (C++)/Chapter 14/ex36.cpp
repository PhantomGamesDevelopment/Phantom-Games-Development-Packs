/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex36.cpp
**/
#include <iostream>
#include <stdlib.h>
using namespace std;

char *getText();

void main() {
	char *name = getText();
	cout << "You typed: " << name << endl;
}

char *getText() {
	cout << "Type your name: ";
	char entry[256];
	cin.getline(entry, 256);
	int len = strlen(entry);
	char *text = (char *)calloc(len, sizeof(char));
	if(text != NULL) {
		strcpy(text, entry);
		return text;
	}
	return "NULL PTR";
}