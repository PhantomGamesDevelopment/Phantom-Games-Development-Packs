#ifndef pgdquery_H
#define pgdquery_H

#include "PGD/Solutions/tcpCurl.h"

#include "platform/platform.h"
#include "platform/platformNet.h"
#include "core/dnet.h"
#include "core/util/tVector.h"
#include "console/console.h"
#include "console/consoleInternal.h"
#include "console/engineAPI.h"
#include "console/simBase.h"
#include "core/strings/stringUnit.h"
#include "core/stream/bitStream.h"
#include "app/banList.h"
#include "app/auth.h"
#include "sim/netConnection.h"
#include "sim/netInterface.h"
#include "T3D/gameBase/gameConnection.h"

//pgdServerQuery: PHP Based Master Server
//Global Defines
#define DefaultRetryCount 4
#define versionString "VER1"

//class definition
class pgdServerQuery {
	//Pre-Definitions
   public:
		struct filter;
		struct Ping;
		struct ServerInfo;

   public:
      pgdServerQuery();
      ~pgdServerQuery();

		static void create();
		static void destroy();

		void setMaster(String s) { masterServer = s; }
		String getMaster() { return masterServer; }

		void startHeartbeat();
		void stopHeartbeat();
		void sendHeartbeat(U32 sequence);

		void queryMasterServer();
		void queryLANServers(U32 port = 28000);
		void queryAllServers();
		void querySingleServer(const NetAddress *addr);

		void cancelServerQuery();

		void pushPingRequest(const NetAddress* addr);
		void pushPingBroadcast(const NetAddress* addr);
		bool addressFinished(const NetAddress* addr);
		S32 findPingEntry(Vector<Ping> &v, const NetAddress* addr);
      ServerInfo* findServerInfo(const NetAddress* addr);
      ServerInfo* findOrCreateServerInfo(const NetAddress* addr);
		void removeServerInfo(const NetAddress* addr);

		void processPingsAndQueries(U32 session, bool schedule = true);
		void updatePingProgress();
		void updateQueryProgress();
		S32 countPingRequests();

      void writeCString( BitStream* stream, const char* string );
      void readCString( BitStream* stream, char* buffer );
      void writeLongCString( BitStream* stream, const char* string );
      void readLongCString( BitStream* stream, char* buffer );

		filter *createFilterFromSettings(String settings);
		filter *defaultFilter;
   private:
		// <- Master Server, can be a DNS Name or IP.
		String masterServer;

		void sendPacket(U8 pType, const NetAddress* addr, U32 key, U32 session, U8 flags);
	//public: structure definitions
   public:
		// Structure to contain our filters
		struct filter {
			enum type {
			   Normal = 1,
				Buddies = 2,
				Offline = 3,
				Favorites = 4,
			};
			enum { //Query Type
			   OnlineQuery = 0,
				OfflineQuery = 1,
			};
			enum { //Filter Flags
            Dedicated = 1,
            NotPassworded = 2,
            Linux = 3,
            CurrentVersion = 4,
            NotXenon = 5,			   
			};

		   U32 minPlayers;
			U32 maxPlayers;
			U32 maxBots;
			U32 maxPing;
			U16 minCPU;
			U8 filterFlags;
			bool banned; //Probably don't want to show servers we're banned from.
			bool passworded;

			String mission;
			String missionType;
			String gameMode;

			type filterType;

			filter() {
			   filterType = Normal;
				filterFlags = 0;
				minPlayers = 0;
				maxPlayers = 0;
				maxBots = 0;
				maxPing = 0;
				minCPU = 0;
				passworded = false;
				banned = false;
			}
			~filter();
		};

		//Ping structure, used to send server info requests
      struct Ping {
         NetAddress address;
         S32 session;
         S32 key;
         U32 time;
         U32 tryCount;
         bool broadcast;
      };

		//This next section is essentially a copy/paste of the original serverInfo as it still functions
		//in a similar fashion to this system. With some needed optimizations of course.
      struct ServerInfo {
			//Phantom: Why were we using BIT(#) here??? No... no no.... this is much better.
         enum StatusFlags {
            // Server Info Flags:
            Status_Dedicated  = 0,
            Status_Linux      = 1,
            Status_Xenon      = 2,
            Status_Passworded = 3,
				Status_Banned     = 4,

            // Server Status flags:
            Status_New        = 5,
            Status_Querying   = 6,
            Status_Updating   = 7,
            Status_Responded  = 8,
            Status_TimedOut   = 9,
         };

         U8          numPlayers;
         U8          maxPlayers;
         U8          numBots;
         char*       name;
         char*       gameType;
         char*       missionName;
         char*       missionType;
         char*       statusString;
         char*       infoString;
         NetAddress  address;
         U32         version;
         U32         ping;
         U32         cpuSpeed;
         bool        isFavorite;
         U32         status;

         ServerInfo() {
            numPlayers = 0;
            maxPlayers = 0;
            numBots = 0;
            name = NULL;
            gameType = NULL;
            missionType = NULL;
            missionName = NULL;
            statusString = NULL;
            infoString = NULL;
            version = 0;
            ping = 0;
            cpuSpeed = 0;
            isFavorite = false;
            status = Status_New;
         }
			~ServerInfo() {
            if ( name )
               dFree( name );
            if ( gameType )
               dFree( gameType );
            if ( missionType )
               dFree( missionType );
            if( missionName )
               dFree( missionName );
            if ( statusString )
               dFree( statusString );
            if ( infoString )
               dFree( infoString );			
			}

         bool isNew()            { return( status == Status_New ); }
         bool isQuerying()       { return( status == Status_Querying ); }
         bool isUpdating()       { return( status == Status_Updating ); }
         bool hasResponded()     { return( status == Status_Responded ); }
         bool isTimedOut()       { return( status == Status_TimedOut ); }

         bool isDedicated()      { return( status == Status_Dedicated ); }
         bool isLinux()          { return( status == Status_Linux ); }
         bool isXenon()          { return( status == Status_Xenon ); }
         bool isPassworded()     { return( status == Status_Passworded ); }
         bool isBanned()         { return( status == Status_Banned ); }
      };
};
extern pgdServerQuery *pSQ;

extern Vector<pgdServerQuery::ServerInfo> gServerList;
extern bool gServerBrowserDirty;

#endif