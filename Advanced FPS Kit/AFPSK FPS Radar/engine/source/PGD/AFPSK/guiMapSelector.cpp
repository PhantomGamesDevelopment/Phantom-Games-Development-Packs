//-----------------------------------------------------------------------------
// guiMapSelector.cpp
// (C) 2014 Robert Fritzen for Phantom Games Development
// Source file for the map selection control for the Advanced FPS Kit for T3D MIT 3.5
//
// This code file renders a map image from which a player may select points on the map and
// have the 3D world position be returned.
//-----------------------------------------------------------------------------

#include "platform/platform.h"
#include "PGD/AFPSK/guiMapSelector.h"
#include "gui/3d/guiTSControl.h"
#include "gui/core/guiCanvas.h"
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
#include "T3D/missionArea.h"
#include "T3D/missionMarker.h"
#include "T3D/player.h"
#include "windowManager/platformWindow.h"

/**
Additional Functions
**/
Point2F GuiMapSelectorCtrl::convertGStoWS(Point2F guiSpace) {
	if(missionArea.len_x() == 0 && missionArea.len_y() == 0) {
		Con::errorf("convertGStoWS() - Cannot convert space coordinates without reference object (MissionArea).");
		return Point2F(0, 0);
	}

	Point2F outCoords;
	outCoords.x = guiSpace.x / distanceFactorX;
	outCoords.y = guiSpace.y / distanceFactorY;

	outCoords.y = missionArea.extent.y - outCoords.y;
	outCoords -= translatedMissionArea;

	return outCoords;
}

Point2F GuiMapSelectorCtrl::convertWSToGS(Point2F worldSpace) {
	if(missionArea.len_x() == 0 && missionArea.len_y() == 0) {
		Con::errorf("convertWSToGS() - Cannot convert space coordinates without reference object (MissionArea).");
		return Point2F(0, 0);
	}

	Point2F outCoords;
	outCoords.x = (worldSpace.x + translatedMissionArea.x);
	outCoords.y = missionArea.extent.y - (worldSpace.y + translatedMissionArea.y);
	//------------------------------------------------
	outCoords.x *= distanceFactorX; 
	outCoords.y *= distanceFactorY;
	return outCoords;
}

F32 GuiMapSelectorCtrl::getTNorthRotation(VectorF v1) {  
   // v1 must be normalized  
   v1.z = 0.0f;  
   if(!v1.isUnitLength()) {    
      v1.normalizeSafe();    
   }    
   F32 ang = atan2(v1.y, v1.x) - atan2(1.0f, 0.0f);
	return ang < 0 ? ang + M_2PI : ang;
} 

F32 GuiMapSelectorCtrl::getPointAngle(Point2F p1, Point2F p2) {
	F32 dX = p2.x - p1.x;
	F32 dY = p2.y - p1.y;

   F32 ang = atan2(dY, dX);
	return (ang - (M_PI / 2));
}

