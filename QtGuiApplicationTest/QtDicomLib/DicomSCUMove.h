#pragma once

#include "DicomSCUBase.h"

class DicomSCUMove : public DicomSCUBase
{
public:
    DicomSCUMove();
    ~DicomSCUMove();

    OFCondition PerformMove();
};
