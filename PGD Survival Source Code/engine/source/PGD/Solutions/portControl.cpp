/**
portControl.cpp
Written By: Phantom139

Copywrite 2012, Phantom Games Development

**/
#include "PGD/Solutions/portControl.h"
#include <windows.h>  

#include "platform/platform.h"
#include "console/console.h"
#include "console/consoleInternal.h"
#include "console/engineAPI.h"

#pragma comment(lib,"Wsock32.lib") 

portControl *pcont = NULL;

void portControl::create() {
	if(pcont == NULL) {
	   pcont = new portControl();
	}
}

void portControl::destroy() {
	if(pcont != NULL) {
	   delete pcont;
		pcont = NULL;
	}
}

portControl::portControl() {
   port = 0;
}

portControl::~portControl() {

}
//---------------------------------------------------------------------------------------------------------------------
bool portControl::scanUDPOpen() {
   WSADATA wsaData;
	//start WSA
	if(WSAStartup(0x0101, &wsaData) == 0) {
      struct sockaddr_in client;          
      int sock;  
     
      client.sin_family      = AF_INET;  
      client.sin_port        = htons(port);  
      client.sin_addr.s_addr = inet_addr("127.0.0.1");  //use local.
      
      sock = (int) socket(AF_INET, SOCK_DGRAM, 0);  
      int bindV = bind(sock, (SOCKADDR FAR *) &client,sizeof(SOCKADDR_IN)) == SOCKET_ERROR; 	   
		WSACleanup(); 

		return bindV;
	}
	WSACleanup(); 
	Con::errorf("portControl::ScanUDPOpen() - WSAStartup failed to initialize.");
	return false;
}
//--------------------------------------------------------------------------------------------------------------------
DefineEngineFunction(setPortScan_port, void, (const char* in),, "(void)set the port for scanning") {
   pcont->setPort(dAtoi(in));
	Con::setVariable("$Net::PortScanPort", in);
	//
	Con::printf("*** Port Scan Set to: %i", pcont->getPort());
}

DefineEngineFunction(portScan_scan, bool, (),, "(void)scan the set port") {
   bool pScan;
	pScan = pcont->scanUDPOpen();

	Con::printf("*** Scanning UDP %i", pcont->getPort());
	return pScan;
} 