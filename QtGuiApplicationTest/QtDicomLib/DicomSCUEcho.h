#pragma once

#include "DicomSCUBase.h"

class DicomSCUEcho : public DicomSCUBase
{
public:
    DicomSCUEcho();
    ~DicomSCUEcho();

    OFCondition PerformEcho();
    virtual OFCondition ExcuteOperation(QSharedPointer<DicomTaskBase> &pDicomTask) override;
};
