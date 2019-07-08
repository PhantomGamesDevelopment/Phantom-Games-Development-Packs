/**
tcpCurl.cpp
(c) Phantom Games Development, 2013
All Rights Reserved

tcpCurl:
Synopsis:
This class is a in-engine bypass to the TCPObject that uses the cURL library
**/

#include "PGD/Solutions/tcpCurl.h"

tcpCurl *tcpBP = NULL;
tcpCurl::tcpCurl() {
   curl_global_init(CURL_GLOBAL_ALL);
}

tcpCurl::~tcpCurl() {

}

void tcpCurl::create() {
   if(tcpBP == NULL) {
	   tcpBP = new tcpCurl();
		Con::printf("PGD Module Loaded: TCP Connector");
	}
}

void tcpCurl::destroy() {
	if(tcpBP != NULL) {
	   delete tcpBP;
		tcpBP = NULL;
		Con::printf("PGD Module Removed: TCP Connector");
	}
}

//---------------------------------------
//cURL memWrite Callback
int curlMemWrite(char *data, size_t size, size_t nmemb, std::string *buffer) {
   int result = 0;
   if (buffer != NULL) {
      buffer->append(data, size * nmemb);
      result = size * nmemb;
   }
   return result;
}

String tcpCurl::readURL(const char *loc, const char *postData) {
	std::string readMe;

   String buffer;
   
	CURL *curl;
   CURLcode result;

   curl = curl_easy_init();

   if(curl) {
      curl_easy_setopt(curl, CURLOPT_URL, loc);
      curl_easy_setopt(curl, CURLOPT_HEADER, 0);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlMemWrite);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readMe);

      result = curl_easy_perform(curl);

      curl_easy_cleanup(curl);

		if(result == CURLE_OK) {
			buffer = readMe.c_str();

         return buffer;
      }
		Con::errorf("error: %i: %s", result, curl_easy_strerror(result));
      return "";
   }
	Con::errorf("error: could not initalize curl");
   return "";   
}