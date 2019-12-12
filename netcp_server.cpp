// This program should open "prueba.txt", read it and send it to the receiver socket
// librerías:

#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include "socket.cpp"

// Constantes:

#define BUFLEN  1024
#define PORT    8888    

// Prototipos:

sockaddr_in make_ip_address(const std::string& ip_address, int port);

int main (int argc, char *argv[]){

  if ( argc == 2) {
    // variables:
      struct sockaddr_in  server_address, client_address;
      int                 addrlen = sizeof(server_address), recv_len, ret;
      char                buffer[BUFLEN];

    //

    // File oppening:
      int fileFd = open(argv[1], O_RDONLY);
      if (fileFd < 0) {
          std::cerr << "Falló la apertura del fichero: " << '\n';
          return  1;
      }
    //

    // Address: 
      memset((char *) &server_address, 0, sizeof(server_address));
      server_address = make_ip_address("", PORT);
    //

    // Socket:
      Socket<Message> server_socket(server_address);

    //

    while(1){
      struct Message message;

      std::cout << "Sending file ... \n";
      
      server_socket.receive_from(message, client_address);
      
      std::cout << "conected to [" << inet_ntoa(client_address.sin_addr)
      << ":" << ntohs(client_address.sin_port) << "]" <<std::endl;
      
      while((ret = read(fileFd, message.text.data(), message.text.size()-1)) > 0) {

        message.text.data()[ret] = 0x00;
        server_socket.send_to(message, client_address); 
      }

    }
  } else {
    std::cout << "falta el nombre del fichero a enviar " << std::endl;
  
  }
}


sockaddr_in make_ip_address(const std::string& ip_address, int port)
{
  sockaddr_in address;

  address.sin_family = AF_INET;  
  
  address.sin_port = htons(port);  
  // Dependiendo de la dirección dada se 
  if (ip_address == "") {
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    
  } else {

    // opción igualmente válida (al parecer)
    // local_address.sin_addr.s_addr = inet_addr(ip_address.c_str());
    inet_aton(ip_address.c_str(), &(address.sin_addr));
    
  }

  return address;
}
