/**
   PGDCOMMOD.h
	~Robert Fritzen
	~(c)2012 Phantom Games Development
**/

#ifndef _CONSOLE_H_
   #include "console/console.h"
   #include "console/consoleInternal.h"
#endif
#include <string>
using namespace std;


#ifndef PCM_H
#define PCM_H
 
class PCM {		   		   
	public:
      PCM();
		~PCM();

		static void create();
		static void destroy();

		std::string verifyCMCertificate(std::string data, std::string guid);
	   
   protected:
};
#endif

extern PCM *communityModerator;