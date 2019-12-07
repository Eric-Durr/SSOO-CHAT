// This program should open "prueba.txt", read it and send it to the receiver socket



#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include "socket.h"

sockaddr_in make_ip_address(const std::string& ip_adrress, int prot);

int main (void){


    Socket senderSocket();
    return 0;
}


sockaddr_in make_ip_address(const std::string& ip_adrress, int prot) {

}