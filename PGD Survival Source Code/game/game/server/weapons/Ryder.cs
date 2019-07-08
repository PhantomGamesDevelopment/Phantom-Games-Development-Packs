//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Sounds
//--------------------------------------------------------------------------

datablock SFXProfile(RyderFireSound)
{
   filename = "art/sound/weapons/wpn_Ryder_fire";
   description = AudioClose3D;
   preload = true;
};

datablock SFXProfile(RyderReloadSound)
{
   filename = "art/sound/weapons/wpn_Ryder_reload";
   description = AudioClose3D;
   preload = true;
};

datablock SFXProfile(RyderSwitchinSound)
{
   filename = "art/sound/weapons/wpn_Ryder_switchin";
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
datablock LightDescription( RyderProjectileLightDesc ) {
   color  = "0.0 0.5 0.7";
   range = 3.0;
};

datablock ProjectileData( RyderProjectile ) {
   projectileShapeName = "";

   directDamage        = 23;
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

function RyderProjectile::onCollision(%this, %obj, %col, %fade, %pos, %normal) {
   // Apply impact force from the projectile.

   // Apply damage to the object all shape base objects
	if ( %col.getType() & $TypeMasks::GameBaseObjectType ) {
		if(%obj.sourceObject.spec(0) $= "Medic") {
			if(%col.client.team == %obj.sourceObject.client.team) {
				//medic challenge stuff
				%damageLevel = %col.getDamageLevel();
				if(%damageLevel > 10) {
				   %award = 10;
				}
				else {
				   %award = %damageLevel;
				}
				commandToClient(%obj.sourceObject.getControllingClient(), 'addRyderMedic', %award);
				//
	         %col.applyRepair(10);
				doMedicUpdate(%col);
			}
		}

      %damage = %this.directDamage;
      if(%obj.sourceObject.spec(0) $= "Firepower I") {
		   %damage *= (1.25);
		}
      else if(%obj.sourceObject.spec(0) $= "Firepower II") {
		   %damage *= (1.5);
		}
      else if(%obj.sourceObject.spec(0) $= "Firepower III") {
		   %damage *= (2);
		}

		%damLoc = firstWord(%col.getDamageLocation(%pos));
      if(%damLoc $= "head") {
		   %col.headshot = 1;
			%col.damage(%obj, %pos, %damage * $WeaponHeadshotBonus, "Ryder");
		}
      %col.damage(%obj, %pos, %damage, "Ryder");
	}
}

//Incindiary Rounds
datablock ParticleData(RyderProjectile_IncindiarySmokeTrail) {
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

datablock ParticleEmitterData(RyderProjectile_IncindiarySmokeTrailEmitter) {
   ejectionPeriodMS = 1;
   periodVarianceMS = 0;
   ejectionVelocity = 0.5;
   velocityVariance = 0;
   thetaMin = 0.0;
   thetaMax = 0.0;
   phiReferenceVel = 90;
   phiVariance = 0;
   particles = "RyderProjectile_IncindiarySmokeTrail";
};

datablock ProjectileData( RyderProjectile_Incindiary : RyderProjectile ) {
   particleEmitter      = RyderProjectile_IncindiarySmokeTrailEmitter;
   particleWaterEmitter = RyderProjectile_IncindiarySmokeTrailEmitter;
};

function RyderProjectile_Incindiary::onCollision(%this, %obj, %col, %fade, %pos, %normal) {
	if ( %col.getType() & $TypeMasks::GameBaseObjectType ) {
	   if(%col.client.team == %obj.sourceObject.client.team) {
		   return;
	   }
		if(getRandom(0, 100) > 85) {
		   %col.damage(%obj, %pos, 0.1, "Ryder");
			Incinerate(%col, %obj);   //bye bye...

			commandToClient(%obj.sourceObject.getControllingClient(), 'addIncineration', 0);
			return;
		}

      %damage = %this.directDamage;
      if(%obj.sourceObject.spec(0) $= "Firepower I") {
		   %damage *= (1.25);
		}
      else if(%obj.sourceObject.spec(0) $= "Firepower II") {
		   %damage *= (1.5);
		}
      else if(%obj.sourceObject.spec(0) $= "Firepower III") {
		   %damage *= (2);
		}

		%damLoc = firstWord(%col.getDamageLocation(%pos));
      if(%damLoc $= "head") {
		   %col.headshot = 1;
			%col.damage(%obj, %pos, %damage * $WeaponHeadshotBonus, "Ryder");
		}
      %col.damage(%obj, %pos, %damage, "Ryder");
	}
}

//Pulse Rounds
datablock ParticleData(RyderProjectile_PulseSmokeTrail) {
   textureName = "art/shapes/particles/spark";
   dragCoeffiecient = 0;
   gravityCoefficient = -0.202686;
   inheritedVelFactor = 0.0;
   constantAcceleration = 0.0;
   lifetimeMS = 750;
   lifetimeVarianceMS = 749;
   useInvAlpha = true;
   spinRandomMin = -60;
   spinRandomMax = 0;
   spinSpeed = 1;

   colors[0] = "0 255 255 1";
   colors[1] = "0 139 139 0.637795";
   colors[2] = "47 47 79 0.330709";

   sizes[0] = 0.1;
   sizes[1] = 0.12;
   sizes[2] = 0.2;

   times[0] = 0.0;
   times[1] = 0.4;
   times[2] = 1.0;
   animTexName = "art/shapes/particles/spark";
   times[3] = "1";
};

datablock ParticleEmitterData(RyderProjectile_PulseSmokeTrailEmitter) {
   ejectionPeriodMS = 1;
   periodVarianceMS = 0;
   ejectionVelocity = 0.5;
   velocityVariance = 0;
   thetaMin = 0.0;
   thetaMax = 0.0;
   phiReferenceVel = 90;
   phiVariance = 0;
   particles = "RyderProjectile_PulseSmokeTrail";
};

datablock ProjectileData( RyderProjectile_PulseRounds : RyderProjectile ) {
   particleEmitter      = RyderProjectile_PulseSmokeTrailEmitter;
   particleWaterEmitter = RyderProjectile_PulseSmokeTrailEmitter;
};

function RyderProjectile_PulseRounds::onCollision(%this, %obj, %col, %fade, %pos, %normal) {
	if ( %col.getType() & $TypeMasks::GameBaseObjectType ) {
	   if(%col.client.team == %obj.sourceObject.client.team) {
		   return;
	   }
		if(%obj.level == 1) {
		   if(getRandom(0, 100) > 85) {
		      %col.damage(%obj, %pos, 0.1, "Ryder");
			   Disintegrate(%col, %obj);   //bye bye...

			   commandToClient(%obj.sourceObject.getControllingClient(), 'addDisintegration', 0);
			   return;
		   }
		}
		else {
		   if(getRandom(0, 100) > 70) {
		      %col.damage(%obj, %pos, 0.1, "Ryder");
			   Disintegrate(%col, %obj);   //bye bye...

			   commandToClient(%obj.sourceObject.getControllingClient(), 'addDisintegration', 0);
			   return;
		   }		
		}

      %damage = %this.directDamage;
      if(%obj.sourceObject.spec(0) $= "Firepower I") {
		   %damage *= (1.25);
		}
      else if(%obj.sourceObject.spec(0) $= "Firepower II") {
		   %damage *= (1.5);
		}
      else if(%obj.sourceObject.spec(0) $= "Firepower III") {
		   %damage *= (2);
		}

		%damLoc = firstWord(%col.getDamageLocation(%pos));
      if(%damLoc $= "head") {
		   %col.headshot = 1;
			%col.damage(%obj, %pos, %damage * $WeaponHeadshotBonus, "Ryder");
		}
      %col.damage(%obj, %pos, %damage, "Ryder");
	}
}

//-----------------------------------------------------------------------------
// Ammo Item
//-----------------------------------------------------------------------------
datablock ItemData(RyderClip)
{
   // Mission editor category
   category = "AmmoClip";

   // Add the Ammo namespace as a parent.  The ammo namespace provides
   // common ammo related functions and hooks into the inventory system.
   className = "AmmoClip";

   // Basic Item properties
   shapeFile = "art/shapes/weapons/Ryder/TP_Ryder.DAE";
   mass = 1;
   elasticity = 0.2;
   friction = 0.6;

   // Dynamic properties defined by the scripts
   pickUpName = "Ryder clip";
   count = 1;
   maxInventory = 15;
};

datablock ItemData(RyderAmmo)
{
   // Mission editor category
   category = "Ammo";

   // Add the Ammo namespace as a parent.  The ammo namespace provides
   // common ammo related functions and hooks into the inventory system.
   className = "Ammo";

   // Basic Item properties
   shapeFile = "art/shapes/weapons/Ryder/TP_Ryder.DAE";
   mass = 1;
   elasticity = 0.2;
   friction = 0.6;

   // Dynamic properties defined by the scripts
   pickUpName = "Ryder bullet";
   maxInventory = 8;
   clip = RyderClip;
};

//--------------------------------------------------------------------------
// Weapon Item.  This is the item that exists in the world, i.e. when it's
// been dropped, thrown or is acting as re-spawnable item.  When the weapon
// is mounted onto a shape, the SoldierWeaponImage is used.
//-----------------------------------------------------------------------------
datablock ItemData(Ryder)
{
   // Mission editor category
   category = "Weapon";

   // Hook into Item Weapon class hierarchy. The weapon namespace
   // provides common weapon handling functions in addition to hooks
   // into the inventory system.
   className = "Weapon";

   // Basic Item properties
   shapeFile = "art/shapes/weapons/Ryder/TP_Ryder.DAE";
   mass = 1;
   elasticity = 0.2;
   friction = 0.6;
   emap = true;
   PreviewImage = "ryder.png";

   // Dynamic properties defined by the scripts
   pickUpName = "Ryder pistol";
   description = "Ryder";
   image = RyderWeaponImage;
   reticle = "crossHair";
};


datablock ShapeBaseImageData(RyderWeaponImage)
{
   // Basic Item properties
   shapeFile = "art/shapes/weapons/Ryder/TP_Ryder.DAE";
   shapeFileFP = "art/shapes/weapons/Ryder/FP_Ryder.DAE";
   emap = true;

   imageAnimPrefix = "Pistol";
   imageAnimPrefixFP = "Pistol";

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

	ammoImage = "Ryder";

   // Projectiles and Ammo.
   item = Ryder;
   ammo = RyderAmmo;
   clip = RyderClip;

	weaponID = 0;

   projectile = RyderProjectile;
   projectileType = Projectile;
   projectileSpread = "0.0";

   casing = BulletShell;
   shellExitDir        = "1.0 0.3 1.0";
   shellExitOffset     = "0.15 -0.56 -0.1";
   shellExitVariance   = 15.0;
   shellVelocity       = 3.0;

   // Weapon lights up while firing
   lightType = "WeaponFireLight";
   lightColor = "0.992126 0.968504 0.700787 1";
   lightRadius = "4";
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
   stateTimeoutValue[1]             = 1.5;
   stateSequence[1]                 = "switch_in";
   stateSound[1]                    = RyderSwitchinSound;

   // Ready to fire, just waiting for the trigger
   stateName[2]                     = "Ready";
   stateTransitionGeneric0In[2]     = "SprintEnter";
   stateTransitionOnMotion[2]       = "ReadyMotion";
   stateScaleAnimation[2]           = false;
   stateScaleAnimationFP[2]         = false;
   stateTransitionOnNoAmmo[2]       = "NoAmmo";
   stateTransitionOnTriggerDown[2]  = "Fire";
   stateSequence[2]                 = "idle";

   // Ready to fire with player moving
   stateName[3]                     = "ReadyMotion";
   stateTransitionGeneric0In[3]     = "SprintEnter";
   stateTransitionOnNoMotion[3]     = "Ready";
   stateWaitForTimeout[3]           = false;
   stateScaleAnimation[3]           = false;
   stateScaleAnimationFP[3]         = false;
   stateSequenceTransitionIn[3]     = true;
   stateSequenceTransitionOut[3]    = true;
   stateTransitionOnNoAmmo[3]       = "NoAmmo";
   stateTransitionOnTriggerDown[3]  = "Fire";
   stateSequence[3]                 = "run";

   // Fire the weapon. Calls the fire script which does
   // the actual work.
   stateName[4]                     = "Fire";
   stateTransitionGeneric0In[4]     = "SprintEnter";
   stateTransitionOnTimeout[4]      = "WaitForRelease";
   stateTimeoutValue[4]             = 0.23;
   stateWaitForTimeout[4]           = true;
   stateFire[4]                     = true;
   stateRecoil[4]                   = "";
   stateAllowImageChange[4]         = false;
   stateSequence[4]                 = "fire";
   stateScaleAnimation[4]           = true;
   stateSequenceNeverTransition[4]  = true;
   stateSequenceRandomFlash[4]      = true;        // use muzzle flash sequence
   stateScript[4]                   = "onFire";
   stateEmitter[4]                  = GunFireSmokeEmitter;
   stateEmitterTime[4]              = 0.025;
   stateEjectShell[4]               = true;
   stateSound[4]                    = RyderFireSound;

   // Wait for the player to release the trigger
   stateName[5]                     = "WaitForRelease";
   stateTransitionGeneric0In[5]     = "SprintEnter";
   stateTransitionOnTriggerUp[5]    = "NewRound";
   stateTimeoutValue[5]             = 0.05;
   stateWaitForTimeout[5]           = true;
   stateAllowImageChange[5]         = false;

   // Put another round in the chamber
   stateName[6]                     = "NewRound";
   stateTransitionGeneric0In[6]     = "SprintEnter";
   stateTransitionOnNoAmmo[6]       = "NoAmmo";
   stateTransitionOnTimeout[6]      = "Ready";
   stateWaitForTimeout[6]           = "0";
   stateTimeoutValue[6]             = 0.05;
   stateAllowImageChange[6]         = false;

   // No ammo in the weapon, just idle until something
   // shows up. Play the dry fire sound if the trigger is
   // pulled.
   stateName[7]                     = "NoAmmo";
   stateTransitionGeneric0In[7]     = "SprintEnter";
   stateTransitionOnMotion[7]       = "NoAmmoMotion";
   stateTransitionOnAmmo[7]         = "ReloadClip";
   stateTimeoutValue[7]             = 0.1;   // Slight pause to allow script to run when trigger is still held down from Fire state
   stateScript[7]                   = "onClipEmpty";
   stateTransitionOnTriggerDown[7]  = "DryFire";
   stateSequence[7]                 = "idle";
   stateScaleAnimation[7]           = false;
   stateScaleAnimationFP[7]         = false;
   
   stateName[8]                     = "NoAmmoMotion";
   stateTransitionGeneric0In[8]     = "SprintEnter";
   stateTransitionOnNoMotion[8]     = "NoAmmo";
   stateWaitForTimeout[8]           = false;
   stateScaleAnimation[8]           = false;
   stateScaleAnimationFP[8]         = false;
   stateSequenceTransitionIn[8]     = true;
   stateSequenceTransitionOut[8]    = true;
   stateTransitionOnAmmo[8]         = "ReloadClip";
   stateTransitionOnTriggerDown[8]  = "DryFire";
   stateSequence[8]                 = "run";

   // No ammo dry fire
   stateName[9]                     = "DryFire";
   stateTransitionGeneric0In[9]     = "SprintEnter";
   stateTransitionOnAmmo[9]         = "ReloadClip";
   stateWaitForTimeout[9]           = "0";
   stateTimeoutValue[9]             = 0.7;
   stateTransitionOnTimeout[9]      = "NoAmmo";
   stateScript[9]                   = "onDryFire";

   // Play the reload clip animation
   stateName[10]                     = "ReloadClip";
   stateTransitionGeneric0In[10]     = "SprintEnter";
   stateTransitionOnTimeout[10]      = "Ready";
   stateWaitForTimeout[10]           = true;
   stateTimeoutValue[10]             = 2.0;
   stateReload[10]                   = true;
   stateSequence[10]                 = "reload";
   stateShapeSequence[10]            = "Reload";
   stateScaleShapeSequence[10]       = true;
   stateSound[10]                    = RyderReloadSound;

   // Start Sprinting
   stateName[11]                    = "SprintEnter";
   stateTransitionGeneric0Out[11]   = "SprintExit";
   stateTransitionOnTimeout[11]     = "Sprinting";
   stateWaitForTimeout[11]          = false;
   stateTimeoutValue[11]            = 0.5;
   stateWaitForTimeout[11]          = false;
   stateScaleAnimation[11]          = false;
   stateScaleAnimationFP[11]        = false;
   stateSequenceTransitionIn[11]    = true;
   stateSequenceTransitionOut[11]   = true;
   stateAllowImageChange[11]        = false;
   stateSequence[11]                = "sprint";

   // Sprinting
   stateName[12]                    = "Sprinting";
   stateTransitionGeneric0Out[12]   = "SprintExit";
   stateWaitForTimeout[12]          = false;
   stateScaleAnimation[12]          = false;
   stateScaleAnimationFP[12]        = false;
   stateSequenceTransitionIn[12]    = true;
   stateSequenceTransitionOut[12]   = true;
   stateAllowImageChange[12]        = false;
   stateSequence[12]                = "sprint";
   
   // Stop Sprinting
   stateName[13]                    = "SprintExit";
   stateTransitionGeneric0In[13]    = "SprintEnter";
   stateTransitionOnTimeout[13]     = "Ready";
   stateWaitForTimeout[13]          = false;
   stateTimeoutValue[13]            = 0.5;
   stateSequenceTransitionIn[13]    = true;
   stateSequenceTransitionOut[13]   = true;
   stateAllowImageChange[13]        = false;
   stateSequence[13]                = "sprint";
};

function RyderWeaponImage::onFire(%this, %obj, %slot) {
   %spread = %obj.getWeaponSpread("Ryder");
	if(%obj.spec(0) $= "Incindiary Rounds") {
	   %projectile = RyderProjectile_Incindiary;
	}
	else if(%obj.spec(0) $= "Pulse Rounds I" || %obj.spec(0) $= "Pulse Rounds II") {
	   %projectile = RyderProjectile_PulseRounds;
	}
	else {
	   %projectile = RyderProjectile;
	}

	%obj.decInventory(%this.ammo, 1);

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
	if(%obj.spec(0) $= "Pulse Rounds I") {
	   %p.level = 1;	   
	}
	if(%obj.spec(0) $= "Pulse Rounds II") {
	   %p.level = 2;	   
	}

   return %p;
}