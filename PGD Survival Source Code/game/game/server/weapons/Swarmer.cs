//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Sound profiles
// ----------------------------------------------------------------------------

datablock SFXProfile(SwarmerReloadSound)
{
   filename = "art/sound/weapons/Crossbow_reload";
   description = AudioClose3d;
   preload = true;
};

datablock SFXProfile(SwarmerFireSound)
{
   filename = "art/sound/weapons/explosion_mono_01";
   description = AudioClose3d;
   preload = true;
};

datablock SFXProfile(SwarmerIncLoadSound)
{
   filename = "art/sound/weapons/relbow_mono_01";
   description = AudioClose3d;
   preload = true;
};

datablock SFXProfile(SwarmerFireEmptySound)
{
   filename = "art/sound/weapons/Crossbow_firing_empty";
   description = AudioClose3d;
   preload = true;
};

// ----------------------------------------------------------------------------
// Lights for the projectile(s)
// ----------------------------------------------------------------------------

datablock LightDescription(GrenadeLauncherLightDesc)
{
   range = 1.0;
   color = "1 1 1";
   brightness = 2.0;
   animationType = PulseLightAnim;
   animationPeriod = 0.25;
   //flareType = SimpleLightFlare0;
};

datablock LightDescription(GrenadeLauncherWaterLightDesc)
{
   radius = 2.0;
   color = "1 1 1";
   brightness = 2.0;
   animationType = PulseLightAnim;
   animationPeriod = 0.25;
   //flareType = SimpleLightFlare0;
};

// ----------------------------------------------------------------------------
// Splash effects
// ----------------------------------------------------------------------------

datablock ParticleData(GrenadeSplashMist)
{
   dragCoefficient = 1.0;
   windCoefficient = 2.0;
   gravityCoefficient = 0.3;
   inheritedVelFactor = 0.0;
   constantAcceleration = 0.0;
   lifetimeMS = 600;
   lifetimeVarianceMS = 100;
   useInvAlpha = false;
   spinRandomMin = -90.0;
   spinRandomMax = 500.0;
   spinSpeed = 1;

   textureName = "art/shapes/particles/smoke";

   colors[0] = "0.7 0.8 1.0 1.0";
   colors[1] = "0.7 0.8 1.0 0.5";
   colors[2] = "0.7 0.8 1.0 0.0";

   sizes[0] = 0.2;
   sizes[1] = 0.4;
   sizes[2] = 0.8;

   times[0] = 0.0;
   times[1] = 0.5;
   times[2] = 1.0;
};

datablock ParticleEmitterData(GrenadeSplashMistEmitter)
{
   ejectionPeriodMS = 5;
   periodVarianceMS = 0;
   ejectionVelocity = 3.0;
   velocityVariance = 2.0;
   ejectionOffset = 0.15;
   thetaMin = 85;
   thetaMax = 85;
   phiReferenceVel = 0;
   phiVariance = 360;
   overrideAdvance = false;
   lifetimeMS = 250;
   particles = "GrenadeSplashMist";
};

datablock ParticleData(GrenadeSplashParticle)
{
   dragCoefficient = 1;
   windCoefficient = 0.9;
   gravityCoefficient = 0.3;
   inheritedVelFactor = 0.2;
   constantAcceleration = -1.4;
   lifetimeMS = 600;
   lifetimeVarianceMS = 200;
   textureName = "art/shapes/particles/droplet";

   colors[0] = "0.7 0.8 1.0 1.0";
   colors[1] = "0.7 0.8 1.0 0.5";
   colors[2] = "0.7 0.8 1.0 0.0";

   sizes[0] = 0.5;
   sizes[1] = 0.25;
   sizes[2] = 0.25;

   times[0] = 0.0;
   times[1] = 0.5;
   times[2] = 1.0;
};

datablock ParticleEmitterData(GrenadeSplashEmitter)
{
   ejectionPeriodMS = 4;
   periodVarianceMS = 0;
   ejectionVelocity = 7.3;
   velocityVariance = 2.0;
   ejectionOffset = 0.0;
   thetaMin = 30;
   thetaMax = 80;
   phiReferenceVel = 00;
   phiVariance = 360;
   overrideAdvance = false;
   orientParticles = true;
   orientOnVelocity = true;
   lifetimeMS = 100;
   particles = "GrenadeSplashParticle";
};

datablock ParticleData(GrenadeSplashRingParticle)
{
   textureName = "art/shapes/particles/wake";
   dragCoefficient = 0.0;
   gravityCoefficient = 0.0;
   inheritedVelFactor = 0.0;
   lifetimeMS = 2500;
   lifetimeVarianceMS = 200;
   windCoefficient = 0.0;
   useInvAlpha = 1;
   spinRandomMin = 30.0;
   spinRandomMax = 30.0;
   spinSpeed = 1;

   animateTexture = true;
   framesPerSec = 1;
   animTexTiling = "2 1";
   animTexFrames = "0 1";

   colors[0] = "0.7 0.8 1.0 1.0";
   colors[1] = "0.7 0.8 1.0 0.5";
   colors[2] = "0.7 0.8 1.0 0.0";

   sizes[0] = 2.0;
   sizes[1] = 4.0;
   sizes[2] = 8.0;

   times[0] = 0.0;
   times[1] = 0.5;
   times[2] = 1.0;
};

