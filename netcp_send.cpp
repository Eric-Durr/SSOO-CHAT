// This program should open "prueba.txt", read it and send it to the receiver socket



#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include "socket.h"

sockaddr_in make_ip_address(const std::string& ip_address, int port);

int main (void){

    Message msg_to_send;
    sockaddr_in senderIp = make_ip_address ("0.0.0.1", 1 );
    sockaddr_in receiverIp = make_ip_address ("0.0.0.2", 2 );
    
    Socket senderSocket(senderIp);
    senderSocket.send_to(msg_to_send, receiverIp);
    return 0;
}



sockaddr_in make_ip_address(const std::string& ip_address, int port)
{

  sockaddr_in local_address{};

  local_address.sin_family = AF_INET;  
  
  // Dependiendo de la dirección dada se 
  if (ip_address == "") {
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);
  } else {
    // opción igualmente válida (al parecer)
    // local_address.sin_addr.s_addr = inet_addr(ip_address.c_str());
    inet_aton(ip_address.c_str(), &(local_address.sin_addr));
    
  }
  
  local_address.sin_port = htons(port);  
  return local_address;
}
