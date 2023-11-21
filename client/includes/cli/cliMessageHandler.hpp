#include "messageTypes.hpp"
#include "models/baseModel.h"
#include "socket/tcpSocket.hpp"
#include <iostream>

class CLIMessageHandler : public MessageHandler
{
  public:
    CLIMessageHandler(const TCPSocket &socket);
};