datablock ParticleEmitterData(GrenadeSplashRingEmitter)
{
   lifetimeMS = "100";
   ejectionPeriodMS = 200;
   periodVarianceMS = 10;
   ejectionVelocity = 0;
   velocityVariance = 0;
   ejectionOffset = 0;
   thetaMin = 89;
   thetaMax = 90;
   phiReferenceVel = 0;
   phiVariance = 1;
   alignParticles = 1;
   alignDirection = "0 0 1";
   particles = "GrenadeSplashRingParticle";
};

datablock SplashData(GrenadeSplash)
{
//    numSegments = 15;
//    ejectionFreq = 15;
//    ejectionAngle = 40;
//    ringLifetime = 0.5;
//    lifetimeMS = 300;
//    velocity = 4.0;
//    startRadius = 0.0;
//    acceleration = -3.0;
//    texWrap = 5.0;
//    texture = "art/shapes/particles/splash";

   emitter[0] = GrenadeSplashEmitter;
   emitter[1] = GrenadeSplashMistEmitter;
   emitter[2] = GrenadeSplashRingEmitter;

//    colors[0] = "0.7 0.8 1.0 0.0";
//    colors[1] = "0.7 0.8 1.0 0.3";
//    colors[2] = "0.7 0.8 1.0 0.7";
//    colors[3] = "0.7 0.8 1.0 0.0";
//
//    times[0] = 0.0;
//    times[1] = 0.4;
//    times[2] = 0.8;
//    times[3] = 1.0;
};

// ----------------------------------------------------------------------------
// Explosion Particle effects
// ----------------------------------------------------------------------------

datablock ParticleData(GrenadeExpFire)
{
   textureName = "art/shapes/particles/fireball";
   dragCoeffiecient = 0;
   windCoeffiecient = 0.5;
   gravityCoefficient = -0.300366;
   inheritedVelFactor = 0.299413;
   constantAcceleration = 0.2;
   lifetimeMS = 2000;//3000;
   lifetimeVarianceMS = 299;//200;
   useInvAlpha = false;
   spinRandomMin = -80.0;
   spinRandomMax = 0;
   spinSpeed = 1;
   colors[0] = "0.795276 0.393701 0 0.795276";
   colors[1] = "0.19685 0.0944882 0 0.393701";
   colors[2] = "0 0 0 0";
   sizes[0] = 0.75;//2;
   sizes[1] = 1.5;
   sizes[2] = 3;//0.5;
   times[0] = 0.0;
   times[1] = 0.498039;
   times[2] = 1.0;
   animTexName = "art/shapes/particles/Fireball";
   times[3] = "1";
};

datablock ParticleEmitterData(GrenadeExpFireEmitter)
{
   ejectionPeriodMS = 10;
   periodVarianceMS = 5;//0;
   ejectionVelocity = 4;//1.0;
   velocityVariance = 1;//0.5;
   thetaMin = 0.0;
   thetaMax = 180.0;
   lifetimeMS = 250;
   particles = "GrenadeExpFire";
};

datablock ParticleData(GrenadeExpDust)
{
   textureName = "art/shapes/particles/smoke";
   dragCoefficient = 1.0;
   gravityCoefficient = -0.01;
   inheritedVelFactor = 0.0;
   constantAcceleration = 0.0;
   lifetimeMS = 2500;
   lifetimeVarianceMS = 500;
   useInvAlpha = true;
   spinSpeed = 1;
   spinRandomMin = -90.0;
   spinRandomMax = 90.0;
   colors[0] = "0.6 0.6 0.6 0.3";
   colors[1] = "0.6 0.6 0.6 0.3";
   colors[2] = "0.6 0.6 0.6 0.0";
   sizes[0] = 1.6;
   sizes[1] = 2.0;
   sizes[2] = 2.4;
   times[0] = 0.0;
   times[1] = 0.7;
   times[2] = 1.0;
};

datablock ParticleEmitterData(GrenadeExpDustEmitter)
{
   ejectionPeriodMS = 1;
   periodVarianceMS = 0;
   ejectionVelocity = 15;
   velocityVariance = 0.0;
   ejectionOffset = 0.0;
   thetaMin = 85;
   thetaMax = 85;
   phiReferenceVel = 0;
   phiVariance = 360;
   overrideAdvances = false;
   lifetimeMS = 200;
   particles = "GrenadeExpDust";
};

datablock ParticleData(GrenadeExpSpark)
{
   textureName = "art/shapes/particles/ricochet";
   dragCoefficient = 1;
   gravityCoefficient = 0.0;
   inheritedVelFactor = 0.2;
   constantAcceleration = 0.0;
   lifetimeMS = 500;
   lifetimeVarianceMS = 250;
   colors[0] = "0.6 0.4 0.3 1";
   colors[1] = "0.6 0.4 0.3 1";
   colors[2] = "1.0 0.4 0.3 0";
   sizes[0] = 0.5;
   sizes[1] = 0.75;
   sizes[2] = 1;
   times[0] = 0;
   times[1] = 0.5;
   times[2] = 1;
};

