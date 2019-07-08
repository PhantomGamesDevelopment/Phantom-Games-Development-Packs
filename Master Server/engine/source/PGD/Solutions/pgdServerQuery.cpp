/**
pgdServerQuery.cpp
(c) Phantom Games Development, 2013
All Rights Reserved

pgdServerQuery:
Synopsis:
This class is a in-engine replacement of the server query system to function with PHP based solutions
**/

//****************
// - BEGIN CODE
//****************
//Includes & Defines
#include "PGD/Control/PGDMain.h"
#include "PGD/Solutions/pgdServerQuery.h"

#define DEBUGMODE 1

//Global Values
static S32 _gPingSession = 0;                                         //Session ID Number for Ping
static U32 _gHeartbeatSequence = 0;                                   //Identification number for simEvent
static U32 _gHeartbeatInterval = 150000;                              //Interval for heartbeats (in MS, 2-3 minutes is good)
static const S32 _gPingRetryCount = 4;                                //Number of ping retries
static const S32 _gPingTimeout = 800;                                 //Ping timeout in MS
static U32 _gServerPingCount = 0;                                     //Used by the GUI for progress meter
static U32 _gServerQueryCount = 0;                                    //See Above
static S32 _gKey = 0;                                                 //Key id for Net::Ping
bool gServerBrowserDirty = false;                                    //Boolean variable to notify GUI that an update is needed.
static bool _gGotFirstListPacket = false;                             //Query Test Variable to check is the first packet is the last
static const S32 _gMaxConcurrentPings = 10;                           //Maximum amount of allowed pings
static const S32 _gMaxConcurrentQueries = 2;                          //Maximum amount of allowed queries
static bool __sgServerQueryActive = false;                            //State variable: test if query is active
//Global Vectors
Vector<pgdServerQuery::ServerInfo> gServerList(__FILE__, __LINE__);  //List of servers before filtering
static Vector<NetAddress> _gFinishedList(__FILE__, __LINE__);         //Finished list, including actives and timed outs
static Vector<pgdServerQuery::Ping> _gPingList(__FILE__, __LINE__);   // <- Self Explanitory :)
static Vector<pgdServerQuery::Ping> _gQueryList(__FILE__, __LINE__);  // See above, but for active queries.

//----------------------------------------------------------------------------------------------------
//Constructor/Destructor
//----------------------------------------------------------------------------------------------------
pgdServerQuery *pSQ = NULL;
pgdServerQuery::pgdServerQuery() {
   setMaster(MasterServer);

	defaultFilter = NULL;
}

//----------------------------------------------------------------------------------------------------
pgdServerQuery::~pgdServerQuery() {

}

//----------------------------------------------------------------------------------------------------
void pgdServerQuery::create() {
   if(pSQ == NULL) {
	   pSQ = new pgdServerQuery();
		Con::printf("PGD Module Loaded: PGD Server Query");
	}
}

//----------------------------------------------------------------------------------------------------
void pgdServerQuery::destroy() {
	if(pSQ != NULL) {
	   delete pSQ;
		pSQ = NULL;
		Con::printf("PGD Module Removed: PGD Server Query");
	}
}

//----------------------------------------------------------------------------------------------------
//Internal & Inline Classes
//----------------------------------------------------------------------------------------------------
class ServerHeartbeatEvent : public SimEvent {
   U32 mSeq;
   public:
      ServerHeartbeatEvent(U32 seq) {
         mSeq = seq;
      }
      void process(SimObject *object) {
         pSQ->sendHeartbeat(mSeq);
      }
};

class ProcessPGDPingEvent : public SimEvent {
   U32 session;
   public:
      ProcessPGDPingEvent(U32 _session) {
         session = _session;
      }
      void process(SimObject *object) {
         pSQ->processPingsAndQueries(session);
      }
};

//----------------------------------------------------------------------------------------------------
//C++ Code
//----------------------------------------------------------------------------------------------------
void pgdServerQuery::sendPacket(U8 pType, const NetAddress* addr, U32 key, U32 session, U8 flags) {
   BitStream *out = BitStream::getPacketStream();
   out->clearStringBuffer();
   out->write(pType);
   out->write(flags);
   out->write(U32((session << 16) | (key & 0xFFFF)));

   char netString[256];
   Net::addressToString(addr, netString);
	//Con::printf("SendPacket to: %s", netString);

   BitStream::sendPacketStream(addr);
}

//----------------------------------------------------------------------------------------------------
void pgdServerQuery::writeCString(BitStream* stream, const char* string) {
   U8 strLen = (string != NULL) ? dStrlen(string) : 0;
   stream->write(strLen);
	for (U32 i = 0; i < strLen; i++) {
      stream->write(U8(string[i]));
	}
}

//----------------------------------------------------------------------------------------------------
void pgdServerQuery::readCString(BitStream* stream, char* buffer) {
   U32 i;
   U8 strLen;
   stream->read(&strLen);
   for (i = 0; i < strLen; i++) {
      U8* ptr = (U8*) buffer;
      stream->read(&ptr[i]);
   }
   buffer[i] = 0;
}

//----------------------------------------------------------------------------------------------------
void pgdServerQuery::writeLongCString(BitStream* stream, const char* string) {
   U16 strLen = (string != NULL) ? dStrlen(string) : 0;
   stream->write(strLen);
	for (U32 i = 0; i < strLen; i++) {
      stream->write(U8(string[i]));
	}
}

