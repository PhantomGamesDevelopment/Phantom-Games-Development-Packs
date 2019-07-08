//-----------------------------------------------------------------------------
// MissileProjectile.cpp
// (C) 2014 Robert Fritzen for Phantom Games Development
// Source file for the missile MissileProjectile for the Advanced FPS Kit for T3D MIT 3.5
//-----------------------------------------------------------------------------

#include "platform/platform.h"
#include "missileProjectile.h"

#include "scene/sceneRenderState.h"
#include "scene/sceneManager.h"
#include "lighting/lightInfo.h"
#include "lighting/lightManager.h"
#include "console/consoleTypes.h"
#include "console/typeValidators.h"
#include "core/resourceManager.h"
#include "core/stream/bitStream.h"
#include "T3D/fx/explosion.h"
#include "T3D/shapeBase.h"
#include "ts/tsShapeInstance.h"
#include "sfx/sfxTrack.h"
#include "sfx/sfxSource.h"
#include "sfx/sfxSystem.h"
#include "sfx/sfxTypes.h"
#include "math/mathUtils.h"
#include "math/mathIO.h"
#include "sim/netConnection.h"
#include "T3D/fx/particleEmitter.h"
#include "T3D/fx/splash.h"
#include "T3D/physics/physicsPlugin.h"
#include "T3D/physics/physicsWorld.h"
#include "gfx/gfxTransformSaver.h"
#include "T3D/containerQuery.h"
#include "T3D/decal/decalManager.h"
#include "T3D/decal/decalData.h"
#include "T3D/lightDescription.h"
#include "console/engineAPI.h"
#include "core/strings/stringUnit.h"

IMPLEMENT_CO_DATABLOCK_V1(MissileProjectileData);

ConsoleDocClass( MissileProjectileData,
   "@brief Stores properties for a MissileProjectile type.\n"

   "@tsexample\n"
		"datablock MissileProjectileData(MissileLauncherProjectile)\n"
		"{\n"
		  " projectileShapeName = \"art/shapes/weapons/SwarmGun/rocket.dts\";\n"
		   "directDamage = 30;\n"
		   "radiusDamage = 30;\n"
		   "damageRadius = 5;\n"
		   "areaImpulse = 2000;\n"

		   "explosion = GrenadeLauncherExplosion;\n"
		   "waterExplosion = GrenadeLauncherWaterExplosion;\n"

		   "decal = ScorchRXDecal;\n"
		   "splash = GrenadeSplash;\n"

		   "particleEmitter = GrenadeProjSmokeTrailEmitter;\n"
		   "particleWaterEmitter = GrenadeTrailWaterEmitter;\n"

		   "muzzleVelocity = 30;\n"
		   "velInheritFactor = 0.3;\n"
		   "lifetime = 10000;\n"
		   "fadeDelay = 4500;\n"

			"accuracy = 100;\n"

		   "lightDesc = GrenadeLauncherLightDesc;\n"

		   "damageType = \"GrenadeDamage\";\n"
		"};\n"
   "@endtsexample\n"

   "@ingroup gameObjects\n"
);

IMPLEMENT_CO_NETOBJECT_V1(MissileProjectile);

ConsoleDocClass( MissileProjectile,
   "@brief MissileProjectile class. Uses the MissileProjectileData class for properties of individual Missile Projectiles.\n"
   "@ingroup gameObjects\n"
);

IMPLEMENT_CALLBACK( MissileProjectileData, onExplode, void, ( MissileProjectile* proj, Point3F pos, F32 fade ), 
                   ( proj, pos, fade ),
				   "@brief Called when a MissileProjectile explodes.\n\n"
                   "This function is only called on server objects.\n"
                   "@param proj The exploding MissileProjectile.\n"
				   "@param pos The position of the explosion.\n"
				   "@param fade The current fadeValue of the MissileProjectile, affects its visibility.\n\n"
				   "@see MissileProjectile\n"
				  );

IMPLEMENT_CALLBACK( MissileProjectileData, onCollision, void, ( MissileProjectile* proj, SceneObject* col, F32 fade, Point3F pos, Point3F normal ),
                   ( proj, col, fade, pos, normal ),
				   "@brief Called when a MissileProjectile collides with another object.\n\n"
                   "This function is only called on server objects."
				   "@param proj The MissileProjectile colliding with SceneObject col.\n"
				   "@param col The SceneObject hit by the MissileProjectile.\n"
				   "@param fade The current fadeValue of the MissileProjectile, affects its visibility.\n"
				   "@param pos The position of the collision.\n"
                   "@param normal The normal of the collision.\n"
				   "@see MissileProjectile\n"
				  );

const U32 MissileProjectile::csmStaticCollisionMask =  TerrainObjectType | StaticShapeObjectType;

const U32 MissileProjectile::csmDynamicCollisionMask = PlayerObjectType | VehicleObjectType;

const U32 MissileProjectile::csmDamageableMask = MissileProjectile::csmDynamicCollisionMask;

U32 MissileProjectile::smProjectileWarpTicks = 5;


//--------------------------------------------------------------------------
//
MissileProjectileData::MissileProjectileData() {
   projectileShapeName = NULL;

   sound = NULL;

   explosion = NULL;
   explosionId = 0;

   waterExplosion = NULL;
   waterExplosionId = 0;

   lightDesc = NULL;

   faceViewer = false;
   scale.set( 1.0f, 1.0f, 1.0f );

	velInheritFactor = 1.0f;
	muzzleVelocity = 50;
   impactForce = 0.0f;

   fadeDelay = 20000 / 32;
   lifetime = 20000 / 32;

   activateSeq = -1;
   maintainSeq = -1;

   particleEmitter = NULL;
   particleEmitterId = 0;

   particleWaterEmitter = NULL;
   particleWaterEmitterId = 0;

	flareDistance = 0.0f;
	terrainScanAhead = 0.0f;
	terrainHeightFail = 0.0f;

   splash = NULL;
   splashId = 0;

   decal = NULL;
   decalId = 0;

   lightDesc = NULL;
   lightDescId = 0;

	accuracy = 0;
}

//--------------------------------------------------------------------------

