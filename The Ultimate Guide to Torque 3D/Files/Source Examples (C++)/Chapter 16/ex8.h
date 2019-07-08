/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex8.h
**/
#include <iostream>
using namespace std;

#ifndef _MYFILE_H_
#define _MYFILE_H_

	template <typename T1, typename T2, typename T3, typename T4>
		class Container {
		public:
			Container(T1 v1, T2 v2, T3 v3, T4 v4) {
				e1 = v1;
				e2 = v2;
				e3 = v3;
				e4 = v4;
				printf("Initialized using generic format.\n");
			}

			~Container() {
				printf("Generic Poof!\n");
			}

			T1 fetch1() { return e1; }
			T2 fetch2() { return e2; }
			T3 fetch3() { return e3; }
			T4 fetch4() { return e4; }
		private:
			T1 e1;
			T2 e2;
			T3 e3;
			T4 e4;
	};

	template <typename T1, typename T2> 
		class Container<char *, T1, int, T2> {
		public:
			Container(char *v1, T1 v2, int v3, T2 v4);

			~Container() {
				printf("Partial Spec. Poof!\n");
			}

			char *fetch1();
			T1 fetch2();
			int fetch3();
			T2 fetch4();
		private:
			char *e1;
			T1 e2;
			int e3;
			T2 e4;
	};

#endif //_MYFILE_H_