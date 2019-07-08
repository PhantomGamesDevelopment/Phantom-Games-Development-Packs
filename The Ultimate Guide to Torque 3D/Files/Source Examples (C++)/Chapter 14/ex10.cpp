/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex10.cpp
**/
#include <iostream>
using namespace std;

const int COLS = 4;
const int ROWS = 2;
int addEmUp(int [][COLS], int);

void main() {
	int values[ROWS][COLS] = {{10, 20, 30, 40}, {10, 20, 30, 40}};
	int result = addEmUp(values, ROWS);
	cout << "The Result is: " << result << endl;
}

int addEmUp(int nums[][COLS], int rows = ROWS) {
	int result = 0;
	for(int x = 0; x < rows; x++) {
		for(int y = 0; y < COLS; y++) {
			cout << "Adding " << nums[x][y] << " to the result..." << endl;
			result += nums[x][y];
		}
	}
	return result;
}