void MissileProjectileData::initPersistFields() {
   addField("particleEmitter", TYPEID< ParticleEmitterData >(), Offset(particleEmitter, MissileProjectileData),
      "@brief Particle emitter datablock used to generate particles while the MissileProjectile is outside of water.\n\n"
      "@note If datablocks are defined for both particleEmitter and particleWaterEmitter, both effects will play "
      "as the MissileProjectile enters or leaves water.\n\n"
      "@see particleWaterEmitter\n");
   addField("particleWaterEmitter", TYPEID< ParticleEmitterData >(), Offset(particleWaterEmitter, MissileProjectileData),
      "@brief Particle emitter datablock used to generate particles while the MissileProjectile is submerged in water.\n\n"
      "@note If datablocks are defined for both particleWaterEmitter and particleEmitter , both effects will play "
      "as the MissileProjectile enters or leaves water.\n\n"
      "@see particleEmitter\n");

   addField("projectileShapeName", TypeShapeFilename, Offset(projectileShapeName, MissileProjectileData),
      "@brief File path to the model of the MissileProjectile.\n\n");
   addField("scale", TypePoint3F, Offset(scale, MissileProjectileData),
      "@brief Scale to apply to the MissileProjectile's size.\n\n"
      "@note This is applied after SceneObject::scale\n");

   addField("sound", TypeSFXTrackName, Offset(sound, MissileProjectileData),
      "@brief SFXTrack datablock used to play sounds while in flight.\n\n");

   addField("explosion", TYPEID< ExplosionData >(), Offset(explosion, MissileProjectileData),
      "@brief Explosion datablock used when the MissileProjectile explodes outside of water.\n\n");
   addField("waterExplosion", TYPEID< ExplosionData >(), Offset(waterExplosion, MissileProjectileData),
      "@brief Explosion datablock used when the MissileProjectile explodes underwater.\n\n");

   addField("splash", TYPEID< SplashData >(), Offset(splash, MissileProjectileData),
      "@brief Splash datablock used to create splash effects as the MissileProjectile enters or leaves water\n\n");

   addField("decal", TYPEID< DecalData >(), Offset(decal, MissileProjectileData),
      "@brief Decal datablock used for decals placed at MissileProjectile explosion points.\n\n");

   addField("lightDesc", TYPEID< LightDescription >(), Offset(lightDesc, MissileProjectileData),
      "@brief LightDescription datablock used for lights attached to the MissileProjectile.\n\n");

   addField("velInheritFactor", TypeF32, Offset(velInheritFactor, MissileProjectileData),
      "@brief Amount of velocity the MissileProjectile recieves from the source that created it.\n\n"
      "Use an amount between 0 and 1 for the best effect. "
      "This value is never modified by the engine.\n"
      "@note This value by default is not transmitted between the server and the client.");
   addField("muzzleVelocity", TypeF32, Offset(muzzleVelocity, MissileProjectileData),
      "@brief Amount of velocity the MissileProjectile recieves from the \"muzzle\" of the gun.\n\n"
      "Used with velInheritFactor to determine the initial velocity of the MissileProjectile. "
      "This value is never modified by the engine.\n\n"
      "@note This value by default is not transmitted between the server and the client.\n\n"
      "@see velInheritFactor");

   addField("accuracy", TypeF32, Offset(accuracy, MissileProjectileData),
      "@brief How accurate the missile tracking is when moving towards a target.");
	addField("flareTypes", TypeString, Offset(flareTypeStr, MissileProjectileData),
		"@brief A tabbed field string containing a list of projectiles that act as missile flares to this projectile.");
	addField("flareDistance", TypeF32, Offset(flareDistance, MissileProjectileData));
	addField("terrainScanAhead", TypeF32, Offset(terrainScanAhead, MissileProjectileData));
	addField("terrainHeightFail", TypeF32, Offset(terrainHeightFail, MissileProjectileData));

   addField("impactForce", TypeF32, Offset(impactForce, MissileProjectileData));

   addProtectedField("lifetime", TypeS32, Offset(lifetime, MissileProjectileData), &setLifetime, &getScaledValue, 
      "@brief Amount of time, in milliseconds, before the MissileProjectile is removed from the simulation.\n\n"
      "Used with fadeDelay to determine the transparency of the MissileProjectile at a given time. "
      "A MissileProjectile may exist up to a maximum of 131040ms (or 4095 ticks) as defined by MissileProjectile::MaxLivingTicks in the source code."
      "@see fadeDelay");

   addProtectedField("armingDelay", TypeS32, Offset(armingDelay, MissileProjectileData), &setArmingDelay, &getScaledValue, 
      "@brief Amount of time, in milliseconds, before the MissileProjectile will cause damage or explode on impact.\n\n"
      "This value must be equal to or less than the MissileProjectile's lifetime.\n\n"
      "@see lifetime");
   addProtectedField("fadeDelay", TypeS32, Offset(fadeDelay, MissileProjectileData), &setFadeDelay, &getScaledValue,
      "@brief Amount of time, in milliseconds, before the MissileProjectile begins to fade out.\n\n"
      "This value must be smaller than the MissileProjectile's lifetime to have an affect.");

   Parent::initPersistFields();
}


//--------------------------------------------------------------------------
bool MissileProjectileData::onAdd() {
   if(!Parent::onAdd())
      return false;

   return true;
}


