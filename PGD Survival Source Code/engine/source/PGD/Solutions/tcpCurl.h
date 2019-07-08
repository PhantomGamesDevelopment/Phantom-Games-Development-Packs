#ifndef tcpcurl_H
#define tcpcurl_H

#  pragma comment ( lib, "libcurl" )
#  pragma comment ( lib, "Wldap32" )
#  pragma comment ( lib, "ws2_32" ) 
//#  pragma comment ( lib, "libcurl" )

#include <string>
#include <fstream>
#include <curl/curl.h>

#ifndef _CONSOLE_H_
   #include "console/console.h"
   #include "console/consoleInternal.h"
#endif

class tcpCurl {
   public:
      tcpCurl();
      ~tcpCurl();

		static void create();
		static void destroy();

		String readURL(const char *loc, const char *postData);
};
extern tcpCurl *tcpBP;

#endif