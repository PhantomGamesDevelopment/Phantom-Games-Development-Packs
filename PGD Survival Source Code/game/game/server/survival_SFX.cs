//survival_SFX.cs
//Phantom139
//Handles all customized SFX emitters and sounds.

function createEmitter(%emitter, %position, %rotation) {
   %object = new ParticleEmitterNode() {
		active = true;
      dataBlock = DefaultEmitterNodeData;
      emitter = %emitter;
      velocity = "1";
   };
   MissionCleanup.add(%object);

	%object.setTransform(%position SPC %rotation);

	%object.setActive(true);
	%object.setEmitterDataBlock(%emitter);

	if(isObject(%object)) {
      return %object;
	}
}

datablock ParticleData(IonStormCloudParticle : DefaultParticle) {
   dragCoefficient = "0.708";
   gravityCoefficient = "-0.25";
   inheritedVelFactor = "0";
   constantAcceleration = "1.667";
   spinSpeed = "0.438";
   textureName = "art/shapes/particles/fireball.png";
   animTexName = "art/shapes/particles/fireball.png";
   colors[0] = "0 0.0470588 1 1";
   colors[1] = "0.298039 0.443137 0.745098 0.888";
   colors[2] = "0.215686 0.760784 0.721569 0.809";
};

datablock ParticleEmitterData(IonStormCloudEmitter : DefaultEmitter) {
   ejectionPeriodMS = "1";
   ejectionVelocity = "1.5";
   ejectionOffset = "0";
   thetaMax = "180";
   particles = "IonStormCloudParticle";
   alignParticles = "1";
   alignDirection = "0 0 1";
};

datablock ParticleData(DisentigrationParticle : DefaultParticle) {
   dragCoefficient = "0.729";
   gravityCoefficient = "-0.29";
   constantAcceleration = "3.75";
   spinSpeed = "0.542";
   spinRandomMin = "-167";
   spinRandomMax = "541.9";
   textureName = "art/shapes/particles/smoke.png";
   animTexName = "art/shapes/particles/smoke.png";
   colors[0] = "0.0196078 0.52549 0.490196 1";
   colors[1] = "0.0784314 0.945098 0.823529 0.637795";
   colors[2] = "0.921569 0.976471 0.972549 0.330709";
};

datablock ParticleEmitterData(DisentigrationEmitter : DefaultEmitter) {
   ejectionVelocity = "1.25";
   velocityVariance = "1.25";
   thetaMax = "18.75";
   particles = "DisentigrationParticle";
   lifetimeMS = "3000";
};

datablock ParticleData(IncinerationParticle : DefaultParticle) {
   dragCoefficient = "0.72825";
   gravityCoefficient = "-0.290598";
   constantAcceleration = "0.417";
   spinSpeed = "0.542";
   spinRandomMin = "-167";
   spinRandomMax = "541";
   textureName = "art/shapes/particles/steam.png";
   animTexName = "art/shapes/particles/steam.png";
	useInvAlpha = "0";
   colors[0] = "0.984314 0.623529 0.278431 1";
   colors[1] = "0.901961 0.317647 0.121569 0.629921";
   colors[2] = "0.996078 0.2 0.0470588 0.330709";
	sizes[0] = "1.00002";
   sizes[1] = "2.08333";
   sizes[2] = "4.16667";
};

datablock ParticleEmitterData(IncinerateEmitter : DefaultEmitter) {
   ejectionVelocity = "1.25";
   velocityVariance = "1.25";
   thetaMax = "18.75";
   particles = "IncinerationParticle";
   lifetimeMS = "3000";
};

datablock ParticleData(ChainDisintegrateInitialParticle) {
   textureName = "art/shapes/particles/wake";
   dragCoefficient = 0.0;
   gravityCoefficient = 0.0;
   inheritedVelFactor = 0.0;
   lifetimeMS = 2500;
   lifetimeVarianceMS = 200;
   windCoefficient = 0.0;
   useInvAlpha = 1;
   spinRandomMin = 30.0;
   spinRandomMax = 30.0;
   spinSpeed = 1;

   animateTexture = true;
   framesPerSec = 1;
   animTexTiling = "2 1";
   animTexFrames = "0 1";

   colors[0] = "0.0196078 0.52549 0.490196 1";
   colors[1] = "0.0784314 0.945098 0.823529 0.637795";
   colors[2] = "0.921569 0.976471 0.972549 0.330709";

   sizes[0] = 2.0;
   sizes[1] = 4.0;
   sizes[2] = 8.0;

   times[0] = 0.0;
   times[1] = 0.5;
   times[2] = 1.0;
};

