//aiCore.cs
//Phantom139
//(C) 2014, Phantom Games Development
//This script file handles the AI task core, as well as the necessary functioning

//AI Names
$AINames[0] = "Botz0r";
$AINames[1] = "I <3 Gunz";
$AINames[2] = "Lord of Headshot";
$AINames[3] = "I Shoot You";
$AINames[4] = "The Slayer";
$AINames[5] = "Pwnz0r Xtreme";
$AINames[6] = "360NoScopez";
$AINames[7] = "Hunter";
//

function initAICore() {
   if(isObject(AIManager)) {
      AIManager.delete();
   }
   if(isObject(AIClientList)) {
      AIClientList.delete();
   }
   
   echo("* Server AI Core Initiated.");
   
   %aiMGR = new SimSet(AIManager) {
      class = "TUAIManager";
   };
   %clList = new SimSet(AIClientList);
   
   $AIList = %clList;
   %aiMGR.add(%clList);
   
   %aiMGR.defineWeapons();
}

function AddMPBot(%dLevel, %team) {
   %nameCount = 0;
   while($AINames[%nameCount] !$= "") {
      %nameCount++;
   }
   
   %name = $AINames[getRandom(0, %nameCount-1)];
   %playerSpawnPoint = pickPlayerSpawnPoint($Game::DefaultPlayerSpawnGroups);
   
   %type = "Soldier"; //standard MP bot type
   
   //create a new AI "client" to hold the AI data
   %counter = 0;
   while(nameToID("AIClient_"@%counter) != -1) {
      %counter++;
   }
   %aiCL = new ScriptObject("AIClient_"@%counter) {
      name = %name;
      team = %team;
      score = 0;
      difficulty = %dLevel;
      player = 0;
      isTUAIClient = 1;
   };
   $AIList.add(%aiCL);
   AIManager.spawnAI(%aiCl, %type, %team, %playerSpawnPoint, %name, %dLevel);
   
   echo("AddMPBot() - AIClient Added: "@%aiCL@" - "@%name TAB %dLevel);
}

function TUAIManager::respawnAI(%this, %obj) {
   %name = %obj.namebase;
   %team = %obj.team;
   %type = %obj.type;
   %dl = %obj.difficulty;
   %client = %obj.client;
   //pick a spawnpoint
   %playerSpawnPoint = pickPlayerSpawnPoint($Game::DefaultPlayerSpawnGroups);
   //schedule the respawn of the next AI
   %this.schedule(Game.respawnTime * 1000, SpawnAI, %client, %type, %team, %playerSpawnPoint, %name, %dl);
}

function TUAIManager::SpawnAI(%this, %aiClient, %type, %team, %spawnPoint, %name, %dLevel) {
   echo("SpawnAI: "@%aiClient TAB %type TAB %team TAB %spawnPoint TAB %name);
   
   %block = %this.getArmorDatablock(%type);
   if(%block $= "") {
      error("SpawnAI: Invalid Type Specified");
      return 0;
   }
   
   %player = new AdvancedAIPlayer() {
      dataBlock = %block;
   };
   %player.setCurrentNavMesh(nameToID("theNavMesh"));    //setCurrentNavMesh only takes the mesh object id as a parameter, don't send it theNavMesh directly, use nameToID()
   
   %aiClient.player = %player;
   %player.client = %aiClient;
   
   %player.difficulty = %dLevel;
   %player.team = %team;
   %player.setTeam(%player.team);
   %player.isAI = 1;
   
   %player.namebase = %name;
   %player.name = %name;
   %player.type = %type;

   //are we moving to attack the player or are we moving towards a node
   %player.attack = 0;

   //are we stuck?
   %player.stuck = 0;

   //pause a moment to let behind the scene stuff work out
   %goal = pickPlayerSpawnPoint($Game::DefaultPlayerSpawnGroups);
   %player.schedule(1000, "aiStartUp", %goal);

   if(%name !$= "") {
      %player.setShapeName(%name);
   }
   %player.setShapeIcon("game/data/image/radar/playerDot.png\tgame/data/image/radar/playerArrow.png");
   
   %position = GameCore::pickPointInSpawnSphere(%player, %spawnPoint);
   %player.setTransform(%position);
   
   %this.giveAILoadout(%player);
   
   return %player;
}

function TUAIManager::getArmorDatablock(%this, %type) {
   switch$(%type) {
      //Basic AI: Used in Multiplayer & Singleplayer
      case "Soldier":
         return "GenericAI";
      default:
         return "GenericAI";
   }
}

function TUAIManager::giveAILoadout(%this, %player) {
   if(Game.customLoadoutsDefined || Game.noLoadouts) {
      //
      %selection = getRandom(0, Game.loadoutCounter);
      //
      %loadout = Game.getLoadout(%selection);
      %pri = getField(%loadout, 0);
      %sec = getField(%loadout, 1);
      %equ = getField(%loadout, 2);
   }
   else {
      //build a loadout from our weapons, pick a primary, secondary, and equipment
      %pINDX = getRandom(0, %this.primaryWeaponCounter);
      %sINDX = getRandom(0, %this.secondaryWeaponCounter);
      %eINDX = getRandom(0, %this.equipmentCounter);
      
      %pri = %this.primaries[%pINDX];
      %sec = %this.secondaries[%sINDX];
      %equ = %this.equipments[%eINDX];
   }
   
   //load the player
   %player.primaryWeapon = %pri;
   %player.secondaryWeapon = %sec;
   %player.equipment = %equ;

   %player.setInventory(%pri, 1);
   %player.setInventory(%pri.image.clip, %player.maxInventory(%pri.image.clip));
   %player.setInventory(%pri.image.ammo, %player.maxInventory(%pri.image.ammo));    // Start the gun loaded

   %player.setInventory(%sec, 1);
   %player.setInventory(%sec.image.clip, %player.maxInventory(%sec.image.clip));
   %player.setInventory(%sec.image.ammo, %player.maxInventory(%sec.image.ammo));    // Start the gun loaded

   %player.setInventory(%equ, %player.maxInventory(%equ));

   %player.use(%pri);
   %player.currentSlot = 1;
}

function TUAIManager::defineWeapons(%this) {
   %this.primaries[0] = "Lurker";
   %this.primaries[1] = "Model1887";
   %this.primaries[2] = "RPK423";
   
   %this.secondaries[0] = "Ryder";
   
   %this.equipments[0] = "ProxMine";
   %this.equipments[1] = "Sentry";
   
   %this.primaryWeaponCounter = 2;
   %this.secondaryWeaponCounter = 0;
   %this.equipmentCounter = 1;
}
