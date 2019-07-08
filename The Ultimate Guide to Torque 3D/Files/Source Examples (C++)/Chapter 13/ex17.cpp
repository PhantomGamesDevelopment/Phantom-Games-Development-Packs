/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
ex17.cpp
**/
#include <iostream>
#include <stdlib.h> 
#include <time.h>
using namespace std;
void main() {
	srand(time(0));
	int random = rand() % 100 + 1;
	cout << "Random is " << random << endl;
	while(random < 90) {		
		random = rand() % 100 + 1;
		cout << "Random is " << random << endl;
	}
	cout << "All Done!" << endl;
}
