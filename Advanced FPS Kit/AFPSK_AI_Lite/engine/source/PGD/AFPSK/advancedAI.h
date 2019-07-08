//-----------------------------------------------------------------------------
// advancedAI.h
// (C) 2014 Robert Fritzen for Phantom Games Development
// Header file for the new AI player for the Advanced FPS Kit for T3D MIT 3.5
//-----------------------------------------------------------------------------

/**
* Thanks Go Out to Steve Acaster for the updated AIPoses Resource, this has been implemented in this AIPlayer Class
* Please make the following code changes to your engine to allow the AIPose code to work.
* ShapeBase.cpp:
ShapeBase::ShapeBase()
 : mDrag( 0.0f ),
   mBuoyancy( 0.0f ),
   mWaterCoverage( 0.0f ),
   ...
   mMoveMotion( false ),
   mIsAiControlled( false ),//added comma yorks
	mAiPose( 0 )//added yorks
	{ ...

* ShapeBase.h:
   F32 mAiPose; //yorks new  
   F32 mEnergy;                     ///< Current enery level.  
   F32 mRechargeRate;               ///< Energy recharge rate (in units/tick).  

* Player.cpp:
   //at the end of UpdateMove() function   
   //...  
   // Update the PlayerPose  
   Pose desiredPose = mPose;  
      
    if (!mIsAiControlled)//yorks new - leave the AiPlayer alone!  
    {  
       if ( mSwimming )  
          desiredPose = SwimPose;   
       else if ( runSurface && move->trigger[sCrouchTrigger] && canCrouch() )       
          desiredPose = CrouchPose;  
       else if ( runSurface && move->trigger[sProneTrigger] && canProne() )  
          desiredPose = PronePose;  
       else if ( move->trigger[sSprintTrigger] && canSprint() )  
          desiredPose = SprintPose;  
       else if ( canStand() )  
          desiredPose = StandPose;  
  
          setPose( desiredPose );  
    }//yorks new  


   //in packUpdate() function  
   //...  
   if (stream->writeFlag(mask & MoveMask))  
   {  
      stream->writeFlag(mFalling);  
  
      stream->writeFlag(mSwimming);//yorks start  
      stream->writeFlag(mJetting);  
      stream->writeInt(mPose, NumPoseBits);//yorks end  
  
      stream->writeInt(mState,NumStateBits);  
      if (stream->writeFlag(mState == RecoverState))  
   //...  


   //in unPackUpdate() function  
   //...  
   // MoveMask  
   if (stream->readFlag()) {  
      mPredictionCount = sMaxPredictionTicks;  
      mFalling = stream->readFlag();  
        
      //calls to update animations correctly  
      mSwimming = stream->readFlag();//yorks start  
      mJetting = stream->readFlag();  
      mPose = (Pose)(stream->readInt(NumPoseBits));  
      //yorks end  
      ActionState actionState = (ActionState)stream->readInt(NumStateBits);  
      if (stream->readFlag()) {  
         mRecoverTicks = stream->readInt(PlayerData::RecoverDelayBits);  
         setState(actionState, mRecoverTicks);  
      }  
      else  
         setState(actionState);  
      Point3F pos,rot;  
   //...  
**/

/**
* Addition Note #2:

Please make sure your build of Torque 3D MIT 3.5 has the "Navigation" module enabled, so Recast pathing may function properly.
You also need to add a little bit of code to navigation/navPath.cpp/.h

* navPath.cpp: (Add this code)
void NavPath::setNavMesh(NavMesh *mesh) {
	if(mesh) {
		mMesh = mesh;
	}
}

* navPath.h:
	//in the public: section after bool finalize():
	void setNavMesh(NavMesh *mesh);
**/

#ifndef _ADVAIPLAYER_H_
#define _ADVAIPLAYER_H_

#include "T3D/player.h"
#include "navigation/navMesh.h"
#include "navigation/navPath.h"
#include "console/consoleInternal.h"
#include "math/mMatrix.h"
#include "T3D/gameBase/moveManager.h"
#include "console/engineAPI.h"
#include "scene/sceneManager.h"

//The new AI Class
class AdvancedAIPlayer : public Player {

	typedef Player Parent;

   public:
	   enum MoveState {
		   ModeStop,                       // AI has stopped moving.
		   ModeMove,                       // AI is currently moving.
		   ModeStuck,                      // AI is stuck, but wants to move.
         ModeSlowing,                    // AI is slowing down as it reaches it's destination.
	   };

   private:
      SimObjectPtr<GameBase> mAimObject;  // Object to point at, overrides location
	   SimObjectPtr<NavMesh> mCurrNavMesh; // The current navigation mesh this AI is using
	   NavPath *mPath;                     // AI Specific Nav-Path.

      MoveState mMoveState;
      F32 mMoveSpeed;
      F32 mMoveTolerance;                 // Distance from destination before we stop
      Point3F mMoveDestination;           // Destination for movement
      Point3F mLastLocation;              // For stuck check
		Point3F mFinDest;
      F32 mMoveStuckTolerance;            // Distance tolerance on stuck check
      S32 mMoveStuckTestDelay;            // The number of ticks to wait before checking if the AI is stuck
      S32 mMoveStuckTestCountdown;        // The current countdown until at AI starts to check if it is stuck
		S32 mCurrentNode;
      bool mMoveSlowdown;                 // Slowdown as we near the destination
		bool usingNewMove;

      bool mAimLocationSet;               // Has an aim location been set?
      Point3F mAimLocation;               // Point to look at
      bool mTargetInLOS;                  // Is target object visible?

      Point3F mAimOffset;

      // Utility Methods
      void throwCallback( const char *name );

   public:
      DECLARE_CONOBJECT( AdvancedAIPlayer );

      AdvancedAIPlayer();
      ~AdvancedAIPlayer();

	   void moveTo(Point3F dest, bool slowdown);

      static void initPersistFields();

      bool onAdd();

	   void setCurrentNavMesh(S32 meshID);
	   NavPath *getAIPathObj() { return (mPath == NULL ? NULL : mPath); }
	   //These two functions can also pin down AI Players, keep that in mind.
	   ShapeBase *getNearestObject(U32 typeMask);
	   ShapeBase *getNearestEnemyObject(U32 typeMask);
	   ShapeBase *getNearestObjectInView(U32 typeMask);
	   ShapeBase *getNearestEnemyObjectInView(U32 typeMask);

		bool hasLosTo(ShapeBase *obj);

      virtual bool getAIMove( Move *move );

      // Targeting and aiming sets/gets
      void setAimObject( GameBase *targetObject );
      void setAimObject( GameBase *targetObject, Point3F offset );
      GameBase* getAimObject() const  { return mAimObject; }
      void setAimLocation( const Point3F &location );
      Point3F getAimLocation() const { return mAimLocation; }
      void clearAim();

      // Movement sets/gets
      void setMoveSpeed( const F32 speed );
      F32 getMoveSpeed() const { return mMoveSpeed; }
      void setMoveTolerance( const F32 tolerance );
      F32 getMoveTolerance() const { return mMoveTolerance; }
      void setMoveDestination( const Point3F &location, bool slowdown );
      Point3F getMoveDestination() const { return mMoveDestination; }
      void stopMove();

	   void setAiPose( const F32 pose );//yorks  
      F32  getAiPose();//yorks  
		void changePose(S32 poseNumber);
};

#endif