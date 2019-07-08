/**
Tornado.cpp
Robert Fritzen
(c) Phantom Games Development, 2013

Central file for the Tornado class.

The tornado object bases of of the ParticleEmitterNode class. It uses the rankine vortex
model to apply forces to objects within a radius of the tornado. The strength of the tornado
determines the inward force the tornado applies and if the tornado can even pull the object
based on the mass of the object.
**/

#include "T3D/fx/Tornado.h"
#include "console/consoleTypes.h"
#include "core/stream/bitStream.h"
#include "T3D/fx/particleEmitter.h"
#include "math/mathIO.h"
#include "sim/netConnection.h"
#include "console/engineAPI.h"

const U32 sTornadoMask = ( VehicleObjectType | PlayerObjectType | ItemObjectType | CorpseObjectType | DebrisObjectType );
const F32 sGravity = 9.81;

IMPLEMENT_CO_NETOBJECT_V1(Tornado);
ConsoleDocClass( Tornado,
   "@brief A particle emitter object that can be positioned in the world and "
   "dynamically enabled or disabled, that applies forces to objects near it.\n\n"

   "@tsexample\n"
   "datablock ParticleEmitterNodeData( SimpleEmitterNodeData )\n"
   "{\n"
   "   timeMultiple = 1.0;\n"
   "};\n\n"

   "%tornado = new Tornado()\n"
   "{\n"
   "   datablock = SimpleEmitterNodeData;\n"
   "   active = true;\n"
   "   emitter = TornadoData;\n"
   "   velocity = 3.5;\n"
   "   strength = 100;\n"
   "   tornadoRadius = 15;\n"
   "   edgeRadius = 35;\n"
   "};\n\n"
   "@endtsexample\n"

   "@note To change the emitter field dynamically (after the ParticleEmitterNode "
   "object has been created) you must use the setEmitterDataBlock() method or the "
   "change will not be replicated to other clients in the game.\n"
   "Similarly, use the setActive() method instead of changing the active field "
   "directly. When changing velocity, you need to toggle setActive() on and off "
   "to force the state change to be transmitted to other clients, changing the "
   "active field also respectively enables and disables the force effect.\n\n"

   "@ingroup Atmosphere\n"
   "@see ParticleEmitterNodeData\n"
);

IMPLEMENT_CALLBACK( Tornado, onEnterOuterZone, void, ( SceneObject* hitObject ),
   ( hitObject ),
   "Informs an object it has entered the outer radius and is being affected.\n"
   "@param hitObject Object that is being affected.\n"
   "@param inOut Boolean variable to state if an object is entering or exiting\n"
   "@tsexample\n"
   "function Tornado::onEnterOuterZone( %this, %hitObject )\n"
   "{\n"
   "   // apply damage to the object\n"
   "   %hitObject.applyDamage( 0.1 );\n"
   "}\n"
   "@endtsexample\n"
);

IMPLEMENT_CALLBACK( Tornado, onEnterInnerZone, void, ( SceneObject* hitObject ),
   ( hitObject ),
   "Informs an object it has entered the inner radius and is being affected.\n"
   "@param hitObject Object that is being affected.\n"
   "@param inOut Boolean variable to state if an object is entering or exiting\n"
   "@tsexample\n"
   "function Tornado::onEnterInnerZone( %this, %hitObject )\n"
   "{\n"
   "   // apply damage to the object\n"
   "   %hitObject.applyDamage( 0.5 );\n"
   "}\n"
   "@endtsexample\n"
);

