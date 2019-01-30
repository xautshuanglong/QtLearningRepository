#pragma once

#include "DicomSCUBase.h"

class DicomSCUEcho : public DicomSCUBase
{
public:
    DicomSCUEcho();
    ~DicomSCUEcho();

    OFCondition PerformEcho();
};
