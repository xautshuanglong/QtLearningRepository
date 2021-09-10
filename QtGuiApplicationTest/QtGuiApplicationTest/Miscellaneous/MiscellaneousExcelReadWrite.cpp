#include "MiscellaneousExcelReadWrite.h"

#include <QAxObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "JCB_Logger/LogUtil.h"

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

    QAxObject *pAxObjExcel = new QAxObject();
    pAxObjExcel->setControl("Excel.Application");
    pAxObjExcel->setProperty("Visible", true);
    pAxObjExcel->setProperty("DisplayAlerts", false);
    pAxObjExcel->setProperty("Caption", "Qt Excel");

    QAxObject *pWorkBooks = pAxObjExcel->querySubObject("WorkBooks");
    QAxObject *pWorkBook = pWorkBooks->querySubObject("Open(const QString&)", QString("E:\\Temp\\QT_SQL_DATABASE_1.xlsx"));

    QAxObject *pWorkBookActive = pAxObjExcel->querySubObject("ActiveWorkBook");
    QAxObject *pWorkSheet = pWorkBookActive->querySubObject("Sheets(int)", 1);

    QAxObject *pCell = Q_NULLPTR;
    QAxObject *pFont = Q_NULLPTR;

    pCell = pWorkSheet->querySubObject("Cells(int,int)", 2, 5);
    pCell->setProperty("Value", "2-2");
    pFont = pCell->querySubObject("Font");
    pFont->setProperty("Color", QColor(74, 51, 255));

    pCell = pWorkSheet->querySubObject("Cells(int,int)", 3, 5);
    pCell->setProperty("Value", "3-2");
    pFont = pCell->querySubObject("Font");
    pFont->setProperty("Color", QColor(255, 255, 0));

    pCell = pWorkSheet->querySubObject("Cells(int,int)", 4, 5);
    pCell->setProperty("Value", "4-2");
    pFont = pCell->querySubObject("Font");
    pFont->setProperty("Color", QColor(255, 0, 0));

    pWorkBook->dynamicCall("Save()");

    delete pAxObjExcel;
}

void MiscellaneousExcelReadWrite::on_btnAxObjectWrite_clicked()
{
    int i = 0;
}
