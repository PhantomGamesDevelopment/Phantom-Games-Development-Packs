/**
Tornado.h
Robert Fritzen
(c) Phantom Games Development, 2013

Central file for the Tornado class.

This class inherits from ParticleEmitterNode but behaves with a different manner.
The tornado uses the rankine vortex model to determine the pull strength on local objects,
that may be defined by the datablock. The strength and size may increase or decrease at any time
and the user can control when the tornado is created and destroyed with a few console calls.

The tangential velocity of a rankine vortex is defined as:

u = (s * d_from_center) / (2*pi*(radius_of_maximum_flow^2)) when D <= r_max
u = (s) / (2*pi*d_from_center) when D > r_max
**/
#ifndef _tornado_H
#define _tornado_H

#include "T3D/fx/particleEmitterNode.h"
#ifndef _GAMEBASE_H_
#include "T3D/gameBase/gameBase.h"
#endif

//--------------------------------------------------------------------------
class Tornado : public ParticleEmitterNode {
  protected:

   typedef ParticleEmitterNode Parent;

   enum MaskBits
   {
      StateMask      = Parent::NextFreeMask << 0,
      EmitterDBMask  = Parent::NextFreeMask << 1,
      NextFreeMask   = Parent::NextFreeMask << 2,
   };

  private:
   ParticleEmitterNodeData* mDataBlock;

  protected:
   DECLARE_CALLBACK( void, onEnterOuterZone, ( SceneObject* hitObject));
	DECLARE_CALLBACK( void, onEnterInnerZone, ( SceneObject* hitObject));

   bool onAdd();
   void onRemove();
   bool onNewDataBlock( GameBaseData *dptr, bool reload );
   void inspectPostApply();

   ParticleEmitterData* mEmitterDatablock;
   S32                  mEmitterDatablockId;

   bool             mActive;

   SimObjectPtr<ParticleEmitter> mEmitter;
   F32              mVelocity;

   //------------------------------
   F32              mStrength;
   F32              mTornadoRadius;
   F32              mEdgeRadius;
   //------------------------------

  public:
   Tornado();
   ~Tornado();
   
   ParticleEmitter *getParticleEmitter() {return mEmitter;}
   
   // Time/Move Management
  public:
   void processTick(const Move* move);
   void advanceTime(F32 dt);
   void applyForces();

   DECLARE_CONOBJECT(Tornado);
   static void initPersistFields();

   U32  packUpdate  (NetConnection *conn, U32 mask, BitStream* stream);
   void unpackUpdate(NetConnection *conn,           BitStream* stream);

   inline bool getActive( void )        { return mActive;                             };
   inline void setActive( bool active ) { mActive = active; setMaskBits( StateMask ); };

   void setEmitterDataBlock(ParticleEmitterData* data);
};

#endif // tornado_H

