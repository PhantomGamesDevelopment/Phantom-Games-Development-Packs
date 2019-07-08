/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex3.h
**/
#include <iostream>
using namespace std;

#ifndef _MYFILE_H_
#define _MYFILE_H_
	class Volume; //<- Predefine for Area

	class Area {
		public:
			Area();
			void assignFrom(int l, int w);
			void assignFrom(Volume v);
			int calculate();

			int fetchL() { return length; }
			int fetchW() { return width; }

		private:
			int length;
			int width;
	};

	class Volume {
		public:
			Volume();
			void assignFrom(int l, int w, int h);
			void assignFrom(Area a, int h);
			int calculate();

			int fetchL() { return length; }
			int fetchW() { return width; }
			int fetchH() { return height; }

		private:
			int length;
			int width;
			int height;
	};

#endif //_MYFILE_H_