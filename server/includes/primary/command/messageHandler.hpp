#pragma once

#include "primary/connectionHandler.hpp"
#include "serverMessageHandler.hpp"
#include <filesystem/directory.hpp>
#include <filesystem/file.hpp>
#include <filesystem/fileChange.hpp>

namespace command
{
/**
 * @class MessageHandler
 * @brief Handles messages specific to the command module.
 *
 * This class is a subclass of ServerMessageHandler and provides
 * additional functionality for handling command module-specific messages.
 */
class MessageHandler : public ServerMessageHandler
{

    // implement contructor
  public:
    /**
     * @brief Constructs a MessageHandler object.
     *
     * Initializes the object with a TCP socket, the IP address of the client,
     * and a directory representing the synchronization folder.
     *
     * @param socket A reference to the common::TCPSocket object.
     * @param ip The IP address of the client.
     */
    MessageHandler(common::TCPSocket &socket, std::string ip)
        : ServerMessageHandler(socket, ip), directory(common::Directory(getSyncFolder()))
    {
    }

  private:
    const common::Directory directory;

    /**
     * @brief Handles non-specific messages received from the server.
     *
     * This function is called when a common::Message object is received from the server,
     * and it handles messages not covered by specific message handling functions.
     *
     * @param message The common::Message received from the server.
     */
    void handleOtherMessage(const common::Message &message) const override;

    /**
     * @brief Handles messages with only a header received from the server.
     *
     * This function is called when a common::MessageHeader object is received from the server,
     * and it handles messages without accompanying data.
     *
     * @param message The common::MessageHeader received from the server.
     */
    void handlePureHeaderMessage(const common::MessageHeader &message) const override;

    /**
     * @brief Callback function called when a file send request is received.
     *
     * This function is called when a common::InitSendFile message is received
     * from the server. Subclasses can override this function to perform
     * custom actions when a file send request is received.
     *
     * @param initSendFile The common::InitSendFile message received.
     */
    void onSendFileMessage(const common::InitSendFile &initSendFile) const override;

    /**
     * @brief Callback function called when a file deletion request is received.
     *
     * This function is called when a common::DeleteFile message is received
     * from the server. Subclasses can override this function to perform
     * custom actions when a file deletion request is received.
     *
     * @param deletedFile The common::DeleteFile message received.
     */
    void onDeleteFileMessage(const common::DeleteFile &deletedFile) const override;
};
} // namespace command