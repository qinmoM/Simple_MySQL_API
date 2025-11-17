#include "MysqlConn.h"

const char* ip = "localhost";
// const char* ip = "127.0.0.1";
const char* user = "root";
const char* pw = "123456";
const char* databaseName = "test";
const int port = 3306;

int main()
{
    MysqlConn con;
    if (!con.connect(user, pw, databaseName, ip, port))
    {
        std::cerr << "Failed to connect." << std::endl;
    }

    if (con.query(R"(select * from table1;)", "../data/test1.json"))
    {
        std::cout << "File created successful." << std::endl;
    }

    // MYSQL* con = mysql_init(nullptr);
    // // mysql_set_character_set(con, "utf8");
    // mysql_options(con, MYSQL_SET_CHARSET_NAME, "utf8");
    //
    // if (!mysql_real_connect(con, ip, user, pw, databaseName, port, nullptr, 0))
    // {
    //     std::cerr << "Failed to connect to database." << std::endl;
    //     return -1;
    // }
    //
    // // if (mysql_query(con, R"(insert into table1 values(8, 1, 1000);)"))
    // // {
    // //     std::cerr << "Failed to insert data." << std::endl;
    // //     return -1;
    // // }
    // //
    // // if (mysql_query(con, R"(select * from table1)"))
    // // {
    // //     std::cerr << "Failed to select data." << std::endl;
    // //     return -1;
    // // }
    // // MYSQL_RES* result = mysql_store_result(con);
    // // if (nullptr == result)
    // // {
    // //     std::cerr << "Failed to get result stone." << std::endl;
    // //     return -1;
    // // }
    // // unsigned int count = mysql_num_fields(result);
    // // MYSQL_ROW row = nullptr;
    // // std::vector<std::string> vec;
    // // while (nullptr != (row = mysql_fetch_row(result)))
    // // {
    // //     for (int i = 0; i < count; ++i)
    // //     {
    // //         char* temp = row[i];
    // //         unsigned long length = mysql_fetch_lengths(result)[i];
    // //         vec.push_back(std::string(temp, length));
    // //     }
    // // }
    // // for (const std::string& s : vec)
    // // {
    // //     std::cout << s << std::endl;
    // // }
    //
    // if (mysql_query(con, R"(select * from table1)"))
    // {
    //     std::cerr << "Failed to select data." << std::endl;
    //     return -1;
    // }
    // MYSQL_RES* result = mysql_store_result(con);
    // if (nullptr == result)
    // {
    //     std::cerr << "Failed to get result stone." << std::endl;
    //     return -1;
    // }
    // unsigned int count = mysql_num_fields(result);
    // std::vector<std::string> columnName;
    // for (unsigned int i = 0; i < count; ++i)
    // {
    //     MYSQL_FIELD* field = mysql_fetch_field_direct(result, i);
    //     columnName.push_back(std::string(field->name, field->name_length));
    // }
    // for (const std::string& s : columnName)
    // {
    //     std::cout << s << std::endl;
    // }
    // MYSQL_ROW row = nullptr;
    // nlohmann::ordered_json jsonArray = nlohmann::ordered_json::array();
    // // std::vector<std::string> vec;
    // while (nullptr != (row = mysql_fetch_row(result)))
    // {
    //     nlohmann::ordered_json tempJson;
    //     for (int i = 0; i < count; ++i)
    //     {
    //         char* temp = row[i];
    //         unsigned long length = mysql_fetch_lengths(result)[i];
    //         tempJson[columnName[i]] = std::string(temp, length);
    //     }
    //     jsonArray.push_back(tempJson);
    // }
    // for (int i = 0; i < jsonArray.size(); ++i)
    // {
    //     std::cout << jsonArray[i] << std::endl;
    // }
    //
    // mysql_free_result(result);
    // mysql_close(con);
    //
    // jsonFile file("../data/test.json");
    // file.write(jsonArray);

    return 0;
}