bool MissileProjectileData::preload(bool server, String &errorStr) {
   if (Parent::preload(server, errorStr) == false)
      return false;
      
   if( !server ) {
      if (!particleEmitter && particleEmitterId != 0)
         if (Sim::findObject(particleEmitterId, particleEmitter) == false)
            Con::errorf(ConsoleLogEntry::General, "MissileProjectileData::preload: Invalid packet, bad datablockId(particleEmitter): %d", particleEmitterId);

      if (!particleWaterEmitter && particleWaterEmitterId != 0)
         if (Sim::findObject(particleWaterEmitterId, particleWaterEmitter) == false)
            Con::errorf(ConsoleLogEntry::General, "MissileProjectileData::preload: Invalid packet, bad datablockId(particleWaterEmitter): %d", particleWaterEmitterId);

      if (!explosion && explosionId != 0)
         if (Sim::findObject(explosionId, explosion) == false)
            Con::errorf(ConsoleLogEntry::General, "MissileProjectileData::preload: Invalid packet, bad datablockId(explosion): %d", explosionId);

      if (!waterExplosion && waterExplosionId != 0)
         if (Sim::findObject(waterExplosionId, waterExplosion) == false)
            Con::errorf(ConsoleLogEntry::General, "MissileProjectileData::preload: Invalid packet, bad datablockId(waterExplosion): %d", waterExplosionId);

      if (!splash && splashId != 0)
         if (Sim::findObject(splashId, splash) == false)
            Con::errorf(ConsoleLogEntry::General, "MissileProjectileData::preload: Invalid packet, bad datablockId(splash): %d", splashId);

      if (!decal && decalId != 0)
         if (Sim::findObject(decalId, decal) == false)
            Con::errorf(ConsoleLogEntry::General, "MissileProjectileData::preload: Invalid packet, bad datablockId(decal): %d", decalId);

      String errorStr;
      if( !sfxResolve( &sound, errorStr ) )
         Con::errorf(ConsoleLogEntry::General, "MissileProjectileData::preload: Invalid packet: %s", errorStr.c_str());

      if (!lightDesc && lightDescId != 0)
         if (Sim::findObject(lightDescId, lightDesc) == false)
            Con::errorf(ConsoleLogEntry::General, "MissileProjectileData::preload: Invalid packet, bad datablockid(lightDesc): %d", lightDescId);   
   }

   if (projectileShapeName && projectileShapeName[0] != '\0') {
      projectileShape = ResourceManager::get().load(projectileShapeName);
      if (bool(projectileShape) == false) {
         errorStr = String::ToString("MissileProjectileData::load: Couldn't load shape \"%s\"", projectileShapeName);
         return false;
      }
      activateSeq = projectileShape->findSequence("activate");
      maintainSeq = projectileShape->findSequence("maintain");
   }

	if (bool(projectileShape)) { // create an instance to preload shape data
      TSShapeInstance* pDummy = new TSShapeInstance(projectileShape, !server);
      delete pDummy;
   }

   return true;
}

//--------------------------------------------------------------------------
void MissileProjectileData::packData(BitStream* stream) {
   Parent::packData(stream);

   stream->writeString(projectileShapeName);
   stream->writeFlag(faceViewer);
   if(stream->writeFlag(scale.x != 1 || scale.y != 1 || scale.z != 1)) {
      stream->write(scale.x);
      stream->write(scale.y);
      stream->write(scale.z);
   }

   if (stream->writeFlag(particleEmitter != NULL))
      stream->writeRangedU32(particleEmitter->getId(), DataBlockObjectIdFirst,
                                                   DataBlockObjectIdLast);

   if (stream->writeFlag(particleWaterEmitter != NULL))
      stream->writeRangedU32(particleWaterEmitter->getId(), DataBlockObjectIdFirst,
                                                   DataBlockObjectIdLast);

   if (stream->writeFlag(explosion != NULL))
      stream->writeRangedU32(explosion->getId(), DataBlockObjectIdFirst,
                                                 DataBlockObjectIdLast);

   if (stream->writeFlag(waterExplosion != NULL))
      stream->writeRangedU32(waterExplosion->getId(), DataBlockObjectIdFirst,
                                                      DataBlockObjectIdLast);

   if (stream->writeFlag(splash != NULL))
      stream->writeRangedU32(splash->getId(), DataBlockObjectIdFirst,
                                              DataBlockObjectIdLast);

   if (stream->writeFlag(decal != NULL))
      stream->writeRangedU32(decal->getId(), DataBlockObjectIdFirst,
                                              DataBlockObjectIdLast);

   sfxWrite( stream, sound );

   if ( stream->writeFlag(lightDesc != NULL))
      stream->writeRangedU32(lightDesc->getId(), DataBlockObjectIdFirst,
                                                 DataBlockObjectIdLast);

   stream->write(impactForce);
   stream->write(lifetime);
   stream->write(armingDelay);
   stream->write(fadeDelay);
	//Write Missile Params
	stream->write(accuracy);
	stream->write(flareTypeStr);
	stream->write(flareDistance);
	stream->write(terrainScanAhead);
	stream->write(terrainHeightFail);
}

void MissileProjectileData::unpackData(BitStream* stream) {
   Parent::unpackData(stream);

   projectileShapeName = stream->readSTString();

   faceViewer = stream->readFlag();
   if(stream->readFlag()) {
      stream->read(&scale.x);
      stream->read(&scale.y);
      stream->read(&scale.z);
   }
   else
      scale.set(1,1,1);

   if (stream->readFlag())
      particleEmitterId = stream->readRangedU32(DataBlockObjectIdFirst, DataBlockObjectIdLast);

   if (stream->readFlag())
      particleWaterEmitterId = stream->readRangedU32(DataBlockObjectIdFirst, DataBlockObjectIdLast);

   if (stream->readFlag())
      explosionId = stream->readRangedU32(DataBlockObjectIdFirst, DataBlockObjectIdLast);

   if (stream->readFlag())
      waterExplosionId = stream->readRangedU32(DataBlockObjectIdFirst, DataBlockObjectIdLast);
   
   if (stream->readFlag())
      splashId = stream->readRangedU32(DataBlockObjectIdFirst, DataBlockObjectIdLast);

   if (stream->readFlag())
      decalId = stream->readRangedU32(DataBlockObjectIdFirst, DataBlockObjectIdLast);
   
   sfxRead( stream, &sound );

   if (stream->readFlag())
      lightDescId = stream->readRangedU32(DataBlockObjectIdFirst, DataBlockObjectIdLast);

   stream->read(&impactForce);

   stream->read(&lifetime);
   stream->read(&armingDelay);
   stream->read(&fadeDelay);
	//Missile Params
	stream->read(&accuracy);
	stream->read(&flareTypeStr);
	stream->read(&flareDistance);
	stream->read(&terrainScanAhead);
	stream->read(&terrainHeightFail);
}

bool MissileProjectileData::setLifetime( void *obj, const char *index, const char *data ) {
	S32 value = dAtoi(data);
   value = scaleValue(value);
   
   MissileProjectileData *object = static_cast<MissileProjectileData*>(obj);
   object->lifetime = value;

   return false;
}

bool MissileProjectileData::setFadeDelay( void *obj, const char *index, const char *data ) {
	S32 value = dAtoi(data);
   value = scaleValue(value);

   MissileProjectileData *object = static_cast<MissileProjectileData*>(obj);
   object->fadeDelay = value;

   return false;
}

