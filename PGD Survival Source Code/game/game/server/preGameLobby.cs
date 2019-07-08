//preGameLobby.cs
//Robert Fritzen (Phantom139)
//[C] Phantom Games Development, 2012
//Controls the serverside aspect of the pre-game lobby
function serverCmdstartGameButton(%client) {
	if(!%client.isAdmin) {
		MessageClient(%client, 'msgFail', "\c3SERVER: Only the host may start the game.");
		return;
	}
	if($Server::GameStarting) {
		MessageClient(%client, 'msgFail', "\c3SERVER: The Game is about to begin.");
		return;	
	}

	MessageAll('msgAdmin', "\c3SERVER: Game Starting in 5 Seconds.");
	schedule(5000, 0, PGL_ServerBeginGame);
	$Server::GameStarting = true;

	$Host::LastMission = $Server::LevelSelection;
}

function serverCmdPushClientRank(%client, %rank) {
   %client.rankNumber = %rank;
	if(isObject(PreGameDetails)) {
		if(PreGameDetails.isInList(%client)) {
		   PreGameDetails.refreshClientInfo(%client);
		}
	}
}

function serverCmdgetLobbyInfo(%client) {
	%level = $Server::LevelSelection;
   
	%info = getLevelInfo(%level);

	%missionName = %info.LevelName;
	%missionGame = %info.gameType;

	%preview = fileBase(%level) @"_preview.png";

	commandToClient(%client, 'pushLobbyInfo', %missionName, %missionGame, %preview);

	//Refresh All:
   PreGameDetails.updateAll();
}

function PGL_ServerBeginGame() {
	loadMission($Server::LevelSelection, true);
	$Server::InPreGame = false;
	$Server::GameStarting = false;

	for(%i = 0; %i < ClientGroup.getCount(); %i++) {
      commandToClient(ClientGroup.getObject(%i), 'PGLPostLaunch');
	}
}

function initializePreGameLobby() {
	if(isObject(PreGameDetails)) {
	   //remove the old one
		PreGameDetails.delete();
	}
	%PGC = new ScriptObject(PreGameDetails) {
	   class = "PreGameControl";
		playerIndex = 0;
	};
}

function returnToLobbyPostGame() {
   $Server::LevelSelection = $Host::LastMission;

   initializePreGameLobby(); //re-init Lobby
	for(%i = 0; %i < ClientGroup.getCount(); %i++) {
	   %client = ClientGroup.getObject(%i);

      PreGameDetails.addToPreGameLobby(%client, true);
	}
}

function PreGameControl::addToPreGameLobby(%this, %client, %DNPG) {
	if(!isObject(PreGameDetails)) {
	   initializePreGameLobby();
	}
	if(!%DNPG) {  
      commandToClient(%client, 'pushOpenPGL');
   }

   %name = %client.naseBase;

	%lastIndex = %this.playerIndex;
	%this.player[%lastIndex] = %client TAB %client.nameBase TAB %client.rankNumber TAB %client.cmLevel;
	%this.playerIndex++;

	%this.updateAll();
}

function PreGameControl::refreshClientInfo(%this, %client) {
   for(%i = 0; %i < %this.playerIndex; %i++) {
	   if(getField(%this.player[%i], 0) == %client) {
		   %this.player[%i] = %client TAB %client.nameBase TAB %client.rankNumber TAB %client.cmLevel;  
			break; //stop here.
		}
	}

	%this.updateAll();
}

function PreGameControl::isInList(%this, %client) {
   %inList = false;
	for(%i = 0; %i < %this.playerIndex; %i++) {
	   if(getField(%this.player[%i], 0) == %client) {
		   %inList = true;
			break;
		}
	}
	return %inList;
}

function PreGameControl::removeFromPreGameLobby(%this, %client) {
	%keepUp = false;
	for(%i = 0; %i < %this.playerIndex; %i++) {
		if(%keepUp) {
		   %this.player[%i] = %this.player[%i+1];
		}
		if(getField(%this.player[%i], 0) == %client) {
		   %keepUp = true;
			%this.player[%i] = %this.player[%i+1];
		}
	}
	%this.lastIndex--;
  
   %this.updateAll();
}

function PreGameControl::updateAll(%this) {
	for(%i = 0; %i < ClientGroup.getCount(); %i++) {
	   %client = ClientGroup.getObject(%i);

		for(%y = 0; %y < %this.playerIndex; %y++) {
		   commandToClient(%client, 'pushPGLInfo', %y, %this.player[%y]);
		}
	}
}