datablock ParticleEmitterData(GrenadeExpSparkEmitter)
{
   ejectionPeriodMS = 2;
   periodVarianceMS = 0;
   ejectionVelocity = 20;
   velocityVariance = 10;
   ejectionOffset = 0.0;
   thetaMin = 0;
   thetaMax = 120;
   phiReferenceVel = 0;
   phiVariance = 360;
   overrideAdvances = false;
   orientParticles = true;
   lifetimeMS = 100;
   particles = "GrenadeExpSpark";
};

datablock ParticleData(GrenadeExpSparks)
{
   textureName = "art/shapes/particles/droplet";
   dragCoefficient = 1;
   gravityCoefficient = 0.0;
   inheritedVelFactor = 0.2;
   constantAcceleration = 0.0;
   lifetimeMS = 500;
   lifetimeVarianceMS = 350;

   colors[0] = "0.6 0.4 0.3 1.0";
   colors[1] = "0.6 0.4 0.3 0.6";
   colors[2] = "1.0 0.4 0.3 0.0";

   sizes[0] = 0.5;
   sizes[1] = 0.5;
   sizes[2] = 0.75;

   times[0] = 0.0;
   times[1] = 0.5;
   times[2] = 1.0;
};

datablock ParticleEmitterData(GrenadeExpSparksEmitter)
{
   ejectionPeriodMS = 2;
   periodVarianceMS = 0;
   ejectionVelocity = 12;
   velocityVariance = 6.75;
   ejectionOffset = 0.0;
   thetaMin = 0;
   thetaMax = 60;
   phiReferenceVel = 0;
   phiVariance = 360;
   overrideAdvances = false;
   orientParticles = true;
   lifetimeMS = 100;
   particles = "GrenadeExpSparks";
};

datablock ParticleData(GrenadeExpSmoke)
{
   textureName = "art/shapes/particles/smoke";
   dragCoeffiecient = 0;
   gravityCoefficient = -0.40293;
   inheritedVelFactor = 0.0;
   constantAcceleration = 0.0;
   lifetimeMS = 800;
   lifetimeVarianceMS = 299;
   useInvAlpha = true;
   spinSpeed = 1;
   spinRandomMin = -80.0;
   spinRandomMax = 0;
   colors[0] = "0.8 0.8 0.8 0.4";
   colors[1] = "0.5 0.5 0.5 0.5";
   colors[2] = "0.75 0.75 0.75 0";
   sizes[0] = 4.49857;
   sizes[1] = 7.49863;
   sizes[2] = 11.2495;
   times[0] = 0;
   times[1] = 0.498039;
   times[2] = 1;
   animTexName = "art/shapes/particles/smoke";
   times[3] = "1";
};

datablock ParticleEmitterData(GrenadeExpSmokeEmitter)
{
   ejectionPeriodMS = 15;
   periodVarianceMS = 5;
   ejectionVelocity = 2.4;
   velocityVariance = 1.2;
   thetaMin = 0.0;
   thetaMax = 180.0;
   ejectionOffset = 1;
   particles = "GrenadeExpSmoke";
};

// ----------------------------------------------------------------------------
// Water Explosion
// ----------------------------------------------------------------------------

datablock ParticleData(GLWaterExpDust)
{
   textureName = "art/shapes/particles/steam";
   dragCoefficient = 1.0;
   gravityCoefficient = -0.01;
   inheritedVelFactor = 0.0;
   constantAcceleration = 0.0;
   lifetimeMS = 2500;
   lifetimeVarianceMS = 250;
   useInvAlpha = false;
   spinSpeed = 1;
   spinRandomMin = -90.0;
   spinRandomMax = 500.0;
   colors[0] = "0.6 0.6 1.0 0.5";
   colors[1] = "0.6 0.6 1.0 0.3";

   sizes[0] = 0.25;
   sizes[1] = 1.5;

   times[0] = 0.0;
   times[1] = 1.0;
};

datablock ParticleEmitterData(GLWaterExpDustEmitter)
{
   ejectionPeriodMS = 1;
   periodVarianceMS = 0;
   ejectionVelocity = 10;
   velocityVariance = 0.0;
   ejectionOffset = 0.0;
   thetaMin = 85;
   thetaMax = 85;
   phiReferenceVel = 0;
   phiVariance = 360;
   overrideAdvances = false;
   lifetimeMS = 75;
   particles = "GLWaterExpDust";
};

datablock ParticleData(GLWaterExpSparks)
{
   textureName = "art/shapes/particles/spark_wet";
   dragCoefficient = 1;
   gravityCoefficient = 0.0;
   inheritedVelFactor = 0.2;
   constantAcceleration = 0.0;
   lifetimeMS = 500;
   lifetimeVarianceMS = 250;

   colors[0] = "0.6 0.6 1.0 1.0";
   colors[1] = "0.6 0.6 1.0 1.0";
   colors[2] = "0.6 0.6 1.0 0.0";

   sizes[0] = 0.5;
   sizes[1] = 0.5;
   sizes[2] = 0.75;

   times[0] = 0.0;
   times[1] = 0.5;
   times[2] = 1.0;
};

