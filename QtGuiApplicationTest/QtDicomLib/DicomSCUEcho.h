#pragma once

#include "DicomSCUBase.h"

class QTDICOMLIB_EXPORT DicomSCUEcho : public DicomSCUBase
{
public:
    DicomSCUEcho();
    ~DicomSCUEcho();

    OFCondition PerformEcho();
};
