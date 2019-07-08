/**
hazardPrecipitation.h
Robert Fritzen
(c) Phantom Games Development, 2013

Central file for the hazardPrecipitation class.

This class essentially acts in the same way the normal Precipitation class
behaves, however it allows you to define an effect to objects when they are
in areas where precip. will land, essentially you could make acid rain zones
that cause damage when outside, but none when inside
**/
#ifndef _hazardPrecipitation_H_
#define _hazardPrecipitation_H_

#include "T3D/fx/precipitation.h"

//--------------------------------------------------------------------------
class hazardPrecipitation : public Precipitation {
  protected:

   typedef Precipitation Parent;
   PrecipitationData*   mDataBlock;

   DECLARE_CALLBACK( void, onHitObject, ( const Point3F& hitPosition, SceneObject* hitObject ));

   Raindrop *mDropHead;    ///< Drop linked list head
   Raindrop *mSplashHead;  ///< Splash linked list head

   Point2F* mTexCoords;     ///< texture coords for rain texture
   Point2F* mSplashCoords;  ///< texture coordinates for splash texture

   SFXSource* mAmbientSound;        ///< Ambient sound

   GFXShaderRef mDropShader;     ///< The shader used for raindrops
   GFXTexHandle mDropHandle;     ///< Texture handle for raindrop
   GFXShaderRef mSplashShader;   ///< The shader used for splashes
   GFXTexHandle mSplashHandle;   ///< Texture handle for splash

   U32 mLastRenderFrame;         ///< Used to skip processTick when we haven't been visible.

   U32 mDropHitMask;             ///< Stores the current drop hit mask.

   //console exposed variables
   bool mFollowCam;                 ///< Does the system follow the camera or stay where it's placed.

   F32  mDropSize;                  ///< Droplet billboard size
   F32  mSplashSize;                ///< Splash billboard size
   bool mUseTrueBillboards;         ///< True to use true billboards, false for axis-aligned billboards
   S32  mSplashMS;                  ///< How long in milliseconds a splash will last
   bool mAnimateSplashes;           ///< Animate the splashes using the frames in the texture.

   S32 mDropAnimateMS;           ///< If greater than zero, will animate the drops from
                                 ///< the frames in the texture

   S32 mNumDrops;                ///< Number of drops in the scene
   F32 mPercentage;              ///< Server-side set var (NOT exposed to console)
                                 ///< which controls how many drops are present [0,1]

   F32 mMinSpeed;                ///< Minimum downward speed of drops
   F32 mMaxSpeed;                ///< Maximum downward speed of drops

   F32 mMinMass;                 ///< Minimum mass of drops
   F32 mMaxMass;                 ///< Maximum mass of drops

   F32 mBoxWidth;                ///< How far away in the x and y directions drops will render
   F32 mBoxHeight;               ///< How high drops will render

   F32 mMaxTurbulence;           ///< Coefficient to sin/cos for adding turbulence
   F32 mTurbulenceSpeed;         ///< How fast the turbulence wraps in a circle
   bool mUseTurbulence;          ///< Whether to use turbulence or not (MAY EFFECT PERFORMANCE)

   bool mUseLighting;            ///< This enables shading of the drops and splashes
                                 ///< by the sun color.

   ColorF mGlowIntensity;        ///< Set it to 0 to disable the glow or use it to control 
                                 ///< the intensity of each channel.

   bool mReflect;                ///< This enables the precipitation to be rendered
                                 ///< during reflection passes.  This is expensive.

   bool mUseWind;                ///< This enables the wind from the sky SceneObject
                                 ///< to effect the velocitiy of the drops.

   bool mRotateWithCamVel;       ///< Rotate the drops relative to the camera velocity
                                 ///< This is useful for "streak" type drops

   bool mDoCollision;            ///< Whether or not to do collision
   bool mDropHitPlayers;         ///< Should drops collide with players
   bool mDropHitVehicles;        ///< Should drops collide with vehicles

   F32 mFadeDistance;            ///< The distance at which fading of the particles begins.
   F32 mFadeDistanceEnd;         ///< The distance at which fading of the particles ends.

   U32 mMaxVBDrops;              ///< The maximum drops allowed in one render batch.

   GFXStateBlockRef mDefaultSB;
   GFXStateBlockRef mDistantSB;

   GFXShaderConstBufferRef mDropShaderConsts;
   
   GFXShaderConstHandle* mDropShaderModelViewSC;
   GFXShaderConstHandle* mDropShaderFadeStartEndSC;
   GFXShaderConstHandle* mDropShaderCameraPosSC;
   GFXShaderConstHandle* mDropShaderAmbientSC;

   GFXShaderConstBufferRef mSplashShaderConsts;
   
   GFXShaderConstHandle* mSplashShaderModelViewSC;
   GFXShaderConstHandle* mSplashShaderFadeStartEndSC;
   GFXShaderConstHandle* mSplashShaderCameraPosSC;
   GFXShaderConstHandle* mSplashShaderAmbientSC;

   //other functions...
   void processTick(const Move*);
   void interpolateTick(F32 delta);

   VectorF getWindVelocity();
   void fillDropList();                       ///< Adds/removes drops from the list to have the right # of drops
   void killDropList();                       ///< Deletes the entire drop list
   void initRenderObjects();                  ///< Re-inits the texture coord lookup tables
   void initMaterials();                      ///< Re-inits the textures and shaders
   void spawnDrop(Raindrop *drop);            ///< Fills drop info with random velocity, x/y positions, and mass
   void spawnNewDrop(Raindrop *drop);         ///< Same as spawnDrop except also does z position

   void createSplash(Raindrop *drop);        ///< Adds a drop to the splash list
   void destroySplash(Raindrop *drop);       ///< Removes a drop from the splash list
   
   void findDropCutoff(Raindrop *drop, const Box3F &box, const VectorF &windVel);   ///< Casts a ray to see if/when a drop will collide
   void wrapDrop(Raindrop *drop, const Box3F &box, const U32 currTime, const VectorF &windVel);         ///< Wraps a drop within the specified box
   void doObjectHit(Point3F hitPosition, SceneObject *object, bool entering);  ///< Handles the object checks and calls the server function to handle effects

   GFXPrimitiveBufferHandle mRainIB;
   GFXVertexBufferHandle<GFXVertexPT> mRainVB;

   bool onAdd();
   void onRemove();

   // Rendering
   void prepRenderImage( SceneRenderState* state );
   void renderObject(ObjectRenderInst *ri, SceneRenderState *state, BaseMatInstance* );

   void setTransform(const MatrixF &mat);

  public:

   hazardPrecipitation();
   ~hazardPrecipitation();
   void inspectPostApply();

   enum {
      DataMask       = Parent::NextFreeMask << 0,
      PercentageMask = Parent::NextFreeMask << 1,
      StormMask      = Parent::NextFreeMask << 2,
      TransformMask  = Parent::NextFreeMask << 3,
      TurbulenceMask = Parent::NextFreeMask << 4,
      NextFreeMask   = Parent::NextFreeMask << 5
   };

   bool onNewDataBlock( GameBaseData *dptr, bool reload );
   DECLARE_CONOBJECT(hazardPrecipitation);
   static void initPersistFields();
   
   U32  packUpdate(NetConnection*, U32 mask, BitStream* stream);
   void unpackUpdate(NetConnection*, BitStream* stream);

   void setPercentage(F32 pct);
   void modifyStorm(F32 pct, U32 ms);

   void setTurbulence(F32 max, F32 speed, U32 ms);
};

#endif // hazardPrecipitation_H_

