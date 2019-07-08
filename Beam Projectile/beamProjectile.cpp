/*
	beamProjectile.cpp
	By: Robert C. Fritzen
	Implements a arcing beam projectile type into T3D MIT
*/

#include "platform/platform.h"
#include "beamProjectile.h"

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

IMPLEMENT_CO_DATABLOCK_V1(BeamProjectileData);
IMPLEMENT_CO_NETOBJECT_V1(BeamProjectile);

BeamProjectileData::BeamProjectileData() {
   ProjectileData::ProjectileData();

	numControlPoints = 2;
	beamWidth = 0.05f;
   interval = 0;
	maximumBeamRange = 0;
	coneRange = 30.0f;
	mBeamColor.set(1.0f, 0, 0);

	drainHealth = 0.0f;
	drainEnergy = 0.0f;
	amountIsHealing = false;

   dMemset( materialListNames, 0, sizeof( materialListNames ) );
   dMemset( materialList, 0, sizeof( materialList ) );
}

// Callbacks

//--------------------------------------------------------------------------

void BeamProjectileData::initPersistFields() {
   Parent::initPersistFields();

   addField("beamWidth", TypeF32, Offset(beamWidth, BeamProjectileData),
      "@brief the width of the rendered beam");
   addField("coneRange", TypeF32, Offset(coneRange, BeamProjectileData),
      "@brief the maximum allowed angle (in degrees) from the center of the screen a target may be aquired from.");
   addField("numControlPoints", TypeS32, Offset(numControlPoints, BeamProjectileData),
      "@brief the amount of bezier control points the beam will use to render the curve.");
   addField( "materialList", TypeString, Offset(materialListNames, BeamProjectileData), maxTextures,
      "List of textures to use to render the beam." );
   addField("maximumBeamRange", TypeF32, Offset(maximumBeamRange, BeamProjectileData),
      "@brief the maximum beam range");
   addField("drainHealth", TypeF32, Offset(drainHealth, BeamProjectileData),
      "@brief amount of health drained from the object every tick");
   addField("drainEnergy", TypeF32, Offset(drainEnergy, BeamProjectileData),
      "@brief amount of energy drained from the object every tick");
	addField("amountIsHealing", TypeBool, Offset(amountIsHealing, BeamProjectileData),
		"@brief if true, the drain values will be restored to the target every tick");
   addField("interval", TypeS32, Offset(interval, BeamProjectileData),
      "@brief the interval of texture change on the beam");
	addField("beamColor", TypeColorF, Offset(mBeamColor, BeamProjectileData), 
		"@brief controls the render color of the beam");
}

//--------------------------------------------------------------------------
void BeamProjectileData::packData(BitStream* stream) {
   Parent::packData(stream);

	stream->write(beamWidth);
	stream->write(coneRange);
	stream->write(interval);
	stream->write(numControlPoints);
	stream->write(maximumBeamRange);
	
	stream->write(drainHealth);
	stream->write(drainEnergy);
	stream->write(amountIsHealing);

	stream->write(mBeamColor.red);
	stream->write(mBeamColor.green);
	stream->write(mBeamColor.blue);

   for (S32 i = 0; i < maxTextures; i++) {
      stream->writeString(materialListNames[i]);
   }
}

void BeamProjectileData::unpackData(BitStream* stream) {
   Parent::unpackData(stream);

   stream->read(&beamWidth);
	stream->read(&coneRange);
	stream->read(&interval);
	stream->read(&numControlPoints);
	stream->read(&maximumBeamRange);

	stream->read(&drainHealth);
	stream->read(&drainEnergy);
	stream->read(&amountIsHealing);

	stream->read(&mBeamColor.red);
	stream->read(&mBeamColor.green);
	stream->read(&mBeamColor.blue);

   for (S32 i = 0; i < maxTextures; i++) {
      materialListNames[i] = stream->readSTString();
   }
}

bool BeamProjectileData::preload(bool server, String &errorStr) {
   if (Parent::preload(server, errorStr) == false)
      return false;

	if(numControlPoints < 1) {
		Con::errorf("BeamProjectileData: Cannot initialize a BeamProjectile that has numControlPoints < 1.");
		return false;
	}

	if(!server) {
      for (U32 i = 0; i < maxTextures; i++) {
         if (materialListNames[i][0])
            materialList[i] = GFXTexHandle(materialListNames[i], &GFXDefaultStaticDiffuseProfile, avar("%s() - materialListNames[%d] (line %d)", __FUNCTION__, i, __LINE__));
      }		
	}
	return true;
}
//--------------------------------------------------------------------------
BeamProjectile::BeamProjectile() {
   Projectile::Projectile();

   mLastTime = 0;
   mIndex = 0;

	mTargetID = -1;
	mTarget = NULL;

	remove = false;
}

