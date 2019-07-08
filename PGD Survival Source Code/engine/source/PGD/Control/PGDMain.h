/**
Phantom Games Development MAIN control header
This file is the central "command and control" for all of the PGD project
source code details and information...
**/

//System build settings
#define _PGDAUTH 1 //DEFINE 1 to build with XXZ568 Package

//Project build settings	
#define _PGD_PGDSURV 1

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
   #include "PGD/Solutions/PGDCOMMOD.h" 
   #include "PGD/Solutions/PGDStore.h"
   #include "PGD/Solutions/portControl.h"
   #include "PGD/Solutions/tcpCurl.h"
#endif

#ifdef _PGD_PGDSURV
   #undef _LOADCCN
   #undef _LOADPGDTU0
   #define _PGD_WINDOWNAME "PGD Survival"

   #ifdef _LOADPGDSURVME
      #include "PGD/Solutions/survivalControl.h"
   #endif
   #ifdef _LOADSTORE
      #include "PGD/Solutions/PGDStore.h"
   #endif
   #ifdef _LOADPGDPCON
      #include "PGD/Solutions/portControl.h"   
   #endif
   #ifdef _LOADPGDCOMMOD
      #include "PGD/Solutions/PGDCOMMOD.h"   
   #endif
   #ifdef _LOADPGDTCPCON
      #include "PGD/Solutions/tcpCurl.h"
   #endif
#endif
  
/**
#undef _LOADAUTH
#undef _LOADCCN
#undef _LOADPGDSURVME
#undef _LOADPGDCOMMOD
#undef _LOADPGDPCON
#undef _LOADPGDSURV
**/