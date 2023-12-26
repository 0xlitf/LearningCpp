// do not use #include "sqlite3.h" when use soci-sqlite3, https://github.com/SOCI/soci/issues/440
#include <iostream>
#include <fstream>
#include "soci/soci.h"
#include "soci/rowset.h"
#include "soci/use.h"
#include "soci/into.h"
#include "soci/empty/soci-empty.h"
#include "soci/sqlite3/soci-sqlite3.h"
//#pragma comment(lib, "sqlite3.lib")

bool test_sqlite3() {
    sqlite_api::sqlite3* db;
    const char* path = "db.sqlite3";
    int rc = sqlite_api::sqlite3_open(path, &db);
    if (rc == SQLITE_OK) {
        std::cout << "open success" << std::endl;
    } else {
        std::cerr << "open failed: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    sqlite_api::sqlite3_close(db);
    return SQLITE_OK;
}

int test_soci() {
    try {
        const std::string databaseFileName = "soci.sqlite3";

        std::ifstream databaseFile(databaseFileName);
        if (!databaseFile) {
            std::cout << "Database file does not exist. Creating it." << std::endl;

            // 创建数据库文件
            std::ofstream createDatabaseFile(databaseFileName);
            if (!createDatabaseFile) {
                std::cerr << "Failed to create database file." << std::endl;
                return 1;
            }
        }

        soci::backend_factory const& backEnd = *soci::factory_sqlite3();
        soci::session sql;
        sql.open(backEnd, databaseFileName);
        std::cout << "db.open(backEnd, \"soci.sqlite3\");" << std::endl;

        // add table
        std::string tableName = "employees";
        int tableCount = 0;
        sql << "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name = :tableName", soci::into(tableCount), soci::use(tableName);

        if (tableCount == 0) {
            std::cout << "Table " << tableName << " does not exist in the database, creating table" << std::endl;
            sql << "CREATE TABLE " << tableName << " (id INT, name TEXT, salary INT)";
        } else {
            std::cout << "Table " << tableName << " exists in the database." << std::endl;
        }

        int id = 1;
        std::string name = "John";
        int salary = 50000;

        // add
        sql << "INSERT INTO employees (id, name, salary) VALUES(:id, :name, :salary)", soci::use(id), soci::use(name), soci::use(salary);

        id = 1 + 1;
        name = "John 1";
        salary = 50001;

        sql << "INSERT INTO employees (id, name, salary) VALUES(:id, :name, :salary)", soci::use(id), soci::use(name), soci::use(salary);

        // delete
        sql << "DELETE FROM employees WHERE id = :id", soci::use(id);

        // update
        int newSalary = 60000;
        sql << "UPDATE employees SET salary = :newSalary WHERE id = :id", soci::use(newSalary), soci::use(id);

        // into
        sql << "SELECT id, name, salary FROM employees", soci::into(id), soci::into(name), soci::into(salary);

        //
        soci::rowset<soci::row> rows = (sql.prepare << "SELECT id, name, salary FROM employees");

        for (soci::rowset<soci::row>::iterator it = rows.begin(); it != rows.end(); ++it) {
            soci::row& row = *it;
            id = row.get<int>(0);
            name = row.get<std::string>(1);
            salary = row.get<int>(2);

            std::cout << "ID: " << id << ", Name: " << name << ", Salary: " << salary << std::endl;
        }

        sql.close();
    } catch (const soci::soci_error& e) {
        // 捕获SOCI异常
        std::cerr << "SOCI Error: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        // 捕获其他标准异常
        std::cerr << "Standard Exception: " << e.what() << std::endl;
    } catch (...) {
        // 捕获其他异常
        std::cerr << "Unknown Exception" << std::endl;
    }

    return 0;
}

struct Person {
    int id;
    std::string name;
    int age;
};

struct PersonTable {
    int id;
    std::string name;
    int age;
};

namespace soci {
    template<> struct type_conversion<Person> {
        typedef values base_type;
        static void from_base(const values& v, indicator ind, Person& person) {
            person.id = v.get<long long>("id");
            person.name = v.get<std::string>("name");
            person.age = v.get<long long>("age");
        }
        static void to_base(const Person& person, values& v, indicator& ind) {
            v.set("id", (long long) person.id);
            v.set("name", person.name);
            v.set("age", (long long) person.age);
        }
    };
}

bool test_soci_insert_struct() {
    try {
        const std::string databaseFileName = "soci_insert_struct.sqlite3";

        std::ifstream databaseFile(databaseFileName);
        if (!databaseFile) {
            std::cout << "Database file does not exist. Creating it." << std::endl;

            // 创建数据库文件
            std::ofstream createDatabaseFile(databaseFileName);
            if (!createDatabaseFile) {
                std::cerr << "Failed to create database file." << std::endl;
                return 1;
            }
        }

        soci::backend_factory const& backEnd = *soci::factory_sqlite3();
        soci::session sql;
        sql.open(backEnd, databaseFileName);
        std::cout << "db.open(backEnd, \"soci.sqlite3\");" << std::endl;

        // add table
        std::string tableName = "people";
        int tableCount = 0;
        sql << "SELECT COUNT(*) FROM sqlite_master WHERE type='table' AND name = :tableName", soci::into(tableCount), soci::use(tableName);

        if (tableCount == 0) {
            std::cout << "Table " << tableName << " does not exist in the database, creating table" << std::endl;
            sql << "CREATE TABLE " << tableName << " (id INT, name TEXT, age INT)";
        } else {
            std::cout << "Table " << tableName << " exists in the database." << std::endl;
        }

        Person person1{.id=1, .name = "John", .age = 30};
        sql << "INSERT INTO people (id, name, age) VALUES (:id, :name, :age)", soci::use(person1);

        Person person2{.id=2, .name = "Ada", .age = 31};
        sql << "INSERT INTO people (id, name, age) VALUES (:id, :name, :age)", soci::use(person2);

        std::vector<Person> people;
        sql << "SELECT id, name, age FROM people", soci::into(people);

//        for (auto& p:people){
//            std::cout << "people vector: " << p.id << ", name: " << p.name << ", age: " << p.age << ", " << std::endl;
//        }

//        soci::rowset<Person> rs = (sql.prepare << "select * from people");
//        for (soci::rowset<Person>::iterator it = rs.begin(); it != rs.end(); ++it)
//        {
//            const Person& p = *it;
//            std::cout << "Person vector: " << p.id << ", name: " << p.name << ", age: " << p.age << ", " << std::endl;
//        }
        soci::rowset<soci::row> rows = (sql.prepare << "SELECT id, name FROM people");

        // 创建一个 vector 来存储结果
        std::vector<std::pair<int, std::string>> result;

        // 使用 range-based for 循环遍历结果并读取数据
        for (const soci::row& row : rows) {
            int id;
            std::string name;
            row.get<int>(0, id);        // 从第 0 列获取整数值
            row.get<std::string>(1, name); // 从第 1 列获取字符串值
            result.push_back(std::make_pair(id, name));

            std::cout << "Person vector: id: " << id << ", name: " << name << std::endl;
        }
    } catch (const soci::soci_error& e) {
        std::cerr << "SOCI Error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Standard Exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown Exception" << std::endl;
        return 1;
    }

    return 0;
}

int main() {
    test_sqlite3();

    // soci::session db1(soci::sqlite3, "db.sqlite");

    test_soci();

    test_soci_insert_struct();

    return 0;
}
