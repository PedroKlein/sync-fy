#pragma once

#include <sqlite_orm/sqlite_orm.h>
#include "./models/User.h"

#define DATABASE_PATH "database.db"

inline auto initStorage()
{
    using namespace sqlite_orm;
    return make_storage(DATABASE_PATH,
                        make_table("users",
                                   make_column("id", &User::id, autoincrement(), primary_key()),
                                   make_column("username", &User::username),
                                   make_column("created_at", &User::created_at)));
}

using Storage = decltype(initStorage());

extern std::unique_ptr<Storage> storage;

class Database
{
private:
    Storage db; // Separate connection for each thread

public:
    ~Database() {}

    void insertUser(const User &user)
    {
        db.insert(user);
    }
};
