/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
ex16.cpp
**/
#include <iostream>
#include <stdlib.h> 
#include <time.h>
using namespace std;
void main() {
	srand(time(0));
	int random = rand() % 100 + 1;
	int chances = 3;
	char strGuess[4];
	cout << "Guess The Number..." << endl;
	for(int i = 0; i < chances; i++) {
		cin.getline(strGuess, 4);
		if(atoi(strGuess) == random) {
			cout << "You Got It!" << endl;
			break;
		}
		else {
		   cout << "Nope..." << endl;
		}
	}
}
