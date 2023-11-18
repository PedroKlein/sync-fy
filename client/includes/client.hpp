#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "socket/socket.hpp"

// class Client
// {
// private:
//     pthread_t socketThread;
//     pthread_t cliThread;
//     pthread_t fileManagerThread;
//     Socket socket;

//     static void* runCLI(void* arg);
//     static void* runFileManager(void* arg);
// public:
//     Client(char *userName, char *serverAddress, int *port);
//     ~Client();
//     void Start();
// };

// // Public Methods
// Client::Client(char *userName, char *serverAddress, int port)  : socket(serverAddress, port)
// {
//     Socket newSocket(serverAddress, port);
//     this->socket = newSocket;
// }

// Client::~Client()
// {
// }

// void Client::Start() {
//     std::clog << "LOG: starting client" << std::endl;
//     if (pthread_create(&this->socketThread, NULL, this->socket.Start, &this->socket) != 0){
//         std::cerr << "ERR: failed to create socket thread\n\t" << strerror(errno) << std::endl;
//         exit(errno);
//     }

//     if (pthread_create(&this->cliThread, NULL, this->runCLI, this) != 0) {
//         std::cerr << "ERR: failed to create cli thread\n\t" << strerror(errno) << std::endl;
//         exit(errno);
//     }

//     if (pthread_create(&this->cliThread, NULL, this->runFileManager, this) != 0) {
//         std::cerr << "ERR: failed to create file manager thread\n\t" << strerror(errno) << std::endl;
//         exit(errno);
//     }

//     pthread_join(this->socketThread, NULL);
//     pthread_join(this->cliThread, NULL);
//     pthread_join(this->fileManagerThread, NULL);
// }


// // Private Methods 
// void* Client::runCLI(void *args) {

//     // Create CLI class 


//     pthread_exit(NULL);
// }

// void* Client::runFileManager(void *args) {

//     // Create FileManager class

//     pthread_exit(NULL);
// }

#endif
