/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex9.h
**/
#include <iostream>
using namespace std;

#ifndef _MYFILE_H_
#define _MYFILE_H_

	class Numerics {
		public:
			Numerics() : one(1), two(2), three(3) { }
			~Numerics() { }

			void set(int, int, int);
			void fetch(int *, int *, int*);

			Numerics &operator+(const Numerics &);
			Numerics &operator-(const Numerics &);
		private:
			int one;
			int two;
			int three;
	};

#endif //_MYFILE_H_