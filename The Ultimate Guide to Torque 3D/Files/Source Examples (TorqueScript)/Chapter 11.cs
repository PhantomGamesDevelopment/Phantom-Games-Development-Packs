//Chapter 11 Code Samples
//The Ultimate Guide To Torque 3D
// By Robert C. Fritzen

function triggerClass::onEnterTrigger(%this, %triggerObj, %objEntering) {
   //Per-Player Based.
   if(!%objEntering.hasTriggered[%triggerObj]) {
      //Do the event.
      %objEntering.hasTriggered[%triggerObj] = true;
   }
   //Per-Trigger Based.
   if(!%triggerObj.hasTriggered) {
      //Do the event.
      %triggerObj.hasTriggered = true;
   }
}

function triggerClass::onLeaveTrigger(%this, %triggerObj, %objLeaving) {
   //Do code here
}

function triggerClass::onTickTrigger (%this, %triggerObj) {
   //Code called every %triggerObj.tickPeriodMS.
   //Asset functions for getting objects: %triggerObj.getNumObjects() & %triggerObj.getObject(%i)
}

function spawnAPlayer(%position) {
   %player = new Player() {
       datablock = "DefaultPlayerData";
      position = %position;
   };
   MissionCleanup.add(%player); //<- Game objects belong in MissionGroup/MissionCleanup.
   return %player;
}

function spawnAPlayer2(%position, %rotation) {
   if(%rotation $= "") {
      %rotation = "0 0 0 0";
   }
   %player = new Player() {
       datablock = "DefaultPlayerData";
   };
   %player.setTransform(%position SPC %rotation);
   MissionCleanup.add(%player); //<- Game objects belong in MissionGroup/MissionCleanup.
   return %player;
}

function findPoseOf(%player) {
   return %player.getPose();
}

function makeObjectMine(%player, %target) {
   %player.setControlObject(%target);
   echo("Player "@%player@" now controls "@%target);
}

function PlayerData::onPoseChange(%this, %player, %oldPose, %newPose) {
   echo("Player "@%player@" is changing from "@%oldPose@" to "@%newPose);
}

function WeaponImage::onFire(%this, %obj, %slot) {
   // Make sure we have valid data
   if (!isObject(%this.projectile)) {
      error("WeaponImage::onFire() - Invalid projectile datablock");
      return;
   }
   // Decrement inventory ammo. The image's ammo state is updated
   // automatically by the ammo inventory hooks.
   if ( !%this.infiniteAmmo )
      %obj.decInventory(%this.ammo, 1);

   // Get the player's velocity, we'll then add it to that of the projectile
   %objectVelocity = %obj.getVelocity();

   %numProjectiles = %this.projectileNum;
   if (%numProjectiles == 0)
      %numProjectiles = 1;

   for (%i = 0; %i < %numProjectiles; %i++) {
      if (%this.projectileSpread) {
         // We'll need to "skew" this projectile a little bit.  We start by
         // getting the straight ahead aiming point of the gun
         %vec = %obj.getMuzzleVector(%slot);

         // Then we'll create a spread matrix by randomly generating x, y, and z
         // points in a circle
         %matrix = "";
         for(%j = 0; %j < 3; %j++)
            %matrix = %matrix @ (getRandom() - 0.5) * 2 * 3.1415926 * %this.projectileSpread @ " ";
         %mat = MatrixCreateFromEuler(%matrix);

         // Which we'll use to alter the projectile's initial vector with
         %muzzleVector = MatrixMulVector(%mat, %vec);
      }
      else {
         // Weapon projectile doesn't have a spread factor so we fire it using
         // the straight ahead aiming point of the gun
         %muzzleVector = %obj.getMuzzleVector(%slot);
      }

      // Add player's velocity
      %muzzleVelocity = VectorAdd(
         VectorScale(%muzzleVector, %this.projectile.muzzleVelocity),
         VectorScale(%objectVelocity, %this.projectile.velInheritFactor));

      // Create the projectile object
      %p = new (%this.projectileType)() {
         dataBlock = %this.projectile;
         initialVelocity = %muzzleVelocity;
         initialPosition = %obj.getMuzzlePoint(%slot);
         sourceObject = %obj;
         sourceSlot = %slot;
         client = %obj.client;
         sourceClass = %obj.getClassName();
      };
      MissionCleanup.add(%p);
   }
}

function PhotonLaserImage::onCharge(%data, %obj, %slot) {
   if(%obj.chargeCountPhotonLaser $= "") {
      %obj.chargeCountPhotonLaser = 0;
   }
   %obj.chargeCountPhotonLaser++;
   if(%obj.chargeCountPhotonLaser > 100) {
      %obj.chargeCountPhotonLaser = 100;
   }
}

