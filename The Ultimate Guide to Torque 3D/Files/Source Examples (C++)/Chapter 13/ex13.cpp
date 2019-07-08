/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
ex13.cpp
**/
#include <iostream>
#include <stdlib.h> 
#include <time.h>
using namespace std;
void main() {
	srand(time(0));
	int random = rand() % 5 + 1;
	switch(random) {
		case 1:
			cout << "It's One!" << endl;
			break;
		case 2:
			cout << "It's Two!" << endl;
			break;		
		case 3:
			cout << "It's Three!" << endl;
			break;		
		case 4:
			cout << "It's Four!" << endl;
			break;		
		case 5:
			cout << "It's Five!" << endl;
			break;		
		default:
			cout << "It's None Of Them!" << endl;
			break;
	}
}