BeamProjectile::~BeamProjectile() {
	mSourceObject = NULL;
	mTarget = NULL;
}

//--------------------------------------------------------------------------
void BeamProjectile::initPersistFields() {
   Parent::initPersistFields();
}

//--------------------------------------------------------------------------
bool BeamProjectile::targetInFov(Point3F tPos) {
   VectorF sVec = mCurrVelocity, tVec = (tPos - mCurrPosition);
	if(!sVec.isUnitLength()) 
	   sVec.normalizeSafe();
	if(!tVec.isUnitLength())
	   tVec.normalizeSafe();
	//
	F32 dot = mDot(sVec, tVec);
	F32 lena = sVec.len();
	F32 lenb = tVec.len();

	if(mAcos((dot) / (lena * lenb)) <= mDegToRad(mDataBlock->coneRange)) {
	   return true;
	}
	return false;
}

//--------------------------------------------------------------------------
U32 BeamProjectile::packUpdate(NetConnection* con, U32 mask, BitStream* stream) {
   U32 retMask = Parent::packUpdate(con, mask, stream);

	stream->writeInt(mLastTime, 8);
	stream->writeInt(mIndex, 8);

	stream->writeInt(mTargetID, 8);

	stream->writeFlag(remove);

	mathWrite(*stream, mCurrPosition);
	mathWrite(*stream, mCurrVelocity);

   return retMask;
}

void BeamProjectile::unpackUpdate(NetConnection* con, BitStream* stream) {
   Parent::unpackUpdate(con, stream);

	mLastTime = stream->readInt(8);
	mIndex = stream->readInt(8);

	mTargetID = stream->readInt(8);

	remove = stream->readFlag();

	mathRead( *stream, &mCurrPosition);
	mathRead( *stream, &mCurrVelocity);
}

//--------------------------------------------------------------------------
bool BeamProjectile::onAdd() {
	ShapeBase *ptr = NULL;
	if(isServerObject()) {
	   String firingBeam; 
	   if (Sim::findObject(mSourceObjectId, ptr)) {
		   firingBeam = ptr->getDataField(StringTable->insert("firingBeam"), 0);
		   if(firingBeam.compare("1") == 0) {
		      //Check if the source object already had the fire trigger down, if so, then halt firing, having multiple beamProjectile
			   // instances from one source object is a very bad thing.
		      if(ptr->getImageTriggerState(0) == true) {
			      remove = true;
			      return false;
		      }
		   }
	   }
	}
	//
	if(!Parent::onAdd()) { 
		return false;
	}

	if(isServerObject()) {
	   if(ptr != NULL) {
	      ptr->setDataField(StringTable->insert("firingBeam"), NULL, "1"); 
		}
	}
      
   return true;
}

void BeamProjectile::onRemove() {
	if(mSourceObject) {
	   mSourceObject->setDataField(StringTable->insert("firingBeam"), NULL, "0"); 
	}
   Parent::onRemove();
}

bool BeamProjectile::onNewDataBlock(GameBaseData* dptr, bool reload) {
   mDataBlock = dynamic_cast<BeamProjectileData*>(dptr);
	if (!mDataBlock || !Parent::onNewDataBlock(dptr, reload)) {
      return false;
	}
   return true;
}

//--------------------------------------------------------------------------
void BeamProjectile::advanceTime(F32 dt) {
	GameBase::advanceTime(dt);
}

void BeamProjectile::processTick(const Move *move) {
   GameBase::processTick( move );
   mCurrTick++;

   simulate( TickSec );	
}

