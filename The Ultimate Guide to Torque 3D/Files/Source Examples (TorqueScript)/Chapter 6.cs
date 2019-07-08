//Chapter 6 Code Samples
//The Ultimate Guide To Torque 3D
// By Robert C. Fritzen

function helloWorld(%moreText) {
   echo("hello world!" @ " " @ %moreText);
}

function addTwoNumbers(%num1, %num2) {
   echo("We're going to add "@%num1@" and "@%num2); //<-- String Concatenation Rules… Remember them!
   %result = %num1 + %num2;
   echo("The Answer is: "@%result);
   return %result;
}

function subTwoNumbers(%num1, %num2) {
   echo("We're going to subtract "@%num2@" from "@%num1); //<-- String Concatenation Rules… Remember them!
   %result = %num1 - %num2;
   echo("The Answer is: "@%result);
   return %result;
}

function multiplyTwoNumbers(%num1, %num2) {
   echo("We're going to multiply "@%num1@" and "@%num2); //<-- String Concatenation Rules… Remember them!
   %result = %num1 * %num2;
   echo("The Answer is: "@%result);
   return %result;
}

function divideTwoNumbers(%num1, %num2) {
   if(%num1 $= "" || %num2 $= "" || %num2 == 0) {
      error("Cannot perform this division...");
      return 0;
   }
   echo("We're going to divide "@%num1@" and "@%num2); //<-- String Concatenation Rules… Remember them!
   %result = %num1 / %num2;
   echo("The Answer is: "@%result);
   return %result;
}

function generalMath(%operation, %num1, %num2) {
   echo("GeneralMath()... Time to do some mathematics");
   if(%operation $= "+") {
      %addResult = addTwoNumbers(%num1, %num2);
      echo("Addition Result: "@%addResult);
   }
   else if(%operation $= "-") {
      %subResult = subTwoNumbers(%num1, %num2);
      echo("Subtraction Result: "@%subResult);
   }
   else if(%operation $= "*") {
      %mulResult = multiplyTwoNumbers(%num1, %num2);
      echo("Multiplication Result: "@%mulResult);
   }
   else if(%operation $= "/") {
      %divResult = divideTwoNumbers(%num1, %num2);
      echo("Division Result: "@%divResult);
   }
   else {
      error("Invalid option, provide a mathematical operator (+, -, *, /) to run this function");
   }
}

function advancedConditional() {
   %myNumber = getRandom(1, 100);
   echo("Number: "@%myNumber);
   if(%myNumber > 25 && %myNumber < 75) {
      echo("Number is greater than 25, but less than 75");
   }
   if(%myNumber > 25 || %myNumber < 75) {
      echo("Where is this number?");
   }
   if(!(%myNumber > 25 && %myNumber < 75)) {
      echo("Where am I now?");
   }
   if((%myNumber > 25 && %myNumber < 75) && (%number != 50)) {
      echo("We got it!");
   }
}

function switchTest() {
   %myNumber = getRandom(1, 100);
   switch(%myNumber) {
      case 1 or 2 or 3 or 4 or 5:
         echo("Our number is between 1 and 5...");
      case 20:
         echo("Our number is 20.");
      default:
         echo("Didn't get anything…");
   }
}

function generalMath_Conditional(%operation, %num1, %num2) {
   echo("generalMath_Conditional()... Time to do some mathematics");
   switch$(%operation) {
      case "+":
         %addResult = addTwoNumbers(%num1, %num2);
         echo("Addition Result: "@%addResult);
      case "-";
         %subResult = subTwoNumbers(%num1, %num2);
         echo("Subtraction Result: "@%subResult);
      case "*":
         %mulResult = multiplyTwoNumbers(%num1, %num2);
         echo("Multiplication Result: "@%mulResult);
      case "/":
         %divResult = divideTwoNumbers(%num1, %num2);
         echo("Division Result: "@%divResult);
      default:
         error("Invalid option, provide a mathematical operator (+, -, *, /) to run this function");
   }
}

function conditionalOperator() {
   %myNumber = getRandom(1, 100);
   %myNumber == 50 ? echo("It's 50") : echo("It's not 50");
   %myNumber > 25 ? (%myNumber < 75 ? echo("yes") : echo ("No.")) : echo("Nope.");
}

function counterLoop_While() {
   %i = 1;
   while(%i <= 10) {
      echo("Counter now at "@%i);
      %i++;
   }
}

function generatorWhile() {
   %myNumber = getRandom(1, 100);
   echo("Number is "@%myNumber);
   while(%myNumber > 25 && %myNumber < 75) {
      echo("The number is where we don't want it, let's generate another!");
      %myNumber = getRandom(1, 100);
      echo("Number is now "@%myNumber);
   }
   echo("We got the number we wanted: "@%myNumber);
}

function counterLoop_For() {
   for(%i = 1; %i <= 10; %i++) {
      echo("Counter now at "@%i);
   }
}
