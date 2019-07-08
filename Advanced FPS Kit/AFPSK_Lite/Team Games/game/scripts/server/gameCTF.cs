// ----------------------------------------------------------------------------
// CTFGame
// Team, 12 players in 2 teams of 6 try to capture the opposing team's flag 3 times
// ----------------------------------------------------------------------------

// DATABLOCKS
datablock ItemData(CTFFlag) {
   // Mission editor category, this datablock will show up in the
   // specified category under the "shapes" root category.
   category = "Seekers";

   className = "GameItem";

   // Basic Item properties
   shapeFile = "art/shapes/CTF/CTF_Flag.dae";
   mass = 2;
   friction = 1;
   elasticity = 0.3;
   emap = true;

   // Dynamic properties defined by the scripts
   pickupName = "";

   availableSkins = "red blue";
};

function CTFFlag::onPickup(%this, %obj, %user, %amount) {
   if(%obj.team == %user.getTeam()) {
      //Same Team Touch...
      if(!Game.flagAtBase[%user.team]) {
         //Return The Flag
         Game.doFlagReturn(%obj.team);
      }
      if(%user.hasCTFFlag && %user.flagTeam != %user.getTeam()) {
         if(Game.flagAtBase[%user.team]) {
            //Capture!
            Game.captureFlag(%user);
         }
      }
   }
   else {
      //Enemy Team Touch
      if(%obj.stolen) {
         //If the flag is already "Taken", don't allow someone else to nab it.
         return;
      }
      %obj.startFade(1, 0, true);
      %obj.stolen = true;
      %user.hasCTFFlag = true;
      %user.flag = %obj;
      Game.flagAtBase[%obj.team] = false;
      Game.flag[%obj.team] = %user;
      //
      MessageAll('msgCTF', (%obj.team == 1 ? "Red " : "Blue ") @ "Flag taken by "@%user.getShapeName());
   }
}

// CODE


if(isObject(HostingList)) {
   HostingList.readIn = GameMode_CTF;
}
if(!isObject(GameMode_CTF)) {
   new ScriptObject(GameMode_CTF) {
      gameName          = "Capture The Flag";
      gameType          = "CTF";
      gameDesc          = "A Standard team game where two teams fight to secure the opposing team's flag.";

      gameOptions_Score[0] = -1;
      gameOptions_Score[1] = 1;
      gameOptions_Score[2] = 2;
      gameOptions_Score[3] = 3;
      gameOptions_Score[4] = 5;
      gameOptions_Score[5] = 7;
      gameOptions_Score[6] = 10;
      gameOptions_Score[7] = 15;
      gameOptions_Score[8] = 20;
      gameOptions_Score[9] = 25;

      gameOptions_Time[0] = -1;
      gameOptions_Time[1] = 5;
      gameOptions_Time[2] = 10;
      gameOptions_Time[3] = 15;
      gameOptions_Time[4] = 20;
      gameOptions_Time[5] = 30;
      gameOptions_Time[6] = 60;

      gameMapRestrict   = "";
   };
}

function CTFGame::PlaceFlags(%game) {
   %Pos1 = Team1Flag.getPosition();
   %Pos2 = Team2Flag.getPosition();
   
   %game.teamFlag[1] = new Item() {
      position = %Pos1;
      rotation = "0 0 1 " @ (getRandom() * 360);
      scale = "1 1 1";
      dataBlock = CTFFlag;
      collideable = "0";
      static = "0";
      rotate = "0";
      team = "1";
   };
   
   %game.teamFlag[2] = new Item() {
      position = %Pos2;
      rotation = "0 0 1 " @ (getRandom() * 360);
      scale = "1 1 1";
      dataBlock = CTFFlag;
      collideable = "0";
      static = "0";
      rotate = "0";
      team = "2";
   };
   %game.teamFlag[2].setSkinName(addTaggedString("CTF_Flag_FlagMat_Red=CTF_Flag_FlagMat_Blue"));
   
   %game.flagAtBase[1] = true;
   %game.flag[1] = "";
   %game.flagAtBase[2] = true;
   %game.flag[2] = "";
}

function CTFGame::doFlagReturn(%game, %flag) {
   %Pos[1] = Team1Flag.getPosition();
   %Pos[2] = Team2Flag.getPosition();

   %flag.setPosition(%pos[%flag.team]);
   %game.flagAtBase[%flag.team] = true;
   %flag.stolen = false;
   %flag.startFade(1, 0, false);
   %game.flag[%flag.team] = "";
   
   MessageAll('msgCTF', (%flag.team == 1 ? "Red " : "Blue ") @ "Flag returned to base");
}

