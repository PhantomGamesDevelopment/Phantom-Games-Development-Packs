datablock ParticleData(GunFireSmoke)
{
   textureName          = "art/shapes/particles/smoke";
   dragCoefficient      = 0;
   gravityCoefficient   = "-1";
   windCoefficient      = 0;
   inheritedVelFactor   = 0.0;
   constantAcceleration = 0.0;
   lifetimeMS           = 500;
   lifetimeVarianceMS   = 200;
   spinRandomMin = -180.0;
   spinRandomMax =  180.0;
   useInvAlpha   = true;

   colors[0]     = "0.795276 0.795276 0.795276 0.692913";
   colors[1]     = "0.866142 0.866142 0.866142 0.346457";
   colors[2]     = "0.897638 0.834646 0.795276 0";

   sizes[0]      = "0.399805";
   sizes[1]      = "1.19941";
   sizes[2]      = "1.69993";

   times[0]      = 0.0;
   times[1]      = "0.498039";
   times[2]      = 1.0;
   animTexName = "art/shapes/particles/smoke";
};

datablock ParticleEmitterData(GunFireSmokeEmitter)
{
   ejectionPeriodMS = 20;
   periodVarianceMS = 10;
   ejectionVelocity = "0";
   velocityVariance = "0";
   thetaMin         = "0";
   thetaMax         = "0";
   lifetimeMS       = 250;
   particles = "GunFireSmoke";
   blendStyle = "NORMAL";
   softParticles = "0";
   originalName = "GunFireSmokeEmitter";
   alignParticles = "0";
   orientParticles = "0";
};

datablock ParticleData(BulletDirtDust)
{
   textureName          = "art/shapes/particles/impact";
   dragCoefficient      = "1";
   gravityCoefficient   = "-0.100122";
   windCoefficient      = 0;
   inheritedVelFactor   = 0.0;
   constantAcceleration = "-0.83";
   lifetimeMS           = 800;
   lifetimeVarianceMS   = 300;
   spinRandomMin = -180.0;
   spinRandomMax =  180.0;
   useInvAlpha   = true;

   colors[0]     = "0.496063 0.393701 0.299213 0.692913";
   colors[1]     = "0.692913 0.614173 0.535433 0.346457";
   colors[2]     = "0.897638 0.84252 0.795276 0";

   sizes[0]      = "0.997986";
   sizes[1]      = "2";
   sizes[2]      = "2.5";

   times[0]      = 0.0;
   times[1]      = "0.498039";
   times[2]      = 1.0;
   animTexName = "art/shapes/particles/impact";
};

datablock ParticleEmitterData(BulletDirtDustEmitter)
{
   ejectionPeriodMS = 20;
   periodVarianceMS = 10;
   ejectionVelocity = "1";
   velocityVariance = 1.0;
   thetaMin         = 0.0;
   thetaMax         = 180.0;
   lifetimeMS       = 250;
   particles = "BulletDirtDust";
   blendStyle = "NORMAL";
};

datablock ExplosionData(BulletDirtExplosion)
{
   soundProfile = BulletImpactSound;
   lifeTimeMS = 65;

   // Volume particles
   particleEmitter = BulletDirtDustEmitter;
   particleDensity = 4;
   particleRadius = 0.3;

   // Point emission
   emitter[0] = BulletDirtSprayEmitter;
   emitter[1] = BulletDirtSprayEmitter;
   emitter[2] = BulletDirtRocksEmitter;
};
