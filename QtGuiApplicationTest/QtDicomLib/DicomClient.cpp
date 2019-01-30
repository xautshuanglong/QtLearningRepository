#include "DicomClient.h"

#include <LogUtil.h>
#include "DicomSCUEcho.h"
#include "DicomSCUFind.h"
#include "DicomSCUGet.h"
#include "DicomSCUMove.h"
#include "DicomSCUStore.h"

/* DICOM standard transfer syntaxes */
static const char* transferSyntaxes[] =
{
    UID_LittleEndianImplicitTransferSyntax, /* default xfer syntax first */
    UID_LittleEndianExplicitTransferSyntax,
    UID_BigEndianExplicitTransferSyntax,
    //      UID_JPEGProcess1TransferSyntax,
    //      UID_JPEGProcess2_4TransferSyntax,
    //      UID_JPEGProcess3_5TransferSyntax,
    //      UID_JPEGProcess6_8TransferSyntax,
    //      UID_JPEGProcess7_9TransferSyntax,
    //      UID_JPEGProcess10_12TransferSyntax,
    //      UID_JPEGProcess11_13TransferSyntax,
    //      UID_JPEGProcess14TransferSyntax,
    //      UID_JPEGProcess15TransferSyntax,
    //      UID_JPEGProcess16_18TransferSyntax,
    //      UID_JPEGProcess17_19TransferSyntax,
    //      UID_JPEGProcess20_22TransferSyntax,
    //      UID_JPEGProcess21_23TransferSyntax,
    //      UID_JPEGProcess24_26TransferSyntax,
    //      UID_JPEGProcess25_27TransferSyntax,
    //      UID_JPEGProcess28TransferSyntax,
    //      UID_JPEGProcess29TransferSyntax,
    //      UID_JPEGProcess14SV1TransferSyntax,
    //      UID_RLELosslessTransferSyntax,
    //      UID_DeflatedExplicitVRLittleEndianTransferSyntax,
    //      UID_JPEGLSLosslessTransferSyntax,
    //      UID_JPEGLSLossyTransferSyntax,
    //      UID_JPEG2000LosslessOnlyTransferSyntax,
    //      UID_JPEG2000TransferSyntax,
    //      UID_JPEG2000Part2MulticomponentImageCompressionLosslessOnlyTransferSyntax,
    //      UID_JPEG2000Part2MulticomponentImageCompressionTransferSyntax,
    //      UID_MPEG2MainProfileAtMainLevelTransferSyntax,
    //      UID_MPEG2MainProfileAtHighLevelTransferSyntax,
    //      UID_MPEG4HighProfileLevel4_1TransferSyntax,
    //      UID_MPEG4BDcompatibleHighProfileLevel4_1TransferSyntax,
    //      UID_MPEG4HighProfileLevel4_2_For2DVideoTransferSyntax,
    //      UID_MPEG4HighProfileLevel4_2_For3DVideoTransferSyntax,
    //      UID_MPEG4StereoHighProfileLevel4_2TransferSyntax,
    //      UID_HEVCMainProfileLevel5_1TransferSyntax,
    //      UID_HEVCMain10ProfileLevel5_1TransferSyntax,
};

DicomClient::DicomClient(QObject *parent)
    : QObject(parent)
    , m_blockFlag(true)
    , m_connectTimeoutSec(30)
    , m_dimseTimeoutSeconds(10)
    , m_acseTimeoutSeconds(30)
    , m_peerPort(5678)
    , m_maxReceivePDU(16384)
    , m_peerHostName("localhost")
    , m_peerAETitle("MGIUSDICOM")
    , m_appAETitle("MGIUSDOCTOR")
{
    m_pDicomEcho = new DicomSCUEcho();
    m_pDicomFind = new DicomSCUFind();
    m_pDicomGet = new DicomSCUGet();
    m_pDicomMove = new DicomSCUMove();
    m_pDicomStore = new DicomSCUStore();
}

