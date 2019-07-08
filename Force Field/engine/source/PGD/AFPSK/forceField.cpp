#include "forceField.h"
#include "T3D/shapeBase.h"
#include "math/mathIO.h"
#include "scene/sceneRenderState.h"
#include "core/stream/bitStream.h"
#include "materials/sceneData.h"
#include "gfx/gfxDebugEvent.h"
#include "gfx/gfxTransformSaver.h"
#include "renderInstance/renderPassManager.h"
#include "console/console.h"
#include "console/consoleTypes.h"
#include "console/engineAPI.h"
#include "collision/boxConvex.h"
#include "collision/earlyOutPolyList.h"
#include "collision/extrudedPolyList.h"

IMPLEMENT_CO_NETOBJECT_V1(ForceField);

IMPLEMENT_CALLBACK( ForceField, onCollision, void, (ForceField* obj, SceneObject *collObj, VectorF vec, F32 len), ( obj, collObj, vec, len ),
   "@brief Called when we collide with another object.\n\n"
   "@param obj The ForceField Instance\n"
   "@param collObj The object we collided with\n"
   "@param vec Collision impact vector\n"
   "@param len Length of the impact vector\n" );

Chunker<ForceField::CollisionTimeout> sTimeoutChunker;
ForceField::CollisionTimeout* ForceField::sFreeTimeoutList = 0;

ForceField::ForceField() : mConvexList( new Convex ), mTimeoutList(NULL) {
   mNetFlags.set(Ghostable | ScopeAlways);
   mTypeMask |= StaticObjectType | StaticShapeObjectType;

   mConvex.init(this);
   mWorkingQueryBox.minExtents.set(-1e9, -1e9, -1e9);
   mWorkingQueryBox.maxExtents.set(-1e9, -1e9, -1e9);

	mColor.set(1.0f, 1.0f, 1.0f, 1.0f);
	mIsTeamPass = false;
	mIsAllPass = false;
	mIsEnabled = true;
	mAllowPlayers = true;
	mAllowVehicles = false;
	mAllowItems = false;
	mTeam = 0;
	//mPassMask = PlayerObjectType;
}

ForceField::~ForceField() {
	SAFE_DELETE(mConvexList);

   CollisionTimeout* ptr = mTimeoutList;
   while (ptr) {
      CollisionTimeout* cur = ptr;
      ptr = ptr->next;
      cur->next = sFreeTimeoutList;
      sFreeTimeoutList = cur;
   }
}

void ForceField::initPersistFields() {
	addGroup("Force Field Parameters");

		addProtectedField("color", TypeColorF, Offset(mColor, ForceField), &_setColor, &defaultProtectedGetFn,
         "The color modulation applied to the force-field" );
		addProtectedField("isTeamPass", TypeBool, Offset(mIsTeamPass, ForceField), &_setTeamPass, &defaultProtectedGetFn,
         "Can friendly objects of mPassMask pass through the field?" );
		addProtectedField("isAllPass", TypeBool, Offset(mIsAllPass, ForceField), &_setAllPass, &defaultProtectedGetFn,
         "Can any object of mPassMask pass through the field?" );
		addProtectedField("isEnabled", TypeBool, Offset(mIsEnabled, ForceField), &_setEnabled, &defaultProtectedGetFn,
         "Is this force field active?" );
		addProtectedField("allowPlayers", TypeBool, Offset(mAllowPlayers, ForceField), &_setAllowPlayers, &defaultProtectedGetFn,
         "Does this force field allow player objects through if they fit the parameters of isTeamPass/isAllPass?" );
		addProtectedField("allowVehicles", TypeBool, Offset(mAllowVehicles, ForceField), &_setAllowVehicles, &defaultProtectedGetFn,
         "Does this force field allow vehicle objects through if they fit the parameters of isTeamPass/isAllPass?" );
		addProtectedField("allowItems", TypeBool, Offset(mAllowItems, ForceField), &_setAllowItems, &defaultProtectedGetFn,
         "Does this force field allow item objects through if they fit the parameters of isTeamPass/isAllPass?" );
		addProtectedField("team", TypeS32, Offset(mTeam, ForceField), &_setTeam, &defaultProtectedGetFn,
         "The team this force field instance is on." );

	endGroup("Force Field Parameters");

   Parent::initPersistFields();
}

bool ForceField::onAdd() {
	if (!Parent::onAdd()) {
      return false;
	}

   mObjBox.set(Point3F(-0.5f, -0.5f, -0.5f), Point3F(0.5f, 0.5f, 0.5f));

   resetWorldBox();
   addToScene();

	//Setup the collision info...
   mObjBox.getCenter(&mConvex.mCenter);
   mConvex.mSize.x = mObjBox.len_x() / 2.0;
   mConvex.mSize.y = mObjBox.len_y() / 2.0;
   mConvex.mSize.z = mObjBox.len_z() / 2.0;
   mWorkingQueryBox.minExtents.set(-1e9, -1e9, -1e9);
   mWorkingQueryBox.maxExtents.set(-1e9, -1e9, -1e9);

   return true;
}

