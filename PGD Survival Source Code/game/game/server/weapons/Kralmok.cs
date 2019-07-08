//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Sounds
//--------------------------------------------------------------------------

datablock SFXProfile(KralmokFireSound)
{
   filename = "art/sound/mantis_gun_pack/wpn_Kralmok_fire";
   description = AudioClose3D;
   preload = true;
};

datablock SFXProfile(KralmokReloadSound)
{
   filename = "art/sound/mantis_gun_pack/wpn_Kralmok_reload";
   description = AudioClose3D;
   preload = true;
};

datablock SFXProfile(KralmokSwitchinSound)
{
   filename = "art/sound/mantis_gun_pack/wpn_Kralmok_switchin";
   description = AudioClose3D;
   preload = true;
};

datablock SFXProfile(KralmokIdleSound)
{
   filename = "art/sound/mantis_gun_pack/wpn_Kralmok_idle";
   description = AudioClose3D;
   preload = true;
};

datablock SFXProfile(KralmokRunSound)
{
   filename = "art/sound/mantis_gun_pack/wpn_Kralmok_run";
   description = AudioClose3D;
   preload = true;
};


// ----------------------------------------------------------------------------
// Particles
// ----------------------------------------------------------------------------
datablock ParticleData(KralmokProjSmokeTrail) {
   textureName = "art/shapes/particles/spark_wet";
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

   colors[0] = "0 255 255 0.598425";
   colors[1] = "0 0 139 0.897638";
   colors[2] = "1 0 0 0";

   sizes[0] = 0.247207;
   sizes[1] = 0.497467;
   sizes[2] = 0.747726;

   times[0] = 0.0;
   times[1] = 0.4;
   times[2] = 1.0;
   animTexName = "art/shapes/particles/spark_wet";
   times[3] = "1";
};

datablock ParticleEmitterData(KralmokProjSmokeTrailEmitter) {
   ejectionPeriodMS = 1;
   periodVarianceMS = 0;
   ejectionVelocity = 0.75;
   velocityVariance = 0;
   thetaMin = 0.0;
   thetaMax = 0.0;
   phiReferenceVel = 90;
   phiVariance = 0;
   particles = "KralmokProjSmokeTrail";
};


//-----------------------------------------------------------------------------
// Explosion
//-----------------------------------------------------------------------------
datablock ParticleData(KralmokExpFire)
{
   gravityCoefficient = "-0.50061";
   lifetimeMS = "400";
   lifetimeVarianceMS = "299";
   spinSpeed = "1";
   spinRandomMin = "-200";
   spinRandomMax = "0";
   textureName = "art/shapes/particles/smoke";
   animTexName = "art/shapes/particles/smoke";
   colors[0] = "0 0 156 1";
   colors[1] = "36 24 130 0.6";
   colors[2] = "0 0 0 0";
   sizes[0] = "1.99902";
   sizes[1] = "7.99915";
   sizes[2] = "3.99805";
   times[1] = "0.392157";
   times[2] = "1";
   times[3] = "1";
};

datablock ParticleEmitterData(KralmokExpFireEmitter)
{
   ejectionPeriodMS = "10";
   periodVarianceMS = "5";
   ejectionVelocity = "3";
   velocityVariance = "2";
   particles = "KralmokExpFire";
   blendStyle = "NORMAL";
};

datablock ParticleData(KralmokExpFireball)
{
   textureName = "art/shapes/particles/fireball.png";
   lifetimeMS = "300";
   lifetimeVarianceMS = "299";
   spinSpeed = "1";
   spinRandomMin = "-400";
   spinRandomMax = "0";
   animTexName = "art/shapes/particles/fireball.png";
   colors[0] = "1 0 0 0.2";
   colors[1] = "1 0 0 0.6";
   colors[2] = "0.0944882 0.0944882 0.0944882 0";
   sizes[0] = "0.997986";
   sizes[1] = "1.99902";
   sizes[2] = "2.99701";
   times[1] = "0.498039";
   times[2] = "1";
   times[3] = "1";
   gravityCoefficient = "-1";
};

