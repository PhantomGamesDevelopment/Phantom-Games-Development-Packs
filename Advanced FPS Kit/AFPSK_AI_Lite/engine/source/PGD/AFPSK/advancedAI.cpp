//-----------------------------------------------------------------------------
// advancedAI.cpp
// (C) 2014 Robert Fritzen for Phantom Games Development
// Source file for the advanced AI for the Advanced FPS Kit for T3D MIT 3.5
// * Please see addition notes in the header file.
// * This is the AI LITE Version of AFPSK AI.
//-----------------------------------------------------------------------------

#include "platform/platform.h"
#include "PGD/AFPSK/advancedAI.h"

IMPLEMENT_CO_NETOBJECT_V1(AdvancedAIPlayer);

/**
 * Constructor
 */
AdvancedAIPlayer::AdvancedAIPlayer() {
	mCurrNavMesh = 0;
	mPath = NULL;

   mMoveDestination.set( 0.0f, 0.0f, 0.0f );
   mMoveSpeed = 1.0f;
   mMoveTolerance = 0.25f;
   mMoveStuckTolerance = 0.01f;
   mMoveStuckTestDelay = 30;
   mMoveStuckTestCountdown = 0;
   mMoveSlowdown = true;
	usingNewMove = false;
   mMoveState = ModeStop;
	mCurrentNode = 0;

   mAimObject = 0;
   mAimLocationSet = false;
   mTargetInLOS = false;
   mAimOffset = Point3F(0.0f, 0.0f, 0.0f);

   mIsAiControlled = true;
}

/**
 * Destructor
 */
AdvancedAIPlayer::~AdvancedAIPlayer() {
	if(mPath != NULL) {
	   delete mPath;
	}
}

void AdvancedAIPlayer::initPersistFields() {
   addGroup( "AI" );

      addField( "mMoveTolerance", TypeF32, Offset( mMoveTolerance, AdvancedAIPlayer ), 
         "@brief Distance from destination before stopping.\n\n"
         "When the AIPlayer is moving to a given destination it will move to within "
         "this distance of the destination and then stop.  By providing this tolerance "
         "it helps the AIPlayer from never reaching its destination due to minor obstacles, "
         "rounding errors on its position calculation, etc.  By default it is set to 0.25.\n");

      addField( "moveStuckTolerance", TypeF32, Offset( mMoveStuckTolerance, AdvancedAIPlayer ), 
         "@brief Distance tolerance on stuck check.\n\n"
         "When the AIPlayer is moving to a given destination, if it ever moves less than "
         "this tolerance during a single tick, the AIPlayer is considered stuck.  At this point "
         "the onMoveStuck() callback is called on the datablock.\n");

      addField( "moveStuckTestDelay", TypeS32, Offset( mMoveStuckTestDelay, AdvancedAIPlayer ), 
         "@brief The number of ticks to wait before testing if the AIPlayer is stuck.\n\n"
         "When the AIPlayer is asked to move, this property is the number of ticks to wait "
         "before the AIPlayer starts to check if it is stuck.  This delay allows the AIPlayer "
         "to accelerate to full speed without its initial slow start being considered as stuck.\n"
         "@note Set to zero to have the stuck test start immediately.\n");

   endGroup( "AI" );

   Parent::initPersistFields();
}

bool AdvancedAIPlayer::onAdd() {
   if (!Parent::onAdd())
      return false;

   MatrixF eye;
   getEyeTransform(&eye);
   mLastLocation = eye.getPosition();

   return true;
}
/**
  Old Code Used From Original AIPlayer.cpp
 **/
void AdvancedAIPlayer::setMoveSpeed( F32 speed ) {
   mMoveSpeed = getMax(0.0f, getMin( 1.0f, speed ));
}

void AdvancedAIPlayer::stopMove() {
   mMoveState = ModeStop;
}

void AdvancedAIPlayer::setMoveTolerance( const F32 tolerance ) {
   mMoveTolerance = getMax( 0.1f, tolerance );
}

void AdvancedAIPlayer::setMoveDestination( const Point3F &location, bool slowdown ) {
   mMoveDestination = location;
   mMoveState = ModeMove;
   mMoveSlowdown = slowdown;
   mMoveStuckTestCountdown = mMoveStuckTestDelay;
	usingNewMove = false;
}

