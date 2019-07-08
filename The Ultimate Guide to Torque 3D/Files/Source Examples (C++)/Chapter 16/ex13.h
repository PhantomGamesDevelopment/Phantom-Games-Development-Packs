/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex13.h
**/
#include <iostream>
using namespace std;

#ifndef _MYFILE_H_
#define _MYFILE_H_

	class Poly {
		public:
			Poly() : width(0), height(0) { }

			void set(int w, int h);
		protected:
			int width;
			int height;
	};

	class Triangle : public Poly {
		public:
			int fetchArea();
	};

	class Rect : public Poly {
		public:
			int fetchArea();
	};

#endif //_MYFILE_H_