DicomClient::~DicomClient()
{
    delete m_pDicomEcho; m_pDicomEcho = Q_NULLPTR;
    delete m_pDicomFind; m_pDicomFind = Q_NULLPTR;
    delete m_pDicomGet;  m_pDicomGet = Q_NULLPTR;
    delete m_pDicomMove; m_pDicomMove = Q_NULLPTR;
    delete m_pDicomStore; m_pDicomStore = Q_NULLPTR;
}

void DicomClient::RegisterObserver()
{
    m_pDicomEcho->RegisterObserver(this);
    m_pDicomFind->RegisterObserver(this);
    m_pDicomGet->RegisterObserver(this);
    m_pDicomMove->RegisterObserver(this);
    m_pDicomStore->RegisterObserver(this);
}

void DicomClient::PerformEcho()
{
    OFList<OFString> transferSyntaxList;
    Uint32 maxSyntaxes = OFstatic_cast(Uint32, (DIM_OF(transferSyntaxes)));
    for (Uint32 i = 0; i < maxSyntaxes; ++i)
    {
        transferSyntaxList.push_back(transferSyntaxes[i]);
    }

    m_pDicomEcho->ClearPresentationContex(); // 考虑使用已接收的表示上下文
    m_pDicomEcho->AddPresentationContext(UID_VerificationSOPClass, transferSyntaxList);

    OFCondition condition = m_pDicomEcho->PerformEcho();
    if (condition.bad())
    {
        OFString errorString;
        LogUtil::Error(CODE_LOCATION, "PerformEcho Error: %s", DimseCondition::dump(errorString, condition).c_str());
        // TODO 向业务层报告错误
    }
}

void DicomClient::PerformFind()
{
    OFList<OFString> transferSyntaxList;
    Uint32 maxSyntaxes = OFstatic_cast(Uint32, (DIM_OF(transferSyntaxes)));
    for (Uint32 i = 0; i < maxSyntaxes; ++i)
    {
        transferSyntaxList.push_back(transferSyntaxes[i]);
    }
    m_pDicomFind->ClearPresentationContex(); // 考虑使用已接收的表示上下文
    m_pDicomFind->AddPresentationContext(UID_FINDStudyRootQueryRetrieveInformationModel, transferSyntaxList);
    //    m_pDicomFind->AddPresentationContext(UID_FINDModalityWorklistInformationModel, transferSyntaxList);
    //    m_pDicomFind->AddPresentationContext(UID_FINDPatientRootQueryRetrieveInformationModel, transferSyntaxList);
    //    m_pDicomFind->AddPresentationContext(UID_RETIRED_FINDPatientStudyOnlyQueryRetrieveInformationModel, transferSyntaxList);

    m_pDicomFind->AddQueryKey("PatientName=HEAD EXP2");
    m_pDicomFind->AddQueryKey("QueryRetrieveLevel=STUDY");
    m_pDicomFind->AddQueryKey("PatientID");
    m_pDicomFind->AddQueryKey("NumberOfStudyRelatedInstances");

    OFCondition condition = m_pDicomFind->PerformFind(FindModel_StudyRoot);
    if (condition.bad())
    {
        OFString errorString;
        LogUtil::Error(CODE_LOCATION, "PerformFind Error: %s", DimseCondition::dump(errorString, condition).c_str());
        // TODO 向业务层报告错误
    }
}

