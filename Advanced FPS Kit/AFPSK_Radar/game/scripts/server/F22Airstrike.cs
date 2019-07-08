//F22Airstrike.cs
//Robert C Fritzen

//NOTE NOTE NOTE: For some reason applyImpulse is currently bugged to the point
// that your game will freeze if you try the airstrike, just use the arillery example instead
// for now.

//Datablocks
datablock ProjectileData(F22_BombProjectile) {
   projectileShapeName = "art/shapes/weapons/GrenadeLauncher/rocket.dts";
   directDamage = 150;
   radiusDamage = 150;
   damageRadius = 15;
   areaImpulse = 2000;

   explosion = GrenadeLauncherExplosion;
   waterExplosion = GrenadeLauncherWaterExplosion;

   decal = ScorchRXDecal;
   splash = GrenadeSplash;

   particleEmitter = GrenadeProjSmokeTrailEmitter;
   particleWaterEmitter = GrenadeTrailWaterEmitter;

   muzzleVelocity = 30;
   velInheritFactor = 0.3;

   armingDelay = 500;
   lifetime = 10000;
   fadeDelay = 4500;

   bounceElasticity = 0.4;
   bounceFriction = 0.3;
   isBallistic = true;
   gravityMod = 0.9;

   lightDesc = GrenadeLauncherLightDesc;

   damageType = "AirstrikeBomb";
};

datablock FlyingVehicleData(F22_JetBomber) {
   spawnOffset = "0 0 2";
   canControl = false;
   catagory = "Vehicles";
   shapeFile = "art/shapes/vehicles/F22/F22.DAE";
   multipassenger = false;
   computeCRC = true;

   drag    = 0.15;
   density = 1.0;

   mountPose[0] = sitting;
   numMountPoints = 1;
   isProtectedMountPoint[0] = false;
   cameraMaxDist = 15;
   cameraOffset = 2.5;
   cameraLag = 0.9;
   
   isInvincible = true;
   
   maxDamage = 10000;
   maxEnergy = 500;      // Afterburner and any energy weapon pool
   rechargeRate = 4;

   minDrag = 25;           // Linear Drag (eventually slows you down when not thrusting...constant drag)
   rotationalDrag = 900;        // Anguler Drag (dampens the drift after you stop moving the mouse...also tumble drag)

   maxAutoSpeed = 57;       // Autostabilizer kicks in when less than this speed. (meters/second)
   autoAngularForce = 400;       // Angular stabilizer force (this force levels you out when autostabilizer kicks in)
   autoLinearForce = 1;        // Linear stabilzer force (this slows you down when autostabilizer kicks in)
   autoInputDamping = 0.8;      // Dampen control input so you don't` whack out at very slow speeds

   // Maneuvering
   maxSteeringAngle = 7.0;    // Max radiens you can rotate the wheel. Smaller number is more maneuverable.
   horizontalSurfaceForce = 6;   // Horizontal center "wing" (provides "bite" into the wind for climbing/diving and turning)
   verticalSurfaceForce = 4;     // Vertical center "wing" (controls side slip. lower numbers make MORE slide.)
   maneuveringForce = 4750;      // Horizontal jets (W,S,D,A key thrust)
   steeringForce = 587;         // Steering jets (force applied when you move the mouse)
   steeringRollForce = 3000;      // Steering jets (how much you heel over when you turn)
   rollForce = 1;                // Auto-roll (self-correction to right you after you roll/invert)
   hoverHeight = 2.5;        // Height off the ground at rest
   createHoverHeight = 1;  // Height off the ground when created
   maxForwardSpeed = 190;  // speed in which forward thrust force is no longer applied (meters/second)

   // Turbo Jet
   jetForce = 2000;      // Afterburner thrust (this is in addition to normal thrust)
   minJetEnergy = 40;     // Afterburner can't be used if below this threshhold.
   jetEnergyDrain = 12;       // Energy use of the afterburners (low number is less drain...can be fractional)                                                                                                                                                                                                                                                                                          // Auto stabilize speed
   vertThrustMultiple = 0.0;

   // Rigid body
   mass = 180;        // Mass of the vehicle
   bodyFriction = 0;     // Don't mess with this.
   bodyRestitution = 0.5;   // When you hit the ground, how much you rebound. (between 0 and 1)
   minRollSpeed = 0;     // Don't mess with this.
   softImpactSpeed = 14;       // Sound hooks. This is the soft hit.
   hardImpactSpeed = 25;    // Sound hooks. This is the hard hit.

   // Ground Impact Damage (uses DamageType::Ground)
   minImpactSpeed = 20;      // If hit ground at speed above this then it's an impact. Meters/second
   speedDamageScale = 0.06;

   // Object Impact Damage (uses DamageType::Impact)
   collDamageThresholdVel = 23.0;
   collDamageMultiplier   = 0.02;

   //
   minTrailSpeed = 150;      // The speed your contrail shows up at.
   //trailEmitter = ContrailEmitter;
   //forwardJetEmitter = FlyerJetEmitter;
   //downJetEmitter = FlyerJetEmitter;

   // Sounds
   engineSound = cheetahEngine;
   //squealSound = cheetahSqueal;
   softImpactSound = softImpact;
   hardImpactSound = hardImpact;

   dustEmitter = CheetahTireEmitter;
   triggerDustHeight = 4.0;
   dustHeight = 1.0;

   minMountDist = 7;

   splashEmitter[0] = VehicleFoamDropletsEmitter;
   splashEmitter[1] = VehicleFoamEmitter;
   
   nameTag = 'F22 Fighter';
};

