//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Sounds
//--------------------------------------------------------------------------

datablock SFXProfile(EnderFireSound)
{
   filename = "art/sound/soldier_gun_pack/wpn_ender_fire";
   description = AudioClose3D;
   preload = true;
};

datablock SFXProfile(EnderReloadSound)
{
   filename = "art/sound/soldier_gun_pack/wpn_ender_reload";
   description = AudioClose3D;
   preload = true;
};

datablock SFXProfile(EnderSwitchinSound)
{
   filename = "art/sound/soldier_gun_pack/wpn_ender_switchin";
   description = AudioClose3D;
   preload = true;
};

datablock SFXProfile(RocketLauncherExplosionSound)
{
   filename = "art/sound/weapons/Crossbow_explosion";
   description = AudioDefault3d;
   preload = true;
};

// ----------------------------------------------------------------------------
// Lights for the projectile(s)
// ----------------------------------------------------------------------------

datablock LightDescription(RocketLauncherLightDesc)
{
   range = 4.0;
   color = "1 1 0";
   brightness = 5.0;
   animationType = PulseLightAnim;
   animationPeriod = 0.25;
   //flareType = SimpleLightFlare0;
};

datablock LightDescription(RocketLauncherWaterLightDesc)
{
   radius = 2.0;
   color = "1 1 1";
   brightness = 5.0;
   animationType = PulseLightAnim;
   animationPeriod = 0.25;
   //flareType = SimpleLightFlare0;
};

//----------------------------------------------------------------------------
// Debris
//----------------------------------------------------------------------------

datablock ParticleData(RocketDebrisTrailParticle)
{
   textureName = "art/shapes/particles/impact";
   dragCoeffiecient = 0;
   inheritedVelFactor = 0.0;
   constantAcceleration = 0.0;
   lifetimeMS = 1200;//1000;
   lifetimeVarianceMS = 299;//500;
   useInvAlpha = true;//false;
   spinSpeed = 1;
   spinRandomMin = -300.0;
   spinRandomMax = 0;
   colors[0] = "1 0.897638 0.795276 0.4";
   colors[1] = "0.795276 0.795276 0.795276 0.6";
   colors[2] = "0 0 0 0";
   sizes[0] = 0.5;//1.0;
   sizes[1] = 2;
   sizes[2] = 1;//1.0;
   times[0] = 0.0;
   times[1] = 0.498039;
   times[2] = 1.0;
   animTexName = "art/shapes/particles/impact";
   times[3] = "1";
};

datablock ParticleEmitterData(RocketDebrisTrailEmitter)
{
   ejectionPeriodMS = 6;//8;
   periodVarianceMS = 2;//4;
   ejectionVelocity = 1.0;
   velocityVariance = 0.5;
   thetaMin = 0.0;
   thetaMax = 180.0;
   phiReferenceVel = 0;
   phiVariance = 360;
   ejectionoffset = 0.0;//0.3;
   particles = "RocketDebrisTrailParticle";
};

datablock DebrisData(RocketDebris)
{
   shapeFile = "art/shapes/weapons/SwarmGun/rocket.dts";
   emitters[0] = RocketDebrisTrailEmitter;
   elasticity = 0.5;
   friction = 0.5;
   numBounces = 1;//2;
   bounceVariance = 1;
   explodeOnMaxBounce = true;
   staticOnMaxBounce = false;
   snapOnMaxBounce = false;
   minSpinSpeed = 400;
   maxSpinSpeed = 800;
   render2D = false;
   lifetime = 0.25;//0.5;//1;//2;
   lifetimeVariance = 0.0;//0.25;//0.5;
   velocity = 35;//30;//15;
   velocityVariance = 10;//5;
   fade = true;
   useRadiusMass = true;
   baseRadius = 0.3;
   gravModifier = 1.0;
   terminalVelocity = 45;
   ignoreWater = false;
};

// ----------------------------------------------------------------------------
// Splash effects
// ----------------------------------------------------------------------------

