#include "cli/clientCommandHandler.hpp"

ClientCommandHandler::ClientCommandHandler(const CLIMessageHandler &cliMessage) : socket(socket)
{
    registerCommand("start", std::bind(&ClientCommandHandler::start, this));
    registerCommand("stop", std::bind(&ClientCommandHandler::stop, this));
    registerCommand("restart", std::bind(&ClientCommandHandler::restart, this));
}

void ClientCommandHandler::start()
{
    std::cout << "Start command executed." << std::endl;
    std::vector<char> message(std::begin("start"), std::end("start") - 1);
    socket.send(message);
    // Your start logic here
}

void ClientCommandHandler::stop()
{
    std::cout << "Stop command executed." << std::endl;
    // Your stop logic here
}

void ClientCommandHandler::restart()
{
    std::cout << "Restart command executed." << std::endl;
    // Your restart logic here
}