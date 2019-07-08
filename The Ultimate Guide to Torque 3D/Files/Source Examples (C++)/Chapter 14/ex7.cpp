/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex7.cpp
**/
#include <iostream>
using namespace std;
void main() {
	int values[4] = {10, 20, 30, 40};
	int result = 0;
	for(int i = 0; i < 4; i++) {
	   cout << "Adding " << values[i] << " to the result..." << endl;
	   result += values[i];
	}
	cout << "The Result is: " << result << endl;
}