#include <iostream>

#include "includes/socket/socket.hpp"
#include "includes/client.hpp"

int main(int argc, char* argv[]) {

    if (argc < 4) {
        std::cerr << "ERR: not enough arguments were provided\n\t";
        std::cerr << "|=> "<< argv[0] << " <username> <server_address> <port>" << std::endl;
        exit(errno);
    }

    // Client client(argv[1], argv[2], argv[3]);
    // client.Start();

    Socket socket(argv[2], atoi(argv[3]));
    socket.SendData("hello world");

    return 0;
}
