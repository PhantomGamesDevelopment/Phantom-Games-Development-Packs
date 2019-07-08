//-----------------------------------------------------------------------------
// guiAdvancedRadarCtrl.cpp
// (C) 2014 Robert Fritzen for Phantom Games Development
// Source file for the radar control for the Advanced FPS Kit for T3D MIT 3.5
//
// This is what I'd like to call a "Matematical" Radar, as it uses advanced orthogonic math to determine
//  the locations of object positions relative to the center point (or the player). Other methods such
//  as RTC Radars were suggested, but overall, math calculations appear to be the quickest route of rendering
//-----------------------------------------------------------------------------

#include "platform/platform.h"
#include "PGD/AFPSK/guiAdvancedRadarCtrl.h"
#include "gui/3d/guiTSControl.h"
#include "scene/sceneManager.h"
#include "console/console.h"
#include "console/consoleTypes.h"
#include "console/engineAPI.h"
#include "sim/netConnection.h"
#include "core/stream/bitStream.h"
#include "sim/netObject.h"
#include "gfx/gfxDevice.h"
#include "gfx/gfxDrawUtil.h"
#include "math/mathUtils.h"
#include "gfx/gfxFontRenderBatcher.h"
#include "gfx/gfxTransformSaver.h"
#include "gfx/gfxPrimitiveBuffer.h"
#include "T3D/missionArea.h"
#include "T3D/missionMarker.h"
#include "T3D/player.h"
#include "core/strings/stringUnit.h"

/**
Additional Functions Needed By The Control
**/
const Point2F GuiAdvancedRadarCtrl::circlePoints[] = {
	Point2F(0.707107f, 0.707107f),
	Point2F(0.923880f, 0.382683f),
	Point2F(1.000000f, 0.000000f),
	Point2F(0.923880f, -0.382684f),
	Point2F(0.707107f, -0.707107f),
	Point2F(0.382683f, -0.923880f),
	Point2F(0.000000f, -1.000000f),
	Point2F(-0.382683f, -0.923880f),
	Point2F(-0.707107f, -0.707107f),
	Point2F(-0.923880f, -0.382684f),
	Point2F(-1.000000f, 0.000000f),
	Point2F(-0.923879f, 0.382684f),
	Point2F(-0.707107f, 0.707107f),
	Point2F(-0.382683f, 0.923880f),
	Point2F(0.000000f, 1.000000f),
	Point2F(0.382684f, 0.923879f)
};

//This function converts a given world-space position into gui-space in the location it should appear on the dialog.
Point2F GuiAdvancedRadarCtrl::convertWSToGS(Point2F worldSpace, bool applyTranslation = false) {
	if(missionArea.len_x() == 0 && missionArea.len_y() == 0) {
		Con::errorf("convertWSToGS() - Cannot convert space coordinates without reference object (MissionArea).");
		return Point2F(0, 0);
	}

	Point2F outCoords;
	outCoords.x = worldSpace.x;
	outCoords.y = missionArea.extent.y - worldSpace.y;
	if(applyTranslation) {
		//Translation is defined as the distance from the player (center) to another object located at worldSpace
		outCoords.x = (translatedPosition.x - (worldSpace.x + translatedMissionArea.x)) * distanceFactorX * -1; //<-- X is flipped.
		outCoords.y = (translatedPosition.y - (worldSpace.y + translatedMissionArea.y)) * distanceFactorY;

		outCoords += (rect.extent / 2);
	}

	return outCoords;
}

//drawRotatableBitmap is a tool needed by a variety of options on the radar. For example, we can rotate both the
//outer layer (like a compass) and the inner layer (map), as well as the individual elements in the radar's image.
//Since T3D out of the box doesn't have a tool to support this, I blended the functionaility of the two methods
//GFXDrawUtil::drawBitmapStretchSR [l.352] and GFXDrawUtil::draw2DSquare [l.545] in gfxDrawUtil.cpp 
//Thanks go out to Nathan Martin for helping to get this working! 
void GuiAdvancedRadarCtrl::drawRotatableBitmap(GFXTextureObject* texture, const RectF &dstRect, const RectF &srcRect, F32 spinAngle, const GFXBitmapFlip in_flip = GFXBitmapFlip_None) {
	// Sanity if no texture is specified.
	if(!texture) {
		return;
	}

	GFXDevice		*mDevice			= GFX;
	ColorI zeColor;
	mDevice->getDrawUtil()->getBitmapModulation(&zeColor);
	GFXVertexColor	mBitmapModulation	= ColorI( 255, 255, 255, 255 );
	mBitmapModulation.set(zeColor);

	GFXVertexBufferHandle<GFXVertexPCT> verts(mDevice, 4, GFXBufferTypeVolatile );
	verts.lock();

	F32 texLeft   = (srcRect.point.x)                    / (texture->mTextureSize.x);
	F32 texRight  = (srcRect.point.x + srcRect.extent.x) / (texture->mTextureSize.x);
	F32 texTop    = (srcRect.point.y)                    / (texture->mTextureSize.y);
	F32 texBottom = (srcRect.point.y + srcRect.extent.y) / (texture->mTextureSize.y);

	F32 screenLeft   = dstRect.point.x;
	F32 screenRight  = (dstRect.point.x + dstRect.extent.x);
	F32 screenTop    = dstRect.point.y;
	F32 screenBottom = (dstRect.point.y + dstRect.extent.y);

   if( in_flip & GFXBitmapFlip_X ) {
      F32 temp = texLeft;
      texLeft = texRight;
      texRight = temp;
   }
   if( in_flip & GFXBitmapFlip_Y ) {
      F32 temp = texTop;
      texTop = texBottom;
      texBottom = temp;
   }

	const F32 fillConv = mDevice->getFillConventionOffset();
	verts[0].point.set( screenLeft  - fillConv, screenTop    - fillConv, 0.f );
	verts[1].point.set( screenRight - fillConv, screenTop    - fillConv, 0.f );
	verts[2].point.set( screenLeft  - fillConv, screenBottom - fillConv, 0.f );
	verts[3].point.set( screenRight - fillConv, screenBottom - fillConv, 0.f );

	verts[0].color = verts[1].color = verts[2].color = verts[3].color = mBitmapModulation;

	verts[0].texCoord.set( texLeft,  texTop );
	verts[1].texCoord.set( texRight, texTop );
	verts[2].texCoord.set( texLeft,  texBottom );
	verts[3].texCoord.set( texRight, texBottom );

	if(spinAngle != 0.f) {
		MatrixF rotMatrix( EulerF( 0.0, 0.0, spinAngle ) );
		Point3F	offset( dstRect.point.x + dstRect.extent.x / 2,
						dstRect.point.y + dstRect.extent.y / 2,
						0.0f);

		for( S32 i = 0; i < 4; i++ ) {
			verts[i].point -= offset;
			rotMatrix.mulP( verts[i].point );
			verts[i].point += offset;
		}
	}

	verts.unlock();

	mDevice->setVertexBuffer( verts );

	// Define the state block for the GFX mode.
	// DrawBitmapStretchSR
	GFXStateBlockDesc bitmapStretchSR;
	bitmapStretchSR.setCullMode(GFXCullNone);
	bitmapStretchSR.setZReadWrite(false);
	bitmapStretchSR.setBlend(true, GFXBlendSrcAlpha, GFXBlendInvSrcAlpha);
	bitmapStretchSR.samplersDefined = true;

	// Linear: Create clamp SB
	bitmapStretchSR.samplers[0] = GFXSamplerStateDesc::getClampLinear();
	GFXStateBlockRef mBitmapStretchLinearSB = mDevice->createStateBlock(bitmapStretchSR);

	mDevice->setStateBlock(mBitmapStretchLinearSB);
	mDevice->setTexture( 0, texture );
	mDevice->setupGenericShaders( GFXDevice::GSModColorTexture );
	mDevice->drawPrimitive( GFXTriangleStrip, 0, 2 );
}

