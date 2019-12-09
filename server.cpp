#include<stdio.h>
#include<iostream>
#include<string.h>
#include <unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>


// Port and buffer constant definition

#define BUFLEN 512
#define PORT 8888

void die(char *s)
{
    perror(s);
    exit(1);
}

int main()
{
  // Sockets creation (server and client) [adresses, size variables, definition, and adress initialization]   
    struct sockaddr_in si_me, si_other;

    int s, i, slen = sizeof(si_other) , recv_len;
    char buf[BUFLEN];

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            return 1;
        
    }

    memset((char*) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
  
    if(bind(s, (struct sockaddr*) &si_me, sizeof(si_me)) == -1) {
            return 2;
        
    }
  //
    
    // calling loop

    while(1){
    
        std::cout << "waiting for data ... \n";
        fflush(stdout);

        // receive call

        if ((recv_len = recvfrom(s, buf, BUFLEN, 0,
                                 (struct sockaddr*) &si_other,(socklen_t*) &slen)) == -1 ) {
            return 3;
        }
        // printing info
        printf("Received package from %s:%d \n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n", buf);

        // send call
        //socklen_t temp = slen;
        //if ((sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, temp)) == -1) {
        //    
        //    return 4;
        //}
    }

    close(s);
    return 0;
}