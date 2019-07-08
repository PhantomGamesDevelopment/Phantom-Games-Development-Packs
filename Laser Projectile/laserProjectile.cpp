/*
   LaserProjectile.cpp
	By: Robert C. Fritzen
	Adapted from the TGE/TGEA Laser Resource For T3D MIT 3.5
*/

#include "platform/platform.h"
#include "laserProjectile.h"

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
#include "gfx/primBuilder.h"
#include "T3D/containerQuery.h"
#include "T3D/decal/decalManager.h"
#include "T3D/decal/decalData.h"
#include "T3D/lightDescription.h"
#include "console/engineAPI.h"

IMPLEMENT_CO_DATABLOCK_V1(LaserProjectileData);
IMPLEMENT_CO_NETOBJECT_V1(LaserProjectile);

LaserProjectileData::LaserProjectileData() {
   ProjectileData::ProjectileData();

   beamStartRadius = 0.0;
   beamEndRadius = 0.0;
   interval = 0;
	fadeBegin = 0;
	range = 0;
	conHitBeam = false;
	conMoveBeam = false;
	mBeamColor.set(1.0f, 0, 0);

   dMemset( materialListNames, 0, sizeof( materialListNames ) );
   dMemset( materialList, 0, sizeof( materialList ) );
}

//--------------------------------------------------------------------------

void LaserProjectileData::initPersistFields() {
   Parent::initPersistFields();

   addField("beamStartRadius", TypeF32, Offset(beamStartRadius, LaserProjectileData),
      "@brief the start radius of the laser beam");
   addField("beamEndRadius", TypeF32, Offset(beamEndRadius, LaserProjectileData),
      "@brief the end radius of the laser beam");
   addField( "materialList", TypeString, Offset(materialListNames, LaserProjectileData), maxTextures,
      "List of textures to use to render laser beam." );
   addField("range", TypeF32, Offset(range, LaserProjectileData),
      "@brief the maximum laser beam range");
   addField("interval", TypeS32, Offset(interval, LaserProjectileData),
      "@brief the interval of texture change on the laser");
	addField("conHitBeam", TypeBool, Offset(conHitBeam, LaserProjectileData),
		"@brief controls if the beam only hits once (false), or continues as long as rendering (true)");
	addField("conMoveBeam", TypeBool, Offset(conMoveBeam, LaserProjectileData),
		"@brief controls if the beam is allowed to keep moving as long as it remains active");
	addField("beamColor", TypeColorF, Offset(mBeamColor, LaserProjectileData), 
		"@brief controls the render color of the laser beam");
   addField("fadeBegin", TypeS32, Offset(fadeBegin, LaserProjectileData),
      "@brief Time (in MS) from the deletion of the beam that fading begins");
}

//--------------------------------------------------------------------------
void LaserProjectileData::packData(BitStream* stream) {
   Parent::packData(stream);

	stream->write(beamStartRadius);
	stream->write(beamEndRadius);
	stream->write(interval);
	stream->write(range);
	stream->write(conHitBeam);
	stream->write(conMoveBeam);

	stream->write(fadeBegin);

	stream->write(mBeamColor.red);
	stream->write(mBeamColor.green);
	stream->write(mBeamColor.blue);

   for (S32 i = 0; i < maxTextures; i++) {
      stream->writeString(materialListNames[i]);
   }
}

void LaserProjectileData::unpackData(BitStream* stream) {
   Parent::unpackData(stream);

   stream->read(&beamStartRadius);
	stream->read(&beamEndRadius);
	stream->read(&interval);
	stream->read(&range);
	stream->read(&conHitBeam);
	stream->read(&conMoveBeam);

	stream->read(&fadeBegin);

	stream->read(&mBeamColor.red);
	stream->read(&mBeamColor.green);
	stream->read(&mBeamColor.blue);

   for (S32 i = 0; i < maxTextures; i++) {
      materialListNames[i] = stream->readSTString();
   }
}

bool LaserProjectileData::preload(bool server, String &errorStr) {
   if (Parent::preload(server, errorStr) == false)
      return false;

	if(!server) {
      for (U32 i = 0; i < maxTextures; i++) {
         if (materialListNames[i][0])
            materialList[i] = GFXTexHandle(materialListNames[i], &GFXDefaultStaticDiffuseProfile, avar("%s() - materialListNames[%d] (line %d)", __FUNCTION__, i, __LINE__));
      }		
	}
	return true;
}
//--------------------------------------------------------------------------
LaserProjectile::LaserProjectile() {
   Projectile::Projectile();

   mLastTime = 0;
   mIndex = 0;

	fadeAlpha = 1.0f;

	mHitPosition.set(0, 0, 0);

   alreadyHit = false;
}

LaserProjectile::~LaserProjectile() {
}

//--------------------------------------------------------------------------
void LaserProjectile::initPersistFields() {
   Parent::initPersistFields();
}

