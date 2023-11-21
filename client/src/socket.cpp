#include "socket.hpp"

Socket::Socket(char *serverAddress, int port)
{
    if (clientSocket != 0)
        close(clientSocket);

    server = gethostbyname(serverAddress);
    if (server == NULL)
    {
        std::cerr << "ERR: could not get host with name " << server << std::endl;
        exit(errno);
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1)
    {
        std::cerr << "ERR: failed to create a socket\n\t|=> " << strerror(errno) << std::endl;
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

Socket::~Socket()
{
    close(clientSocket);
}

void Socket::SendData(std::string userName) const
{

    // char buffer[BUFSIZ];
    char msg[BUFFER_SIZE];
    while (true)
    {
        printf("Enter the message: ");
        // bzero(buffer, BUFFER_SIZE);
        bzero(msg, BUFFER_SIZE);
        fgets(msg, BUFFER_SIZE, stdin);

        Json::Value jsonObject;
        jsonObject["user"] = userName;
        jsonObject["msg"] = msg;

        std::ostringstream jsonStringStream;
        jsonStringStream << jsonObject;
        // buffer = jsonStringStream.str();

        /* write in the socket */
        ssize_t n = write(clientSocket, &jsonStringStream.str()[0], strlen(&(jsonStringStream.str())[0]));
        if (n < 0)
            printf("ERROR writing to socket\n");
    }
}

void Socket::StartReceivePingThread()
{
    if (pthread_create(&receivePingThread, nullptr, &receivePing, this) != 0)
    {
        std::cerr << "Error creating receive ping thread." << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Socket::connectToServer()
{
    int err = connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if (err == -1)
    {
        std::cerr << "ERR: failed to connect to socket server\n\t|=> " << strerror(errno) << std::endl;
        exit(errno);
    }

    std::clog << "LOG: successfully connected to server " << serverAddress.sin_addr.s_addr << ":"
              << serverAddress.sin_port << std::endl;
}

void *Socket::receivePing(void *arg)
{
    Socket *socketInstance = static_cast<Socket *>(arg);

    char buffer[BUFFER_SIZE];
    while (true)
    {
        bzero(buffer, BUFFER_SIZE);
        ssize_t readBytes = read(socketInstance->clientSocket, buffer, BUFFER_SIZE);
        if (readBytes > 0)
        {
            // Handle the received ping message, e.g., print it
            std::cout << "Received Ping: " << buffer << std::endl;
        }
        else if (readBytes == 0)
        {
            // Connection closed by the server, handle accordingly
            std::cerr << "Connection closed by the server." << std::endl;
            break;
        }
        else
        {
            // Handle error if needed
            std::cerr << "Error receiving ping message." << std::endl;
        }
    }

    pthread_exit(nullptr);
}
