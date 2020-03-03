#include "MiscellaneousWxSqlite3.h"

// wxSqlite3 Headers
/************************************************************************/
/* wxSlite3 基于 wxWidgets 开发                                          */
/* wxSlite3 分用户授权、密码认证两部分：                                   */
/*    前者模拟用户登录，后者用于数据库文件加解密。                          */
/************************************************************************/
#include <wx/wxsqlite3.h>

#include <LogUtil.h>

MiscellaneousWxSqlite3::MiscellaneousWxSqlite3(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousWxSqlite3::~MiscellaneousWxSqlite3()
{

}

QString MiscellaneousWxSqlite3::GetTitle()
{
    return QObject::tr("wxSqlite3");
}

QString MiscellaneousWxSqlite3::GetTitleTooltip()
{
    return QObject::tr("How to use wxSqlite3 for secure sqlite.");
}

MiscellaneousTestGroup MiscellaneousWxSqlite3::GetGroupID()
{
    return MiscellaneousTestGroup::Database;
}

MiscellaneousTestItem MiscellaneousWxSqlite3::GetItemID()
{
    return MiscellaneousTestItem::Database_Sqlite_wxSqlite3;
}

void MiscellaneousWxSqlite3::on_btnEncryptPassword_clicked()
{
    //wxString dbFileName(wxS("E:\\Temp\\sqlite3\\NavicatSqlite3.db"));
    wxString dbFileName(wxS("E:\\Temp\\sqlite3\\HaveEncrypted_Test123.db"));
    wxString dbKey(wxS("Test123"));
    wxSQLite3CipherAes128 sqlCipher; // 与 Navicat 加密方式相同（配合 Navicat 自带 Sqlite3.dll）。
                                     // 与 sqlcipher 兼容性实验失败，使用默认加密算法，组合也不通过

    try
    {
        wxSQLite3Database db;
        db.Open(dbFileName, sqlCipher, dbKey);
        wxString keySalt = db.GetKeySalt();
        //cout << "Cipher salt: " << (const char*)(keySalt.mb_str()) << endl;
        LogUtil::Debug(CODE_LOCATION, "Cipher salt: %s", keySalt.mb_str());

        if (db.IsOpen())
        {
            int i = 0;
        }
        if (db.IsEncrypted())
        {
            int i = 0;
        }

        LogUtil::Debug(CODE_LOCATION, "Version: %s", db.GetVersion().mb_str());

        int numRows = db.ExecuteScalar("SELECT COUNT(*) FROM user");
        LogUtil::Debug(CODE_LOCATION, "Total number of rows = %d", numRows);
        LogUtil::Debug(CODE_LOCATION, "Distinct tuples:");
        wxSQLite3ResultSet set = db.ExecuteQuery("SELECT DISTINCT * FROM user");
        wxString rowString;
        int rowCount = 0;
        int colCount = set.GetColumnCount();

        while (set.NextRow())
        {
            ++rowCount;
            rowString = "";
            for (int colIndex = 0; colIndex < colCount; ++colIndex)
            {
                if (colIndex != 0)
                {
                    rowString.append(", ");
                }
                rowString.append(set.GetString(colIndex));
            }
            LogUtil::Debug(CODE_LOCATION, "%d: %s", rowCount, (const char*)(rowString.mb_str()));
        }
        db.Close();
    }
    catch (wxSQLite3Exception& e)
    {
        LogUtil::Error(CODE_LOCATION, "%d: %s", e.GetErrorCode(), e.GetMessage().mb_str());
    }
}

void MiscellaneousWxSqlite3::on_btnUserAuthorization_clicked()
{
    int i = 0;
}
