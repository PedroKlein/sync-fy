#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class TCPSocket
{
  public:
    TCPSocket(int socket) : socket(socket)
    {
    }

    virtual ~TCPSocket()
    {
        close(socket);
    }

    void send(const std::string &data)
    {
        ::send(socket, data.c_str(), data.size(), 0);
    }

    std::string receive(size_t size)
    {
        std::vector<char> buffer(size);
        ssize_t received = recv(socket, buffer.data(), size, 0);
        return std::string(buffer.begin(), buffer.begin() + received);
    }

  protected:
    int socket;
};