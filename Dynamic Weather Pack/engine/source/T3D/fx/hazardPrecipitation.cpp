/**
hazardPrecipitation.cpp
Robert Fritzen
(c) Phantom Games Development, 2013

Central file for the hazardPrecipitation class.

This class essentially acts in the same way the normal Precipitation class
behaves, however it allows you to define an effect to objects when they are
in areas where precip. will land, essentially you could make acid rain zones
that cause damage when outside, but none when inside
**/

#include "platform/platform.h"
#include "T3D/fx/hazardPrecipitation.h"

#include "math/mathIO.h"
#include "console/consoleTypes.h"
#include "console/typeValidators.h"
#include "scene/sceneManager.h"
#include "scene/sceneRenderState.h"
#include "lighting/lightInfo.h"
#include "lighting/lightManager.h"
#include "materials/shaderData.h"
#include "T3D/gameBase/gameConnection.h"
#include "T3D/player.h"
#include "core/stream/bitStream.h"
#include "platform/profiler.h"
#include "renderInstance/renderPassManager.h"
#include "sfx/sfxSystem.h"
#include "sfx/sfxTrack.h"
#include "sfx/sfxSource.h"
#include "sfx/sfxTypes.h"
#include "console/engineAPI.h"
#include "particleEmitter.h"

static const U32 dropHitMask = 
   TerrainObjectType |
   WaterObjectType |
   StaticShapeObjectType;

IMPLEMENT_CO_NETOBJECT_V1(hazardPrecipitation);

ConsoleDocClass( hazardPrecipitation,
   "@brief Defines a hazardPrecipitation based storm (rain, snow, etc).\n\n"

   "The hazardPrecipitation effect works by creating many 'drops' within a fixed size "
   "box. This box can be configured to move around with the camera (to simulate "
   "level-wide hazardPrecipitation), or to remain in a fixed position (to simulate "
   "localized hazardPrecipitation). When #followCam is true, the box containing the "
   "droplets can be thought of as centered on the camera then pushed slightly "
   "forward in the direction the camera is facing so most of the box is in "
   "front of the camera (allowing more drops to be visible on screen at once).\n\n"

   "The effect can also be configured to create a small 'splash' whenever a drop "
   "hits another world object.\n\n"

   "@tsexample\n"
   "// The following is added to a level file (.mis) by the World Editor\n"
   "new hazardPrecipitation( TheRain ) {\n"
   "   dropSize = \"0.5\";\n"
   "   splashSize = \"0.5\";\n"
   "   splashMS = \"250\";\n"
   "   animateSplashes = \"1\";\n"
   "   dropAnimateMS = \"0\";\n"
   "   fadeDist = \"0\";\n"
   "   fadeDistEnd = \"0\";\n"
   "   useTrueBillboards = \"0\";\n"
   "   useLighting = \"0\";\n"
   "   glowIntensity = \"0 0 0 0\";\n"
   "   reflect = \"0\";\n"
   "   rotateWithCamVel = \"1\";\n"
   "   doCollision = \"1\";\n"
   "   hitPlayers = \"1\";\n"
   "   hitVehicles = \"0\";\n"
   "   useWind = \"0\";\n"
   "   minSpeed = \"1.5\";\n"
   "   maxSpeed = \"2\";\n"
   "   minMass = \"0.75\";\n"
   "   maxMass = \"0.85\";\n"
   "   useTurbulence = \"0\";\n"
   "   maxTurbulence = \"0.1\";\n"
   "   turbulenceSpeed = \"0.2\";\n"
   "   numDrops = \"1024\";\n"
   "   boxWidth = \"200\";\n"
   "   boxHeight = \"100\";\n"
   "   dataBlock = \"HeavyRain\";\n"
   "};\n"
   "@endtsexample\n"
   "@ingroup FX\n"
   "@ingroup Atmosphere\n"
   "@see hazardPrecipitationData\n"
);

IMPLEMENT_CALLBACK( hazardPrecipitation, onHitObject, void, ( const Point3F& hitPosition, SceneObject* hitObject ),
   ( hitPosition, hitObject ),
   "Informs an object that the hazardPrecipitation is affecting it.\n"
   "@param hitPosition World position hit by the individual droplet.\n"
   "@param hitObject Player or Vehicle object that was hit.\n"
   "@tsexample\n"
   "function hazardPrecipitation::onHitObject( %this, %hitPosition, %hitObject )\n"
   "{\n"
   "   // apply damage to the object\n"
   "   %hitObject.applyDamage( 0.1 );\n"
   "}\n"
   "@endtsexample\n"
);

