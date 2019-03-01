#pragma once

#include <QObject>

class DicomSCUExecutor : public QObject
{
public:
    DicomSCUExecutor(QObject *parent=Q_NULLPTR);
    ~DicomSCUExecutor();
};

