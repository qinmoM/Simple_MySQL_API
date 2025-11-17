#pragma once

#include "jsonFile.hpp"
#include "mysql.h"
#include <string>
#include <vector>
#include <memory>

class MysqlConn
{
public:
    struct Table
    {
        Table(const std::vector<std::string>& colName)
            : colName(colName)
        {
            value = nlohmann::ordered_json(nlohmann::ordered_json::array());
        }

        std::vector<std::string> colName;
        nlohmann::ordered_json value;
    };

    struct MysqlResDeleter
    {
        void operator()(MYSQL_RES* res) const
        {
            if (res) mysql_free_result(res);
        }
    };

public:
    MysqlConn();
    ~MysqlConn();

public:
    bool connect(const std::string& user, const std::string& password, const std::string& database, const std::string& ip = "localhost", unsigned int port = 3306);

    bool update(const std::string& sql);

    bool query(const std::string& sql, const std::string& filePath);

    std::unique_ptr<Table> query(const std::string& sql);

    bool transaction();

    bool commit();

    bool rollback();

private:
    MYSQL* con_ = nullptr;

};

MysqlConn::MysqlConn()
{
    con_ = mysql_init(nullptr);
    mysql_set_character_set(con_, "utf8");
}

MysqlConn::~MysqlConn()
{
    mysql_close(con_);
}

bool MysqlConn::connect(const std::string& user, const std::string& password, const std::string& database, const std::string& ip, unsigned int port)
{
    if (!mysql_real_connect(con_, ip.c_str(), user.c_str(), password.c_str(), database.c_str(), port, nullptr, 0))
    {
        return false;
    }
    return true;
}

bool MysqlConn::update(const std::string& sql)
{
    if (mysql_query(con_, sql.c_str()))
    {
        return false;
    }
    return true;
}

bool MysqlConn::query(const std::string& sql, const std::string& filePath)
{
    std::unique_ptr<MysqlConn::Table> table = query(sql);
    if (!table)
    {
        return false;
    }
    jsonFile file(filePath);
    return file.write(table->value);
}

std::unique_ptr<MysqlConn::Table> MysqlConn::query(const std::string& sql)
{
    if (!con_ || mysql_query(con_, sql.c_str()))
    {
        return nullptr;
    }

    std::unique_ptr<MYSQL_RES, MysqlResDeleter> result(mysql_store_result(con_));
    std::vector<std::string> colName;
    if (0 == mysql_field_count(con_))
    {
        return std::make_unique<MysqlConn::Table>(colName);
    }
    if (!result)
    {
        if (mysql_field_count(con_))
        {
            std::cerr << "store == nullptr and field > 0\n" << mysql_errno(con_) << " : " << mysql_error(con_) << std::endl;
        }
        return nullptr;
    }

    unsigned int count = mysql_num_fields(result.get());
    for (int i = 0; i < count; ++i)
    {
        MYSQL_FIELD* field = mysql_fetch_field_direct(result.get(), i);
        colName.push_back(std::string(field->name, field->name_length));
    }

    std::unique_ptr<MysqlConn::Table> table = std::make_unique<MysqlConn::Table>(colName);
    MYSQL_ROW row = nullptr;
    while ((row = mysql_fetch_row(result.get())) != nullptr)
    {
        nlohmann::ordered_json jsonTemp = nlohmann::ordered_json::object();
        unsigned long* lengthArray = mysql_fetch_lengths(result.get());
        for (int i = 0; i < count; ++i)
        {
            if (row[i])
            {
                jsonTemp[colName[i]] = std::string(row[i], lengthArray[i]);
            }
            else
            {
                jsonTemp[colName[i]] = nullptr;
            }
        }
        table->value.push_back(jsonTemp);
    }

    return table;
}

bool MysqlConn::transaction()
{
    return mysql_autocommit(con_, false);
}

bool MysqlConn::commit()
{
    return mysql_commit(con_);
}

bool MysqlConn::rollback()
{
    return mysql_rollback(con_);
}
