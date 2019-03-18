#include "DicomClient.h"

#include <LogUtil.h>
#include "DicomSCUEcho.h"
#include "DicomSCUFind.h"
#include "DicomSCUGet.h"
#include "DicomSCUMove.h"
#include "DicomSCUStore.h"
#include "DicomExecutor.h"

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

    DicomExecutor *pDicomExecutor = DicomExecutor::GetInstance();
    pDicomExecutor->SetExecutor(EexcutorType_ECHO, m_pDicomEcho);
    pDicomExecutor->SetExecutor(EexcutorType_FIND, m_pDicomFind);
    pDicomExecutor->SetExecutor(EexcutorType_GET, m_pDicomGet);
    pDicomExecutor->SetExecutor(EexcutorType_MOVE, m_pDicomMove);
    pDicomExecutor->SetExecutor(EexcutorType_SOTRE, m_pDicomStore);
    pDicomExecutor->Start();
}

DicomClient::~DicomClient()
{
    DicomExecutor::GetInstance()->Stop();
    delete m_pDicomEcho; m_pDicomEcho = Q_NULLPTR;
    delete m_pDicomFind; m_pDicomFind = Q_NULLPTR;
    delete m_pDicomGet;  m_pDicomGet = Q_NULLPTR;
    delete m_pDicomMove; m_pDicomMove = Q_NULLPTR;
    delete m_pDicomStore; m_pDicomStore = Q_NULLPTR;
}

void DicomClient::SetConnectionTimeout(int timeoutSeconds)
{
    m_connectTimeoutSec = timeoutSeconds;
    m_pDicomEcho->SetTimeoutConnection(timeoutSeconds);
    m_pDicomFind->SetTimeoutConnection(timeoutSeconds);
    m_pDicomGet->SetTimeoutConnection(timeoutSeconds);
    m_pDicomMove->SetTimeoutConnection(timeoutSeconds);
    m_pDicomStore->SetTimeoutConnection(timeoutSeconds);
}

void DicomClient::SetDIMSETimeout(int timeoutSeconds)
{
    m_dimseTimeoutSeconds = timeoutSeconds;
    m_pDicomEcho->SetTimeoutDimse(timeoutSeconds);
    m_pDicomFind->SetTimeoutDimse(timeoutSeconds);
    m_pDicomGet->SetTimeoutDimse(timeoutSeconds);
    m_pDicomMove->SetTimeoutDimse(timeoutSeconds);
    m_pDicomStore->SetTimeoutDimse(timeoutSeconds);
}

void DicomClient::SetACSETimeout(int timeoutSeconds)
{
    m_acseTimeoutSeconds = timeoutSeconds;
    m_pDicomEcho->SetTimeoutAcse(timeoutSeconds);
    m_pDicomFind->SetTimeoutAcse(timeoutSeconds);
    m_pDicomGet->SetTimeoutAcse(timeoutSeconds);
    m_pDicomMove->SetTimeoutAcse(timeoutSeconds);
    m_pDicomStore->SetTimeoutAcse(timeoutSeconds);
}

void DicomClient::SetPeerHostName(const QString& peerHostName)
{
    m_peerHostName = peerHostName;
    m_pDicomEcho->SetPeerHostname(peerHostName.toStdString().c_str());
    m_pDicomFind->SetPeerHostname(peerHostName.toStdString().c_str());
    m_pDicomGet->SetPeerHostname(peerHostName.toStdString().c_str());
    m_pDicomMove->SetPeerHostname(peerHostName.toStdString().c_str());
    m_pDicomStore->SetPeerHostname(peerHostName.toStdString().c_str());
}

void DicomClient::SetPeerPort(const unsigned short peerPort)
{
    m_peerPort = peerPort;
    m_pDicomEcho->SetPeerPort(peerPort);
    m_pDicomFind->SetPeerPort(peerPort);
    m_pDicomGet->SetPeerPort(peerPort);
    m_pDicomMove->SetPeerPort(peerPort);
    m_pDicomStore->SetPeerPort(peerPort);
}

