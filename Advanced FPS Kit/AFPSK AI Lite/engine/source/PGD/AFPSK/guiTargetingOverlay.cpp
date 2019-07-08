//-----------------------------------------------------------------------------
// guiTargetingOverlay.cpp
// (C) 2014 Robert Fritzen for Phantom Games Development
// Source file for the targeting overlay gui for the Advanced FPS Kit for T3D MIT 3.5
//-----------------------------------------------------------------------------

#include "platform/platform.h"
#include "PGD/AFPSK/guiTargetingOverlay.h"
#include "gui/3d/guiTSControl.h"
#include "scene/sceneManager.h"
#include "console/console.h"
#include "console/consoleTypes.h"
#include "console/engineAPI.h"
#include "gfx/gfxDevice.h"
#include "gfx/gfxDrawUtil.h"
#include "math/mathUtils.h"
#include "gfx/gfxFontRenderBatcher.h"
#include "gfx/gfxTransformSaver.h"
#include "gfx/gfxPrimitiveBuffer.h"

//TO-DO: Add new methods directly on the control for lock functions
// Fix any remaining acc. vios.

IMPLEMENT_CONOBJECT(GuiTargetingOverlay);

GuiTargetingOverlay::GuiTargetingOverlay() {
   mPreLockColor.set( 0, 1, 0, 1 );
   mLockedColor.set( 1, 0, 0, 1 );
	maxRange = 0.0f;
	toEnabled = false;
	imgCounter = 0;
	lockingMask = 0;
	imgTicks = 0;
	shapePos.set(0, 0, 0);
   cam.set(EulerF(0, 0, 0));
   camPos.set(0, 0, 0);
   camDir.set(0, 0, 0);
	shapeDir.set(0, 0, 0);
	shapeDist = 0.0f;
	canLock = true;
	texture = NULL;
}

void GuiTargetingOverlay::initPersistFields() {
   addGroup("Colors");     
      addField( "preLockColor",  TypeColorF, Offset( mPreLockColor, GuiTargetingOverlay ), "Color for a target being locked onto." );
      addField( "lockedColor", TypeColorF, Offset( mLockedColor, GuiTargetingOverlay ), "Color for a target that is locked." );
   endGroup("Colors");     
   Parent::initPersistFields();
}