void AdvancedAIPlayer::setAimObject( GameBase *targetObject ) {
   mAimObject = targetObject;
   mTargetInLOS = false;
   mAimOffset = Point3F(0.0f, 0.0f, 0.0f);
}

void AdvancedAIPlayer::setAimObject( GameBase *targetObject, Point3F offset ) {
   mAimObject = targetObject;
   mTargetInLOS = false;
   mAimOffset = offset;
}

void AdvancedAIPlayer::setAimLocation( const Point3F &location ) {
   mAimObject = 0;
   mAimLocationSet = true;
   mAimLocation = location;
   mAimOffset = Point3F(0.0f, 0.0f, 0.0f);
}

void AdvancedAIPlayer::clearAim() {
   mAimObject = 0;
   mAimLocationSet = false;
   mAimOffset = Point3F(0.0f, 0.0f, 0.0f);
}

bool AdvancedAIPlayer::getAIMove(Move *movePtr) {
   *movePtr = NullMove;

   // Use the eye as the current position.
   MatrixF eye;
   getEyeTransform(&eye);
   Point3F location = eye.getPosition();
   Point3F rotation = getRotation();

   // Orient towards the aim point, aim object, or towards
   // our destination.
   if (mAimObject || mAimLocationSet || mMoveState != ModeStop)  {
      // Update the aim position if we're aiming for an object
		if (mAimObject) {
         mAimLocation = mAimObject->getPosition() + mAimOffset;
		}
		else {
			if (!mAimLocationSet) {
            mAimLocation = mMoveDestination;
			}
		}

      F32 xDiff = mAimLocation.x - location.x;
      F32 yDiff = mAimLocation.y - location.y;

      if (!mIsZero(xDiff) || !mIsZero(yDiff)) {
         // First do Yaw
         // use the cur yaw between -Pi and Pi
         F32 curYaw = rotation.z;
         while (curYaw > M_2PI_F)
            curYaw -= M_2PI_F;
         while (curYaw < -M_2PI_F)
            curYaw += M_2PI_F;

         // find the yaw offset
         F32 newYaw = mAtan2( xDiff, yDiff );
         F32 yawDiff = newYaw - curYaw;

         // make it between 0 and 2PI
         if( yawDiff < 0.0f )
            yawDiff += M_2PI_F;
         else if( yawDiff >= M_2PI_F )
            yawDiff -= M_2PI_F;

         // now make sure we take the short way around the circle
         if( yawDiff > M_PI_F )
            yawDiff -= M_2PI_F;
         else if( yawDiff < -M_PI_F )
            yawDiff += M_2PI_F;

         movePtr->yaw = yawDiff;

         // Next do pitch.
         if (!mAimObject && !mAimLocationSet) {
            // Level out if were just looking at our next way point.
            Point3F headRotation = getHeadRotation();
            movePtr->pitch = -headRotation.x;
         }
         else {
            // This should be adjusted to run from the
            // eye point to the object's center position. Though this
            // works well enough for now.
            F32 vertDist = mAimLocation.z - location.z;
            F32 horzDist = mSqrt(xDiff * xDiff + yDiff * yDiff);
            F32 newPitch = mAtan2( horzDist, vertDist ) - ( M_PI_F / 2.0f );
            if (mFabs(newPitch) > 0.01f) {
               Point3F headRotation = getHeadRotation();
               movePtr->pitch = newPitch - headRotation.x;
            }
         }
      }
   }
   else {
      // Level out if we're not doing anything else
      Point3F headRotation = getHeadRotation();
      movePtr->pitch = -headRotation.x;
   }

   // Move towards the destination
   if (mMoveState != ModeStop) {
      F32 xDiff = mMoveDestination.x - location.x;
      F32 yDiff = mMoveDestination.y - location.y;

      // Check if we should mMove, or if we are 'close enough'
      if (mFabs(xDiff) < mMoveTolerance && mFabs(yDiff) < mMoveTolerance) {
         mMoveState = ModeStop;
			if(usingNewMove) {
				//If we're using recast, check to see if we're at the last node, otherwie proceed.
				if((mFinDest - mPath->getNode(mCurrentNode)).len() <= mMoveTolerance) {
				   throwCallback("onReachDestination");
				}
				else {
					if(mCurrentNode > mPath->getCount()) {
						Con::printf("AdvAI: Dest. Reach on node override.");
					   throwCallback("onReachDestination");
					}
					else {
				      mCurrentNode++;
					   mMoveDestination = mPath->getNode(mCurrentNode);
					   mMoveState = ModeMove;
					}
				}
			}
			else {
            throwCallback("onReachDestination");
			}
      }
      else {
         // Build move direction in world space
         if (mIsZero(xDiff))
            movePtr->y = (location.y > mMoveDestination.y) ? -1.0f : 1.0f;
         else
            if (mIsZero(yDiff))
               movePtr->x = (location.x > mMoveDestination.x) ? -1.0f : 1.0f;
            else
               if (mFabs(xDiff) > mFabs(yDiff)) {
                  F32 value = mFabs(yDiff / xDiff);
                  movePtr->y = (location.y > mMoveDestination.y) ? -value : value;
                  movePtr->x = (location.x > mMoveDestination.x) ? -1.0f : 1.0f;
               }
               else {
                  F32 value = mFabs(xDiff / yDiff);
                  movePtr->x = (location.x > mMoveDestination.x) ? -value : value;
                  movePtr->y = (location.y > mMoveDestination.y) ? -1.0f : 1.0f;
               }

         // Rotate the move into object space (this really only needs
         // a 2D matrix)
         Point3F newMove;
         MatrixF moveMatrix;
         moveMatrix.set(EulerF(0.0f, 0.0f, -(rotation.z + movePtr->yaw)));
         moveMatrix.mulV( Point3F( movePtr->x, movePtr->y, 0.0f ), &newMove );
         movePtr->x = newMove.x;
         movePtr->y = newMove.y;

         // Set movement speed.  We'll slow down once we get close
         // to try and stop on the spot...
         if (mMoveSlowdown) {
            F32 speed = mMoveSpeed;
            F32 dist = mSqrt(xDiff*xDiff + yDiff*yDiff);
            F32 maxDist = 5.0f;
            if (dist < maxDist)
               speed *= dist / maxDist;
            movePtr->x *= speed;
            movePtr->y *= speed;

            mMoveState = ModeSlowing;
         }
         else {
            movePtr->x *= mMoveSpeed;
            movePtr->y *= mMoveSpeed;

            mMoveState = ModeMove;
         }

         if (mMoveStuckTestCountdown > 0)
            --mMoveStuckTestCountdown;
         else {
            // We should check to see if we are stuck...
            F32 locationDelta = (location - mLastLocation).len();
            if (locationDelta < mMoveStuckTolerance && mDamageState == Enabled) {
               // If we are slowing down, then it's likely that our location delta will be less than
               // our move stuck tolerance. Because we can be both slowing and stuck
               // we should TRY to check if we've moved. This could use better detection.
               if ( mMoveState != ModeSlowing || locationDelta == 0 ) {
                  mMoveState = ModeStuck;
                  throwCallback("onMoveStuck");
               }
            }
         }
      }
   }

   // Test for target location in sight if it's an object. The LOS is
   // run from the eye position to the center of the object's bounding,
   // which is not very accurate.
   if (mAimObject) {
      MatrixF eyeMat;
      getEyeTransform(&eyeMat);
      eyeMat.getColumn(3,&location);
      Point3F targetLoc = mAimObject->getBoxCenter();

      // This ray ignores non-static shapes. Cast Ray returns true
      // if it hit something.
      RayInfo dummy;
      if (getContainer()->castRay( location, targetLoc,
            StaticShapeObjectType | StaticObjectType |
            TerrainObjectType, &dummy)) {
         if (mTargetInLOS) {
            throwCallback( "onTargetExitLOS" );
            mTargetInLOS = false;
         }
      }
      else
         if (!mTargetInLOS) {
            throwCallback( "onTargetEnterLOS" );
            mTargetInLOS = true;
         }
   }

   // Replicate the trigger state into the move so that
   // triggers can be controlled from scripts.
   for( int i = 0; i < MaxTriggerKeys; i++ )
      movePtr->trigger[i] = getImageTriggerState(i);
  
    switch (mPose) {  
       case StandPose:  
            movePtr->trigger[3] = false;    // crouch
            movePtr->trigger[4] = false;    // prone
            movePtr->trigger[5] = false;    // sprint
            break;  
       case CrouchPose:                  
            movePtr->trigger[3] = true;      // crouch  
            movePtr->trigger[4] = false;     // prone
            movePtr->trigger[5] = false;     // sprint
            break;  
       case PronePose:  
            movePtr->trigger[3] = false;     // crouch
            movePtr->trigger[4] = true;      // prone    
            movePtr->trigger[5] = false;     // sprint
            break;
       case SprintPose:
            movePtr->trigger[3] = false;     // crouch
            movePtr->trigger[4] = false;     // prone
            movePtr->trigger[5] = true;      // sprint
            break;
    }  
   mLastLocation = location;

   return true;
}