void BeamProjectile::simulate(F32 dt) {
	//This prevents the object from being deleted too soon.
	if(remove) {
		if(isServerObject()) {
		   safeDeleteObject();
		}
		return;
	}
   mCurrTick++;

	/*
		Beam Projectile Steps:
		1. Find Source Object
		2. Check trigger state of source object
		3. Fetch a target
	*/
   if (isServerObject()) {
		if(mSourceObject != NULL) {
			if(mSourceObject->getImageTriggerState(0) == true) {
				if(!fetchTarget()) {
					remove = true;
					return;
				}
				else {
					//Apply Damage & Energy Drain
					if(mDataBlock->amountIsHealing) {
						mTarget.getObject()->applyRepair(mDataBlock->drainHealth);
						mTarget.getObject()->setEnergyLevel(mTarget.getObject()->getEnergyLevel() + mDataBlock->drainEnergy);				
					}
					else {
						mTarget.getObject()->applyDamage(mDataBlock->drainHealth);
						mTarget.getObject()->setEnergyLevel(mTarget.getObject()->getEnergyLevel() - mDataBlock->drainEnergy);
					}
				}			
			}
			else {
				//Trigger gone, delete.
				remove = true;
			}
			//Now, we can run through our calculations steps.
         Point3F mMuzzlePosition, mMuzzleVector;
         MatrixF muzzleTrans;
         mSourceObject->getMuzzleTransform(mSourceObjectSlot, &muzzleTrans);
		   muzzleTrans.getColumn(3, &mMuzzlePosition);
         muzzleTrans.getColumn(1, &mMuzzleVector);

         mCurrPosition = mMuzzlePosition;

         MatrixF xform(true);
         xform.setColumn(3, mCurrPosition);
         setTransform(xform);
		}
   }
	else {
		//Update the source transform to match that of the muzzle, this will ensure processTick keeps getting called, and it will ensure
		//we're rendering the beam in the correct spot every time :)
      Point3F mMuzzlePosition, mMuzzleVector;
      MatrixF muzzleTrans;
      mSourceObject->getMuzzleTransform(mSourceObjectSlot, &muzzleTrans);
		muzzleTrans.getColumn(3, &mMuzzlePosition);
      muzzleTrans.getColumn(1, &mMuzzleVector);

      mCurrPosition = mMuzzlePosition;

      MatrixF xform(true);
      xform.setColumn(3, mCurrPosition);
      setTransform(xform);
	}
	//No Source Object Yet, check for a target anyways, if there isn't one, then there's no point to continue.
	if(!fetchTarget()) {
		remove = true;
		return;
	}
}

bool BeamProjectile::fetchTarget() {
	//Note: If at some point, someone manages to get a cone search working, please let me know so I can update this.. ~Phantom
	// First things first, if we already have a target object or a target ID, validate it.
	if(remove) {
		return false;
	}
	bool disableSourceObjCollision = (mSourceObject.isValid() && mCurrTick <= SourceIdTimeoutTicks);
	if ( disableSourceObjCollision ) {
		mSourceObject->disableCollision();
	}
   disableCollision();

	RayInfo rinfo;
	U32 TestMask = PlayerObjectType | VehicleObjectType;
	if(mTarget != NULL || mTargetID != -1) {
		if(mTarget.getObject()) { 
			//Out of Range?
		   if((mTarget->getPosition() - mCurrPosition).len() > mDataBlock->maximumBeamRange) {
			   mTarget = NULL;
			   mTargetID = -1;
			}
			//Outside FOV cone?
			else if(!targetInFov(mTarget->getPosition())) {
			   mTarget = NULL;
			   mTargetID = -1;			
			}
			//Objects in between?
			else if(gServerContainer.castRay(mCurrPosition, mTarget->getPosition(), (TerrainObjectType | StaticObjectType | TestMask), &rinfo)) {
				if(rinfo.object != mTarget) {
			      mTarget = NULL;
			      mTargetID = -1;				
				}
			}
			//Or nothing at all?
			else if(!gServerContainer.castRay(mCurrPosition, mTarget->getPosition(), (TerrainObjectType | StaticObjectType | TestMask), &rinfo)) {
			   mTarget = NULL;
			   mTargetID = -1;			
			}
			//All good.
			else {
				enableCollision();
				if(disableSourceObjCollision) {
					mSourceObject->enableCollision();
				}
				return true;
			}
		}
		else {
			//If we have the ID, but no object, we need to see if it's still there.
			ShapeBase *ptr;
			if(Sim::findObject(mTargetID, ptr)) {
				mTarget = ptr;
				//Yep, got it, now run through our tests.
				//Out of Range?
				if((mTarget->getPosition() - mCurrPosition).len() > mDataBlock->maximumBeamRange) {
					mTarget = NULL;
					mTargetID = -1;
				}
				//Outside FOV cone?
				else if(targetInFov(mTarget->getPosition())) {
					mTarget = NULL;
					mTargetID = -1;			
				}
				//Objects in between?
				else if(gServerContainer.castRay(mCurrPosition, mTarget->getPosition(), (TerrainObjectType | StaticObjectType | TestMask), &rinfo)) {
					if(rinfo.object != mTarget) {
						mTarget = NULL;
						mTargetID = -1;				
					}
				}
				//Or nothing at all?
				else if(!gServerContainer.castRay(mCurrPosition, mTarget->getPosition(), (TerrainObjectType | StaticObjectType | TestMask), &rinfo)) {
					mTarget = NULL;
					mTargetID = -1;			
				}
				//All good.
				else {
				   enableCollision();
				   if(disableSourceObjCollision) {
				   	mSourceObject->enableCollision();
				   }
					return true;
		   	}
			}
		}
	}
	//And if the above fails, then we have no target object, and need to scan for one.
	F32 closest = mDataBlock->maximumBeamRange;
	SceneObject *test = NULL;
	ShapeBase *target = NULL;
	//We use a radius search to find targets
	gServerContainer.initRadiusSearch(mCurrPosition, mDataBlock->maximumBeamRange, TestMask);
	while((test = gServerContainer.containerSearchNextObject()) != NULL) {
		if(dynamic_cast<ShapeBase *>(test)->isDestroyed()) {
		   //Don't hunt out destroyed stuff.
			continue;
		}
		if(test != this) {
			if((test->getPosition() - getPosition()).len() < closest) {
				//FoV Test.
				if(targetInFov(test->getPosition())) {	
					//Raycast Test
					if(gServerContainer.castRay(mCurrPosition, test->getPosition(), (TerrainObjectType | StaticObjectType | TestMask), &rinfo)) {
						if(rinfo.object == test) {
							closest = (test->getPosition() - getPosition()).len();
			            target = dynamic_cast<ShapeBase *>(test);
						}
					}
			   }
		   }
		}
	}
	//
	enableCollision();
	if(disableSourceObjCollision) {
		mSourceObject->enableCollision();
	}
	//Pull the target.
	if(target) {
		mTargetID = target->getId();
		mTarget = target;
		return true;
	}
	else {
		return false;
	}
}