void GuiTargetingOverlay::onRender( Point2I, const RectI &updateRect) {
	if(!toEnabled) {
	   //Only perform ops if the gui is enabled
		return;
	}

	if(!canLock) {
		//If we can't lock, then obviously we need to clear flags
		if(isLocking || lockObject != NULL) {
		   isLocking = false;
		   lockTicks = 0;
         lockObject = NULL;
		}
	   return;
	}
   // Must be in a TS Control
   GuiTSCtrl *parent = dynamic_cast<GuiTSCtrl*>(getParent());
   if (!parent) return;

   // Must have a connection and control object
   GameConnection* conn = GameConnection::getConnectionToServer();
   if (!conn) return;
   GameBase * control = dynamic_cast<GameBase*>(conn->getControlObject());
   if (!control) return;

   // Get control camera info
   conn->getControlCameraTransform(0,&cam);
   cam.getColumn(3, &camPos);
   cam.getColumn(1, &camDir);


   F32 camFov;
   conn->getControlCameraFov(&camFov);
   camFov = mDegToRad(camFov) / 6; //<<< NOTE NOTE NOTE: Adjust the denominator to fit where in the screen targets may be locked.
                                   // 2 = Full Screen, 4 = Half Screen, ect.

   // Collision info. We're going to be running LOS tests and we
   // don't want to collide with the control object.
   static U32 losMask = TerrainObjectType | ShapeBaseObjectType;
   control->disableCollision();

   // All ghosted objects are added to the server connection group,
   // so we can find all the shape base objects by iterating through
   // our current connection.
	if(!isLocking) {
      controlLocked = false;
		imgCounter = 0;
		imgTicks = 0;

      for (SimSetIterator itr(conn); *itr; ++itr) {
         ShapeBase* shape = dynamic_cast< ShapeBase* >(*itr);
         if (shape && (shape->getTypeMask() & lockingMask) && (shape->getTeam() != dynamic_cast<ShapeBase *>(control)->getTeam())) {
            if (shape != control)  {
               // Target pos to test, if it's a player run the LOS to his eye
               // point, otherwise we'll grab the generic box center.
               if (shape->getTypeMask() & PlayerObjectType) {
                  MatrixF eye;
                  // Use the render eye transform, otherwise we'll see jittering
                  shape->getRenderEyeTransform(&eye);
                  eye.getColumn(3, &shapePos);
               } 
               else {
                  // Use the render transform instead of the box center
                  // otherwise it'll jitter.
                  MatrixF srtMat = shape->getRenderTransform();
                  srtMat.getColumn(3, &shapePos);
               }
               shapeDir = shapePos - camPos;

               // Test to see if it's in range
               shapeDist = shapeDir.lenSquared();
				   if (shapeDist == 0 || shapeDist > maxRange) {
                  continue;
				   }
               shapeDist = mSqrt(shapeDist);

               // Test to see if it's within our viewcone, this test doesn't
               // actually match the viewport very well, should consider
               // projection and box test.
               shapeDir.normalize();
               F32 dot = mDot(shapeDir, camDir);
				   if (dot < camFov) {
                  continue;
				   }

               // Test to see if it's behind something, and we want to
               // ignore anything it's mounted on when we run the LOS.
               RayInfo info;
               shape->disableCollision();
               SceneObject *mount = shape->getObjectMount();
			   	if (mount) {
                  mount->disableCollision();
				   }
               bool los = !gClientContainer.castRay(camPos, shapePos, losMask, &info);
               shape->enableCollision();
				   if (mount) {
                  mount->enableCollision();
				   }
				   if (!los) {
                  continue;
				   }

					//Set the target
					lockTicks = 0;
					lockObject = shape;
					isLocking = true;
					sendLockWarnEvent(shape);
					return;
				}
         }
      }
   }
	else {
	   //If we're locking onto a target, increase the lock count until we actually complete the lock process
		if(lockObject == NULL) {
		   //Stops crashing due to Null ref.
			isLocking = false;
			lockTicks = 0;
         lockObject = NULL;
			return;
		}
		if(lockObject->isDestroyed() || lockObject->isDeleted()) {
			//Object is destroyed or is about to be deleted.
			isLocking = false;
			lockTicks = 0;
         lockObject = NULL;
			return;
		}

		controlLocked = isLocked();
      if (lockObject->getTypeMask() & PlayerObjectType) {
         MatrixF eye;
         // Use the render eye transform, otherwise we'll see jittering
         lockObject->getRenderEyeTransform(&eye);
         eye.getColumn(3, &shapePos);
      } 
      else {
         // Use the render transform instead of the box center
         // otherwise it'll jitter.
         MatrixF srtMat = lockObject->getRenderTransform();
         srtMat.getColumn(3, &shapePos);
      }
      shapeDir = shapePos - camPos;

      // Test to see if it's in range
      shapeDist = shapeDir.lenSquared();
		if (shapeDist == 0 || shapeDist > maxRange) {
			sendLockBreakEvent(lockObject);
			return;
		}
      shapeDist = mSqrt(shapeDist);
      // Test to see if it's within our viewcone, this test doesn't
      // actually match the viewport very well, should consider
      // projection and box test.
      shapeDir.normalize();
      F32 dot = mDot(shapeDir, camDir);
		if (dot < camFov) {
			sendLockBreakEvent(lockObject);
			return;
		}
      // Test to see if it's behind something, and we want to
      // ignore anything it's mounted on when we run the LOS.
      RayInfo info;
      lockObject->disableCollision();
      SceneObject *mount = lockObject->getObjectMount();
		if (mount) {
         mount->disableCollision();
		}
      bool los = !gClientContainer.castRay(camPos, shapePos, losMask, &info);
      lockObject->enableCollision();
		if (mount) {
         mount->enableCollision();
		}
		if (!los) {
			sendLockBreakEvent(lockObject);
			return;
		}
		//Otherwise, we're still good, increase the lock tick until we're "locked" on
		if(!isLocked()) {
		   lockTicks++;
			if(isLocked()) {
				//We're locked now.
			   sendCompleteLockEvent(lockObject);
			}
		}
		//Prepare the lock circle.
		Point3F projPnt;
		if (!parent->project(shapePos, &projPnt)) {
			Con::errorf("guiTargetingOverlay: unable to project target position for %i.", lockObject->getId());
         return;
		}
		//
		GFX->getDrawUtil()->clearBitmapModulation();
		//
		if(!isLocked()) {
		   if(lockingImg.compare("") == 0) {
			   control->enableCollision();
			   Con::errorf("guiTargetingOverlay: No Locking Image Set.");
            return;
			}
			//Set the image
			GFX->getDrawUtil()->setBitmapModulation(mPreLockColor);
			circleTexHandle.set(StringUnit::getUnit(lockingImg, imgCounter, "\t\n "), &GFXDefaultGUIProfile, avar("%s() - elementHandle (line %d)", __FUNCTION__, __LINE__)); 
         imgTicks++;
			if(imgTicks >= 5) {
				imgTicks = 0;
			   imgCounter++;
			   if(imgCounter >= StringUnit::getUnitCount(lockingImg, "\t\n ")) {
			      imgCounter = 0;
			   }
			}
		}
		else {
		   if(lockedImg.compare("") == 0) {
		      control->enableCollision();
			   Con::errorf("guiTargetingOverlay: No Locked Image Set.");
            return;
			}	
			//Set the image
			GFX->getDrawUtil()->setBitmapModulation(mLockedColor);
			circleTexHandle.set(StringUnit::getUnit(lockedImg, imgCounter, "\t\n "), &GFXDefaultGUIProfile, avar("%s() - elementHandle (line %d)", __FUNCTION__, __LINE__)); 
         imgTicks++;
			if(imgTicks >= 5) {
				imgTicks = 0;			
			   imgCounter++;
			   if(imgCounter >= StringUnit::getUnitCount(lockedImg, "\t\n ")) {
			      imgCounter = 0;
			   }
			}
		}
		//Check to ensure we have a valid texture:
		if(!circleTexHandle.isNull()) { 
		   texture = circleTexHandle;
		   projPnt.x -= (texture->mBitmapSize.x / 2);
		   projPnt.y -= (texture->mBitmapSize.y / 2);
		   //And Render.
		   GFX->getDrawUtil()->drawBitmapStretchSR(texture, 
			   RectI(Point2I(projPnt.x, projPnt.y), Point2I(texture->mBitmapSize.x, texture->mBitmapSize.y)), 
			   RectI(Point2I(0, 0), Point2I(texture->mBitmapSize.x, texture->mBitmapSize.y)));
		}
	}
   // Restore control object collision
   control->enableCollision();
}