/**
 Begin Advanced AI Code
**/
/**
 * AI Targeting Functions
*/
ShapeBase *AdvancedAIPlayer::getNearestObject(U32 typeMask) {
   SimSet * missionGroup = dynamic_cast<SimSet*>(Sim::findObject("MissionCleanup"));
   if (!missionGroup) {
      Con::errorf(ConsoleLogEntry::Script, "getNearestObject(): no mission cleanup found");
      return NULL;
   }
	F32 closest = 999999;
	SceneObject *test = NULL;
	ShapeBase *target = NULL;
	//We use a radius search to find targets
	//Note: For future: You may want to insert a F32 radius argument to replace 99999 for projects using a heavy object search load.
	gServerContainer.initRadiusSearch(getPosition(), 99999, typeMask);
	while((test = gServerContainer.containerSearchNextObject()) != NULL) {
		if(dynamic_cast<ShapeBase *>(test)->isDestroyed()) {
		   //Don't hunt out destroyed stuff.
			continue;
		}
		if(test != this) {
		   if((test->getPosition() - getPosition()).len() < closest) {
		      closest = (test->getPosition() - getPosition()).len();
			   target = dynamic_cast<ShapeBase *>(test);
		   }
		}
	}

	return target;
}

ShapeBase *AdvancedAIPlayer::getNearestEnemyObject(U32 typeMask) {
   SimSet * missionGroup = dynamic_cast<SimSet*>(Sim::findObject("MissionCleanup"));
   if (!missionGroup) {
      Con::errorf(ConsoleLogEntry::Script, "getNearestEnemyObject(): no mission cleanup found");
      return NULL;
   }

	F32 closest = 999999;
	SceneObject *test = NULL;
	ShapeBase *target = NULL;
	//We use a radius search to find targets
	//Note: For future: You may want to insert a F32 radius argument to replace 99999 for projects using a heavy object search load.
	gServerContainer.initRadiusSearch(getPosition(), 99999, typeMask);
	while((test = gServerContainer.containerSearchNextObject()) != NULL) {
		if(dynamic_cast<ShapeBase *>(test)->isDestroyed()) {
		   //Don't hunt out destroyed stuff.
			continue;
		}
		if(test != this && (getTeam() != dynamic_cast<ShapeBase *>(test)->getTeam())) {
		   if((test->getPosition() - getPosition()).len() < closest) {
		      closest = (test->getPosition() - getPosition()).len();
			   target = dynamic_cast<ShapeBase *>(test);
		   }
		}
	}

	return target;
}