const char *MissileProjectileData::getScaledValue( void *obj, const char *data) {

	S32 value = dAtoi(data);
   value = scaleValue(value, false);

   String stringData = String::ToString(value);
   char *strBuffer = Con::getReturnBuffer(stringData.size());
   dMemcpy( strBuffer, stringData, stringData.size() );

   return strBuffer;
}

S32 MissileProjectileData::scaleValue( S32 value, bool down ) {
   S32 minV = 0;
   S32 maxV = MissileProjectile::MaxLivingTicks;
   
   // scale down to ticks before we validate
   if( down )
      value /= TickMs;
   
   if(value < minV || value > maxV) {
      Con::errorf("MissileProjectileData::scaleValue(S32 value = %d, bool down = %b) - Scaled value must be between %d and %d", value, down, minV, maxV);
		if(value < minV)
			value = minV;
		else if(value > maxV)
			value = maxV;
	}

   // scale up from ticks after we validate
   if( !down )
      value *= TickMs;

   return value;
}

//--------------------------------------------------------------------------
//--------------------------------------
//
MissileProjectile::MissileProjectile()
 : mPhysicsWorld( NULL ),
   mCurrPosition( 0, 0, 0 ),
   mCurrVelocity( 0, 0, 1 ),
   mSourceObjectId( -1 ),
   mSourceObjectSlot( -1 ),
   mCurrTick( 0 ),
   mParticleEmitter( NULL ),
   mParticleWaterEmitter( NULL ),
   mSound( NULL ),
   mProjectileShape( NULL ),
   mActivateThread( NULL ),
   mMaintainThread( NULL ),
   mHasExploded( false ),
   mFadeValue( 1.0f ) {
   // Todo: ScopeAlways?
   mNetFlags.set(Ghostable);
   mTypeMask |= ProjectileObjectType | LightObjectType | DynamicShapeObjectType;

	mTarget = NULL;
	mTargetID = -1;

	deflected = false;
	avoidingTerrain = false;

   mLight = LightManager::createLightInfo();
   mLight->setType( LightInfo::Point );   

   mLightState.clear();
   mLightState.setLightInfo( mLight );
}

MissileProjectile::~MissileProjectile() {
   SAFE_DELETE(mLight);

   delete mProjectileShape;
   mProjectileShape = NULL;
}
//

IMPLEMENT_CALLBACK( MissileProjectile, onMissileTarget, void, ( S32 targetID ), (), "");

//--------------------------------------------------------------------------
void MissileProjectile::initPersistFields() {
   addGroup("Physics");

   addProtectedField("initialPosition",  TypePoint3F, Offset(mInitialPosition, MissileProjectile), &_setInitialPosition, &defaultProtectedGetFn,
      "@brief Starting position for the MissileProjectile.\n\n");
   //addField("initialPosition",  TypePoint3F, Offset(mCurrPosition, MissileProjectile),
   //   "@brief Starting position for the MissileProjectile.\n\n");
   addProtectedField("initialVelocity", TypePoint3F, Offset(mInitialVelocity, MissileProjectile), &_setInitialVelocity, &defaultProtectedGetFn,
      "@brief Starting velocity for the MissileProjectile.\n\n");
   //addField("initialVelocity", TypePoint3F, Offset(mCurrVelocity, MissileProjectile),
   //   "@brief Starting velocity for the MissileProjectile.\n\n");

   endGroup("Physics");

   addGroup("Source");

   addField("sourceObject",     TypeS32,     Offset(mSourceObjectId, MissileProjectile),
      "@brief ID number of the object that fired the MissileProjectile.\n\n"
      "@note If the MissileProjectile was fired by a WeaponImage, sourceObject will be "
      "the object that owns the WeaponImage. This is usually the player.");
   addField("sourceSlot",       TypeS32,     Offset(mSourceObjectSlot, MissileProjectile),
      "@brief The sourceObject's weapon slot that the MissileProjectile originates from.\n\n");
   addField("target",       TypeS32,     Offset(mTargetID, MissileProjectile),
      "@brief The target the missile will track towards.\n\n");

   endGroup("Source");


   Parent::initPersistFields();
}

bool MissileProjectile::_setInitialPosition( void *object, const char *index, const char *data ) {
   MissileProjectile* p = static_cast<MissileProjectile*>( object );
   if ( p ) {
	   Point3F pos;

	   S32 count = dSscanf( data, "%f %f %f", 
		   &pos.x, &pos.y, &pos.z);
   	
	   if ( (count != 3) ) {
         Con::printf("MissileProjectile: Failed to parse initial position \"px py pz\" from '%s'", data);
         return false;
      }

      p->setInitialPosition( pos );
   }
   return false;
}

void MissileProjectile::setInitialPosition( const Point3F& pos ) {
   mInitialPosition = pos;
   mCurrPosition = pos;
}

bool MissileProjectile::_setInitialVelocity( void *object, const char *index, const char *data ) {
   MissileProjectile* p = static_cast<MissileProjectile*>( object );
   if ( p ) {
	   Point3F vel;

	   S32 count = dSscanf( data, "%f %f %f", 
		   &vel.x, &vel.y, &vel.z);
   	
	   if ( (count != 3) ) {
         Con::printf("MissileProjectile: Failed to parse initial velocity \"vx vy vz\" from '%s'", data);
         return false;
      }

      p->setInitialVelocity( vel );
   }
   return false;
}

void MissileProjectile::setInitialVelocity( const Point3F& vel ) {
   mInitialVelocity = vel;
   mCurrVelocity = vel;
}

//--------------------------------------------------------------------------

bool MissileProjectile::calculateImpact(float,
                                 Point3F& pointOfImpact,
											float&   impactTime) {
   Con::warnf(ConsoleLogEntry::General, "MissileProjectile::calculateImpact: Should never be called");

   impactTime = 0;
   pointOfImpact.set(0, 0, 0);
   return false;
}


//--------------------------------------------------------------------------
F32 MissileProjectile::getUpdatePriority(CameraScopeQuery *camInfo, U32 updateMask, S32 updateSkips) {
   F32 ret = Parent::getUpdatePriority(camInfo, updateMask, updateSkips);
   // if the camera "owns" this object, it should have a slightly higher priority
   if(mSourceObject == camInfo->camera)
      return ret + 0.2;
   return ret;
}