datablock ParticleEmitterData(KralmokExpFireballEmitter)
{
   particles = "KralmokExpFireball";
   blendStyle = "ADDITIVE";
   ejectionPeriodMS = "10";
   periodVarianceMS = "5";
   ejectionVelocity = "4";
   velocityVariance = "2";
   ejectionOffset = "2";
   thetaMax = "120";
};

datablock ParticleData(KralmokExpSmoke)
{
   lifetimeMS = 1200;//"1250";
   lifetimeVarianceMS = 299;//200;//"250";
   textureName = "art/shapes/particles/smoke";
   animTexName = "art/shapes/particles/smoke";
   useInvAlpha = "1";
   gravityCoefficient = "-0.100122";
   spinSpeed = "1";
   spinRandomMin = "-100";
   spinRandomMax = "0";
   colors[0] = "0 255 255 0.4";//"0.192157 0.192157 0.192157 0.0944882";
   colors[1] = "139 10 80 0.8";//"0.454902 0.454902 0.454902 0.897638";
   colors[2] = "0.4 0.4 0.4 0";//"1 1 1 0";
   sizes[0] = "1.99597";
   sizes[1] = "3.99805";
   sizes[2] = "7.99915";
   times[1] = "0.494118";
   times[2] = "1";
   times[3] = "1";
};

datablock ParticleEmitterData(KralmokExpSmokeEmitter)
{
   ejectionPeriodMS = "15";
   periodVarianceMS = "5";
   //ejectionOffset = "1";
   thetaMax = "180";
   particles = "RocketExpSmoke";
   blendStyle = "NORMAL";
};

datablock ParticleData(KralmokExpSparks)
{
   textureName = "art/shapes/particles/droplet.png";
   lifetimeMS = "100";
   lifetimeVarianceMS = "50";
   animTexName = "art/shapes/particles/droplet.png";
   inheritedVelFactor = "0.391389";
   sizes[0] = "1.99902";
   sizes[1] = "2.49954";
   sizes[2] = "0.997986";
   colors[0] = "1.0 0.9 0.8 0.2";
   colors[1] = "1.0 0.9 0.8 0.8";
   colors[2] = "0.8 0.4 0.0 0.0";
   times[0] = "0";
   times[1] = "0.34902";
   times[2] = "1";
   times[3] = "1";
};

datablock ParticleEmitterData(KralmokExpSparksEmitter)
{
   particles = "RocketExpSparks";
   blendStyle = "NORMAL";
   ejectionPeriodMS = "10";
   periodVarianceMS = "5";
   ejectionVelocity = "60";
   velocityVariance = "10";
   thetaMax = "120";
   phiReferenceVel = 0;
   phiVariance = "360";
   ejectionOffset = "0";
   orientParticles = true;
   orientOnVelocity = true;
};

datablock ParticleData(KralmokExpSubFireParticles)
{
   textureName = "art/shapes/particles/fireball.png";
   gravityCoefficient = "-0.202686";
   lifetimeMS = "400";
   lifetimeVarianceMS = "299";
   spinSpeed = "1";
   spinRandomMin = "-200";
   spinRandomMax = "0";
   animTexName = "art/shapes/particles/fireball.png";
   colors[0] = "1 0.897638 0.795276 0.2";
   colors[1] = "1 0.496063 0 1";
   colors[2] = "0.0944882 0.0944882 0.0944882 0";
   sizes[0] = "0.997986";
   sizes[1] = "1.99902";
   sizes[2] = "2.99701";
   times[1] = "0.498039";
   times[2] = "1";
   times[3] = "1";
};

datablock ParticleEmitterData(KralmokExpSubFireEmitter)
{
   particles = "RocketExpSubFireParticles";
   blendStyle = "ADDITIVE";
   ejectionPeriodMS = "10";
   periodVarianceMS = "5";
   ejectionVelocity = "4";
   velocityVariance = "2";
   thetaMax = "120";
};

datablock ParticleData(KralmokExpSubSmoke)
{
   textureName = "art/shapes/particles/smoke";
   gravityCoefficient = "-0.40293";
   lifetimeMS = "800";
   lifetimeVarianceMS = "299";
   spinSpeed = "1";
   spinRandomMin = "-200";
   spinRandomMax = "0";
   animTexName = "art/shapes/particles/smoke";
   colors[0] = "0.4 0.35 0.3 0.393701";
   colors[1] = "0.45 0.45 0.45 0.795276";
   colors[2] = "0.4 0.4 0.4 0";
   sizes[0] = "1.99902";
   sizes[1] = "3.99805";
   sizes[2] = "7.99915";
   times[1] = "0.4";
   times[2] = "1";
   times[3] = "1";
};

