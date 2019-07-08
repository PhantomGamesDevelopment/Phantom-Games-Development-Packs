//radar.cs
//Robert C. Fritzen - Phantom139
//Server-Side of Radar / Map Selector Controls, AFPSK

package AFPSK_Radar_Server {
   //Update sendLoadInfo to send our current mission file, and in extension, the file name for the radar.
   function sendLoadInfoToClient( %client ) {
      messageClient( %client, 'MsgLoadInfo', "", theLevelInfo.levelName, $GameModeName, fileBase($Server::MissionFile) );

      // Send Mission Description a line at a time
      for( %i = 0; theLevelInfo.desc[%i] !$= ""; %i++ ) {
         messageClient( %client, 'MsgLoadDescripition', "", theLevelInfo.desc[%i] );
      }

      messageClient( %client, 'MsgLoadInfoDone' );
   }
   
   function GameConnection::onConnect(%client, %name) {
      Parent::onConnect(%client, %name);

      //Send NARC
      %GuiName = "TheRadar";
      %RadarRange = 25;
      %UAVFlag = 0;
      %UAVSweepTime = 7000;
      %JamRadar = 0;
      SendNARCEvent(%client, 1, %GuiName @"\t"@ %RadarRange @"\t"@ %UAVFlag @"\t"@ %UAVSweepTime @"\t"@ %JamRadar);
   }
   
   function GameCore::spawnPlayer(%game, %client, %spawnPoint, %noControl) {
      Parent::spawnPlayer(%game, %client, %spawnPoint, %noControl);
      if(%client.team !$= "") {
         %client.player.setTeam(%client.team);
      }
      %client.player.setShapeIcon("art/gui/radar/playerDot.png\tart/gui/radar/playerArrow.png");
   }
   
   function WeaponImage::onFire(%this, %obj, %slot) {
      Parent::onFire(%this, %obj, %slot);

      //Radar Hook
      cancel(%obj.removeDotSchd);
      for(%i = 0; %i < ClientGroup.getCount(); %i++) {
         %client = ClientGroup.getObject(%i);
         SendNARCEvent(%client, 0, "TheRadar\t1\t"@%obj@"\t0");
      }
      %obj.removeDotSchd = schedule(4000, 0, removeRadarDot, %obj);
   }
};
activatePackage(AFPSK_Radar_Server);

package afpsk_Server_Loader_Radar {
   function GameCore::createGame() {
      Parent::createGame();
      deactivatePackage(AFPSK_Radar_Server);
      activatePackage(AFPSK_Radar_Server);
   }
};
activatePackage(afpsk_Server_Loader_Radar);

function serverCmdpushPositionSelection(%client, %position) {
   %worldPos = %position @" 0";
   %client.selector1 = %worldPos;
   if(%client.returnedSelector $= "Artillery") {
      launchSingleShell(%client.player, %client.selector1);
      %client.artilleryCalls--;
      if(%client.artilleryCalls == 0) {
         commandToClient(%client, 'DoneWithSelections'); //Tell them to close their dialog now
      }
   }
}

function serverCmdpushDirectionSelection(%client, %position) {
   %worldPos = %position @" 0";
   %client.selector2 = %worldPos;
   
   if(%client.returnedSelector $= "Airstrike") {
      prepareAirstrike(%client.player, %client.selector1, %client.selector2);
      commandToClient(%client, 'DoneWithSelections'); //Tell them to close their dialog now
   }
}

function removeRadarDot(%object) {
   for(%i = 0; %i < ClientGroup.getCount(); %i++) {
      %client = ClientGroup.getObject(%i);
      SendNARCEvent(%client, 0, "TheRadar\t0\t"@%object@"\t0");
   }
}

//Sample UAV Code
function GameCore::doUAV(%this, %team, %type) {
   if(%this.currentUAV[%team] == 2 && %type == 1) {
      //Don't cancel out advanced uav's with regular ones.
   }
   else if(%this.currentUAV[%team] != 2) {
      //If the UAV isn't advanced we can safely cancel it out.
      if(isEventPending(%this.uav[%team])) {
         cancel(%this.uav[%team]);
      }
      %this.currentUAV[%team] = %type+1;
   }

   if(%type == 1) {
      //Advanced UAV - Increases Radar Range to 75m and reveals all enemies
      %GuiName = "TheRadar";
      %RadarRange = 75;
      %UAVFlag = 2;
      %UAVSweepTime = 7000;
      %JamRadar = 0;
      for(%i = 0; %i < ClientGroup.getCount(); %i++) {
         %client = ClientGroup.getObject(%i);
         if(%client.team == %team) {
            SendNARCEvent(%client, 1, %GuiName @"\t"@ %RadarRange @"\t"@ %UAVFlag @"\t"@ %UAVSweepTime @"\t"@ %JamRadar);
         }
      }
      %this.uav[%team] = %this.schedule(25000, "endUAV", %team);
   }
   else {
      //Regular
      %GuiName = "TheRadar";
      %RadarRange = 25;
      %UAVFlag = 1;
      %UAVSweepTime = 7000;
      %JamRadar = 0;
      for(%i = 0; %i < ClientGroup.getCount(); %i++) {
         %client = ClientGroup.getObject(%i);
         if(%client.team == %team) {
            SendNARCEvent(%client, 1, %GuiName @"\t"@ %RadarRange @"\t"@ %UAVFlag @"\t"@ %UAVSweepTime @"\t"@ %JamRadar);
         }
      }
      %this.uav[%team] = %this.schedule(25000, "endUAV", %team);
   }
}

function GameCore::endUAV(%this, %team) {
   %this.currentUAV[%team] = -1;
   %this.uav[%team] = 0;
   //
   %GuiName = "TheRadar";
   %RadarRange = 25;
   %UAVFlag = 0;
   %UAVSweepTime = 7000;
   %JamRadar = 0;
   for(%i = 0; %i < ClientGroup.getCount(); %i++) {
      %client = ClientGroup.getObject(%i);
      if(%client.team == %team) {
         SendNARCEvent(%client, 1, %GuiName @"\t"@ %RadarRange @"\t"@ %UAVFlag @"\t"@ %UAVSweepTime @"\t"@ %JamRadar);
      }
   }
}

//Use this to spawn a waypoint that can be used by the radar
function spawnLabeledWaypoint(%team, %name, %transform) {
   %wp = new WayPoint() {
      team = %team;
      name = %name;
      dataBlock = "WayPointMarker";
      position = getWords(%transform, 0, 2);
      rotation = getWords(%transform, 3, 3);
      scale = "1 1 1";
      canSave = "1";
      canSaveDynamicFields = "1";
   };
   
   %wp.setShapeIcon("art/gui/radar/playerHeavy.png");
   //Note: If you want enemy radars to have access to this waypoint use a narc event like so:
   //schedule(2500, 0, SendNARCEvent, %client, 0, "TheRadar\t1\t"@%wp@"\t2");
}