void MissileProjectile::setMissileTarget(S32 targetID) {
	if(targetID) {
		if(Sim::findObject(targetID) != NULL) {
			mTarget = dynamic_cast<ShapeBase*>(Sim::findObject(targetID));
			//Send a missile alert callback.
			onMissileTarget_callback(targetID);
			return;
		}
	}
	mTarget = NULL;
}

bool MissileProjectile::onAdd() {
   if(!Parent::onAdd())
      return false;

   if (isServerObject()) {
      ShapeBase* ptr;
      if (Sim::findObject(mSourceObjectId, ptr)) {
         mSourceObject = ptr;

         // Since we later do processAfter( mSourceObject ) we must clearProcessAfter
         // if it is deleted. SceneObject already handles this in onDeleteNotify so
         // all we need to do is register for the notification.
         deleteNotify( ptr );
      }
      else {
         if (mSourceObjectId != -1)
            Con::errorf(ConsoleLogEntry::General, "MissileProjectile::onAdd: mSourceObjectId is invalid");
         mSourceObject = NULL;
      }

      // If we're on the server, we need to inherit some of our parent's velocity
      //
      mCurrTick = 0;
   }
   else {
      if (bool(mDataBlock->projectileShape)) {
         mProjectileShape = new TSShapeInstance(mDataBlock->projectileShape, isClientObject());

         if (mDataBlock->activateSeq != -1) {
            mActivateThread = mProjectileShape->addThread();
            mProjectileShape->setTimeScale(mActivateThread, 1);
            mProjectileShape->setSequence(mActivateThread, mDataBlock->activateSeq, 0);
         }
      }
      if (mDataBlock->particleEmitter != NULL) {
         ParticleEmitter* pEmitter = new ParticleEmitter;
         pEmitter->onNewDataBlock(mDataBlock->particleEmitter,false);
         if (pEmitter->registerObject() == false) {
            Con::warnf(ConsoleLogEntry::General, "Could not register particle emitter for particle of class: %s", mDataBlock->getName());
            delete pEmitter;
            pEmitter = NULL;
         }
         mParticleEmitter = pEmitter;
      }

      if (mDataBlock->particleWaterEmitter != NULL) {
         ParticleEmitter* pEmitter = new ParticleEmitter;
         pEmitter->onNewDataBlock(mDataBlock->particleWaterEmitter,false);
         if (pEmitter->registerObject() == false) {
            Con::warnf(ConsoleLogEntry::General, "Could not register particle emitter for particle of class: %s", mDataBlock->getName());
            delete pEmitter;
            pEmitter = NULL;
         }
         mParticleWaterEmitter = pEmitter;
      }
   }
   if (mSourceObject.isValid())
      processAfter(mSourceObject);

	//Is there a pre-defined target?
	if(mTargetID) {
		if(Sim::findObject(mTargetID) != NULL) {
			mTarget = dynamic_cast<ShapeBase*>(Sim::findObject(mTargetID));
		}
	}

   // Setup our bounding box
   if (bool(mDataBlock->projectileShape) == true)
      mObjBox = mDataBlock->projectileShape->bounds;
   else
      mObjBox = Box3F(Point3F(0, 0, 0), Point3F(0, 0, 0));

   MatrixF initialTransform( true );
   initialTransform.setPosition( mCurrPosition );
   setTransform( initialTransform );   // calls resetWorldBox

   addToScene();

   if ( PHYSICSMGR )
      mPhysicsWorld = PHYSICSMGR->getWorld( isServerObject() ? "server" : "client" );

   return true;
}


void MissileProjectile::onRemove() {
   if( !mParticleEmitter.isNull() ) {
      mParticleEmitter->deleteWhenEmpty();
      mParticleEmitter = NULL;
   }

   if( !mParticleWaterEmitter.isNull() ) {
      mParticleWaterEmitter->deleteWhenEmpty();
      mParticleWaterEmitter = NULL;
   }

   SFX_DELETE( mSound );

   removeFromScene();
   Parent::onRemove();
}


bool MissileProjectile::onNewDataBlock( GameBaseData *dptr, bool reload ) {
   mDataBlock = dynamic_cast<MissileProjectileData*>( dptr );
   if ( !mDataBlock || !Parent::onNewDataBlock( dptr, reload ) )
      return false;

   if ( isGhost() ) {
      // Create the sound ahead of time.  This reduces runtime
      // costs and makes the system easier to understand.

      SFX_DELETE( mSound );

      if ( mDataBlock->sound )
         mSound = SFX->createSource( mDataBlock->sound );
   }

   return true;
}

void MissileProjectile::submitLights( LightManager *lm, bool staticLighting ) {
   if ( staticLighting || mHasExploded || !mDataBlock->lightDesc )
      return;
   
   mDataBlock->lightDesc->submitLight( &mLightState, getRenderTransform(), lm, this );   
}

bool MissileProjectile::pointInWater(const Point3F &point) {   
   // This is pretty much a hack so we can use the existing ContainerQueryInfo
   // and findObject router.
   
   // We only care if we intersect with water at all 
   // so build a box at the point that has only 1 z extent.
   // And test if water coverage is anything other than zero.

   Box3F boundsBox( point, point );
   boundsBox.maxExtents.z += 1.0f;

   ContainerQueryInfo info;
   info.box = boundsBox;
   info.mass = 0.0f;
   
   // Find and retreive physics info from intersecting WaterObject(s)
   if(mContainer != NULL) {
      mContainer->findObjects( boundsBox, WaterObjectType, findRouter, &info );
   }
   else {
      AssertFatal(isClientObject(), "Server MissileProjectile has not been properly added");
      gClientContainer.findObjects( boundsBox, WaterObjectType, findRouter, &info );
   }

   return ( info.waterCoverage > 0.0f );
}

//----------------------------------------------------------------------------