void DicomClient::PerformGet()
{
    OFList<OFString> transferSyntaxList;
    Uint32 maxSyntaxes = OFstatic_cast(Uint32, (DIM_OF(transferSyntaxes)));
    for (Uint32 i = 0; i < maxSyntaxes; ++i)
    {
        transferSyntaxList.push_back(transferSyntaxes[i]);
    }
    m_pDicomGet->SetMaxReceivePDU(m_maxReceivePDU);
    m_pDicomGet->ClearPresentationContex(); // 考虑使用已接收的表示上下文
    m_pDicomGet->AddPresentationContext(UID_GETPatientRootQueryRetrieveInformationModel, transferSyntaxList);
    for (Uint16 j = 0; j < numberOfDcmLongSCUStorageSOPClassUIDs; j++)
    {
        m_pDicomGet->AddPresentationContext(dcmLongSCUStorageSOPClassUIDs[j], transferSyntaxList, ASC_SC_ROLE_SCP);
    }

    //m_pDicomGet->AddQueryKey("PatientName=HEAD EXP2");
    m_pDicomGet->AddQueryKey("PatientName=GAMAGE^MARY");

    OFCondition condition = m_pDicomGet->PerformGet(GetModel_PatientRoot);
    if (condition.bad())
    {
        OFString errorString;
        LogUtil::Error(CODE_LOCATION, "PerformGet Error: %s", DimseCondition::dump(errorString, condition).c_str());
        // TODO 向业务层报告错误
    }
}

void DicomClient::PerformMove()
{
    OFList<OFString> transferSyntaxList;
    Uint32 maxSyntaxes = OFstatic_cast(Uint32, (DIM_OF(transferSyntaxes)));
    for (Uint32 i = 0; i < maxSyntaxes; ++i)
    {
        transferSyntaxList.push_back(transferSyntaxes[i]);
    }
    m_pDicomMove->SetMaxReceivePDU(m_maxReceivePDU);
    m_pDicomMove->ClearPresentationContex(); // 考虑使用已接收的表示上下文
    m_pDicomMove->AddPresentationContext(UID_MOVEPatientRootQueryRetrieveInformationModel, transferSyntaxList);
    for (Uint16 j = 0; j < numberOfDcmLongSCUStorageSOPClassUIDs; j++)
    {
        m_pDicomMove->AddPresentationContext(dcmLongSCUStorageSOPClassUIDs[j], transferSyntaxList, ASC_SC_ROLE_SCP);
    }

    m_pDicomMove->AddQueryKey("PatientName=HEAD EXP2");
    //m_pDicomMove->AddQueryKey("PatientName=GAMAGE^MARY");

    OFCondition condition = m_pDicomMove->PerformMove(MoveModel_PatientRoot);
    if (condition.bad())
    {
        OFString errorString;
        LogUtil::Error(CODE_LOCATION, "PerformMove Error: %s", DimseCondition::dump(errorString, condition).c_str());
        // TODO 向业务层报告错误
    }
}

void DicomClient::PerformStore()
{
    OFList<OFString> transferSyntaxList;
    Uint32 maxSyntaxes = OFstatic_cast(Uint32, (DIM_OF(transferSyntaxes)));
    for (Uint32 i = 0; i < maxSyntaxes; ++i)
    {
        transferSyntaxList.push_back(transferSyntaxes[i]);
    }
    m_pDicomStore->ClearPresentationContex(); // 考虑使用已接收的表示上下文
    for (int i = 0; i < numberOfDcmShortSCUStorageSOPClassUIDs; i++)
    {
        m_pDicomStore->AddPresentationContext(dcmShortSCUStorageSOPClassUIDs[i], transferSyntaxList);
    }

    OFString filename = "./image11.dcm";
    OFCondition condition = m_pDicomStore->PerformStore(filename);
    if (condition.bad())
    {
        OFString errorString;
        LogUtil::Error(CODE_LOCATION, "PerformStore Error: %s", DimseCondition::dump(errorString, condition).c_str());
        // TODO 向业务层报告错误
    }
}

void DicomClient::HandleResponseEcho()
{
    emit SigResponseEcho();
}

void DicomClient::HandleResponseFind()
{
    emit SigResponseFind();
}

void DicomClient::HandleResponseGet()
{
    emit SigResponseGet();
}

void DicomClient::HandleResponseMove()
{
    emit SigResponseMove();
}

void DicomClient::HandleResponseStore()
{
    emit SigResponseStore();
}