//----------------------------------------------------------
// hazardPrecipitation!
//----------------------------------------------------------
hazardPrecipitation::hazardPrecipitation() {
   mTypeMask |= ProjectileObjectType;

   mDataBlock = NULL;

   mTexCoords = NULL;
   mSplashCoords = NULL;

   mDropShader = NULL;
   mDropHandle = NULL;

   mSplashShader = NULL;
   mSplashHandle = NULL;

   mDropHead   = NULL;
   mSplashHead = NULL;
   mNumDrops   = 1024;
   mPercentage = 1.0;

   mMinSpeed   = 1.5;
   mMaxSpeed   = 2.0;

   mFollowCam = true;

   mLastRenderFrame = 0;

   mDropHitMask = 0;

   mDropSize          = 0.5;
   mSplashSize        = 0.5;
   mUseTrueBillboards = false;
   mSplashMS          = 250;

   mAnimateSplashes  = true;
   mDropAnimateMS    = 0;

   mUseLighting = false;
   mGlowIntensity = ColorF( 0,0,0,0 );

   mReflect = false;

   mUseWind = false;

   mBoxWidth   = 200;
   mBoxHeight  = 100;
   mFadeDistance = 0;
   mFadeDistanceEnd = 0;

   mMinMass    = 0.75f;
   mMaxMass    = 0.85f;

   mMaxTurbulence = 0.1f;
   mTurbulenceSpeed = 0.2f;
   mUseTurbulence = false;

   mRotateWithCamVel = true;

   mDoCollision = true;
   mDropHitPlayers = false;
   mDropHitVehicles = false;

   mStormData.valid = false;
   mStormData.startPct = 0;
   mStormData.endPct = 0;
   mStormData.startTime = 0;
   mStormData.totalTime = 0;

   mTurbulenceData.valid = false;
   mTurbulenceData.startTime = 0;
   mTurbulenceData.totalTime = 0;
   mTurbulenceData.startMax = 0;
   mTurbulenceData.startSpeed = 0;
   mTurbulenceData.endMax = 0;
   mTurbulenceData.endSpeed = 0;

   mAmbientSound = NULL;

   mDropShaderModelViewSC = NULL;
   mDropShaderFadeStartEndSC = NULL;
   mDropShaderCameraPosSC = NULL;
   mDropShaderAmbientSC = NULL;

   mSplashShaderModelViewSC = NULL;
   mSplashShaderFadeStartEndSC = NULL;
   mSplashShaderCameraPosSC = NULL;
   mSplashShaderAmbientSC = NULL;
}

hazardPrecipitation::~hazardPrecipitation() {
   SAFE_DELETE_ARRAY(mTexCoords);
   SAFE_DELETE_ARRAY(mSplashCoords);
}

void hazardPrecipitation::inspectPostApply() {
   mObjBox.minExtents = -Point3F(mBoxWidth/2, mBoxWidth/2, mBoxHeight/2);
   mObjBox.maxExtents = Point3F(mBoxWidth/2, mBoxWidth/2, mBoxHeight/2);

   resetWorldBox();
   setMaskBits(DataMask);
}

void hazardPrecipitation::setTransform(const MatrixF & mat) {
   Parent::setTransform(mat);

   setMaskBits(TransformMask);
}

//--------------------------------------------------------------------------
// Console stuff...
//--------------------------------------------------------------------------
void hazardPrecipitation::initPersistFields() {
   Parent::initPersistFields();
}

//-----------------------------------
// Console methods...
DefineEngineMethod(hazardPrecipitation, setPercentage, void, (F32 percentage), (1.0f),
   "Sets the maximum number of drops in the effect, as a percentage of #numDrops.\n"
   "The change occurs instantly (use modifyStorm() to change the number of drops "
   "over a period of time.\n"
   "@param percentage New maximum number of drops value (as a percentage of "
   "#numDrops). Valid range is 0-1.\n"
   "@tsexample\n"
   "%percentage = 0.5;  // The percentage, from 0 to 1, of the maximum drops to display\n"
   "%hazardPrecipitation.setPercentage( %percentage );\n"
   "@endtsexample\n"
   "@see modifyStorm\n" )
{
   object->setPercentage(percentage);
}