//----------------------------------------------------------------------------------------------------
void pgdServerQuery::readLongCString(BitStream* stream, char* buffer) {
   U32 i;
   U16 strLen;
   stream->read(&strLen);
   for (i = 0; i < strLen; i++) {
      U8* ptr = (U8*) buffer;
      stream->read(&ptr[i]);
   }
   buffer[i] = 0;
}

//----------------------------------------------------------------------------------------------------
void pgdServerQuery::startHeartbeat() {
	Con::printf("* Beginning heartbeat requests");
	_gHeartbeatSequence++;
	sendHeartbeat(_gHeartbeatSequence);
}

//----------------------------------------------------------------------------------------------------
void pgdServerQuery::stopHeartbeat() {
	Con::printf("* Stopping heartbeat requests");
	_gHeartbeatSequence = 0;
}

//----------------------------------------------------------------------------------------------------
void pgdServerQuery::sendHeartbeat(U32 sequence) {
	if(sequence != _gHeartbeatSequence) {
		if(_gHeartbeatSequence != 0) { //stopHeartbeat() sets to zero to purposefully trigger this.
		   Con::errorf("sendHeartbeat() - Sequence identifier not matching global value (This shouldn't happen)");
		}
      return;
	}
	Con::printf("* Sending heartbeat request to master server");
   //Send the data up to the server
   String payload = "mode=1";
	String result = tcpBP->readURL(getMaster().c_str(), payload.c_str());
   String FirstWord = StringUnit::getUnit( result, 0, " \t\n");
	
	if(FirstWord.compare("$PGDUPDATE") == 0 || FirstWord.compare("$PGDREGISTER") == 0) {
		Con::printf("* Heartbeat confirmed with master server.");
	}
	else {
		Con::errorf("* Hearbeat Error: Server Reply - %s", FirstWord.c_str());
	}
	//Post the next event
   Sim::postEvent( Sim::getRootGroup(), new ServerHeartbeatEvent(sequence), Sim::getCurrentTime() + _gHeartbeatInterval );
}

//----------------------------------------------------------------------------------------------------
void pgdServerQuery::queryMasterServer() {
	_gGotFirstListPacket = false;
   __sgServerQueryActive = true;

   Con::printf("* Querying master server");
   //Get the list of servers from the master
   String payload = "mode=2";
   String result = tcpBP->readURL(getMaster().c_str(), payload.c_str());
   #ifdef DEBUGMODE
      Con::printf("DEBUG: Master Server Query Response: \n%s", result.c_str());
   #endif;
   String FirstWord = StringUnit::getUnit( result, 0, " \t\n");
   NetAddress addr;
   if(FirstWord.compare("$PGD$LIST") != 0) {
      Con::errorf("* Master Server Error: %s", result.c_str());
	  return; //die :P
   }
   // - Clean the result string for IP reading
   result = result.replace("$", "");
   result = result.replace("PGDLIST ", "");
   // - Fetch the list of servers and begin ping requests
   String server;
   int serverListCount = StringUnit::getUnitCount(result, " \t\n");
   for(int i = 0; i < serverListCount; i++) {
      server = "IP:";
	  server += StringUnit::getUnit(result, i, " \t\n");
      #ifdef DEBUGMODE
	     Con::printf("DEBUG: Ping Server: %s", server.c_str());
      #endif
	  Net::stringToAddress(server.c_str(), &addr);
      pushPingRequest(&addr);
	  server = "";
   }
   processPingsAndQueries(_gPingSession);
}

//----------------------------------------------------------------------------------------------------
void pgdServerQuery::queryLANServers(U32 port) {
   __sgServerQueryActive = true;

   Con::printf("* Querying local servers");
	defaultFilter->filterType = filter::Offline;
	//clear the filter
	defaultFilter->mission = "Any";
	defaultFilter->gameMode = "Any";
	defaultFilter->filterFlags = 0;

   NetAddress addr;
   char addrText[256];
   dSprintf(addrText, sizeof(addrText), "IP:BROADCAST:%d", port);
   Net::stringToAddress(addrText, &addr);
   pushPingBroadcast(&addr);

   Con::executef("onServerQueryStatus", "start", "Querying LAN servers", "0");
   processPingsAndQueries(_gPingSession);
}

//----------------------------------------------------------------------------------------------------
void pgdServerQuery::queryAllServers() {
   queryMasterServer();
	queryLANServers();
}

//----------------------------------------------------------------------------------------------------
void pgdServerQuery::cancelServerQuery() {
   // Cancel the current query, if there is anything left
   // on the ping list, it's dropped.
   if (__sgServerQueryActive) {
      Con::printf( "Server query canceled." );
      ServerInfo* si;

      // Clear the ping list:
      while (_gPingList.size()) {
         si = findServerInfo(&_gPingList[0].address);
			if (si && (si->status != ServerInfo::Status_Responded)) {
            si->status = ServerInfo::Status_TimedOut;
			}
         _gPingList.erase( U32( 0 ) );
      }

      // Clear the query list:
      while (_gQueryList.size()) {
         si = findServerInfo(&_gQueryList[0].address);
			if (si && (si->status != ServerInfo::Status_Responded)) {
            si->status = ServerInfo::Status_TimedOut;
			}
         _gQueryList.erase( U32( 0 ) );
      }
      __sgServerQueryActive = false;
      gServerBrowserDirty = true;
   }
}

//----------------------------------------------------------------------------------------------------
S32 pgdServerQuery::findPingEntry(Vector<Ping> &v, const NetAddress* addr) {
	for (U32 i = 0; i < v.size(); i++) {
		if (Net::compareAddresses(addr, &v[i].address)) {
         return S32(i);
		}
	}
   return -1;
}

