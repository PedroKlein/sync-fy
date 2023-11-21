#include "clientSocket.hpp"

ClientSocket::ClientSocket(const std::string &serverAddress, int port) : TCPSocket()
{
    server = gethostbyname(serverAddress.c_str());
    if (server == NULL)
    {
        std::cerr << "ERR: could not get host with name " << server << std::endl;
        exit(errno);
    }

    bzero((char *)&this->serverAddress, sizeof(this->serverAddress));
    this->serverAddress.sin_family = AF_INET;

    bcopy((char *)server->h_addr, (char *)&this->serverAddress.sin_addr.s_addr, server->h_length);
    if (port < 1000)
        this->serverAddress.sin_port = htons(DEFAULT_PORT);
    else
        this->serverAddress.sin_port = htons(port);

    connectToServer();
}

ClientSocket::~ClientSocket()
{
}

void ClientSocket::connectToServer()
{
    int err = connect(socketId, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (err == -1)
    {
        std::cerr << "ERR: failed to connect to socket server\n\t|=> " << strerror(errno) << std::endl;
        exit(errno);
    }

    std::clog << "LOG: successfully connected to server " << serverAddress.sin_addr.s_addr << ":"
              << serverAddress.sin_port << std::endl;
}
