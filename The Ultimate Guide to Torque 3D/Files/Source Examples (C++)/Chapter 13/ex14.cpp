/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
ex14.cpp
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
		case 2:	
		case 3:
			cout << "It's Less Than Or Equal To Three!" << endl;
			break;		
		case 4:		
		case 5:
			cout << "It's Greater Than Three!" << endl;
			break;		
		default:
			cout << "It's None Of Them!" << endl;
			break;
	}
}