void ForceField::onRemove() {
   removeFromScene();
   Parent::onRemove();

   mWorkingQueryBox.minExtents.set(-1e9, -1e9, -1e9);
   mWorkingQueryBox.maxExtents.set(-1e9, -1e9, -1e9);
}

void ForceField::setTransform(const MatrixF & mat) {
   Parent::setTransform(mat);
   setMaskBits(TransformMask);
}

U32 ForceField::packUpdate(NetConnection *conn, U32 mask, BitStream *stream) {
   U32 retMask = Parent::packUpdate( conn, mask, stream );
	//And now write updated field masks
   if (stream->writeFlag(mask & TransformMask)) {
      mathWrite(*stream, getTransform());
      mathWrite(*stream, getScale());
   }
	if (stream->writeFlag(mask & ColorMask)) {
		stream->writeFloat(mColor.red, 8);
		stream->writeFloat(mColor.green, 8);
		stream->writeFloat(mColor.blue, 8);
		stream->writeFloat(mColor.alpha, 8);
	}
	if (stream->writeFlag(mask & InfoMask)) { 
		stream->writeFlag(mIsTeamPass);
		stream->writeFlag(mIsAllPass);
		stream->writeFlag(mIsEnabled);
		stream->writeFlag(mAllowPlayers);
		stream->writeFlag(mAllowVehicles);
		stream->writeFlag(mAllowItems);
		stream->writeInt(mTeam, 8);
		//stream->write(mPassMask);
	}

   return retMask;
}

void ForceField::unpackUpdate(NetConnection *conn, BitStream *stream) {
   Parent::unpackUpdate(conn, stream);
	if (stream->readFlag()) {  // TransformMask
      mathRead(*stream, &mObjToWorld);
      mathRead(*stream, &mObjScale);
      setTransform( mObjToWorld );
		//Position/Scale updates require a change in the field's geometry, do this now.
		mVertexBuffer = NULL;
   }
	if (stream->readFlag()) {  // ColorMask
		mColor.red = stream->readFloat(8);
		mColor.green = stream->readFloat(8);
		mColor.blue = stream->readFloat(8);
		mColor.alpha = stream->readFloat(8);
		//Color updates require a change in the field's geometry, do this now.
		mVertexBuffer = NULL;
	}
	if (stream->readFlag()) {  // InfoMask
		mIsTeamPass = stream->readFlag();
		mIsAllPass = stream->readFlag();
		mIsEnabled = stream->readFlag();
		mAllowPlayers = stream->readFlag();
		mAllowVehicles = stream->readFlag();
		mAllowItems = stream->readFlag();
		mTeam = stream->readInt(8);
		//stream->read(&mPassMask);		
	}
}

void ForceField::processTick(const Move *move) {
	Parent::processTick(move);
}

static MatrixF FFMat(1);
bool ForceField::buildPolyList(PolyListContext context, AbstractPolyList* polyList, const Box3F&, const SphereF&) {
   Point3F pos;
   mObjToWorld.getColumn(3,&pos);
   FFMat.setColumn(3,pos);
   polyList->setTransform(&FFMat, mObjScale);
   polyList->setObject(this);
   polyList->addBox(mObjBox);
   return true;
}

void ForceField::buildConvex(const Box3F& box, Convex* convex) {
	if(!mIsEnabled) {
	   //If there's no forcefield active, then it's nothing there :)
		return;
	}


   mConvexList->collectGarbage();

	if (box.isOverlapped(getWorldBox()) == false) {
      return;
	}
   Convex* cc = 0;
   CollisionWorkingList& wl = convex->getWorkingList();
   for (CollisionWorkingList* itr = wl.wLink.mNext; itr != &wl; itr = itr->wLink.mNext) {
      if (itr->mConvex->getType() == BoxConvexType && itr->mConvex->getObject() == this) {
         cc = itr->mConvex;
         break;
      }
   }
	if (cc) {
      return;
	}

   // Create a new convex.
   BoxConvex* cp = new BoxConvex;
   mConvexList->registerObject(cp);
   convex->addToWorkingList(cp);
   cp->init(this);

   mObjBox.getCenter(&cp->mCenter);
   cp->mSize.x = mObjBox.len_x() / 2.0f;
   cp->mSize.y = mObjBox.len_y() / 2.0f;
   cp->mSize.z = mObjBox.len_z() / 2.0f;
}

