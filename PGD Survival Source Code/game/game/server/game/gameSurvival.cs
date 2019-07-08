//gameSurvival.cs
//Phantom139

//Generic GameCore Overloads
function SurvivalGame::onMissionLoaded(%game) {
   $Server::MissionType = "Survival";
   parent::onMissionLoaded(%game);

	placeMapArmories();
}

function SurvivalGame::initGameVars(%game) {
   $Game::defaultPlayerClass = "Player";
   $Game::defaultPlayerDataBlock = "DefaultPlayerData";
   $Game::defaultPlayerSpawnGroups = "PlayerSpawnPoints PlayerDropPoints";

   $Game::defaultCameraClass = "Camera";
   $Game::defaultCameraDataBlock = "Observer";
   $Game::defaultCameraSpawnGroups = "CameraSpawnPoints PlayerSpawnPoints PlayerDropPoints";

   // Set the gameplay parameters
   %game.duration = 0;
   %game.endgameScore = 0;
   %game.endgamePause = 10;
   %game.allowCycling = false;   // Is mission cycling allowed?

	%game.isTutorial = 0;
}

function SurvivalGame::startGame(%game) {
   parent::startGame(%game);
   
	if(!$Server::InDeveloperMode) {
      %game.startSurvival(); //Phantom139: Added Here
	}
}

function SurvivalGame::endGame(%game) {
   if (!$Game::Running) {
      error("endGame: No game running!");
      return;
   }
   cancel($Game::Schedule);

	if(isObject(ArmoryGroup)) {
	   ArmoryGroup.delete();
	}

   for (%clientIndex = 0; %clientIndex < ClientGroup.getCount(); %clientIndex++) {
      %cl = ClientGroup.getObject(%clientIndex);
      commandToClient(%cl, 'GameEnd', $Game::EndGamePause);     
      commandToClient(%cl, 'SaveFromServer');
		//if this is a MP Server, push back to lobby in 10 seconds, otherwise we can safely call disconnet();
		if($Server::isMultiplayerServer) {
			//Rebuild the PGL
			returnToLobbyPostGame();
			//have all clients send up their new info, and then in 10 seconds push them all back up
			schedule(2500, 0, commandToClient, %cl, 'RequestClientRank');
		   schedule(10000, 0, commandToClient, %cl, 'ReturnLobby');
		}
		else {
	      schedule(10000, 0, commandToClient, %cl, 'safeDisconnect');
		}
   }

   $Game::Running = false;
}

function SurvivalGame::onGameDurationEnd(%game) {
   parent::onGameDurationEnd(%game);
}

function SurvivalGame::onClientEnterGame(%game, %client) {
   parent::onClientEnterGame(%game, %client);
   
   %client.team = 1;
   %game.checkClientSpawn(%client); //Phantom139: Added Here
}

function SurvivalGame::onClientLeaveGame(%game, %client) {
   parent::onClientLeaveGame(%game, %client);
}
// End Generic Overloads

function SurvivalGame::checkClientSpawn(%game, %client) {
   if(%client.out || %game.outName[%client.namebase]) {
      return false; //no spawning.
   }
   else {
      //spawn a player
      %game.preparePlayer(%client);
   }
}

function SurvivalGame::startSurvival(%game) {
   %game.wave = 0;
   %game.zombieCount = 0;
   
   %game.zombieGroup = new arrayobject() {};
   
   %game.multiFactor = 1.3333; //Increasing makes more zombies come
   %game.divisionFactor = 2;   //Increasing makes less zombies come
   
   %game.schedule(7500, startNextWave);
}

function SurvivalGame::onDeath(%game, %client, %sourceObject, %sourceClient, %damageType, %damLoc) {
   parent::onDeath(%game, %client, %sourceObject, %sourceClient, %damageType, %damLoc);
	if(!$Server::InDeveloperMode) {
      %client.out = 1;
      %game.outName[%client.namebase] = 1;

	   MessageAll('msgDeath', "\c3"@%client.namebase@" has been taken down by the zombie horde.");
   
      %safe = 0;
      for(%i = 0; %i < ClientGroup.getCount(); %i++) {
         %cl = ClientGroup.getObject(%i);
         if(!%cl.out) {
            %safe = 1;
         }
      }
      //
      if(!%safe) {
         %game.endGame();
		}
   }
}

function SurvivalGame::startNextWave(%game) {
   %game.wave += 1;
   %game.zombieCount = mFloor((15 + (%game.multiFactor * %game.wave * $Server::PlayerCount)) / %game.divisionFactor);  //Phantom139: This can be adjusted at any time
   %game.waveCount = %game.zombieCount;
   
   for(%i = 0; %i < ClientGroup.getCount(); %i++) {
      %client = ClientGroup.getObject(%i);
      commandToClient(%client, 'informNextWave', %game.wave);
		commandToClient(%client, 'UpdateZCounter', %game.waveCount);
      //-------- Respawn Dead Players
      if(%client.out || !isObject(%client.player)) {
         %client.out = 0;
         %game.outName[%client.namebase] = 0;
         %game.preparePlayer(%client);
      }
   }
   //
   for(%i = 0; %i < %game.zombieCount; %i++) {
      %game.schedule(getRandom(4500, 25000), spawnAZombie);
   }
   //
   $Zombie::DamageFactor = mFloor(%game.wave / 15);
	$Zombie::SpeedFactor = mCeil(%game.wave / 10);
}

function SurvivalGame::spawnAZombie(%game) {
   //grab the locations of the spawnSpheres
   %sph = pickPlayerSpawnPoint("ZombieDropPoints");
   
   %player = spawnZombie(getRandom(1,6), %sph.getTransform());
}

function SurvivalGame::checkNextWave(%game) {
   if(%game.zombieCount == 0) {
		WaveEndHP();
      //wave success!
      for(%i = 0; %i < ClientGroup.getCount(); %i++) {
         %client = ClientGroup.getObject(%i);
			%client.out = 0;
			%game.outName[%client.namebase] = 0;
			if(!isObject(%client.player) || %client.player.getState() $= "dead") {
			   %game.preparePlayer(%client);   
			}
         %client.points += 1500 + (250 * %game.wave);
         commandToClient(%client, 'updateClientPoints', %client.points);
         //
      }
      // Push next wave in 7 seconds
      %game.schedule(7500, startNextWave);
   }
}

function SurvivalGame::awardZombieAssist(%game, %client) {
   %client.points += 75;
	%client.score += 3;
   commandToClient(%client, 'updateClientPoints', %client.points);
}

function SurvivalGame::awardZombieKill(%game, %client) {
   %client.points += 150;
	%client.kills++;
	%client.score += 10;
   commandToClient(%client, 'updateClientPoints', %client.points);
}