void GuiAdvancedRadarCtrl::drawRotatableBitmap(GFXTextureObject* texture, const Point2I &in_rAt, const GFXBitmapFlip in_flip, const GFXTextureFilterType filter , bool in_wrap /*= true*/, F32 spinAngle /*= 0.0f*/) {
   AssertFatal( texture != 0, "No texture specified for drawBitmap()" );

   RectF subRegion( 0.0f, 0.0f, (F32)texture->mBitmapSize.x, (F32)texture->mBitmapSize.y );
   RectF stretch( (F32)in_rAt.x, (F32)in_rAt.y, (F32)texture->mBitmapSize.x, (F32)texture->mBitmapSize.y );
   drawRotatableBitmap( texture, stretch, subRegion, spinAngle );
}

//draw2DCirlce is a tool I've thrown together using drawCylinder. For some reason T3D doesn't have drawCircle?
// I could have just wrote my own using the cylinder code, but why bother when a circle is just a cylinder with 0 height.

void GuiAdvancedRadarCtrl::draw2DCircle(const GFXStateBlockDesc &desc, const Point2F &loc, F32 radius, const ColorI &color) {
	Point3F botNTop(loc.x, loc.y, 0);
	GFX->getDrawUtil()->drawCylinder(desc, botNTop, botNTop, radius, color); 
}

void GuiAdvancedRadarCtrl::drawRotatableBitmapCircle(GFXTextureObject* texture, const Point2F &center, F32 radius, F32 spinAngle) {
   VectorF uvec = Point3F(center.x, center.y, 0.0f);
   uvec.normalize();
   MatrixF mat(true);
   MathUtils::getMatrixFromUpVector(uvec, &mat);   
   mat.setPosition(Point3F(center.x, center.y, 0.0f));

   Point3F scale(radius, radius, 1);
   mat.scale(scale);
   GFXTransformSaver saver;

   GFX->pushWorldMatrix();
   GFX->multWorld(mat);

	ColorI modulation;
	GFX->getDrawUtil()->getBitmapModulation(&modulation);

   S32 numPoints = sizeof(circlePoints)/sizeof(Point2F);
   GFXVertexBufferHandle<GFXVertexPC> verts(GFX, numPoints * 4 + 4, GFXBufferTypeVolatile);
   verts.lock();
   for (S32 i=0; i<numPoints + 1; i++) {
      S32 imod = i % numPoints;
      verts[i].point = Point3F(circlePoints[imod].x,circlePoints[imod].y, 0.5f);
      verts[i].color = modulation;
      verts[i + numPoints + 1].point = Point3F(circlePoints[imod].x,circlePoints[imod].y, 0);
      verts[i + numPoints + 1].color = modulation;

      verts[2*numPoints + 2 + 2 * i].point = Point3F(circlePoints[imod].x,circlePoints[imod].y, 0.5f);
      verts[2*numPoints + 2 + 2 * i].color = modulation;
      verts[2*numPoints + 2 + 2 * i + 1].point = Point3F(circlePoints[imod].x,circlePoints[imod].y, 0);
      verts[2*numPoints + 2 + 2 * i + 1].color = modulation;
   }
   if(spinAngle != 0.f) {
      MatrixF rotMatrix(EulerF(0.0, 0.0, spinAngle));

      for(S32 i = 0; i < 4; i++) {
         rotMatrix.mulP(verts[i].point);
         verts[i].point += Point3F(center.x, center.y, 0.0f);
      }
   }

   verts.unlock();

   GFX->setVertexBuffer(verts);
   // Define the state block for the GFX mode.
   GFXStateBlockDesc bitmapStretchSR;
   bitmapStretchSR.setCullMode(GFXCullNone);
   bitmapStretchSR.setZReadWrite(false);
   bitmapStretchSR.setBlend(true, GFXBlendSrcAlpha, GFXBlendInvSrcAlpha);
   bitmapStretchSR.samplersDefined = true;
   bitmapStretchSR.samplers[0] = GFXSamplerStateDesc::getClampLinear();
   GFXStateBlockRef mBitmapStretchLinearSB = GFX->createStateBlock(bitmapStretchSR);
	
	GFX->setStateBlock(mBitmapStretchLinearSB);
 
   GFX->setTexture(0, texture);
   GFX->setupGenericShaders(GFXDevice::GSModColorTexture);

   GFX->drawPrimitive(GFXTriangleFan, 0, numPoints);
   GFX->drawPrimitive(GFXTriangleFan, numPoints + 1, numPoints);
   GFX->drawPrimitive(GFXTriangleStrip, 2 * numPoints + 2, 2 * numPoints);

   GFX->popWorldMatrix();
}

//getRotationAngle is used to determine the angle at which we render the map layer when set to rotation,
// simply put a run through calculation of the angle between v1 and v2, mAtan2 handles nicely. :)
F32 GuiAdvancedRadarCtrl::getRotationAngle(VectorF v1, VectorF v2) {
   // v1 & v2 must be normalized  
   v1.z = v2.z = 0.0f;  
   if(!v1.isUnitLength()) {    
      v1.normalizeSafe();    
   }    
   if(!v2.isUnitLength()) {    
      v2.normalizeSafe();    
   }   
   F32 ang = atan2(v1.y, v1.x) - atan2(v2.y, v2.x);
	return ang < 0 ? ang + M_2PI : ang;
}

F32 GuiAdvancedRadarCtrl::getTNorthRotation(VectorF v1) {  
   // v1 must be normalized  
   v1.z = 0.0f;  
   if(!v1.isUnitLength()) {    
      v1.normalizeSafe();    
   }    
   F32 ang = atan2(v1.y, v1.x) - atan2(1.0f, 0.0f);
	return ang < 0 ? ang + M_2PI : ang;
} 

//getPointAngle is used to calculate the angle between two 2D points, and is useful for two stationary objects
F32 GuiAdvancedRadarCtrl::getPointAngle(Point2F p1, Point2F p2) {
   F32 ang = atan2(p2.y - p1.y, p2.x - p1.x);
	return ang;
}

//This is used in onRender to check for friendly scanners and jammers
GuiAdvancedRadarCtrl::RadarFlags GuiAdvancedRadarCtrl::getRadarFlag(ShapeBase *object) {
	if(dAtoi(object->getDataField(StringTable->insert("isScanner"), 0)) == 1) {
		return ScannerObject;
	}
	else if(dAtoi(object->getDataField(StringTable->insert("isJammer"), 0)) == 1) {
		return JammerObject;
	}
	else {
		return NormalObject;
	}
}

/**
End
**/

IMPLEMENT_CONOBJECT(GuiAdvancedRadarCtrl);

ConsoleDocClass( GuiAdvancedRadarCtrl,
   "@brief A gui control that is used to display 2D positions relative to the 3D world.\n\n"
   
	"The control has two images, mBackgroundName and mMapLayerName, they render mMap -> mBackground\n"
   "these images by default are non-wrapped and streched to the control.\n\n"

	"For mMapLayer, you need to make an image whose dimensions match that of your world's 2D extent\n"
	"This makes use of the MissionArea object, without one, the layer will not render."

	"There are many other radar parameters, icon colors: mFriendlyColor, mEnemyColor, and mNeutralColor\n\n"

	"Map rotations are handled by mRotateBackLayer and mRotateBackLayer, setting these will rotate your\n"
	"map based on the map's true north <0 1 0> from center.\n\n"

	"mRenderMode determines whether to render the radar as a rectangle or a circle and can be defined with\n"
	"either RectangularMode or CircularMode. By default the radar is in RectangularMode.\n\n"

	"mJammed defines if the radar if being jammed. mRange is the effective viewing range of the radar.\n"
	"UAV parameters are mUAVSweepTime and mUAVFlag which may be 0, 1, or 2\n\n"

	"By the control's rule, this must be a child of the GuiTSCtrl object in playGui.gui.\n\n"
   
   "@tsexample\n"
   "// Create a GuiAdvancedRadarCtrl with no rotation.\n"
   "%radar = new GuiAdvancedRadarCtrl() {\n"
   "   Background = \"myCompass.png\";\n"
	"   MapLayer = \"currentMap.png\";\n"
	"   FriendlyColor = \"0 255 0 255\";\n"
	"   EnemyColor = \"255 0 0 255\";\n"
	"   NeutralColor = \"255 255 255 255\";\n"
	"   rotateMapLayer = false;\n"
	"   rotateBackLayer = false;\n"
	"   renderMode = RectangularMode;\n"
   "};\n"
   "@endtsexample\n\n"
   
   "@ingroup GuiControls"
);

