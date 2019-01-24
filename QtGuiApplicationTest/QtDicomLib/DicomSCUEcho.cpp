#include "DicomSCUEcho.h"

#include <LogUtil.h>
#include <dcmtk/dcmnet/diutil.h>

DicomSCUEcho::DicomSCUEcho()
{
}

DicomSCUEcho::~DicomSCUEcho()
{
}

OFCondition DicomSCUEcho::PerformEcho()
{
    OFCondition condition;

    condition = this->InitNetwork();
    if (condition.good())
    {
        condition = this->NegotiateAssociation();
        // TODO 处理关联协商结果
    }
    if (condition.good())
    {
        condition = this->EchoUser();
        if (condition.bad())
        {
            OFString errorString;
            LogUtil::Error(CODE_LOCATION, "DicomSCUBase::EchoUser --> %s", DimseCondition::dump(errorString, condition).c_str());
        }
        // TODO 处理响应消息
    }

    condition = this->DropNetwork();
    if (condition.bad())
    {
        OFString errorString;
        LogUtil::Error(CODE_LOCATION, "DicomSCUBase::DropNetwork --> %s", DimseCondition::dump(errorString, condition).c_str());
    }

    return condition;
}

