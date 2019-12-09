#include "socket.h"

template<class T>
Socket<T>::Socket(const sockaddr_in& address)
{
  fd_ = socket(AF_INET, SOCK_DGRAM, 0);
  
  if (fd_ < 0) {
    std::cerr << "no se pudo crear el socket: " << 
    strerror(errno) << '\n';
    assert(1);
  }

    int result = bind(fd_, (struct sockaddr*) &address, sizeof(address));
    if (result < 0) {
        std::cerr << "falló bind: "  << strerror(errno)<<'\n';
        assert(1);
    }

}

template<class T>
Socket<T>::~Socket() 
{
  close(fd_);
}

template<class T>
void Socket<T>::send_to(const T& message, const sockaddr_in& address) 
{
  int result = sendto(fd_, &message, sizeof(message), 0,
               reinterpret_cast<const sockaddr*> (&address),
               sizeof(address));
  if (result < 0) {
      std::cerr << "falló sendto " << '\n';
      assert(1);
  }
}
template<class T>
void Socket<T>::receive_from(T& message, sockaddr_in& address) {
  
  socklen_t src_len = sizeof(address);
  int result = recvfrom(fd_, &message, sizeof(message), 0,
               (struct sockaddr*) &address, 
               &src_len);
  if (result < 0) {
  std::cerr << "falló recvfrom: "  << '\n';
    assert(1);
  }
}