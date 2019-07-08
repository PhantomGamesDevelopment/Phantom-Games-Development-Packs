//Attack Datablock
$Zombie::DamageFactor = 1;

$melee_check2hit =
      $TypeMasks::VehicleObjectType |
      $TypeMasks::PlayerObjectType |
      $TypeMasks::TerrainObjectType |
      $TypeMasks::StaticTSObjectType |
      $TypeMasks::StaticShapeObjectType |
      $TypeMasks::ForestObjectType;

function Melee_Attack(%obj) {
   %name = "Attack"@getRandom(1, 2);
   %obj.setActionThread(%name);

   %eyeVec = %obj.getEyeVector();

   %startPos = %obj.getEyePoint();
   %endPos = VectorAdd(%startPos, VectorScale(%eyeVec, 2));

   %target = ContainerRayCast(%startPos, %endPos, $melee_check2hit, %obj);
   %col = firstWord(%target);

   if(%col == 0) {
      return;
   }
   
   if($Zombie::DamageFactor == 0) {
      $Zombie::DamageFactor = 1;
   }

   // Apply damage to the object all shape base objects
   if (%col.getType() & $TypeMasks::ShapeBaseObjectType) {
      %col.damage(%obj, %pos, 15*$Zombie::DamageFactor, "Boomer Slap");

      %vpos = %col.getWorldBoxCenter();
      %pushDirection = VectorSub(%vpos,%obj.getWorldBoxCenter());
      %pushDirection = VectorNormalize(%pushDirection);

      %pushVec = VectorScale(%pushDirection,1000);
      %pushVec = getwords(%pushVec,0,1);
      %col.applyImpulse(%vpos, %pushVec);
   }
}

//Zombie Player: Primary Datablock
datablock PlayerData(ZombiePlayerData : DefaultPlayerData) {
   shapeFile = "art/shapes/actors/GG_zombie/Zombie.DAE";
   scale = "1 1 1";
};

// Zombie Player: Secondary Inherited Datablocks
datablock PlayerData(Zombie02PlayerData : ZombiePlayerData) {
   shapeFile = "art/shapes/actors/GG_zombie/Zombie_02.DAE";
};

datablock PlayerData(Zombie03PlayerData : ZombiePlayerData) {
   shapeFile = "art/shapes/actors/GG_zombie/Zombie_03.DAE";
};

datablock PlayerData(Zombie04PlayerData : ZombiePlayerData) {
   shapeFile = "art/shapes/actors/GG_zombie/Zombie_04.DAE";
};

datablock PlayerData(Zombie05PlayerData : ZombiePlayerData) {
   shapeFile = "art/shapes/actors/GG_zombie/Zombie_05.DAE";
};

datablock PlayerData(MegaZombiePlayerData : ZombiePlayerData) {
   shapeFile = "art/shapes/actors/GG_zombie/MegaZombie.DAE";
   maxDamage = "160";
   scale = "2 2 1.8";
};


//Navegation Interfacing
function AiPlayer::createNewPath(%this) {
   %path = new NavPath() {
      from = %this.getPosition();
      to = "10 1 1";
      mesh = "Nav";
   };
   MissionCleanup.add(%path);
   
   %this.path = %path;
   
   return %path;
}

function getRandomPosition(%scale, %ignore_z_comp) {
   %neg = getRandom(0, 1);
   %factor = 1;
   if(%neg) {
      %factor = -1;
   }
   
   %x = %factor * getRandom(1, %scale);
   %y = %factor * getRandom(1, %scale);
   %z = %factor * getRandom(1, %scale);
   
   if(%ignore_z_comp) {
      return %x SPC %y SPC 0;
   }
   else {
      return %x SPC %y SPC %z;
   }
}

