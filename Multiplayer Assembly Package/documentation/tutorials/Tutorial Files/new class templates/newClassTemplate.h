#ifndef my_new_class_H
#define my_new_class_H

#ifndef _CONSOLE_H_
   #include "console/console.h"
   #include "console/consoleInternal.h"
#endif
 
class myNewClass {
   public:
      static void create();
	   static void destroy();

   	myNewClass();
   	~myNewClass();
   private:
};

#endif

extern myNewClass *mein;