void MissileProjectile::emitParticles(const Point3F& from, const Point3F& to, const Point3F& vel, const U32 ms) {
   if ( mHasExploded )
      return;

   Point3F axis = -vel;

   if( axis.isZero() )
      axis.set( 0.0, 0.0, 1.0 );
   else
      axis.normalize();

   bool fromWater = pointInWater(from);
   bool toWater   = pointInWater(to);

   if (!fromWater && !toWater && bool(mParticleEmitter))                                        // not in water
      mParticleEmitter->emitParticles(from, to, axis, vel, ms);
   else if (fromWater && toWater && bool(mParticleWaterEmitter))                                // in water
      mParticleWaterEmitter->emitParticles(from, to, axis, vel, ms);
   else if (!fromWater && toWater && mDataBlock->splash) {
      // cast the ray to get the surface point of the water
      RayInfo rInfo;
      if (gClientContainer.castRay(from, to, WaterObjectType, &rInfo)) {
         MatrixF trans = getTransform();
         trans.setPosition(rInfo.point);

         Splash *splash = new Splash();
         splash->onNewDataBlock(mDataBlock->splash, false);
         splash->setTransform(trans);
         splash->setInitialState(trans.getPosition(), Point3F(0.0, 0.0, 1.0));
         if (!splash->registerObject()) {
            delete splash;
            splash = NULL;
         }

         // create an emitter for the particles out of water and the particles in water
         if (mParticleEmitter)
            mParticleEmitter->emitParticles(from, rInfo.point, axis, vel, ms);

         if (mParticleWaterEmitter)
            mParticleWaterEmitter->emitParticles(rInfo.point, to, axis, vel, ms);
      }
   }
	else if (fromWater && !toWater && mDataBlock->splash) {     // leaving water
      // cast the ray in the opposite direction since that point is out of the water, otherwise
      //  we hit water immediately and wont get the appropriate surface point
      RayInfo rInfo;
      if (gClientContainer.castRay(to, from, WaterObjectType, &rInfo)) {
         MatrixF trans = getTransform();
         trans.setPosition(rInfo.point);

         Splash *splash = new Splash();
         splash->onNewDataBlock(mDataBlock->splash,false);
         splash->setTransform(trans);
         splash->setInitialState(trans.getPosition(), Point3F(0.0, 0.0, 1.0));
         if (!splash->registerObject()) {
            delete splash;
            splash = NULL;
         }

         // create an emitter for the particles out of water and the particles in water
         if (mParticleEmitter)
            mParticleEmitter->emitParticles(rInfo.point, to, axis, vel, ms);

         if (mParticleWaterEmitter)
            mParticleWaterEmitter->emitParticles(from, rInfo.point, axis, vel, ms);
      }
   }
}

void MissileProjectile::explode( const Point3F &p, const Point3F &n, const U32 collideType ) {
   // Make sure we don't explode twice...
   if ( mHasExploded )
      return;

   mHasExploded = true;

   // Move the explosion point slightly off the surface to avoid problems with radius damage
   Point3F explodePos = p + n * 0.001f;

   if ( isServerObject() ) {
      // Do what the server needs to do, damage the surrounding objects, etc.
      mExplosionPosition = explodePos;
      mExplosionNormal = n;
      mCollideHitType  = collideType;

	   mDataBlock->onExplode_callback( this, mExplosionPosition, mFadeValue );

      setMaskBits(ExplosionMask);

      // Just wait till the timeout to self delete. This 
      // gives server object time to get ghosted to the client.
   } 
   else {
      // Client just plays the explosion at the right place...
      //       
      Explosion* pExplosion = NULL;

      if (mDataBlock->waterExplosion && pointInWater(p)) {
         pExplosion = new Explosion;
         pExplosion->onNewDataBlock(mDataBlock->waterExplosion, false);
      }
      else
      if (mDataBlock->explosion) {
         pExplosion = new Explosion;
         pExplosion->onNewDataBlock(mDataBlock->explosion, false);
      }

      if( pExplosion ) {
         MatrixF xform(true);
         xform.setPosition(explodePos);
         pExplosion->setTransform(xform);
         pExplosion->setInitialState(explodePos, n);
         pExplosion->setCollideType( collideType );
         if (pExplosion->registerObject() == false) {
            Con::errorf(ConsoleLogEntry::General, "MissileProjectile(%s)::explode: couldn't register explosion",
                        mDataBlock->getName() );
            delete pExplosion;
            pExplosion = NULL;
         }
      }

      // Client (impact) decal.
      if ( mDataBlock->decal )     
         gDecalManager->addDecal( p, n, 0.0f, mDataBlock->decal );

      // Client object
      updateSound();
   }
}

void MissileProjectile::updateSound() {
   if (!mDataBlock->sound)
      return;

   if ( mSound ) {
      if ( mHasExploded )
         mSound->stop();
      else {
         if ( !mSound->isPlaying() )
            mSound->play();

         mSound->setVelocity( getVelocity() );
         mSound->setTransform( getRenderTransform() );
      }
   }
}

void MissileProjectile::processTick( const Move *move ) {
   Parent::processTick( move );
   mCurrTick++;

   simulate( TickSec );
}

