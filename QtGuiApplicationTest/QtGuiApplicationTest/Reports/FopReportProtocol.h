#pragma once

#include <QString>
#include <QTcpSocket>

class FopReportProtocol
{
public:
    ~FopReportProtocol();

    static FopReportProtocol* GetInstance();

    void ReadMessage(QTcpSocket& socket);
    QByteArray PackMessageCommand(const QString& cmd, const QString& xmlFile, const QString& xslFile, const QString& outFile);

private:
    FopReportProtocol();
    void ParseMessage(const char *pData, int dataLen);

private:
    static int gMsgID;
};
