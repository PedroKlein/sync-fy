
#include "serverMessageHandler.hpp"
#include "userConnection.hpp"
#include <filesystem/directory.hpp>
#include <memory>
#include <string>

namespace localMonitor
{
class LocalMonitor
{
  public:
    LocalMonitor(ServerMessageHandler &messageHandler, std::shared_ptr<FileChangesQueue> changeQueue);

    void monitorChanges();
    void stopMonitoring();

  private:
    ServerMessageHandler &messageHandler;
    std::shared_ptr<FileChangesQueue> changeQueue;
    const common::Directory directory;
    bool isMonitoring;

    void initialSync();
    void sendFileChange(const common::FileChange &fileChange) const;
};
} // namespace localMonitor
