/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
ex8.cpp
**/
#include <iostream>
using namespace std;

int addEm(int a, int b = 20);

void main() {
   cout << "The Result Is: " << addEm(10); 
}

int addEm(int a, int b = 20) {
   return a + b;
}
