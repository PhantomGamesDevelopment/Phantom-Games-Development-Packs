#ifndef _FORCEFIELD_H_
#define _FORCEFIELD_H_

#ifndef _SCENEOBJECT_H_
	#include "scene/sceneObject.h"
#endif
#ifndef _SHAPEBASE_H_
	#include "T3D/shapeBase.h"
#endif
#ifndef _GFXSTATEBLOCK_H_
	#include "gfx/gfxStateBlock.h"
#endif
#ifndef _GFXVERTEXBUFFER_H_
	#include "gfx/gfxVertexBuffer.h"
#endif
#ifndef _GFXPRIMITIVEBUFFER_H_
	#include "gfx/gfxPrimitiveBuffer.h"
#endif
#ifndef _BOXCONVEX_H_
   #include "collision/boxConvex.h"
#endif

class BaseMatInstance;

class ForceField : public SceneObject {
   typedef SceneObject Parent;

   enum MaskBits {
      TransformMask = Parent::NextFreeMask << 0,
      NextFreeMask  = Parent::NextFreeMask << 1,
		ColorMask     = Parent::NextFreeMask << 2,
		InfoMask      = Parent::NextFreeMask << 3
   };

	enum Consts {
		CollisionTimeoutValue = 250
	};
   GFXStateBlockRef mStateBlockRef;
   GFXVertexBufferHandle<GFXVertexPCN> mVertexBuffer;
   OrthoBoxConvex mConvex;
	Convex *       mConvexList;
   Box3F          mWorkingQueryBox;

	public:
		ForceField();
		virtual ~ForceField();

		DECLARE_CONOBJECT(ForceField);
		DECLARE_CALLBACK( void, onCollision, (ForceField* obj, SceneObject* collObj, VectorF vec, F32 len));

		//Script Functions
		static void initPersistFields();
		bool onAdd();
		void onRemove();

		//Object Update Functions
		void processTick(const Move *move);

		//Networking Stuff
		void setTransform( const MatrixF &mat );
		U32 packUpdate( NetConnection *conn, U32 mask, BitStream *stream );
		void unpackUpdate( NetConnection *conn, BitStream *stream );

		//Rendering functions
		void createGeometry();

		void prepRenderImage( SceneRenderState *state );
		void render( ObjectRenderInst *ri, SceneRenderState *state, BaseMatInstance *overrideMat );

		//Collision functions
		bool buildPolyList(PolyListContext context, AbstractPolyList* polyList, const Box3F&, const SphereF&);
		void buildConvex(const Box3F& box, Convex* convex);
		void queueCollision(ForceField *me, ShapeBase *colObj, const VectorF &vec);
		void notifyCollision();
		void onCollision(SceneObject *object, const VectorF &vec);

		//Collision Stuff
		struct CollisionTimeout {
			CollisionTimeout* next;
			SceneObject* object;
			U32 objectNumber;
			SimTime expireTime;
			VectorF vector;
		};
		CollisionTimeout* mTimeoutList;
		static CollisionTimeout* sFreeTimeoutList;

		//Etc...
		bool shouldCollide(ShapeBase *object);

		void setColor(ColorF col) { mColor = col; setMaskBits(ColorMask); }
		ColorF getColor() { return mColor; }
		void setTeamPass(bool set) { mIsTeamPass = set; setMaskBits(InfoMask); }
		bool getTeamPass() { return mIsTeamPass; }
		void setAllPass(bool set) { mIsAllPass = set; setMaskBits(InfoMask); }
		bool getAllPass() { return mIsAllPass; }
		void setEnabled(bool set) { mIsEnabled = set; setMaskBits(InfoMask); }
		bool getEnabled() { return mIsEnabled; }
		void setAllowPlayers(bool set) { mAllowPlayers = set; setMaskBits(InfoMask); }
		bool getAllowPlayers() { return mAllowPlayers; }
		void setAllowVehicles(bool set) { mAllowVehicles = set; setMaskBits(InfoMask); }
		bool getAllowVehicles() { return mAllowVehicles; }
		void setAllowItems(bool set) { mAllowItems = set; setMaskBits(InfoMask); }
		bool getAllowItems() { return mAllowItems; }
		void setTeam(S32 set) { mTeam = set; setMaskBits(InfoMask); }
		bool getTeam() { return mTeam; }

   protected:
		ColorF mColor;
		bool mIsTeamPass;
		bool mIsAllPass;
		bool mIsEnabled;
		bool mAllowPlayers;
		bool mAllowVehicles;
		bool mAllowItems;
		S32 mTeam;

	private:
		static bool _setColor(void *object, const char *index, const char *data);
		static bool _setTeamPass(void *object, const char *index, const char *data);
		static bool _setAllPass(void *object, const char *index, const char *data);
		static bool _setEnabled(void *object, const char *index, const char *data);
		static bool _setAllowPlayers(void *object, const char *index, const char *data);
		static bool _setAllowVehicles(void *object, const char *index, const char *data);
		static bool _setAllowItems(void *object, const char *index, const char *data);
		static bool _setTeam(void *object, const char *index, const char *data);
};

#endif  // _H_FORCEFIELD_