datablock ParticleEmitterData(GLWaterExpSparkEmitter)
{
   ejectionPeriodMS = 2;
   periodVarianceMS = 0;
   ejectionVelocity = 12;
   velocityVariance = 6.75;
   ejectionOffset = 0.0;
   thetaMin = 0;
   thetaMax = 60;
   phiReferenceVel = 0;
   phiVariance = 360;
   overrideAdvances = false;
   orientParticles = true;
   lifetimeMS = 100;
   particles = "GLWaterExpSparks";
};

datablock ParticleData(GLWaterExpSmoke)
{
   textureName = "art/shapes/particles/smoke";
   dragCoeffiecient = 0.4;
   gravityCoefficient = -0.25;
   inheritedVelFactor = 0.025;
   constantAcceleration = -1.1;
   lifetimeMS = 1250;
   lifetimeVarianceMS = 0;
   useInvAlpha = false;
   spinSpeed = 1;
   spinRandomMin = -200.0;
   spinRandomMax = 200.0;

   colors[0] = "0.1 0.1 1.0 1.0";
   colors[1] = "0.4 0.4 1.0 1.0";
   colors[2] = "0.4 0.4 1.0 0.0";

   sizes[0] = 2.0;
   sizes[1] = 6.0;
   sizes[2] = 2.0;

   times[0] = 0.0;
   times[1] = 0.5;
   times[2] = 1.0;
};

datablock ParticleEmitterData(GLWaterExpSmokeEmitter)
{
   ejectionPeriodMS = 15;
   periodVarianceMS = 0;
   ejectionVelocity = 6.25;
   velocityVariance = 0.25;
   thetaMin = 0.0;
   thetaMax = 90.0;
   lifetimeMS = 250;
   particles = "GLWaterExpSmoke";
};

datablock ParticleData(GLWaterExpBubbles)
{
   textureName = "art/shapes/particles/millsplash01";
   dragCoefficient = 0.0;
   gravityCoefficient = -0.05;
   inheritedVelFactor = 0.0;
   constantAcceleration = 0.0;
   lifetimeMS = 1500;
   lifetimeVarianceMS = 250;
   useInvAlpha = false;

   spinRandomMin = -100.0;
   spinRandomMax = 100.0;
   spinSpeed = 1;

   colors[0] = "0.7 0.8 1.0 0.0";
   colors[1] = "0.7 0.8 1.0 0.4";
   colors[2] = "0.7 0.8 1.0 0.0";

   sizes[0] = 0.2;
   sizes[1] = 0.4;
   sizes[2] = 0.8;

   times[0] = 0.0;
   times[1] = 0.5;
   times[2] = 1.0;
};

datablock ParticleEmitterData(GLWaterExpBubbleEmitter)
{
   ejectionPeriodMS = 5;
   periodVarianceMS = 0;
   ejectionVelocity = 1.0;
   ejectionOffset = 3.0;
   velocityVariance = 0.5;
   thetaMin = 0;
   thetaMax = 80;
   phiReferenceVel = 0;
   phiVariance = 360;
   overrideAdvances = false;
   particles = "GLWaterExpBubbles";
};

datablock ExplosionData(GrenadeLauncherWaterExplosion)
{
   //soundProfile = GLWaterExplosionSound;

   emitter[0] = GLWaterExpDustEmitter;
   emitter[1] = GLWaterExpSparkEmitter;
   emitter[2] = GLWaterExpSmokeEmitter;
   emitter[3] = GLWaterExpBubbleEmitter;

   shakeCamera = true;
   camShakeFreq = "10.0 11.0 9.0";
   camShakeAmp = "20.0 20.0 20.0";
   camShakeDuration = 1.5;
   camShakeRadius = 20.0;

   lightStartRadius = 20.0;
   lightEndRadius = 0.0;
   lightStartColor = "0.9 0.9 0.8";
   lightEndColor = "0.6 0.6 1.0";
   lightStartBrightness = 2.0;
   lightEndBrightness = 0.0;
};

// ----------------------------------------------------------------------------
// Dry/Air Explosion Objects
// ----------------------------------------------------------------------------

datablock ExplosionData(GrenadeSubExplosion)
{
   offset = 0.25;
   emitter[0] = GrenadeExpSparkEmitter;

   lightStartRadius = 4.0;
   lightEndRadius = 0.0;
   lightStartColor = "0.9 0.7 0.7";
   lightEndColor = "0.9 0.7 0.7";
   lightStartBrightness = 2.0;
   lightEndBrightness = 0.0;
};

datablock ExplosionData(GrenadeLauncherExplosion)
{
   soundProfile = GrenadeLauncherExplosionSound;
   lifeTimeMS = 400; // Quick flash, short burn, and moderate dispersal

   // Volume particles
   particleEmitter = GrenadeExpFireEmitter;
   particleDensity = 75;
   particleRadius = 2.25;

   // Point emission
   emitter[0] = GrenadeExpDustEmitter;
   emitter[1] = GrenadeExpSparksEmitter;
   emitter[2] = GrenadeExpSmokeEmitter;

   // Sub explosion objects
   subExplosion[0] = GrenadeSubExplosion;

   // Camera Shaking
   shakeCamera = true;
   camShakeFreq = "10.0 11.0 9.0";
   camShakeAmp = "15.0 15.0 15.0";
   camShakeDuration = 1.5;
   camShakeRadius = 20;

   lightStartRadius = 4.0;
   lightEndRadius = 0.0;
   lightStartColor = "1.0 1.0 1.0";
   lightEndColor = "1.0 1.0 1.0";
   lightStartBrightness = 4.0;
   lightEndBrightness = 0.0;
   lightNormalOffset = 2.0;
};

