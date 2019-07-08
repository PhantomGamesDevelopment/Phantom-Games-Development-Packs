//Chapter 9 Code Samples
//The Ultimate Guide To Torque 3D
// By Robert C. Fritzen

function powerExample(%x, %y) {
   echo("Raising "@%x@" to the power of "@%y);
   return mPow(%x, %y);
}

function squareRootExample(%x) {
   echo("Taking the square root of "@%x);
   return mSqrt(%x);
}

function logWithBase(%value, %newBase) {
   return (mLog(%value) / mLog(%newBase));
}

function clampExample(%x, %min, %max) {
   echo("Clamping "@%x@" between "@%min@" and "@%max);
   %clamp = mClamp(%x, %min, %max);
   %saturate = mSaturate(%x);
   echo("The clamped value is: "@%clamp@"\nThe saturated value is "@%saturate);
}

function vectorConstruct(%x, %y, %z) {
   if(%x $= "" || (%y $= "" && %z $= "")) {
      error("Cannot construct vector, need at least 3 coordinates.");
      return "0 0 0";
   }
   return %x @ " " @ (%y $= "" ? %z : (%y @ (%z $= "" ? "" : " "@%z)));
}

function vectorLengthExample() {
   %vector = "5 5 5";
   %len = vectorLen(%vector);
   echo("The magnitude of "@%vector@" is "@%len);
}

function vectorDistExample() {
   %vector[0] = "5 5 5";
   %vector[1] = "10 20 30";
   %dist = vectorDist(%vector[0], %vector[1]);
   echo("The distance between these vectors is "@%dist);
}

function vectorNormalizeExample() {
   %vector = "5 5 5";
   %unitVector = vectorNormalize(%vector);
   echo("The unit vector of "@%vector@" is "@%unitVector);
}

function vectorAddExample() {
   %vector[0] = "5 5 5";
   %vector[1] = "10 20 30";
   %result = vectorAdd(%vector[0], %vector[1]);
   echo("The result of adding "@%vector[0]@" to "@%vector[1]@" is: "@%result);
}

function vectorSubExample() {
   %vector[0] = "5 5 5";
   %vector[1] = "10 20 30";
   %result = vectorSub(%vector[0], %vector[1]);
   echo("The result of subtracting "@%vector[1]@" from "@%vector[0]@" is: "@%result);
}

function vectorScaleExample() {
   %vector = "5 5 5";
   %result = vectorScale(%vector, 5);
   echo("The result of performing the vector scale of 5 * "@%vector@" is: "@%result);
}

function vectorDotExample() {
   %vector[0] = "5 5 5";
   %vector[1] = "10 20 30";
   %result = vectorDot(%vector[0], %vector[1]);
   echo("The dot product of "@%vector[0]@" and "@%vector[1]@" is: "@%result);
}

function vectorAngleExample() {
   %vector[0] = "5 5 5";
   %vector[1] = "10 20 30";
   %dot = vectorDot(%vector[0], %vector[1]);
   %mag[0] = vectorLen(%vector[0]);
   %mag[1] = vectorLen(%vector[1]);
   %angle = mACos((%dot) / (%mag[0] * %mag[1])); //What angular notation is this in?
   echo("The angle between "@%vector[0]@" and "@%vector[1]@" is "@%angle);
}

function vectorCrossExample() {
   %vector[0] = "5 5 5";
   %vector[1] = "10 20 30";
   %result = vectorCross(%vector[0], %vector[1]);
   echo("The cross product of "@%vector[0]@" and "@%vector[1]@" is: "@%result);
}

function matrixAnalysisExample(%spreadForce) {
   %fVec = "10 0 0";
   %x = (getRandom() - 0.5) * m2Pi() * %spreadForce;
   %y = (getRandom() - 0.5) * m2Pi() * %spreadForce;
   %z = (getRandom() - 0.5) * m2Pi() * %spreadForce;
   %mat = MatrixCreateFromEuler(%x SPC %y SPC %z);
   %newvector = MatrixMulVector(%mat, %fVec);
}

