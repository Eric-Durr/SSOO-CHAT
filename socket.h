#include <iostream>
#include <cassert>
#include <array>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

struct Message
{
// ...
    std::array<char, 1024> text;
// ...
};

class Socket
{
public:
    Socket(const sockaddr_in& address);
    ~Socket();

    void send_to(const Message& message, const sockaddr_in& address);
    void receive_from(Message& message, sockaddr_in& address);
    
private:
    int fd_;
};