GuiAdvancedRadarCtrl::GuiAdvancedRadarCtrl(void)
 : mBackgroundName(),
   mMapLayerName(),
	mFriendlyColor(0, 255, 0),
	mEnemyColor(255, 0, 0),
	mNeutralColor(255, 255, 255),
	mRotateMapLayer(false),
	mRotateBackLayer(false),
	mJammed(false),
	mRange(75.0f),
	mUAVSweepTime(3500),
	mUAVFlag(0),
	UAVSeq(0),
	UAVLineCount(0),
	mRenderMode(RectangularMode){	

		distanceFactorX = distanceFactorY = spinAngle = 0.0f;
		fVec.set(0, 0, 0);
		tNorth.set(0, 1, 0);
		tNorth.normalizeSafe();
		pVec.set(0, 0, 0);
		playerPos.set(0, 0);
		translatedMissionArea.set(0, 0);
		translatedPosition.set(0, 0);
		tempPos.set(0, 0);
		missionArea.set(0, 0, 0, 0);
		arrowPath = "";
		canRender = false;
		doingUAVRenderPass = false;
}

void GuiAdvancedRadarCtrl::initPersistFields() {
   addGroup( "Bitmap" );
   
      addField( "background",   TypeImageFilename,     Offset( mBackgroundName, GuiAdvancedRadarCtrl ),
         "The background layer of the radar (Required), use setImageLayer(0, name) to set. CHANGING THIS HAS NO EFFECT!!!" );
      addField( "mapLayer",   TypeImageFilename,     Offset( mMapLayerName, GuiAdvancedRadarCtrl ),
         "The map layer of the radar, use setImageLayer(1, name) to set. CHANGING THIS HAS NO EFFECT!!!" );

   endGroup( "Bitmap" );

	addGroup( "Radar" );
      addField( "rotateMapLayer",   TypeBool,     Offset( mRotateMapLayer, GuiAdvancedRadarCtrl ),
         "If true, the map layer of the radar will rotate relative to the player's movement." );
      addField( "rotateBackLayer",   TypeBool,     Offset( mRotateBackLayer, GuiAdvancedRadarCtrl ),
         "If true, the back layer of the radar will rotate relative to the player's movement." );
		addField( "friendlyColor", TypeColorI, Offset( mFriendlyColor, GuiAdvancedRadarCtrl), 
			"Color of friendly elements rendered on the radar.");
		addField( "neutralColor", TypeColorI, Offset( mNeutralColor, GuiAdvancedRadarCtrl), 
			"Color of neutral elements rendered on the radar.");
		addField( "enemyColor", TypeColorI, Offset( mEnemyColor, GuiAdvancedRadarCtrl), 
			"Color of enemy elements rendered on the radar.");
	endGroup( "Radar" );

   Parent::initPersistFields();
}

bool GuiAdvancedRadarCtrl::onWake() {
	if (! Parent::onWake()) {
      return false;
	}

	//Catch the MissionArea if it exists?
	MissionArea * obj = MissionArea::getServerObject();
	if(!obj) {
		Con::errorf("Cannot locate the MissionArea object, disabling control.");
		canRender = false;
	   return false;
	}
	//translates mission area to image coords and player position to fit in the translated area.
	translatedMissionArea.set(obj->getArea().point.x*-1, obj->getArea().point.y*-1);
	missionArea.set(0, 0, obj->getArea().extent.x, obj->getArea().extent.y);
	//And let us render.
	canRender = true;
   setActive(true);
   setImageLayer(0, mBackgroundName);
	setImageLayer(1, mMapLayerName);
   return true;
}

void GuiAdvancedRadarCtrl::onSleep() {
	if ( !mBackgroundName.equal("texhandle", String::NoCase) ) {
      mBackgroundTO = NULL;
	}
	if ( !mMapLayerName.equal("texhandle", String::NoCase) ) {
      mMapLayerTO = NULL;
	}

   Parent::onSleep();
}

//-------------------------------------
void GuiAdvancedRadarCtrl::setImageLayer(S32 layer, const char *name) {
	if(!Platform::isFile(name)) {
		Con::errorf("::setImageLayer(%i) - Invalid File %s", layer, name);
		return;
	}

	if(layer == 0) {
		mBackgroundName = name;
		if ( !isAwake() )
			return;
		if ( mBackgroundName.isNotEmpty() ) 
		   mBackgroundTO.set( mBackgroundName, &GFXDefaultGUIProfile, avar("%s() - mBackgroundTO (line %d)", __FUNCTION__, __LINE__) );
		else
			mBackgroundTO = NULL;
	}
	else {
		mMapLayerName = name;
		if ( !isAwake() )
			return;
		if ( mMapLayerName.isNotEmpty() ) 
		   mMapLayerTO.set( mMapLayerName, &GFXDefaultGUIProfile, avar("%s() - mMapLayerTO (line %d)", __FUNCTION__, __LINE__) );
		else
			mMapLayerTO = NULL;
	}

	setUpdate();
}

void GuiAdvancedRadarCtrl::inspectPostApply() {
   // if the extent is set to (0,0) in the gui editor and appy hit, this control will
   // set it's extent to be exactly the size of the bitmap (if present)
   Parent::inspectPostApply();

   if ((getExtent().x == 0) && (getExtent().y == 0) && mBackgroundTO) {
      setExtent( mBackgroundTO->getWidth(), mBackgroundTO->getHeight());
   }
	inExtent.set(getExtent().x, getExtent().y);
}

void GuiAdvancedRadarCtrl::updateSizing() {
	if(!getParent()) {
      return;
	}

   // updates our bounds according to our horizSizing and verSizing rules
   RectI fakeBounds( getPosition(), getParent()->getExtent());

	calculateDistanceFactor();

   parentResized( fakeBounds, fakeBounds);
}

void GuiAdvancedRadarCtrl::calculateDistanceFactor() {
   distanceFactorX = getExtent().x / (mRange*2);
	distanceFactorY = getExtent().y / (mRange*2);
}

void GuiAdvancedRadarCtrl::onRender(Point2I offset, const RectI &updateRect) {
	if(!canRender) {
	   //Test for Mission Area...
	   MissionArea * obj = MissionArea::getServerObject();
	   if(!obj) {
	      return;
	   }
	   //translates mission area to image coords and player position to fit in the translated area.
	   translatedMissionArea.set(obj->getArea().point.x*-1, obj->getArea().point.y*-1);
	   missionArea.set(0, 0, obj->getArea().extent.x, obj->getArea().extent.y);
	   //And let us render.
	   canRender = true;
		//Update the control and go.
      setActive(true);
      setImageLayer(0, mBackgroundName);
	   setImageLayer(1, mMapLayerName);
	}
	inOffset.set(offset.x, offset.y);
	if(mRenderMode == RectangularMode) {
		renderRectangle(offset, updateRect);
	}
	else {
		//In Development!
		//renderCircle(offset, updateRect);
		Con::errorf("::onRender() - CircularMode is currently in development, switching to Rectangular Mode");
		mRenderMode = RectangularMode;
		renderRectangle(offset, updateRect);
	}
}

void GuiAdvancedRadarCtrl::renderCircle(Point2I offset, const RectI &updateRect) {
	//Stub: For now, I just need to get drawBitmapCircle done and this will be done immediately.
}

