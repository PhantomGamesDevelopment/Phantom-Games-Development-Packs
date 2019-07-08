//Registration.cs
//Robert Fritzen (Phantom139)
//(c)Phantom Games Development 2011
//This script handles the scripts for PGD side Registration checks, as well as the actual registration process

function TestServerList() {
   if($TCP::AuthServer !$= "") {
      return "yes";
   }

   %initial = 0;
   while($Authentication::ServerList[%initial] !$= "") {
      TCP_TestServer($Authentication::ServerList[%initial], $Authentication::AuthTestPort[%initial], %initial);
      %initial++;
   }
   $Authentication::LastServer = %initial - 1;
}
