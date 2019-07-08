//-----------------------------------------------------------------------------
// guiTargetingOverlay.h
// (C) 2014 Robert Fritzen for Phantom Games Development
// Header file for the targeting overlay gui for the Advanced FPS Kit for T3D MIT 3.5
//-----------------------------------------------------------------------------

#ifndef _GUITARGETINGOVERLAY_H_
#define _GUITARGETINGOVERLAY_H_

#ifndef _GUICONTROL_H_
#include "gui/core/guiControl.h"
#endif

#include "T3D/gameBase/gameConnection.h"
#include "T3D/shapeBase.h"

class GuiTargetingOverlay : public GuiControl {
   typedef GuiControl Parent;

   // field data
   ColorF   mPreLockColor;
	ColorF   mLockedColor;
	String lockingImg;
	String lockedImg;
	GFXTexHandle circleTexHandle;
	GFXTextureObject* texture;

	bool toEnabled;
	bool isLocking;
	bool controlLocked;
	bool canLock; //This is used to slow the control after breaks to block some Acc. Vios
	F32 maxRange;
	S32 neededTicks;
	S32 lockTicks;
	S32 imgTicks;
	ShapeBase *lockObject;
	U32 lockingMask;

	Point3F shapePos;
   MatrixF cam;
   Point3F camPos;
   VectorF camDir;
	VectorF shapeDir;
	F32 shapeDist;

	S32 imgCounter;

   protected:
		void sendLockWarnEvent(ShapeBase *shape);
		void sendCompleteLockEvent(ShapeBase *shape);
		void sendLockBreakEvent(ShapeBase *shape);

   public:
      GuiTargetingOverlay();

		void setLockCap(bool e) { canLock = e; }

		bool isLocked() { return (toEnabled) && (lockTicks >= neededTicks); }
		ShapeBase *getLockedTarget() {
			if(lockObject != NULL) {
			   return lockObject;
			}
			return NULL;
		}

		void setControlProperties(bool e, String l1, String l2, F32 r, S32 n, U32 lM) {
			toEnabled = e;
		   lockingImg = l1;
			lockedImg = l2;
			lockingMask = lM;
			maxRange = r;
			neededTicks = n;

         lockTicks = 0;
			lockObject = NULL;
		}

      // GuiControl
      virtual void onRender(Point2I offset, const RectI &updateRect);

      static void initPersistFields();
      DECLARE_CONOBJECT( GuiTargetingOverlay );
      DECLARE_CATEGORY( "Gui Game" );
      DECLARE_DESCRIPTION( "Used to target objects in the game from set properties.\n"
         "Must be a child of a GuiTSCtrl and a server connection must be present." );
};

#endif
