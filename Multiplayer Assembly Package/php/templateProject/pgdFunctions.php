<?php

   function XXZHash($string) {
      $interior = sha1($string);
      $exterior = hash('whirlpool', $interior);
      return $exterior;  
   }

   function getUTC() {
      return time();
   }
   
   function collapseEscape($str) {
      $exploded = explode($str);
      $out = "";
      for ($i = 0; $i < strlen($str); $i++)
         $out += "\\x" . bin2hex($exploded[$i]);
	
      return $out;
   }
   
   function expandEscape($str) {
      $out = "";
      for ($i = 0; $i < strlen($str); $i++)
         $out += "\\x" . bin2hex($str[$i]);
	
      return $out;
   }   
   
   function hex2bin($data) {
      $len = strlen($data);
      for($i=0;$i<$len;$i+=2) {
         $newdata .= pack("C",hexdec(substr($data,$i,2)));
      }
      return $newdata;
   }

   function CA_private() {
      $privateKey = "YOUR PRIVATE KEY HERE";
      return $privateKey;
   }
   function CA_public() {
      $publicKey = "YOUR PUBLIC KEY HERE";
      return $publicKey;
   }
?>