void MissileProjectile::simulate( F32 dt ) {         
   if ( isServerObject() && mCurrTick >= mDataBlock->lifetime ) {
      deleteObject();
      return;
   }
   
   if ( mHasExploded )
      return;

   // ... otherwise, we have to do some simulation work.
   RayInfo rInfo;
   Point3F oldPosition;
   Point3F newPosition;

   oldPosition = mCurrPosition;
	if(mTarget != NULL) {
		//Set proper tracking mask for client/server update
		setMaskBits(SeekingMask);
		//Pre-Defs
		F32 pSpeed;
		VectorF tVec, tPos, cP, eP;
		//If we're seeking a target, then flares can deflect us.
		if(!deflected) {
			SceneObject *test = NULL;
		   gServerContainer.initRadiusSearch(getPosition(), mDataBlock->flareDistance, ProjectileObjectType);
		   while((test = gServerContainer.containerSearchNextObject()) != NULL) {
			   if(test != this) { 
	    	      for(S32 i = 0; i < StringUnit::getUnitCount(mDataBlock->flareTypeStr, "\t\n"); i++) {
					   if(dStrcmp(dynamic_cast<Projectile *>(test)->getDataBlock()->getName(), 
						   StringUnit::getUnit(mDataBlock->flareTypeStr, i, "\t\n")) == 0) {
						      //We've got a flare, and are now deflected.
							   deflected = true;
								setMissileTarget(test->getId());
								break;
					   }
				   }
					if(deflected) {
						break;
					}
			   }
		   }
		}
		//Terrain Avoidance, there are a few different forms of terrain avoidance we use, but they all rely on if the
		// missile can hit the target it's going for or not, if we already have LOS to the target object, then we're 
		// fine and can keep moving on, but if something is in the way, then we'll dodge it.
		cP = mCurrPosition;
		eP = mTarget->getPosition();
		tPos = eP - cP;
		tPos.normalizeSafe();
		//We can only go so far along the path.
		eP = tPos * mDataBlock->terrainScanAhead;
		disableCollision();
		RayInfo rinfo;
		if(gServerContainer.castRay(cP, eP, TerrainObjectType, &rinfo)) {
			//Terrain in the way.
			eP.set(rinfo.point); //Set the terrain position hit at.
			avoidingTerrain = true;
		}
		else {
		   avoidingTerrain = false;
		}
		enableCollision();
		if(avoidingTerrain) {
			//To avoid terrain, we can simply calculate the path between the colision point and the height fail variable.
			tPos.set(eP.x, eP.y, eP.z + mDataBlock->terrainHeightFail);
			//Essentially what this does is once we see terrain ahead the missile will begin to bank upwards until it can get "over" the
			// terrain.
		}
		else {
		   //Get the vectors, correct position for center hit (Thanks to Derk Adams)
		   tPos = mTarget->getPosition();
		   tPos.z += (mTarget->getObjBox().len_z()/2);  
		   //
		}
		pSpeed = mCurrVelocity.len(); //<-- Store here since we need to normalize this vector
		tVec = tPos - mCurrPosition; //Vector Subtraction to determine the direction we need to go.
		tVec.normalize();
		mCurrVelocity.normalize();
		//Since the vector is normalized, we can now add in the factor of accuracy to the missile, the higher this value, basically
		//the quicker the "vector" reaches it's destination, or basically, it makes it harder to dodge evil missiles.
		tVec *= mDataBlock->accuracy;
		mCurrVelocity *= (100 - mDataBlock->accuracy);
		tVec += mCurrVelocity;
		//Normalize and multiply for final result.
		tVec.normalize();
		tVec *= pSpeed;
		mCurrVelocity = tVec;
	}

   newPosition = oldPosition + mCurrVelocity * dt;

   // disable the source objects collision reponse for a short time while we
   // determine if the MissileProjectile is capable of moving from the old position
   // to the new position, otherwise we'll hit ourself
   bool disableSourceObjCollision = (mSourceObject.isValid() && mCurrTick <= SourceIdTimeoutTicks);
   if ( disableSourceObjCollision )
      mSourceObject->disableCollision();
   disableCollision();

   // Determine if the MissileProjectile is going to hit any object between the previous
   // position and the new position. This code is executed both on the server
   // and on the client (for prediction purposes). It is possible that the server
   // will have registered a collision while the client prediction has not. If this
   // happens the client will be corrected in the next packet update.

   // Raycast the abstract PhysicsWorld if a PhysicsPlugin exists.
   bool hit = false;

   if ( mPhysicsWorld )
      hit = mPhysicsWorld->castRay( oldPosition, newPosition, &rInfo, Point3F( newPosition - oldPosition) * mDataBlock->impactForce );            
   else 
      hit = getContainer()->castRay(oldPosition, newPosition, csmDynamicCollisionMask | csmStaticCollisionMask, &rInfo);

   if ( hit ) {
      MatrixF xform( true );
      xform.setColumn( 3, rInfo.point );
      setTransform( xform );
      mCurrPosition    = rInfo.point;
      mCurrVelocity    = Point3F::Zero;

      // Get the object type before the onCollision call, in case
      // the object is destroyed.
      U32 objectType = rInfo.object->getTypeMask();

      // re-enable the collision response on the source object since
      // we need to process the onCollision and explode calls
      if ( disableSourceObjCollision )
         mSourceObject->enableCollision();

      onCollision( rInfo.point, rInfo.normal, rInfo.object );
      explode( rInfo.point, rInfo.normal, objectType );

      // break out of the collision check, since we've exploded
      // we don't want to mess with the position and velocity
   }

   // re-enable the collision response on the source object now
   // that we are done processing the ballistic movement
   if ( disableSourceObjCollision )
      mSourceObject->enableCollision();
   enableCollision();

   if ( isClientObject() ) {
      emitParticles( mCurrPosition, newPosition, mCurrVelocity, U32( dt * 1000.0f ) );
      updateSound();
   }

   mCurrDeltaBase = newPosition;
   mCurrBackDelta = mCurrPosition - newPosition;
   mCurrPosition = newPosition;

   MatrixF xform( true );
   xform.setColumn( 3, mCurrPosition );
   setTransform( xform );
}


void MissileProjectile::advanceTime(F32 dt) {
   Parent::advanceTime(dt);

   if ( mHasExploded || dt == 0.0)
      return;

   if (mActivateThread &&
         mProjectileShape->getDuration(mActivateThread) > mProjectileShape->getTime(mActivateThread) + dt) {
      mProjectileShape->advanceTime(dt, mActivateThread);
   }
   else {
      if (mMaintainThread) {
         mProjectileShape->advanceTime(dt, mMaintainThread);
      }
      else if (mActivateThread && mDataBlock->maintainSeq != -1) {
         mMaintainThread = mProjectileShape->addThread();
         mProjectileShape->setTimeScale(mMaintainThread, 1);
         mProjectileShape->setSequence(mMaintainThread, mDataBlock->maintainSeq, 0);
         mProjectileShape->advanceTime(dt, mMaintainThread);
      }
   }
}

void MissileProjectile::interpolateTick(F32 delta) {
   Parent::interpolateTick(delta);

   if( mHasExploded )
      return;

   Point3F interpPos = mCurrDeltaBase + mCurrBackDelta * delta;
   Point3F dir = mCurrVelocity;
   if(dir.isZero())
      dir.set(0,0,1);
   else
      dir.normalize();

   MatrixF xform(true);
	xform = MathUtils::createOrientFromDir(dir);
   xform.setPosition(interpPos);
   setRenderTransform(xform);

   // fade out the MissileProjectile image
   S32 time = (S32)(mCurrTick - delta);
   if(time > mDataBlock->fadeDelay) {
      F32 fade = F32(time - mDataBlock->fadeDelay);
      mFadeValue = 1.0 - (fade / F32(mDataBlock->lifetime));
   }
   else
      mFadeValue = 1.0;

   updateSound();
}