void DicomClient::SetPeerAETitle(const QString& peerAETitle)
{
    m_peerAETitle = peerAETitle;
    m_pDicomEcho->SetPeerTitle(OFString(peerAETitle.toStdString().c_str()));
    m_pDicomFind->SetPeerTitle(OFString(peerAETitle.toStdString().c_str()));
    m_pDicomGet->SetPeerTitle(OFString(peerAETitle.toStdString().c_str()));
    m_pDicomMove->SetPeerTitle(OFString(peerAETitle.toStdString().c_str()));
    m_pDicomStore->SetPeerTitle(OFString(peerAETitle.toStdString().c_str()));
}

void DicomClient::SetAppAETitle(const QString& appAETitle)
{
    m_appAETitle = appAETitle;
    m_pDicomEcho->SetAppTitle(OFString(appAETitle.toStdString().c_str()));
    m_pDicomFind->SetAppTitle(OFString(appAETitle.toStdString().c_str()));
    m_pDicomGet->SetAppTitle(OFString(appAETitle.toStdString().c_str()));
    m_pDicomMove->SetAppTitle(OFString(appAETitle.toStdString().c_str()));
    m_pDicomStore->SetAppTitle(OFString(appAETitle.toStdString().c_str()));
}

void DicomClient::SetMaxReceivePDULength(const unsigned int& maxPDU)
{
    m_maxReceivePDU = maxPDU;
    m_pDicomEcho->SetMaxReceivePDU(maxPDU);
    m_pDicomFind->SetMaxReceivePDU(maxPDU);
    m_pDicomGet->SetMaxReceivePDU(maxPDU);
    m_pDicomMove->SetMaxReceivePDU(maxPDU);
    m_pDicomStore->SetMaxReceivePDU(maxPDU);
}

void DicomClient::SetDIMSEBlockingMode(const bool blockFlag)
{
    m_blockFlag = blockFlag;
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

    QSharedPointer<DicomTaskBase> pEchoTask(DicomTaskHelper::NewTask<DicomTaskEcho>());
    DicomExecutor::GetInstance()->AddTask(pEchoTask);

    return;

    m_pDicomEcho->PerformEcho();
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

//void DicomClient::PerformStore()
//{
//    OFList<OFString> transferSyntaxList;
//    Uint32 maxSyntaxes = OFstatic_cast(Uint32, (DIM_OF(transferSyntaxes)));
//    for (Uint32 i = 0; i < maxSyntaxes; ++i)
//    {
//        transferSyntaxList.push_back(transferSyntaxes[i]);
//    }
//    m_pDicomStore->ClearPresentationContex(); // 考虑使用已接收的表示上下文
//    for (int i = 0; i < numberOfDcmShortSCUStorageSOPClassUIDs; i++)
//    {
//        m_pDicomStore->AddPresentationContext(dcmShortSCUStorageSOPClassUIDs[i], transferSyntaxList);
//    }
//
//    QString filename = "E:\\Temp\\DicomTesting\\DcmtkBin\\image11.dcm";
//    QString filename1 = "E:\\Temp\\DicomTesting\\DcmtkBin\\image18.dcm";
//
//    DicomTaskStore *pStoreTask = DicomTaskHelper::NewTask<DicomTaskStore>();
//    pStoreTask->SetFilename(filename);
//    QSharedPointer<DicomTaskBase> pStoreTaskPointer(pStoreTask);
//    m_pDicomExecutor->AddTask(pStoreTaskPointer);
//
//    pStoreTask = DicomTaskHelper::NewTask<DicomTaskStore>();
//    pStoreTask->SetFilename(filename1);
//    QSharedPointer<DicomTaskBase> pStoreTaskPointer1(pStoreTask);
//    m_pDicomExecutor->AddTask(pStoreTaskPointer1);
//
//    //OFCondition condition = m_pDicomStore->PerformStore(filename);
//    //if (condition.bad())
//    //{
//    //    OFString errorString;
//    //    LogUtil::Error(CODE_LOCATION, "PerformStore Error: %s", DimseCondition::dump(errorString, condition).c_str());
//    //    // TODO 向业务层报告错误
//    //}
//}

void DicomClient::MakeStoreTask(const QString &filename)
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

    DicomTaskStore *pStoreTask = DicomTaskHelper::NewTask<DicomTaskStore>();
    pStoreTask->SetFilename(filename);
    QSharedPointer<DicomTaskBase> pStoreTaskPointer(pStoreTask);
    DicomExecutor::GetInstance()->AddTask(pStoreTaskPointer);
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