//Event Class
class GTCControlUnlockEvent : public SimEvent {
   public:
      GuiTargetingOverlay *sender;
		GTCControlUnlockEvent(GuiTargetingOverlay *s) {
		   sender = s;
		}
      
		void process(SimObject *object) {
         sender->setLockCap(true);      
      }
};

void GuiTargetingOverlay::sendLockWarnEvent(ShapeBase *shape) {
   //Fetch C.S. Ghost ID
	S32 objectID = NetConnection::getConnectionToServer()->getGhostIndex(dynamic_cast<NetObject *>(shape));
	if(objectID == -1) {
		Con::errorf("sendLockWarnEvent() - Cannot resolve ghost id, object is likely not networked.");
		return;
	}
	//Push
	Con::evaluatef("commandToServer('sendLockWarnEvent', %i);", objectID);
}

void GuiTargetingOverlay::sendCompleteLockEvent(ShapeBase *shape) {
   //Fetch C.S. Ghost ID
	S32 objectID = NetConnection::getConnectionToServer()->getGhostIndex(dynamic_cast<NetObject *>(shape));
	if(objectID == -1) {
		Con::errorf("sendCompleteLockEvent() - Cannot resolve ghost id, object is likely not networked.");
		return;
	}
	//Push
	Con::evaluatef("commandToServer('sendCompleteLockEvent', %i);", objectID);
}

