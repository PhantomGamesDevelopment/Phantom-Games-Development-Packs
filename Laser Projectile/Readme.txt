Laser Projectile Readme

To install, simply drop these two source files in your Engine/Source directory somewhere and recompile
the engine to use the new code. THen you can create a laser datablock using the new fields below. A sample
is provided at the bottom.

The following are Datablock Fields accepted on the LaserProjectile:

beamStartRadius - F32: Declares the starting width of the laser beam when it fires from the gun

beamEndRadius - F32: Declares the end width of the laser beam (essentially allows you to create beams
that appear to get smaller, or larger as you move along the beam)

materialList - String Array: Allows you to define up to 20 materials that the beam will use when rendering.
See (interval)

range - F32: The maximum range of the laser projectile beam.

interval - S32: Time in miliseconds between the usage of the materials defined in materialList.

conHitBeam - Bool: Allows the beam to continue doing damage to targets that collide with it as long
as the beam remains in the rending state.

conMoveBeam - Bool: Allows the beam to continue firing from the gun during it's lifetime.

beamColor - ColorF: Declares the color modulation applied to the materials.

fadeBegin - S32 time in milliseconds (prior to lifetime) before the fade effect begins on the laser beam.


Laser Projectile Sample:

datablock LaserProjectileData( LaserRifleProjectile ) {
   projectileShapeName = "";

   directDamage        = 100;
   radiusDamage        = 0;
   damageRadius        = 0.5;
   areaImpulse         = 0.5;
   impactForce         = 1;
   
   conHitBeam          = false;
   conMoveBeam         = false;

   explosion           = BulletDirtExplosion;
   decal               = BulletHoleDecal;
   
   beamStartRadius     = 0.05;
   beamEndRadius       = 0.15;
   range               = 500.0;
   interval            = 6.0;
   
   materialList[0]     = "art/shapes/weapons/LaserRifle/sniper00.png";
   
   beamColor           = "1 0 0";
   
   fadeBegin           = 350;

   muzzleVelocity      = 120;
   velInheritFactor    = 1;

   armingDelay         = 0;
   lifetime            = 700;
   fadeDelay           = 800;
   bounceElasticity    = 0;
   bounceFriction      = 0;
   isBallistic         = false;
   gravityMod          = 1;
};