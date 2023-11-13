#include <iostream>
#include <string>
#include <sqlite_orm/sqlite_orm.h>

// Define the Person class
struct Person
{
    int id;
    std::string name;
    int age;
};

// Map the Person class to the SQLite database
using namespace sqlite_orm;

auto initStorage()
{
    return make_storage("example.sqlite",
                        make_table("persons",
                                   make_column("id", &Person::id, autoincrement(), primary_key()),
                                   make_column("name", &Person::name),
                                   make_column("age", &Person::age)));
}

int main()
{
    auto storage = initStorage();

    // Create a table if it doesn't exist
    storage.sync_schema();

    // Insert a person into the database
    auto person = Person{0, "John Doe", 30};
    storage.insert(person);

    // Query all persons in the database
    auto people = storage.get_all<Person>();

    // Display the results
    for (const auto &p : people)
    {
        std::cout << "ID: " << p.id << ", Name: " << p.name << ", Age: " << p.age << std::endl;
    }

    return 0;
}