//-----------------------------------------------------------------------------
// Tornado
//-----------------------------------------------------------------------------
Tornado::Tornado() {
   // Todo: ScopeAlways?
   mNetFlags.set(Ghostable);
   mTypeMask |= EnvironmentObjectType;

   mActive = true;

   mDataBlock          = NULL;
   mEmitterDatablock   = NULL;
   mEmitterDatablockId = 0;
   mEmitter            = NULL;
   mVelocity           = 1.0;
   mStrength           = 1.0;
   mTornadoRadius      = 1.0;
   mEdgeRadius         = 1.0;
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------
Tornado::~Tornado() {
   //
}

//-----------------------------------------------------------------------------
// initPersistFields
//-----------------------------------------------------------------------------
void Tornado::initPersistFields() {
   addField( "active", TYPEID< bool >(), Offset(mActive,Tornado),
      "Controls whether particles are emitted from this node." );
   addField( "emitter",  TYPEID< ParticleEmitterData >(), Offset(mEmitterDatablock, Tornado),
      "Datablock to use when emitting particles." );
   addField( "velocity", TYPEID< F32 >(), Offset(mVelocity, Tornado),
      "Velocity to use when emitting particles (in the direction of the "
      "ParticleEmitterNode object's up (Z) axis)." );
   addField( "strength", TYPEID< F32 >(), Offset(mStrength, Tornado),
      "The tornado's radial wind speed, equavalent to the pulling force "
      "of the tornado used to overcome the mass force of a target." );
   addField( "tornadoRadius", TYPEID< F32 >(), Offset(mTornadoRadius, Tornado),
	  "The tornado's radius of maximum winds (edge of funnel).");
   addField( "edgeRadius", TYPEID< F32 >(), Offset(mEdgeRadius, Tornado),
	  "The maximum radius where the force test is applied.");

   Parent::initPersistFields();
}

//-----------------------------------------------------------------------------
// onAdd
//-----------------------------------------------------------------------------
bool Tornado::onAdd() {
   if( !Parent::onAdd() ) {
      return false;
   }

   if( !mEmitterDatablock && mEmitterDatablockId != 0 ) {
      if( Sim::findObject(mEmitterDatablockId, mEmitterDatablock) == false ) {
         Con::errorf(ConsoleLogEntry::General, "Tornado::onAdd: Invalid packet, bad datablockId(mEmitterDatablock): %d", mEmitterDatablockId);
	  }
   }

   if( isClientObject() ) {
      setEmitterDataBlock( mEmitterDatablock );
   }
   else {
      setMaskBits( StateMask | EmitterDBMask );
   }

   mObjBox.minExtents.set(-0.5, -0.5, -0.5);
   mObjBox.maxExtents.set( 0.5,  0.5,  0.5);
   resetWorldBox();
   addToScene();

   return true;
}

//-----------------------------------------------------------------------------
// onRemove
//-----------------------------------------------------------------------------
void Tornado::onRemove() {
   removeFromScene();
   if( isClientObject() ) {
      if( mEmitter ) {
         mEmitter->deleteWhenEmpty();
         mEmitter = NULL;
      }
   }

   Parent::onRemove();
}

//-----------------------------------------------------------------------------
// onNewDataBlock
//-----------------------------------------------------------------------------
bool Tornado::onNewDataBlock( GameBaseData *dptr, bool reload ) {
   mDataBlock = dynamic_cast<ParticleEmitterNodeData*>( dptr );
   if ( !mDataBlock || !Parent::onNewDataBlock( dptr, reload ) ) {
      return false;
   }

   scriptOnNewDataBlock();
   return true;
}

//-----------------------------------------------------------------------------
void Tornado::inspectPostApply() {
   Parent::inspectPostApply();
   setMaskBits(StateMask | EmitterDBMask);
}

//-----------------------------------------------------------------------------
// advanceTime
//-----------------------------------------------------------------------------
void Tornado::processTick(const Move* move) {
   Parent::processTick(move);

	applyForces();

   if ( isMounted() ) {
      MatrixF mat;
      mMount.object->getMountTransform( mMount.node, mMount.xfm, &mat );
      setTransform( mat );
   }
}

void Tornado::advanceTime(F32 dt) {
   Parent::advanceTime(dt);
   
   if(!mActive || mEmitter.isNull() || !mDataBlock) {
      return;
   }

   Point3F emitPoint, emitVelocity;
   Point3F emitAxis(0, 0, 1);
   getTransform().mulV(emitAxis);
   getTransform().getColumn(3, &emitPoint);
   emitVelocity = emitAxis * mVelocity;

   mEmitter->emitParticles(emitPoint, emitPoint,
                           emitAxis,
                           emitVelocity, (U32)(dt * mDataBlock->timeMultiple * 1000.0f));
}

//-----------------------------------------------------------------------------
// applyForces
//-----------------------------------------------------------------------------
void Tornado::applyForces() {
   if(!mActive) {
      return;
   }
	Point3F inVec, tMom, tPos, center = getTransform().getPosition();
	Point2F tMom2, inVec2;
   String outTest, inTest;

	F32 distanceToCenter, objMass, radialForce, z;
   Box3F triggerBox( mEdgeRadius * 2 );
   triggerBox.setCenter( getTransform().getPosition() );

   SimpleQueryList sql;
   getContainer()->findObjects( triggerBox, sTornadoMask, SimpleQueryList::insertionCallback, &sql );
   for ( int i = 0; i < sql.mList.size(); i++ ) {
      //start with some object properties
		tMom = sql.mList[i]->getMomentum();
		objMass = sql.mList[i]->getMass();
		tPos = sql.mList[i]->getPosition();

		outTest = sql.mList[i]->getDataField(StringTable->insert("inOuterZone"), 0);
		inTest = sql.mList[i]->getDataField(StringTable->insert("inInnerZone"), 0);
		//Even though the force may not be active, we may want to start up some debris damage
		//or send the player some kind of alert notification
		if(dAtoi(outTest.c_str()) == 0) {
	      sql.mList[i]->setDataField(StringTable->insert("inOuterZone"), NULL, "1"); 
		   onEnterOuterZone_callback(sql.mList[i]);
	   }  

		//calculate the distance to the center of the rotation
		distanceToCenter = (tPos - center).len();
		if(distanceToCenter <= mTornadoRadius) {
		   if(dAtoi(inTest.c_str()) == 0) {
	         sql.mList[i]->setDataField(StringTable->insert("inInnerZone"), NULL, "1"); 
		      onEnterInnerZone_callback(sql.mList[i]);
	      }  		
		}

		//Con::printf("Got Object %i: Mass: %f, Momentum: <%f, %f, %f>, DTC: %f", sql.mList[i]->getId(), objMass, tMom.x, tMom.y, tMom.z, distanceToCenter);
      //check the radial strength
		//TODO: Update this rankine vortex model to use an exponentially decreasing radial force
		//from the tornadoRadius to the tornadoEdge such that the force on the radius is maximized
		//and the force on the edge is minimal or zero
		if(distanceToCenter > mTornadoRadius) {
		   radialForce = (mStrength * distanceToCenter) / (2 * M_PI * mPow(mTornadoRadius, 2));
			//Con::printf("Tornado (%f, %f): dTC > tRad: %f", mStrength, mTornadoRadius, radialForce);
		}
		else {
		   radialForce = (mStrength) / (2 * M_PI * distanceToCenter);
			//Con::printf("Tornado (%f): dTC <= tRad: %f", mStrength, radialForce);
		}
		//mass is a downward acting force that applies (.5 * g * m) friction to the pull, subtract this from the radialForce
		radialForce -= (0.5 * sGravity * objMass);
		if(radialForce > 0) {
			radialForce = radialForce * -1; //inward pull, please :D
		   //if a force remains, we need to construct an inward vector with the force.
			inVec = (center - tPos);
			//subtract off any x-y momentum opposing the pull
			inVec2 = inVec.asPoint2F();
			tMom2 = tMom.asPoint2F();
			inVec2 = inVec2 - tMom2;
			z = inVec.z;
         //set the 3D vector accordingly
			inVec.set(inVec2.x, inVec2.y, z);
			//normalize it
			inVec.normalizeSafe();
			//scale it with the radialForce
			inVec *= radialForce;
			//apply the radial force vector to the target
			sql.mList[i]->applyImpulse(tPos, inVec);
		}
   }
}

//-----------------------------------------------------------------------------
// packUpdate
//-----------------------------------------------------------------------------
U32 Tornado::packUpdate(NetConnection* con, U32 mask, BitStream* stream) {
   U32 retMask = Parent::packUpdate(con, mask, stream);

   if ( stream->writeFlag( mask & InitialUpdateMask ) ) {
      mathWrite(*stream, getTransform());
      mathWrite(*stream, getScale());
   }

   if ( stream->writeFlag( mask & EmitterDBMask ) ) {
      if( stream->writeFlag(mEmitterDatablock != NULL) ) {
         stream->writeRangedU32(mEmitterDatablock->getId(), DataBlockObjectIdFirst,
            DataBlockObjectIdLast);
      }
   }

   if ( stream->writeFlag( mask & StateMask ) ) {
      stream->writeFlag( mActive );
      stream->write( mVelocity );
		stream->write( mStrength );
		stream->write( mTornadoRadius );
		stream->write( mEdgeRadius );
   }

   return retMask;
}

//-----------------------------------------------------------------------------
// unpackUpdate
//-----------------------------------------------------------------------------
void Tornado::unpackUpdate(NetConnection* con, BitStream* stream) {
   Parent::unpackUpdate(con, stream);

   if ( stream->readFlag() ) {
      MatrixF temp;
      Point3F tempScale;
      mathRead(*stream, &temp);
      mathRead(*stream, &tempScale);

      setScale(tempScale);
      setTransform(temp);
   }

   if ( stream->readFlag() ) {
      mEmitterDatablockId = stream->readFlag() ?
         stream->readRangedU32(DataBlockObjectIdFirst, DataBlockObjectIdLast) : 0;

      ParticleEmitterData *emitterDB = NULL;
      Sim::findObject( mEmitterDatablockId, emitterDB );
      if ( isProperlyAdded() ) {
         setEmitterDataBlock( emitterDB );
	  }
   }

   if ( stream->readFlag() ) {
      mActive = stream->readFlag();
      stream->read( &mVelocity );
		stream->read( &mStrength );
		stream->read( &mTornadoRadius );
		stream->read( &mEdgeRadius );
   }
}

void Tornado::setEmitterDataBlock(ParticleEmitterData* data) {
   if ( isServerObject() ) {
      setMaskBits( EmitterDBMask );
   }
   else {
      ParticleEmitter* pEmitter = NULL;
      if ( data ) {
         // Create emitter with new datablock
         pEmitter = new ParticleEmitter;
         pEmitter->onNewDataBlock( data, false );
         if( pEmitter->registerObject() == false ) {
            Con::warnf(ConsoleLogEntry::General, "Could not register base emitter for particle of class: %s", data->getName() ? data->getName() : data->getIdString() );
            delete pEmitter;
            return;
         }
      }

      // Replace emitter
      if ( mEmitter ) {
         mEmitter->deleteWhenEmpty();
	  }

      mEmitter = pEmitter;
   }

   mEmitterDatablock = data;
}
 
DefineEngineMethod(Tornado, setEmitterDataBlock, void, (ParticleEmitterData* emitterDatablock), (0),
   "Assigns the datablock for this emitter node.\n"
   "@param emitterDatablock ParticleEmitterData datablock to assign\n"
   "@tsexample\n"
   "// Assign a new emitter datablock\n"
   "%emitter.setEmitterDatablock( %emitterDatablock );\n"
   "@endtsexample\n" ) {
   if ( !emitterDatablock ) {
      Con::errorf("ParticleEmitterData datablock could not be found when calling setEmitterDataBlock in Tornado.");
      return;
   }

   object->setEmitterDataBlock(emitterDatablock);
}

DefineEngineMethod(Tornado, setActive, void, (bool active),,
   "Turns the emitter on or off.\n"
   "@param active New emitter state\n" ) {
   object->setActive( active );
}
