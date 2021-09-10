#include "MiscellaneousPostgresql.h"

#include <iostream>
#include <iomanip>
#include <sstream>

#include <QDateTime>

// libpq/libpqxx Headers
/************************************************************************/
/* postgresql ���ݿ���ز�������                                          */
/*     libpq   c   ���Խӿ�                                              */
/*     libpqxx c++ ���Խӿ�                                              */
/************************************************************************/
//#include <libpq-fe.h>
//#include <pqxx/pqxx>
//
//template <> QDateTime pqxx::field::as<QDateTime>() const
//{
//    //QString timeString2("2019-07-08 17:52:02.987"); // 2020-02-17 17:19:52.122236+08
//    //QDateTime timeTest2 = QDateTime::fromString(timeString2, "yyyy-MM-dd hh:mm:ss.zzz");
//    return QDateTime::fromString(QString(this->c_str()).left(23), "yyyy-MM-dd hh:mm:ss.z");
//}

#include "JCB_Logger/LogUtil.h"

MiscellaneousPostgresql::MiscellaneousPostgresql(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousPostgresql::~MiscellaneousPostgresql()
{
}

QString MiscellaneousPostgresql::GetTitle()
{
    return QObject::tr("Postgresql");
}

QString MiscellaneousPostgresql::GetTitleTooltip()
{
    return QObject::tr("How to use libpq/libpqxx for postgresql.");
}

MiscellaneousTestGroup MiscellaneousPostgresql::GetGroupID()
{
    return MiscellaneousTestGroup::Database;
}

MiscellaneousTestItem MiscellaneousPostgresql::GetItemID()
{
    return MiscellaneousTestItem::Database_Postgresql;
}

void MiscellaneousPostgresql::on_btnLibpqInsert_clicked()
{
    int i = 0;
}

void MiscellaneousPostgresql::on_btnLibpqDelete_clicked()
{
    int i = 0;
}

void MiscellaneousPostgresql::on_btnLibpqUpdate_clicked()
{
    int i = 0;
}

void MiscellaneousPostgresql::on_btnLibpqSelect_clicked()
{
    //PGconn     *conn;
    //PGresult   *res;
    //int         nFields;
    //int         i, j;

    ///* �������ݿ� */
    //conn = PQconnectdb("dbname=test_db user=postgres password=shuanglong hostaddr=127.0.0.1 port=5432");

    ///* ��������ӳɹ����� */
    //if (PQstatus(conn) != CONNECTION_OK)
    //{
    //    LogUtil::Error(CODE_LOCATION, "Connection to database failed: %s",
    //            PQerrorMessage(conn));
    //    PQfinish(conn);
    //    return;
    //}

    ///*
    // * ���ǵĲ���ʵ���漰�α��ʹ�ã����ʱ�����Ǳ���ʹ������顣
    // * ���ǿ��԰�ȫ���������һ��  "select * from pg_database"
    // * PQexec() ���������̫���ˣ����Ǹ������ӡ�
    // */

    // /* ��ʼһ������� */
    //res = PQexec(conn, "BEGIN");
    //if (PQresultStatus(res) != PGRES_COMMAND_OK)
    //{
    //    LogUtil::Error(CODE_LOCATION, "BEGIN command failed: %s", PQerrorMessage(conn));
    //    PQclear(res);
    //    PQfinish(conn);
    //    return;
    //}

    ///*
    // * Ӧ���ڽ������Ҫ��ʱ�� PQclear PGresult���Ա����ڴ�й©
    // */
    //PQclear(res);

    ///*
    // * ��ϵͳ�� pg_database�����ݿ��ϵͳĿ¼����ץȡ����
    // */
    //res = PQexec(conn, "DECLARE myportal CURSOR FOR select * from public.\"Users\"");
    //if (PQresultStatus(res) != PGRES_COMMAND_OK)
    //{
    //    LogUtil::Error(CODE_LOCATION, "DECLARE CURSOR failed: %s", PQerrorMessage(conn));
    //    PQclear(res);
    //    PQfinish(conn);
    //    return;
    //}
    //PQclear(res);

    //res = PQexec(conn, "FETCH ALL in myportal");
    //if (PQresultStatus(res) != PGRES_TUPLES_OK)
    //{
    //    LogUtil::Error(CODE_LOCATION, "FETCH ALL failed: %s", PQerrorMessage(conn));
    //    PQclear(res);
    //    PQfinish(conn);
    //    return;
    //}

    ///* ���ȣ���ӡ�������� */
    //std::stringstream oss;
    //oss.str("");
    //nFields = PQnfields(res);
    //for (i = 0; i < nFields; i++)
    //{
    //    oss << std::setw(i < 3 ? 15 : 30) << std::left << PQfname(res, i);
    //}
    //LogUtil::Info(CODE_LOCATION, "---------------------------------------------------------------------------");
    //LogUtil::Info(CODE_LOCATION, "%s", oss.str().c_str());

    ///* Ȼ���ӡ�� */
    //for (i = 0; i < PQntuples(res); i++)
    //{
    //    oss.str("");
    //    for (j = 0; j < nFields; j++)
    //    {
    //        oss << std::setw(j < 3 ? 15 : 30) << std::left << PQgetvalue(res, i, j);
    //    }
    //    LogUtil::Info(CODE_LOCATION, "%s", oss.str().c_str());
    //}
    //LogUtil::Info(CODE_LOCATION, "---------------------------------------------------------------------------");

    //PQclear(res);

    ///* �ر���� ... ���ǲ��ü����� ... */
    //res = PQexec(conn, "CLOSE myportal");
    //PQclear(res);

    ///* �������� */
    //res = PQexec(conn, "END");
    //PQclear(res);

    ///* �ر����ݿ����Ӳ����� */
    //PQfinish(conn);
}

void MiscellaneousPostgresql::on_btnLibpqxxInsert_clicked()
{
    //try
    //{
    //    // insert into public."Users" (name, passwd, reg_time) values ('zhangsan', 'zhangtest', current_timestamp);
    //    qint64 currentSecond = QDateTime::currentMSecsSinceEpoch();
    //    QString username = QString("user_%1").arg(currentSecond);
    //    QString password = QString("passwd_%1").arg(currentSecond);
    //    QString insertSatence = QString("insert into public.\"Users\" (name, passwd, reg_time) values ('%1', '%2', current_timestamp)").arg(username).arg(password);

    //    pqxx::connection conn("dbname=test_db user=postgres password=shuanglong hostaddr=127.0.0.1 port=5432");
    //    LogUtil::Info(CODE_LOCATION, "Connected to : %s", conn.dbname());
    //    pqxx::work work(conn);

    //    pqxx::result res = work.exec(insertSatence.toUtf8().data());
    //    work.commit();
    //    LogUtil::Info(CODE_LOCATION, "OK.");
    //}
    //catch (const std::exception &e)
    //{
    //    LogUtil::Error(CODE_LOCATION, "pqxx error: %s", e.what());
    //}
}

void MiscellaneousPostgresql::on_btnLibpqxxDelete_clicked()
{
    int i = 0;
}

void MiscellaneousPostgresql::on_btnLibpqxxUpdate_clicked()
{
    int i = 0;
}

void MiscellaneousPostgresql::on_btnLibpqxxSelect_clicked()
{
    //try
    //{
    //    pqxx::connection conn("dbname=test_db user=postgres password=shuanglong hostaddr=127.0.0.1 port=5432");
    //    LogUtil::Info(CODE_LOCATION, "Connected to : %s", conn.dbname());
    //    pqxx::work work(conn);

    //    pqxx::result res = work.exec("select * from public.\"Users\"");
    //    LogUtil::Info(CODE_LOCATION, "Found %d users:", res.size());
    //    for (pqxx::row user : res)
    //    {
    //        LogUtil::Info(CODE_LOCATION, "%s %s %s %s",
    //                      user[0].c_str(),
    //                      user[1].c_str(),
    //                      user[2].c_str(),
    //                      user[3].c_str());
    //        QDateTime reg_time = user[3].as<QDateTime>();
    //        LogUtil::Info(CODE_LOCATION, "QDateTime: %s", reg_time.toString(Qt::ISODateWithMs).toUtf8().data());

    //        QDateTime reg_time_at = user.at("reg_time").as<QDateTime>();
    //        LogUtil::Info(CODE_LOCATION, "Row at QDateTime: %s", reg_time.toString(Qt::ISODateWithMs).toUtf8().data());
    //    }

    //    work.commit();
    //    std::cout << "OK." << std::endl;
    //}
    //catch (pqxx::sql_error const &e)
    //{
    //    LogUtil::Error(CODE_LOCATION, "SQL error: %s", e.what());
    //    LogUtil::Error(CODE_LOCATION, "Query was: %s", e.query().c_str());
    //}
    //catch (const std::exception &e)
    //{
    //    LogUtil::Error(CODE_LOCATION, "pqxx error: %s", e.what());
    //}
}