bool ForceField::shouldCollide(ShapeBase *object) {
	bool isOnSameTeam = (object->getTeam() == mTeam);
	U32 targetMask = object->getTypeMask();
	bool isOfSafeMask = (((targetMask & PlayerObjectType) && mAllowPlayers) ||
		((targetMask & VehicleObjectType) && mAllowVehicles) ||
		((targetMask & ItemObjectType) && mAllowItems));
	if(isOfSafeMask) {
		if(mIsAllPass) {
		   return false;
		}
		if(isOnSameTeam && mIsTeamPass) {
		   return false;
		}
	}
	return true;
}

void ForceField::queueCollision(ForceField *me, ShapeBase *obj, const VectorF &vec) {
   // Add object to list of collisions.
   SimTime time = Sim::getCurrentTime();
   S32 num = obj->getId();
	//Setup some vars....
	if(!shouldCollide(obj)) {
		return;
	}
	CollisionTimeout** adr = &mTimeoutList;
   CollisionTimeout* ptr = mTimeoutList;
   while (ptr) {
      if (ptr->objectNumber == num) {
         if (ptr->expireTime < time) {
            ptr->expireTime = time + CollisionTimeoutValue;
            ptr->object = obj;
            ptr->vector = vec;
         }
         return;
      }
      // Recover expired entries
      if (ptr->expireTime < time) {
         CollisionTimeout* cur = ptr;
         *adr = ptr->next;
         ptr = ptr->next;
         cur->next = sFreeTimeoutList;
         sFreeTimeoutList = cur;
      }
      else {
         adr = &ptr->next;
         ptr = ptr->next;
      }
   }
   if (sFreeTimeoutList != NULL) {
      ptr = sFreeTimeoutList;
      sFreeTimeoutList = ptr->next;
      ptr->next = NULL;
   }
   else {
      ptr = sTimeoutChunker.alloc();
   }

   ptr->object = obj;
   ptr->objectNumber = obj->getId();
   ptr->vector = vec;
   ptr->expireTime = time + CollisionTimeoutValue;
   ptr->next = mTimeoutList;

   mTimeoutList = ptr;
}

void ForceField::notifyCollision() {
   SimTime expireTime = Sim::getCurrentTime() + CollisionTimeoutValue;
   for (CollisionTimeout* ptr = mTimeoutList; ptr; ptr = ptr->next) {
      if (ptr->expireTime == expireTime && ptr->object) {
         SimObjectPtr<SceneObject> safePtr(ptr->object);
         SimObjectPtr<SceneObject> safeThis(this);
         onCollision(ptr->object, ptr->vector);
         ptr->object = 0;

         if(!bool(safeThis))
            return;

         if(bool(safePtr))
            safePtr->onCollision(this,ptr->vector);

         if(!bool(safeThis))
            return;
      }
   }
}

void ForceField::onCollision(SceneObject *object, const VectorF &vec) {
	if (!isGhost()) {
		onCollision_callback(this, object, vec, vec.len());
	}
}

void ForceField::createGeometry() {
   static const Point3F cubePoints[8] = {
      Point3F( 1.0f, -1.0f, -1.0f), Point3F( 1.0f, -1.0f,  1.0f),
      Point3F( 1.0f,  1.0f, -1.0f), Point3F( 1.0f,  1.0f,  1.0f),
      Point3F(-1.0f, -1.0f, -1.0f), Point3F(-1.0f,  1.0f, -1.0f),
      Point3F(-1.0f, -1.0f,  1.0f), Point3F(-1.0f,  1.0f,  1.0f)
   };

   static const Point3F cubeNormals[6] = {
      Point3F( 1.0f,  0.0f,  0.0f), Point3F(-1.0f,  0.0f,  0.0f),
      Point3F( 0.0f,  1.0f,  0.0f), Point3F( 0.0f, -1.0f,  0.0f),
      Point3F( 0.0f,  0.0f,  1.0f), Point3F( 0.0f,  0.0f, -1.0f)
   };

   static const U32 cubeFaces[36][3] = {
      { 3, 0, 0 }, { 0, 0, 0 }, { 1, 0, 0 },
      { 2, 0, 0 }, { 0, 0, 0 }, { 3, 0, 0 },
      { 7, 1, 0 }, { 4, 1, 0 }, { 5, 1, 0 },
      { 6, 1, 0 }, { 4, 1, 0 }, { 7, 1, 0 },
      { 3, 2, 1 }, { 5, 2, 1 }, { 2, 2, 1 },
      { 7, 2, 1 }, { 5, 2, 1 }, { 3, 2, 1 },
      { 1, 3, 1 }, { 4, 3, 1 }, { 6, 3, 1 },
      { 0, 3, 1 }, { 4, 3, 1 }, { 1, 3, 1 },
      { 3, 4, 2 }, { 6, 4, 2 }, { 7, 4, 2 },
      { 1, 4, 2 }, { 6, 4, 2 }, { 3, 4, 2 },
      { 2, 5, 2 }, { 4, 5, 2 }, { 0, 5, 2 },
      { 5, 5, 2 }, { 4, 5, 2 }, { 2, 5, 2 }
   };

   // Fill the vertex buffer
   GFXVertexPCN *pVert = NULL;

   mVertexBuffer.set( GFX, 36, GFXBufferTypeStatic );
   pVert = mVertexBuffer.lock();

   Point3F halfSize = getObjBox().getExtents() * 0.5f;

   for (U32 i = 0; i < 36; i++) {
      const U32& vdx = cubeFaces[i][0];
      const U32& ndx = cubeFaces[i][1];

      pVert[i].point  = cubePoints[vdx] * halfSize;
      pVert[i].normal = cubeNormals[ndx];
      pVert[i].color  = mColor;
   }

   mVertexBuffer.unlock();

   GFXStateBlockDesc desc;
	desc.setBlend( true, GFXBlendSrcAlpha, GFXBlendOne);
   mStateBlockRef = GFX->createStateBlock( desc );
}

