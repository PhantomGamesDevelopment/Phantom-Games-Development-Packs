//-----------------------------------------------------------------------------
// guiMapSelector.h
// (C) 2014 Robert Fritzen for Phantom Games Development
// Header file for the map selection control for the Advanced FPS Kit for T3D MIT 3.5
//-----------------------------------------------------------------------------

#ifndef _GUIMAPSELECTORCTRL_H_
#define _GUIMAPSELECTORCTRL_H_

#ifndef _GUICONTROL_H_
#include "gui/core/guiControl.h"
#endif

#include "T3D/gameBase/gameConnection.h"
#include "T3D/shapeBase.h"

/// Renders a map with capabilities to select positions from it.
class GuiMapSelectorCtrl : public GuiControl {
   public:
   
      typedef GuiControl Parent;

   private:
		F32 distanceFactorX, distanceFactorY, spinAngle;
		Point2F translatedMissionArea, tempPos, inOffset, inExtent, lastPosition, lastPosition2;
		String arrowPath;
		RectF temp1, temp2, rect, missionArea;
		VectorF pVec;

		enum SelectorCurrent {
			CURSOR = 0,
			DIRECTION = 1,
			NOCURSOR = 2
		};

   protected:
		//Radar Parameters
		ColorI mFriendlyColor;   // <- Color of friendly objects
		ColorI mEnemyColor;      // <- Color of enemy objets
		ColorI mNeutralColor;    // <- Color of neutral (team0) objects

		S32 mUAVFlag;            // <- For this control the flag can either be 0 or 1, 0 is nothing, 1 shows all enemies

		String mMapLayerName;    
		GFXTexHandle mMapLayerTO;
		String mSelectorCursorName;    
		GFXTexHandle mCursorObject;
		String mSelectorDCursName;    
		GFXTexHandle mDCursorObject;

		bool needsDirection;
		bool canRender;
		bool noCursorSelect;
		bool tracking;
		SelectorCurrent currentSelection;

   public:
      GuiMapSelectorCtrl();
      static void initPersistFields();

      void setImageLayer(S32 layer, const char *name);

      // GuiControl.
      bool onWake();
      void onSleep();
      void inspectPostApply();

      void updateSizing();

		void calculateDistanceFactor();

      void onRender(Point2I offset, const RectI &updateRect);
		void onMouseDown(const GuiEvent &event);
		void onMouseUp(const GuiEvent &event);
		void onMouseEnter(const GuiEvent & event);
		void onMouseLeave(const GuiEvent & event);
		void onMouseMove(const GuiEvent & event);

		void calculateSelection();

		void applySettings(S32 flg, bool nrt, bool nc) { 
			mUAVFlag = flg; 
			needsDirection = nrt; 
			noCursorSelect = nc; 
			if(noCursorSelect) 
				currentSelection = NOCURSOR;
			else 
		      currentSelection = CURSOR; 
			tracking = false;
		} //<--- Reset selector too...

		//Helper Functions
		F32 getTNorthRotation(VectorF v);
		F32 getPointAngle(Point2F p1, Point2F p2);
		void drawRotatableBitmap(GFXTextureObject* texture, const RectF &dstRect, const RectF &srcRect, F32 spinAngle, const GFXBitmapFlip in_flip);
		Point2F convertGStoWS(Point2F guiSpace);
		Point2F convertWSToGS(Point2F worldSpace);
		Point2F asPoint2F(Point3I i) { return Point2F(i.x, i.y); }
		Point2F asPoint2F(Point2I i) { return Point2F(i.x, i.y); }

      DECLARE_CONOBJECT( GuiMapSelectorCtrl );
      DECLARE_DESCRIPTION( "An advanced control that allows a player to pick 3D world coords from a 2D map.");
};

#endif