//----------------------------------------------------------------------------------------------------
bool pgdServerQuery::addressFinished(const NetAddress* addr) {
	for (U32 i = 0; i < _gFinishedList.size(); i++) {
		if (Net::compareAddresses( addr, &_gFinishedList[i])) {
         return true;
		}
	}
   return false;
}

//----------------------------------------------------------------------------------------------------
pgdServerQuery::ServerInfo* pgdServerQuery::findServerInfo(const NetAddress* addr) {
	for (U32 i = 0; i < gServerList.size(); i++) {
		if (Net::compareAddresses( addr, &gServerList[i].address)) {
         return &gServerList[i];
		}
	}
   return NULL;
}

//----------------------------------------------------------------------------------------------------
void pgdServerQuery::removeServerInfo(const NetAddress* addr) {
   for (U32 i = 0; i < gServerList.size(); i++) {
      if (Net::compareAddresses( addr, &gServerList[i].address)) {
         gServerList.erase( i );
         gServerBrowserDirty = true;
      }
   }
}

//----------------------------------------------------------------------------------------------------
pgdServerQuery::ServerInfo* pgdServerQuery::findOrCreateServerInfo(const NetAddress* addr) {
   ServerInfo* ret = findServerInfo(addr);
	if (ret) {
      return ret;
	}

   ServerInfo si;
   si.address = *addr;
   gServerList.push_back(si);

   return &gServerList.last();
}

//----------------------------------------------------------------------------------------------------
void pgdServerQuery::pushPingRequest(const NetAddress* addr) {
	if(addressFinished(addr)) {
      return;
	}

   Ping p;
   p.address = *addr;
   p.session = _gPingSession;
   p.key = 0;
   p.time = 0;
   p.tryCount = _gPingRetryCount;
   p.broadcast = false;
   _gPingList.push_back( p );
   _gServerPingCount++;
}

//----------------------------------------------------------------------------------------------------
void pgdServerQuery::pushPingBroadcast(const NetAddress* addr) {
	if(addressFinished(addr)) {
      return;
	}

   Ping p;
   p.address = *addr;
   p.session = _gPingSession;
   p.key = 0;
   p.time = 0;
   p.tryCount = 1; // only try this once
   p.broadcast = true;
   _gPingList.push_back( p );
   // Don't increment gServerPingCount, broadcasts are not
   // counted as requests.
}
//----------------------------------------------------------------------------------------------------
void pgdServerQuery::processPingsAndQueries(U32 session, bool schedule) {
	if(session != _gPingSession) {
      return;
	}
   U32 i = 0;
   U32 time = Platform::getVirtualMilliseconds();
   char addressString[256];
	U8 flags = filter::OnlineQuery;
   //bool waitingForMaster = (defaultFilter->filterType == filter::Normal) && !_gGotFirstListPacket && __sgServerQueryActive;
	//Con::printf("PPAQ: waitingForMaster: %i (%i, %i, %i)", waitingForMaster == true ? 1 : 0,
	//	(defaultFilter->filterType == filter::Normal) ? 1 : 0,
	//	!_gGotFirstListPacket ? 1 : 0,
	//	__sgServerQueryActive ? 1 : 0);

   for (i = 0; i < _gPingList.size() && i < _gMaxConcurrentPings;) {
      Ping &p = _gPingList[i];
      if (p.time + _gPingTimeout < time) {
         if (!p.tryCount) {
            // it's timed out.
            if (!p.broadcast) {
               Net::addressToString(&p.address, addressString);
               Con::printf("Ping to server %s timed out.", addressString);
            }
            // If server info is in list (favorite), set its status:
            ServerInfo* si = findServerInfo( &p.address );
            if (si) {
               si->status = ServerInfo::Status_TimedOut;
               gServerBrowserDirty = true;
            }

            _gFinishedList.push_back(p.address);
            _gPingList.erase(i);

				//if (!waitingForMaster) {
               updatePingProgress();
				//}
         }
         else {
            p.tryCount--;
            p.time = time;
            p.key = _gKey++;

            Net::addressToString(&p.address, addressString);

				if (p.broadcast) {
               Con::printf("LAN server ping: %s...", addressString);
				}
				else {
               Con::printf("Pinging Server %s (%d)...", addressString, p.tryCount);
				}
            sendPacket(NetInterface::GamePingRequest, &p.address, p.key, p.session, flags);
            i++;
         }
      }
		else {
         i++;
		}
   }
   if (!_gPingList.size() /* && !waitingForMaster*/) {
      // Start the query phase:
      for (U32 i = 0; i < _gQueryList.size() && i < _gMaxConcurrentQueries;) {
         Ping &p = _gQueryList[i];
         if (p.time + _gPingTimeout < time) {
            ServerInfo* si = findServerInfo(&p.address);
            if (!si) {
               // Server info not found, so remove the query:
               _gQueryList.erase(i);
               gServerBrowserDirty = true;
               continue;
            }
            Net::addressToString(&p.address, addressString);
            if (!p.tryCount) {
               Con::printf("Query to server %s timed out.", addressString);
               si->status = ServerInfo::Status_TimedOut;
               _gQueryList.erase(i);
               gServerBrowserDirty = true;
				}
            else {
               p.tryCount--;
               p.time = time;
               p.key = _gKey++;

               Con::printf("Querying Server %s (%d)...", addressString, p.tryCount);
               sendPacket(NetInterface::GameInfoRequest, &p.address, p.key, p.session, flags);
               if (!si->isQuerying()) {
                  si->status |= ServerInfo::Status_Querying;
                  gServerBrowserDirty = true;
               }
               i++;
            }
         }
			else {
            i++;
			}
      }
   }

   if (_gPingList.size() || _gQueryList.size() /*|| waitingForMaster*/) {
      // The LAN query function doesn't always want to schedule
      // the next ping.
		if (schedule) {
         Sim::postEvent(Sim::getRootGroup(), new ProcessPGDPingEvent(session), Sim::getTargetTime() + 1);
		}
   }
   else {
      // All done!
      char msg[64];
      U32 foundCount = gServerList.size();
      if (foundCount == 0)
         dStrcpy(msg, "No servers found.");
      else if (foundCount == 1)
         dStrcpy(msg, "One server found.");
      else
         dSprintf(msg, sizeof( msg ), "%d servers found.", foundCount);

      Con::executef("onServerQueryStatus", "done", msg, "1");
   }
}

