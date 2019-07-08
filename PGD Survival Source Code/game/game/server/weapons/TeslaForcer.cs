datablock SFXProfile(TeslaForcerFireSound)
{
   filename = "art/sound/mantis_gun_pack/wpn_TeslaForcer_fire";
   description = AudioClose3D;
   preload = true;
};

datablock SFXProfile(TeslaForcerReloadSound)
{
   filename = "art/sound/mantis_gun_pack/wpn_TeslaForcer_reload";
   description = AudioClose3D;
   preload = true;
};

datablock SFXProfile(TeslaForcerSwitchinSound)
{
   filename = "art/sound/mantis_gun_pack/wpn_TeslaForcer_switchin";
   description = AudioClose3D;
   preload = true;
};


// ----------------------------------------------------------------------------
// Particles
// ----------------------------------------------------------------------------
datablock ParticleData(TeslaForcerExplosion_FlashParticle : DefaultParticle) {
   textureName = "art/shapes/particles/spark.png";
   animTexName = "art/shapes/particles/spark.png";
   colors[0] = "0.141176 0.392157 0.384314 1";
   colors[1] = "0.0470588 0.454902 0.509804 0.637795";
   colors[2] = "0.0784314 0.278431 0.792157 0.330709";
   colors[3] = "0.0509804 0.2 0.866667 0";
   sizes[1] = "1.04167";
   sizes[2] = "1.04167";
   sizes[3] = "9.375";
   times[0] = "0.1875";
   times[1] = "0.4375";
   times[2] = "0.729167";
};

datablock ParticleData(TeslaForcerExplosion_SuctionParticle : DefaultParticle) {
   textureName = "art/shapes/particles/millsplash01.png";
   animTexName = "art/shapes/particles/millsplash01.png";
   colors[0] = "0 0.976471 1 1";
   colors[1] = "0 0.372549 1 0.637795";
   sizes[0] = "8.33333";
   sizes[1] = "0";
   sizes[3] = "0";
   times[0] = "0.0833333";
};

datablock ParticleData(TeslaForcerExplosion_VortexParticle : DefaultParticle) {
   gravityCoefficient = "0.875";
   inheritedVelFactor = "2.708";
   constantAcceleration = "1.667";
   textureName = "art/shapes/particles/steam.png";
   animTexName = "art/shapes/particles/steam.png";
   colors[0] = "0 1 0.835294 1";
   sizes[0] = "6.25";
   times[0] = "0.125";
};
//-----------------------------------------------------------------------------
// Explosion
//-----------------------------------------------------------------------------
datablock ParticleEmitterData(TeslaForcerExplosionEmitter : DefaultEmitter) {
   particles = "TeslaForcerExplosion_FlashParticle TeslaForcerExplosion_SuctionParticle TeslaForcerExplosion_VortexParticle";
};

datablock ExplosionData(TeslaForcerExplosion) {
   soundProfile = ThunderCrash1Sound;
   lifeTimeMS = 1500; // Boom, burn, and die...

   // Point emission
   emitter[0] = TeslaForcerExplosionEmitter;

   lightStartRadius = 6.0;
   lightEndRadius = 0.0;
   lightStartColor = "0.0 0.0 1.0";
   lightEndColor = "0.0 0.0 0.5";
   lightStartBrightness = 5.5;
   lightEndBrightness = 0.0;
   lightNormalOffset = 3.0;
};
//--------------------------------------------------------------------------
// Shell ejected during reload.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Projectile Object
//-----------------------------------------------------------------------------
datablock ParticleData(TeslaForcerProjectileTrailParticle) {
   textureName = "art/shapes/particles/millsplash01";
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

   colors[0] = "0 0.976471 1 1";
   colors[1] = "0 0.372549 1 0.637795";
	colors[2] = "0 0.372549 1 0.637795";

   sizes[0] = 0.1;
   sizes[1] = 0.12;
   sizes[2] = 0.2;

   times[0] = 0.0;
   times[1] = 0.4;
   times[2] = 1.0;
   animTexName = "art/shapes/particles/millsplash01";
   times[3] = "1";
};

datablock ParticleEmitterData(TeslaForcerProjectileTrailEmitter) {
   ejectionPeriodMS = 1;
   periodVarianceMS = 0;
   ejectionVelocity = 0.75;
   velocityVariance = 0;
   thetaMin = 0.0;
   thetaMax = 0.0;
   phiReferenceVel = 90;
   phiVariance = 0;
   particles = "TeslaForcerProjectileTrailParticle";
};