datablock ParticleEmitterData(ChainDisintegrateInitialEmitter) {
   lifetimeMS = "750";
   ejectionPeriodMS = 200;
   periodVarianceMS = 10;
   ejectionVelocity = 0;
   velocityVariance = 0;
   ejectionOffset = 0;
   thetaMin = 89;
   thetaMax = 90;
   phiReferenceVel = 0;
   phiVariance = 1;
   alignParticles = 1;
   alignDirection = "0 0 1";
   particles = "ChainDisintegrateInitialParticle";
};

function ChainDisintegrate(%initial, %radius, %source) {
	%chainCounter = 1;
   //who did this?
	%sourcePlayer = %initial.lastDamagedBy;
	Disintegrate(%initial, %source);
   %emitter = createEmitter(ChainDisintegrateInitialEmitter, %initial.getPosition(), "1 0 0 0");
	%emitter.schedule(750, delete);
	//now perform a radius check for additional targets
	InitContainerRadiusSearch(%initial.getPosition(), %radius, $TypeMasks::ShapeBaseObjectType);
   while ((%targetObject = containerSearchNext()) != 0) {
      %coverage = calcExplosionCoverage(%position, %targetObject,
                                        $TypeMasks::InteriorObjectType |
                                        $TypeMasks::TerrainObjectType |
                                        $TypeMasks::ForceFieldObjectType |
                                        $TypeMasks::StaticShapeObjectType |
                                        $TypeMasks::VehicleObjectType);
		if (%coverage == 0) {
         continue;
		}
		if(%targetObject.client.team == %source.sourceObject.client.team) {
		   continue;
		}
	   //Chain Disintegrate.... really nasty stuff... initial target goes, then it ripples out to additional targets
		%targetObject.damage(%source, %pos, 0.1, %initial.damageType[%sourcePlayer.client]);
		Disintegrate(%targetObject, %source);   //bye bye...
		%chainCounter++;
	}

	return %chainCounter;
}

function Disintegrate(%object, %source) {
	if(%object.client.team == %source.sourceObject.client.team) {
		return;
	}
   //who did this?
	%sourcePlayer = %object.lastDamagedBy;
   //%object.blowup();
	%object.startFade(3000, 250, true);
	%object.damage(%source, %object.getPosition(), 10000, %object.damageType[%sourcePlayer.client]);

   //create a disentigrate emitter
   %emitter = createEmitter(DisentigrationEmitter, %object.getPosition(), "1 0 0 0");
	%emitter.schedule(3000, delete);
}

function Incinerate(%object, %source) {
	if(%object.client.team == %source.sourceObject.client.team) {
		return;
	}
   //who did this?
	%sourcePlayer = %object.lastDamagedBy;
   //%object.blowup();
	%object.damage(%source, %object.getPosition(), 10000, %object.damageType[%sourcePlayer.client]);

   //create a disentigrate emitter
   %emitter = createEmitter(IncinerateEmitter, %object.getPosition(), "1 0 0 0");
	%emitter.schedule(3000, delete);
}

function createSingleStormStrike(%source, %position) {
	%down = "0 0 -1";
   %p = new (Projectile)() {
      dataBlock = TeslaForcerProjectile;
      initialVelocity = %down;
      initialPosition = vectorAdd(%position, "0 0 8");
   };
   MissionCleanup.add(%p);

	%p.sourceObject = %source;
	%p.client = %source.client;
}

function createIonStorm(%source, %position) {
   %stormCloud = createEmitter(IonStormCloudEmitter, vectorAdd(%position, "0 0 8"), "1 0 0 0");
	%stormCloud.schedule(15000, delete);

	for(%i = 0; %i < 15; %i++) {
	   schedule(%i * 1000, 0, createSingleStormStrike, %source, %position);   
	}
}