//----------------------------------------------------------------------------------------------------
void pgdServerQuery::updatePingProgress() {
   if (!_gPingList.size()) {
      updateQueryProgress();
      return;
   }
   char msg[64];
   U32 pingsLeft = countPingRequests();
   dSprintf( msg, sizeof(msg),
      (!pingsLeft && _gPingList.size())?
         "Waiting for lan servers...":
         "Pinging servers: %d left...",
      pingsLeft );

   // Ping progress is 0 -> 0.5
   F32 progress = 0.0f;
	if (_gServerPingCount) {
      progress = F32(_gServerPingCount - pingsLeft) / F32(_gServerPingCount * 2);
	}
   Con::executef("onServerQueryStatus", "ping", msg, Con::getFloatArg(progress));
}

//----------------------------------------------------------------------------------------------------
void pgdServerQuery::updateQueryProgress() {
	if (_gPingList.size()) {
      return;
	}
   char msg[64];
   U32 queriesLeft = _gQueryList.size();
   dSprintf(msg, sizeof( msg ), "Querying servers: %d left...", queriesLeft);

   // Query progress is 0.5 -> 1
   F32 progress = 0.5f;
	if (_gServerQueryCount) {
      progress += (F32(_gServerQueryCount - queriesLeft) / F32(_gServerQueryCount * 2));
	}
   Con::executef("onServerQueryStatus", "query", msg, Con::getFloatArg(progress));
}

//----------------------------------------------------------------------------------------------------
S32 pgdServerQuery::countPingRequests() {
   // Need a function here because the ping list also includes
   // broadcast pings we don't want counted.
   U32 pSize = _gPingList.size(), count = pSize;
	for (U32 i = 0; i < pSize; i++) {
		if (_gPingList[i].broadcast) {
         count--;
		}
	}
   return count;
}

//----------------------------------------------------------------------------------------------------
pgdServerQuery::filter* pgdServerQuery::createFilterFromSettings(String s) {
	Con::printf("* Constructing ServerQuery Filter.");
   filter *f = new filter();
   String item;
	//Break the string
	//String Indicies: (Tab Indexed)
	//0: Filter Code (enum type)
	//1: Min Players
	//2: Max Players
	//3: Max Bots
	//4: Max Ping
	//5: Min CPU
	//6: Banned
	//7: Passworded
	//8: Specific Mission or any
	//9: Specific Game Mode or any
	int filterNameCount = StringUnit::getUnitCount(s, "\t\n");
	for(int i = 0; i < filterNameCount; i++) {
      item = StringUnit::getUnit(s, 0, "\t\n");
		switch(i) {
			case 0:
				switch(dAtoi(item.c_str())) {
			      case 1:
						f->filterType = filter::Normal;
			      case 2:
						f->filterType = filter::Buddies;
			      case 3:
						f->filterType = filter::Offline;
			      case 4:
						f->filterType = filter::Favorites;
				}

			case 1:
				f->minPlayers = dAtoui(item.c_str());
				Con::printf("** Min Player Count: %s", item.c_str());

			case 2:
				f->maxPlayers = dAtoui(item.c_str());
				Con::printf("** Max Player Count: %s", item.c_str());

			case 3:
				f->maxBots = dAtoui(item.c_str());
				Con::printf("** Max Bot Count: %s", item.c_str());

			case 4:
				f->maxPing = dAtoui(item.c_str());
				Con::printf("** Max Ping: %s", item.c_str());

			case 5:
				f->minCPU = dAtoi(item.c_str());
				Con::printf("** Min CPU: %s", item.c_str());

			case 6:
				if(dAtoi(item.c_str()) == 1) {
					f->banned = true;
				}
				else {
					f->banned = false;
				}
				Con::printf("** Filter Banned Servers: %s", item.c_str());

			case 7:
				if(dAtoi(item.c_str()) == 1) {
					f->passworded = true;
				}
				else {
					f->passworded = false;
				}
				Con::printf("** Filter Passworded Servers: %s", item.c_str());

			case 8:
				f->mission = item;
				Con::printf("** Required Mission: %s", item.c_str());

			case 9:
				f->gameMode = item;
				Con::printf("** Required Game Mode: %s", item.c_str());

			default:
				Con::errorf("** Unknown Item Index Gotten: %s", item.c_str());
		}
	}
	f->missionType = "Multiplayer";
	Con::printf("* ServerQuery Filter Constructed");
	return f;
}

//----------------------------------------------------------------------------------------------------
//NetInterface Definition
//----------------------------------------------------------------------------------------------------
class PGDNetInterface : public NetInterface {
   public:
      void handleInfoPacket(const NetAddress *address, U8 packetType, BitStream *stream);
};

PGDNetInterface gNetInterface;

