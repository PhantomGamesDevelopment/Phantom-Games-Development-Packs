/**
Phantom Games Development MAIN control header
This file is the central "command and control" for all of the PGD project
source code details and information...
**/

#define _PGDSERVERQUERY_ACTIVE 1 

#define MasterServer "www.path/to/master/index.php"

#ifdef _LOADPGDPMASTER
   #include "PGD/Solutions/pgdServerQuery.h"
#endif