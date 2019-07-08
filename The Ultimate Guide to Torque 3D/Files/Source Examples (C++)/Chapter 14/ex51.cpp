/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex51.cpp
**/
#include <iostream>
#include <vector>
#include <stdlib.h> 
#include <time.h>
using namespace std;

void main() {
	//Insert 30 numbers
	std::vector<int> myList;
	std::vector<int>::iterator it;
	for(int i = 0; i < 30; i++) {
		myList.push_back(rand() % 1000 + 1);
	}
	//Delete the first number
	myList.erase(myList.begin());
	//Delete the last number
	myList.pop_back();
	//Delete the 10th number
	it = myList.begin();
	advance(it, 9);
	myList.erase(it);
	cout << "Printing the list: ";
	for(it = myList.begin(); it != myList.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
}