void GuiAdvancedRadarCtrl::renderRectangle(Point2I offset, const RectI &updateRect) {
   // The Radar must be a child in a TS Control
   GuiTSCtrl *parent = dynamic_cast<GuiTSCtrl*>(getParent());
	if (!parent) { 
		return;
	}

   // Must have a connection and control object for positioning purposes
   GameConnection* conn = GameConnection::getConnectionToServer();
	if (!conn) { 
		return;
	}
   GameBase * control = conn->getControlObject();
	if (!control) {
	   return;
	}
	GFXTextureObject* texture = NULL;
	GFXTexHandle elementHandle;// = new GFXTexHandle(arrowPath, &GFXDefaultGUIProfile, avar("%s() - mTextureObject (line %d)", __FUNCTION__, __LINE__));
	//Gather player information relative to map information
	playerPos.set(control->getPosition().x, control->getPosition().y);
	fVec.set(control->getTransform().getForwardVector());
	fVec.normalizeSafe();	
	rect.set(updateRect.point.x, updateRect.point.y, updateRect.extent.x, updateRect.extent.y);
	//Radar requires a background texture object.
   if (mBackgroundTO) {
		GFX->getDrawUtil()->clearBitmapModulation();   //First.... Always
		//Render the back layer, check for rotation as well.
		spinAngle = 0.0f;
		if(mRotateBackLayer) {
			spinAngle = getTNorthRotation(fVec);
		}
		temp1.set( 0.f, 0.f, (F32)mBackgroundTO->mBitmapSize.x, (F32)mBackgroundTO->mBitmapSize.y );
		texture = mBackgroundTO;
		drawRotatableBitmap(texture, rect, temp1, spinAngle); 
      //Some radars (Halo) use just a backround, others (CoD) render a internal map. My radar can do both ;p
		//Radar's with a map layer need to do a little bit of pre-calculations before rendering.
		if(mMapLayerTO) {
		   /** If we're using a map layer, render it first, to do so, we need to do a little bit of math.
			 To do this, we need to assume the map uses the MissionArea as it's bounds, so check for the MissionArea first.
			 Once we've got the MissionArea we can get the transform of the control object and cast it down to the 2D position
			 on the radar. Lastly, using this and mRange, which is the radar's "Viewing" range we can scale the map texture
			 object to the proper size and move the image around to get the proper position of it.

			 NOTE: The 'y' axis is apparently flipped in gui coords, calculate accordingly...
			**/
			translatedPosition = playerPos + translatedMissionArea;
			//--------------------------------------------------------
			texture = mMapLayerTO;
			RectF radarBounds;  //<-- This is the bounds of the radar, the center is the location of the player relative to mapBounds
			radarBounds.set(convertWSToGS(translatedPosition).x - (mRange), 
				             convertWSToGS(translatedPosition).y - (mRange), 
				             mRange*2, mRange*2);
			//radarBounds corrections?
			temp2 = rect;
			if(radarBounds.point.x < 0 || radarBounds.point.y < 0 
				|| ((radarBounds.point.x + radarBounds.extent.x) > missionArea.extent.x)
				|| ((radarBounds.point.y + radarBounds.extent.y) > missionArea.extent.y)) {
				//This code will "correct" the bounds when the radar extend's it's maximum reach of the image.
			   //How it works is it will move the render rectangle to be within the GUI, but out of normal position
				//such that only the relevant area on the image will render, and the background behind it.
				if(radarBounds.point.x < 0) {
					//Cases where point.x < 0
					temp2.point.x += (mRange - translatedPosition.x) * distanceFactorX;
					radarBounds.point.x = 0;
				}
				if(radarBounds.point.y < 0) {
					//Cases where point.y < 0
					temp2.point.y += (mRange - convertWSToGS(translatedPosition).y) * distanceFactorY;
					radarBounds.point.y = 0;
				}
				if((radarBounds.point.x + radarBounds.extent.x) > missionArea.extent.x) {
					//Cases where point.x > bounds.x
					temp2.point.x -= ((radarBounds.point.x + radarBounds.extent.x) - missionArea.extent.x) * distanceFactorX;
				   radarBounds.point.x = missionArea.extent.x - mRange*2;
				}
				if((radarBounds.point.y + radarBounds.extent.y) > missionArea.extent.y) {
					//Cases where point.y > bounds.y
					temp2.point.y -= ((radarBounds.point.y + radarBounds.extent.y) - missionArea.extent.y) * distanceFactorY;
				   radarBounds.point.y = missionArea.extent.y - mRange*2;
				}
			}
			//Consider rotations.
			if(mRotateMapLayer) {
				spinAngle = getTNorthRotation(fVec); //getRotationAngle(fVec, tNorth);
			}	
			drawRotatableBitmap(texture, temp2, radarBounds, spinAngle);
		}
		//Now we render "ally" elements, enemy and neutral objects are handled by placeOnRadar()/removeFromRadar()
		if(!mJammed) {
		   //Render the player's arrow
			GFX->getDrawUtil()->setBitmapModulation(mFriendlyColor);
			if(dynamic_cast<ShapeBase *>(control)->getShapeIcon()) {
				arrowPath = dynamic_cast<ShapeBase *>(control)->getShapeIcon();
				if(control->getTypeMask() & PlayerObjectType && (StringUnit::getUnitCount(dynamic_cast<ShapeBase *>(control)->getShapeIcon(), "\t\n")) > 1) {
				   arrowPath = StringUnit::getUnit(dynamic_cast<ShapeBase *>(control)->getShapeIcon(), 1, "\t\n");
				}
				elementHandle.set(arrowPath.c_str(), &GFXDefaultGUIProfile, avar("%s() - elementHandle (line %d)", __FUNCTION__, __LINE__)); 
				texture = elementHandle;
				if(texture) {
					if(!mRotateMapLayer) {
						//arrow faces direction of travel
						spinAngle = getTNorthRotation(fVec); //getRotationAngle(fVec, tNorth);
					}
					else {
						//arrow always faces "up"
						spinAngle = 0.0f;
					}
					temp1.set(0.f, 0.f, (F32)texture->mBitmapSize.x, (F32)texture->mBitmapSize.y );
					//this goes in the middle of rect
					temp2.set((rect.point.x + (rect.extent.x / 2)) - ((F32)texture->mBitmapSize.x / 2), 
								 (rect.point.y + (rect.extent.y / 2)) - ((F32)texture->mBitmapSize.y / 2),
								 (F32)texture->mBitmapSize.x, (F32)texture->mBitmapSize.y);
					drawRotatableBitmap(texture, temp2, temp1, spinAngle);
				}
			}
			//Render all "friendly" objects that can be rendered now.
			for (SimSetIterator itr(conn); *itr; ++itr) {
				ShapeBase* shape = dynamic_cast< ShapeBase* >(*itr);
				if (shape) {
					//Waypoint Support: 1.1
					if (shape->getShapeIcon() && dStrcmp(shape->getClassName(), "WayPoint") == 0) {
					   arrowPath = shape->getShapeIcon();
						elementHandle.set(arrowPath.c_str(), &GFXDefaultGUIProfile, avar("%s() - elementHandle (line %d)", __FUNCTION__, __LINE__));
						texture = elementHandle;
						//Note: If you want to use enemy waypoints, you'll need to modify the below if statement.
						if(texture && (dynamic_cast<WayPoint *>(shape)->mTeam.mTeamId == dynamic_cast<ShapeBase *>(control)->getTeam() 
							&& dynamic_cast<WayPoint *>(shape)->mTeam.mTeamId == 0)) {
					      //Arrow direction is determined based on rotation modes from true north tNorth
							pVec = shape->getTransform().getForwardVector();
							if(!mRotateMapLayer) {									
								spinAngle = getRotationAngle(pVec, tNorth);
							}
							else {
								spinAngle = getRotationAngle(pVec, fVec);
							}
							temp1.set(0.f, 0.f, (F32)texture->mBitmapSize.x, (F32)texture->mBitmapSize.y);
							tempPos = convertWSToGS(shape->getPosition().asPoint2F(), true);
							tempPos -= asPoint2F(texture->mBitmapSize) / 2;
							temp2.set(rect.point + tempPos, asPoint2F(texture->mBitmapSize));
						   //Same team waypoints.
						   if(dynamic_cast<WayPoint *>(shape)->mTeam.mTeamId == dynamic_cast<ShapeBase *>(control)->getTeam()) {
								GFX->getDrawUtil()->setBitmapModulation(mFriendlyColor);					
						   }
						   //Neutral waypoints...
						   if(dynamic_cast<WayPoint *>(shape)->mTeam.mTeamId == 0) {
								GFX->getDrawUtil()->setBitmapModulation(mNeutralColor);
						   }
							drawRotatableBitmap(texture, temp2, temp1, spinAngle); 						
						}

					}
					//End
					if (shape->getShapeIcon() && (shape->getTeam() == dynamic_cast<ShapeBase *>(control)->getTeam())) {
						//If the shape is a tracker or a jammer, render the sphere accordingly
						if(getRadarFlag(shape) == ScannerObject || getRadarFlag(shape) == JammerObject) {
						   //pull the range of the object
							S32 radiusOfEffect = dAtoi(shape->getDataField(StringTable->insert("radarRange"), 0));
							S32 neededDistanceForRender = mRange + radiusOfEffect;
							if((shape->getPosition().asPoint2F() - control->getPosition().asPoint2F()).len() <= neededDistanceForRender) {
							   //It's entirely possible here to have the center of the circle render outside mRange, which in effect is outside
								//the GUI rectangle, thankfully, the way T3D handles this is to render the full circle with anything outside being
								//100% transparent, yay for not needing epic ultra level mathz0rz! Also another "fun" feature is that jammers and
								//scanners use their shapeIcon as the map render, so another bite of code goes away :)
								arrowPath = shape->getShapeIcon();
								elementHandle.set(arrowPath.c_str(), &GFXDefaultGUIProfile, avar("%s() - elementHandle (line %d)", __FUNCTION__, __LINE__)); 
								texture = elementHandle;
								if(texture) {
									spinAngle = getPointAngle(control->getPosition().asPoint2F(), shape->getPosition().asPoint2F());
									tempPos = convertWSToGS(shape->getPosition().asPoint2F(), true);
									tempPos -= asPoint2F(texture->mBitmapSize) / 2;
									//And lastly, render it.
									drawRotatableBitmapCircle(texture, Point2F(rect.point + tempPos), (F32)radiusOfEffect, 0.0f);
								}
							}
						}					
					}
					if (shape != control && shape->getShapeIcon() && (shape->getPosition().asPoint2F() - control->getPosition().asPoint2F()).len() <= mRange && (shape->getTeam() == dynamic_cast<ShapeBase *>(control)->getTeam())) { // <- getShapeIcon() is defined in shapeBase.h by the pack
						//Arrow Rendering
						if(shape->getTypeMask() & PlayerObjectType) {
							if(dStrcmp(dynamic_cast<Player *>(shape)->getStateName(), "Dead") == 0) {
							   continue; //No sense in rendering dead guys...
							}
							//Player Objects have a tabbed string ShapeIcon field, containing the enemy "dot" and the "arrow".
							arrowPath = StringUnit::getUnit(shape->getShapeIcon(), 1, "\t\n");
							elementHandle.set(arrowPath.c_str(), &GFXDefaultGUIProfile, avar("%s() - elementHandle (line %d)", __FUNCTION__, __LINE__)); 
							texture = elementHandle;
							if(texture) {
							   //Arrow direction is determined based on rotation modes from true north tNorth
								pVec = shape->getTransform().getForwardVector();
								if(!mRotateMapLayer) {
									spinAngle = getRotationAngle(pVec, tNorth);
								}
								else {
									spinAngle = getRotationAngle(pVec, fVec);
								}
								temp1.set(0.f, 0.f, (F32)texture->mBitmapSize.x, (F32)texture->mBitmapSize.y);
								tempPos = convertWSToGS(shape->getPosition().asPoint2F(), true);
								tempPos -= asPoint2F(texture->mBitmapSize) / 2;
								temp2.set(rect.point + tempPos, asPoint2F(texture->mBitmapSize));
								drawRotatableBitmap(texture, temp2, temp1, spinAngle); 
							}
						}
						else {
							//Render it in tracker mode.
							arrowPath = shape->getShapeIcon();
							elementHandle.set(arrowPath.c_str(), &GFXDefaultGUIProfile, avar("%s() - elementHandle (line %d)", __FUNCTION__, __LINE__));
							texture = elementHandle;
							if(texture) {
							   //Arrow direction is determined based on rotation modes from true north tNorth
								pVec = shape->getTransform().getForwardVector();
								if(!mRotateMapLayer) {									
									spinAngle = getRotationAngle(pVec, tNorth);
								}
								else {
									spinAngle = getRotationAngle(pVec, fVec);
								}
								temp1.set(0.f, 0.f, (F32)texture->mBitmapSize.x, (F32)texture->mBitmapSize.y);
								tempPos = convertWSToGS(shape->getPosition().asPoint2F(), true);
								tempPos -= asPoint2F(texture->mBitmapSize) / 2;
								temp2.set(rect.point + tempPos, asPoint2F(texture->mBitmapSize));
								drawRotatableBitmap(texture, temp2, temp1, spinAngle); 						
							}
						}
					}
				}
			}
			GFX->getDrawUtil()->clearBitmapModulation();
			//Now call the code to render enemy objects
			renderEnemies(offset, updateRect);
		}
		else {
		   //Jammed! Render a grey background to indicate this.
			const ColorI c(64, 64, 64), c2(255, 0, 0);
			GFX->getDrawUtil()->drawRect(rect, c);
			GFont *f = new GFont();
			f->create("Arial", 18);
			Point2I center;
			center.x = (rect.point.x + rect.extent.x) - ((rect.extent.x / 2) + 30);
			center.y = (rect.point.y + rect.extent.y) - ((rect.extent.y / 2) + 9);
			GFX->getDrawUtil()->drawText(f, center, "JAMMED", &c2);
		}
	}
	//Phantom139: I have no idea why you'd event want "child" controls on the radar. It's more or less it's own entity
	//that can handle rendering everything it needs by itself, and thus, we ignore this generic T3D code portion.
	//Feel free to uncomment if you feel the need to add controls to your radar, I highly doubt it. :)
   //renderChildControls(offset, updateRect);
}