// ----------------------------------------------------------------------------
// Normal-fire Projectile Object
// ----------------------------------------------------------------------------

datablock ParticleData(GrenadeProjSmokeTrail)
{
   textureName = "art/shapes/particles/smoke";
   dragCoeffiecient = 0.0;
   gravityCoefficient = -0.2;
   inheritedVelFactor = 0.0;
   constantAcceleration = 0.0;
   lifetimeMS = 750;
   lifetimeVarianceMS = 250;
   useInvAlpha = true;
   spinRandomMin = -60;
   spinRandomMax = 60;
   spinSpeed = 1;

   colors[0] = "0.9 0.8 0.8 0.6";
   colors[1] = "0.6 0.6 0.6 0.9";
   colors[2] = "0.3 0.3 0.3 0";

   sizes[0] = 0.25;
   sizes[1] = 0.5;
   sizes[2] = 0.75;

   times[0] = 0.0;
   times[1] = 0.4;
   times[2] = 1.0;
};

datablock ParticleEmitterData(GrenadeProjSmokeTrailEmitter)
{
   ejectionPeriodMS = 10;
   periodVarianceMS = 0;
   ejectionVelocity = 0.75;
   velocityVariance = 0;
   thetaMin = 0.0;
   thetaMax = 0.0;
   phiReferenceVel = 90;
   phiVariance = 0;
   particles = "GrenadeProjSmokeTrail";
};

datablock ProjectileData(GrenadeLauncherProjectile)
{
   projectileShapeName = "art/shapes/weapons/GrenadeLauncher/rocket.dts";
   directDamage = 30;
   radiusDamage = 30;
   damageRadius = 5;
   areaImpulse = 2000;

   explosion = GrenadeLauncherExplosion;
   waterExplosion = GrenadeLauncherWaterExplosion;

   decal = ScorchRXDecal;
   splash = GrenadeSplash;

   particleEmitter = GrenadeProjSmokeTrailEmitter;
   particleWaterEmitter = GrenadeProjSmokeTrailEmitter;

   muzzleVelocity = 30;
   velInheritFactor = 0.3;

   armingDelay = 750;
   lifetime = 10000;
   fadeDelay = 4500;

   bounceElasticity = 0.4;
   bounceFriction = 0.3;
   isBallistic = true;
   gravityMod = 0.9;

   lightDesc = GrenadeLauncherLightDesc;

   damageType = "Swarmer";
};

function GrenadeLauncherProjectile::onExplode(%data, %proj, %position, %mod) {
   %damage = %data.radiusDamage;
	%impulse = %data.areaImpulse;
   if(%proj.sourceObject.spec(4) $= "Shockwave I") {
		%damage *= (1.1);
		%impulse *= (1.1);
   }
   else if(%proj.sourceObject.spec(4) $= "Shockwave II") {
	   %damage *= (1.25);
		%impulse *= (1.25);
	}
   else if(%proj.sourceObject.spec(4) $= "Shockwave III") {
		%damage *= (1.5);
		%impulse *= (1.5);
   }
   else if(%proj.sourceObject.spec(4) $= "Firepower I") {
		%damage *= (1.25);
   }
   else if(%proj.sourceObject.spec(4) $= "Firepower II") {
	   %damage *= (1.5);
	}
   else if(%proj.sourceObject.spec(4) $= "Firepower III") {
		%damage *= (2);
   }

	if(%proj.sourceObject.spec(4) $= "Shockwave I" || %proj.sourceObject.spec(4) $= "Shockwave II" || %proj.sourceObject.spec(4) $= "Shockwave III") {
	   radiusDamage(%proj, %position, %data.damageRadius, %damage, %data.damageType, %impulse, "GrenadeLauncherProjectile_ShockwavePostDamage");
	}
	else {
	   radiusDamage(%proj, %position, %data.damageRadius, %damage, %data.damageType, %impulse);
	}
}

function GrenadeLauncherProjectile_ShockwavePostDamage(%srcPlayer, %target) {
   if(%target.getState() $= "dead") {
      commandToClient(%srcPlayer.getControllingClient(), 'addShockwave', 4);
	}
}

//Incindiary
datablock ParticleData(GrenadeProjSmokeTrail_Incindiary) {
   textureName = "art/shapes/particles/smoke";
   dragCoeffiecient = 0.0;
   gravityCoefficient = -0.2;
   inheritedVelFactor = 0.0;
   constantAcceleration = 0.0;
   lifetimeMS = 750;
   lifetimeVarianceMS = 250;
   useInvAlpha = true;
   spinRandomMin = -60;
   spinRandomMax = 60;
   spinSpeed = 1;

   colors[0] = "0.984314 0.623529 0.278431 1";
   colors[1] = "0.901961 0.317647 0.121569 0.629921";
   colors[2] = "0.996078 0.2 0.0470588 0.330709";

   sizes[0] = 0.25;
   sizes[1] = 0.5;
   sizes[2] = 0.75;

   times[0] = 0.0;
   times[1] = 0.4;
   times[2] = 1.0;
};

