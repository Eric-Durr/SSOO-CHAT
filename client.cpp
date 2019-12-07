#include<stdio.h>
#include<iostream>
#include<string.h>
#include <unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define SERVER "127.0.0.1"
#define BUFLEN 512
#define PORT 8888


int main()
{
    struct sockaddr_in si_other;
    int s, i, slen = sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            return 1;
    }

    memset((char*) &si_other, 0, sizeof(si_other));

    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if ( inet_aton(SERVER, &si_other.sin_addr) == 0)
    {
        return 2;
    }

    while(1) { 
        printf("Enter message: ");
        gets(message);

        socklen_t temp = slen;
        if ((sendto(s, message, strlen(message), 0, (struct sockaddr*) &si_other, temp)) == -1) {
            
            return 3;
        }

        memset(buf, '\0', BUFLEN);

        if ((recvfrom(s, buf, BUFLEN, 0,
             (struct sockaddr*) &si_other,(socklen_t*) &slen)) == -1 ) {
            return 4;
        }
        puts(buf);
    }

    close(s);
    return 0;
}