static void handleGameMasterInfoRequest(const NetAddress* address, U32 key, U8 flags) {
   if(GNet->doesAllowConnections()) {
      U8 temp8;
      U32 temp32;

      char netString[256];
      Net::addressToString(address, netString);

      Con::printf("Received info request from [%s].", netString);

      BitStream *out = BitStream::getPacketStream();
      out->clearStringBuffer();

      out->write(U8(NetInterface::GameMasterInfoResponse));
      out->write(U8(flags));
      out->write(key);

      pSQ->writeCString(out, Con::getVariable("Server::GameType"));
      pSQ->writeCString(out, Con::getVariable("Server::MissionType"));
      temp8 = U8(Con::getIntVariable("pref::Server::MaxPlayers"));
      out->write(temp8);
      temp32 = Con::getIntVariable("pref::Server::RegionMask");
      out->write(temp32);
      temp32 = getVersionNumber();
      out->write(temp32);
      temp8 = 0;
      #if defined(TORQUE_OS_LINUX) || defined(TORQUE_OS_OPENBSD)
         temp8 |= pgdServerQuery::ServerInfo::Status_Linux;
      #endif
      #if defined(TORQUE_OS_XENON)
            temp8 |= pgdServerQuery::ServerInfo::Status_Xenon;
      #endif
		if (Con::getBoolVariable("Server::Dedicated")) {
			temp8 |= pgdServerQuery::ServerInfo::Status_Dedicated;
		}
		if (dStrlen(Con::getVariable("pref::Server::Password")) > 0) {
         temp8 |= pgdServerQuery::ServerInfo::Status_Passworded;
		}
      out->write(temp8);
      temp8 = U8(Con::getIntVariable("Server::BotCount"));
      out->write(temp8);
      out->write(Platform::SystemInfo.processor.mhz);

      U8 playerCount = U8(Con::getIntVariable("Server::PlayerCount"));
      out->write(playerCount);

      const char* guidList = Con::getVariable("Server::GuidList");
      char* buf = new char[dStrlen(guidList) + 1];
      dStrcpy(buf, guidList);
      char* temp = dStrtok( buf, "\t" );
      temp8 = 0;
      for (;temp && temp8 < playerCount; temp8++) {
         out->write(U32(dAtoi(temp)));
         temp = dStrtok(NULL, "\t");
         temp8++;
      }

		for (;temp8 < playerCount; temp8++) {
         out->write(U32(0));
		}

      delete [] buf;

      BitStream::sendPacketStream(address);
   }
}

static void handleGamePingRequest(const NetAddress* address, U32 key, U8 flags) {
   // Do not respond if a mission is not running:
   if (GNet->doesAllowConnections()) {
      // Do not respond if this is a single-player game:
		if (dStricmp(Con::getVariable("Server::ServerType"), "SinglePlayer") == 0) {
         return;
		}

      // Do not respond to offline queries if this is an online server:
		if (flags & pgdServerQuery::filter::Offline) {
         return;
		}
      BitStream *out = BitStream::getPacketStream();
      out->clearStringBuffer();

      out->write(U8(NetInterface::GamePingResponse));
      out->write(flags);
      out->write(key);

      out->writeString(versionString);

      out->write(GameConnection::CurrentProtocolVersion);
      out->write(GameConnection::MinRequiredProtocolVersion);
      out->write(getVersionNumber());

      // Enforce a 24-character limit on the server name:
      char serverName[25];
      dStrncpy(serverName, Con::getVariable("pref::Server::Name"), 24);
      serverName[24] = 0;
      out->writeString(serverName);

      BitStream::sendPacketStream(address);
   }
}