datablock ParticleEmitterData(GrenadeProjSmokeTrailEmitter_Incindiary) {
   ejectionPeriodMS = 10;
   periodVarianceMS = 0;
   ejectionVelocity = 0.75;
   velocityVariance = 0;
   thetaMin = 0.0;
   thetaMax = 0.0;
   phiReferenceVel = 90;
   phiVariance = 0;
   particles = "GrenadeProjSmokeTrail_Incindiary";
};

datablock ProjectileData(GrenadeLauncherProjectile_Incindiary : GrenadeLauncherProjectile) {
   particleEmitter = GrenadeProjSmokeTrailEmitter_Incindiary;
   particleWaterEmitter = GrenadeProjSmokeTrailEmitter_Incindiary;
};

function GrenadeLauncherProjectile_Incindiary::onExplode(%data, %proj, %position, %mod) {
   %damage = %data.radiusDamage;
   if(%proj.sourceObject.spec(4) $= "Firepower I") {
		%damage *= (1.25);
   }
   else if(%proj.sourceObject.spec(4) $= "Firepower II") {
	   %damage *= (1.5);
	}
   else if(%proj.sourceObject.spec(4) $= "Firepower III") {
		%damage *= (2);
   }
	radiusDamage(%proj, %position, %data.damageRadius, %damage, %data.damageType, %data.areaImpulse, "GrenadeLauncherProjectile_IncindiaryPostDamage");
}

function GrenadeLauncherProjectile_IncindiaryPostDamage(%srcPlayer, %target) {
	if(getRandom(0, 100) > 75) {
	   if(%target.client.team == %srcPlayer.client.team) {
		   return;
	   }
		%target.damage(%srcPlayer, %target.getPosition(), 0.1, "Swarmer");
	   Incinerate(%target, %srcPlayer);   //bye bye...

		commandToClient(%srcPlayer.getControllingClient(), 'addIncineration', 4);	
	}
}

// ----------------------------------------------------------------------------
// Particle Emitter played when firing.
// ----------------------------------------------------------------------------

datablock ParticleData(Swarmerfiring1Particle)
{
   textureName = "art/shapes/particles/Fireball";
   dragCoefficient = 100.0;
   gravityCoefficient = -0.25;//-0.5;//0.0;
   inheritedVelFactor = 0.25;//1.0;
   constantAcceleration = 0.1;
   lifetimeMS = 400;
   lifetimeVarianceMS = 100;
   useInvAlpha = false;
   spinSpeed = 1;
   spinRandomMin = -200;
   spinRandomMax = 200;
   colors[0] = "1 0.9 0.8 0.1";
   colors[1] = "1 0.5 0 0.3";
   colors[2] = "0.1 0.1 0.1 0";
   sizes[0] = 0.2;//1;
   sizes[1] = 0.25;//0.15;//0.75;
   sizes[2] = 0.3;//0.1;//0.5;
   times[0] = 0.0;
   times[1] = 0.5;//0.294118;
   times[2] = 1.0;
};

datablock ParticleEmitterData(Swarmerfiring1Emitter)
{
   ejectionPeriodMS = 15;//75;
   periodVarianceMS = 5;
   ejectionVelocity = 1;
   ejectionOffset = 0.0;
   velocityVariance = 0;
   thetaMin = 0.0;
   thetaMax = 180;//10.0;
   particles = "Swarmerfiring1Particle";
   blendStyle = "ADDITIVE";
};

datablock ParticleData(Swarmerfiring2Particle)
{
   textureName = "art/shapes/particles/impact";
   dragCoefficient = 100.0;
   gravityCoefficient = -0.5;//0.0;
   inheritedVelFactor = 0.25;//1.0;
   constantAcceleration = 0.1;
   lifetimeMS = 1600;//400;
   lifetimeVarianceMS = 400;//100;
   useInvAlpha = false;
   spinSpeed = 1;
   spinRandomMin = -200;
   spinRandomMax = 200;
   colors[0] = "0.4 0.4 0.4 0.2";
   colors[1] = "0.4 0.4 0.4 0.1";
   colors[2] = "0.0 0.0 0.0 0.0";
   sizes[0] = 0.2;//1;
   sizes[1] = 0.15;//0.75;
   sizes[2] = 0.1;//0.5;
   times[0] = 0.0;
   times[1] = 0.5;//0.294118;
   times[2] = 1.0;
};

datablock ParticleEmitterData(Swarmerfiring2Emitter)
{
   ejectionPeriodMS = 15;//75;
   periodVarianceMS = 5;
   ejectionVelocity = 1;
   ejectionOffset = 0.0;
   velocityVariance = 0;
   thetaMin = 0.0;
   thetaMax = 180;//10.0;
   particles = "Swarmerfiring2Particle";
   blendStyle = "NORMAL";
};


// ----------------------------------------------------------------------------
// Ammo Item
// ----------------------------------------------------------------------------

