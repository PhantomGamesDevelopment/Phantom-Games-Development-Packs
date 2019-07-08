/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex49.cpp
**/
#include <iostream>
#include <vector>
using namespace std;

void main() {
	std::vector<int> myVec;
	for(int i = 0; i <= 10; i++) {
		myVec.push_back(i * 10);
	}
	int total = 0, itC = 0;
	cout << "Adding up the vector" << endl;
	for(std::vector<int>::iterator it = myVec.begin(); it != myVec.end(); it++, itC++) {
		total += myVec[itC];
	}
	cout << "The total is: " << total << endl;
}