static void handleGamePingResponse(const NetAddress* address, BitStream* stream, U32 key) {
   // Broadcast has timed out or query has been cancelled:
	if(!_gPingList.size()) {
      return;
	}

   S32 index = pSQ->findPingEntry(_gPingList, address);
   if(index == -1) {
      // an anonymous ping response - if it's not already timed
      // out or finished, ping it.  Probably from a broadcast
		if(!pSQ->addressFinished(address)) {
         pSQ->pushPingRequest(address);
		}
      return;
   }
	pgdServerQuery::Ping &p = _gPingList[index];
   U32 infoKey = (p.session << 16) | (p.key & 0xFFFF);
	if(infoKey != key) {
      return;
	}

   // Find if the server info already exists (favorite or refreshing):
	pgdServerQuery::ServerInfo* si = pSQ->findServerInfo(address);
   bool applyFilter = false;
	if (pSQ->defaultFilter->filterType == pgdServerQuery::filter::Normal) {
      applyFilter = si ? !si->isUpdating() : true;
	}

   char addrString[256];
   Net::addressToString(address, addrString);
	//bool waitingForMaster = (pSQ->defaultFilter->filterType == pgdServerQuery::filter::Normal) && !_gGotFirstListPacket;

   // Verify the version:
   char buf[256];
   stream->readString(buf);
   if (dStrcmp(buf, versionString) != 0) {
      // Version is different, so remove it from consideration:
      Con::printf("Server %s is a different version.", addrString);
      Con::printf("Wanted version %s, got version %s", versionString, buf);
      _gFinishedList.push_back(*address);
      _gPingList.erase(index);
      if (si) {
			si->status = pgdServerQuery::ServerInfo::Status_TimedOut;
         gServerBrowserDirty = true;
      }
		//if (!waitingForMaster) {
         pSQ->updatePingProgress();
		//}
      return;
   }

   // See if the server meets our minimum protocol:
   U32 temp32;
   stream->read(&temp32);
   if (temp32 < GameConnection::MinRequiredProtocolVersion ) {
      Con::printf("Protocol for server %s does not meet minimum protocol.", addrString);
      _gFinishedList.push_back(*address);
      _gPingList.erase(index);
      if (si) {
			si->status = pgdServerQuery::ServerInfo::Status_TimedOut;
         gServerBrowserDirty = true;
      }
		//if (!waitingForMaster) {
         pSQ->updatePingProgress();
		//}
      return;
   }

   // See if we meet the server's minimum protocol:
   stream->read(&temp32);
   if (GameConnection::CurrentProtocolVersion < temp32) {
      Con::printf("You do not meet the minimum protocol for server %s.", addrString);
      _gFinishedList.push_back(*address);
      _gPingList.erase(index);
      if (si) {
			si->status = pgdServerQuery::ServerInfo::Status_TimedOut;
         gServerBrowserDirty = true;
      }
		//if (!waitingForMaster) {
         pSQ->updatePingProgress();
		//}
      return;
   }

   // Calculate the ping:
   U32 time = Platform::getVirtualMilliseconds();
   U32 ping = (time > p.time) ? time - p.time : 0;

   // Check for max ping filter:
	if (applyFilter && pSQ->defaultFilter->maxPing > 0 && ping > pSQ->defaultFilter->maxPing) {
      // Ping is too high, so remove this server from consideration:
      Con::printf("Server %s filtered out by maximum ping.", addrString);
      _gFinishedList.push_back( *address );
      _gPingList.erase( index );
		if (si) {
         pSQ->removeServerInfo(address);
		}
		//if (!waitingForMaster) {
         pSQ->updatePingProgress();
		//}
      return;
   }

   // Get the server build version:
   stream->read(&temp32);
   if (applyFilter
	   && (pSQ->defaultFilter->filterFlags & pgdServerQuery::filter::CurrentVersion)
      && (temp32 != getVersionNumber())) {
      Con::printf("Server %s filtered out by version number.", addrString);
      _gFinishedList.push_back(*address);
      _gPingList.erase(index);
		if (si) {
         pSQ->removeServerInfo(address);
		}
		//if (!waitingForMaster) {
         pSQ->updatePingProgress();
		//}
      return;
   }

   // OK, we can finally create the server info structure:
	if (!si) {
      si = pSQ->findOrCreateServerInfo(address);
	}
   si->ping = ping;
   si->version = temp32;

   // Get the server name:
   stream->readString(buf);
   if (!si->name) {
      si->name = (char*)dMalloc(dStrlen(buf) + 1);
      dStrcpy(si->name, buf);
   }

   // Set the server up to be queried:
	Con::printf("Server %s ready for query.", addrString);
   _gFinishedList.push_back(*address);
   p.key = 0;
   p.time = 0;
   p.tryCount = DefaultRetryCount;
   _gQueryList.push_back(p);
   _gServerQueryCount++;
   _gPingList.erase(index);
	//if (!waitingForMaster) {
      pSQ->updatePingProgress();
	//}
   // Update the server browser gui!
   gServerBrowserDirty = true;
}

static void handleGameInfoRequest(const NetAddress* address, U32 key, U8 flags) {
   // Do not respond unless there is a server running:
   if (GNet->doesAllowConnections()) {
      // Do not respond to offline queries if this is an online server:
		if (flags & pgdServerQuery::filter::Offline) {
         return;
		}
      BitStream *out = BitStream::getPacketStream();
      out->clearStringBuffer();

      out->write(U8(NetInterface::GameInfoResponse));
      out->write(flags);
      out->write(key);

      out->writeString(Con::getVariable("Server::GameType"));
      out->writeString(Con::getVariable("Server::MissionType"));
      out->writeString(Con::getVariable("Server::MissionName"));

      U8 status = 0;
      #if defined(TORQUE_OS_LINUX) || defined(TORQUE_OS_OPENBSD)
		   status |= pgdServerQuery::ServerInfo::Status_Linux;
      #endif
      #if defined(TORQUE_OS_XENON)
			status |= pgdServerQuery::ServerInfo::Status_Xenon;
      #endif
		if (Con::getBoolVariable("Server::Dedicated")) {
			status |= pgdServerQuery::ServerInfo::Status_Dedicated;
		}
		if (dStrlen(Con::getVariable("pref::Server::Password"))) {
			status |= pgdServerQuery::ServerInfo::Status_Passworded;
		}
      out->write(status);

      out->write(U8(Con::getIntVariable( "Server::PlayerCount")));
      out->write(U8(Con::getIntVariable( "pref::Server::MaxPlayers")));
      out->write(U8(Con::getIntVariable( "Server::BotCount")));
      out->write(U16(Platform::SystemInfo.processor.mhz));

      out->writeString(Con::getVariable( "pref::Server::Info"));
      pSQ->writeLongCString(out, Con::evaluate("onServerInfoQuery();"));

      BitStream::sendPacketStream(address);
   }
}