datablock ParticleData(RocketSplashMist)
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
   sizes[0] = 0.2;//0.5;
   sizes[1] = 0.4;//0.5;
   sizes[2] = 0.8;
   times[0] = 0.0;
   times[1] = 0.5;
   times[2] = 1.0;
};

datablock ParticleEmitterData(RocketSplashMistEmitter)
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
   particles = "RocketSplashMist";
};

datablock ParticleData(RocketSplashParticle)
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

datablock ParticleEmitterData(RocketSplashEmitter)
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
   particles = "RocketSplashParticle";
};

datablock ParticleData(RocketSplashRingParticle)
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

datablock ParticleEmitterData(RocketSplashRingEmitter)
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
   particles = "RocketSplashRingParticle";
};

datablock SplashData(RocketSplash)
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

   emitter[0] = RocketSplashEmitter;
   emitter[1] = RocketSplashMistEmitter;
   emitter[2] = RocketSplashRingEmitter;

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

datablock ParticleData(RocketExpFire)
{
   gravityCoefficient = "-0.50061";
   lifetimeMS = "400";
   lifetimeVarianceMS = "299";
   spinSpeed = "1";
   spinRandomMin = "-200";
   spinRandomMax = "0";
   textureName = "art/shapes/particles/smoke";
   animTexName = "art/shapes/particles/smoke";
   colors[0] = "1 0.897638 0.795276 1";
   colors[1] = "0.795276 0.393701 0 0.6";
   colors[2] = "0 0 0 0";
   sizes[0] = "1.99902";
   sizes[1] = "7.99915";
   sizes[2] = "3.99805";
   times[1] = "0.392157";
   times[2] = "1";
   times[3] = "1";
};

datablock ParticleEmitterData(RocketExpFireEmitter)
{
   ejectionPeriodMS = "10";
   periodVarianceMS = "5";
   ejectionVelocity = "3";
   velocityVariance = "2";
   particles = "RocketExpFire";
   blendStyle = "NORMAL";
};

datablock ParticleData(RocketExpFireball)
{
   textureName = "art/shapes/particles/fireball.png";
   lifetimeMS = "300";
   lifetimeVarianceMS = "299";
   spinSpeed = "1";
   spinRandomMin = "-400";
   spinRandomMax = "0";
   animTexName = "art/shapes/particles/fireball.png";
   colors[0] = "1 0.897638 0.795276 0.2";
   colors[1] = "1 0.496063 0 0.6";
   colors[2] = "0.0944882 0.0944882 0.0944882 0";
   sizes[0] = "0.997986";
   sizes[1] = "1.99902";
   sizes[2] = "2.99701";
   times[1] = "0.498039";
   times[2] = "1";
   times[3] = "1";
   gravityCoefficient = "-1";
};

datablock ParticleEmitterData(RocketExpFireballEmitter)
{
   particles = "RocketExpFireball";
   blendStyle = "ADDITIVE";
   ejectionPeriodMS = "10";
   periodVarianceMS = "5";
   ejectionVelocity = "4";
   velocityVariance = "2";
   ejectionOffset = "2";
   thetaMax = "120";
};

datablock ParticleData(RocketExpSmoke)
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
   colors[0] = "0.897638 0.795276 0.692913 0.4";//"0.192157 0.192157 0.192157 0.0944882";
   colors[1] = "0.897638 0.897638 0.897638 0.8";//"0.454902 0.454902 0.454902 0.897638";
   colors[2] = "0.4 0.4 0.4 0";//"1 1 1 0";
   sizes[0] = "1.99597";
   sizes[1] = "3.99805";
   sizes[2] = "7.99915";
   times[1] = "0.494118";
   times[2] = "1";
   times[3] = "1";
};

datablock ParticleEmitterData(RocketExpSmokeEmitter)
{
   ejectionPeriodMS = "15";
   periodVarianceMS = "5";
   //ejectionOffset = "1";
   thetaMax = "180";
   particles = "RocketExpSmoke";
   blendStyle = "NORMAL";
};

datablock ParticleData(RocketExpSparks)
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

datablock ParticleEmitterData(RocketExpSparksEmitter)
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

datablock ParticleData(RocketExpSubFireParticles)
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

