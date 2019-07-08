/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex56.cpp
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
	using namespace Tools::Math;
	const char *progName = Tools::Str::getName();
	int result = addEm(10, 20);

	cout << "Program " << progName << " says 10 + 20 is " << result << endl;
}