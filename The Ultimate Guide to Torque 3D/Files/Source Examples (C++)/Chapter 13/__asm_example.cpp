/**
The Ultimate Guide To Torque 3D
Chapter 13
By: Robert C Fritzen
__asm_example.cpp
**/
#include <stdio.h>
char text[] = "Hello World";
char fmt[] = "%s\n";
void main() {
	__asm {
		lea eax,text
		push eax
		lea eax,fmt
		push eax
		call DWORD ptr printf
		pop ebx
		pop ebx
	}
}  
