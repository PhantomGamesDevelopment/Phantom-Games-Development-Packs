/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
ex5.cpp
**/
#include <iostream>
using namespace std;

int main(int argc, const char **argv) {
   for(int i = 0; i < argc; i++) {
      cout << "Parameter " << i << ": " << argv[i] << endl;
   }
   return 0;
}
