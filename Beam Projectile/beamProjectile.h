/*
	beamProjectile.h
	By: Robert C. Fritzen
	Implements a arcing beam projectile type into T3D MIT
*/

#ifndef _BEAMPROJECTILE_H_
#define _BEAMPROJECTILE_H_

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

class BeamProjectile;

//--------------------------------------------------------------------------
class BeamProjectileData : public ProjectileData {
   typedef ProjectileData Parent;
      
   public:

		enum BeamConstants {
			maxTextures = 20,
		};

		StringTableEntry  materialListNames[maxTextures];
		GFXTexHandle  materialList[maxTextures];

		//This is used to build the "arc" loop
		S32 numControlPoints;
		//Maximum width of the beam
      F32 beamWidth;
		//Maximum Range the beam may go
		F32 maximumBeamRange;
		//Maximum cone angle from the source that a beam may "grab" on to, also, this is used to determine if the target still remains
		// within the cone.
		F32 coneRange;
		S32 interval;
		ColorF mBeamColor;

		bool amountIsHealing;
		F32 drainHealth;
		F32 drainEnergy;

		BeamProjectileData();

		void packData(BitStream*);
		void unpackData(BitStream*);

		bool preload(bool server, String &errorStr);

		static void initPersistFields();
		DECLARE_CONOBJECT(BeamProjectileData);
};

//--------------------------------------------------------------------------
class BeamProjectile : public Projectile {
   typedef Projectile Parent;
   BeamProjectileData* mDataBlock;

   protected:
		S32           mIndex;
		S32           mLastTime;

		bool remove;

		//BeamNode Methods
		S32 nodeCount;
		void removeAll();

	   SimObjectPtr<ShapeBase> mTarget;   
	   S32 mTargetID;   

		GFXStateBlockRef mSBlock;

		bool targetInFov(Point3F tPos);

		bool onAdd();
		void onRemove();
		bool onNewDataBlock(GameBaseData*, bool);
		void processTick(const Move* move); 
		void advanceTime( F32 dt );
		void simulate( F32 dt );
		U32  packUpdate  (NetConnection *conn, U32 mask, BitStream *stream);
		void unpackUpdate(NetConnection *conn,           BitStream *stream);

		bool fetchTarget();

		// Rendering
		void prepRenderImage(SceneRenderState* state);
		void renderObject(ObjectRenderInst *ri, SceneRenderState *state, BaseMatInstance* overrideMat);

   public:

      static void initPersistFields();

		BeamProjectile();
		~BeamProjectile();

		DECLARE_CONOBJECT(BeamProjectile);
};

#endif //_BEAMPROJECTILE_H_