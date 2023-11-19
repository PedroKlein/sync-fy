#include <iostream>
#include <thread>

#include "database/Database.h"

#ifdef __linux__
#include <pthread.h>
#endif

int main()
{
    try
    {
        SQLiteDB db("example.db");
        db.createTable();

        const int threadCount = 3;
        const int recordCount = 5;

        std::vector<std::thread> threads;

        // Thread 1: Insert records
        threads.emplace_back([&db, recordCount]() {
            std::cout << "Thread1_Record" << std::endl;
            db.insertRecord("Thread1_Record", 100);
        });

        // Thread 2: Update records
        threads.emplace_back([&db, recordCount]() {
            std::cout << "Thread2_Updated" << std::endl;
            db.updateRecord(1, "Thread2_Updated", 200);
        });

        // Thread 3: Delete records
        threads.emplace_back([&db, recordCount]() {
            std::cout << "Thread3_Deleted" << std::endl;
            db.deleteRecord(1);
        });

        std::cout << "\nAAAA:\n" << std::endl;

        // Wait for all threads to finish
        for (auto &thread : threads)
        {
            thread.join();
        }

        // Display the final state of the records
        std::cout << "\nFinal records:\n" << std::endl;
        db.selectAllRecords();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}