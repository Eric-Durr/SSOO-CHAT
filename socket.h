#include <iostream>
#include <cassert>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>

class Socket
{
public:
    Socket(const sockaddr_in& address);
    ~Socket();

private:
    int fd_;
};

