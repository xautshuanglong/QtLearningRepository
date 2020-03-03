#include "MiscellaneousSqlCipher.h"

#define TEST_FLAG_SQL_CIPHER 1
#if TEST_FLAG_SQL_CIPHER
#pragma comment(lib, "sqlite3.lib")
#endif

#define SQLITE_HAS_CODEC
#include <sqlcipher/sqlite3.h>

#include <LogUtil.h>

MiscellaneousSqlCipher::MiscellaneousSqlCipher(QWidget *parent)
    : MiscellaneousBase(parent)
    , mShowHeaderFlag(false)
{
    ui.setupUi(this);
}

MiscellaneousSqlCipher::~MiscellaneousSqlCipher()
{
}

QString MiscellaneousSqlCipher::GetTitle()
{
    return QObject::tr("SqlCipher");
}

QString MiscellaneousSqlCipher::GetTitleTooltip()
{
    return QObject::tr("How to use SqlCipher for secure sqlite.");
}

MiscellaneousTestGroup MiscellaneousSqlCipher::GetGroupID()
{
    return MiscellaneousTestGroup::Database;
}

MiscellaneousTestItem MiscellaneousSqlCipher::GetItemID()
{
    return MiscellaneousTestItem::Database_Sqlite_SqlCipher;
}

void MiscellaneousSqlCipher::on_btnInsert_clicked()
{
    int i = 0;
}

void MiscellaneousSqlCipher::on_btnDelete_clicked()
{
    int i = 0;
}

void MiscellaneousSqlCipher::on_btnUpdate_clicked()
{
    int i = 0;
}

void MiscellaneousSqlCipher::on_btnSelect_clicked()
{
#if TEST_FLAG_SQL_CIPHER
    const char* unencryptedDataPath = "E:\\Temp\\sqlite3\\NoEncrypted_Test123.db"; // 未加密
    sqlite3 *pDB1 = NULL;
    //int result = sqlite3_open_v2(path, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE, NULL);
    int result = sqlite3_open_v2(unencryptedDataPath, &pDB1, SQLITE_OPEN_READONLY | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE, NULL);
    if (result == SQLITE_OK)
        LogUtil::Info(CODE_LOCATION, "Open unencrypted database successfully: %s", unencryptedDataPath);
    else
        LogUtil::Error(CODE_LOCATION, "Open unencrypted database failed: %s", unencryptedDataPath);

    if (result == SQLITE_OK)
    {
        char *errmsg1 = NULL;
        mShowHeaderFlag = true;
        result = sqlite3_exec(pDB1, "SELECT * FROM user;", MiscellaneousSqlCipher::Sqlite3_Callback, this, &errmsg1);
        if (result == SQLITE_OK)
        {
            LogUtil::Info(CODE_LOCATION, "Execut select successfully.");
        }
        else
        {
            LogUtil::Error(CODE_LOCATION, "Execut select failed. ErrMsg: %s", errmsg1);
        }
    }

    sqlite3_close(pDB1);

    //----------------------------------------------------------------------------------------------------

    sqlite3 *pDB2 = NULL;
    const char* encryptedDataPath = "E:\\Temp\\sqlite3\\HaveEncrypted_Test123.db"; // 已加密
    const char* pwd = "Test123";
    result = sqlite3_open(encryptedDataPath, &pDB2);
    if (result == SQLITE_OK)
        LogUtil::Info(CODE_LOCATION, "Open encrypted database successfully: %s", encryptedDataPath);
    else
        LogUtil::Error(CODE_LOCATION, "Open encrypted database failed: %s", encryptedDataPath);
    result = sqlite3_key(pDB2, pwd, strnlen(pwd, 1000));
    LogUtil::Debug(CODE_LOCATION, "Open encrypted database with password: %s result=%d", pwd, result);

    //result = sqlite3_exec(db, "CREATE TABLE [t1](id integer PRIMARY KEY AUTOINCREMENT UNIQUE, name varchar);", NULL, NULL, &errmsg);
    //if (result == SQLITE_OK)
    //{
    //    cout << "SQL 语句执行成功" << endl;
    //}
    //else
    //{
    //    cout << "SQL 语句执行时发生错误：" << sqlite3_errmsg(db) << endl;
    //}

    //result = sqlite3_exec(db, "INSERT INTO t1(name) VALUES('张三');", NULL, NULL, &errmsg);
    //if (result == SQLITE_OK)
    //{
    //    cout << "SQL 语句执行成功" << endl;
    //}
    //else
    //{
    //    cout << "SQL 语句执行时发生错误：" << sqlite3_errmsg(db) << endl;
    //}

    if (result == SQLITE_OK)
    {
        char *errmsg2 = NULL;
        mShowHeaderFlag = true;
        result = sqlite3_exec(pDB2, "SELECT * FROM user;", MiscellaneousSqlCipher::Sqlite3_Callback, this, &errmsg2);
        if (result == SQLITE_OK)
        {
            LogUtil::Info(CODE_LOCATION, "Execut select successfully.");
        }
        else
        {
            LogUtil::Error(CODE_LOCATION, "Execut select failed. ErrMsg: %s", errmsg2);
        }
    }

    sqlite3_close(pDB2);
#else
    LogUtil::Warn(CODE_LOCATION, "Define TEST_FLAG_SQL_CIPHER not zero to enable sqlcipher ...");
#endif
}

int MiscellaneousSqlCipher::Sqlite3_Callback(void *notUsed, int argc, char **argv, char **azColName)
{
    MiscellaneousSqlCipher *pThis = (MiscellaneousSqlCipher*)notUsed;
    const int BUFFER_LEN = 512;
    if (pThis!=NULL && pThis->mShowHeaderFlag)
    {
        pThis->mShowHeaderFlag = false;
        char rowHeader[BUFFER_LEN] = { 0 };
        for (int i = 0; i < argc; i++)
        {
            if (i * 20 > 512)
            {
                LogUtil::Info(CODE_LOCATION, "RowHeader buffer is too small ...");
                break;
            }
            snprintf(rowHeader + i * 20, BUFFER_LEN, "%20s", azColName[i]);
        }
        LogUtil::Info(CODE_LOCATION, "--------------------------------------------------------------------------------");
        LogUtil::Info(CODE_LOCATION, "%s", rowHeader);
        LogUtil::Info(CODE_LOCATION, "--------------------------------------------------------------------------------");
    }
    char rowContent[512] = { 0 };
    for (int i = 0; i < argc; i++)
    {
        snprintf(rowContent+ i * 20, BUFFER_LEN, "%20s", argv[i] ? argv[i] : "NULL");
    }
    LogUtil::Info(CODE_LOCATION, "%s", rowContent);
    return 0;
}