function linearBezierExample(%start, %end, %quality) {
   //We use %start and %end as position variables, to solve a Bezier curve, we need to strip out the components.
   //The quality variable is used to determine how many parametric terms to solve for.
   %sX = getWord(%start, 0);
   %sY = getWord(%start, 1);
   %sZ = getWord(%start, 2);
   %eX = getWord(%end, 0);
   %eY = getWord(%end, 1);
   %eZ = getWord(%end, 2);
   for(%i = 0; %i < %quality; %i ++) {
      %t = (%i * 1.0) / (%quality * 1.0); //Multiply the variables by 1.0 to convert them to decimals
      %x = (1 - %t) * %sX + %t * %eX;
      %y = (1 - %t) * %sY + %t * %eY;
      %z = (1 - %t) * %sZ + %t * %eZ;
      %pos[%i] = %x SPC %y SPC %z;
   }
   //Print out the positions.
   for(%i = 0; %i < %quality; %i++) {
      echo("Point "@%i+1@": "@%pos[%i]);
   }
}

function quadraticBezierExample(%start, %end, %quality) {
   %sX = getWord(%start, 0);
   %sY = getWord(%start, 1);
   %sZ = getWord(%start, 2);
   %eX = getWord(%end, 0);
   %eY = getWord(%end, 1);
   %eZ = getWord(%end, 2);
   //Solve for the midpoint.
   %midL = vectorLen(%start, %end) / 2; //Length between start and end divided by 2
   %dir = vectorNormalize(vectorSub(%end, %start)); //Unit vector pointing in the direction from start -> end
   %midPt = vectorAdd(%start, vectorScale(%dir, %midL)); // Add the start to the scaled (unit vector * length) to get mid point
   //Strip points.
   %mX = getWord(%midPt, 0);
   %mY = getWord(%midPt, 1);
   %mZ = getWord(%midPt, 2) + 5.0; //Add 5.0 to create a curve that "bends" up.
   for(%i = 0; %i < %quality; %i ++) {
      %t = (%i * 1.0) / (%quality * 1.0); //Multiply the variables by 1.0 to convert them to decimals
      %x = mPow((1 - %t), 2) * %sX + 2 * (1 - %t) * %t * %mX + mPow(%t, 2) * %eX;
      %y = mPow((1 - %t), 2) * %sY + 2 * (1 - %t) * %t * %mY + mPow(%t, 2) * %eY;
      %z = mPow((1 - %t), 2) * %sZ + 2 * (1 - %t) * %t * %mZ + mPow(%t, 2) * %eZ;
      %pos[%i] = %x SPC %y SPC %z;
   }
   //Print out the positions.
   for(%i = 0; %i < %quality; %i++) {
      echo("Point "@%i+1@": "@%pos[%i]);
   }
}

function getRandomExample_one() {
   %rand = getRandom();
   echo("Generated :"@%rand);
}

function getRandomExample_two(%max) {
   %rand = getRandom(%max);
   echo("Generated the following number in the [0-"@%max@"] range:"@%rand);
}

function getRandomExample_three(%min, %max) {
   %rand = getRandom(%min, %max);
   echo("Generated the following number in the ["@%min@"-"@%max@"] range:"@%rand);
}

function getRandomExample_four(%min, %max) {
   %cSeed = getRandomSeed();
   echo("The current seed is: "@%cSeed@", setting this to 10.");
   setRandomSeed(10);
   %rand1 = getRandom(%min, %max);
   echo("With a seed of 10, we generated: "@%rand1);
   //set the seed to the current UTC time
   setRandomSeed(-1);
   %rand2 = getRandom(%min, %max);
   echo("With the current time seed, we generated:"@%rand);
}

package randomGenerator_Update {
   function getRandom(%min, %max) {
      setRandomSeed(-1);
      parent::getRandom(%min, %max);
   }
}
if(!isActivePackage(randomGenerator_Update)) {
   activatePackage(randomGenerator_Update);
}