static void handleGameInfoResponse(const NetAddress* address, BitStream* stream) {
	if (!_gQueryList.size()) {
      return;
	}

   S32 index = pSQ->findPingEntry(_gQueryList, address);
	if (index == -1) {
      return;
	}

   // Remove the server from the query list since it has been so kind as to respond:
   _gQueryList.erase(index);
   pSQ->updateQueryProgress();
	pgdServerQuery::ServerInfo *si = pSQ->findServerInfo(address);
	if (!si) {
      return;
	}

   bool isUpdate = si->isUpdating();
	bool applyFilter = pSQ->defaultFilter != NULL && !isUpdate && (pSQ->defaultFilter->filterType == pgdServerQuery::filter::Normal);
   char addrString[256];
   Net::addressToString(address, addrString);

	//Con::printf("hGIR: applyFilter: %i", applyFilter == true ? 1 : 0);
   // Get the rules set:
   char stringBuf[2048];   // Who knows how big this should be?

	//Con::printf("hGIR: reading gameType");
   stream->readString(stringBuf);
   if (!si->gameType || dStricmp(si->gameType, stringBuf) != 0) {
      si->gameType = (char*)dRealloc((void*)si->gameType, dStrlen(stringBuf) + 1);
      dStrcpy(si->gameType, stringBuf);

      // Test against the active filter:
		if (applyFilter && (pSQ->defaultFilter->gameMode.compare("any") != 0) 
			&& pSQ->defaultFilter->gameMode.compare(si->gameType) != 0) {
         Con::printf("Server %s filtered out by rules set. (%s:%s)", addrString, pSQ->defaultFilter->gameMode.c_str(), si->gameType);
         pSQ->removeServerInfo(address);
         return;
      }
   }
	//Con::printf("hGIR: gameType done");

   // Get the mission type:
	//Con::printf("hGIR: reading missionType");
   stream->readString(stringBuf);
   if (!si->missionType || dStrcmp(si->missionType, stringBuf) != 0) {
      si->missionType = (char*)dRealloc((void*)si->missionType, dStrlen(stringBuf) + 1);
      dStrcpy(si->missionType, stringBuf);

      // Test against the active filter:
		if (applyFilter && (pSQ->defaultFilter->missionType.compare("any") != 0) 
			&& pSQ->defaultFilter->missionType.compare(si->missionType) != 0) {
         Con::printf( "Server %s filtered out by mission type. (%s:%s)", addrString, pSQ->defaultFilter->missionType, si->missionType );
         pSQ->removeServerInfo(address);
         return;
      }
   }
	//Con::printf("hGIR: missionType done");

   // Get the mission name:
	//Con::printf("hGIR: reading missionName");
   stream->readString(stringBuf);
   // Clip the file extension off:
   char* temp = dStrstr(static_cast<char*>(stringBuf), const_cast<char*>(".mis" ));
	if (temp) {
      *temp = '\0';
	}
   if (!si->missionName || dStrcmp(si->missionName, stringBuf) != 0) {
      si->missionName = (char*)dRealloc((void*)si->missionName, dStrlen(stringBuf) + 1);
      dStrcpy(si->missionName, stringBuf);
   }
	//Con::printf("hGIR: missionName done");

   // Get the server status:
   U8 temp_U8;
	//Con::printf("hGIR: reading serverStatus");
   stream->read( &temp_U8 );
   si->status = temp_U8;
	//Con::printf("hGIR: serverStatusdone");

   // Filter by the flags:
   if (applyFilter) {
		if (pSQ->defaultFilter->filterFlags & pgdServerQuery::filter::Dedicated && !si->isDedicated()) {
         Con::printf("Server %s filtered out by dedicated flag.", addrString);
         pSQ->removeServerInfo(address);
         return;
      }

      if (pSQ->defaultFilter->filterFlags & pgdServerQuery::filter::NotPassworded && si->isPassworded()) {
         Con::printf("Server %s filtered out by no-password flag.", addrString);
         pSQ->removeServerInfo(address);
         return;
      }

      if (pSQ->defaultFilter->filterFlags & pgdServerQuery::filter::NotXenon && si->isXenon()) {
         Con::printf("Server %s filtered out by no-xenon flag.", addrString);
         pSQ->removeServerInfo(address);
         return;
      }
   }
	si->status = pgdServerQuery::ServerInfo::Status_Responded;
	//Con::printf("hGIR: Filter flags set, server response set");

   // Get the player count:
	//Con::printf("hGIR: reading playerCount");
   stream->read(&si->numPlayers);

   // Test player count against active filter:
   if (applyFilter && (si->numPlayers < pSQ->defaultFilter->minPlayers || si->numPlayers > pSQ->defaultFilter->maxPlayers)) {
      Con::printf("Server %s filtered out by player count.", addrString);
      pSQ->removeServerInfo(address);
      return;
   }
	//Con::printf("hGIR: playerCount done");

   // Get the max players and bot count:
	//Con::printf("hGIR: reading maxPlayers & bots");
   stream->read(&si->maxPlayers);
   stream->read(&si->numBots);

   // Test bot count against active filter:
   if (applyFilter && (si->numBots > pSQ->defaultFilter->maxBots)) {
      Con::printf("Server %s filtered out by maximum bot count.", addrString);
      pSQ->removeServerInfo(address);
      return;
   }
	//Con::printf("hGIR: maxPlayers & bots done");

   // Get the CPU speed;
	//Con::printf("hGIR: reading CPUSpeed");
   U16 temp_U16;
   stream->read(&temp_U16);
   si->cpuSpeed = temp_U16;

   // Test CPU speed against active filter:
   if (applyFilter && ( si->cpuSpeed < pSQ->defaultFilter->minCPU)) {
      Con::printf("Server %s filtered out by minimum CPU speed.", addrString);
      pSQ->removeServerInfo(address);
      return;
   }
	//Con::printf("hGIR: CPUSpeed done");

   // Get the server info:
	//Con::printf("hGIR: reading server info");
   stream->readString(stringBuf);
   if (!si->statusString || (isUpdate && dStrcmp(si->statusString, stringBuf) != 0)) {
      si->infoString = (char*)dRealloc((void*)si->infoString, dStrlen(stringBuf) + 1);
      dStrcpy(si->infoString, stringBuf);
   }

	//Con::printf("hGIR: reading status string");
   // Get the content string:
   pSQ->readLongCString(stream, stringBuf);
   if (!si->statusString || (isUpdate && dStrcmp(si->statusString, stringBuf) != 0)) {
      si->statusString = (char*)dRealloc((void*)si->statusString, dStrlen(stringBuf) + 1);
      dStrcpy(si->statusString, stringBuf);
   }

	//Con::printf("hGIR: All done");

   // Update the server browser gui!
   gServerBrowserDirty = true;
}