void ForceField::prepRenderImage(SceneRenderState *state) {
	if(!mIsEnabled) {
	   //If this force-field is offline, don't mess with the render function...
		return;
	}
	if (mVertexBuffer.isNull()) {
      createGeometry();
	}

   ObjectRenderInst *ri = state->getRenderPass()->allocInst<ObjectRenderInst>();
   ri->renderDelegate.bind( this, &ForceField::render );
   ri->type = RenderPassManager::RIT_Object;
   ri->defaultKey = 0;
   ri->defaultKey2 = 0;
   state->getRenderPass()->addInst( ri );
}

void ForceField::render( ObjectRenderInst *ri, SceneRenderState *state, BaseMatInstance *overrideMat ) {
	if (overrideMat) {
      return;
	}

	if (mVertexBuffer.isNull()) {
      return;
	}

   GFXTransformSaver saver;
   MatrixF objectToWorld = getRenderTransform();
   objectToWorld.scale( getScale() );
   GFX->multWorld( objectToWorld );
   GFX->setStateBlock( mStateBlockRef );
   GFX->setupGenericShaders( GFXDevice::GSModColorTexture );
   GFX->setVertexBuffer( mVertexBuffer );
   GFX->drawPrimitive( GFXTriangleList, 0, 12 );
}

bool ForceField::_setColor(void *object, const char *index, const char *data) {
   ForceField* so = static_cast<ForceField*>(object);
   if (so) {
      ColorF col;
      Con::setData(TypeColorF, &col, 0, 1, &data);
      so->setColor(col);
   }
   return false;
}

bool ForceField::_setTeamPass(void *object, const char *index, const char *data) {
   ForceField* so = static_cast<ForceField*>(object);
   if (so) {
      bool set;
      Con::setData(TypeBool, &set, 0, 1, &data);
      so->setTeamPass(set);
   }
   return false;
}

bool ForceField::_setAllPass(void *object, const char *index, const char *data) {
   ForceField* so = static_cast<ForceField*>(object);
   if (so) {
      bool set;
      Con::setData(TypeBool, &set, 0, 1, &data);
      so->setAllPass(set);
   }
   return false;
}

bool ForceField::_setEnabled(void *object, const char *index, const char *data) {
   ForceField* so = static_cast<ForceField*>(object);
   if (so) {
      bool set;
      Con::setData(TypeBool, &set, 0, 1, &data);
      so->setEnabled(set);
   }
   return false;
}

bool ForceField::_setAllowPlayers(void *object, const char *index, const char *data) {
   ForceField* so = static_cast<ForceField*>(object);
   if (so) {
      bool set;
      Con::setData(TypeBool, &set, 0, 1, &data);
      so->setAllowPlayers(set);
   }
   return false;
}

bool ForceField::_setAllowVehicles(void *object, const char *index, const char *data) {
   ForceField* so = static_cast<ForceField*>(object);
   if (so) {
      bool set;
      Con::setData(TypeBool, &set, 0, 1, &data);
      so->setAllowVehicles(set);
   }
   return false;
}

bool ForceField::_setAllowItems(void *object, const char *index, const char *data) {
   ForceField* so = static_cast<ForceField*>(object);
   if (so) {
      bool set;
      Con::setData(TypeBool, &set, 0, 1, &data);
      so->setAllowItems(set);
   }
   return false;
}

bool ForceField::_setTeam(void *object, const char *index, const char *data) {
   ForceField* so = static_cast<ForceField*>(object);
   if (so) {
      S32 set;
      Con::setData(TypeS32, &set, 0, 1, &data);
      so->setTeam(set);
   }
   return false;
}