void GuiAdvancedRadarCtrl::renderEnemies(Point2I offset, const RectI &updateRect) {
   GameConnection* conn = GameConnection::getConnectionToServer();
	if (!conn) { 
		return;
	}
   GameBase * control = conn->getControlObject();
	if (!control) {
	   return;
	}
	//
	if(objs.size() == 0 && mUAVFlag == 0) {
		//If there is nothing to render, stop now.
		return;
	}

	//Pre-Decs
	ShapeBase *tmp = NULL;
	RadarFlags objFlag;
	//
	GFXTexHandle elementHandle;// = new GFXTexHandle(arrowPath, &GFXDefaultGUIProfile, avar("%s() - mTextureObject (line %d)", __FUNCTION__, __LINE__));
	GFXTextureObject *texture = NULL;
	//Gather player information relative to map information
	Point2F tempFlPt;
	//Now we need to loop through the object list and place them on the radar
	for(S32 i = 0; i < objs.size(); i++) {
		tmp = dynamic_cast<ShapeBase *>(Sim::findObject(dAtoi(StringUnit::getUnit(objs[i], 0, "\t\n"))));
		if(tmp == NULL) {
			objs.erase(i);
			continue;
		}
		objFlag = S32ToFlag(dAtoi(StringUnit::getUnit(objs[i], 1, "\t\n")));
		if(tmp->getTeam() == dynamic_cast<ShapeBase *>(control)->getTeam()) {
			objs.erase(i);
		   continue; //<<---- We're only interested in enemies...
		}
		//
		if(tmp->getTeam() == 0) {
		   GFX->getDrawUtil()->setBitmapModulation(mNeutralColor);
		}
		else {
		   GFX->getDrawUtil()->setBitmapModulation(mEnemyColor);
		}
		//If it's a player we need to check for Tracker vs. Normal, otherwise it's just render if it's in the list.
		if((tmp->getTypeMask() & PlayerObjectType) && tmp->getShapeIcon() && ((tmp->getPosition().asPoint2F() - control->getPosition().asPoint2F()).len() <= mRange)) {
			//First and foremost, is a simple check to see if the UAV scan is commencing now, if it is, then we ignore this "sweep"
			// as the UAV will handle the location render.
			// NOTE! To those of you looking to make Anti-UAV Perks, ect. You'd actually want to add a check here to see if the perk is on
			// otherwise the player will be able to run completely free of detection when a UAV is up.
			if(dStrcmp(dynamic_cast<Player *>(tmp)->getStateName(), "Dead") == 0) {
			   continue; //No sense in rendering dead guys...
			}
			if(mUAVFlag > 0 && Sim::getCurrentTime() >= (UAVSeq + mUAVSweepTime) && objFlag != TrackedObject && objFlag != TrackedDot) {
				//A UAV Sweep is about to come on through so we ignore, unless... the player is already being tracked
			}
			else if(mUAVFlag == 2) {
				//Tracker UAV... Ignore all renders
			}
			else {
			   //Go ahead and perform the render
				if(objFlag == TrackedObject) {
					arrowPath = StringUnit::getUnit(tmp->getShapeIcon(), 1, "\t\n"); 
				}
				else {
				   arrowPath = StringUnit::getUnit(tmp->getShapeIcon(), 0, "\t\n"); 
				}
				//
				elementHandle.set(arrowPath.c_str(), &GFXDefaultGUIProfile, avar("%s() - elementHandle (line %d)", __FUNCTION__, __LINE__)); 
				texture = elementHandle;
				if(texture) {
				   //Arrow direction is determined based on rotation modes from true north tNorth
					pVec = tmp->getTransform().getForwardVector();
					if(!mRotateMapLayer) {
					   //If the map doesn't rotate the angle is between tNorth and the player						
						spinAngle = getRotationAngle(pVec, tNorth);
					}
					else {
						//Otherwise, it's between the player and the target
					   spinAngle = getRotationAngle(pVec, fVec);
					}
					temp1.set(0.f, 0.f, (F32)texture->mBitmapSize.x, (F32)texture->mBitmapSize.y);
					if(objFlag == NormalObject) {
						tempFlPt.set(dAtof(StringUnit::getUnit(StringUnit::getUnit(objs[i], 2, "\t\n"), 0, "\t\n ")), 
							dAtof(StringUnit::getUnit(StringUnit::getUnit(objs[i], 2, "\t\n"), 1, "\t\n ")));
					}
					else {
						tempFlPt = tmp->getPosition().asPoint2F();
					}
					tempPos = convertWSToGS(tempFlPt, true);
					tempPos -= asPoint2F(texture->mBitmapSize) / 2;
					temp2.set(rect.point + tempPos, asPoint2F(texture->mBitmapSize));
					drawRotatableBitmap(texture, temp2, temp1, spinAngle); 						
				}
			}
		}
		else {
		   //Not a player, these have their own shape icons and unless they are special (missiles-o-d00m), will likely not show up here
			// but in the case they do, they render exclusively in tracker mode (regardless if normal mode is on or not)
			arrowPath = tmp->getShapeIcon();
			elementHandle.set(arrowPath.c_str(), &GFXDefaultGUIProfile, avar("%s() - elementHandle (line %d)", __FUNCTION__, __LINE__));
			texture = elementHandle;
			if(texture) {
				pVec = tmp->getTransform().getForwardVector();
				if(!mRotateMapLayer) {
					//If the map doesn't rotate the angle is between tNorth and the player
					spinAngle = getRotationAngle(pVec, tNorth);
				}
				else {
				  spinAngle = getRotationAngle(pVec, fVec);
				}					
				temp1.set(0.f, 0.f, (F32)texture->mBitmapSize.x, (F32)texture->mBitmapSize.y);
				tempPos = convertWSToGS(tmp->getPosition().asPoint2F(), true);
				tempPos -= asPoint2F(texture->mBitmapSize) / 2;
				temp2.set(rect.point + tempPos, asPoint2F(texture->mBitmapSize));
				drawRotatableBitmap(texture, temp2, temp1, spinAngle); 		
			}
		}
	}
	//Last thing to do is to run through the UAV sequence. Basically, if a UAV is running, we need to scan through each object
	// when it's time to scan and run through the calcs. UAV's only track players, so if it's not a player, we can skip it.
	// First thing we do is render a pretty little line that goes across the screen :P
	if(mUAVFlag) {
		if(mUAVFlag == 1) {
			//Standard "Sweep"
			doUAVSweep(offset, updateRect, 1);
		}
		else {
			doUAVSweep(offset, updateRect, 2);
		}
	}
	GFX->getDrawUtil()->clearBitmapModulation(); //Last thing we must always do!
}

