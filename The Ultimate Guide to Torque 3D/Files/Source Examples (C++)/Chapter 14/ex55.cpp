/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex55.cpp
**/
#include <iostream>
using namespace std;

namespace Tools {
	namespace Str {
		const char *getName();
	};
	namespace Math {
		int addEm(int a, int b) {
			return a + b;
		}
		void doProgram() {
			int res = addEm(10, 20);
			const char *pName = ::Tools::Str::getName();
			cout << "Program " << pName << " says 10 + 20 is " << res << endl;
		}
	};
	namespace Str {
		const char *getName() {
			return "Math Program";
		}
	};
};

void main() {
	Tools::Math::doProgram();
}