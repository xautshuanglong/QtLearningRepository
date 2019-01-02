#pragma once

// QT Headers
#include <QObject>

#include "qtdicomlib_global.h"

class QTDICOMLIB_EXPORT DicomTest : public QObject
{
public:
    explicit DicomTest(QObject *parent = Q_NULLPTR);
    ~DicomTest();

    void DcmtkTestEcho();
    void DcmtkTestFind();
    void DcmtkTestGet();
    void DcmtkTestMove();
    void DcmtkTestStore();
};