datablock ParticleEmitterData(KralmokExpSubSmokeEmitter)
{
   particles = "RocketExpSubSmoke";
   ejectionPeriodMS = "30";
   periodVarianceMS = "10";
   ejectionVelocity = "2";
   velocityVariance = "1";
   ejectionOffset = 1;//"2";
   blendStyle = "NORMAL";
};

datablock ExplosionData(KralmokSubExplosion) {
   lifeTimeMS = 100;
   offset = 0.4;
   emitter[0] = KralmokExpSubFireEmitter;
   emitter[1] = KralmokExpSubSmokeEmitter;
};


datablock ExplosionData(KralmokExplosion)
{
   soundProfile = RocketLauncherExplosionSound;
   lifeTimeMS = 200; // I want a quick bang and dissipation, not a slow burn-out

   // Volume particles
   particleEmitter = KralmokExpSmokeEmitter;
   particleDensity = 10;//20;
   particleRadius = 1;//2;

   // Point emission
   emitter[0] = KralmokExpFireEmitter;
   emitter[1] = KralmokExpSparksEmitter;
   emitter[2] = KralmokExpSparksEmitter;
   emitter[3] = KralmokExpFireballEmitter;

   // Sub explosion objects
   subExplosion[0] = KralmokSubExplosion;

   // Camera Shaking
   shakeCamera = true;
   camShakeFreq = "10.0 11.0 9.0";
   camShakeAmp = "15.0 15.0 15.0";
   camShakeDuration = 1.5;
   camShakeRadius = 20;

   lightStartRadius = 6.0;
   lightEndRadius = 0.0;
   lightStartColor = "1.0 0.7 0.2";
   lightEndColor = "0.9 0.7 0.0";
   lightStartBrightness = 2.5;
   lightEndBrightness = 0.0;
   lightNormalOffset = 3.0;
};


//--------------------------------------------------------------------------
// Shell ejected during reload.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Projectile Object
//-----------------------------------------------------------------------------
datablock ProjectileData(KralmokProjectile) {
   projectileShapeName = "";
   directDamage = 90;
   radiusDamage = 95;
   damageRadius = 7;
   areaImpulse = 2500;

   explosion = KralmokExplosion;
   waterExplosion = KralmokExplosion;

   decal = ScorchRXDecal;
   splash = RocketSplash;

   particleEmitter = KralmokProjSmokeTrailEmitter;
   particleWaterEmitter = KralmokProjSmokeTrailEmitter;

   muzzleVelocity = 100;
   velInheritFactor = 0.3;

   armingDelay = 0;
   lifetime = 5000; //(500m / 100m/s = 5000ms)
   fadeDelay = 4500;

   bounceElasticity = 0;
   bounceFriction = 0;
   isBallistic = false;
   gravityMod = 0.80;

   lightDesc = RocketLauncherLightDesc;

   damageType = "Kralmok";
};