void PGDNetInterface::handleInfoPacket(const NetAddress* address, U8 packetType, BitStream* stream) {
   U8 flags;
   U32 key;

	Con::printf("handleInfoPacket Called.");
   char addrString[256];
   Net::addressToString(address, addrString);

   stream->read(&flags);
   stream->read(&key);
   stream->clearStringBuffer();

   switch(packetType) {
      case GamePingRequest:
			Con::printf("GamePingRequest: %s", addrString);
         handleGamePingRequest(address, key, flags);
         break;

      case GamePingResponse:
			Con::printf("GamePingResponse: %s", addrString);
         handleGamePingResponse(address, stream, key);
         break;

      case GameInfoRequest:
			Con::printf("GameInfoRequest: %s", addrString);
         handleGameInfoRequest(address, key, flags);
         break;

      case GameInfoResponse:
			Con::printf("GameInfoResponse: %s", addrString);
         handleGameInfoResponse(address, stream);
         break;

      case GameMasterInfoRequest:
			Con::printf("GameMasterInfoRequest: %s", addrString);
         handleGameMasterInfoRequest(address, key, flags);
         break;
   }
}

//----------------------------------------------------------------------------------------------------
//TorqueScript Definitions
//----------------------------------------------------------------------------------------------------
DefineEngineFunction(startHeartbeat, void, (),, "(void)") {
   pSQ->startHeartbeat();
}

DefineEngineFunction(stopHeartbeat, void, (),, "(void)") {
   pSQ->stopHeartbeat();
} 

DefineEngineFunction(queryPGDMaster, void, (),, "(void)") {
	if(pSQ->defaultFilter == NULL || dAtoi(Con::getVariable("$FilterHasChanged")) == 1) {
      String filter;
	   filter = Con::getVariable("$ActiveServerFilter");

	   pgdServerQuery::filter *f = pSQ->createFilterFromSettings(filter);
	   pSQ->defaultFilter = f;

		Con::setVariable("$FilterHasChanged", "0");
	}

   pSQ->queryMasterServer();
}

DefineEngineFunction(queryLocalServers, void, (U32 port),, "(void)") {
	if(pSQ->defaultFilter == NULL || dAtoi(Con::getVariable("$FilterHasChanged")) == 1) {
      String filter;
	   filter = Con::getVariable("$ActiveServerFilter");

	   pgdServerQuery::filter *f = pSQ->createFilterFromSettings(filter);
	   pSQ->defaultFilter = f;

		Con::setVariable("$FilterHasChanged", "0");
	}

	pSQ->queryLANServers(port != 0 ? port : 28000);
}

DefineEngineFunction(queryAllServers, void, (),, "(void)") {
	if(pSQ->defaultFilter == NULL || dAtoi(Con::getVariable("$FilterHasChanged")) == 1) {
      String filter;
	   filter = Con::getVariable("$ActiveServerFilter");

	   pgdServerQuery::filter *f = pSQ->createFilterFromSettings(filter);
	   pSQ->defaultFilter = f;

		Con::setVariable("$FilterHasChanged", "0");
	}
	pSQ->queryAllServers();
}

DefineEngineFunction(stopQuery, void, (),, "(void)") {
   pSQ->cancelServerQuery();
}

DefineEngineFunction(getServerCount, int, (),, "(void)" ) {
   return gServerList.size();
}

DefineEngineFunction(setServerInfo, bool, (int index),, "(int index)") {
   if (index >= 0 && index < gServerList.size()) {
		pgdServerQuery::ServerInfo& info = gServerList[index];

      char addrString[256];
      Net::addressToString( &info.address, addrString );

      Con::setIntVariable("ServerInfo::Status",info.status);
      Con::setVariable("ServerInfo::Address",addrString);
      Con::setVariable("ServerInfo::Name",info.name);
      Con::setVariable("ServerInfo::GameType",info.gameType);
      Con::setVariable("ServerInfo::MissionName",info.missionName);
      Con::setVariable("ServerInfo::MissionType",info.missionType);
      Con::setVariable("ServerInfo::State",info.statusString);
      Con::setVariable("ServerInfo::Info",info.infoString);
      Con::setIntVariable("ServerInfo::PlayerCount",info.numPlayers);
      Con::setIntVariable("ServerInfo::MaxPlayers",info.maxPlayers);
      Con::setIntVariable("ServerInfo::BotCount",info.numBots);
      Con::setIntVariable("ServerInfo::Version",info.version);
      Con::setIntVariable("ServerInfo::Ping",info.ping);
      Con::setIntVariable("ServerInfo::CPUSpeed",info.cpuSpeed);
      Con::setBoolVariable("ServerInfo::Favorite",info.isFavorite);
      Con::setBoolVariable("ServerInfo::Dedicated",info.isDedicated());
      Con::setBoolVariable("ServerInfo::Password",info.isPassworded());
      return true;
   }
   return false;
}