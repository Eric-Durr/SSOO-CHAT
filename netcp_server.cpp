// This program should open socket which will eait to receive the file info and
// show it's content through the screen.

#include<iostream>
#include<stdio.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include "socket.cpp"

// Constantes:

#define SERVER "127.0.0.1"
#define BUFLEN  1024
#define PORT    8888    

// Prototipos:

sockaddr_in make_ip_address(const std::string& ip_address, int port);

// Main program:

int main (void){
  // variables:
    struct sockaddr_in  client_address;
    int                 addrlen = sizeof(client_address);
    char                buffer[BUFLEN];
    struct Message      message;

    
  // dirección:
    memset((char*) &client_address, 0, sizeof(client_address));
    client_address = make_ip_address(SERVER, PORT);
    if (client_address.sin_addr.s_addr == 0) {
      printf("Error en la creación de la dirección del socket");
      return 1;
    }

  // socket:
    Socket<Message> client_socket(client_address);
    message.text.at(0) = '.';
  // try-catch loop
    while(1 && message.text.at(0) == '.') {
      std::cout << "Lectura del fichero en el servidor: \n";
      client_socket.send_to(message, client_address);
      client_socket.receive_from(message, client_address);
      std::cout << "Datos recibidos: \n\n\n";
      std::cout << message.text.data();
      std::cout << "\n\n\n";
    }

    return 0;
}


// Functions

sockaddr_in make_ip_address(const std::string& ip_address, int port)
{
  sockaddr_in local_address;

  local_address.sin_family = AF_INET;  
  local_address.sin_port = htons(port);  
  
  // Dependiendo de la dirección dada se 
  if (ip_address == "") {
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);
  } else {
   
    // local_address.sin_addr.s_addr = inet_addr(ip_address.c_str());
    inet_aton(ip_address.c_str(), &(local_address.sin_addr));
    
  }
  
  return local_address;
}