// Zombie Spawn Function
function spawnZombie(%number, %position, %absoluteSpawn) {
   if(%number == 1) {
      %block = "ZombiePlayerData";
   }
   else if(%number == 6) {
      %block = "MegaZombiePlayerData";
   }
   else {
      %block = "Zombie0"@%number@"PlayerData";
   }
   
   if(%number < 1 || %number > 6) {
      %block = "ZombiePlayerData";
   }

   %player = new AiPlayer() {
      dataBlock = %block;
      position = %position;
   };
   MissionCleanup.add(%player);
   
   %player.isZombie = true;
   
	if($Zombie::SpeedFactor == 0) {
	   $Zombie::SpeedFactor = 1;
	}
   %player.setMoveSpeed(0.5 + ($Zombie::SpeedFactor / 10));
   //%player.doStand();
   //%player.setActionThread("run2");
   
   %path = %player.createNewPath();

	%spawnLocation = %position;
	if(!%absoulteSpawn) {
      %spawnLocation = vectorAdd(getRandomPosition(8, 1), %position);
	}
   %player.setTransform(%spawnLocation);
   game.zombieGroup.add(%player, %path);

   %player.stuck = 0;

   %player.currentNode = 0;
   %player.targetNode = 0;

   %player.schedule(1000, "aiStartUp");

   return %player;
}

function AIPlayer::aiStartUp(%this) {
   if(!isObject(%this) || %this.getState() $= "Dead") {
      return;
   }
	if(Game.isTutorial) {
	   return; //no zombie attacks in tutorial mode.
	}
   // Initialize the zombie's parameters
   %path = %this.path;
   // Begin the combat routine
   %enemy = %this.getNearestPlayerTarget();
   if(%enemy == -1) {
      %this.pathUpdate("rand");
   }
   else {
      %this.pathUpdate(%enemy);
   }
   
   %this.zombieHitDetect();
}

function AIPlayer::zombieHitDetect(%this) {
   if(!isObject(%this) || %this.getState() $= "Dead") {
      return;
   }
   //push the updated position
   %enemy = %this.getNearestPlayerTarget();
   %path = %this.path;
   
   //echo("Zombie Hit Detect: "@%this@" -> "@%enemy@"");
   if(%enemy == -1) {
      %this.pathUpdate("rand", 1);
   }
   else {
      %this.pathUpdate(%enemy, 1);
      // check for 3m distance
      if(%this.getTargetDistance(%enemy) <= 3) {
         //check for visual hit
         if(%this.playerLOS(%enemy)) {
            Melee_Attack(%this);
         }
      }
   }
   %this.schedule(500, zombieHitDetect);
}

function AIPlayer::pathUpdate(%this, %tObj) {
   if(!isObject(%this) || %this.getState() $= "dead") {
      return;
   }
   %path = %this.path;
   %myP = %this.getPosition();
   
   %path.from = %myP;
   if(%tObj $= "rand" || !isObject(%tObj) || %tObj.getState() $= "dead") {
      //path out a random position
      %path.to = vectorAdd(%this.getPosition(), getRandomPosition(5, 1));
   }
   else {
      %path.to = %tObj.getPosition();
      if(%this.playerLOS(%tObj)) {
         //skip the re-cast and attempt to run directly at the player.
         %this.setMoveDestination(%tObj.getTransform(), true);
         return;
      }
   }
   //follow the path
	%path.plan();
   %this.currentNode = 0;
   %this.followPath(%path, 1024);
}

//******************************************************************************
//******************************************************************************
//******************************************************************************
//                   Zombie Block Functions                                   \\
//******************************************************************************
//******************************************************************************
//******************************************************************************
function ZombiePlayerData::onReachDestination(%this,%obj) {
   //echo( %obj @ " onReachDestination" );

   // Moves to the next node on the path.
   // Override for all player.  Normally we'd override this for only
   // a specific player datablock or class of players.
   if (%obj.path !$= "") {
      if (%obj.currentNode == %obj.targetNode)
         %this.onEndOfPath(%obj,%obj.path);
      else
         %obj.moveToNextNode();
   }
}

function ZombiePlayerData::onMoveStuck(%this,%obj) {
   //echo( %obj @ " onMoveStuck" );
   %obj.stuck++;
   if(%obj.stuck >= 20) {
      //%obj.stop();
   
      %obj.stuck = 0;
   }
}

function ZombiePlayerData::onTargetExitLOS(%this,%obj) {
   //echo( %obj @ " onTargetExitLOS" );
}

function ZombiePlayerData::onTargetEnterLOS(%this, %obj) {
   //echo( %obj @ " onTargetEnterLOS" );
}

function ZombiePlayerData::onEndOfPath(%this, %obj, %path) {
   %obj.nextTask();
}

function ZombiePlayerData::onEndSequence(%this, %obj, %slot) {
   //echo("Sequence Done!");
   %obj.stopThread(%slot);
   %obj.nextTask();
}
