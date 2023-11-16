#include <thread>
#include <vector>
#include "models/User.h"
#include "database/Database.h"

int main()
{
    const int numThreads = 4;

    // Create a vector of threads
    std::vector<std::thread> threads(numThreads);

    for (int i = 0; i < numThreads; ++i)
    {
        threads[i] = std::thread([i]
                                 {
            // Create a new ThreadSafeDatabase instance for this thread
            Database db();
 
            // Insert a new user into the database
            User user("user" + std::to_string(i));
            db.insertUser(user); });
    }
    // Wait for all threads to finish
    for (auto &thread : threads)
    {
        thread.join();
    }

    return 0;
}
