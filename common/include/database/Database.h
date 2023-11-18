#include <fstream>
#include <iostream>
#include <mutex>
#include <sqlite3.h>
#include <sstream>
#include <string>
#include <vector>

#define DATABASE_POOL_SIZE 5

class SQLiteDB
{
  public:
    SQLiteDB(const std::string &dbName, int poolSize = DATABASE_POOL_SIZE) : dbName(dbName)
    {
        // Initialize the connection pool
        for (int i = 0; i < poolSize; ++i)
        {
            sqlite3 *db;
            int result = sqlite3_open(dbName.c_str(), &db);
            if (result != SQLITE_OK)
            {
                // Handle error
                throw std::runtime_error("Error opening database: " + std::string(sqlite3_errmsg(db)));
            }
            connectionPool.push_back(db);
        }
    }

    ~SQLiteDB()
    {
        // Close all connections in the pool
        for (sqlite3 *db : connectionPool)
        {
            sqlite3_close(db);
        }
    }

    // Create
    void createTable()
    {
        executeScriptFromFile("create_table.sql");
    }

    void insertRecord(const std::string &name, int age)
    {
        sqlite3 *db = getConnection();
        std::string query = "INSERT INTO records (name, age) VALUES ('" + name + "', " + std::to_string(age) + ");";
        executeQuery(db, query);
        releaseConnection(db);
    }

    // Read
    void selectAllRecords()
    {
        sqlite3 *db = getConnection();

        // SQLite callback function for processing each row
        auto callback = [](void *data, int argc, char **argv, char **colNames) -> int {
            for (int i = 0; i < argc; ++i)
            {
                std::cout << colNames[i] << ": " << (argv[i] ? argv[i] : "NULL") << "\t";
            }
            std::cout << std::endl;
            return 0;
        };

        executeQuery(db, "SELECT * FROM records;", callback);
        releaseConnection(db);
    }

    // Update
    void updateRecord(int id, const std::string &newName, int newAge)
    {
        sqlite3 *db = getConnection();
        std::string query = "UPDATE records SET name = '" + newName + "', age = " + std::to_string(newAge) +
                            " WHERE id = " + std::to_string(id) + ";";
        executeQuery(db, query);
        releaseConnection(db);
    }

    // Delete
    void deleteRecord(int id)
    {
        sqlite3 *db = getConnection();
        std::string query = "DELETE FROM records WHERE id = " + std::to_string(id) + ";";
        executeQuery(db, query);
        releaseConnection(db);
    }

  private:
    std::vector<sqlite3 *> connectionPool;
    std::string dbName;
    std::mutex poolMutex;

    sqlite3 *getConnection()
    {
        std::lock_guard<std::mutex> lock(poolMutex);
        if (!connectionPool.empty())
        {
            sqlite3 *db = connectionPool.back();
            std::cout << db << std::endl;
            connectionPool.pop_back();
            return db;
        }
        else
        {
            throw std::runtime_error("Connection pool exhausted.");
        }
    }

    void releaseConnection(sqlite3 *db)
    {
        std::lock_guard<std::mutex> lock(poolMutex);
        connectionPool.push_back(db);
    }

    void executeQuery(sqlite3 *db, const std::string &query)
    {
        char *errMsg = nullptr;
        int result = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errMsg);

        if (result != SQLITE_OK)
        {
            // Handle error
            throw std::runtime_error("Error executing query: " + std::string(errMsg));
        }
    }

    void executeQuery(sqlite3 *db, const std::string &query, sqlite3_callback callback)
    {
        char *errMsg = nullptr;
        int result = sqlite3_exec(db, query.c_str(), callback, nullptr, &errMsg);

        if (result != SQLITE_OK)
        {
            // Handle error
            throw std::runtime_error("Error executing query: " + std::string(errMsg));
        }
    }

    void executeScriptFromFile(const std::string &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::runtime_error("Error opening file: " + filename);
        }

        std::ostringstream script;
        script << file.rdbuf();
        file.close();

        sqlite3 *db = getConnection();
        executeQuery(db, script.str());
        releaseConnection(db);
    }
};