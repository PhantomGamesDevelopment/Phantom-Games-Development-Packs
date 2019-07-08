/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
dynamic_cast_example.cpp
**/
#include <iostream>
using namespace std;
struct Base {
   int a;
};
struct Child : public Base {
	int a;
	int b;
};
void main() {
	Child *c = new Child();
	if(Base *b = dynamic_cast<Base*>(c)) {
	   cout << "Conversion Successful!" << endl;
	}
}