function PhotonLaserImage::onFire(%data, %obj, %slot) {
   if(%obj.chargeCountPhotonLaser == 100) {
      Parent::onFire(%data, %obj, %slot);
   }
   %obj.chargeCountPhotonLaser = 0;
}

//NOTE NOTE NOTE
// PLEASE READ CH.11, THIS CODE IS AN EXAMPLE OF SOMETHING THAT WILL NOT WORK
function multipleTypemaskExample1(%object) {
   %typemask = %object.getType();
   if(%typemask == $TypeMasks::ShapeBaseObjectType && %typemask == $TypeMasks::VehicleObjectType) {
      //Do something?
   }
}
// END NOTE

function multipleTypemaskExample2(%object) {
   %typemask = %object.getType();
   if(%typemask & ($TypeMasks::ShapeBaseObjectType | $TypeMasks::VehicleObjectType)) {
      //Do something?
   }
}

function multipleTypemaskExample3(%object) {
   %typemask = %object.getType();
   if(%typemask | ($TypeMasks::ShapeBaseObjectType | $TypeMasks::VehicleObjectType)) {
      //Do something?
   }
}

function raycastExampleOne(%startPoint, %endPoint, %searchMask) {
   %raycast = containerRaycast(%startPoint, %endPoint, %searchMask, -1, false);
   %tObj = getWord(%raycast, 0);
   if(isObject(%tObj)) {
      %tPos = getWords(%raycast, 1, 3);
      %tNorm = getWords(%raycast, 4, 3);
   }
}

function raycastExampleTwo(%object, %distanceFromObject, %searchMask) {
   %startPoint = %object.getEyePoint();
   %eyeVec = %object.getEyeVector();
   %endPoint = vectorAdd(%startPoint, vectorScale(%eyeVec, %distanceFromObject));
   %raycast = containerRaycast(%startPoint, %endPoint, %searchMask, -1, false);
   %tObj = getWord(%raycast, 0);
   if(isObject(%tObj)) {
      %tPos = getWords(%raycast, 1, 3);
      %tNorm = getWords(%raycast, 4, 3);
   }
}

function radiusSearchExample(%startPoint, %radius, %searchMask) {
   initContainerRadiusSearch(%startPoint, %radius, %searchMask, false);
   while(%tObj = containerSearchNext()) {
      //Do something?
   }
}

function typeSearchExample(%searchMask) {
   initContainerTypeSearch(%searchMask, false);
   while(%tObj = containerSearchNext()) {
      //Do something?
   }
}

function AISpawnExample(%position) {
   %aiPlayer = new AIPlayer() {
      datablock = GenericAI;
   };
   %aiPlayer.setTransform(%position SPC @"0 0 0 0");
   return %aiPlayer;
}

function TutorialAI::onReachDestination(%this, %obj) {
   if (%obj.path !$= "") {
      if (%obj.currentNode == %obj.targetNode) {
         %this.onEndOfPath(%obj, %obj.path);
      }
      else {
         %obj.moveToNextNode();
      }
   }
}

function TutorialAI::onMoveStuck(%this, %obj) {
   //Do Something?
}

function TutorialAI::onEndOfPath(%this, %obj, %path) {
   %obj.aiDecide();
}

function AITutorialDemo(%position) {
   %aiPlayer = new AIPlayer() {
      datablock = TutorialAI;
   };
   %aiPlayer.setTransform(%position SPC @"0 0 0 0");
   return %aiPlayer;
}

function TutorialAI::followPath(%this, %path, %node) {
   if (!isObject(%path)) {
      %this.path = "";
      return;
   }
   if (%node > %path.getCount() - 1) {
      %this.targetNode = %path.getCount() - 1;
   }
   else {
      %this.targetNode = %node;
   }
   if (%this.path $= %path) {
      %this.moveToNode(%this.currentNode);
   }
   else {
      %this.path = %path;
      %this.moveToNode(0);
   }
}

function TutorialAI::moveToNextNode(%this) {
   if (%this.targetNode < 0 || %this.currentNode < %this.targetNode) {
      if (%this.currentNode < %this.path.getCount() - 1) {
         %this.moveToNode(%this.currentNode + 1);
      }
      else {
         %this.moveToNode(0);
      }
   }
   else {
      if (%this.currentNode == 0) {
         %this.moveToNode(%this.path.getCount() - 1);
      }
      else {
         %this.moveToNode(%this.currentNode - 1);
      }
   }
}

function TutorialAI::moveToNode(%this, %index) {
   // Move to the given path node index
   %this.currentNode = %index;
   %node = %this.path.getObject(%index);
   %this.setMoveDestination(%node.getTransform());
}

function DoDemoLevelAI() {
   %spawnPosition = getWords(AISpawn1.getTransform(), 0, 2);
   %demoAI = AITutorialDemo(%spawnPosition);
   %demoAI.followPath(AIPath1, -1);
}
