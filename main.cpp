// This program should open "prueba.txt", read it and send it to the receiver socket
// librerías:

#include <iostream>
#include <vector>
#include <set>
#include <csignal>
#include <unistd.h>
#include <atomic>
#include <thread>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include "socket.cpp"

// Constantes:

#define BUFLEN  1024
#define ADDR1    "127.0.0.1"

#define ADDR2    "127.0.0.2"
#define LPORT   8880
#define EPORT   8881    

std::atomic<bool> quit(false);
std::set<std::pair<int, std::thread *>> active_threads;

struct sockaddr_in  server_local_address, server_external_address,
                    client_local_address, client_external_address;


struct CommandLineArguments
{
    // flags
    bool show_help = false;
    bool server_mode = false;
    bool client_mode = false;
    
    // valores
    unsigned short conn_port = 0;
    std::string conn_addr = 0;
    
    // otros
    std::vector<std::string> other_arguments;
    
    CommandLineArguments(int argc, char* argv[]);
};


CommandLineArguments::CommandLineArguments(int argc, char* argv[]) {

    std::cout << "entro" << std::endl;
    int arg;
    while ((arg = getopt(argc, argv, "hcsp:01")) != -1)
    {
        switch (arg)
        {
        case '0':
        case '1':
            std::cerr << "opción " << arg << std::endl;
            break;
        case 'h':
            std::cerr << "opción h\n";
            show_help = true;
            break;
        case 'c':
            std::cerr << "opción c\n";
            client_mode = true;
            conn_addr = std::string(optarg);
            break;
        case 's':
            std::cerr << "opción s\n";
            server_mode = true;
            break;
        case 'p':
            std::cerr << "opción p con valor: " << optarg << std::endl;
            conn_port = std::atoi(optarg);
            break;
        case '?':
            throw std::invalid_argument("Argumento de linea de comandos desconocido");
        default:
            throw std::runtime_error("Error procesando la linea de comandos");
        
            break;
        }
    }

    // Recuperamos el resto de argumentos
    if (optind < argc) {
        std::cerr << "-- argumentos no opción --\n";
        for (; optind < argc; ++optind) {
            std::cerr << "argv[" << optind << "]: "
            << argv[optind] << std::endl;
            other_arguments.push_back(argv[optind]);
        }
    }
        std::cerr << std::endl;
        std::cerr << std::endl;
        std::cerr << std::endl;
        std::cerr << std::endl;
}

// Prototipos:

sockaddr_in make_ip_address(const std::string& ip_address, int port);
void read_send(std::exception_ptr& eptr, std::string &filename, Socket<Message> &local_socket, sockaddr_in &ext_addr);
void receive_msg(std::exception_ptr& eptr, Socket<Message> &local_socket, sockaddr_in &ext_addr);
void request_cancellation(std::thread& thread);
void int_signal_handler(int signum);
void create_thread(std::string name, std::exception_ptr & eptr ,Socket<Message> &socket, sockaddr_in &external_address, int& threadnum);

