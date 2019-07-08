/**
The Ultimate Guide To Torque 3D
Chapter 15
By: Robert C Fritzen
MyNewEngineFile.cpp
**/
#include "platform/platform.h"
#include "console/console.h"
#include "console/consoleInternal.h"
#include "console/engineAPI.h"

//Ex1
DefineEngineFunction(helloWorld, void,,, "Prints out the Hello World!") {
	Con::printf("Hello World!");
}

//Ex2
DefineEngineFunction(echoClone, void, (const char *str, const char *fmt), (""), "We just rewrote echo, because reasons...") {
	Con::printf(str, fmt);
}

//Ex3
DefineEngineFunction(maxima, S32, (S32 a, S32 b), (0, 0), "[int, int] Returns the max number") {
	S32 maxima = a > b ? a : b;
	return maxima;
}

//Ex4
DefineEngineFunction(divideIt, S32, (S32 a, S32 b), (1, 1), "[int, int] Perform Division") {
	if(b == 0) {
		Con::errorf("divideIt(): Cannot perform division by zero, returning 0.");
		return 0;
	}
	return a / b;
}

//Ex5
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

const char *fetchMathResult(S32 a, S32 b) {
	using namespace Tools::Math;
	const char *progName = Tools::Str::getName();
	S32 result = addEm(a, b);

	UTF8 *str = new UTF8[256];
	dSprintf(str, 256, "%s says %i+%i is: %i", progName, a, b, result);
	return str;
}


DefineEngineFunction(fetchMathResult, const char *, (S32 a, S32 b), (1, 1), "[int, int] External function example") {
	const char *res = fetchMathResult(a, b);
	Con::printf("Result: %s", res);
	return res;
}

//Ex6
/*
Assume we have the following function:
   function doTSMath(%a, %b, %c) {
		return %a + %b + %c;
	}
*/
DefineEngineFunction(CallTSMath, S32, (S32 a, S32 b, S32 c), (0, 0, 0), "Calls a function inside the engine") {
	const char *result = Con::evaluatef("doTSMath(%i, %i, %i)", a, b, c);
	S32 intResult = dAtoi(result);
	return intResult;
}

//Ex7
/*
Assume we have the following function:
   function doDivisionTS(%a, %b) {
		if(%b == 0) {
			error("Cannot divide by zero.");
			return 0;
		}
		return %a / %b;
	}
*/
DefineEngineFunction(CallTSMath2, S32, (S32 a, S32 b), (1, 1), "Calls a function inside the engine") {
	const char *result = Con::evaluatef("doDivisionTS(%i, %i)", a, b);
	S32 intResult = dAtoi(result);
	if(intResult == 0) {
		Con::errorf("CallTSMath2(): Divide by zero error caught on C++, forwarding 0");
		return 0;
	}
	return intResult;
}

//Ex8
DefineEngineFunction(reprintName, const char *, (const char *name),, "Prints out the name created in local memory") {
	char *buffer = Con::getReturnBuffer(dStrlen(name) + 1);
	dStrcpy(buffer, name);
	Con::printf("The name is: %s", buffer);
	return buffer;
}

//Ex9
DefineEngineFunction(reprintNumber, int, (const char *name),, "Prints out the number created in local memory") {
	int *value = new int(1207);
	String numStr = String::ToString("%i", *value);
	char *buffer = Con::getReturnBuffer(numStr.length() + 1);
	dStrcpy(buffer, numStr.c_str());
	Con::printf("The Number is: %i", dAtoi(buffer));
	return dAtoi(buffer);
}
