// This program should open "prueba.txt", read it and send it to the receiver socket



#include<iostream>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include "socket.cpp"

sockaddr_in make_ip_address(const std::string& ip_address, int port);

int main (int argc, char *argv[]){

    Message msg_to_send;
    char buffer[1024];


    // File oppening:
    int fileFd = open("./prueba.txt", O_RDONLY);
    if (fileFd < 0) {
        std::cerr << "Falló la apertura del fichero: " << '\n';
        return  1;
    }

    sockaddr_in senderIp = make_ip_address ("", 0 );
    sockaddr_in receiverIp = make_ip_address ("8.8.8.8", 8080 );
    
    Socket senderSocket(senderIp);

    std::string s("hello world");

    //memset(&senderIp, 0, sizeof(senderIp));
    int ret;
    //while ((ret = read(fileFd, buffer, sizeof(buffer) - 1)) > 0 ) {
    //    buffer[ret] = 0x00;
    //    std::string message_text = buffer;
    //    message_text.copy(msg_to_send.text.data(), 
    //                      msg_to_send.text.size() - 1, 0);
    //}
    s.copy(msg_to_send.text.data(), msg_to_send.text.size() - 1 , 0);
    senderSocket.send_to(msg_to_send, receiverIp);
    //senderSocket.send_to(msg_to_send, receiverIp);
    return 0;
}



sockaddr_in make_ip_address(const std::string& ip_address, int port)
{

  sockaddr_in address{};

  address.sin_family = AF_INET;  
  
  // Dependiendo de la dirección dada se 
  if (ip_address == "") {
    address.sin_addr.s_addr = htonl(INADDR_ANY);
  } else {
    // opción igualmente válida (al parecer)
    // local_address.sin_addr.s_addr = inet_addr(ip_address.c_str());
    inet_aton(ip_address.c_str(), &(address.sin_addr));
    
  }
  
  address.sin_port = htons(port);  
  return address;
}