function KralmokProjectile::onExplode(%data, %proj, %position, %mod) {
	%totalD = 0;
   InitContainerRadiusSearch(%position, %data.damageRadius, $TypeMasks::ShapeBaseObjectType);

   while ((%targetObject = containerSearchNext()) != 0) {
      %coverage = calcExplosionCoverage(%position, %targetObject,
                                        $TypeMasks::InteriorObjectType |
                                        $TypeMasks::TerrainObjectType |
                                        $TypeMasks::ForceFieldObjectType |
                                        $TypeMasks::StaticShapeObjectType |
                                        $TypeMasks::VehicleObjectType);
		if (%coverage == 0) {
         continue;
		}
		if(%targetObject.client.team == %proj.sourceObject.client.team) {
		   continue;
		}

		if(%proj.sourceObject.spec(9) $= "Shockwave I" || %proj.sourceObject.spec(9) $= "Shockwave II" || %proj.sourceObject.spec(9) $= "Shockwave III") {
		   //Kralmok Shockwave = Insta Chain Disentegrate
			%counter = ChainDisintegrate(%targetObject, 5, %proj);
			if(%counter >= 5) {
			   commandToClient(%proj.sourceObject.getControllingClient(), 'addShockwave', 9);
			}
			break;
		}
		else {
		   if(getRandom(0, 100) > 35) {
		      %targetObject.damage(%proj, %pos, 0.1, "Kralmok");
			   Disintegrate(%targetObject, %proj);   //bye bye...

			   %totalD++;
			   return;		   
		   }
		}
	}

	if(%totalD >= 7) {
	   commandToClient(%proj.sourceObject.client, 'addKralmokSeven');
	}

   %damage = %data.radiusDamage;
	%impulse = %data.areaImpulse;
   if(%proj.sourceObject.spec(9) $= "Shockwave I") {
		%damage *= (1.1);
		%impulse *= (1.1);
   }
   else if(%proj.sourceObject.spec(9) $= "Shockwave II") {
	   %damage *= (1.25);
		%impulse *= (1.25);
	}
   else if(%proj.sourceObject.spec(9) $= "Shockwave III") {
		%damage *= (1.5);
		%impulse *= (1.5);
   }
   else if(%proj.sourceObject.spec(9) $= "Firepower I") {
		%damage *= (1.25);
   }
   else if(%proj.sourceObject.spec(9) $= "Firepower II") {
	   %damage *= (1.5);
	}
   else if(%proj.sourceObject.spec(9) $= "Firepower III") {
		%damage *= (2);
   }
	radiusDamage(%proj, %position, %data.damageRadius, %damage, %data.damageType, %data.areaImpulse);
}



//--------------------------------------------------------------------------
// Weapon Item.  This is the item that exists in the world, i.e. when it's
// been dropped, thrown or is acting as re-spawnable item.  When the weapon
// is mounted onto a shape, the KralmokWeaponImage is used.
//-----------------------------------------------------------------------------
datablock ItemData(KralmokClip)
{
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
   pickUpName = "Kralmok clip";
   maxInventory = 7;
};

datablock ItemData(KralmokAmmo)
{
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
   pickUpName = "Kralmok ammo";
   maxInventory = 1;
	clip = KralmokClip;
};

datablock ItemData(Kralmok)
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
   PreviewImage = "kralmok.png";
   pickUpName = "Kralmok Plasma Cannon";
   description = "Kralmok";
   image = KralmokWeaponImage;
   reticle = "crossHair";
};


datablock ShapeBaseImageData(KralmokWeaponImage)
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

	ammoImage = "Kralmok";

   // Projectiles and Ammo.
   item = Kralmok;
   ammo = KralmokAmmo;
	clip = KralmokClip;
   //usesEnergy = true;

	weaponID = 9;
   
   projectile = KralmokProjectile;
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
   stateSound[1]                    = KralmokSwitchinSound;

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
   stateSound[2]                    = KralmokIdleSound;

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
   stateSound[4]                    = KralmokRunSound;

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
   stateSound[5]                    = KralmokFireSound;
   //stateEmitter[5]                  = GunFireSmokeEmitter;
   //stateEmitterTime[5]              = 0.025;

   // Play the reload animation, and transition into
   stateName[6]                     = "Reload";
   stateTransitionGeneric0In[6]     = "SprintEnter";
   stateTransitionOnNoAmmo[6]       = "NoAmmo";
   stateTransitionOnTimeout[6]      = "Ready";
   stateWaitForTimeout[6]           = "0";
   stateTimeoutValue[6]             = 3.8;
   stateAllowImageChange[6]         = false;
   stateSequence[6]                 = "reload";
   stateShapeSequence[6]            = "Reload";
   stateEjectShell[6]               = true;
   stateSound[6]                    = KralmokReloadSound;

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
   stateTimeoutValue[10]             = 3.8;
   stateReload[10]                   = true;
   stateSequence[10]                 = "reload";
   stateShapeSequence[10]            = "Reload";
   stateScaleShapeSequence[10]       = true;
   stateSound[10]                    = KralmokReloadSound;

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

function KralmokWeaponImage::onFire(%this, %obj, %slot) {
   %spread = %obj.getWeaponSpread("Kralmok");

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