ShapeBase *AdvancedAIPlayer::getNearestObjectInView(U32 typeMask) {
   SimSet * missionGroup = dynamic_cast<SimSet*>(Sim::findObject("MissionCleanup"));
   if (!missionGroup) {
      Con::errorf(ConsoleLogEntry::Script, "getNearestObject(): no mission cleanup found");
      return NULL;
   }
	F32 closest = 999999;
	SceneObject *test = NULL;
	ShapeBase *target = NULL;
	//We use a radius search to find targets
	//Note: For future: You may want to insert a F32 radius argument to replace 99999 for projects using a heavy object search load.
	gServerContainer.initRadiusSearch(getPosition(), 99999, typeMask);
	while((test = gServerContainer.containerSearchNextObject()) != NULL) {
		if(dynamic_cast<ShapeBase *>(test)->isDestroyed()) {
		   //Don't hunt out destroyed stuff.
			continue;
		}
		if(test != this) {
		   if((test->getPosition() - getPosition()).len() < closest) {
				if(hasLosTo(dynamic_cast<ShapeBase *>(test))) {
		         closest = (test->getPosition() - getPosition()).len();
			      target = dynamic_cast<ShapeBase *>(test);
				}
		   }
		}
	}

	return target;
}

ShapeBase *AdvancedAIPlayer::getNearestEnemyObjectInView(U32 typeMask) {
   SimSet * missionGroup = dynamic_cast<SimSet*>(Sim::findObject("MissionCleanup"));
   if (!missionGroup) {
      Con::errorf(ConsoleLogEntry::Script, "getNearestEnemyObject(): no mission cleanup found");
      return NULL;
   }

	F32 closest = 999999;
	SceneObject *test = NULL;
	ShapeBase *target = NULL;
	//We use a radius search to find targets
	//Note: For future: You may want to insert a F32 radius argument to replace 99999 for projects using a heavy object search load.
	gServerContainer.initRadiusSearch(getPosition(), 99999, typeMask);
	while((test = gServerContainer.containerSearchNextObject()) != NULL) {
		if(dynamic_cast<ShapeBase *>(test)->isDestroyed()) {
		   //Don't hunt out destroyed stuff.
			continue;
		}
		if(test != this && (getTeam() != dynamic_cast<ShapeBase *>(test)->getTeam())) {
		   if((test->getPosition() - getPosition()).len() < closest) {
				if(hasLosTo(dynamic_cast<ShapeBase *>(test))) {
		         closest = (test->getPosition() - getPosition()).len();
			      target = dynamic_cast<ShapeBase *>(test);
				}
		   }
		}
	}

	return target;
}