void GuiTargetingOverlay::sendLockBreakEvent(ShapeBase *shape) {
	canLock = false;
	//Post a unlock event in about 3 seconds.
   Sim::postEvent( Sim::getRootGroup(), new GTCControlUnlockEvent(const_cast<GuiTargetingOverlay *>(this)), Sim::getCurrentTime() + 3000 );
   //Fetch C.S. Ghost ID
	S32 objectID = NetConnection::getConnectionToServer()->getGhostIndex(dynamic_cast<NetObject *>(shape));
	if(objectID == -1) {
		Con::errorf("sendLockBreakEvent() - Cannot resolve ghost id, object is likely not networked.");
		return;
	}
	//Push
	Con::evaluatef("commandToServer('sendLockBreakEvent', %i);", objectID);
}

DefineEngineMethod(GuiTargetingOverlay, setupParams, void, (bool enabled, String lock1, String lock2, F32 range, S32 needed, U32 mask),, "") {
   object->setControlProperties(enabled, lock1, lock2, range, needed, mask);
}

DefineEngineMethod(GuiTargetingOverlay, forceLockBreak, void, (),, "") {
   object->setLockCap(false);
	//Repost in 3 seconds.
	Sim::postEvent( Sim::getRootGroup(), new GTCControlUnlockEvent(object), Sim::getCurrentTime() + 3000 );
}

//Connection Lock Function
// Parsing Steps:
//  * Server requests client lock through TS function
//  * Client calls getLockedTarget() to get the local ID
//  * Client transmits this information to the server through a CMD
//  * Server alls resolveGTCTarget() using client id and info sent to get it's object
DefineEngineMethod(GuiTargetingOverlay, getLockedTarget, S32, (),, "(void) - returns a target locked by the player's guiTargetingOverlay") {
   //This function returns the client's ghost id
	if(!object->isLocked()) {
		return -1;
	}
	if(object->getLockedTarget() == NULL) {
	   return -1;
	}
	S32 objectID = NetConnection::getConnectionToServer()->getGhostIndex(dynamic_cast<NetObject *>(object->getLockedTarget()));
	if(objectID == -1) {
		Con::errorf("getLockedTarget() - Cannot resolve ghost id, object is likely not networked.");
		return -1;
	}
	return objectID;
}

DefineEngineFunction(resolveGTCTarget, S32, (S32 clientID, S32 objectID),, "(S32 cID, S32 gID)") {
	if(objectID == -1) {
		Con::errorf("resolveGTCTarget - Cannot parse an invalid client object id.");
	   return -1;
	}
	SimObject *find = Sim::findObject(clientID);
	if(find != NULL) {
		//Con::printf("Found Object %i under class %s", tclID, find->getClassName());
	   if(dStrcmp(find->getClassName(), "GameConnection") != 0) {
			Con::errorf("resolveGTCTarget(%i) - Control is not a GameConnection, %s", clientID, find->getClassName());
		   return -1;		
	   }
	}
	else {
		Con::errorf("resolveGTCTarget(%i) - No such object (%i) found.", clientID);
		return -1;
	}
	NetConnection *tCl = dynamic_cast<NetConnection *>(find);
	//And now resolve the target
	NetObject *ghost = tCl->resolveObjectFromGhostIndex(objectID);
	if(!ghost) {
		Con::errorf("resolveGTCTarget() - Unable to resolve %i, object is likely not networked.", objectID);
	   return -1;
	}
	return ghost->getId();
}