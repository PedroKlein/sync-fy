
#include "serverMessageHandler.hpp"
#include "userConnection.hpp"
#include <filesystem/directory.hpp>
#include <string>

namespace localMonitor
{
class LocalMonitor
{
  public:
    LocalMonitor(ServerMessageHandler &messageHandler, FileChangesQueue &changeQueue);

    void monitorChanges();

  private:
    ServerMessageHandler &messageHandler;
    FileChangesQueue &changeQueue;
    const common::Directory directory;

    void initialSync();
    void sendFileChange(const common::FileChange &fileChange) const;
};
} // namespace localMonitor
