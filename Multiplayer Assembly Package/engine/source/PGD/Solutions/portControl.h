#ifndef pcontrol_H
#define pcontrol_H

#include <string>

#ifndef _CONSOLE_H_
   #include "console/console.h"
   #include "console/consoleInternal.h"
#endif
 
class portControl {
   public:
      static void create();
		static void destroy();

		portControl();
		~portControl();

		bool scanUDPOpen();
		//bool upnpOpenUDP();

		void setPort(int p) { port = p; }
		int getPort() { return port; }

   private:
      int port;
};

#endif

extern portControl *pcont;