datablock ItemData(SwarmerClip)
{
   // Mission editor category
   category = "AmmoClip";

   // Add the Ammo namespace as a parent. The ammo namespace provides
   // common ammo related functions and hooks into the inventory system.
   className = "AmmoClip";

   // Basic Item properties
   shapeFile = "art/shapes/weapons/SwarmGun/rocket.dts";
   mass = 2;
   elasticity = 0.2;
   friction = 0.6;

   // Dynamic properties defined by the scripts
   pickUpName = "Rockets";
   maxInventory = 10;
};

datablock ItemData(SwarmerAmmo)
{
   // Mission editor category
   category = "Ammo";

   // Add the Ammo namespace as a parent. The ammo namespace provides
   // common ammo related functions and hooks into the inventory system.
   className = "Ammo";

   // Basic Item properties
   shapeFile = "art/shapes/weapons/SwarmGun/rocket.dts";
   mass = 2;
   elasticity = 0.2;
   friction = 0.6;

   // Dynamic properties defined by the scripts
   pickUpName = "Rockets";
   maxInventory = 3;
	clip = SwarmerClip;
};

// ----------------------------------------------------------------------------
// Weapon Item. This is the item that exists in the world,
// i.e. when it's been dropped, thrown or is acting as re-spawnable item.
// When the weapon is mounted onto a shape, the Image is used.
// ----------------------------------------------------------------------------

datablock ItemData(Swarmer)
{
   // Mission editor category
   category = "Weapon";

   // Hook into Item Weapon class hierarchy. The weapon namespace
   // provides common weapon handling functions in addition to hooks
   // into the inventory system.
   className = "Weapon";

   // Basic Item properties
   shapefile = "art/shapes/weapons/SwarmGun/swarmgun.dts";
   mass = 5;
   elasticity = 0.2;
   friction = 0.6;
   emap = true;

   // Dynamic properties defined by the scripts
   pickUpName = "SwarmGun";
   description = "Swarmer";
   image = SwarmerImage;

   // weaponHUD
   PreviewImage = "swarmer.png";
   reticle = 'reticle_rocketlauncher';
   zoomReticle = 'bino';
};

// ----------------------------------------------------------------------------
// Image which does all the work. Images do not normally exist in
// the world, they can only be mounted on ShapeBase objects.
// ----------------------------------------------------------------------------

