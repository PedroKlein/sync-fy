
#include "primary/clientConnection/userConnection.hpp"
#include "serverMessageHandler.hpp"
#include <filesystem/directory.hpp>
#include <memory>
#include <string>

namespace localMonitor
{
/**
 * @class LocalMonitor
 * @brief Monitors and handles file changes locally, interacting with a server.
 *
 * The LocalMonitor class is responsible for monitoring changes in a local directory
 * and interacting with a server through a ServerMessageHandler. It uses a shared
 * FileChangesQueue to communicate detected changes to other components.
 */
class LocalMonitor
{
  public:
    /**
     * @brief Constructs a LocalMonitor object.
     *
     * @param messageHandler A reference to the ServerMessageHandler for server communication.
     * @param changeQueue A shared pointer to a FileChangesQueue for communication of file changes.
     */
    LocalMonitor(ServerMessageHandler &messageHandler, std::shared_ptr<FileChangesQueue> changeQueue);

    /**
     * @brief Monitors changes in the local directory and communicates with the server.
     *
     * This function continuously monitors the local directory for changes
     * and communicates with the server through the provided ServerMessageHandler.
     */
    void monitorChanges();

    /**
     * @brief Stops the monitoring process.
     *
     * This function stops the monitoring process initiated by monitorChanges().
     */
    void stopMonitoring();

  private:
    ServerMessageHandler &messageHandler;
    std::shared_ptr<FileChangesQueue> changeQueue;
    const common::Directory directory;
    bool isMonitoring;

    /**
     * @brief Performs an initial synchronization with the server.
     *
     * This function synchronizes the local directory with the server by sending
     * necessary file changes to the client.
     */
    void initialSync();

    /**
     * @brief Sends a file change to the client.
     *
     * This function sends a common::FileChange to the server through the ServerMessageHandler.
     *
     * @param fileChange The file change to be sent to the client.
     */
    void sendFileChange(const common::FileChange &fileChange) const;
};
} // namespace localMonitor