void GuiAdvancedRadarCtrl::doUAVSweep(Point2I offset, const RectI &updateRect, S32 type) {
   GameConnection* conn = GameConnection::getConnectionToServer();
	if (!conn) { 
		return;
	}
   GameBase * control = conn->getControlObject();
	if (!control) {
	   return;
	}
	//
	GFX->getDrawUtil()->setBitmapModulation(mEnemyColor);
	GFXTexHandle elementHandle;
	GFXTextureObject *texture = NULL;
	Point2F tempFlPt;
	//---------------------
	if(type == 1) {
		//Render the lines if we have to, I estimate that this function is being called every 30ms - 32ms when active
		//so by taking in a little factor consideration, we know exactly when this needs to be done.
		S32 sweepFactor = _UAV_LINES * 20;
		if(Sim::getCurrentTime() >= ((UAVSeq + mUAVSweepTime) - sweepFactor)) {
			//We need to be careful with the aboveassumption however, less you want lines randomly jumping.
			//Make sure mUAVSweepTime is NEVER <= sweepFactor, the default is ~600ms, and I doubt you'll have that.
			doingUAVRenderPass = true;
		}
		if(doingUAVRenderPass) {
			S32 width = getExtent().x / _UAV_LINES;
			GFX->getDrawUtil()->drawLine(((F32)offset.x)-2 + (UAVLineCount * width), (F32)offset.y, ((F32)offset.x + (UAVLineCount * width)), ((F32)offset.y + getExtent().y), mFriendlyColor);
			UAVLineCount++;
			if(UAVLineCount >= _UAV_LINES) {
				UAVLineCount = 0;
				doingUAVRenderPass = false;
			}
		}
		if(Sim::getCurrentTime() >= (UAVSeq + mUAVSweepTime)) {
			//Time for update.
			UAVSeq = Sim::getCurrentTime();
			for (SimSetIterator itr(conn); *itr; ++itr) {
				ShapeBase* shape = dynamic_cast< ShapeBase* >(*itr);
				if (shape) {
					if (shape->getShapeIcon() && (shape->getTeam() != dynamic_cast<ShapeBase *>(control)->getTeam())) {
						//Add to the radar list...
						placeOnRadar(shape, NormalObject);
					}
				}
			}
		}
	}
	else {
		//Tracker Mode UAVs (Think Orbital OP.Sat & SR-OP-71 from CoD.... plz nerf)
		for (SimSetIterator itr(conn); *itr; ++itr) {
			ShapeBase* shape = dynamic_cast< ShapeBase* >(*itr);
			if (shape) {
				if (shape->getShapeIcon() && (shape->getTeam() != dynamic_cast<ShapeBase *>(control)->getTeam())) {
					if(shape->getTypeMask() & PlayerObjectType && shape->getShapeIcon() && ((shape->getPosition().asPoint2F() - control->getPosition().asPoint2F()).len() <= mRange)) {
					   if(dStrcmp(dynamic_cast<Player *>(shape)->getStateName(), "Dead") == 0) {
					      continue; //No sense in rendering dead guys...
						}
						arrowPath = StringUnit::getUnit(shape->getShapeIcon(), 1, "\t\n"); 
						elementHandle.set(arrowPath.c_str(), &GFXDefaultGUIProfile, avar("%s() - elementHandle (line %d)", __FUNCTION__, __LINE__));
						texture = elementHandle;
						if(texture) {
							//Arrow direction is determined based on rotation modes from true north tNorth
							pVec = shape->getTransform().getForwardVector();
							if(!mRotateMapLayer) {
								//If the map doesn't rotate the angle is between tNorth and the player							
								spinAngle = getRotationAngle(pVec, tNorth);
							}
							else {
								//If the map rotates, the arrow is always up, so the angle is the v.dif between the player and the other player
								spinAngle = getRotationAngle(pVec, fVec);
							}
							temp1.set(0.f, 0.f, (F32)texture->mBitmapSize.x, (F32)texture->mBitmapSize.y);
							tempPos = convertWSToGS(shape->getPosition().asPoint2F(), true);
							tempPos -= asPoint2F(texture->mBitmapSize) / 2;
							temp2.set(rect.point + tempPos, asPoint2F(texture->mBitmapSize));
							drawRotatableBitmap(texture, temp2, temp1, spinAngle); 							
						}							
					}
				}
			}
		}
	}
}