//--------------------------------------------------------------------------
U32 LaserProjectile::packUpdate(NetConnection* con, U32 mask, BitStream* stream) {
   U32 retMask = Parent::packUpdate(con, mask, stream);

	stream->writeInt(mLastTime, 8);
	stream->writeInt(mIndex, 8);
	stream->writeFlag(alreadyHit);

	stream->writeFloat(fadeAlpha, 8);

	mathWrite(*stream, mCurrPosition);
	mathWrite(*stream, mCurrVelocity);

   return retMask;
}

void LaserProjectile::unpackUpdate(NetConnection* con, BitStream* stream) {
   Parent::unpackUpdate(con, stream);

	mLastTime = stream->readInt(8);
	mIndex = stream->readInt(8);
	alreadyHit = stream->readFlag();

	fadeAlpha = stream->readFloat(8);

	mathRead( *stream, &mCurrPosition);
	mathRead( *stream, &mCurrVelocity);
}

//--------------------------------------------------------------------------
bool LaserProjectile::onAdd() {
	if(!Parent::onAdd()) { 
		return false;
	}
      
   return true;
}

bool LaserProjectile::onNewDataBlock(GameBaseData* dptr, bool reload) {
   mDataBlock = dynamic_cast<LaserProjectileData*>(dptr);
	if (!mDataBlock || !Parent::onNewDataBlock(dptr, reload)) {
      return false;
	}
   return true;
}

//--------------------------------------------------------------------------
void LaserProjectile::processTick(const Move* move) {

   mCurrTick++;

   if (isServerObject() && mCurrTick >= mDataBlock->lifetime) {
      deleteObject();
      return;
   }

	EaseF timeEase;
	if(mCurrTick >= (mDataBlock->lifetime - mDataBlock->fadeBegin)) {
		//Begin Fade
		fadeAlpha = timeEase.getValue( mCurrTick, 0.f, 255.f, mDataBlock->lifetime );
	}

	bool disableSourceObjCollision = (mSourceObject.isValid() && mCurrTick <= SourceIdTimeoutTicks);
	if ( disableSourceObjCollision ) {
		mSourceObject->disableCollision();
	}
   disableCollision();

	if(mDataBlock->conMoveBeam) {
      if(mSourceObject.isValid()) {
         Point3F newPosition, mMuzzlePosition, mMuzzleVector;
         MatrixF muzzleTrans;
         mSourceObject->getMuzzleTransform(mSourceObjectSlot, &muzzleTrans);
		   muzzleTrans.getColumn(3, &mMuzzlePosition);
         muzzleTrans.getColumn(1, &mMuzzleVector);
 
		   newPosition = mMuzzleVector * mDataBlock->range;
         newPosition += mMuzzlePosition;
			mHitPosition = newPosition;

		   if(!mDataBlock->conHitBeam && !alreadyHit) {
            RayInfo rInfo;
            if (gServerContainer.castRay(mMuzzlePosition, newPosition, csmDynamicCollisionMask | csmStaticCollisionMask, &rInfo)) {
			      if(!mDataBlock->conHitBeam) {
			         alreadyHit = true;
			      }

               MatrixF xform1(true);
               xform1.setColumn(3, rInfo.point);
               setTransform(xform1);

					if ( disableSourceObjCollision ) {
                  mSourceObject->enableCollision();
					}
               onCollision(rInfo.point, rInfo.normal, rInfo.object);
					mHitPosition = rInfo.point;
            }
		   }
			
			if ( disableSourceObjCollision ) {
				mSourceObject->enableCollision();
			}
			enableCollision();

         if(isClientObject()) {
            emitParticles(mCurrPosition, newPosition, Point3F(1, 1, 1), TickMs * 4);
            updateSound();
         }

         mCurrDeltaBase = newPosition;
         mCurrBackDelta = mCurrPosition - newPosition;
         mCurrPosition = mMuzzlePosition;

         MatrixF xform(true);
         xform.setColumn(3, mCurrPosition);
         setTransform(xform);
		}
		else {
			//No source, so it must be the "first" render instance.
			Point3F newPos = mCurrVelocity * mDataBlock->range;
			newPos += mCurrPosition;

			mHitPosition = newPos;
		   if(!mDataBlock->conHitBeam && !alreadyHit) {
            RayInfo rInfo;
            if (gServerContainer.castRay(mCurrPosition, newPos, csmDynamicCollisionMask | csmStaticCollisionMask, &rInfo)) {
			      if(!mDataBlock->conHitBeam) {
			         alreadyHit = true;
			      }

               MatrixF xform1(true);
               xform1.setColumn(3, rInfo.point);
               setTransform(xform1);

					if ( disableSourceObjCollision ) {
                  mSourceObject->enableCollision();
					}
               onCollision(rInfo.point, rInfo.normal, rInfo.object);
					mHitPosition = rInfo.point;
            }
		   }
			if ( disableSourceObjCollision ) {
				mSourceObject->enableCollision();
			}
			enableCollision();

         if(isClientObject()) {
            emitParticles(mCurrPosition, newPos, Point3F(1, 1, 1), TickMs * 4);
            updateSound();
         }
		}
   } 
	else {
		//Non-Moving Beam.
		Point3F newPos = mCurrVelocity * mDataBlock->range;
		newPos += mCurrPosition;

		mHitPosition = newPos;
		if(!mDataBlock->conHitBeam && !alreadyHit) {
         RayInfo rInfo;
         if (gServerContainer.castRay(mCurrPosition, newPos, csmDynamicCollisionMask | csmStaticCollisionMask, &rInfo)) {
			   if(!mDataBlock->conHitBeam) {
			      alreadyHit = true;
			   }
            newPos = rInfo.point;

				if ( disableSourceObjCollision ) {
               mSourceObject->enableCollision();
				}
            onCollision(rInfo.point, rInfo.normal, rInfo.object);
				mHitPosition = rInfo.point;
         }
		}
		if ( disableSourceObjCollision ) {
			mSourceObject->enableCollision();
		}
		enableCollision();

      if(isClientObject()) {
         emitParticles(mCurrPosition, newPos, Point3F(1, 1, 1), TickMs * 4);
         updateSound();
      }
   }
}