//--------------------------------------------------------------------------
void BeamProjectile::prepRenderImage(SceneRenderState* state) {
	ObjectRenderInst *ri = state->getRenderPass()->allocInst<ObjectRenderInst>(); //gRenderInstManager.allocInst();
   ri->renderDelegate.bind(this, &BeamProjectile::renderObject);
	//Foliage works best for "beam" instances.
   ri->type = RenderPassManager::RIT_ObjectTranslucent;
   state->getRenderPass()->addInst( ri );
}

void BeamProjectile::renderObject(ObjectRenderInst *ri, SceneRenderState *state, BaseMatInstance* overrideMat) {
	if (overrideMat) {
      return;
	}

	if(remove) {
		return;
	}

	if(!mSourceObject.getObject() || !mTarget.getObject()) {
		return;
	}

	if(mSBlock.isNull()) {
	   GFXStateBlockDesc desc;  

		desc.setBlend( true, GFXBlendSrcAlpha, GFXBlendOne);
      desc.setCullMode(GFXCullNone);
      desc.zWriteEnable = true;
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
	PrimBuild::color4f( mDataBlock->mBeamColor.red, mDataBlock->mBeamColor.green, mDataBlock->mBeamColor.blue, 1.0f );
	PrimBuild::begin(GFXTriangleStrip, mDataBlock->numControlPoints * 2);
	//Obtain the midpoint
   Point3F mMuzzlePosition, mMuzzleVector;
   MatrixF muzzleTrans;
   mSourceObject->getMuzzleTransform(mSourceObjectSlot, &muzzleTrans);
	muzzleTrans.getColumn(3, &mMuzzlePosition);
   muzzleTrans.getColumn(1, &mMuzzleVector);
	//
	Point3F controlPoint;
	F32 midPtLength = (mTarget->getPosition() - mMuzzlePosition).len();
	controlPoint = mMuzzleVector;
	controlPoint.normalizeSafe();
	controlPoint *= midPtLength;
	controlPoint += mMuzzlePosition;
	//Generate a new node list using a bezier curve.
	Point3F cam = state->getCameraPosition(), dir, cross, dirBeam;
	Point3F newPos, trgPos;
	F32 t = 0.0f;

	//Note: If you want to hit a certain part of the player, or do some offset stuff, here's the place.
   trgPos.set(mTarget->getPosition());

	for(S32 i = 0; i <= mDataBlock->numControlPoints; i++) {
		t = F32(i) / F32(mDataBlock->numControlPoints);
		newPos.set((mMuzzlePosition.x * mPow(t, 2)) + (2 * controlPoint.x * t * (1 - t)) + (trgPos.x * mPow((1 - t), 2)), 
			(mMuzzlePosition.y * mPow(t, 2)) + (2 * controlPoint.y * t * (1 - t)) + (trgPos.y * mPow((1 - t), 2)), 
			(mMuzzlePosition.z * mPow(t, 2)) + (2 * controlPoint.z * t * (1 - t)) + (trgPos.z * mPow((1 - t), 2)));

		dir = newPos - cam;
		dirBeam = i == mDataBlock->numControlPoints-1 ? Point3F(0, 0, 0) : newPos;
		dirBeam.normalizeSafe();

		mCross(dir, dirBeam, &cross);
		cross.normalizeSafe();
		cross *= mDataBlock->beamWidth * 0.5f;

      PrimBuild::texCoord2f( i%2, 1.0 );
		PrimBuild::vertex3fv( newPos - cross );

      PrimBuild::texCoord2f( i%2, 0.0 );
		PrimBuild::vertex3fv( newPos + cross );
	}
	PrimBuild::end();
}
