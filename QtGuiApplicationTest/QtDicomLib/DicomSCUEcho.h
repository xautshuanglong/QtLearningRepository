#pragma once

#include "DicomSCUBase.h"

class DicomSCUEcho : public DicomSCUBase
{
public:
    DicomSCUEcho();
    ~DicomSCUEcho();

    virtual OFCondition PerformEcho() override;
};
