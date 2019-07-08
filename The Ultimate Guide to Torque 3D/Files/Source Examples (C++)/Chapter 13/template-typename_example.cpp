/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
template-typename_example.cpp
**/
#include <iostream>
using namespace std;
template<typename T> class Z {
	public:
		Z(T t) {
			a = t;
		}
		T get() { return a; }
	private:
		T a;
};
void main() {
	Z<int> myClass1(15);
	Z<char> myClass2('a');
	cout << "Value of 1 is: " << myClass1.get() << endl;
	cout << "Value of 2 is: " << myClass2.get() << endl;
}
