#include "socket.h"

Socket::Socket(const sockaddr_in& address)
{
  fd_ = socket(AF_INET,SOCK_DGRAM,0);
  
  if (fd_ < 0) {
    std::cerr << "no se pudo crear el socket: " << 
    strerror(errno) << '\n';
    assert(1);
  }

    int result = bind(fd_, reinterpret_cast<const sockaddr*> (&address),
                      sizeof(address));
    if (result < 0) {
        std::cerr << "falló bind " << '\n';
        assert(1);
    }

}

Socket::~Socket() {
  close(fd_);
}