/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
ex11.cpp
**/
#include <iostream>
#include <stdlib.h> 
#include <time.h>
using namespace std;
void main() {
	srand(time(0));
	int random = rand() % 100 + 1;
	if(random > 25 && random < 75) {
		cout << "It's Between 25 and 75!" << endl;
	}
	else {
		cout << "Maybe Next Time!" << endl;
	}
}
