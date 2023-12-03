#include "cli/commandHandler.hpp"

namespace cli
{
CommandHandler::CommandHandler(const MessageHandler &messageHandler)
    : messageHandler(messageHandler), directory(common::Directory(messageHandler.getSyncFolder()))
{
    registerCommand("upload", [this](const std::vector<std::string> &parameters) { upload(parameters[0]); });
    registerCommand("delete", [this](const std::vector<std::string> &parameters) { deleteFile(parameters[0]); });
    registerCommand("list", [this](const std::vector<std::string> &parameters) { listFiles(parameters[0]); });
    registerCommand("download", [this](const std::vector<std::string> &parameters) { download(parameters[0]); });
    registerCommand("exit", [this](const std::vector<std::string> &parameters) { exit(); });
}

void CommandHandler::executeCommand(std::string command, const std::vector<std::string> &parameters) const
{
    std::transform(command.begin(), command.end(), command.begin(), ::tolower); // Convert command to lowercase

    if (commands.find(command) != commands.end())
    {
        commands.at(command)(parameters);
    }
    else
    {
        std::cout << "Command not found." << std::endl;
    }
}

void CommandHandler::download(const std::string &filename) const
{
    try
    {
        messageHandler.sendDownloadFileMessage(filename);
    }
    catch (const std::exception &e)
    {
        std::cout << "File " << filename << " not found." << std::endl;
    }
}

void CommandHandler::upload(const std::string &filepath) const
{
    try
    {
        common::File file(filepath);
        messageHandler.sendFileMessage(file);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void CommandHandler::deleteFile(const std::string &filename) const
{
    messageHandler.sendDeleteFileMessage(filename);
}

void CommandHandler::listFiles(const std::string &from) const
{
    std::vector<common::FileInfo> files;

    if (from == "server")
    {
        messageHandler.sendListServerFilesMessage();
        files = messageHandler.receiveListFilesMessage();
    }
    else if (from == "client")
    {
        files = directory.listFiles();
    }
    else
    {
        std::cout << "Invalid argument for list command." << std::endl;
        return;
    }

    std::cout << std::left << std::setw(30) << "Filename" << std::setw(30) << "Modification time" << std::setw(30)
              << "Access time" << std::setw(30) << "Creation time" << std::setw(30) << "Filesize" << std::endl;
    for (const auto &file : files)
    {

        std::cout << std::left << std::setw(30) << file.filename << std::setw(30)
                  << common::DateTime::toLocalTime(file.modificationTime) << std::setw(30)
                  << common::DateTime::toLocalTime(file.accessTime) << std::setw(30)
                  << common::DateTime::toLocalTime(file.creationTime) << std::setw(30) << file.filesize << std::endl;
    }
}

void CommandHandler::exit() const
{
    std::cout << "Exiting...bye :)" << std::endl;
    ::exit(0);
}

void CommandHandler::registerCommand(const std::string &command,
                                     std::function<void(const std::vector<std::string> &)> method)
{
    commands.insert(std::make_pair(command, method));
}
} // namespace cli