//--------------------------------------------------------------------------
void MissileProjectile::onCollision(const Point3F& hitPosition, const Point3F& hitNormal, SceneObject* hitObject) {
   // No client specific code should be placed or branched from this function
   if(isClientObject())
      return;

   if (hitObject != NULL && isServerObject()) {
	   mDataBlock->onCollision_callback( this, hitObject, mFadeValue, hitPosition, hitNormal );
   }
}

//--------------------------------------------------------------------------
U32 MissileProjectile::packUpdate( NetConnection *con, U32 mask, BitStream *stream ) {
   U32 retMask = Parent::packUpdate( con, mask, stream );

   const bool isInitalUpdate = mask & GameBase::InitialUpdateMask;

   // InitialUpdateMask
   if ( stream->writeFlag( isInitalUpdate ) ) {
      stream->writeRangedU32( mCurrTick, 0, MaxLivingTicks );

      if ( mSourceObject.isValid() ) {
         // Potentially have to write this to the client, let's make sure it has a
         //  ghost on the other side...
         S32 ghostIndex = con->getGhostIndex( mSourceObject );
         if ( stream->writeFlag( ghostIndex != -1 ) ) {
            stream->writeRangedU32( U32(ghostIndex), 
                                    0, 
                                    NetConnection::MaxGhostCount );

            stream->writeRangedU32( U32(mSourceObjectSlot),
                                    0, 
                                    ShapeBase::MaxMountedImages - 1 );
         }
         else 
            // have not recieved the ghost for the source object yet, try again later
            retMask |= GameBase::InitialUpdateMask;
      }
      else
         stream->writeFlag( false );
   }

   // ExplosionMask
   //
   // ExplosionMask will be set during the initial update but hidden is
   // only true if we have really exploded.
   if ( stream->writeFlag( ( mask & ExplosionMask ) && mHasExploded ) ) {
      mathWrite(*stream, mExplosionPosition);
      mathWrite(*stream, mExplosionNormal);
      stream->write(mCollideHitType);
   }

	// SeekerMask
	// SeekerMask is set when the missile has a active target and needs more rapid updates of position/velocity
	if( stream->writeFlag( ( mask & SeekingMask) ) ) {
		mathWrite(*stream, mCurrPosition);
		mathWrite(*stream, mCurrVelocity);
		stream->writeFlag(deflected);
		stream->writeFlag(avoidingTerrain);
	}

   return retMask;
}

void MissileProjectile::unpackUpdate(NetConnection* con, BitStream* stream) {
   Parent::unpackUpdate(con, stream);
   
   if ( stream->readFlag() ) {
      mCurrTick = stream->readRangedU32( 0, MaxLivingTicks );
      if ( stream->readFlag() ) {
         mSourceObjectId   = stream->readRangedU32( 0, NetConnection::MaxGhostCount );
         mSourceObjectSlot = stream->readRangedU32( 0, ShapeBase::MaxMountedImages - 1 );

         NetObject* pObject = con->resolveGhost( mSourceObjectId );
         if ( pObject != NULL )
            mSourceObject = dynamic_cast<ShapeBase*>( pObject );
      }
      else {
         mSourceObjectId   = -1;
         mSourceObjectSlot = -1;
         mSourceObject     = NULL;
      }
   }
   
   if ( stream->readFlag() ) {
      Point3F explodePoint;
      Point3F explodeNormal;
      mathRead( *stream, &explodePoint );
      mathRead( *stream, &explodeNormal );
      stream->read( &mCollideHitType );

      // start the explosion visuals
      explode( explodePoint, explodeNormal, mCollideHitType );
   }

	if( stream->readFlag() ) {
		mathRead( *stream, &mCurrPosition);
		mathRead( *stream, &mCurrVelocity);

		deflected = stream->readFlag();
		avoidingTerrain = stream->readFlag();
	}
}

//--------------------------------------------------------------------------
void MissileProjectile::prepRenderImage( SceneRenderState* state ) {
   if (mHasExploded || mFadeValue <= (1.0/255.0))
      return;

   if ( mDataBlock->lightDesc ) {
      mDataBlock->lightDesc->prepRender( state, &mLightState, getRenderTransform() );
   }

   prepBatchRender( state );
}

void MissileProjectile::prepBatchRender( SceneRenderState *state ) {
   if ( !mProjectileShape )
      return;

   GFXTransformSaver saver;

   // Set up our TS render state.
   TSRenderState rdata;
   rdata.setSceneState( state );

   // We might have some forward lit materials
   // so pass down a query to gather lights.
   LightQuery query;
   query.init( getWorldSphere() );
   rdata.setLightQuery( &query );

   MatrixF mat = getRenderTransform();
   mat.scale( mObjScale );
   mat.scale( mDataBlock->scale );
   GFX->setWorldMatrix( mat );

   mProjectileShape->setDetailFromPosAndScale( state, mat.getPosition(), mObjScale );
   mProjectileShape->animate();

   mProjectileShape->render( rdata );
}

DefineEngineMethod(MissileProjectile, presimulate, void, (F32 seconds), (1.0f), 
                                       "@brief Updates the MissileProjectile's positional and collision information.\n\n"
                                       "This function will first delete the MissileProjectile if it is a server object and is outside it's MissileProjectileData::lifetime. "
                                       "Also responsible for applying gravity, determining collisions, triggering explosions, "
                                       "emitting trail particles, and calculating bounces if necessary."
									            "@param seconds Amount of time, in seconds since the simulation's start, to advance.\n"
									            "@tsexample\n"
									               "// Tell the MissileProjectile to process a simulation event, and provide the amount of time\n"
										            "// that has passed since the simulation began.\n"
										            "%seconds = 2.0;\n"
										            "%MissileProjectile.presimulate(%seconds);\n"
									            "@endtsexample\n"
                                       "@note This function is not called if the SimObject::hidden is true.") {
	object->simulate( seconds );
}

DefineEngineMethod(MissileProjectile, setTarget, void, (S32 targetID),, "(S32) set's the active missile target.") {
	object->setMissileTarget(targetID);
}

DefineEngineMethod(MissileProjectile, getTarget, S32,(),, "(void) return the active missile object") {
	if(object->getMissileTarget() != NULL) {
		return object->getId();
	}
	return 0;
}