datablock ParticleEmitterData(RocketExpSubFireEmitter)
{
   particles = "RocketExpSubFireParticles";
   blendStyle = "ADDITIVE";
   ejectionPeriodMS = "10";
   periodVarianceMS = "5";
   ejectionVelocity = "4";
   velocityVariance = "2";
   thetaMax = "120";
};

datablock ParticleData(RocketExpSubSmoke)
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

datablock ParticleEmitterData(RocketExpSubSmokeEmitter)
{
   particles = "RocketExpSubSmoke";
   ejectionPeriodMS = "30";
   periodVarianceMS = "10";
   ejectionVelocity = "2";
   velocityVariance = "1";
   ejectionOffset = 1;//"2";
   blendStyle = "NORMAL";
};

// ----------------------------------------------------------------------------
// Water Explosion
// ----------------------------------------------------------------------------

datablock ParticleData(RLWaterExpDust)
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

datablock ParticleEmitterData(RLWaterExpDustEmitter)
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
   particles = "RLWaterExpDust";
};

datablock ParticleData(RLWaterExpSparks)
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

datablock ParticleEmitterData(RLWaterExpSparkEmitter)
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
   particles = "RLWaterExpSparks";
};

datablock ParticleData(RLWaterExpSmoke)
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

datablock ParticleEmitterData(RLWaterExpSmokeEmitter)
{
   ejectionPeriodMS = 15;
   periodVarianceMS = 0;
   ejectionVelocity = 6.25;
   velocityVariance = 0.25;
   thetaMin = 0.0;
   thetaMax = 90.0;
   lifetimeMS = 250;
   particles = "RLWaterExpSmoke";
};

datablock ParticleData(RLWaterExpBubbles)
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

datablock ParticleEmitterData(RLWaterExpBubbleEmitter)
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
   particles = "RLWaterExpBubbles";
};