//updateRadar is called by a NARC event, and is used to set radar parameters by the server.
void GuiAdvancedRadarCtrl::updateRadar(F32 range, S32 uavFl, S32 uavSwT, S32 jam) {
	if(_LOG_ERRORS == 1) {
		Con::printf("::updateRadar(%f, %i, %i, %i)", range, uavFl, uavSwT, jam);
	}
	mRange = range;

	calculateDistanceFactor();

	if(mUAVFlag != uavFl || mUAVSweepTime != uavSwT) {
		UAVLineCount = 0;
	}
	if(mUAVFlag <= 0 && uavFl >= 1) {
		//UAV is now online, grab the current time.
		UAVSeq = Sim::getCurrentTime();
	}
	mUAVFlag = uavFl;
	mUAVSweepTime = uavSwT;
	mJammed = (jam == 1 ? true : false);
	if(_LOG_ERRORS == 1) {
		Con::printf("::updateRadar() - Radar successfully updated");
	}
}

void GuiAdvancedRadarCtrl::placeOnRadar(ShapeBase *object, RadarFlags flag) {
	if(_LOG_ERRORS == 1) {
		Con::printf("::placeOnRadar() - Making a call...");
	}
	// This adds the object to the radar, basically this is a Vector manipulator function that runs through
	//  to check if an object is already on the render list, if it is, we just do a flag update, otherwise
	//  we add it to the list.
	// Temp Allocations
	String add;
	add = String::ToString("%i\t%i\t%f %f", object->getId(), FlagToS32(flag), object->getPosition().x, object->getPosition().y);
	// Check for existing instance of the object:
	for(S32 i = 0; i < objs.size(); i++) {
		if(dAtoi(StringUnit::getUnit(objs[i], 0, "\t\n")) == object->getId()) {
		   //This object is already in the list, just update the field, and by that, we delete element i and add a new instance.
			objs.erase(i, 1);
		}
	}
	objs.push_front(add);
	if(_LOG_ERRORS == 1) {
		Con::printf("::placeOnRadar() - Done!");
	}
}

void GuiAdvancedRadarCtrl::removeFromRadar(ShapeBase *object) {
	if(_LOG_ERRORS == 1) {
		Con::printf("::removeFromRadar() - Called");
	}
	// Removes an object from the Radar vector.
	for(S32 i = 0; i < objs.size(); i++) {
		if(dAtoi(StringUnit::getUnit(objs[i], 0, "\t\n")) == object->getId()) {
		   //This object is already in the list, just update the field, and by that, we delete element i and add a new instance.
			objs.erase(i, 1);
		}
	}
	if(_LOG_ERRORS == 1) {
		Con::printf("::removeFromRadar() - Done");
	}
}

DefineEngineMethod(GuiAdvancedRadarCtrl, setImageLayer, void, (S32 layer, const char *file),, "(S32, String) Set the image layer of the radar") {
	if(layer < 0 || layer > 1) {
		Con::errorf("setImageLayer() - Attemtping to access an invalid image layer of the radar control. Only 0 and 1 exist.");
		return;
	}
   char filename[1024];
   Con::expandScriptFilename(filename, sizeof(filename), file);
	object->setImageLayer(layer, filename);
}

/**
Server Side of the GUI

This is the server's side of this specific GUI control. It handles the NetEvents related to sending controls to the client
to update the GUI based on parameters of the map, or conditions, ect. I mainly am doing it this way to completely minimize the
exterior entry points of the radar control. The less TS entry points open, the less likely the radar itself can be hacked by
outside scripts. Doing it this way also eliminates any possibility that individual clients can access radar parameters without
the authorization of the server beforehand
**/
#ifndef _NET_ARC_EVENT
#define _NET_ARC_EVENT

class NET_AdvancedRadarControl_Event : public NetEvent {
	public:
		typedef NetEvent Parent;

	private:
		char *radarGuiName;
		char *payload;

	public:
		NET_AdvancedRadarControl_Event();
		~NET_AdvancedRadarControl_Event();

		virtual void pack(NetConnection* conn, BitStream *stream);
		virtual void write(NetConnection* conn, BitStream *stream);
		virtual void unpack(NetConnection* conn, BitStream *stream);
		virtual void process(NetConnection *conn);

		void addToRadar(const char *radarName, S32 objectID, S32 cFlag);
		void removeFromRadar(const char *radarName, S32 objectID, S32 cFlag);
		void updateRadar(const char *radarName, F32 range, S32 uavFl, S32 uavSwT, bool jam);

		void set(S32 flg, const char *rgn);

		GuiAdvancedRadarCtrl *getCtrlByName(const char *name);

		DECLARE_CONOBJECT(NET_AdvancedRadarControl_Event);
};

IMPLEMENT_CO_NETEVENT_V1(NET_AdvancedRadarControl_Event);
#endif

NET_AdvancedRadarControl_Event::~NET_AdvancedRadarControl_Event() {
	dFree(radarGuiName);
	dFree(payload);
}
NET_AdvancedRadarControl_Event::NET_AdvancedRadarControl_Event() {
	radarGuiName = NULL;
	payload = NULL;
}

void NET_AdvancedRadarControl_Event::pack(NetConnection *conn, BitStream *stream) {
	if(_LOG_ERRORS == 1) {
		Con::printf("NARC::pack();");
	}
	stream->writeString(radarGuiName);
	stream->writeString(payload);
	if(_LOG_ERRORS == 1) {
		Con::printf("NARC::pack() - done");
	}
}

void NET_AdvancedRadarControl_Event::write(NetConnection *conn, BitStream *stream) {
	pack(conn, stream);
}

void NET_AdvancedRadarControl_Event::unpack(NetConnection *conn, BitStream *stream) {
	if(_LOG_ERRORS == 1) {
		Con::printf("NARC::unpack();");
	}

	char radarName[256], mPayload[256];
	stream->readString(radarName);
	radarGuiName = dStrdup(radarName);
	stream->readString(mPayload);
	payload = dStrdup(mPayload);

	S32 update = dAtoi(StringUnit::getUnit(payload, 0, "\t\n"));
	if(update == 1) {
		//1: Update Radar
   	F32 radarRange = dAtof(StringUnit::getUnit(payload, 1, "\t\n"));
	   S32 uavFlag = dAtoi(StringUnit::getUnit(payload, 2, "\t\n"));
	   S32 uavSweepTime = dAtoi(StringUnit::getUnit(payload, 3, "\t\n"));
	   S32 jammed = dAtoi(StringUnit::getUnit(payload, 4, "\t\n"));
		//Set the control
		updateRadar(radarGuiName, radarRange, uavFlag, uavSweepTime, jammed);
	}
	else {
	   S32 addRemove = dAtoi(StringUnit::getUnit(payload, 1, "\t\n"));
		S32 objectID = dAtoi(StringUnit::getUnit(payload, 2, "\t\n"));
		S32 callFlag = dAtoi(StringUnit::getUnit(payload, 3, "\t\n"));
		//Perform the task.
		addRemove == 1 ? addToRadar(radarGuiName, objectID, callFlag) : removeFromRadar(radarGuiName, objectID, callFlag);
	}
	if(_LOG_ERRORS == 1) {
		Con::printf("NARC::unpack() - done");
	}
}
 
