/*
	tracerProjectile.h
	By: Robert C. Fritzen
	Implements a tracer projectile type into T3D MIT
*/
#ifndef _TRACERPROJECTILE_H_
#define _TRACERPROJECTILE_H_

#ifndef _GAMEBASE_H_
#include "T3D/gameBase/gameBase.h"
#endif
#ifndef __RESOURCE_H__
#include "core/resource.h"
#endif
#ifndef _TSSHAPE_H_
#include "ts/tsShape.h"
#endif
#ifndef _LIGHTDESCRIPTION_H_
#include "T3D/lightDescription.h"
#endif
#ifndef _LIGHTINFO_H_
#include "lighting/lightInfo.h"
#endif

#include "T3D/projectile.h"

class TracerProjectile;

//--------------------------------------------------------------------------
class TracerProjectileData : public ProjectileData {
   typedef ProjectileData Parent;
      
   public:

		enum TracerConstants {
			maxTextures = 20,
		};

		StringTableEntry  materialListNames[maxTextures];
		GFXTexHandle  materialList[maxTextures];

      F32 tracerLength;
		F32 tracerWidth;
		S32 interval;
		ColorF mTracerColor;

		TracerProjectileData();

		void packData(BitStream*);
		void unpackData(BitStream*);

		bool preload(bool server, String &errorStr);

		static void initPersistFields();
		DECLARE_CONOBJECT(TracerProjectileData);
};

//--------------------------------------------------------------------------
class TracerProjectile : public Projectile {
   typedef Projectile Parent;
   TracerProjectileData* mDataBlock;

   protected:
		S32           mIndex;
		S32           mLastTime;

		GFXStateBlockRef mSBlock;

		bool onAdd();
		bool onNewDataBlock(GameBaseData*, bool);
		void processTick(const Move* move);
		void simulate(F32 dt);
		U32  packUpdate  (NetConnection *conn, U32 mask, BitStream *stream);
		void unpackUpdate(NetConnection *conn,           BitStream *stream);

		// Rendering
		void prepRenderImage(SceneRenderState* state);
		void renderObject(ObjectRenderInst *ri, SceneRenderState *state, BaseMatInstance* overrideMat);

   public:

      static void initPersistFields();

		TracerProjectile();
		~TracerProjectile();

		DECLARE_CONOBJECT(TracerProjectile);
};

#endif //_TRACERPROJECTILE_H_