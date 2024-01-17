

class BackupConnection
{
  public:
    BackupConnection(const std::string &ip, int serverId) : ip_(ip), id_(getNextId(serverId))
    {
    }

    std::string getIP() const
    {
        return ip_;
    }

    int getID() const
    {
        return id_;
    }

  private:
    std::string ip_;
    int id_;

    static int getNextId(int serverId)
    {
        static int nextId = 0;
        return serverId + (nextId++);
    }
};
