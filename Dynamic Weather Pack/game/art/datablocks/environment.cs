//-----------------------------------------------------------------------------
// Copyright (c) 2012 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

// ENVIROMENTAL EFFECTS GO HERE (PRECIPITATION - LIGHTNING)

// ----------------------------------------------------------------------------
// Rain
// ----------------------------------------------------------------------------

datablock SFXProfile(HeavyRainSound)
{
   filename = "art/sound/environment/amb";
   description = AudioLoop2d;
};

datablock PrecipitationData(HeavyRain)
{
   soundProfile = "HeavyRainSound";

   dropTexture = "art/environment/precipitation/rain";
   splashTexture = "art/environment/precipitation/water_splash";
   dropSize = 0.35;
   splashSize = 0.1;
   useTrueBillboards = false;
   splashMS = 500;
};

datablock PrecipitationData(Rain)
{
   soundProfile = "HeavyRainSound";

   dropTexture = "art/environment/precipitation/rain2";
   splashTexture = "art/environment/precipitation/water_splash";
   dropSize = 0.35;
   splashSize = 0.1;
   useTrueBillboards = false;
   splashMS = 500;
};

datablock PrecipitationData(Hail)
{
   //soundProfile = "HeavyRainSound";

   dropTexture = "art/environment/precipitation/hail";
   splashTexture = "art/environment/precipitation/water_splash";
   dropSize = 0.35;
   splashSize = 0.1;
   useTrueBillboards = false;
   splashMS = 500;
};

datablock PrecipitationData(Snow)
{
   //soundProfile = "HeavyRainSound";

   dropTexture = "art/environment/precipitation/snow";
   //splashTexture = "art/environment/precipitation/water_splash";
   dropSize = 0.35;
   //splashSize = 0.1;
   useTrueBillboards = false;
   //splashMS = 500;
};

datablock PrecipitationData(Snow2)
{
   //soundProfile = "HeavyRainSound";

   dropTexture = "art/environment/precipitation/snow2";
   //splashTexture = "art/environment/precipitation/water_splash";
   dropSize = 0.35;
   //splashSize = 0.1;
   useTrueBillboards = false;
   //splashMS = 500;
};

datablock ParticleData(TornadoSmoke) {
   dragCoeffiecient     = 0.1;
   gravityCoefficient   = -0.6;
   inheritedVelFactor   = 0.4;

   constantAcceleration = -0.0;

   lifetimeMS           = 5000;
   lifetimeVarianceMS   = 250;

   textureName          = "particleTest";

   useInvAlpha =  true;
   spinRandomMin = -150.0;
   spinRandomMax =  150.0;

   textureName = "art/particles/smoke";

   colors[0]     = "0.3 0.3 0.3 1.0";
   colors[1]     = "0.3 0.3 0.3 0.5";
   colors[2]     = "0.3 0.3 0.3 0.0";

   sizes[0]      = 5.0;
   sizes[1]      = 7.0;
   sizes[2]      = 10.0;

   times[0]      = 0.0;
   times[1]      = 0.2;
   times[2]      = 1.0;

};

datablock ParticleEmitterData(TornadoSmokeEmitter) {
   ejectionPeriodMS = 50;
   periodVarianceMS = 0;

   ejectionVelocity = 1.0;
   velocityVariance = 0.5;

   thetaMin         = 0.0;
   thetaMax         = 180.0;

   particles = "TornadoSmoke";
};

// ----------------------------------------------------------------------------
// Lightning
// ----------------------------------------------------------------------------

// When setting up thunder sounds for lightning it should be known that:
// - strikeSound is a 3d sound
// - thunderSounds[n] are 2d sounds

datablock SFXProfile(ThunderCrash1Sound)
{
   filename = "art/sound/environment/thunder1";
   description = Audio2d;
};

datablock SFXProfile(ThunderCrash2Sound)
{
   filename = "art/sound/environment/thunder2";
   description = Audio2d;
};

datablock SFXProfile(ThunderCrash3Sound)
{
   filename = "art/sound/environment/thunder3";
   description = Audio2d;
};

datablock SFXProfile(ThunderCrash4Sound)
{
   filename = "art/sound/environment/thunder4";
   description = Audio2d;
};


datablock LightningData(DefaultStorm)
{
   thunderSounds[0] = ThunderCrash1Sound;
   thunderSounds[1] = ThunderCrash2Sound;
   thunderSounds[2] = ThunderCrash3Sound;
   thunderSounds[3] = ThunderCrash4Sound;
};
