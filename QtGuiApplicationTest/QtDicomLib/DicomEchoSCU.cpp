#include "DicomEchoSCU.h"

// DCMTK Headers
#include <dcmtk/dcmnet/scu.h>

static const char* transferSyntaxes[] =
{
    UID_LittleEndianImplicitTransferSyntax, /* default xfer syntax first */
    UID_LittleEndianExplicitTransferSyntax,
    UID_BigEndianExplicitTransferSyntax,
    UID_JPEGProcess1TransferSyntax,
    UID_JPEGProcess2_4TransferSyntax,
    UID_JPEGProcess3_5TransferSyntax,
    UID_JPEGProcess6_8TransferSyntax,
    UID_JPEGProcess7_9TransferSyntax,
    UID_JPEGProcess10_12TransferSyntax,
    UID_JPEGProcess11_13TransferSyntax,
    UID_JPEGProcess14TransferSyntax,
    UID_JPEGProcess15TransferSyntax,
    UID_JPEGProcess16_18TransferSyntax,
    UID_JPEGProcess17_19TransferSyntax,
    UID_JPEGProcess20_22TransferSyntax,
    UID_JPEGProcess21_23TransferSyntax,
    UID_JPEGProcess24_26TransferSyntax,
    UID_JPEGProcess25_27TransferSyntax,
    UID_JPEGProcess28TransferSyntax,
    UID_JPEGProcess29TransferSyntax,
    UID_JPEGProcess14SV1TransferSyntax,
    UID_RLELosslessTransferSyntax,
    UID_DeflatedExplicitVRLittleEndianTransferSyntax,
    UID_JPEGLSLosslessTransferSyntax,
    UID_JPEGLSLossyTransferSyntax,
    UID_JPEG2000LosslessOnlyTransferSyntax,
    UID_JPEG2000TransferSyntax,
    UID_JPEG2000Part2MulticomponentImageCompressionLosslessOnlyTransferSyntax,
    UID_JPEG2000Part2MulticomponentImageCompressionTransferSyntax,
    UID_MPEG2MainProfileAtMainLevelTransferSyntax,
    UID_MPEG2MainProfileAtHighLevelTransferSyntax,
    UID_MPEG4HighProfileLevel4_1TransferSyntax,
    UID_MPEG4BDcompatibleHighProfileLevel4_1TransferSyntax,
    UID_MPEG4HighProfileLevel4_2_For2DVideoTransferSyntax,
    UID_MPEG4HighProfileLevel4_2_For3DVideoTransferSyntax,
    UID_MPEG4StereoHighProfileLevel4_2TransferSyntax,
    UID_HEVCMainProfileLevel5_1TransferSyntax,
    UID_HEVCMain10ProfileLevel5_1TransferSyntax
};

DicomEchoSCU::DicomEchoSCU(QObject *parent /* = Q_NULLPTR */)
    : QObject(parent)
    , mTimeoutSeconds(30)
    , mServerIP("localhost")
    , mServerPort(5678)
    , mMaxReceivePDU(32768)
    , mpTLSLayer(Q_NULLPTR)
    , mBlockingMode(true)
{
    mpDcmSCU = new DcmSCU();
    mpDcmSCU->setAETitle("DebugTest");
    mpDcmSCU->setPeerHostName("localhost");
    mpDcmSCU->setPeerPort(5678);
}

DicomEchoSCU::~DicomEchoSCU()
{
    if (mpDcmSCU)
    {
        delete mpDcmSCU;
        mpDcmSCU = Q_NULLPTR;
    }
}

void DicomEchoSCU::SetConnectionTimeout(int timeoutSeconds)
{
    mpDcmSCU->setConnectionTimeout(timeoutSeconds);
}

void DicomEchoSCU::SetDIMSETimeout(int timeoutSeconds)
{
    mpDcmSCU->setDIMSETimeout(timeoutSeconds);
}

void DicomEchoSCU::SetACSETimeout(int timeoutSeconds)
{
    mpDcmSCU->setACSETimeout(timeoutSeconds);
}

void DicomEchoSCU::SetPeerHostName(const QString& peerHostName)
{
    mpDcmSCU->setPeerHostName(peerHostName.toStdString().c_str());
}

void DicomEchoSCU::SetPeerPort(const unsigned short peerPort)
{
    mpDcmSCU->setPeerPort(peerPort);
}

void DicomEchoSCU::SetPeerAETitle(const QString& peerAETitle)
{
    mpDcmSCU->setPeerAETitle(peerAETitle.toStdString().c_str());
}

void DicomEchoSCU::SetAppAETitle(const QString& appAETitle)
{
    mpDcmSCU->setAETitle(appAETitle.toStdString().c_str());
}

void DicomEchoSCU::SetMaxReceivePDULength(const unsigned int& maxPDU)
{
    mpDcmSCU->setMaxReceivePDULength(maxPDU);
}

void DicomEchoSCU::SetDIMSEBlockingMode(const bool blockFlag)
{
    if (blockFlag)
    {
        mpDcmSCU->setDIMSEBlockingMode(DIMSE_BLOCKING);
    }
    else
    {
        mpDcmSCU->setDIMSEBlockingMode(DIMSE_NONBLOCKING);
    }
}

void DicomEchoSCU::PerformEcho()
{
    OFList<OFString> transferSyntaxList;
    int transferSyntexCount = sizeof(transferSyntaxes) / sizeof(transferSyntaxes[0]);
    for (int i = 0; i < transferSyntexCount; ++i)
    {
        transferSyntaxList.push_back(transferSyntaxes[i]);
    }
    mpDcmSCU->addPresentationContext(UID_VerificationSOPClass, transferSyntaxList);

    mpDcmSCU->initNetwork();
    mpDcmSCU->negotiateAssociation();
    mpDcmSCU->sendECHORequest(0);
    mpDcmSCU->freeNetwork();
}