function CTFGame::captureFlag(%game, %player) {
   %flag = %player.flag;
   %team = %player.getTeam();

   %pos[1] = Team1Flag.getPosition();
   %pos[2] = Team2Flag.getPosition();
   
   %player.hasCTFFlag = false;
   %player.flag = "";

   %flag.setPosition(%pos[%flag.team]);
   %game.flagAtBase[%flag.team] = true;
   %flag.stolen = false;
   %flag.startFade(1, 0, false);
   %game.flag[%flag.team] = "";

   MessageAll('msgCTF', (%team == 1 ? "Red " : "Blue ") @ "Team captured the enemy flag!");
   %game.caps[%team]++;
   if($Game::EndGameScore > 0 && %game.caps[%team] >= $Game::EndGameScore) {
      %game.cycleGame();
   }
}

function CTFGame::onMissionLoaded(%game) {
   $Server::MissionType = "CTF";
   parent::onMissionLoaded(%game);
}

function CTFGame::initGameVars(%game) {
   $Game::defaultPlayerClass = "Player";
   $Game::defaultPlayerDataBlock = "DefaultPlayerData";
   $Game::defaultPlayerSpawnGroups = "PlayerSpawnPoints PlayerDropPoints";

   $Game::defaultCameraClass = "Camera";
   $Game::defaultCameraDataBlock = "Observer";
   $Game::defaultCameraSpawnGroups = "CameraSpawnPoints PlayerSpawnPoints PlayerDropPoints";

   // Set the gameplay parameters, Phantom: Removed the ones affected by the server options
   %game.preGamePeriod = 20;
   %game.HPRestoreTime = 7500; //Phantom139: This is a server parameter, but it's not implemented yet!
   %game.HPRestoreRate = 2; //Phantom139: This is a server parameter, but it's not implemented yet!
   //%game.duration = 30 * 60;
   //%game.endgameScore = 3;
   %game.endgamePause = 10;
   %game.respawnTime = 10;  //Phantom139: This is a server parameter, but it's not implemented yet!
   %game.allowCycling = false;   // Is mission cycling allowed?
}

function CTFGame::startGame(%game) {
   %game.PlaceFlags();
   
   %game.caps[1] = 0;
   %game.caps[2] = 0;

   parent::startGame(%game);
   //Assign Teams
   for (%clientIndex = 0; %clientIndex < ClientGroup.getCount(); %clientIndex++) {
      %cl = ClientGroup.getObject(%clientIndex);
      %cl.AssignTeam(%game, true);
   }
}

function CTFGame::endGame(%game) {
   parent::endGame(%game);
}

function CTFGame::cycleGame(%game) {
   parent::cycleGame(%game);
}

function CTFGame::onGameDurationEnd(%game) {
   parent::onGameDurationEnd(%game);
}

function CTFGame::onClientLeaveGame(%game, %client) {
   parent::onClientLeaveGame(%game, %client);
}

function CTFGame::onClientEnterGame(%game, %client) {
   parent::onClientEnterGame(%game, %client);

   //Assign them to a team.
   %client.AssignTeam(%game, true);
}

function CTFGame::preparePlayer(%game, %client) {
   //Pick a spawn-sphere from our list.
   %sphereGroup = TeamDropPoints;
   for(%i = 0; %i < %sphereGroup.getCount(); %i++) {
      %sphere = %sphereGroup.getObject(%i);
      if(%sphere.team == %client.team) {
         break;
      }
   }

   %playerSpawnPoint = %sphere;
   %game.spawnPlayer(%client, %playerSpawnPoint);
}

function CTFGame::spawnPlayer(%game, %client, %spawnPoint, %noControl) {
   parent::spawnPlayer(%game, %client, %spawnPoint);
   
   %game.loadOut(%client.player);
}

function CTFGame::onDeath(%game, %player, %client, %sourceObject, %sourceClient, %damageType, %damLoc) {
   if(%player.hasCTFFlag) {
      %flag = %player.flag;
      //Reveal Flag
      %flag.setTransform(%player.getTransform());
      %flag.stolen = false;
      %flag.startFade(1, 0, false);
      %game.flag[%flag.team] = "";
      
      MessageAll('msgCTF', (%flag.team == 1 ? "Red " : "Blue ") @ "Team Flag dropped!");
   }

   %client.player.setShapeName("");
   %client.player.setShapeIcon("");
   if (isObject(%client.camera) && isObject(%client.player)) {
      %client.camera.setMode("Corpse", %client.player);
      %client.setControlObject(%client.camera);
   }
   %client.player = 0;
   %sendMsgFunction = "sendMsgClientKilled_" @ %damageType;
   if (!isFunction(%sendMsgFunction)) {
      %sendMsgFunction = "sendMsgClientKilled_Default";
   }
   call(%sendMsgFunction, 'MsgClientKilled', %client, %sourceClient, %damLoc);

   if ((%damageType $= "Suicide" || %sourceClient == %client) && isObject(%sourceClient)) {
      %game.incDeaths(%client, 1, true);
   }
   else {
      %game.incDeaths(%client, 1, false);
      %game.incKills(%sourceClient, 1, false);
   }
}
