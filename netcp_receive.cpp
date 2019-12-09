// This program should open socket which will eait to receive the file info and
// show it's content through the screen.

#include<iostream>
#include<stdio.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include "socket.cpp"

// Constantes:

#define BUFFER  1024
#define PORT    8080    

// Prototipos:

sockaddr_in make_ip_address(const std::string& ip_address, int port);


int main (void){

  
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
