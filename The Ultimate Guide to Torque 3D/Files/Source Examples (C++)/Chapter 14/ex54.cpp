/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex54.cpp
**/
#include <iostream>
using namespace std;

namespace Tools {
	namespace Math {
		int addEm(int a, int b) {
			return a + b;
		}
	};
	namespace Str {
		const char *getName() {
			return "Math Program";
		}
	};
};

void main() {
	const char *progName = Tools::Str::getName();
	int result = Tools::Math::addEm(10, 20);

	cout << "Program " << progName << " says 10 + 20 is " << result << endl;
}