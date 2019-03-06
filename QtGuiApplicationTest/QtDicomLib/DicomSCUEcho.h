#pragma once

#include "DicomSCUBase.h"

class DicomSCUEcho : public DicomSCUBase
{
public:
    DicomSCUEcho();
    ~DicomSCUEcho();

    virtual OFCondition ExcuteOperation(QSharedPointer<DicomTaskBase> &pDicomTask) override;
    virtual OFCondition PerformEcho() override;
};