DefineEngineMethod(hazardPrecipitation, modifyStorm, void, (F32 percentage, F32 seconds), (1.0f, 5.0f),
   "Smoothly change the maximum number of drops in the effect (from current "
   "value to #numDrops * @a percentage).\n"
   "This method can be used to simulate a storm building or fading in intensity "
   "as the number of drops in the hazardPrecipitation box changes.\n"
   "@param percentage New maximum number of drops value (as a percentage of "
   "#numDrops). Valid range is 0-1.\n"
   "@param seconds Length of time (in seconds) over which to increase the drops "
   "percentage value. Set to 0 to change instantly.\n"
   "@tsexample\n"
   "%percentage = 0.5;  // The percentage, from 0 to 1, of the maximum drops to display\n"
   "%seconds = 5.0;     // The length of time over which to make the change.\n"
   "%hazardPrecipitation.modifyStorm( %percentage, %seconds );\n"
   "@endtsexample\n" )
{
   object->modifyStorm(percentage, S32(seconds * 1000.0f));
}

DefineEngineMethod(hazardPrecipitation, setTurbulence, void, (F32 max, F32 speed, F32 seconds), (1.0f, 5.0f, 5.0),
   "Smoothly change the turbulence parameters over a period of time.\n"
   "@param max New #maxTurbulence value. Set to 0 to disable turbulence.\n"
   "@param speed New #turbulenceSpeed value.\n"
   "@param seconds Length of time (in seconds) over which to interpolate the "
   "turbulence settings. Set to 0 to change instantly.\n"
   "@tsexample\n"
   "%turbulence = 0.5;     // Set the new turbulence value. Set to 0 to disable turbulence.\n"
   "%speed = 5.0;          // The new speed of the turbulance effect.\n"
   "%seconds = 5.0;        // The length of time over which to make the change.\n"
   "%hazardPrecipitation.setTurbulence( %turbulence, %speed, %seconds );\n"
   "@endtsexample\n" )
{
   object->setTurbulence( max, speed, S32(seconds * 1000.0f));
}

//--------------------------------------------------------------------------
// Backend
//--------------------------------------------------------------------------
bool hazardPrecipitation::onAdd() {
   if(!Parent::onAdd())
      return false;

   mObjBox.minExtents = -Point3F(mBoxWidth/2, mBoxWidth/2, mBoxHeight/2);
   mObjBox.maxExtents = Point3F(mBoxWidth/2, mBoxWidth/2, mBoxHeight/2);

   resetWorldBox();

   if (isClientObject()) {
      fillDropList();
      initRenderObjects();
      initMaterials();
   }

   addToScene();

   return true;
}

void hazardPrecipitation::onRemove() {
   removeFromScene();
   Parent::onRemove();

   SFX_DELETE( mAmbientSound );

   if (isClientObject())
      killDropList();
}

bool hazardPrecipitation::onNewDataBlock( GameBaseData *dptr, bool reload ) {
   mDataBlock = dynamic_cast<PrecipitationData*>( dptr );
   if ( !mDataBlock || !Parent::onNewDataBlock( dptr, reload ) )
      return false;

   if (isClientObject())
   {
      SFX_DELETE( mAmbientSound );

      if ( mDataBlock->soundProfile )
      {
         mAmbientSound = SFX->createSource( mDataBlock->soundProfile, &getTransform() );
         if ( mAmbientSound )
            mAmbientSound->play();
      }

      initRenderObjects();
      initMaterials();
   }

   scriptOnNewDataBlock();
   return true;
}

void hazardPrecipitation::initMaterials() {
	Parent::initMaterials();
}

U32 hazardPrecipitation::packUpdate(NetConnection* con, U32 mask, BitStream* stream) {
   Parent::packUpdate(con, mask, stream);

	return 0;
}

void hazardPrecipitation::unpackUpdate(NetConnection* con, BitStream* stream) {
   Parent::unpackUpdate(con, stream);
}

//--------------------------------------------------------------------------
// Support functions
//--------------------------------------------------------------------------
VectorF hazardPrecipitation::getWindVelocity() {
   // The WindManager happens to set global-wind velocity here, it is not just for particles.
   return mUseWind ? ParticleEmitter::mWindVelocity : Point3F::Zero;
}

