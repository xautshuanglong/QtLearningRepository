#include "DicomSCUEcho.h"

#include <LogUtil.h>
#include <dcmtk/dcmnet/diutil.h>

/* DICOM standard transfer syntaxes */
static const char* transferSyntaxes[] =
{
    UID_LittleEndianImplicitTransferSyntax,
    UID_LittleEndianExplicitTransferSyntax,
    UID_BigEndianExplicitTransferSyntax
};

DicomSCUEcho::DicomSCUEcho()
{
}

DicomSCUEcho::~DicomSCUEcho()
{
}

OFCondition DicomSCUEcho::ExcuteOperation(QSharedPointer<DicomTaskBase> &pDicomTask)
{
    OFList<OFString> transferSyntaxList;
    Uint32 maxSyntaxes = OFstatic_cast(Uint32, (DIM_OF(transferSyntaxes)));
    for (Uint32 i = 0; i < maxSyntaxes; ++i)
    {
        transferSyntaxList.push_back(transferSyntaxes[i]);
    }

    this->ClearPresentationContex(); // ����ʹ���ѽ��յı�ʾ������
    this->AddPresentationContext(UID_VerificationSOPClass, transferSyntaxList);

    OFCondition condition = this->PerformEcho();
    if (condition.bad())
    {
        OFString errorString;
        LogUtil::Error(CODE_LOCATION, "PerformEcho Error: %s", DimseCondition::dump(errorString, condition).c_str());
        // TODO ��ҵ��㱨�����
    }

    return condition;
}

OFCondition DicomSCUEcho::PerformEcho()
{
    OFCondition condition;

    condition = this->InitNetwork();
    if (condition.good())
    {
        condition = this->UseSecureConnection();
    }
    if (condition.good())
    {
        condition = this->NegotiateAssociation();
        // TODO �������Э�̽��
    }
    if (condition.good())
    {
        condition = this->EchoUser();
        if (condition.good())
        {
            condition = this->ReleaseAssociation();
        }
        else if (condition == DUL_PEERREQUESTEDRELEASE)
        {
            condition = this->AbortAssociation();
        } 
        else
        {
            OFString errorString;
            LogUtil::Error(CODE_LOCATION, "DicomSCUBase::EchoUser --> %s", DimseCondition::dump(errorString, condition).c_str());
        }
        // TODO ������Ӧ��Ϣ
    }

    condition = this->DropNetwork();
    if (condition.bad())
    {
        OFString errorString;
        LogUtil::Error(CODE_LOCATION, "DicomSCUBase::DropNetwork --> %s", DimseCondition::dump(errorString, condition).c_str());
    }

    return condition;
}
