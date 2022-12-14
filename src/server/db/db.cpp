#include "db.h"
#include <muduo/base/Logging.h>

// 数据库配置信息
static string server = "127.0.0.1";
static string user = "root";
static string password = "123456";
static string dbname = "chat";

MySQL::MySQL()
{
    conn_ = mysql_init(nullptr);
}

MySQL::~MySQL()
{
    if (conn_ != nullptr)
        mysql_close(conn_);
}

bool MySQL::connect()
{
    MYSQL *p = mysql_real_connect(conn_, server.c_str(), user.c_str(),
                                  password.c_str(), dbname.c_str(), 3306, nullptr, 0);
    if (p != nullptr)
    {
        // C/C++ 代码默认编码字符是 ASCII，如果不设置，从MySQL上拉下来的中文会显示 ？
        mysql_query(conn_, "set names gbk");
        LOG_INFO << "connect mysql success!";
    }
    else
    {
        LOG_INFO << "connect mysql fail!";
    }
    return p;
}

bool MySQL::update(string sql)
{
    if (mysql_query(conn_, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
                 << sql << "更新失败!";
        return false;
    }
    return true;
}

MYSQL_RES *MySQL::query(string sql)
{
    if (mysql_query(conn_, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
                 << sql << "查询失败!";
        return nullptr;
    }
    return mysql_use_result(conn_);
}

MYSQL* MySQL::getConnection()
{
    return conn_;
}