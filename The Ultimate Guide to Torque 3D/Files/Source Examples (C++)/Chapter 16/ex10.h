/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex10.h
**/
#include <iostream>
using namespace std;

#ifndef _MYFILE_H_
#define _MYFILE_H_

	struct Numbers {
		int a;
		int b;
		int c;

		void set(int n1, int n2, int n3);
		void fetch(int n[3]);
	};

	class Numerics {
		public:
			Numerics() : one(1), two(2), three(3) { }
			~Numerics() { }

			void set(int, int, int);
			void operator<<(Numbers *o);
			Numerics &operator+=(int nums[3]);
		private:
			int one;
			int two;
			int three;
	};

#endif //_MYFILE_H_