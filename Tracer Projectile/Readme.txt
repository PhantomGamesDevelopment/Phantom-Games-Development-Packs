Tracer Projectile Pack

To Install: Drop these two source files somewhere in Engine/Source and compile!

The following are Tracer Projectile Fields:

materialList - String Array: A list of materials that the tracer projectile will cycle through.

tracerLength - F32: A length factor of the Tracer Projectile. 

tracerWidth - F32: The width of the tracer projectile

interval - S32: The interval (in milliseconds) that the texture used in materialList will cycle.

tracerColor - ColorF: The color modulation applied to the material textures.

Here's a sample datablock:

datablock TracerProjectileData( SSENProjectile ) {
   projectileShapeName = "";

   directDamage        = 12;
   radiusDamage        = 0;
   damageRadius        = 0.5;
   areaImpulse         = 0.5;
   impactForce         = 1;
   
   interval            = 6.0;

   materialList[0]     = "art/shapes/weapons/LaserRifle/sniper00.png";
   tracerColor         = "1 0 0";
   tracerLength        = 0.5;
   tracerWidth         = 0.05;

   explosion           = BulletDirtExplosion;
   decal               = BulletHoleDecal;

   muzzleVelocity      = 120;
   velInheritFactor    = 1;

   armingDelay         = 0;
   lifetime            = 992;
   fadeDelay           = 1472;
   bounceElasticity    = 0;
   bounceFriction      = 0;
   isBallistic         = false;
   gravityMod          = 1;
};