// This program should open "prueba.txt", read it and send it to the receiver socket
// librerías:

#include<iostream>
#include<thread>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/fcntl.h>
#include "socket.cpp"

// Constantes:

#define BUFLEN  1024
#define ADDR1    "127.0.0.1"

#define ADDR2    "127.0.0.2"
#define LPORT   8880
#define EPORT   8881    

// Prototipos:

sockaddr_in make_ip_address(const std::string& ip_address, int port);
void read_send(std::exception_ptr& eptr, char* &fileFD, Socket<Message> &local_socket, sockaddr_in &ext_addr);
void receive_msg(std::exception_ptr& eptr, Socket<Message> &local_socket, sockaddr_in &ext_addr);



int protected_main (int argc, char *argv[])
{  
  if ( argc == 2) {
    // variables:
      struct sockaddr_in  local_address, external_address;
      int                 addrlen = sizeof(local_address), recv_len, ret;
      char                buffer[BUFLEN];

    //

    // File oppening:

      
    //


  // dirección:
    memset((char*) &local_address, 0, sizeof(local_address));
    local_address = make_ip_address(ADDR1, LPORT);
    if (local_address.sin_addr.s_addr == 0) {
      throw std::system_error(errno, std::system_category(),
                            "no se pudo crear la dirección local.");
    }
    memset((char*) &external_address, 0, sizeof(external_address));
    external_address = make_ip_address(ADDR2, EPORT);
    if (external_address.sin_addr.s_addr == 0) {
  
      throw std::system_error(errno, std::system_category(),
                            "no se pudo crear la dirección externa.");
    }

  // socket:
    Socket<Message> local_socket(local_address);
    Socket<Message> external_socket(external_address);
    
  // leer y enviar el archivo
    std::exception_ptr eptr{};
    
    std::thread sendingThread(&read_send, std::ref(eptr),std::ref(argv[1]), std::ref(local_socket), std::ref(external_address));
    std::thread receiveThread(&receive_msg, std::ref(eptr), std::ref(external_socket), std::ref(local_address));

    
    receiveThread.join();
    sendingThread.join();
    if (eptr) {
        std::rethrow_exception(eptr);
    } 
  } else {
    std::cout << "falta el nombre del fichero a enviar " << std::endl;
  
  }

  return 0;
}


// --- error managing ---

int main (int argc, char *argv[])
{
  try
  {
    return protected_main(argc, argv);
  }
  catch(const std::bad_alloc& e)
  {
    std::cerr << "mitalk :" << "memoria insuficiente\n";
    return 1;
  }
  catch(const std::system_error& e)
  {
    std::cerr << "mitalk :" << e.what() << "\n";
    return 2;
  }

  catch(...)
  {
    std::cout << "Error desconocido\n";
    return 99;
  }
  
}


//------ functions ------

sockaddr_in make_ip_address(const std::string& ip_address, int port)
{
  sockaddr_in address;

  address.sin_family = AF_INET;  
  
  address.sin_port = htons(port);  
  // Dependiendo de la dirección dada se 
  if (ip_address == "") {
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    
  } else {

    // opción igualmente válida (al parecer)
    // local_address.sin_addr.s_addr = inet_addr(ip_address.c_str());
    inet_aton(ip_address.c_str(), &(address.sin_addr));
    
  }

  return address;
}


void read_send(std::exception_ptr& eptr, char* &filename, Socket<Message> &local_socket, sockaddr_in &ext_addr)
{
    int ret ;
    struct Message message ;

    try {
      int fileFd = open(filename, O_RDONLY);
      if (fileFd  < 0) {
        throw std::system_error(errno, std::system_category(),
                            "no se pudo abrir el fichero.");
  
      }


        while((ret = read(fileFd, message.text.data(), message.text.size()-1)) > 0 ) {
          message.text.data()[ret] = 0x00;
          local_socket.send_to(message, ext_addr);
        }

    if ( int is_closed = close(fileFd) < 0) {
      throw std::system_error(errno, std::system_category(),
                            "no se pudo cerrar el fichero.");
  
    }

    } catch(...) {
        eptr = std::current_exception();
    }
}

void receive_msg(std::exception_ptr& eptr, Socket<Message> &local_socket, sockaddr_in &ext_addr) {
  try {

    struct Message message;

    
    while(1) {
      std::cout << "Lectura del fichero en el servidor: \n";
      local_socket.receive_from(message, ext_addr);
      std::cout << "Datos recibidos: \n\n\n";
      std::cout << message.text.data();
      std::cout << "\n\n\n";
    }

  }  catch(...) {
        eptr = std::current_exception();
  }
}