void NET_AdvancedRadarControl_Event::process(NetConnection *conn) {
   //Stub, See my notes on the GG Forums about why using ::unpack instead of ::process can weed out a ton of access violations.
}

void NET_AdvancedRadarControl_Event::set(S32 flg, const char *rgn) {
	if(_LOG_ERRORS == 1) {
		Con::printf("NARC::set(%i, %s)", flg, rgn);
	}
	if(flg == 0) {
		radarGuiName = dStrdup(rgn);   
	}
	else {
		payload = dStrdup(rgn);
	}
	if(_LOG_ERRORS == 1) {
		Con::printf("NARC::set(STR) - done");
	}
}

//Fetches a GuiAdvancedRadarCtrl by it's name, returns NULL if not found or not a Radar Gui.
GuiAdvancedRadarCtrl *NET_AdvancedRadarControl_Event::getCtrlByName(const char *name) {
	SimObject *find = Sim::findObject(name);
	if(find) {
		if(dStrcmp(find->getClassName(), "GuiAdvancedRadarCtrl") == 0) {
			return dynamic_cast<GuiAdvancedRadarCtrl *>(find);
		}
		else {
			if(_LOG_ERRORS == 1) {
				Con::errorf("::getCtrlByName(%s) - Control is not a GuiAdvancedRadarCtrl, %s", name, find->getClassNamespace());
			}
			return NULL;
		}
	}
	return NULL;
}

//These three functions are all recieved and called on the client side of the NetEvent.
void NET_AdvancedRadarControl_Event::addToRadar(const char *radarName, S32 objectID, S32 cFlag) {
	//NOTE!!! We need to use the Ghost Index here as client's will have different object ID's from the server...
	GuiAdvancedRadarCtrl *radar = getCtrlByName(radarName);
	if(radar) {
		//Resolve the ghost ID on the client, stop the function if it isn't ghosted.
		NetObject* pObject = NULL; 
		pObject = NetConnection::getConnectionToServer()->resolveGhost(objectID);
		if(pObject) {
		   radar->placeOnRadar(dynamic_cast<ShapeBase *>(pObject), radar->S32ToFlag(cFlag));
		}
		else {
			Con::errorf("addToRadar() - unable to resolve ghost %i, it may not be ghosted yet...", objectID);
		}
	}
	else {
		Con::errorf("::addToRadar() - Attempting to add %i to %s, however %s does not exist or is of the improper class.", objectID, radarName, radarName);
	}
}

void NET_AdvancedRadarControl_Event::removeFromRadar(const char *radarName, S32 objectID, S32 cFlag) {
	GuiAdvancedRadarCtrl *radar = getCtrlByName(radarName);
	if(radar) {
		//Resolve the ghost ID on the client, stop the function if it isn't ghosted.
		NetObject* pObject = NULL; 
		pObject = NetConnection::getConnectionToServer()->resolveGhost(objectID);
		if(pObject) {
		   radar->removeFromRadar(dynamic_cast<ShapeBase *>(pObject));
		}
		else {
			Con::errorf("removeFromRadar() - unable to resolve ghost %i, it may not be ghosted yet...", objectID);
		}
	}
	else {
		Con::errorf("::removeFromRadar() - Attempting to remove %i from %s, however %s does not exist or is of the improper class.", objectID, radarName, radarName);
	}
}

void NET_AdvancedRadarControl_Event::updateRadar(const char *radarName, F32 range, S32 uavFl, S32 uavSwT, bool jam) {
	GuiAdvancedRadarCtrl *radar = getCtrlByName(radarName);
	if(radar) {
		radar->updateRadar(range, uavFl, uavSwT, jam);
	}
	else {
		Con::errorf("::updateRadar() - Attempting to update %s, however %s does not exist or is of the improper class.", radarName, radarName);
	}
}

//And the relevant TorqueScript calls for this.
//a NARC Event simply defines a type flag and the necessary arguments to handle all three of the above functions.
DefineEngineFunction(SendNARCEvent, void, (S32 tclID, S32 typeFlag, String args),, "Sends NARC events to clients based on typeFlag and args") {
	if(_LOG_ERRORS == 1) {
		Con::printf("SendNARCEvent(%i, %i, %s);", tclID, typeFlag, args.c_str());
	}
	//Con::printf("Looking for object %i", tclID);
	SimObject *find = Sim::findObject(tclID);
	if(find != NULL) {
		//Con::printf("Found Object %i under class %s", tclID, find->getClassName());
	   if(dStrcmp(find->getClassName(), "GameConnection") != 0) {
		   if(_LOG_ERRORS == 1) {
			   Con::errorf("SendNARCEvent(%i) - Control is not a GameConnection, %s", tclID, find->getClassName());
		   }
		   return;		
	   }
	}
	else {
		Con::errorf("SendNARCEvent(%i) - No such object (%i) found.", tclID);
		return;
	}
	NetConnection *tCl = dynamic_cast<NetConnection *>(find);
	NET_AdvancedRadarControl_Event *NARC = new NET_AdvancedRadarControl_Event();
	String radarName, payload;
	S32 serverID = 0, clientID = 0;
	switch(typeFlag) {
		case 0:
			//typeFlag == 0
			//args = String radarName \t S32 arFlag \t S32 objID \t S32 cFlag
         serverID = dAtoi(StringUnit::getUnit(args.c_str(), 2, "\t\n"));
			if(serverID == -1 || Sim::findObject(serverID) == NULL) {
				Con::errorf("Invalid object ID sent to NARC add/remove...");
				return;
			}
			clientID = tCl->getGhostIndex(dynamic_cast<NetObject *>(Sim::findObject(serverID)));
			if(clientID == -1) {
				Con::errorf("NARC Error: Cannot transmit object %i to client %i, it is not ghosted...", serverID, tclID);
			   return;
			}

			radarName = StringUnit::getUnit(args.c_str(), 0, "\t\n");
			payload = String::ToString("0\t%s\t%i\t%s", StringUnit::getUnit(args.c_str(), 1, "\t\n"), clientID, StringUnit::getUnit(args.c_str(), 3, "\t\n"));

			//Create the event
			NARC->set(0, radarName.c_str());
			NARC->set(1, payload.c_str());
			//Send It.
			tCl->postNetEvent(NARC);
			break;

		case 1:
			//typeFlag == 1
			//args = String radarName \t F32 radarRange \t S32 uavFlag \t S32 uavSweepTime \t S32 jammed
			radarName = StringUnit::getUnit(args.c_str(), 0, "\t\n");
			payload = "1\t";
			payload += StringUnit::getUnit(args.c_str(), 1, "\t\n");
			payload += "\t";
			payload += StringUnit::getUnit(args.c_str(), 2, "\t\n");
			payload += "\t";
			payload += StringUnit::getUnit(args.c_str(), 3, "\t\n");
			payload += "\t";
			payload += StringUnit::getUnit(args.c_str(), 4, "\t\n");
			//Create the event
			NARC->set(0, radarName.c_str());
			NARC->set(1, payload.c_str());
			//Send It.
			tCl->postNetEvent(NARC);
			break;

		default:
			Con::errorf("SendNARCEvent() - Invalid event handle flag sent.");
			break;
	}
}
