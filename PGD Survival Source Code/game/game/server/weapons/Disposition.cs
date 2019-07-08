//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Sounds
//--------------------------------------------------------------------------

datablock SFXProfile(DispositionFireSound)
{
   filename = "art/sound/soldier_gun_pack/wpn_disposition_fire";
   description = AudioClose3D;
   preload = true;
};

datablock SFXProfile(DispositionReloadSound)
{
   filename = "art/sound/soldier_gun_pack/wpn_disposition_reload";
   description = AudioClose3D;
   preload = true;
};

datablock SFXProfile(DispositionSwitchinSound)
{
   filename = "art/sound/soldier_gun_pack/wpn_disposition_switchin";
   description = AudioClose3D;
   preload = true;
};

datablock SFXProfile(DispositionCockSound)
{
   filename = "art/sound/soldier_gun_pack/wpn_disposition_cock";
   description = AudioClose3D;
   preload = true;
};

// ----------------------------------------------------------------------------
// Particles
// ----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Explosion
//-----------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Shell ejected during reload.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Projectile Object
//-----------------------------------------------------------------------------
datablock LightDescription( DispositionProjectileLightDesc ) {
   color  = "0.0 0.5 0.7";
   range = 3.0;
};

datablock ProjectileData( DispositionProjectile ) {
   projectileShapeName = "";

   directDamage        = 40;
   radiusDamage        = 0;
   damageRadius        = 0.5;
   areaImpulse         = 0.5;
   impactForce         = 1;

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

function DispositionProjectile::onCollision(%this, %obj, %col, %fade, %pos, %normal) {
   // Apply impact force from the projectile.

   // Apply damage to the object all shape base objects
	if ( %col.getType() & $TypeMasks::GameBaseObjectType ) {
      %damage = %this.directDamage;
      if(%obj.sourceObject.spec(3) $= "Firepower I") {
		   %damage *= (1.25);
		}
      else if(%obj.sourceObject.spec(3) $= "Firepower II") {
		   %damage *= (1.5);
		}
      else if(%obj.sourceObject.spec(3) $= "Firepower III") {
		   %damage *= (2);
		}

      %damLoc = firstWord(%col.getDamageLocation(%pos));
      if(%damLoc $= "head") {
		   %col.headshot = 1;
			%col.damage(%obj, %pos, %damage * $WeaponHeadshotBonus, "Disposition");
		}
      %col.damage(%obj, %pos, %damage, "Disposition");
	}
}

//Incindiary Rounds
datablock ParticleData(DispositionProjectile_IncindiarySmokeTrail) {
   textureName = "art/shapes/particles/fireball";
   dragCoeffiecient = 0;
   gravityCoefficient = -0.202686;
   inheritedVelFactor = 0.0;
   constantAcceleration = 0.0;
   lifetimeMS = 750;
   lifetimeVarianceMS = 749;
   useInvAlpha = false;
   spinRandomMin = -60;
   spinRandomMax = 0;
   spinSpeed = 1;

   colors[0] = "0.984314 0.623529 0.278431 1";
   colors[1] = "0.901961 0.317647 0.121569 0.629921";
   colors[2] = "0.996078 0.2 0.0470588 0.330709";

   sizes[0] = 0.1;
   sizes[1] = 0.12;
   sizes[2] = 0.2;

   times[0] = 0.0;
   times[1] = 0.4;
   times[2] = 1.0;
   animTexName = "art/shapes/particles/fireball";
   times[3] = "1";
};

datablock ParticleEmitterData(DispositionProjectile_IncindiarySmokeTrailEmitter) {
   ejectionPeriodMS = 1;
   periodVarianceMS = 0;
   ejectionVelocity = 0.3;
   velocityVariance = 0;
   thetaMin = 0.0;
   thetaMax = 0.0;
   phiReferenceVel = 90;
   phiVariance = 0;
   particles = "DispositionProjectile_IncindiarySmokeTrail";
};

datablock ProjectileData( DispositionProjectile_Incindiary : DispositionProjectile ) {
   particleEmitter      = DispositionProjectile_IncindiarySmokeTrailEmitter;
   particleWaterEmitter = DispositionProjectile_IncindiarySmokeTrailEmitter;
};

function DispositionProjectile_Incindiary::onCollision(%this, %obj, %col, %fade, %pos, %normal) {
	if ( %col.getType() & $TypeMasks::GameBaseObjectType ) {
	   if(%col.client.team == %obj.sourceObject.client.team) {
		   return;
	   }
		if(getRandom(0, 100) > 80) {
		   %col.damage(%obj, %pos, 0.1, "Disposition");
			Incinerate(%col, %obj);   //bye bye...

			commandToClient(%obj.sourceObject.getControllingClient(), 'addIncineration', 3);
			return;
		}

      %damage = %this.directDamage;
      if(%obj.sourceObject.spec(3) $= "Firepower I") {
		   %damage *= (1.25);
		}
      else if(%obj.sourceObject.spec(3) $= "Firepower II") {
		   %damage *= (1.5);
		}
      else if(%obj.sourceObject.spec(3) $= "Firepower III") {
		   %damage *= (2);
		}

		%damLoc = firstWord(%col.getDamageLocation(%pos));
      if(%damLoc $= "head") {
		   %col.headshot = 1;
			%col.damage(%obj, %pos, %damage * $WeaponHeadshotBonus, "Disposition");
		}
      %col.damage(%obj, %pos, %damage, "Disposition");
	}
}

//-----------------------------------------------------------------------------
// Ammo Item
//-----------------------------------------------------------------------------
datablock ItemData(DispositionClip)
{
   // Mission editor category
   category = "AmmoClip";

   // Add the Ammo namespace as a parent.  The ammo namespace provides
   // common ammo related functions and hooks into the inventory system.
   className = "AmmoClip";

   // Basic Item properties
   shapeFile = "art/shapes/weapons/Disposition/TP_Disposition.DAE";
   mass = 1;
   elasticity = 0.2;
   friction = 0.6;

   // Dynamic properties defined by the scripts
   pickUpName = "Disposition clip";
   count = 1;
   maxInventory = 5;
};

datablock ItemData(DispositionAmmo)
{
   // Mission editor category
   category = "Ammo";

   // Add the Ammo namespace as a parent.  The ammo namespace provides
   // common ammo related functions and hooks into the inventory system.
   className = "Ammo";

   // Basic Item properties
   shapeFile = "art/shapes/weapons/Disposition/TP_Disposition.DAE";
   mass = 1;
   elasticity = 0.2;
   friction = 0.6;

   // Dynamic properties defined by the scripts
   pickUpName = "Disposition ammo";
   maxInventory = 8;
   clip = DispositionClip;
};

//--------------------------------------------------------------------------
// Weapon Item.  This is the item that exists in the world, i.e. when it's
// been dropped, thrown or is acting as re-spawnable item.  When the weapon
// is mounted onto a shape, the DispositionWeaponImage is used.
//-----------------------------------------------------------------------------
datablock ItemData(Disposition)
{
   // Mission editor category
   category = "Weapon";

   // Hook into Item Weapon class hierarchy. The weapon namespace
   // provides common weapon handling functions in addition to hooks
   // into the inventory system.
   className = "Weapon";

   // Basic Item properties
   shapeFile = "art/shapes/weapons/Disposition/TP_Disposition.DAE";
   mass = 1;
   elasticity = 0.2;
   friction = 0.6;
   emap = true;

   // Dynamic properties defined by the scripts
   PreviewImage = "Disposition.png";
   pickUpName = "Disposition rifle";
   description = "Disposition";
   image = DispositionWeaponImage;
   reticle = "crossHair";
};


datablock ShapeBaseImageData(DispositionWeaponImage)
{
   // Basic Item properties
   shapeFile = "art/shapes/weapons/Disposition/TP_Disposition.DAE";
   shapeFileFP = "art/shapes/weapons/Disposition/FP_Disposition.DAE";
   emap = true;

   imageAnimPrefix = "Disposition";
   imageAnimPrefixFP = "Shotgun";

   // Specify mount point & offset for 3rd person, and eye offset
   // for first person rendering.
   mountPoint = 0;
   firstPerson = true;
   useEyeNode = true;
   animateOnServer = true;

   // When firing from a point offset from the eye, muzzle correction
   // will adjust the muzzle vector to point to the eye LOS point.
   // Since this weapon doesn't actually fire from the muzzle point,
   // we need to turn this off.
   correctMuzzleVector = true;

   // Add the WeaponImage namespace as a parent, WeaponImage namespace
   // provides some hooks into the inventory system.
   class = "WeaponImage";
   className = "WeaponImage";

	ammoImage = "Disposition";

   // Projectiles and Ammo.
   item = Disposition;
   ammo = DispositionAmmo;
   clip = DispositionClip;

	weaponID = 3;
   
   projectile = DispositionProjectile;
   projectileType = Projectile;
   projectileNum = 8;

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

   // Images have a state system which controls how the animations
   // are run, which sounds are played, script callbacks, etc. This
   // state system is downloaded to the client so that clients can
   // predict state changes and animate accordingly.  The following
   // system supports basic ready->fire->reload transitions as
   // well as a no-ammo->dryfire idle state.

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
   stateTimeoutValue[7]             = 0.66;
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

function DispositionWeaponImage::onFire(%this, %obj, %slot) {
   %spread = %obj.getWeaponSpread("Disposition");
	if(%obj.spec(3) $= "Incindiary Rounds") {
	   %projectile = DispositionProjectile_Incindiary;
	}
	else {
	   %projectile = DispositionProjectile;
	}

	%obj.decInventory(%this.ammo, 1);
   for(%shotCount = 0; %shotCount < %this.projectileNum; %shotCount++) {
      %vec = %obj.getMuzzleVector(%slot);
      %x = (getRandom() - 0.5) * 2 * 3.1415926 * %spread;
      %y = (getRandom() - 0.5) * 2 * 3.1415926 * %spread;
      %z = (getRandom() - 0.5) * 2 * 3.1415926 * %spread;
      %mat = MatrixCreateFromEuler(%x @ " " @ %y @ " " @ %z);

      // Which we'll use to alter the projectile's initial vector with
      %muzzleVector = MatrixMulVector(%mat, %vec);

      // Get the player's velocity, we'll then add it to that of the projectile
      %objectVelocity = %obj.getVelocity();
      %muzzleVelocity = VectorAdd(
         VectorScale(%muzzleVector, %this.projectile.muzzleVelocity),
         VectorScale(%objectVelocity, %this.projectile.velInheritFactor));

      // Create the projectile object
      %p = new (%this.projectileType)() {
         dataBlock = %projectile;
         initialVelocity = %muzzleVelocity;
         initialPosition = %obj.getMuzzlePoint(%slot);
         sourceObject = %obj;
         sourceSlot = %slot;
         client = %obj.client;
      };
      MissionCleanup.add(%p);
   }
   return %p;
}