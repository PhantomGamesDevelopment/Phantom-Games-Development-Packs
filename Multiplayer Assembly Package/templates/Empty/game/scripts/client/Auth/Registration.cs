//Registration.cs
//Robert Fritzen (Phantom139)
//Nearly depricated, all that remains is the TCPCall for testing server list, so for now I'll keep this.

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
