// This program should open socket which will eait to receive the file info and
// show it's content through the screen.

#include<iostream>
#include<stdio.h>

#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include "socket.cpp"

// Constantes:

#define SERVER "127.1.1.1"
#define BUFLEN  1024
#define PORT    8888    

// Prototipos:

sockaddr_in make_ip_address(const std::string& ip_address, int port);

// Main program:

int protected_main (int argc, char *argv[]){
  // variables:
    struct sockaddr_in  local_address;
    int                 addrlen = sizeof(local_address);
    char                buffer[BUFLEN];
    struct Message      message;

    
  // dirección:
    memset((char*) &local_address, 0, sizeof(local_address));
    local_address = make_ip_address(SERVER, 0);
    if (local_address.sin_addr.s_addr == 0) {
      printf("Error en la creación de la dirección del socket");
      return 1;
    }

  // socket:
    Socket<Message> local_socket(local_address);
    message.text.at(0) = '.';
  // try-catch loop
    while(1 && message.text.at(0) == '.') {
      std::cout << "Lectura del fichero en el servidor: \n";

      local_socket.receive_from(message, local_address);
      std::cout << "Datos recibidos: \n\n\n";
      std::cout << message.text.data();
      std::cout << "\n\n\n";
    }

    return 0;
}

int main (int argc, char *argv[])
{
  try
  {
    return protected_main(argc, argv);
  }
  catch(const std::bad_alloc& e)
  {
    std::cerr << "mitalk :" << "memoria insuficiente\n";
    return 1;
  }
  catch(const std::system_error& e)
  {
    std::cerr << "mitalk :" << e.what() << "\n";
    return 2;
  }

  catch(...)
  {
    std::cout << "Error desconocido\n";
    return 99;
  }
  
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