datablock ShapeBaseImageData(SwarmerImage)
{
   // Basic Item properties
   shapefile = "art/shapes/weapons/SwarmGun/swarmgun.dts";
   emap = true;

   // Specify mount point & offset for 3rd person, and eye offset
   // for first person rendering.
   mountPoint = 0;
   offset = "0.0 0.15 0.025";
   eyeOffset = "0.25 0.6 -0.4"; // 0.25=right/left 0.5=forward/backward, -0.5=up/down

   // When firing from a point offset from the eye, muzzle correction
   // will adjust the muzzle vector to point to the eye LOS point.
   // Since this weapon doesn't actually fire from the muzzle point,
   // we need to turn this off.
   correctMuzzleVector = false;

   // Add the WeaponImage namespace as a parent, WeaponImage namespace
   // provides some hooks into the inventory system.
   className = "WeaponImage";

	ammoImage = "Swarmer";

   // Projectile && Ammo.
   item = Swarmer;
   ammo = SwarmerAmmo;
   clip = SwarmerClip;

	weaponID = 4;
   
   projectile = GrenadeLauncherProjectile;
   projectileType = Projectile;

   // shell casings
   casing = RocketlauncherShellCasing;
   shellExitDir = "1.0 0.3 1.0";
   shellExitOffset = "0.15 -0.56 -0.1";
   shellExitVariance = 15.0;
   shellVelocity = 3.0;

   // Let there be light - NoLight, ConstantLight, PulsingLight, WeaponFireLight.
   lightType = "WeaponFireLight";
   lightColor = "1.0 1.0 0.9";
   lightDuration = 200;
   lightRadius = 10;

   // Images have a state system which controls how the animations
   // are run, which sounds are played, script callbacks, etc. This
   // state system is downloaded to the client so that clients can
   // predict state changes and animate accordingly. The following
   // system supports basic ready->fire->reload transitions as
   // well as a no-ammo->dryfire idle state.

   // Initial start up state
   stateName[0] = "Preactivate";
   stateTransitionOnLoaded[0] = "Activate";
   stateTransitionOnNoAmmo[0] = "NoAmmo";

   // Activating the gun.
   // Called when the weapon is first mounted and there is ammo.
   stateName[1] = "Activate";
   stateTransitionOnTimeout[1] = "Ready";
   stateTimeoutValue[1] = 0.6;
   stateSequence[1] = "Activate";

   // Ready to fire, just waiting for the trigger
   stateName[2] = "Ready";
   stateTransitionOnNoAmmo[2] = "NoAmmo";
   stateTransitionOnTriggerDown[2] = "ChargeUp1";
   stateTransitionOnAltTriggerDown[2] = "ChargeUp1";
   stateSequence[2] = "Ready";

   // Fire the weapon. Calls the fire script which does the actual work.
   stateName[3] = "Fire";
   stateTransitionOnTimeout[3] = "PostFire";
   stateTimeoutValue[3] = 0.9;
   stateFire[3] = true;
   stateRecoil[3] = LightRecoil;
   stateAllowImageChange[3] = false;
   stateSequence[3] = "Fire";
   stateScript[3] = "onFire";
   stateSound[3] = SwarmerFireSound;
   stateEmitter[3] = Swarmerfiring1Emitter;
   stateEmitterTime[3] = 0.6;

   // Check ammo
   stateName[4] = "PostFire";
   stateTransitionOnAmmo[4] = "Reload";
   stateTransitionOnNoAmmo[4] = "NoAmmo";

   // Play the reload animation, and transition into
   stateName[5] = "Reload";
   stateTransitionOnTimeout[5] = "Ready";
   stateTimeoutValue[5] = 0.9;
   stateAllowImageChange[5] = false;
   stateSequence[5] = "Reload";
   stateEjectShell[5] = false; // set to true to enable shell casing eject
   stateSound[5] = SwarmerReloadSound;
   stateEmitter[5] = Swarmerfiring2Emitter;
   stateEmitterTime[5] = 2.4;

   // No ammo in the weapon, just idle until something shows up.
   // Play the dry fire sound if the trigger iS pulled.
   stateName[6]                     = "NoAmmo";
   stateTransitionOnAmmo[6]         = "ReloadClip";
   stateTimeoutValue[6]             = 0.1;   // Slight pause to allow script to run when trigger is still held down from Fire state
   stateScript[6]                   = "onClipEmpty";
   stateTransitionOnTriggerDown[6]  = "DryFire";
   stateSequence[6]                 = "idle";
   stateScaleAnimation[6]           = false;
   stateScaleAnimationFP[6]         = false;

   // No ammo dry fire
   stateName[7] = "DryFire";
   stateTimeoutValue[7] = 1.0;
   stateTransitionOnTimeout[7] = "NoAmmo";
   stateSound[7] = SwarmerFireEmptySound;

   // Begin "charge up", 1 in the pipe
   stateName[8] = "ChargeUp1";
   stateScript[8] = "readyLoad";
   stateSound[8] = SwarmerIncLoadSound;
   stateTransitionOnTriggerUp[8] = "Fire";
   stateTransitionOnTimeout[8] = "ChargeUp2";
   stateTimeoutValue[8] = 0.8;
   stateWaitForTimeout[8] = false;

   // Charge up, 2 in the pipe
   stateName[9] = "ChargeUp2";
   stateScript[9] = "incLoad";
   stateSound[9] = SwarmerIncLoadSound;
   stateTransitionOnTriggerUp[9] = "Fire";
   stateTransitionOnTimeout[9] = "ChargeUp3";
   stateTimeoutValue[9] = 0.8;
   stateWaitForTimeout[9] = false;

   // Charge up, 3 in the pipe
   stateName[10] = "ChargeUp3";
   stateScript[10] = "incLoad";
   stateSound[10] = SwarmerIncLoadSound;
   stateTransitionOnTriggerUp[10] = "Fire";
   stateTransitionOnTimeout[10] = "Fire";  // lets force them to fire
   stateTimeOutValue[10] = 1.2;
   stateWaitForTimeout[10] = false;

   stateName[11]                     = "ReloadClip";
   stateTransitionOnTimeout[11]      = "Ready";
   stateWaitForTimeout[11]           = true;
   stateTimeoutValue[11]             = 5.0;
   stateReload[11]                   = true;
   stateSequence[11]                 = "reload";
   stateShapeSequence[11]            = "Reload";
   stateScaleShapeSequence[11]       = true;
   stateSound[11]                    = SwarmerReloadSound;
};

function SwarmerImage::readyLoad(%this) {
   //echo("\c4SwarmerImage::readyLoad("@ %this.getName()@")");
   %this.loadCount = 1;
   //echo("\c4 loadCount = "@ %this.loadCount);
}

function SwarmerImage::incLoad(%this) {
   //echo("\c4SwarmerImage::incLoad("@ %this.getName()@")");
   %this.loadCount++;
   //echo("\c4 loadCount = "@ %this.loadCount);
}

// ----------------------------------------------------------------------------
// The fire method that does all of the work
// ----------------------------------------------------------------------------
function SwarmerImage::onFire(%this, %obj, %slot) {
   %spread = %obj.getWeaponSpread("Swarmer");
	if(%obj.spec(4) $= "Incindiary Rounds") {
	   %projectile = GrenadeLauncherProjectile_Incindiary;
	}
	else {
	   %projectile = GrenadeLauncherProjectile;
	}

   %currentAmmo = %obj.getInventory(%this.ammo);
   if(%currentAmmo < %this.loadCount)
      %this.loadCount = %currentAmmo;

	if(%this.loadCount <= 1) {
	   %obj.isBurstFire = 0;
	}
	else if(%this.loadCount > 1 && %this.loadCount == %currentAmmo) {
	   %obj.isBurstFire = 1;
	}

   for(%shotCount = 0; %shotCount < %this.loadCount; %shotCount++) {
      // Decrement inventory ammo. The image's ammo state is updated
      // automatically by the ammo inventory hooks.
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
   }
   return %p;
}
