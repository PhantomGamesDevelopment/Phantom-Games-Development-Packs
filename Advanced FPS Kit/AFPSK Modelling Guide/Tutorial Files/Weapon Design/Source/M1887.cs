//-----------------------------------------------------------------------------
// M1887.cs
// (c) Phantom Games Development 2014, All Rights Reserved
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Particles
// ----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Explosion
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Projectile Object
//-----------------------------------------------------------------------------
datablock ProjectileData( Model1887Projectile ) {
   projectileShapeName = "";

   directDamage        = 30;
   radiusDamage        = 0;
   damageRadius        = 0.5;
   areaImpulse         = 0.5;
   impactForce         = 1;

   explosion           = BulletDirtExplosion;
   decal               = BulletHoleDecal;

   muzzleVelocity      = 120;
   velInheritFactor    = 1;

   armingDelay         = 0;
   lifetime            = 350;
   fadeDelay           = 972;
   bounceElasticity    = 0;
   bounceFriction      = 0;
   isBallistic         = false;
   gravityMod          = 1;
};

function Model1887Projectile::onCollision(%this,%obj,%col,%fade,%pos,%normal) {
   // Apply impact force from the projectile.

   // Apply damage to the object all shape base objects
   if ( %col.getType() & $TypeMasks::GameBaseObjectType ) {
      %col.damage(%obj, %pos, %this.directDamage, "Model1887");
   }
}

//-----------------------------------------------------------------------------
// Ammo Item
//-----------------------------------------------------------------------------
datablock ItemData(Model1887Clip) {
   // Mission editor category
   category = "AmmoClip";
   className = "AmmoClip";

   // Basic Item properties
   shapeFile = "art/shapes/weapons/M1887/Model1887.DAE";
   mass = 1;
   elasticity = 0.2;
   friction = 0.6;

   // Dynamic properties defined by the scripts
   pickUpName = "Model 1887 clip";
   count = 1;
   maxInventory = 10;
};

datablock ItemData(Model1887Ammo) {
   // Mission editor category
   category = "Ammo";
   className = "Ammo";

   // Basic Item properties
   shapeFile = "art/shapes/weapons/M1887/Model1887.DAE";
   mass = 1;
   elasticity = 0.2;
   friction = 0.6;

   // Dynamic properties defined by the scripts
   pickUpName = "Model 1887 ammo";
   maxInventory = 5;
   clip = Model1887Clip;
};

//--------------------------------------------------------------------------
// Weapon Item
//-----------------------------------------------------------------------------
datablock ItemData(Model1887) {
   // Mission editor category
   category = "Weapon";
   className = "Weapon";

   // Basic Item properties
   shapeFile = "art/shapes/weapons/M1887/Model1887.DAE";
   mass = 1;
   elasticity = 0.2;
   friction = 0.6;
   emap = true;

   // Dynamic properties defined by the scripts
   PreviewImage = "Model1887.png";
   pickUpName = "Model 1887";
   description = "Model1887";
   image = Model1887WeaponImage;
   reticle = "crossHair";
   heldWeaponName = "Model 1887 Shotgun";
};