datablock ProjectileData( TeslaForcerProjectile ) {
   projectileShapeName = "";

   directDamage        = 70;
   radiusDamage        = 95;
   damageRadius        = 5.0;
   areaImpulse         = 3.5;
   impactForce         = 1;

   explosion           = TeslaForcerExplosion;
   decal               = BulletHoleDecal;
   
   particleEmitter      = TeslaForcerProjectileTrailEmitter;
   particleWaterEmitter = TeslaForcerProjectileTrailEmitter;

   muzzleVelocity      = 250;
   velInheritFactor    = 1;

   armingDelay         = 0;
   lifetime            = 992;
   fadeDelay           = 1472;
   bounceElasticity    = 0;
   bounceFriction      = 0;
   isBallistic         = false;
   gravityMod          = 1;

	damageType = "Tesla Forcer";
};

function TeslaForcerProjectile::onCollision(%this, %obj, %col, %fade, %pos, %normal) {
   // Apply impact force from the projectile.
	if(%obj.sourceObject.spec(10) $= "Ion Storm") {
	   createIonStorm(%obj.sourceObject, %pos);
	}

   // Apply damage to the object all shape base objects
	if ( %col.getType() & $TypeMasks::GameBaseObjectType ) {
      %damage = %this.directDamage;
      if(%obj.sourceObject.spec(10) $= "Firepower I") {
		   %damage *= (1.25);
		}
      else if(%obj.sourceObject.spec(10) $= "Firepower II") {
		   %damage *= (1.5);
		}
      else if(%obj.sourceObject.spec(10) $= "Firepower III") {
		   %damage *= (2);
		}

		%damLoc = firstWord(%col.getDamageLocation(%pos));
      if(%damLoc $= "head") {
		   %col.headshot = 1;
			%col.damage(%obj, %pos, %damage * $WeaponHeadshotBonus, "Tesla Forcer");
		}
      %col.damage(%obj, %pos, %damage, "Tesla Forcer");
	}
}

function TeslaForcerProjectile::onExplode(%data, %proj, %position, %mod) {
   %damage = %data.radiusDamage;
	%impulse = %data.areaImpulse;
   if(%proj.sourceObject.spec(10) $= "Firepower I") {
		%damage *= (1.25);
   }
   else if(%proj.sourceObject.spec(10) $= "Firepower II") {
	   %damage *= (1.5);
	}
   else if(%proj.sourceObject.spec(10) $= "Firepower III") {
		%damage *= (2);
   }

	radiusDamage(%proj, %position, %data.damageRadius, %damage, %data.damageType, %impulse);
}

//--------------------------------------------------------------------------
// Weapon Item.  This is the item that exists in the world, i.e. when it's
// been dropped, thrown or is acting as re-spawnable item.  When the weapon
// is mounted onto a shape, the TeslaForcerWeaponImage is used.
//-----------------------------------------------------------------------------
datablock ItemData(TeslaForcerClip) {
   // Mission editor category
   category = "AmmoClip";

   // Add the Ammo namespace as a parent.  The ammo namespace provides
   // common ammo related functions and hooks into the inventory system.
   className = "AmmoClip";

   // Basic Item properties
   shapeFile = "art/shapes/weapons/Kralmok/TP_Kralmok.DAE";
   mass = 1;
   elasticity = 0.2;
   friction = 0.6;

   // Dynamic properties defined by the scripts
   pickUpName = "Tesla Forcer clip";
   maxInventory = 3;
};

datablock ItemData(TeslaForcerAmmo) {
   // Mission editor category
   category = "Ammo";

   // Add the Ammo namespace as a parent.  The ammo namespace provides
   // common ammo related functions and hooks into the inventory system.
   className = "Ammo";

   // Basic Item properties
   shapeFile = "art/shapes/weapons/Kralmok/TP_Kralmok.DAE";
   mass = 1;
   elasticity = 0.2;
   friction = 0.6;

   // Dynamic properties defined by the scripts
   pickUpName = "Tesla Forcer ammo";
   maxInventory = 1;
	clip = TeslaForcerClip;
};

datablock ItemData(TeslaForcer)
{
   // Mission editor category
   category = "Weapon";

   // Hook into Item Weapon class hierarchy. The weapon namespace
   // provides common weapon handling functions in addition to hooks
   // into the inventory system.
   className = "Weapon";

   // Basic Item properties
   shapeFile = "art/shapes/weapons/Kralmok/TP_Kralmok.DAE";
   mass = 1;
   elasticity = 0.2;
   friction = 0.6;
   emap = true;

   // Dynamic properties defined by the scripts
   PreviewImage = "TeslaForcer.png";
   pickUpName = "Tesla Forcer";
   description = "TeslaForcer";
   image = TeslaForcerWeaponImage;
   reticle = "crossHair";
};

