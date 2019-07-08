/**
MAP Control Header
This file is the central "command and control" for all of the PGD project
source code details and information...
**/

//System build settings
#define _PGDAUTH 1 //DEFINE 1 to build with XXZ568 Package

//Project build settings	
#define _PGD_MYGAME 1

//NOTE NOTE NOTE: PLEASE COMPLETE THE FULL TUTORIAL REGARDING ENABLING THE PHP MASTER SERVER
//SOLUTION BEFORE ENABLING THIS PORTION OF CODE
//#define _PGDSERVERQUERY_ACTIVE 1 //Define this if using the new master server query system.

/**
Please do not modify anthing below this line!
This is the initiator handle, IE: it tells the files which headers to load
**/
#ifdef _PGDAUTH
   #ifdef _LOADAUTH
      #include "PGD/Crypto/cryptoPackage.h"
   #endif
#endif

#ifdef _LOADALL
   #include "PGD/Crypto/cryptoPackage.h"
   #include "PGD/Solutions/PGDStore.h"
   #include "PGD/Solutions/portControl.h"
   #include "PGD/Solutions/tcpCurl.h"
#endif

#ifdef _PGD_MYGAME
   //define MAP Module Loads here
   //#undef _PGD_OTHERGAME //example: call #undef on all of ther game defines here.
   //these are the three included "modules" with MAP, I recommend loading all three.
   #define MasterServer "www.path/to/master/index.php"

   #ifdef _LOADSTORE
      #include "PGD/Solutions/PGDStore.h"
   #endif
   #ifdef _LOADPGDPCON
      #include "PGD/Solutions/portControl.h"   
   #endif
   #ifdef _LOADPGDTCPCON
      #include "PGD/Solutions/tcpCurl.h"
   #endif
#endif

#ifdef _LOADPGDPMASTER
   #include "PGD/Solutions/pgdServerQuery.h"
#endif