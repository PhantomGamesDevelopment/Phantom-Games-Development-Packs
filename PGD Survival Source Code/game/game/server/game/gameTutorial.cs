//gameSurvival.cs
//Phantom139

//Generic GameCore Overloads
function TutorialGame::onMissionLoaded(%game) {
   $Server::MissionType = "Tutorial";
   parent::onMissionLoaded(%game);
}

function TutorialGame::initGameVars(%game) {
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

	%game.isTutorial = 1;
}

function TutorialGame::startGame(%game) {
   parent::startGame(%game);
   
   %game.startSurvival(); //Phantom139: Added Here

	//Add Armory
	spawnArmory(ArmorySpawnMarker.getTransform());
}

function TutorialGame::endGame(%game) {
   parent::endGame(%game);
}

function TutorialGame::onGameDurationEnd(%game) {
   parent::onGameDurationEnd(%game);
}

function TutorialGame::onClientEnterGame(%game, %client) {
   parent::onClientEnterGame(%game, %client);
   
   %client.team = 1;
   %game.checkClientSpawn(%client); //Phantom139: Added Here
}

function TutorialGame::onClientLeaveGame(%game, %client) {
   parent::onClientLeaveGame(%game, %client);
}
// End Generic Overloads

function TutorialGame::checkClientSpawn(%game, %client) {
   %game.preparePlayer(%client);

   %client.camera.setMode("Lock", %client.player);
   %client.setControlObject(%client.camera);

	//initial print.
	CenterPrint(%client, "Welcome to PGD Survival!", 5, 3);
	schedule(5000, 0, CenterPrint, %client, "This Tutorial will give you a brief introduction on the controls and gameplay mechanics behind PGD Survival", 5, 3);
   schedule(10000, 0, CenterPrint, %client, "You can view and change your settings at any time by pressing Ctrl + O", 5, 3);
   schedule(15000, 0, CenterPrint, %client, "To Move, use 'w' to move forward, 's' to move backwards, and 'a / d' to move left and right respectively", 5, 3);
   schedule(20000, 0, CenterPrint, %client, "Move forward into the tutorial range now.", 3, 3);

	%client.schedule(23000, setControlObject, %client.player);
}

function TutorialGame::loadOut(%game, %player) {
   %client = %player.client;
   
   %client.setWeapons("", "", 0, 0); //Default Starting Value
}

function TutorialGame::startSurvival(%game) {
   //Nav.build(); //ensure the navMap is up
   %game.zombieCount = 0;
   
   %game.zombieGroup = new arrayobject() {};

	%game.spawnAZombie(getWords(ZSpawn1.getTransform(), 0, 2));
	%game.spawnAZombie(getWords(ZSpawn2.getTransform(), 0, 2));
	%game.spawnAZombie(getWords(ZSpawn3.getTransform(), 0, 2));
}

function TutorialGame::onDeath(%game, %client, %sourceObject, %sourceClient, %damageType, %damLoc) {
   parent::onDeath(%game, %client, %sourceObject, %sourceClient, %damageType, %damLoc);
}

function TutorialGame::spawnAZombie(%game, %position) {
   
   %player = spawnZombie(getRandom(1,6), %position, true);
}

function TutorialGame::informArmory(%game) {
   %client = ClientGroup.getObject(0);
	if(!%client.hasSeenArmory) {
	   CenterPrint(%client, "This is an armory, to use it press 'e' (World Use Keybind).\nFrom here, you can buy weapons and ammo, open the armory, and purchase a Ryder.", 7, 3);
	   %client.points += 500;
		%client.hasSeenArmory = 1;
	}
}

function TutorialGame::giveSecondWeapon(%game) {
   %client = ClientGroup.getObject(0);
	if(!%client.hasSeenArmory2) {
      %client.camera.setMode("Lock", %client.player);
      %client.setControlObject(%client.camera);
		CenterPrint(%client, "Up ahead there are two more zombies, a pistol is obviously not the best choice of weapon.", 5, 3);
		%client.schedule(5000, setWeapons, "Ryder", "Lurker", 1, 1);
		schedule(5000, 0, CenterPrint, %client, "You have been armed with a Lurker. To switch between your guns, press 'q'.", 5, 3);
		%client.player.switchWeapons(2);

      %client.schedule(10000, setControlObject, %client.player);
	   %client.hasSeenArmory2 = 1;
	}
}

function TutorialGame::onReachEnd(%game) {
   %client = ClientGroup.getObject(0);
	if(!%client.hasSeenArmory3) {
      %client.camera.setMode("Lock", %client.player);
      %client.setControlObject(%client.camera);	

		CenterPrint(%client, "You have completed this combat training range and are now ready to face the horde!", 3, 3);
		%client.setBlackOut(true, 4500);
      commandToClient(%client, 'endTutorial');

		%client.hasSeenArmory3 = 1;
	}
}

function TutorialGame::checkNextWave(%game) {

}