void GuiMapSelectorCtrl::drawRotatableBitmap(GFXTextureObject* texture, const RectF &dstRect, const RectF &srcRect, F32 spinAngle, const GFXBitmapFlip in_flip = GFXBitmapFlip_None) {
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


/**
End
**/

IMPLEMENT_CONOBJECT(GuiMapSelectorCtrl);

GuiMapSelectorCtrl::GuiMapSelectorCtrl(void) : 
   mMapLayerName(),
	mSelectorCursorName(),
	mSelectorDCursName(),
	mUAVFlag(0),
	mFriendlyColor(0, 255, 0),
	mEnemyColor(255, 0, 0),
	mNeutralColor(255, 255, 255),
	spinAngle(0),
	currentSelection(CURSOR),
	needsDirection(false),
	canRender(false),
	noCursorSelect(false),
	tracking(false) {

		lastPosition.set(0, 0);
		lastPosition2 = lastPosition;
		pVec.set(0, 0, 0);
}

void GuiMapSelectorCtrl::initPersistFields() {
   addGroup( "Bitmap" );

      addField( "mapLayer",   TypeImageFilename,     Offset( mMapLayerName, GuiMapSelectorCtrl),
         "The map layer of the map, use setImageLayer(0, name) to set. CHANGING THIS HAS NO EFFECT!!!" );
      addField( "selectionCursor",   TypeImageFilename,     Offset( mSelectorCursorName, GuiMapSelectorCtrl),
         "The selector cursor of the map, use setImageLayer(1, name) to set. CHANGING THIS HAS NO EFFECT!!!" );
      addField( "directionCursor",   TypeImageFilename,     Offset( mSelectorDCursName, GuiMapSelectorCtrl),
         "The direction selection cursor of the map, use setImageLayer(2, name) to set. CHANGING THIS HAS NO EFFECT!!!" );

   endGroup( "Bitmap" );

	addGroup( "Map" );
		addField( "friendlyColor", TypeColorI, Offset( mFriendlyColor, GuiMapSelectorCtrl), 
			"Color of friendly elements rendered on the map.");
		addField( "neutralColor", TypeColorI, Offset( mNeutralColor, GuiMapSelectorCtrl), 
			"Color of neutral elements rendered on the map.");
		addField( "enemyColor", TypeColorI, Offset( mEnemyColor, GuiMapSelectorCtrl), 
			"Color of enemy elements rendered on the map.");
	endGroup( "Map" );

   Parent::initPersistFields();
}

bool GuiMapSelectorCtrl::onWake() {
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

   currentSelection = CURSOR; //<-- Reset Vars
	lastPosition = Point2F(0, 0);
	lastPosition2 = Point2F(0, 0);

	//translates mission area to image coords and player position to fit in the translated area.
	translatedMissionArea.set(obj->getArea().point.x*-1, obj->getArea().point.y*-1);
	missionArea.set(0, 0, obj->getArea().extent.x, obj->getArea().extent.y);
	//And let us render.
	canRender = true;
   setActive(true);
   setImageLayer(0, mMapLayerName);
	setImageLayer(1, mSelectorCursorName);
	setImageLayer(2, mSelectorDCursName);

	calculateDistanceFactor();
   return true;
}

void GuiMapSelectorCtrl::onSleep() {
	if ( !mMapLayerName.equal("texhandle", String::NoCase) ) {
      mMapLayerTO = NULL;
	}
	if ( !mSelectorCursorName.equal("texhandle", String::NoCase) ) {
      mCursorObject = NULL;
	}
	if ( !mSelectorDCursName.equal("texhandle", String::NoCase) ) {
      mDCursorObject = NULL;
	}

   Parent::onSleep();
}

void GuiMapSelectorCtrl::setImageLayer(S32 layer, const char *name) {
	if(!Platform::isFile(name)) {
		Con::errorf("::setImageLayer(%i) - Invalid File %s", layer, name);
		return;
	}

	switch(layer) {
		case 0:
	      mMapLayerName = name;
	      if ( !isAwake() )
		      return;
	      if ( mMapLayerName.isNotEmpty() ) 
		      mMapLayerTO.set( mMapLayerName, &GFXDefaultGUIProfile, avar("%s() - mMapLayerTO (line %d)", __FUNCTION__, __LINE__) );
	      else
		      mMapLayerTO = NULL;
			break;

		case 1:
	      mSelectorCursorName = name;
	      if ( !isAwake() )
		      return;
	      if ( mSelectorCursorName.isNotEmpty() ) 
		      mCursorObject.set( mSelectorCursorName, &GFXDefaultGUIProfile, avar("%s() - mCursorObject (line %d)", __FUNCTION__, __LINE__) );
	      else
		      mCursorObject = NULL;
			break;

		case 2:
	      mSelectorDCursName = name;
	      if ( !isAwake() )
		      return;
	      if ( mSelectorDCursName.isNotEmpty() ) 
		      mDCursorObject.set( mSelectorDCursName, &GFXDefaultGUIProfile, avar("%s() - mDCursorObject (line %d)", __FUNCTION__, __LINE__) );
	      else
		      mDCursorObject = NULL;
			break;
	}

	setUpdate();
}

void GuiMapSelectorCtrl::inspectPostApply() {
   // if the extent is set to (0,0) in the gui editor and appy hit, this control will
   // set it's extent to be exactly the size of the bitmap (if present)
   Parent::inspectPostApply();

   if ((getExtent().x == 0) && (getExtent().y == 0) && mMapLayerTO) {
      setExtent( mMapLayerTO->getWidth(), mMapLayerTO->getHeight());
   }

	inExtent.set(getExtent().x, getExtent().y);
}

void GuiMapSelectorCtrl::updateSizing() {
	if(!getParent()) {
      return;
	}

   // updates our bounds according to our horizSizing and verSizing rules
   RectI fakeBounds( getPosition(), getParent()->getExtent());

	calculateDistanceFactor();

   parentResized( fakeBounds, fakeBounds);
}

void GuiMapSelectorCtrl::calculateDistanceFactor() {
	//Distance factor = 1 unit of gui space / 1 world unit
	distanceFactorX = F32(getExtent().x) / (missionArea.extent.x);
	distanceFactorY = F32(getExtent().y) / (missionArea.extent.y);
}

void GuiMapSelectorCtrl::onRender(Point2I offset, const RectI &updateRect) {
	//Pre-Render Checks
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
      setImageLayer(0, mMapLayerName);
	   setImageLayer(1, mSelectorCursorName);
	   setImageLayer(2, mSelectorDCursName);

		calculateDistanceFactor();
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
	//
	inOffset.set(offset.x, offset.y);
	GFXTextureObject* texture = NULL;
	GFXTexHandle elementHandle;
	rect.set(updateRect.point.x, updateRect.point.y, updateRect.extent.x, updateRect.extent.y);
	//Drawing the map is simple, since all we are doing is filling the box with the map image...
   if (mMapLayerTO) {
		spinAngle = 0.0f;
		GFX->getDrawUtil()->clearBitmapModulation();
		temp1.set( 0.f, 0.f, (F32)mMapLayerTO->mBitmapSize.x, (F32)mMapLayerTO->mBitmapSize.y );
		texture = mMapLayerTO;
		//Draw the map to the rectangle.
		GFX->getDrawUtil()->drawBitmapStretchSR(texture, rect, temp1);
      //Draw the location of all friendly elements...
		for (SimSetIterator itr(conn); *itr; ++itr) {
			ShapeBase* shape = dynamic_cast< ShapeBase* >(*itr);
			if (shape) {
				if (shape->getShapeIcon()) {
					pVec = shape->getTransform().getForwardVector();
					spinAngle = getTNorthRotation(pVec);
					//Waypoint Support
               if (dStrcmp(shape->getClassName(), "WayPoint") == 0) {
					   arrowPath = shape->getShapeIcon();
						elementHandle.set(arrowPath.c_str(), &GFXDefaultGUIProfile, avar("%s() - elementHandle (line %d)", __FUNCTION__, __LINE__));
						texture = elementHandle;
						//Note: If you want to use enemy waypoints, you'll need to modify the below if statement.
						if(texture && (dynamic_cast<WayPoint *>(shape)->mTeam.mTeamId == dynamic_cast<ShapeBase *>(control)->getTeam() 
							&& dynamic_cast<WayPoint *>(shape)->mTeam.mTeamId == 0)) {
							temp1.set(0.f, 0.f, (F32)texture->mBitmapSize.x, (F32)texture->mBitmapSize.y);
							tempPos = convertWSToGS(shape->getPosition().asPoint2F());
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
					if(shape->getTeam() == dynamic_cast<ShapeBase *>(control)->getTeam()) {
						GFX->getDrawUtil()->setBitmapModulation(mFriendlyColor);
						if(shape->getTypeMask() & PlayerObjectType) {
							if(dStrcmp(dynamic_cast<Player *>(shape)->getStateName(), "Dead") == 0) {
							   continue; //No sense in rendering dead guys...
							}
							//Player Objects have a tabbed string ShapeIcon field, containing the enemy "dot" and the "arrow".
							arrowPath = StringUnit::getUnit(shape->getShapeIcon(), 1, "\t\n");
						}
						else {
						   arrowPath = shape->getShapeIcon();
						}							
						elementHandle.set(arrowPath.c_str(), &GFXDefaultGUIProfile, avar("%s() - elementHandle (line %d)", __FUNCTION__, __LINE__)); 
						texture = elementHandle;
						if(texture) {
							temp1.set(0.f, 0.f, (F32)texture->mBitmapSize.x, (F32)texture->mBitmapSize.y);
							tempPos = convertWSToGS(shape->getPosition().asPoint2F());
							tempPos -= asPoint2F(texture->mBitmapSize) / 2;
							temp2.set(rect.point + tempPos, asPoint2F(texture->mBitmapSize));
							drawRotatableBitmap(texture, temp2, temp1, spinAngle); 							
						}
					}
					else {
					   //Then we check for UAV flag to see if we need to render enemies
						if(mUAVFlag) {
							if(shape->getTeam() == 0) {
							   GFX->getDrawUtil()->setBitmapModulation(mNeutralColor);
							}
							else {
					   	   GFX->getDrawUtil()->setBitmapModulation(mEnemyColor);
							}
							//
						   if(shape->getTypeMask() & PlayerObjectType) {
							   if(dStrcmp(dynamic_cast<Player *>(shape)->getStateName(), "Dead") == 0) {
							      continue; //No sense in rendering dead guys...
							   }
							   //Player Objects have a tabbed string ShapeIcon field, containing the enemy "dot" and the "arrow".
							   arrowPath = StringUnit::getUnit(shape->getShapeIcon(), 1, "\t\n");
						   }
						   else {
						      arrowPath = shape->getShapeIcon();
						   }							
						   elementHandle.set(arrowPath.c_str(), &GFXDefaultGUIProfile, avar("%s() - elementHandle (line %d)", __FUNCTION__, __LINE__)); 
						   texture = elementHandle;
						   if(texture) {
							   temp1.set(0.f, 0.f, (F32)texture->mBitmapSize.x, (F32)texture->mBitmapSize.y);
							   tempPos = convertWSToGS(shape->getPosition().asPoint2F());
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
	GFX->getDrawUtil()->clearBitmapModulation();
	//Now we do our cursor stuff...
	if(mCursorObject && currentSelection != NOCURSOR) {
	   if(currentSelection == CURSOR) {
	      //Current mode is cursor
		   //First step is to check if we're currently tracking the mouse or not, if we are, it moves, if not, the position remains static
		   texture = mCursorObject;
			temp1.set(0.f, 0.f, (F32)texture->mBitmapSize.x, (F32)texture->mBitmapSize.y);
			tempPos = lastPosition;
			tempPos -= asPoint2F(texture->mBitmapSize) / 2;
			temp2.set(rect.point + tempPos, asPoint2F(texture->mBitmapSize));
		   //Render the cursor on the position
		   GFX->getDrawUtil()->drawBitmapStretchSR(texture, temp2, temp1);
	   }
	   else {
	      //Current mode is direction
			if(mDCursorObject) {
			   texture = mDCursorObject;
				spinAngle = getPointAngle(convertGStoWS(lastPosition), convertGStoWS(lastPosition2));
			   temp1.set(0.f, 0.f, (F32)texture->mBitmapSize.x, (F32)texture->mBitmapSize.y);
			   tempPos = lastPosition;
			   tempPos -= asPoint2F(texture->mBitmapSize) / 2;
			   temp2.set(rect.point + tempPos, asPoint2F(texture->mBitmapSize));
				drawRotatableBitmap(texture, temp2, temp1, spinAngle);
			}
	   }
	}
   
	//Note To All: You can easily interface the radar UAV with this by adding a command on the radar to push an update command here
	// when UAV's turn off and on if you want to combine the functioning. I had originally intended to do so, but timing constraints
	// due to the radar axis controversey prevented me from doing so.

	//Phantom139: I have no idea why you'd event want "child" controls on the map. It's more or less it's own entity
	//that can handle rendering everything it needs by itself, and thus, we ignore this generic T3D code portion.
	//Feel free to uncomment if you feel the need to add controls to your map, I highly doubt it. :)
   //renderChildControls(offset, updateRect);
}

void GuiMapSelectorCtrl::calculateSelection() {
   //This code bit handles all of the GS -> WS stuff as well as console exposure.
	if(!needsDirection) {
		tempPos = convertGStoWS(lastPosition);
	   Con::executef(this, "onSelectPosition", String::ToString("%f %f", tempPos.x, tempPos.y));
	}
	else {
	   if(currentSelection == CURSOR) {
		   tempPos = convertGStoWS(lastPosition);
	      Con::executef(this, "onSelectPosition", String::ToString("%f %f", tempPos.x, tempPos.y));		
		}
		else {
			if(lastPosition == lastPosition2) {
			   lastPosition2 += Point2F(1, 1); 
				Con::errorf("lastPos1 == lastPos2, adding 1,1 to vector lastPos2...");
			}
		   tempPos = convertGStoWS(lastPosition2);
	      Con::executef(this, "onSelectSecondPosition", String::ToString("%f %f", tempPos.x, tempPos.y));		
		}
	}
}

// Left Click
void GuiMapSelectorCtrl::onMouseDown(const GuiEvent &event) {
	if(currentSelection == NOCURSOR) {
		//Do Nothing.
	   return;
	}

	if(!needsDirection) {
	   //send the position to our call
		lastPosition = asPoint2F(event.mousePoint) - rect.point;
		calculateSelection();
	}
	else {
	   //switch the cursor to directional
		if(currentSelection == CURSOR) {
		   lastPosition = asPoint2F(event.mousePoint) - rect.point;
			calculateSelection();
			currentSelection = DIRECTION;
		}
		else {
		   lastPosition2 = asPoint2F(event.mousePoint) - rect.point;
			calculateSelection();
		}		
	}
   //mMouseState = MovingLight;
   //mLastMousePoint = event.mousePoint;
   mouseLock();
}

// Left Click Release
void GuiMapSelectorCtrl::onMouseUp(const GuiEvent &event) {
	if(currentSelection == NOCURSOR) {
		//Do Nothing.
	   return;
	}

   mouseUnlock();
   //mMouseState = None;
}

void GuiMapSelectorCtrl::onMouseEnter(const GuiEvent & event) {
   //Switch on Cursor & Tracking
   tracking = true;
}

void GuiMapSelectorCtrl::onMouseLeave(const GuiEvent & event) {
   //Switch off Cursor & Tracking
   tracking = false;
}

void GuiMapSelectorCtrl::onMouseMove(const GuiEvent &event) {
	if(tracking) {
	   if(currentSelection == CURSOR) {
			lastPosition = asPoint2F(event.mousePoint) - rect.point;
		}
		else {
		   lastPosition2 = asPoint2F(event.mousePoint) - rect.point;
		}
	}
}

DefineEngineMethod(GuiMapSelectorCtrl, setImageLayer, void, (S32 layer, const char *file),, "(S32, String) Set the image layer of the map") {
	if(layer < 0 || layer > 2) {
		Con::errorf("setImageLayer() - Attemtping to access an invalid image layer of the radar control. Only 0, 1, and 2 exist.");
		return;
	}
   char filename[1024];
   Con::expandScriptFilename(filename, sizeof(filename), file);
	object->setImageLayer(layer, filename);
}

//Really should do this via NetEvent, but we should be fine...
DefineEngineMethod(GuiMapSelectorCtrl, setControlOptions, void, (S32 uavFlag, bool needsRot, bool useCursor),, "") {
	object->applySettings(uavFlag, needsRot, useCursor);
}