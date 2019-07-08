/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex8.cpp
**/
#include <iostream>
using namespace std;
void main() {
	int values[2][4] = {10, 20, 30, 40, 10, 20, 30, 40};
	int result = 0;
	for(int x = 0; x < 2; x++) {
		for(int y = 0; y < 4; y++) {
			cout << "Adding " << values[x][y] << " to the result..." << endl;
			result += values[x][y];
		}
	}
	cout << "The Result is: " << result << endl;
}