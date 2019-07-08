/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
ex7.cpp
**/
#include <iostream>
using namespace std;

int addEm(int a, int b = 20) {
   return a + b;
}

void main() {
   int result = addEm(10);
   cout << "The Result Is: " << result; 
}