datablock ExplosionData(RocketLauncherWaterExplosion)
{
   //soundProfile = RLWaterExplosionSound;

   emitter[0] = RLWaterExpDustEmitter;
   emitter[1] = RLWaterExpSparkEmitter;
   emitter[2] = RLWaterExpSmokeEmitter;
   emitter[3] = RLWaterExpBubbleEmitter;

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

datablock ExplosionData(RocketSubExplosion)
{
   lifeTimeMS = 100;
   offset = 0.4;
   emitter[0] = RocketExpSubFireEmitter;
   emitter[1] = RocketExpSubSmokeEmitter;
};


datablock ExplosionData(RocketLauncherExplosion)
{
   soundProfile = RocketLauncherExplosionSound;
   lifeTimeMS = 200; // I want a quick bang and dissipation, not a slow burn-out

   // Volume particles
   particleEmitter = RocketExpSmokeEmitter;
   particleDensity = 10;//20;
   particleRadius = 1;//2;

   // Point emission
   emitter[0] = RocketExpFireEmitter;
   emitter[1] = RocketExpSparksEmitter;
   emitter[2] = RocketExpSparksEmitter;
   emitter[3] = RocketExpFireballEmitter;

   // Sub explosion objects
   subExplosion[0] = RocketSubExplosion;

   // Camera Shaking
   shakeCamera = true;
   camShakeFreq = "10.0 11.0 9.0";
   camShakeAmp = "15.0 15.0 15.0";
   camShakeDuration = 1.5;
   camShakeRadius = 20;

   // Exploding debris
   debris = RocketDebris;
   debrisThetaMin = 0;//10;
   debrisThetaMax = 90;//80;
   debrisNum = 5;
   debrisNumVariance = 2;
   debrisVelocity = 1;//2;
   debrisVelocityVariance = 0.2;//0.5;

   lightStartRadius = 6.0;
   lightEndRadius = 0.0;
   lightStartColor = "1.0 0.7 0.2";
   lightEndColor = "0.9 0.7 0.0";
   lightStartBrightness = 2.5;
   lightEndBrightness = 0.0;
   lightNormalOffset = 3.0;
};

// ----------------------------------------------------------------------------
// Normal-fire Projectile Object
// ----------------------------------------------------------------------------

datablock ParticleData(RocketProjSmokeTrail)
{
   textureName = "art/shapes/particles/smoke";
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

   colors[0] = "0.3 0.3 0.3 0.598425";
   colors[1] = "0.9 0.9 0.9 0.897638";
   colors[2] = "0.9 0.9 0.9 0";

   sizes[0] = 0.247207;
   sizes[1] = 0.497467;
   sizes[2] = 0.747726;

   times[0] = 0.0;
   times[1] = 0.4;
   times[2] = 1.0;
   animTexName = "art/shapes/particles/smoke";
   times[3] = "1";
};

datablock ParticleEmitterData(RocketProjSmokeTrailEmitter)
{
   ejectionPeriodMS = 1;
   periodVarianceMS = 0;
   ejectionVelocity = 0.75;
   velocityVariance = 0;
   thetaMin = 0.0;
   thetaMax = 0.0;
   phiReferenceVel = 90;
   phiVariance = 0;
   particles = "RocketProjSmokeTrail";
};

datablock ProjectileData(RocketLauncherProjectile)
{
   projectileShapeName = "art/shapes/weapons/SwarmGun/rocket.dts";
   directDamage = 90;
   radiusDamage = 85;
   damageRadius = 5;
   areaImpulse = 2500;

   explosion = RocketLauncherExplosion;
   waterExplosion = RocketLauncherWaterExplosion;

   decal = ScorchRXDecal;
   splash = RocketSplash;

   particleEmitter = RocketProjSmokeTrailEmitter;
   particleWaterEmitter = RocketProjSmokeTrailEmitter;

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

   damageType = "Rocket Launcher";
};

function RocketLauncherProjectile::onExplode(%data, %proj, %position, %mod) {
   %damage = %data.radiusDamage;
	%impulse = %data.areaImpulse;
   if(%proj.sourceObject.spec(5) $= "Shockwave I") {
		%damage *= (1.1);
		%impulse *= (1.1);
   }
   else if(%proj.sourceObject.spec(5) $= "Shockwave II") {
	   %damage *= (1.25);
		%impulse *= (1.25);
	}
   else if(%proj.sourceObject.spec(5) $= "Shockwave III") {
		%damage *= (1.5);
		%impulse *= (1.5);
   }
   else if(%proj.sourceObject.spec(5) $= "Firepower I") {
		%damage *= (1.25);
   }
   else if(%proj.sourceObject.spec(5) $= "Firepower II") {
	   %damage *= (1.5);
	}
   else if(%proj.sourceObject.spec(5) $= "Firepower III") {
		%damage *= (2);
   }

	if(%proj.sourceObject.spec(5) $= "Shockwave I" || %proj.sourceObject.spec(5) $= "Shockwave II" || %proj.sourceObject.spec(5) $= "Shockwave III") {
	   radiusDamage(%proj, %position, %data.damageRadius, %damage, %data.damageType, %impulse, "RocketLauncherProjectile_ShockwavePostDamage");
	}
	else {
	   radiusDamage(%proj, %position, %data.damageRadius, %damage, %data.damageType, %impulse);
	}
}

function RocketLauncherProjectile_ShockwavePostDamage(%srcPlayer, %target) {
   if(%target.getState() $= "dead") {
      commandToClient(%srcPlayer.getControllingClient(), 'addShockwave', 5);
	}
}

//------------------------------------------------------
//Mini-Nuke
datablock ParticleData(RocketLauncher_MiniNukeDomeParticle : DefaultParticle) {
   dragCoefficient = "0.125";
   gravityCoefficient = "1";
   inheritedVelFactor = "3.125";
   constantAcceleration = "-0.42";
   spinSpeed = "0.146";
   spinRandomMin = "-0.5";
   textureName = "art/shapes/particles/steam.png";
   animTexName = "art/shapes/particles/steam.png";
   colors[0] = "0.309804 0.254902 0.0470588 1";
   colors[1] = "0.686275 0.341176 0.0901961 0.637795";
   sizes[0] = "5.20833";
   sizes[1] = "3.125";
   sizes[2] = "2";
   times[1] = "0.5625";
   times[2] = "0.9375";
};

datablock ParticleEmitterData(RocketLauncherMiniNuke_BaseEmitter : DefaultEmitter) {
   ejectionPeriodMS = "1";
   ejectionVelocity = "8.333";
   ejectionOffset = "2.708";
   thetaMax = "138.8";
   particles = "RocketLauncher_MiniNukeDomeParticle";
   lifetimeMS = "1500";
};

datablock ExplosionData(RocketLauncherMiniNukeExplosion) {
   soundProfile = RocketLauncherExplosionSound;
   lifeTimeMS = 1500; // Boom, burn, and die...

   // Volume particles
   particleEmitter = RocketExpSmokeEmitter;
   particleDensity = 10;//20;
   particleRadius = 1;//2;

   // Point emission
   emitter[0] = RocketLauncherMiniNuke_BaseEmitter;
   emitter[1] = RocketExpSparksEmitter;
   emitter[2] = RocketExpSparksEmitter;
   emitter[3] = RocketExpFireballEmitter;

   // Sub explosion objects
   subExplosion[0] = RocketSubExplosion;

   // Camera Shaking
   shakeCamera = true;
   camShakeFreq = "10.0 11.0 9.0";
   camShakeAmp = "15.0 15.0 15.0";
   camShakeDuration = 1.5;
   camShakeRadius = 20;

   // Exploding debris
   debris = RocketDebris;
   debrisThetaMin = 0;//10;
   debrisThetaMax = 90;//80;
   debrisNum = 5;
   debrisNumVariance = 2;
   debrisVelocity = 1;//2;
   debrisVelocityVariance = 0.2;//0.5;

   lightStartRadius = 6.0;
   lightEndRadius = 0.0;
   lightStartColor = "1.0 0.0 0.0";
   lightEndColor = "0.9 0.7 0.0";
   lightStartBrightness = 5.5;
   lightEndBrightness = 0.0;
   lightNormalOffset = 3.0;
};

datablock ProjectileData(RocketLauncherMiniNukeProjectile) {
   projectileShapeName = "art/shapes/weapons/SwarmGun/rocket.dts";
   directDamage = 90;
   radiusDamage = 125;
   damageRadius = 7.5;
   areaImpulse = 2500;

   explosion = RocketLauncherMiniNukeExplosion;
   waterExplosion = RocketLauncherMiniNukeExplosion;

   decal = ScorchRXDecal;
   splash = RocketSplash;

   particleEmitter = RocketProjSmokeTrailEmitter;
   particleWaterEmitter = RocketProjSmokeTrailEmitter;

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

   damageType = "Rocket Launcher";
};

function RocketLauncherMiniNukeProjectile::onExplode(%data, %proj, %position, %mod) {
   %damage = %data.radiusDamage;
	%impulse = %data.areaImpulse;
   if(%proj.sourceObject.spec(5) $= "Shockwave I") {
		%damage *= (1.1);
		%impulse *= (1.1);
   }
   else if(%proj.sourceObject.spec(5) $= "Shockwave II") {
	   %damage *= (1.25);
		%impulse *= (1.25);
	}
   else if(%proj.sourceObject.spec(5) $= "Shockwave III") {
		%damage *= (1.5);
		%impulse *= (1.5);
   }
   else if(%proj.sourceObject.spec(5) $= "Firepower I") {
		%damage *= (1.25);
   }
   else if(%proj.sourceObject.spec(5) $= "Firepower II") {
	   %damage *= (1.5);
	}
   else if(%proj.sourceObject.spec(5) $= "Firepower III") {
		%damage *= (2);
   }

	radiusDamage(%proj, %position, %data.damageRadius, %damage, %data.damageType, %impulse, "RocketLauncherProjectile_MiniNukePostDamage");
}

function RocketLauncherProjectile_MiniNukePostDamage(%srcPlayer, %target) {
   if(%target.getState() $= "dead") {
      commandToClient(%srcPlayer.getControllingClient(), 'addMiniNuke');
	}
}

// ----------------------------------------------------------------------------
// Shell that's ejected during reload.
// ----------------------------------------------------------------------------

datablock DebrisData(RocketlauncherShellCasing)
{
   shapeFile = "art/shapes/weapons/SwarmGun/rocket.dts";
   lifetime = 6.0;
   minSpinSpeed = 300.0;
   maxSpinSpeed = 400.0;
   elasticity = 0.65;
   friction = 0.05;
   numBounces = 5;
   staticOnMaxBounce = true;
   snapOnMaxBounce = false;
   fade = true;
};

//-----------------------------------------------------------------------------
// Ammo Item
//-----------------------------------------------------------------------------
datablock ItemData(EnderClip)
{
   // Mission editor category
   category = "AmmoClip";

   // Add the Ammo namespace as a parent.  The ammo namespace provides
   // common ammo related functions and hooks into the inventory system.
   className = "AmmoClip";

   // Basic Item properties
   shapeFile = "art/shapes/weapons/Ender/TP_Ender.DAE";
   mass = 1;
   elasticity = 0.2;
   friction = 0.6;

   // Dynamic properties defined by the scripts
   pickUpName = "Ender clip";
   maxInventory = 8;
};

datablock ItemData(EnderAmmo)
{
   // Mission editor category
   category = "Ammo";

   // Add the Ammo namespace as a parent.  The ammo namespace provides
   // common ammo related functions and hooks into the inventory system.
   className = "Ammo";

   // Basic Item properties
   shapeFile = "art/shapes/weapons/Ender/TP_Ender.DAE";
   mass = 1;
   elasticity = 0.2;
   friction = 0.6;

   // Dynamic properties defined by the scripts
   pickUpName = "Ender ammo";
   maxInventory = 1;
	clip = EnderClip;
};
//--------------------------------------------------------------------------
// Weapon Item.  This is the item that exists in the world, i.e. when it's
// been dropped, thrown or is acting as re-spawnable item.  When the weapon
// is mounted onto a shape, the EnderWeaponImage is used.
//-----------------------------------------------------------------------------
datablock ItemData(Ender)
{
   // Mission editor category
   category = "Weapon";

   // Hook into Item Weapon class hierarchy. The weapon namespace
   // provides common weapon handling functions in addition to hooks
   // into the inventory system.
   className = "Weapon";

   // Basic Item properties
   shapeFile = "art/shapes/weapons/Ender/TP_Ender.DAE";
   mass = 1;
   elasticity = 0.2;
   friction = 0.6;
   emap = true;

   // Dynamic properties defined by the scripts
   PreviewImage = "RocketLauncher.png";
   pickUpName = "Ender rocket launcher";
   description = "Ender";
   image = EnderWeaponImage;
   reticle = "crossHair";
};


datablock ShapeBaseImageData(EnderWeaponImage)
{
   // Basic Item properties
   shapeFile = "art/shapes/weapons/Ender/TP_Ender.DAE";
   shapeFileFP = "art/shapes/weapons/Ender/FP_Ender.DAE";
   emap = true;

   imageAnimPrefix = "Ender";
   imageAnimPrefixFP = "Ender";

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

	ammoImage = "Ender";

   // Projectiles and Ammo.
   item = Ender;
   ammo = EnderAmmo;
   clip = EnderClip; 

	weaponID = 5;
   
   projectile = RocketLauncherProjectile;
   wetProjectile = RocketLauncherProjectile;
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
   stateSound[1]                    = EnderSwitchinSound;

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
   stateSound[5]                    = EnderFireSound;
   stateEmitter[5]                  = GunFireSmokeEmitter;
   stateEmitterTime[5]              = 0.025;

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
   stateSound[6]                    = EnderReloadSound;

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
   stateSound[10]                    = EnderReloadSound;

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

function EnderWeaponImage::onFire(%this, %obj, %slot) {
   %spread = %obj.getWeaponSpread("Ender");
   if(%obj.spec(5) $= "Mini Nuke") {
	   %projectile = RocketLauncherMiniNukeProjectile;
	}
	else {
	   %projectile = RocketLauncherProjectile;
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

   return %p;
}