void hazardPrecipitation::fillDropList() {
	Parent::fillDropList();
}

void hazardPrecipitation::initRenderObjects() {
	Parent::initRenderObjects();
}

void hazardPrecipitation::killDropList() {
	Parent::killDropList();
}

void hazardPrecipitation::spawnDrop(Raindrop *drop) {
	Parent::spawnDrop(drop);
}

void hazardPrecipitation::spawnNewDrop(Raindrop *drop) {
	Parent::spawnNewDrop(drop);
}

void hazardPrecipitation::wrapDrop(Raindrop *drop, const Box3F &box, const U32 currTime, const VectorF &windVel) {
	Parent::wrapDrop(drop, box, currTime, windVel);
}

void hazardPrecipitation::findDropCutoff(Raindrop *drop, const Box3F &box, const VectorF &windVel) {
	Parent::findDropCutoff(drop, box, windVel);
}

void hazardPrecipitation::createSplash(Raindrop *drop) {
	Parent::createSplash(drop);
}

void hazardPrecipitation::destroySplash(Raindrop *drop) {
	Parent::destroySplash(drop);
}

//--------------------------------------------------------------------------
// Processing
//--------------------------------------------------------------------------
void hazardPrecipitation::setPercentage(F32 pct) {
	Parent::setPercentage(pct);
}

void hazardPrecipitation::modifyStorm(F32 pct, U32 ms) {
	Parent::modifyStorm(pct, ms);
}

void hazardPrecipitation::setTurbulence(F32 max, F32 speed, U32 ms) {
	Parent::setTurbulence(max, speed, ms);
}

void hazardPrecipitation::interpolateTick(F32 delta) {
	Parent::interpolateTick(delta);
}

void hazardPrecipitation::processTick(const Move *m) {
   if (isServerObject() || mDataBlock != NULL) {
      // Perform checks to see what is and is not in the precip. zone
      Box3F queryBox;
	   queryBox = getWorldBox();
      
	   SimpleQueryList sql;
      getContainer()->findObjects(queryBox, (PlayerObjectType | VehicleObjectType),
                                  SimpleQueryList::insertionCallback, &sql);

      for( U32 i = 0; i < sql.mList.size(); i++ ) {
         Point3F objectCenter;
         sql.mList[i]->getObjBox().getCenter(&objectCenter);
         objectCenter.convolve(sql.mList[i]->getScale());
         sql.mList[i]->getTransform().mulP(objectCenter);	

         RayInfo rayInfo;
         Point3F start = objectCenter;
         start.z += 1000; //Phantom139: Abstract value
         Point3F end = objectCenter;

         bool rayHit = gServerContainer.castRay( start, end,
                                                (0xFFFFFFFF),
                                                &rayInfo);

         if( rayHit && rayInfo.object == sql.mList[i] ) {
            //Welcome to the kill box.
            doObjectHit(end, sql.mList[i], true);
		   }
		   else {
		      //out of the kill-zone
            doObjectHit(end, sql.mList[i], false);
		   }
	   }
   }
	Parent::processTick(m);
}

//--------------------------------------------------------------------------
// Hazard Effect
//--------------------------------------------------------------------------
void hazardPrecipitation::doObjectHit(Point3F hitPosition, SceneObject *object, bool entering) {
   //we've already checked by means of the splash if the object is being "hit"	
	if(entering) {
      String result = object->getDataField(StringTable->insert("inHazardZone"), 0);
		if(dAtoi(result.c_str()) == 0) {
	      object->setDataField(StringTable->insert("inHazardZone"), NULL, "1"); 
		   onHitObject_callback(hitPosition, object);
	   }   
   }
   else {
		object->setDataField(StringTable->insert("inHazardZone"), NULL, "0");   
   }
}

//--------------------------------------------------------------------------
// Rendering
//--------------------------------------------------------------------------
void hazardPrecipitation::prepRenderImage(SceneRenderState* state) {
	Parent::prepRenderImage(state);
}

void hazardPrecipitation::renderObject(ObjectRenderInst *ri, SceneRenderState *state, BaseMatInstance* overrideMat) {
	Parent::renderObject(ri, state, overrideMat);
}