//--------------------------------------------------------------------------
void LaserProjectile::prepRenderImage(SceneRenderState* state) {
	ObjectRenderInst *ri = state->getRenderPass()->allocInst<ObjectRenderInst>(); //gRenderInstManager.allocInst();
   ri->renderDelegate.bind(this, &LaserProjectile::renderObject);
	//Foliage works best for "beam" instances.
   ri->type = RenderPassManager::RIT_ObjectTranslucent;
   state->getRenderPass()->addInst( ri );
}

void LaserProjectile::renderObject(ObjectRenderInst *ri, SceneRenderState *state, BaseMatInstance* overrideMat) {
   if (overrideMat)
      return;

	if(mSBlock.isNull()) {
	   GFXStateBlockDesc desc;  

		desc.setBlend( true, GFXBlendSrcAlpha, GFXBlendOne);
      desc.setCullMode(GFXCullNone);
      desc.zWriteEnable = false;
      desc.samplersDefined = true;
      desc.samplers[0].magFilter = GFXTextureFilterLinear;
      desc.samplers[0].minFilter = GFXTextureFilterLinear;
      desc.samplers[0].addressModeU = GFXAddressWrap;
      desc.samplers[0].addressModeV = GFXAddressWrap;

		mSBlock = GFX->createStateBlock(desc);
	}
	GFX->setStateBlock(mSBlock);

	S32 thisTime = Sim::getCurrentTime();
	S32 timeDelta = thisTime - mLastTime;
	if (timeDelta > mDataBlock->interval) {
		mIndex ++;
		if (dStrcmp(mDataBlock->materialListNames[mIndex], "") == 0) {
		   mIndex = 0;
		}
	}

   GFX->setTexture(0, mDataBlock->materialList[mIndex]);
	PrimBuild::color4f( mDataBlock->mBeamColor.red, mDataBlock->mBeamColor.green, mDataBlock->mBeamColor.blue, fadeAlpha );
	PrimBuild::begin(GFXTriangleStrip, 4);

	Point3F crossA, crossB, sPt1, sPt2, ePt1, ePt2;
	Point3F clientView = state->getCameraPosition();
	Point3F clientVec = clientView - mCurrPosition;
	clientVec.normalize();

	crossA = mCross(clientVec, mCurrVelocity);
	crossB = mCross(mCurrVelocity, clientVec);
	crossA.normalize();
	crossB.normalize();

	sPt1 = (crossA * mDataBlock->beamStartRadius) + mCurrPosition;
	sPt2 = (crossB * mDataBlock->beamStartRadius) + mCurrPosition;
	ePt1 = (crossA * mDataBlock->beamEndRadius) + mHitPosition;
	ePt2 = (crossB * mDataBlock->beamEndRadius) + mHitPosition;

	PrimBuild::texCoord2f(0, 0); 
	PrimBuild::vertex3f(sPt1.x, sPt1.y, sPt1.z);
	PrimBuild::texCoord2f(1, 0);
	PrimBuild::vertex3f(sPt2.x, sPt2.y, sPt2.z);
	PrimBuild::texCoord2f(1, 1);
	PrimBuild::vertex3f(ePt2.x, ePt2.y, ePt2.z);
	PrimBuild::texCoord2f(0, 1);
	PrimBuild::vertex3f(ePt1.x, ePt1.y, ePt1.z);
	PrimBuild::end();  
}
