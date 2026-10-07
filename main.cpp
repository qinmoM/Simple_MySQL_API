#include "MysqlConn.h"
#include <iostream>

const char* ip = "localhost"; // const char* ip = "127.0.0.1";
const char* user = "root";
const char* pw = "123456";
const char* databaseName = "test";
const int port = 3306;


template <class T>
using optional = qinmo::Optional<T>;
using MysqlConnect = qinmo::MysqlConn;



int main()
{
    MysqlConnect conn;
    conn.connect(user, pw, databaseName, ip, port);
    conn.execute(R"(select * from table1;)");
    {
        auto res = conn.getResult();

        while (res->next())
        {
            optional<std::string> id = res->getString();
            optional<std::string> gender = res->getString();
            optional<std::string> salary = res->getString();

            qinmo::print("id ", *id, ": ");
            gender
                .or_else([](){
                    qinmo::print("unknown");
                    return optional<std::string>();
                })
                .transform([](const std::string& str){
                    qinmo::print("1" == str ? "male" : "female");
                    return str;
                });
            qinmo::println(", salary: ", *salary);
        }
    }

    if (conn.execute(R"(select * from table1;)"))
        qinmo::println("success.");

    conn.getResult();


    return 0;
}