//Script
function Player::useAirstrike(%player) {
   %player.client.returnedSelector = "Airstrike";
   commandToClient(%player.client, 'PushMapSelector', 1);
}

function prepareAirstrike(%source, %pos1, %pos2) {
   //%pos1 = Target Position
   //%pos2 = Second Marked Position, Used to construct approach vector
   //First thing we need to do is to determine the approach vector, and where to spawn.
   %offset = 500; //How far away from the inverted approach vector do we spawn?
   %approachVec = vectorNormalize(vectorSub(%pos2, %pos1));
   %spawnPosition = vectorAdd(%pos1, vectorScale((%approachVec), -1 * %offset));
   %spawnPosition = vectorAdd(%spawnPosition, "0 0 "@150 + getTerrainHeight(%spawnPosition));
   %trueTarget = getWords(%pos1, 0, 1) @" "@ getTerrainHeight(%pos1);
   //Now we can spawn our bomber
   %Bomber = new FlyingVehicle() {
      dataBlock = F22_JetBomber;
      position = %spawnPosition;
   };
   %Bomber.setTeam(%source.getTeam());
   %Bomber.setShapeIcon("game/data/image/radar/jet.png");
   echo("Go...");
   AirstrikeLoop(%Bomber, %approachVec, %trueTarget);
   %Bomber.schedule(20000, "Delete");
}

function AirstrikeLoop(%Bomber, %approachVec, %trueTarget) {
   echo("AS Loop "@%Bomber@" "@%approachVec@" "@%trueTarget);
   if(!isObject(%Bomber)) {
      return;
   }
   //Move along the approach vector
   if(vectorLen(%Bomber.getVelocity()) < 50) { //Acts as a speed cap on the velocity of the bomber
      echo("Speed...");
      %Bomber.applyImpulse(%Bomber.getPosition(), vectorScale(%approachVec, 1));
   }
   if(!%Bomber.bombing) {
      if(vectorDist(%Bomber.getPosition(), %trueTarget) < 400) {
         echo("Begin Bombing");
         %Bomber.bombing = 1;
      }
   }
   else {
      //Drop a bomb every 5 ticks
      %Bomber.ticks++;
      if(%Bomber.ticks == 5) {
         %Bomber.ticks = 0;
         //Bomb!
         echo("Bomb....");
         %p = new (Projectile)() {
            dataBlock        = F22_BombProjectile;
            initialDirection = "0 0 -5";
            initialPosition  = vectorAdd(%Bomber.getPosition(), "0 0 -5");
            sourceObject     = %Bomber;
            sourceSlot       = 0;
         };
         MissionCleanup.add(%p);
      }
      //
      if(vectorDist(%Bomber.getPosition(), %trueTarget) > 400) {
         echo("end Bombing");
         //Stop Bombing Now...
         %Bomber.bombing = 0;
      }
   }
   schedule(150, 0, AirstrikeLoop, %Bomber, %approachVec, %trueTarget);
}
