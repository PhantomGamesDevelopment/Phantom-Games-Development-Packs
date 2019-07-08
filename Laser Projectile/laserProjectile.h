/*
   LaserProjectile.h
	By: Robert C. Fritzen
	Adapted from the TGE/TGEA Laser Resource For T3D MIT 3.5
*/

#ifndef _LASERPROJECTILE_H_
#define _LASERPROJECTILE_H_

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

class LaserProjectile;

//--------------------------------------------------------------------------
class LaserProjectileData : public ProjectileData {
   typedef ProjectileData Parent;
      
   public:

		enum LaserConstants {
			maxTextures = 20,
		};

		StringTableEntry  materialListNames[maxTextures];
		GFXTexHandle  materialList[maxTextures];

      F32 beamStartRadius;
		F32 beamEndRadius;
		F32 range;
		S32 interval;
		S32 fadeBegin;
		ColorF mBeamColor;

		bool conHitBeam;
		bool conMoveBeam;

		LaserProjectileData();

		void packData(BitStream*);
		void unpackData(BitStream*);

		bool preload(bool server, String &errorStr);

		static void initPersistFields();
		DECLARE_CONOBJECT(LaserProjectileData);
};

//--------------------------------------------------------------------------
class LaserProjectile : public Projectile {
   typedef Projectile Parent;
   LaserProjectileData* mDataBlock;

   protected:
		S32           mIndex;
		S32           mLastTime;

		F32           fadeAlpha;

		Point3F       mHitPosition;

		bool          alreadyHit;

		GFXStateBlockRef mSBlock;

		bool onAdd();
		bool onNewDataBlock(GameBaseData*, bool);
		void processTick(const Move* move);
		U32  packUpdate  (NetConnection *conn, U32 mask, BitStream *stream);
		void unpackUpdate(NetConnection *conn,           BitStream *stream);

		// Rendering
		void prepRenderImage(SceneRenderState* state);
		void renderObject(ObjectRenderInst *ri, SceneRenderState *state, BaseMatInstance* overrideMat);

   public:

      static void initPersistFields();

		LaserProjectile();
		~LaserProjectile();

		DECLARE_CONOBJECT(LaserProjectile);
};

#endif //_LASERPROJECTILE_H_