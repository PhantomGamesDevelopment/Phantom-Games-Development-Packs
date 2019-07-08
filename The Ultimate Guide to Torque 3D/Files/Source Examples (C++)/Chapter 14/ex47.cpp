/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex47.cpp
**/
#include <iostream>
#include <list>
#include <stdlib.h> 
#include <time.h>
using namespace std;

void main() {
	srand(time(0));
	std::list<int> list1;
	int num, i;
	for(i = 0; i < 15; i++) {
		num = rand() % 1000 + 1;
		list1.push_front(num);
	}
	cout << "Inserted " << i << " numbers... reading..." << endl;
	for(std::list<int>::iterator it = list1.begin(); it != list1.end(); it++) {
		cout << "Read: " << *it << endl;
	}
}