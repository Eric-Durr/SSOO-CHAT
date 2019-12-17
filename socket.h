#pragma once

#include <iostream>
#include <cassert>
#include <array>
#include <cerrno>
#include <cstring>
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

template<class T>
class Socket
{
public:
    Socket(const sockaddr_in& address);
    ~Socket();

    void send_to(const T& message, const sockaddr_in& address);
    void receive_from(T& message, sockaddr_in& address);
    
private:
    int fd_;
};