bool AdvancedAIPlayer::hasLosTo(ShapeBase *obj) {
	if(obj == NULL) {
	   return false;
	}
	if(obj->isDestroyed()) {
	   return false; //Dead.
	}
   S32 trgID = obj->getId();

   Point3F startPos, endPos;
	MatrixF eye;
	
	getEyeTransform(&eye);
	startPos = eye.getPosition();

	if(obj->getTypeMask() & PlayerObjectType) {
	   obj->getEyeTransform(&eye);
		endPos = eye.getPosition();
	}
	else {
	   obj->getWorldBox().getCenter(&endPos);
	}

	U32 colMask = TerrainObjectType | StaticObjectType | obj->getTypeMask();

   RayInfo rinfo;
	disableCollision();
	if(gServerContainer.castRay(startPos, endPos, colMask, &rinfo) == true) {
		enableCollision();
		if(rinfo.object == NULL) {
		   return false;
		}
		return rinfo.object->getId() == trgID;
	}
	enableCollision();
	return false;
}

void AdvancedAIPlayer::setCurrentNavMesh(S32 meshID) {
	if(!meshID) {
		Con::errorf("setCurrentNavMesh(): Invalid Nav Mesh ID");
		if(mPath) {
			delete mPath;
			mPath = NULL;
		}
		return;
	}
	SimObject *object = Sim::findObject(meshID);
	if(object) {
		if(dStrcmp(object->getClassName(), "NavMesh") == 0) {
			mCurrNavMesh = dynamic_cast<NavMesh *>(object);
			//Create our nav-path now
			if(!mPath) {
				mPath = new NavPath();
				mPath->setNavMesh(mCurrNavMesh);
			}
		}
		else {
			Con::errorf("setCurrentNavMesh(): Provided Object is not a Navigation Mesh Object");
			if(mPath) {
				delete mPath;
				mPath = NULL;
			}
		}
	}
}

//This is our general "move" function, basically call this, and the AI will plan out a path
// to the target location, and follow the path.
void AdvancedAIPlayer::moveTo(Point3F dest, bool slowdown) {
	//
	if(mPath == NULL) {
		Con::errorf("AdvancedAIPlayer() - Cannot use moveTo without a valid path object, please call setCurrentNavMesh() first.");
		return;
	}
	mFinDest = dest;
	mPath->mFrom = getPosition();
	mPath->mFromSet = true;
   mPath->mTo = dest;  
	mPath->mToSet = true;
	if(mPath->plan()) {
	   usingNewMove = true;
		mCurrentNode = 0;
	   // Move to the first node.
      mMoveDestination = mPath->getNode(mCurrentNode);
      mMoveState = ModeMove;
      mMoveStuckTestCountdown = mMoveStuckTestDelay;	 
		mMoveSlowdown = slowdown;
	}
}