datablock ShapeBaseImageData(Model1887WeaponImage) {
   // Basic Item properties
   shapeFile = "art/shapes/weapons/M1887/Model1887.DAE";
   shapeFileFP = "art/shapes/weapons/M1887/Model1887.DAE";
   emap = true;

   imageAnimPrefix = "Disposition";
   imageAnimPrefixFP = "Shotgun";

   mountPoint = 0;
   firstPerson = true;
   useEyeNode = true;
   animateOnServer = true;

   correctMuzzleVector = true;

   class = "WeaponImage";
   className = "WeaponImage";
   
   ammoImage = "Shotgun";

   // Projectiles and Ammo.
   item = Model1887;
   ammo = Model1887Ammo;
   clip = Model1887Clip;
   
   projectile = Model1887Projectile;
   projectileType = Projectile;
   projectileSpread = "0.005";
   projectileNum = 5;

   casing = BulletShell;
   shellExitDir        = "1.0 0.3 1.0";
   shellExitOffset     = "0.15 -0.56 -0.1";
   shellExitVariance   = 15.0;
   shellVelocity       = 3.0;

   // Weapon lights up while firing
   lightType = "WeaponFireLight";
   lightColor = "0.992126 0.968504 0.708661 1";
   lightRadius = "8";
   lightDuration = "100";
   lightBrightness = 2;

   // Shake camera while firing.
   shakeCamera = false;
   camShakeFreq = "0 0 0";
   camShakeAmp = "0 0 0";

   useRemainderDT = true;

   // Initial start up state
   stateName[0]                     = "Preactivate";
   stateTransitionOnLoaded[0]       = "Activate";
   stateTransitionOnNoAmmo[0]       = "NoAmmo";

   // Activating the gun.  Called when the weapon is first
   // mounted and there is ammo.
   stateName[1]                     = "Activate";
   stateTransitionGeneric0In[1]     = "SprintEnter";
   stateTransitionOnTimeout[1]      = "Ready";
   stateTimeoutValue[1]             = 1.0;
   stateSequence[1]                 = "switch_in";
   stateSound[1]                    = DispositionSwitchinSound;

   // Ready to fire, just waiting for the trigger
   stateName[2]                     = "Ready";
   stateTransitionGeneric0In[2]     = "SprintEnter";
   stateTransitionOnMotion[2]       = "ReadyMotion";
   stateTransitionOnTimeout[2]      = "ReadyFidget";
   stateTimeoutValue[2]             = 10;
   stateWaitForTimeout[2]           = false;
   stateScaleAnimation[2]           = false;
   stateScaleAnimationFP[2]         = false;
   stateTransitionOnNoAmmo[2]       = "NoAmmo";
   stateTransitionOnTriggerDown[2]  = "Fire";
   stateSequence[2]                 = "idle";

   // Same as Ready state but plays a fidget sequence
   stateName[3]                     = "ReadyFidget";
   stateTransitionGeneric0In[3]     = "SprintEnter";
   stateTransitionOnMotion[3]       = "ReadyMotion";
   stateTransitionOnTimeout[3]      = "Ready";
   stateTimeoutValue[3]             = 1;
   stateWaitForTimeout[3]           = false;
   stateTransitionOnNoAmmo[3]       = "NoAmmo";
   stateTransitionOnTriggerDown[3]  = "Fire";
   stateSequence[3]                 = "idle_fidget1";
   stateSound[3]                    = DispositionCockSound;

   // Ready to fire with player moving
   stateName[4]                     = "ReadyMotion";
   stateTransitionGeneric0In[4]     = "SprintEnter";
   stateTransitionOnNoMotion[4]     = "Ready";
   stateWaitForTimeout[4]           = false;
   stateScaleAnimation[4]           = false;
   stateScaleAnimationFP[4]         = false;
   stateSequenceTransitionIn[4]     = true;
   stateSequenceTransitionOut[4]    = true;
   stateTransitionOnNoAmmo[4]       = "NoAmmo";
   stateTransitionOnTriggerDown[4]  = "Fire";
   stateSequence[4]                 = "run";

   // Fire the weapon. Calls the fire script which does
   // the actual work.
   stateName[5]                     = "Fire";
   stateTransitionGeneric0In[5]     = "SprintEnter";
   stateTransitionOnTimeout[5]      = "Reload";
   stateTimeoutValue[5]             = 0.33;
   stateFire[5]                     = true;
   stateRecoil[5]                   = "";
   stateAllowImageChange[5]         = false;
   stateSequence[5]                 = "fire";
   stateScaleAnimation[5]           = true;
   stateSequenceNeverTransition[5]  = true;
   stateSequenceRandomFlash[5]      = true;        // use muzzle flash sequence
   stateScript[5]                   = "onFire";
   stateEmitter[5]                  = GunFireSmokeEmitter;
   stateEmitterTime[5]              = 0.025;
   stateSound[5]                    = DispositionFireSound;

   // Play the reload animation, and transition into
   stateName[6]                     = "Reload";
   stateTransitionGeneric0In[6]     = "SprintEnter";
   stateTransitionOnNoAmmo[6]       = "NoAmmo";
   stateTransitionOnTimeout[6]      = "Cocking";
   stateWaitForTimeout[6]           = "0";
   stateTimeoutValue[6]             = 0.0;
   stateAllowImageChange[6]         = false;
   
   // Cock the shotgun
   stateName[7]                     = "Cocking";
   stateTransitionGeneric0In[7]     = "SprintEnter";
   stateTransitionOnNoAmmo[7]       = "NoAmmo";
   stateTransitionOnTimeout[7]      = "Ready";
   stateTimeoutValue[7]             = 0.75;
   stateWaitForTimeout[7]           = true;
   stateSequence[7]                 = "fire_cocking";
   stateEjectShell[7]               = true;
   stateAllowImageChange[7]         = false;
   stateSound[7]                    = DispositionCockSound;
   
   // No ammo in the weapon, just idle until something
   // shows up. Play the dry fire sound if the trigger is
   // pulled.
   stateName[8]                     = "NoAmmo";
   stateTransitionGeneric0In[8]     = "SprintEnter";
   stateTransitionOnMotion[8]       = "NoAmmoMotion";
   stateTransitionOnAmmo[8]         = "ReloadClip";
   stateTimeoutValue[8]             = 0.1;   // Slight pause to allow script to run when trigger is still held down from Fire state
   stateScript[8]                   = "onClipEmpty";
   stateTransitionOnTriggerDown[8]  = "DryFire";
   stateSequence[8]                 = "idle";
   stateScaleAnimation[8]           = false;
   stateScaleAnimationFP[8]         = false;
   
   stateName[9]                     = "NoAmmoMotion";
   stateTransitionGeneric0In[9]     = "SprintEnter";
   stateTransitionOnNoMotion[9]     = "NoAmmo";
   stateWaitForTimeout[9]           = false;
   stateScaleAnimation[9]           = false;
   stateScaleAnimationFP[9]         = false;
   stateSequenceTransitionIn[9]     = true;
   stateSequenceTransitionOut[9]    = true;
   stateTransitionOnAmmo[9]         = "ReloadClip";
   stateTransitionOnTriggerDown[9]  = "DryFire";
   stateSequence[9]                 = "run";

   // No ammo dry fire
   stateName[10]                     = "DryFire";
   stateTransitionGeneric0In[10]     = "SprintEnter";
   stateTimeoutValue[10]             = 1.0;
   stateTransitionOnTimeout[10]      = "NoAmmo";
   stateScript[10]                   = "onDryFire";

   // Play the reload clip animation
   stateName[11]                     = "ReloadClip";
   stateTransitionGeneric0In[11]     = "SprintEnter";
   stateTransitionOnTimeout[11]      = "Ready";
   stateWaitForTimeout[11]           = true;
   stateTimeoutValue[11]             = 3.0;
   stateReload[11]                   = true;
   stateSequence[11]                 = "reload";
   stateShapeSequence[11]            = "Reload";
   stateScaleShapeSequence[11]       = true;
   stateSound[11]                    = DispositionReloadSound;

   // Start Sprinting
   stateName[12]                    = "SprintEnter";
   stateTransitionGeneric0Out[12]   = "SprintExit";
   stateTransitionOnTimeout[12]     = "Sprinting";
   stateWaitForTimeout[12]          = false;
   stateTimeoutValue[12]            = 0.5;
   stateWaitForTimeout[12]          = false;
   stateScaleAnimation[12]          = false;
   stateScaleAnimationFP[12]        = false;
   stateSequenceTransitionIn[12]    = true;
   stateSequenceTransitionOut[12]   = true;
   stateAllowImageChange[12]        = false;
   stateSequence[12]                = "sprint";

   // Sprinting
   stateName[13]                    = "Sprinting";
   stateTransitionGeneric0Out[13]   = "SprintExit";
   stateWaitForTimeout[13]          = false;
   stateScaleAnimation[13]          = false;
   stateScaleAnimationFP[13]        = false;
   stateSequenceTransitionIn[13]    = true;
   stateSequenceTransitionOut[13]   = true;
   stateAllowImageChange[13]        = false;
   stateSequence[13]                = "sprint";
   
   // Stop Sprinting
   stateName[14]                    = "SprintExit";
   stateTransitionGeneric0In[14]    = "SprintEnter";
   stateTransitionOnTimeout[14]     = "Ready";
   stateWaitForTimeout[14]          = false;
   stateTimeoutValue[14]            = 0.5;
   stateSequenceTransitionIn[14]    = true;
   stateSequenceTransitionOut[14]   = true;
   stateAllowImageChange[14]        = false;
   stateSequence[14]                = "sprint";
};
