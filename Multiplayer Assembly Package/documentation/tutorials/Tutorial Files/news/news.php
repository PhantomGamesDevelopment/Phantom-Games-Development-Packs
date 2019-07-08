<?php
   // News Data
   // Accesses the MySQL Table to send out News to the game clients
   require("path/to/sql.php");

   function obtainNews() {
      $sql = sqlLogin("host_newsUsr", "sqlPassowrd", "gameNews");

      $Highest = 0;

      $result = sqlCall($sql, "SELECT * FROM News");
      while($row = $result->fetch_array(MYSQLI_ASSOC)) {
         $PiD = $row["postIndex"];
         $PTitle[$PiD] = $row["postTitle"];
         $PBy[$PiD] = $row["postAuthor"];
         $PDate[$PiD] = $row["postDate"];
         $PCont[$PiD] = $row["postContent"];
         if($PiD > $Highest) {
            $Highest = $PiD;
         }
      }

      //loop backwards...
      for($x = $Highest; $x > 0; $x--) {
         echo "$"."MAP"."$"."NEWS ".$PTitle[$x]."\t"."<Font:Arial:20><Just:Center>".$PTitle[$x]."[n]<Color:6B238E><Font:Arial:18>Author: ".$PBy[$x]." | ".$PDate[$x]."[n]<Just:Left><Color:000000><Font:Arial:16>".$PCont[$x]."\n";
      }
      $sql->close();
   }

   obtainNews();

?>

