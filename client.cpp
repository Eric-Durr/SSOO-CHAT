#include<stdio.h>
#include<iostream>
#include<string.h>
#include <unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>


// Constants for conection:

#define SERVER "127.0.0.1"
#define BUFLEN 2048
#define PORT 8888


int main()
{
    // local variables that will define our sockets:
    
    struct sockaddr_in si_other;       // --> Socket adress that will define the serverside
    int s, i, slen = sizeof(si_other); // --> Integer values and dimentions that we will use
    char buf[BUFLEN];                  // --> buffer size
    char message[BUFLEN];              // --> message that will be sended through the socket

    // Creation and checking of the socket (if the result isn't higer than 0, the program is ended)    
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            return 1;
    }

    // Memory setting for socket address
    memset((char*) &si_other, 0, sizeof(si_other));

    // establishing adress parameters:

    si_other.sin_family = AF_INET;      // -->  Socket kind of family
    si_other.sin_port = htons(PORT);    // -->  Socket port (must be the same as the server)

    if ( inet_aton(SERVER, &si_other.sin_addr) == 0) // --> Socket IP address stablishing and convertion (if the result isn't 0 the program is ended up)
    {
        return 2;
    }

    // Concetion loop (eternal)
    while(1) { 

        // Asking for the message that we will send
        printf("Enter message: ");
        std::string aux;
        std::cin >> aux;
        aux.copy(message,sizeof(message));

        // Sending the message to the socket established
        socklen_t temp = slen;
        if ((sendto(s, message, strlen(message), 0, (struct sockaddr*) &si_other, temp)) == -1) {
            
            return 3;
        }
        // buffer stablishing of memory 

        memset(buf, '\0', BUFLEN);

        // receiving the feedback message
        //if ((recvfrom(s, buf, BUFLEN, 0,
        //     (struct sockaddr*) &si_other,(socklen_t*) &slen)) == -1 ) {
        //    return 4;
        //}
        //puts(buf);
    }

    close(s);
    return 0;
}