//yorks start 
void AdvancedAIPlayer::changePose(S32 poseNumber) {    
   Pose mPose = StandPose;  // default  
 
   switch (poseNumber) {  
      case 0: 
		   mPose = StandPose;    break;    //   { Player::StandPose,    "Stand",    "Standard movement pose.n" },  
      case 1: 
			mPose = SprintPose;   break;    //   { Player::SprintPose,   "Sprint",   "Sprinting pose.n" },  
      case 2: 
			mPose = CrouchPose;   break;    //   { Player::CrouchPose,   "Crouch",   "Crouch pose.n" },  
      case 3: 
			mPose = PronePose;    break;    //   { Player::PronePose,    "Prone",    "Prone pose.n" },  
      case 4: 
			mPose = SwimPose;     break;    //   { Player::SwimPose,     "Swim",     "Swimming pose.n" },  
   }  
	setPose(mPose);    
}  

void AdvancedAIPlayer::setAiPose( F32 pose ) {  
   mAiPose = pose;  
}  
  
F32 AdvancedAIPlayer::getAiPose() {  
   return mAiPose;   
}  
//yorks end  
   
/**
 * Utility function to throw callbacks. Callbacks always occure
 * on the datablock class.
 *
 * @param name Name of script function to call
 */
void AdvancedAIPlayer::throwCallback( const char *name ) {
   Con::executef(getDataBlock(), name, getIdString());
}

// --------------------------------------------------------------------------------------------
// Console Functions
// --------------------------------------------------------------------------------------------

DefineEngineMethod( AdvancedAIPlayer, getClosestObject, S32, (U32 typeMask, bool viewReq), ( false), "(U32 t, bool needLos) Returns the closest object, regardless of team of typemask t") {
   ShapeBase *c = NULL;
	c = viewReq ? object->getNearestObjectInView(typeMask) : object->getNearestObject(typeMask);
	return (c == NULL ? -1 : c->getId());
}

DefineEngineMethod( AdvancedAIPlayer, getClosestEnemyObject, S32, (U32 typeMask, bool viewReq), ( false), "(U32 t, bool needLos) Returns the closest enemy object, of typemask t") {
   ShapeBase *c = NULL;
	c = viewReq? object->getNearestEnemyObjectInView(typeMask) : object->getNearestEnemyObject(typeMask);
	return (c == NULL ? -1 : c->getId());
}

DefineEngineMethod( AdvancedAIPlayer, setCurrentNavMesh, bool, (S32 meshObj),, "(S32 mesh) Sets the AI Player's navMesh object and initializes the AI Path.") {
	object->setCurrentNavMesh(meshObj);  
	return object->getAIPathObj() != NULL;
}

DefineEngineMethod( AdvancedAIPlayer, hasLOSTo, bool, (ShapeBase *obj),, "(Obj) Test if the AI has a LOS view on obj") {
	if(obj == NULL) {
		Con::errorf("AdvAI: Cannot perform hasLOSTo using invalid object.");
	   return false;
	}
   return object->hasLosTo(obj);
}

DefineEngineMethod( AdvancedAIPlayer, setMoveDestination, void, ( Point3F goal, bool slowDown ), ( true ), "This is the new move function, same args as the old one.") {
   object->moveTo(goal, slowDown);
}

DefineEngineMethod( AdvancedAIPlayer, stop, void, ( ),,
   "@brief Tells the AIPlayer to stop moving.\n\n") {
   object->stopMove();
}

DefineEngineMethod( AdvancedAIPlayer, clearAim, void, ( ),,
   "@brief Use this to stop aiming at an object or a point.\n\n"
   
   "@see setAimLocation()\n"
   "@see setAimObject()\n")
{
   object->clearAim();
}

DefineEngineMethod( AdvancedAIPlayer, setMoveSpeed, void, ( F32 speed ),,
   "@brief Sets the move speed for an AI object.\n\n"

   "@param speed A speed multiplier between 0.0 and 1.0.  "
   "This is multiplied by the AIPlayer's base movement rates (as defined in "
   "its PlayerData datablock)\n\n"
   
   "@see getMoveDestination()\n")
{
	object->setMoveSpeed(speed);
}

