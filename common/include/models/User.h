#pragma once

#include <string>
#include "datetime/DateTime.h"

struct User
{
    int id;
    std::string username;
    std::string created_at;

    User() = default;

    User(const std::string &username)
        : username(username), created_at(DateTime::getCurrentDateTime()) {}

    User(const std::string &username, const std::string &creation_date)
        : username(username), created_at(creation_date) {}
};
