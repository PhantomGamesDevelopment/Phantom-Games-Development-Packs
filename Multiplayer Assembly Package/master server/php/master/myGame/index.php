<?php
   require("sql.php");
   require("settings.php");
   global $MasterVariables;

   function registerServer($con, $IP, $exp) {
      global $MasterVariables;
      $table = $MasterVariables["table"];
   
      $added = 0;
      $IP = $con->real_escape_string($IP);
      $exp = $con->real_escape_string($exp);
      //
      $result = sqlCall($con, "SELECT * FROM $table");
      while($row = $result->fetch_array(MYSQLI_ASSOC)) {
         $rowIP = $row["IP"];
         if(strcmp($IP, $rowIP) == 0) {
            $added = 1;
            sqlCall($con, "UPDATE $table SET UTCExpry='$exp' WHERE IP='$IP'");
            //server is already there, add 5 more minutes to it's live time
            echo "$"."PGDUPDATE ".$IP;
            break; // break the loop here, we have finished our work!
         }
      }
      if($added == 0) {
         //server is not in the list, add it now
         sqlCall($con, "INSERT INTO $table (IP, UTCExpry) VALUES ('$IP', '$exp')");
         echo "$"."PGDREGISTER ".$IP;
      }
   }

   function optimizeDatabase($con) {
      global $MasterVariables;
      $table = $MasterVariables["table"];
   
      $result = sqlCall($con, "SELECT * FROM $table");
      while($row = $result->fetch_array(MYSQLI_ASSOC)) {
         $utc = $row["UTCExpry"];
         $ip = $row["IP"];
         if(time() > $utc) {
            $result2 = "DELETE FROM $table WHERE IP='$ip'";
            sqlCall($con, $result2);
         }
      }
   }
   
   $con = sqlLogin($MasterVariables["db_username"], $MasterVariables["db_password"], $MasterVariables["database"]) or die("$"."INTERNAL_ERROR");
   $table = $MasterVariables["table"];
   $mode = $_POST["mode"];
   switch($mode) {
      //server add request
      case 1:
         $currentUTC = time();
         $minutes = 5;
         $expryUTC = $currentUTC + ($minutes * 60);
         //
         if(!isSet($_POST["port"])) {
            $port = 28000;
         }
         else {
            $port = $_POST["port"];
         }
         $ipAddress = $_SERVER['REMOTE_ADDR'] .":". $port;
         //Add the new server
         RegisterServer($con, $ipAddress, $expryUTC);
         //
         optimizeDatabase($con);
         return;
      //list request
      case 2:
         optimizeDatabase($con);  //clear old servers out before sending the list to the player
      	 echo "$"."PGD"."$"."LIST ";
         $result = sqlCall($con, "SELECT * FROM $table");
         while($row2 = $result->fetch_array(MYSQLI_ASSOC)) {
            echo "$".$row2["IP"]." ";//space after each server in the list!
         } 
         return;
      default:
         optimizeDatabase($con);  //PGD will access this script every 15 - 30 minutes to optimize it automatically.
   }
?>