datablock ShapeBaseImageData(TeslaForcerWeaponImage)
{
   // Basic Item properties
   shapeFile = "art/shapes/weapons/Kralmok/TP_Kralmok.DAE";
   shapeFileFP = "art/shapes/weapons/Kralmok/FP_Kralmok.DAE";
   emap = true;

   imageAnimPrefix = "Kralmok";
   imageAnimPrefixFP = "Kralmok";

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

	ammoImage = "TeslaForcer";

   // Projectiles and Ammo.
   item = TeslaForcer;
   ammo = TeslaForcerAmmo;
	clip = TeslaForcerClip;
   //usesEnergy = true;

	weaponID = 10;
   
   projectile = TeslaForcerProjectile;
   projectileType = Projectile;
   projectileSpread = "0.005";

   casing = BulletShell;
   shellExitDir        = "1.0 0.3 1.0";
   shellExitOffset     = "0.15 -0.56 -0.1";
   shellExitVariance   = 15.0;
   shellVelocity       = 3.0;

   // Weapon lights up while firing
   lightType = "WeaponFireLight";
   lightColor = "0.992126 0.968504 0.708661 1";
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
   stateTimeoutValue[1]             = 2.0;
   stateSequence[1]                 = "switch_in";
   stateSound[1]                    = TeslaForcerSwitchinSound;

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
   stateTimeoutValue[3]             = 6;
   stateWaitForTimeout[3]           = false;
   stateTransitionOnNoAmmo[3]       = "NoAmmo";
   stateTransitionOnTriggerDown[3]  = "Fire";
   stateSequence[3]                 = "idle_fidget1";

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
   stateTimeoutValue[5]             = 0.466;
   stateFire[5]                     = true;
   stateRecoil[5]                   = "";
   stateAllowImageChange[5]         = false;
   stateSequence[5]                 = "fire";
   stateScaleAnimation[5]           = false;
   stateSequenceNeverTransition[5]  = true;
   stateSequenceRandomFlash[5]      = true;        // use muzzle flash sequence
   stateScript[5]                   = "onFire";
   stateSound[5]                    = TeslaForcerFireSound;
   //stateEmitter[5]                  = GunFireSmokeEmitter;
   //stateEmitterTime[5]              = 0.025;

   // Play the reload animation, and transition into
   stateName[6]                     = "Reload";
   stateTransitionGeneric0In[6]     = "SprintEnter";
   stateTransitionOnNoAmmo[6]       = "NoAmmo";
   stateTransitionOnTimeout[6]      = "Ready";
   stateWaitForTimeout[6]           = "0";
   stateTimeoutValue[6]             = 3.3;
   stateAllowImageChange[6]         = false;
   stateSequence[6]                 = "reload";
   stateShapeSequence[6]            = "Reload";
   stateEjectShell[6]               = true;
   stateSound[6]                    = TeslaForcerReloadSound;

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
   stateTransitionOnAmmo[8]       = "ReloadClip";
   stateTransitionOnTriggerDown[8]  = "DryFire";
   stateSequence[8]                 = "run";

   // No ammo dry fire
   stateName[9]                     = "DryFire";
   stateTransitionGeneric0In[9]     = "SprintEnter";
   stateTimeoutValue[9]             = 1.0;
   stateTransitionOnTimeout[9]      = "NoAmmo";
   stateScript[9]                   = "onDryFire";

   // Play the reload clip animation
   stateName[10]                     = "ReloadClip";
   stateTransitionGeneric0In[10]     = "SprintEnter";
   stateTransitionOnTimeout[10]      = "Ready";
   stateWaitForTimeout[10]           = true;
   stateTimeoutValue[10]             = 3.0;
   stateReload[10]                   = true;
   stateSequence[10]                 = "reload";
   stateShapeSequence[10]            = "Reload";
   stateScaleShapeSequence[10]       = true;
   stateSound[10]                    = TeslaForcerReloadSound;

   // Start Sprinting
   stateName[11]                     = "SprintEnter";
   stateTransitionGeneric0Out[11]    = "SprintExit";
   stateTransitionOnTimeout[11]      = "Sprinting";
   stateWaitForTimeout[11]           = false;
   stateTimeoutValue[11]             = 0.5;
   stateWaitForTimeout[11]           = false;
   stateScaleAnimation[11]           = false;
   stateScaleAnimationFP[11]         = false;
   stateSequenceTransitionIn[11]     = true;
   stateSequenceTransitionOut[11]    = true;
   stateAllowImageChange[11]         = false;
   stateSequence[11]                 = "run2sprint";

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
   stateSequence[13]                = "sprint2run";
};

function TeslaForcerWeaponImage::onFire(%this, %obj, %slot) {
   %spread = %obj.getWeaponSpread("TeslaForcer");

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
      dataBlock = %this.projectile;
      initialVelocity = %muzzleVelocity;
      initialPosition = %obj.getMuzzlePoint(%slot);
      sourceObject = %obj;
      sourceSlot = %slot;
      client = %obj.client;
   };
   MissionCleanup.add(%p);

   return %p;
}