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

  // variables:
    struct sockaddr_in  server_address, client_address;
    int                 addrlen = sizeof(server_address), recv_len;
    char*                buffer[BUFLEN];
    
  //

  // File oppening:
  //  int fileFd = open("./prueba.txt", O_RDONLY);
  //  if (fileFd < 0) {
  //      std::cerr << "Falló la apertura del fichero: " << '\n';
  //      return  1;
  //  }
  //

  // Address: 
    memset((char *) &server_address, 0, sizeof(server_address));
    server_address = make_ip_address("", PORT);
  //

  // Socket:
    Socket<char *[BUFLEN]> server_socket(server_address);
    
    std::cout << server_address.sin_port << std::endl;

    std::cout << server_address.sin_addr.s_addr << std::endl;
  //

  while(1){

    std::cout << "waiting for data ... \n";
    fflush(stdout);

    server_socket.receive_from(buffer, client_address);
    
    std::cout << "Received package from: "
    << inet_ntoa(client_address.sin_addr) << ":" 
    << ntohs(client_address.sin_port) << std::endl;
    std::cout << "Data: " << buffer;
    //printf("Received package from %s:%d \n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
    //printf("Data: %s\n", buf);



  }

}



sockaddr_in make_ip_address(const std::string& ip_address, int port)
{
  sockaddr_in address{};

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
