/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
ex2.cpp
**/
#include <iostream>
using namespace std;

void main() {
   //Character Types
   char a = 'a';
   wchar_t b = 'b';
   //Numerical Types
   short c = 1;
   int d = 2;
   long e = 3;
   long long f = 4;
   float g = 1.0;
   double h = 2.0;
   long double i = 3.0;
   //Others
   bool j = true;
   /* Now that we've defined our types, let's print them out! */
   cout << "Character Types:" << endl << "A: " << a << ", " << sizeof(char) * 8 << endl;
   cout << "B: " << b << " [" << (char)b << "], " << sizeof(wchar_t) * 8 << endl;
   cout << "Numerical Types:" << endl << "C: " << c << ", " << sizeof(short) * 8 << endl;
   cout << "D: " << d << ", " << sizeof(int) * 8 << endl;
   cout << "E: " << e << ", " << sizeof(long) * 8 << endl;
   cout << "F: " << f << ", " << sizeof(long long) * 8 << endl;
   cout << "G: " << g << ", " << sizeof(float) * 8 << endl;
   cout << "H: " << h << ", " << sizeof(double) * 8 << endl;
   cout << "I: " << i << ", " << sizeof(long double) * 8 << endl;
   cout << "Other Types:" << endl << "J: " << j << ", " << sizeof(bool) * 8 << endl;
}
