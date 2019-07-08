<?php
   require("path/to/sql.php");
   
   $guid = $_POST["guid"];
   $cert = $_POST["cert"];
   $data = $_POST["data"];
   
   //change this line... yada yada yada
   $sql = sqlLogin("host_authUsr", "sqlPassowrd", "authSystem");
   //check the GUID & signature to see if they are who they say they are
   $guid = $sql->real_escape_string($guid);
   $cert = $sql->real_escape_string($cert);
   $data = $sql->real_escape_string($data);

   $result = sqlCall($sql, "SELECT * FROM Accounts WHERE guid='$guid' AND signature='$cert'") or die("$MAP"."$INTERNAL_ERROR\n");
   if($result->fetch_array(MYSQLI_ASSOC) == NULL) {
      //naughty naughty hacker...
      die("$MAP"."$INVALID_ACCOUNT");
   }
   
   $sql->close();
   $sql = sqlLogin("host_othrUsr", "otherSQLPassword", "rankSystem");
   //DATA == GET - Send Down Rank, Else: Save Info
   if(strcmp($data, "GET")) {
     $result = sqlCall($sql, "SELECT * FROM RankTable WHERE guid='$guid'") or die("$MAP"."$INTERNAL_ERROR\n");
     if($result->fetch_array(MYSQLI_ASSOC) == NULL) {
        die("$MAP"."$NO_DATA_ON_SERVER");
     }
     else {
        $row = $result->fetch_array(MYSQLI_ASSOC);
        die("$MAP"."$DOWNLOAD ".$row[rankdata]);
     }
   }
   else {
     $result = sqlCall($sql, "SELECT * FROM RankTable WHERE guid='$guid'") or die("$MAP"."$INTERNAL_ERROR\n");
     if($result->fetch_array(MYSQLI_ASSOC) == NULL) {
        $result2 = sqlCall($sql, "INSERT INTO RankTable (guid, rankdata) VALUES ('$guid', '$data')");
        if($result2 == -1) {
           die("$MAP"."$UPLOAD_FAIL");
        }
        die("$MAP"."$UPLOAD_SUCCESS");
     }
     else {
        $result2 = sqlCall($sql, "UPDATE RankTable SET rankdata='$data' WHERE guid='$guid'");
        if($result2 == -1) {
           die("$MAP"."$UPLOAD_FAIL");
        }
        die("$MAP"."$UPLOAD_SUCCESS");
     }
   }
?>