int protected_main (int argc, char *argv[])
{  

    // variables:
      int                 recv_len, ret;
      std::string         command;
      char                buffer[BUFLEN];

    //

    // manejo de argumentos

      CommandLineArguments arguments(argc, argv);

    std::cout << "entro" << std::endl;
    //
  if (arguments.show_help || argc < 2) {
    
    std::cout << "--- Mensaje de ayuda del programa ---" << std::endl;
    std::cout << std::endl;
    std::cout << "server mode  | Netcp -s -p [PORT]" << std::endl;
    std::cout << "client mode  | Netcp -c [IP_ADDR] -p [PORT]" << std::endl;
    std::cout << "help message | Netcp -h (shows this message)" << std::endl;
    std::cout << std::endl;
    return 0;

  } else if (arguments.server_mode ) {

    std::signal(SIGINT, &int_signal_handler);
    std::signal(SIGTERM, &int_signal_handler);
    std::signal(SIGHUP, &int_signal_handler);

    memset((char*) &server_local_address, 0, sizeof(server_local_address));
    server_local_address = make_ip_address(arguments.conn_addr, arguments.conn_port);
    server_external_address = client_local_address;
    if (server_local_address.sin_addr.s_addr == 0) {
      throw std::system_error(errno, std::system_category(),
                            "no se pudo crear la dirección local.");
    }
    

    Socket<Message> server_socket(server_local_address);

    std::exception_ptr sEptr {};

    std::thread server_thread(&receive_msg, std::ref(sEptr), std::ref(server_socket), std::ref(server_external_address));

    while(!quit);

    request_cancellation(server_thread);
    server_thread.join();

    if(sEptr) {
      std::rethrow_exception(sEptr);
    }

  } else if (arguments.client_mode ) {

    std::signal(SIGINT, &int_signal_handler);
    std::signal(SIGTERM, &int_signal_handler);
    std::signal(SIGHUP, &int_signal_handler);
    // direcciones:
      
      memset((char*) &client_local_address, 0, sizeof(client_local_address));
      client_local_address = make_ip_address("",0);

      memset((char*) &client_external_address, 0, sizeof(client_external_address));
      client_external_address = make_ip_address(arguments.conn_addr, arguments.conn_port);
      if (client_external_address.sin_addr.s_addr == 0) {    
        throw std::system_error(errno, std::system_category(),
                              "no se pudo crear la dirección externa.");
      }
    //
    // sockets:
      Socket<Message> client_socket(client_local_address);
    //
    // leer y enviar el archivo
      int threadnum = 0;
      std::exception_ptr cEptr{};
      while(!quit) {
        getline(std::cin, command);
        if (command == "/quit") {
          quit = true;
        } else {
          std::string name = command; 
          std::thread*  client_thread =  new  std::thread(&read_send, std::ref(cEptr), std::ref(name),std::ref(client_socket), std::ref(client_external_address));
          
          active_threads.insert(std::pair<int, std::thread *>(threadnum, client_thread));
          threadnum ++;
        }
      }

      //while(!active_threads.empty()) {
      //  std::pair<int, std::thread *> aux = active_threads.back();
      //  request_cancellation(*aux.second);
      //  aux.second->join();
      //}
      
      if (cEptr) { 
          std::rethrow_exception(cEptr);
      }

  } else {
    throw std::system_error(errno, std::system_category(),
    "no se ha seleccionado un rol, o ambos han sido seleccionados");
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


void read_send(std::exception_ptr& eptr, std::string &filename, Socket<Message> &local_socket, sockaddr_in &ext_addr)
{
    try {
      sigset_t set;
      sigaddset(&set, SIGINT);
      sigaddset(&set, SIGTERM);
      sigaddset(&set, SIGHUP);

      pthread_sigmask(SIG_BLOCK, &set, nullptr);

      int ret ;
      struct Message message ;

    
      int fileFd = open(filename.c_str(), O_RDONLY);
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

      quit = true;

    } catch(...) {
        eptr = std::current_exception();
    }
}

void receive_msg(std::exception_ptr& eptr, Socket<Message> &local_socket, sockaddr_in &ext_addr) 
{
  try {
    //sigset_t set;
    //sigaddset(&set, SIGINT);
    //sigaddset(&set, SIGTERM);
    //sigaddset(&set, SIGHUP);

    //pthread_sigmask(SIG_BLOCK, &set, nullptr);

    int fileFd = open("out_file.txt", O_WRONLY);
    if (fileFd < 0){
      throw std::system_error(errno, std::system_category(),
                              "no se pudo abrir un fichero de salida");
    }
    struct Message message;
    std::string command;

    
    while(1) {

      std::cout << "Lectura del fichero en el servidor: \n";
      local_socket.receive_from(message, ext_addr);
      std::cout << "Datos recibidos: \n\n\n";
      std::cout << message.text.data() << std::endl;
      
      //write(fileFd, message.text.data(), message.text.size()-1);
      std::cout << "\n\n\n";

    }
    if (int result = close(fileFd) < 0){
      throw std::system_error(errno, std::system_category(),
                              "no se pudo cerrar el fichero de salida");  
    }

    quit = true;
  }  catch(...) {
        eptr = std::current_exception();
  }
}

void request_cancellation(std::thread& thread)
{
    ;
    if(int result = pthread_cancel(thread.native_handle()) <  0){
        throw std::system_error(errno, std::system_category(),
                            "no se pudo cancelar el hilo.");
    }
}

void int_signal_handler(int signum) 
{
    quit = true;
}

void create_thread(std::string name, std::exception_ptr & eptr ,Socket<Message> &socket, sockaddr_in &external_address, int& threadnum) 
{
  
}