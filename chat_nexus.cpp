#include <iostream>
#include <array>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int Socket(int domain, int type, int protocol);

sockaddr_in make_ip_address(const std::string& ip_address, int port);


struct Message
{
    std::array<char, 1024> text;
};


int main () 
{

    int fd1 = Socket(AF_INET,SOCK_DGRAM,0);

    int fd2 = Socket(AF_INET,SOCK_DGRAM,0);

    sockaddr_in local_address1 = make_ip_address("0.0.0.1",htons(0));
    sockaddr_in local_address2 = make_ip_address("0.0.0.2",htons(1));

    int result1 = bind(fd1, reinterpret_cast<const sockaddr*> (&local_address1),
                      sizeof(local_address1));
    
    int result2 = bind(fd1, reinterpret_cast<const sockaddr*> (&local_address2),
                      sizeof(local_address2);
    
    if (result1 < 0 || result2 < 0) {
        std::cerr << "falló bind " << '\n';
        return 5;
    }

    std::string message_text("Hola Mundo!");
    Message message;

}

int Socket(int domain, int type, int protocol)
{
  int fd = socket(domain,type,protocol);
  // comprobar el éxito del socket antes de continuar
  if (fd < 0) {
    std::cerr << "no se pudo crear el socket: " << 
      strerror(errno) << '\n';
    return 3;
  }
  return fd; 
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