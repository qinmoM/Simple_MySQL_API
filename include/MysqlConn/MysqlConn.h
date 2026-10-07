#pragma once

#include "mysql/mysql.h"
#include "qinmo/tool.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>


namespace qinmo
{

class MysqlConn
{
public:
    class Result
    {
    public:
        bool next()
        {
            row_ = mysql_fetch_row(res_);
            lengths_ = mysql_fetch_lengths(res_);
            col_ = 0;
            return nullptr != row_;
        }
        qinmo::Optional<std::string> getString()
        {
            if (nullptr == row_)
                throw std::runtime_error("MysqlConn::Result::getString: row_ does not exist");

            if (nullptr == lengths_)
                throw std::runtime_error("MysqlConn::Result::getString: lengths_ does not exist");

            if (mysql_num_fields(res_) <= col_)
                throw std::runtime_error("MysqlConn::Result::getString: access number out of range");

            qinmo::Optional<std::string> op;
            if (nullptr != row_[col_])
                op.emplace(row_[col_], lengths_[col_]);

            ++col_;
            return op;
        }

        Result(MYSQL_RES* res, bool& hasResult)
            : res_(res)
            , hasResult_(hasResult)
        { }
        ~Result()
        {
            mysql_free_result(res_);
            res_ = nullptr;
            hasResult_ = false;
            row_ = nullptr;
            lengths_ = nullptr;
        }

    private:
        friend class MysqlConn;

        MYSQL_ROW row_ = nullptr;
        unsigned long* lengths_ = nullptr;
        std::size_t col_ = 0;
        MYSQL_RES* res_;
        bool& hasResult_;
    };

public:
    MysqlConn();
    ~MysqlConn();

public:
/*
        init
*/

    bool connect(const std::string& user, const std::string& password, const std::string& database, const std::string& ip = "localhost", unsigned int port = 3306);

/*
        function
*/
    /// @note must call `getResult()` after query(`select ...`)
    bool execute(const std::string& sql);
    /// @return a handle of result, return `nullptr` on failure
    /// @note cannot used after call the next `execute`
    /// @note the lifetime of the result must be shorter than `MysqlConn`
    std::unique_ptr<Result> getResult();

/*
        transaction
*/

    bool transaction();
    bool commit();
    bool rollback();

private:
    MYSQL* conn_ = nullptr;
    bool hasResult_ = false;

};



inline MysqlConn::MysqlConn()
{
    conn_ = mysql_init(nullptr);
}

inline MysqlConn::~MysqlConn()
{
    if (conn_)
    {
        mysql_close(conn_);
        conn_ = nullptr;
    }
}

inline bool MysqlConn::connect(const std::string& user, const std::string& password, const std::string& database, const std::string& ip, unsigned int port)
{
    if (!mysql_real_connect(conn_, ip.c_str(), user.c_str(), password.c_str(), database.c_str(), port, nullptr, 0))
        return false;

    return 0 == mysql_set_character_set(conn_, "utf8mb4");
}

inline bool MysqlConn::execute(const std::string& sql)
{
    if (nullptr == conn_ || hasResult_)
        return false;

    if (mysql_query(conn_, sql.c_str()))
        return false;

    return true;
}

inline std::unique_ptr<MysqlConn::Result> MysqlConn::getResult()
{
    if (nullptr == conn_ || hasResult_)
        return nullptr;

    MYSQL_RES* res = mysql_store_result(conn_);
    if (nullptr == res)
        return nullptr;

    hasResult_ = true;
    return qinmo::make_unique<MysqlConn::Result>(res, hasResult_);
}

inline bool MysqlConn::transaction()
{
    return 0 == mysql_autocommit(conn_, false);
}

inline bool MysqlConn::commit()
{
    return 0 == mysql_commit(conn_);
}

inline bool MysqlConn::rollback()
{
    return 0 == mysql_rollback(conn_);
}

} // namespace qinmo