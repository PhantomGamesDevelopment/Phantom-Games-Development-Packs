<?php
   //requires
   require_once("pgdFunctions.php");
   //require("../admin/globals.php");
   require("projectSettings.php");
   require("../admin/sql.php");

   include_once("../admin/sources/php/Crypt/RSA.php");
   //-- end requires
   if(!$AuthOnline) {
      die("The authentication server is currently offline.");
   }
   $mode = $_POST["authMode"];
   $current = "".substr(dirname($_SERVER[PHP_SELF]),strrpos(dirname($_SERVER[PHP_SELF]),'/') + 1)."";
   if(!isSet($mode)) {
      die("This portion of our site is off limits....");
   }
   $con = sqlLogin($XXZVariables["db_username"][$current], $XXZVariables["db_password"][$current], $XXZVariables["database"][$current]) or die("$MAP"."$INTERNAL_ERROR\n");
   //begin the processes here
   switch($mode) {
      case 1:
         die("Invalid Request.");
         //
      case 2:
         //2: Verify Data, Check Key, and Sign Certificate
         //Stage 1: Data verification
         $payload = $_POST["fullPayload"];
         $clData = explode("\t", $payload);

         $name = $con->real_escape_string($clData[0]);
         $cond = checkName($name, $con);
         if($cond == 1 || $cond == 2 || $cond == 3 || $cond == 4) {
            die("$"."MAP"."$"."BAD_NAME");
         }
         else if ($cond == 5) {
            die("$"."MAP"."$"."NAME_TAKEN");
         }
      	 $email = $con->real_escape_string($clData[1]);
    	 if(!isSet($email)) {
            die("$"."MAP"."$"."NO_EMAIL");
         }
         if( strStr($email, "@") == -1 || strStr($email, ".") == -1 ) {
            die("$"."MAP"."$"."BAD_EMAIL");
         }
         $key = $con->real_escape_string($clData[2]);
         if(!isSet($key)) {
            die("$"."MAP"."$"."NO_KEY");
         }
         if(!preg_match("/^[a-zA-Z0-9]+?$/", $key)) {
            die("$"."MAP"."$"."INVALID_KEY");
         }
         //Stage 2: Key Check (Server Stores Whirlpool(Key))
         $AT = $con->real_escape_string($XXZVariables["key_table"][$current]);
   	     $result2 = sqlCall($con, "SELECT * FROM $AT") or die("$"."INTERNAL_ERROR");
   	     while($row2 = $result2->fetch_array(MYSQLI_ASSOC)) {
            if(strCmp($row2["AccKey"], $key) == 0)  {
               $PurchCode[$key] = $row2["PurchaseCode"];
               $PurchEmail[$key] = $row2["Email"];
            }
         }
         sqlDC($result2);
         //echo "SERV: ".$PurchEmail[$key]." ".$email;
         if (strcmp(strtolower($PurchEmail[$key]), strtolower($email)) != 0 && $XXZVariables["AK_BindEmail"][$current] == 1 && strcmp(strtolower($PurchEmail[$key]), "any") != 0) {
            sqlDC($con);
            die("$"."MAP"."$"."EMAIL_NOT_BOUND");
         }
         //ok, all the data looks good, now lets go!
         //Stage 3: Certificate Signing / database adding
         //fields we now have: username ($name), password($hashpass), email($email)
         //key ($key)
         $email = strtolower($email);
         $guid = CreateGUID($con);
         $private = $con->real_escape_string($clData[3]); //client encrypts before sending
         $priv_dec_hash = $con->real_escape_string($clData[6]);
         $public = $con->real_escape_string($clData[4]);
         $exp = $con->real_escape_string($clData[5]);
         $accountData = $con->real_escape_string($clData[7]); //Whirlpool(username @ password @ 2.71828)
         //
         //
         $FullAccountData = XXZHash($guid.$name.$exp.$public); //what we need
         $rsa = new Crypt_RSA();
	     $rsa->loadKey(CA_private());

	     $rsa->setSignatureMode(CRYPT_RSA_SIGNATURE_PKCS1);
	     $signature = $rsa->sign($FullAccountData);
         $rsa->loadKey(CA_public());
         $ok = $rsa->verify($FullAccountData, $signature);
         if($ok == 1) {
            $sigFinal = bin2hex($signature);
            //successful account creation
            //update the database adding the new user's data
            $AT = $XXZVariables["account_table"][$current];
            $AK = $XXZVariables["key_table"][$current];

            $sql1="INSERT INTO $AT (guid, E, N, D, D_DEC_HASH, signature, username, email, hashpass)
            VALUES ('$guid', '$exp', '$public', '$private', '$priv_dec_hash', '$sigFinal', '$name', '$email', '$accountData')";
            $sql2 = "DELETE FROM $AK WHERE AccKey='$key' AND Email='$email'";

            if(!sqlCall($con, $sql1)) {
               die("$PGD"."INTERNAL_ERROR");
            }
            //sqlDC($result1);
            if(!sqlCall($con, $sql2)) {
               die("$PGD".'INTERNAL_ERROR');
            }
            //sqlDC($result2);
            sqlDC($con);
            die("$"."MAP"."$"."CERT ". $name ."\t". $guid ."\t" . $sigFinal ."\n"); // complete their public certificate
         }
         else {
            sqlDC($con);
            die("$"."MAP"."$"."SIGN_ERROR ".$ok."");
         }
         return;
      case 3:
         //3: Account Re-Download (Certificate Lookup)
         $sentHash = $con->real_escape_string($_POST["hashsend"]); //correct hash = whirlpool(base64(salt)) @ whirlpool(username @ password @ 2.71828) <- //what the DB stores
         $utc = time();
         $utc = "$utc";
         $salt = substr($utc, 0, -3);
         $saltHash = sha1($salt);

         $AT = $con->real_escape_string($XXZVariables["account_table"][$current]);
         $result = sqlCall($con, "SELECT * FROM $AT") or die("$MAP"."$INTERNAL_ERROR\n");

         while($row = $result->fetch_array(MYSQLI_ASSOC))  {
            if(strCmp(strtolower($row["username"]), strtolower($_POST["request"])) == 0) {
               //ok, this is the account we are looking for
               $storedPassword = $row["hashpass"];
               //tag the UTC to it for a salt against replay attacks
               $check = $saltHash . $storedPassword;
               if(strCmp($sentHash, $check) == 0) {
                  die("$"."MAP"."$"."RECVR ". $row["username"] ."[NL]". $row["guid"] ."[NL]". $row["email"] ."[NL]". $row["E"] ."[NL]". $row["N"] ."[NL]". $row["signature"] ."[NL]". $row["D_DEC_HASH"] ."[NL]". $row["D"] ."[NL]");
               }
               else {
                  die("$"."MAP"."$"."INCORRECT_PASS");
               }
            }
         }
         sqlDC($con);
         die("$"."MAP"."$"."NO_SUCH_ACCOUNT");
         return;
      case 4:
         sqlDC($con);
         die("$"."MAP"."$"."SERVER_REPLY");
         return;
   }

   function CreateGUID($con) {
      global $XXZVariables;
      $current = "".substr(dirname($_SERVER[PHP_SELF]),strrpos(dirname($_SERVER[PHP_SELF]),'/') + 1)."";

      $startingVal = 10000003; //8 digit number
      $iteration = rand(13, 29);
      //Ok, here's how this works

      //Step 1: Access the Database, and sort it by the guids
      $AT = $con->real_escape_string($XXZVariables["account_table"][$current]);
      $result = sqlCall($con, "SELECT * FROM $AT ORDER BY guid ASC");
      $i = 0;
      while($row = $result->fetch_array(MYSQLI_ASSOC)) {
         $i++;
         $rowGuid[$i] = $row["guid"];
         //echo "ROW ".$i.": ".$rowGuid[$i]." or ".$row["guid"]."\n";
      }
      sqlDC($result);
      //Step 2: Pick the final entry in the database
      $lastguid = $rowGuid[$i];
      if(!isSet($lastguid)) {
         //lets go one before it
         $i--;
         $lastguid = $rowGuid[$i];
         //is it still not there?
         if(!isSet($lastguid)) {
            //we must be the first guy, lulz
            return $startingVal;
         }
         else {
            //Iterate the last guid with a random iteration (from 13-29)
            $newGuid = $lastguid + $iteration;
            //Step 4: Return the Added Number
            return $newGuid;
         }
      }
      else {
         //Iterate the last guid with a random iteration (from 13-29)
         $newGuid = $lastguid + $iteration;
         //Step 4: Return the Added Number
         return $newGuid;
      }
   }

   function checkName($name, $con) {
      global $XXZVariables;
      $current = "".substr(dirname($_SERVER[PHP_SELF]),strrpos(dirname($_SERVER[PHP_SELF]),'/') + 1)."";

      if (!isset($name)) {
         //die("$"."EMPTY?");
         return 1;
      }
      if (strLen($name) < 5) {
         //die("$"."TOO_LONG");
         return 2;
      }
      if (strLen($name) > 30) {
         //die("$"."TOO_LONG");
         return 3;
      }
      if (!preg_match("/^[A-Za-z0-9]+?$/", $name)) {
         //die("$"."BAD_CHARS\n");
         return 4;
      }

      $AT = $con->real_escape_string($XXZVariables["account_table"][$current]);
      $result = sqlCall($con, "SELECT * FROM $AT") or die("$"."INTERNAL_ERROR\n");

      while($row = $result->fetch_array(MYSQLI_ASSOC))  {
         if(strCmp(strtolower($row["username"]), strtolower($name)) == 0) {
            return 5;
         }
      }
      sqlDC($result);
      return 0;
   }
?>
