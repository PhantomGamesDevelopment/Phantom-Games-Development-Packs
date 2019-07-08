#ifndef pgdstore_H
#define pgdstore_H

#define _LOADAUTH 1
#include "PGD/Control/PGDMain.h" 

#include <string>
#include <fstream>

#ifndef _CONSOLE_H_
   #include "console/console.h"
   #include "console/consoleInternal.h"
#endif

class PGDStore {
   public:
      PGDStore();
      ~PGDStore();

		static void create();
		static void destroy();

		void Add_PGS_Table(const char *n);

		struct _PGS_Value {
		   const char *name;
			std::string value;

			_PGS_Value *nic;

		   public:
		      _PGS_Value(const char *n);
				void set_value(std::string v);
				std::string get_value();
		};
		_PGS_Value *chain_head;
      _PGS_Value *getByName(const char *n);  

		std::string get_value(const char *n);
		bool set_value(const char *n, const char *nv);

		void saveToFile(const char *filePath);
		void loadFromFile(const char *filePath);
};
extern PGDStore *pgs_main;

#endif