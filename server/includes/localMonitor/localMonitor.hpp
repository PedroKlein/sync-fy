
#include "serverMessageHandler.hpp"
#include "userConnection.hpp"
#include <string>

class LocalMonitor
{
  public:
    LocalMonitor(ServerMessageHandler &messageHandler, FileChangesQueue &changeQueue);

    void monitorChanges();

  private:
    ServerMessageHandler &messageHandler;
    FileChangesQueue &changeQueue;

    void sendFileChange(const common::FileChange &fileChange) const;
};
