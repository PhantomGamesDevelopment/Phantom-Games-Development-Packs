//-----------------------------------------------------------------------------
// guiAdvancedRadarCtrl.h
// (C) 2014 Robert Fritzen for Phantom Games Development
// Header file for the radar control for the Advanced FPS Kit for T3D MIT 3.5
//-----------------------------------------------------------------------------

#ifndef _GUIADVANCEDRADARCTRL_H_
#define _GUIADVANCEDRADARCTRL_H_

#ifndef _GUICONTROL_H_
#include "gui/core/guiControl.h"
#endif

#include "T3D/gameBase/gameConnection.h"
#include "T3D/shapeBase.h"

//Phantom: Set to "1" to allow the console to print specific errors. This is mainly a debug tool that should only be used
// To hunt down C++ issues of the radar. If you're sending invalid names via NARC events, that is not a reason to use this.
// You have been warned, this will output A LOT of stuff to the console.
#define _LOG_ERRORS 0
// _UAV_LINES is a render counter of how many lines will be rendered in a "sweep"
#define _UAV_LINES 20

/// Renders an advanced 2D Radar.
class GuiAdvancedRadarCtrl : public GuiControl {
   static const Point2F circlePoints[];

   public:
   
      typedef GuiControl Parent;

   private:
      F32 distanceFactorX, distanceFactorY;
		VectorF fVec, tNorth, pVec;
		Point2F playerPos, translatedMissionArea, translatedPosition, tempPos;
		F32 spinAngle;
		String arrowPath;
		RectF temp1, temp2, rect, missionArea;
		Point2F inOffset, inExtent;
		bool canRender, doingUAVRenderPass;

   protected:
      //Special Radar Flag Enums
		enum RadarFlags {
			NormalObject = 0,    // 0 - Standard "dot", used for objects firing, basically a stationary dot.
			TrackedDot = 1,      // 1 - Tracked Dot uses the "dot" method, but allows for tracking, see Scanner/UAV
			TrackedObject = 2,   // 2 - Tracked Object, shows position and direction travelling
			ScannerObject = 3,   // 3 - This is a scanner object, renders a "circle" on the map showing the scan area
			JammerObject = 4,    // 4 - This is a jammer object, same as above
		};

		enum RenderMode {
			RectangularMode = 0,
			CircularMode = 1,
		};

		//Radar Parameters
		ColorI mFriendlyColor;   // <- Color of friendly objects
		ColorI mEnemyColor;      // <- Color of enemy objets
		ColorI mNeutralColor;    // <- Color of neutral (team0) objects

		bool mRotateMapLayer;    // <- Map Layer Rotates with the Player
		bool mRotateBackLayer;   // <- Background Layer Rotates with the Player
		F32 mRange;              // <- Range of the Radar
		S32 mUAVFlag;            // <- UAV Flag: Named it this seeing as lots of FPS games have "UAV's" Now
									    // 0 - Normal Mode: Reveals on Shooting, or when scanned by an object.
		                         // 1 - Sweep Mode: Radar Sweeps reveal targets when scanned
		                         // 2 - Tracking Mode: Radar Shows "Arrows" showing the direction the target is moving
		U32 UAVSeq;              // This is basically a numeric storage for when the UAV "began" last.
		S32 mUAVSweepTime;       // When UAVFlag is 1 or 2, this indicates the MS time between "sweeps", <100 = essetially an always sweep
		S32 UAVLineCount;        // This is used for the pretty line that goes across the GUI during a UAV sweep
		bool mJammed;            // <- Is the radar currently jammed by an object?
		Vector<String> objs;     // <- Objects currently detected by the radar
		Vector<String> resolved; // <- Resolved Ghost Objects: FMT: "Client #\tServer #"

		RenderMode mRenderMode;  // <- Do we render a circle radar or a rectangle radar?

      // The Background and the Map Layer Image Strings
      String mBackgroundName;
		String mMapLayerName;    
      // Loaded textures for the above.
      GFXTexHandle mBackgroundTO;
		GFXTexHandle mMapLayerTO;

		RadarFlags getRadarFlag(ShapeBase *object);

      static bool setBitmapName( void *object, const char *index, const char *data );
      static const char *getBitmapName( void *obj, const char *data );

   public:
		RadarFlags S32ToFlag(S32 in) { switch(in) { case 0: return NormalObject; case 1: return TrackedDot; case 2: return TrackedObject; case 3: return ScannerObject; case 4: return JammerObject; default: return NormalObject; } }
		S32 FlagToS32(RadarFlags in) { switch(in) { case NormalObject: return 0; case TrackedDot: return 1; case TrackedObject: return 2; case ScannerObject: return 3; case JammerObject: return 4; default: return 1; } }

      GuiAdvancedRadarCtrl();
      static void initPersistFields();

      void setImageLayer(S32 layer, const char *name);

		//These methods are for "enemy" objects, because onRender handles all allied objects by default
		void placeOnRadar(ShapeBase *object, RadarFlags flag);
		void removeFromRadar(ShapeBase *object);
		void updateRadar(F32 range, S32 uavFl, S32 uavSwT, S32 jam);

		void renderRectangle(Point2I offset, const RectI &updateRect);
		void renderCircle(Point2I offset, const RectI &updateRect);
		void renderEnemies(Point2I offset, const RectI &updateRect);
		void doUAVSweep(Point2I offset, const RectI &updateRect, S32 type);

		void calculateDistanceFactor();

		//Asset Functions / Decs
		void drawRotatableBitmap(GFXTextureObject* texture, const RectF &dstRect, const RectF &srcRect, F32 spinAngle, const GFXBitmapFlip in_flip);
		void drawRotatableBitmap(GFXTextureObject* texture, const Point2I &in_rAt, const GFXBitmapFlip in_flip, const GFXTextureFilterType filter, bool in_wrap, F32 spinAngle);
		void draw2DCircle(const GFXStateBlockDesc &desc, const Point2F &loc, F32 radius, const ColorI &color);
		void drawRotatableBitmapCircle(GFXTextureObject* texture, const Point2F &center, F32 radius, F32 spinAngle);
		F32 getRotationAngle(VectorF v1, VectorF v2);
		F32 getTNorthRotation(VectorF v);
		F32 getPointAngle(Point2F p1, Point2F p2);

		Point2F convertWSToGS(Point2F worldSpace, bool applyTranslation);
		Point2F asPoint2F(Point3I i) { return Point2F(i.x, i.y); }

      // GuiControl.
      bool onWake();
      void onSleep();
      void inspectPostApply();

      void updateSizing();

      void onRender(Point2I offset, const RectI &updateRect);

      DECLARE_CONOBJECT( GuiAdvancedRadarCtrl );
      DECLARE_DESCRIPTION( "An advanced control that has a two layered background and renders 2D images based on object positions in the 3D world.");
};

#endif
