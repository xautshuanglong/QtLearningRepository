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
    return MiscellaneousTestItem::Sqlite_wxSqlite3;
}

void MiscellaneousWxSqlite3::on_btnEncryptPassword_clicked()
{
    wxString dbFileName(wxS("E:\\VS2015\\wxWidgets\\wxsqlite3-4.4.8\\samples\\NavicatSqlite3.db"));
    wxString dbKey(wxS("test"));
    wxSQLite3CipherAes128 sqlCipher;
    sqlCipher.InitializeFromGlobalDefault();

    try
    {
        wxSQLite3Database db;
        db.Open(dbFileName, sqlCipher, dbKey);
        wxString keySalt = db.GetKeySalt();
        //cout << "Cipher salt: " << (const char*)(keySalt.mb_str()) << endl;
        LogUtil::Debug(CODE_LOCATION, "Cipher salt: %s", keySalt.mb_str());
        int numRows = db.ExecuteScalar("SELECT COUNT(*) FROM t1");
        LogUtil::Debug(CODE_LOCATION, "Total number of rows = %d", numRows);
        LogUtil::Debug(CODE_LOCATION, "Distinct tuples:");
        wxSQLite3ResultSet set = db.ExecuteQuery("SELECT DISTINCT * FROM t1");
        int count = 0;
        while (set.NextRow())
        {
            ++count;
            wxString col1 = set.GetString(0);
            wxString col2 = set.GetString(1);
            LogUtil::Debug(CODE_LOCATION, "%d: %s, %s", count, (const char*)(col1.mb_str()), (const char*)(col2.mb_str()));
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
