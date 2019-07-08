/*
	tracerProjectile.cpp
	By: Robert C. Fritzen
	Implements a tracer projectile type into T3D MIT
*/

#include "platform/platform.h"
#include "tracerProjectile.h"

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

IMPLEMENT_CO_DATABLOCK_V1(TracerProjectileData);
IMPLEMENT_CO_NETOBJECT_V1(TracerProjectile);

TracerProjectileData::TracerProjectileData() {
   ProjectileData::ProjectileData();

	tracerWidth = 1.0f;
	tracerLength = 1.0f;
   interval = 0;
	mTracerColor.set(1.0f, 0, 0);

   dMemset( materialListNames, 0, sizeof( materialListNames ) );
   dMemset( materialList, 0, sizeof( materialList ) );
}

//--------------------------------------------------------------------------

void TracerProjectileData::initPersistFields() {
   Parent::initPersistFields();

   addField( "materialList", TypeString, Offset(materialListNames, TracerProjectileData), maxTextures,
      "List of textures to use to render tracer beam." );
   addField("tracerLength", TypeF32, Offset(tracerLength, TracerProjectileData),
      "@brief the length of the tracer beam");
   addField("tracerWidth", TypeF32, Offset(tracerWidth, TracerProjectileData),
      "@brief the width of the tracer beam");
   addField("interval", TypeS32, Offset(interval, TracerProjectileData),
      "@brief the interval of texture change on the tracer");
	addField("tracerColor", TypeColorF, Offset(mTracerColor, TracerProjectileData), 
		"@brief controls the render color of the tracer");
}

//--------------------------------------------------------------------------
void TracerProjectileData::packData(BitStream* stream) {
   Parent::packData(stream);

	stream->write(tracerLength);
	stream->write(tracerWidth);
	stream->write(interval);

	stream->write(mTracerColor.red);
	stream->write(mTracerColor.green);
	stream->write(mTracerColor.blue);

   for (S32 i = 0; i < maxTextures; i++) {
      stream->writeString(materialListNames[i]);
   }
}

void TracerProjectileData::unpackData(BitStream* stream) {
   Parent::unpackData(stream);

   stream->read(&tracerLength);
	stream->read(&tracerWidth);
	stream->read(&interval);

	stream->read(&mTracerColor.red);
	stream->read(&mTracerColor.green);
	stream->read(&mTracerColor.blue);

   for (S32 i = 0; i < maxTextures; i++) {
      materialListNames[i] = stream->readSTString();
   }
}

bool TracerProjectileData::preload(bool server, String &errorStr) {
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
TracerProjectile::TracerProjectile() {
   Projectile::Projectile();

   mLastTime = 0;
   mIndex = 0;
}

TracerProjectile::~TracerProjectile() {
}

//--------------------------------------------------------------------------
void TracerProjectile::initPersistFields() {
   Parent::initPersistFields();
}

//--------------------------------------------------------------------------
U32 TracerProjectile::packUpdate(NetConnection* con, U32 mask, BitStream* stream) {
   U32 retMask = Parent::packUpdate(con, mask, stream);

	stream->writeInt(mLastTime, 8);
	stream->writeInt(mIndex, 8);

   return retMask;
}

void TracerProjectile::unpackUpdate(NetConnection* con, BitStream* stream) {
   Parent::unpackUpdate(con, stream);

	mLastTime = stream->readInt(8);
	mIndex = stream->readInt(8);
}

//--------------------------------------------------------------------------
bool TracerProjectile::onAdd() {
	if(!Parent::onAdd()) { 
		return false;
	}
      
   return true;
}

bool TracerProjectile::onNewDataBlock(GameBaseData* dptr, bool reload) {
   mDataBlock = dynamic_cast<TracerProjectileData*>(dptr);
	if (!mDataBlock || !Parent::onNewDataBlock(dptr, reload)) {
      return false;
	}
   return true;
}

//--------------------------------------------------------------------------
void TracerProjectile::processTick(const Move* move) {
   Parent::processTick( move );
}

void TracerProjectile::simulate(F32 dt) {
	Parent::simulate(dt);
}

//--------------------------------------------------------------------------
void TracerProjectile::prepRenderImage(SceneRenderState* state) {
	ObjectRenderInst *ri = state->getRenderPass()->allocInst<ObjectRenderInst>(); //gRenderInstManager.allocInst();
   ri->renderDelegate.bind(this, &TracerProjectile::renderObject);
   ri->type = RenderPassManager::RIT_ObjectTranslucent;
   state->getRenderPass()->addInst( ri );
}

void TracerProjectile::renderObject(ObjectRenderInst *ri, SceneRenderState *state, BaseMatInstance* overrideMat) {
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
	PrimBuild::color4f( mDataBlock->mTracerColor.red, mDataBlock->mTracerColor.green, mDataBlock->mTracerColor.blue, 1.0f );
	PrimBuild::begin(GFXTriangleStrip, 4);

	Point3F crossA, crossB, sPt1, sPt2, ePt1, ePt2;
	Point3F clientView = state->getCameraPosition();
	Point3F clientVec = clientView - mCurrPosition;
	clientVec.normalize();

	crossA = mCross(clientVec, mCurrVelocity);
	crossB = mCross(mCurrVelocity, clientVec);
	crossA.normalize();
	crossB.normalize();

	sPt1 = ((crossA * mDataBlock->tracerWidth) + mCurrPosition) - (mCurrVelocity * mDataBlock->tracerLength * 0.1f);
	sPt2 = ((crossB * mDataBlock->tracerWidth) + mCurrPosition) - (mCurrVelocity * mDataBlock->tracerLength * 0.1f);
	ePt1 = (crossA * mDataBlock->tracerWidth) + mCurrPosition;
	ePt2 = (crossB * mDataBlock->tracerWidth) + mCurrPosition;

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