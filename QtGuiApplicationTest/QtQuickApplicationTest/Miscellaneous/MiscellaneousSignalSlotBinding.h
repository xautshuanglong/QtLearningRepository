#pragma once

#include <QObject>

class MiscellaneousSignalSlotBinding : public QObject
{
    Q_OBJECT

public:
    explicit MiscellaneousSignalSlotBinding(QObject *parent = Q_NULLPTR);
    ~MiscellaneousSignalSlotBinding();

public slots:
    void slotMainWindowCompleted(int intValue, QString strValue);
    void slotFromMainWinToBindingTest(int intValue, QString strValue);

signals:
    void sigFromMainWinToBindingTest(int intValue, QString strValue);
};