DefineEngineMethod( AdvancedAIPlayer, getMoveSpeed, F32, ( ),,
   "@brief Gets the move speed of an AI object.\n\n"

   "@return A speed multiplier between 0.0 and 1.0.\n\n"

   "@see setMoveSpeed()\n")
{
   return object->getMoveSpeed();
}

DefineEngineMethod( AdvancedAIPlayer, setMoveDestination_old, void, ( Point3F goal, bool slowDown ), ( true ),
   "@brief Tells the AI to move to the location provided\n\n"

   "@param goal Coordinates in world space representing location to move to.\n"
   "@param slowDown A boolean value. If set to true, the bot will slow down "
   "when it gets within 5-meters of its move destination. If false, the bot "
   "will stop abruptly when it reaches the move destination. By default, this is true.\n\n"

   "@note Upon reaching a move destination, the bot will clear its move destination and "
   "calls to getMoveDestination will return \"0 0 0\"."

	"This function is the standard AIPlayer setMoveDestination function, it has no input to the new code."
   
   "@see getMoveDestination()\n")
{
   object->setMoveDestination(goal, slowDown);
}

DefineEngineMethod( AdvancedAIPlayer, getMoveDestination, Point3F, (),,
   "@brief Get the AIPlayer's current destination.\n\n"

   "@return Returns a point containing the \"x y z\" position "
   "of the AIPlayer's current move destination. If no move destination "
   "has yet been set, this returns \"0 0 0\"."
   
   "@see setMoveDestination()\n")
{
	return object->getMoveDestination();
}

DefineEngineMethod( AdvancedAIPlayer, setAimLocation, void, ( Point3F target ),,
   "@brief Tells the AIPlayer to aim at the location provided.\n\n"

   "@param target An \"x y z\" position in the game world to target.\n\n"
   
   "@see getAimLocation()\n")
{
	object->setAimLocation(target);
}

DefineEngineMethod( AdvancedAIPlayer, getAimLocation, Point3F, (),,
   "@brief Returns the point the AIPlayer is aiming at.\n\n"

   "This will reflect the position set by setAimLocation(), "
   "or the position of the object that the bot is now aiming at.  "
   "If the bot is not aiming at anything, this value will "
   "change to whatever point the bot's current line-of-sight intercepts."

   "@return World space coordinates of the object AI is aiming at. Formatted as \"X Y Z\".\n\n"
   
   "@see setAimLocation()\n"
   "@see setAimObject()\n")
{
	return object->getAimLocation();
}

ConsoleMethod( AdvancedAIPlayer, setAimObject, void, 3, 4, "( GameBase obj, [Point3F offset] )"
              "Sets the bot's target object. Optionally set an offset from target location."
			  "@hide")
{
   Point3F off( 0.0f, 0.0f, 0.0f );

   // Find the target
   GameBase *targetObject;
   if( Sim::findObject( argv[2], targetObject ) )
   {
      if (argc == 4)
         dSscanf( argv[3], "%g %g %g", &off.x, &off.y, &off.z );

      object->setAimObject( targetObject, off );
   }
   else
      object->setAimObject( 0, off );
}

DefineEngineMethod( AdvancedAIPlayer, getAimObject, S32, (),,
   "@brief Gets the object the AIPlayer is targeting.\n\n"

   "@return Returns -1 if no object is being aimed at, "
   "or the SimObjectID of the object the AIPlayer is aiming at.\n\n"
   
   "@see setAimObject()\n")
{
	GameBase* obj = object->getAimObject();
   return obj? obj->getId(): -1;
}

DefineEngineMethod( AdvancedAIPlayer, setAiPose, void, ( F32 pose ),,  
   "@brief Sets the AiPose for an AI object."  
  
   "@param pose StandPose=0,CrouchPose=1,PronePose=2,SprintPose=3.  "  
   "Uses the new AiPose variable from shapebase (as defined in "  
   "its PlayerData datablock)")  
{  
    object->setAiPose(pose);  
}  
  
DefineEngineMethod( AdvancedAIPlayer, getAiPose, F32, (),,  
   "@brief Get the object's current AiPose."  
  
   "@return StandPose=0,CrouchPose=1,PronePose=2,SprintPose=3")  
{  
   return object->getAiPose();  
} 