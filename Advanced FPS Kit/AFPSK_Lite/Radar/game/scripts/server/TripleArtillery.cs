//TripleArtillery.cs
//Robert Fritzen
//Designate Three Points to be Struck By Artillery :P

//Datablocks
datablock ProjectileData(ArtilleryBombProjectile) {
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

   damageType = "ArtilleryBomb";
};

//Code
function Player::useArtillery(%player) {
   %player.client.returnedSelector = "Artillery";
   %player.client.artilleryCalls = 3; //3 shots
   commandToClient(%player.client, 'PushMapSelector', 0); //<-- Only need position for artillery :)
}

function launchSingleShell(%source, %target) {
   %finalPos = vectorAdd(%target, "0 0 100");
   //Spawn a downward moving shell...
   %p = new (Projectile)() {
       dataBlock        = ArtilleryBombProjectile;
       initialDirection = "0 0 -5";
       initialPosition  = %finalPos;
       sourceSlot       = 0;
   };
   MissionCleanup.add(%p);
   %p.sourceObject = %source;
}
