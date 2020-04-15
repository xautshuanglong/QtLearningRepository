#include "MiscellaneousExcelReadWrite.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "LogUtil.h"

MiscellaneousExcelReadWrite::MiscellaneousExcelReadWrite(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousExcelReadWrite::~MiscellaneousExcelReadWrite()
{
}

QString MiscellaneousExcelReadWrite::GetTitle()
{
    return QObject::tr("Excel Read Write");
}

QString MiscellaneousExcelReadWrite::GetTitleTooltip()
{
    return QObject::tr("How to read and write excel by QT.");
}

MiscellaneousTestGroup MiscellaneousExcelReadWrite::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousExcelReadWrite::GetItemID()
{
    return MiscellaneousTestItem::QT_Excel_ReadWrite;
}

void MiscellaneousExcelReadWrite::on_btnSqlSelect_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "xlsx_connection");
    if (db.isValid())
    {
        db.setDatabaseName("DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=" + QString("E:\\Temp\\QT_SQL_DATABASE_1.xlsx"));
        if (db.open())
        {
            QSqlQuery query("select * from [" + QString("Sheet1") + "$]", db);
            //QSqlQuery query("select * from [" + QStringLiteral("中文") + "$]", db);
            QString executedQueryStr = query.executedQuery();
            QSqlError queryError = query.lastError();
            QSqlError::ErrorType errorType = queryError.type();
            QString errorText = errorType == QSqlError::NoError ? QString("No Error.") : queryError.text();
            LogUtil::Info(CODE_LOCATION, "Executed query string: %s, with error [%d] %s",
                          executedQueryStr.toUtf8().data(), errorType, errorText.toUtf8().data());

            int id = 0, age = 0;
            QString name;

            while (query.next())
            {
                id = query.value("id").toInt();
                name = query.value("name").toString();
                age = query.value("age").toInt();
                LogUtil::Info(CODE_LOCATION, "ID=%-5d AGE=%-5d NAME=%-10s", id, age, name.toUtf8().data());
            }
            query.clear();
            db.close();
        }
        else
        {
            LogUtil::Error(CODE_LOCATION, "The SQL Database open failed .");
        }

        db.setDatabaseName("DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};DBQ=" + QString("E:\\Temp\\QT_SQL_DATABASE_1.xls"));
        if (db.open())
        {
            //QSqlQuery query("select * from [" + QString("Sheet1") + "$]", db);
            QSqlQuery query("select * from [" + QStringLiteral("中文") + "$]", db);
            QString executedQueryStr = query.executedQuery();
            QSqlError queryError = query.lastError();
            QSqlError::ErrorType errorType = queryError.type();
            QString errorText = errorType == QSqlError::NoError ? QString("No Error.") : queryError.text();
            LogUtil::Info(CODE_LOCATION, "Executed query string: %s, with error [%d] %s",
                          executedQueryStr.toUtf8().data(), errorType, errorText.toUtf8().data());

            int id = 0, age = 0;
            QString name;

            while (query.next())
            {
                id = query.value("id").toInt();
                name = query.value("name").toString();
                age = query.value("age").toInt();
                LogUtil::Info(CODE_LOCATION, "ID=%-5d AGE=%-5d NAME=%-10s", id, age, name.toUtf8().data());
            }
            query.clear();
            db.close();
        }
        else
        {
            LogUtil::Error(CODE_LOCATION, "The SQL Database open failed .");
        }
    }
    else
    {
        LogUtil::Error(CODE_LOCATION, "The SQL Database is not valid .");
    }

    QSqlDatabase::removeDatabase("xlsx_connection");
}

void MiscellaneousExcelReadWrite::on_btnSqlUpdateAndInsert_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "xlsx_connection");
    if (db.isValid())
    {
        db.setDatabaseName("DRIVER={Microsoft Excel Driver (*.xls, *.xlsx, *.xlsm, *.xlsb)};READONLY=FALSE;DBQ=" + QString("E:\\Temp\\QT_SQL_DATABASE_1.xlsx"));
        if (db.open())
        {
            QSqlQuery query("update [" + QString("Sheet1") + "$] set name='UpdateTest' where id=5", db);
            QString executedQueryStr = query.executedQuery();
            QSqlError queryError = query.lastError();
            QSqlError::ErrorType errorType = queryError.type();
            QString errorText = errorType == QSqlError::NoError ? QString("No Error.") : queryError.text();
            LogUtil::Info(CODE_LOCATION, "Executed query string: %s, with error [%d] %s",
                          executedQueryStr.toUtf8().data(), errorType, errorText.toUtf8().data());

            int id = 0, age = 0;
            QString name;

            while (query.next())
            {
                id = query.value("id").toInt();
                name = query.value("name").toString();
                age = query.value("age").toInt();
                LogUtil::Info(CODE_LOCATION, "ID=%-5d AGE=%-5d NAME=%-10s", id, age, name.toUtf8().data());
            }
            query.clear();

            QSqlQuery queryInsert(db);
            queryInsert.exec("insert into [" + QString("Sheet1") + "$] (id, name, age) values (7, 'InsertTest', 700)");
            executedQueryStr = queryInsert.executedQuery();
            queryError = queryInsert.lastError();
            errorType = queryError.type();
            errorText = errorType == QSqlError::NoError ? QString("No Error.") : queryError.text();
            LogUtil::Info(CODE_LOCATION, "Executed query string: %s, with error [%d] %s",
                          executedQueryStr.toUtf8().data(), errorType, errorText.toUtf8().data());
            queryInsert.clear();

            db.close();
        }
        else
        {
            LogUtil::Error(CODE_LOCATION, "The SQL Database open failed .");
        }
    }
    else
    {
        LogUtil::Error(CODE_LOCATION, "The SQL Database is not valid .");
    }

    QSqlDatabase::removeDatabase("xlsx_connection");
}

void MiscellaneousExcelReadWrite::on_btnAxObjectRead_clicked()
{
    int i = 0;
}

void MiscellaneousExcelReadWrite::on_btnAxObjectWrite_clicked()
{
    int i = 0;
}
