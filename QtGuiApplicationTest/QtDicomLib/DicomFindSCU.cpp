#include "DicomFindSCU.h"

#include <dcmtk/dcmnet/dfindscu.h>
#include <dcmtk/oflog/oflog.h>

static OFLogger findscuLogger = OFLog::getLogger("dcmtk.apps.QtDicomTest");

// 关联可用的查询模型
//typedef struct tagAbstractSyntaxModel
//{
//    int modelIndex;
//    QString descritption;
//    QString abstractSyntax;
//};

DicomFindSCU::DicomFindSCU(QObject *parent /* = Q_NULLPTR */)
    : QObject(parent)
    , mTimeoutSeconds(30)
    , mServerIP("localhost")
    , mServerPort(5678)
    , mMaxReceivePDU(32768)
    , mpTLSLayer(Q_NULLPTR)
    , mOutputDirectory("./")
{
    mpDcmFindSCU = new DcmFindSCU();
}

DicomFindSCU::~DicomFindSCU()
{
    if (mpDcmFindSCU)
    {
        delete mpDcmFindSCU;
        mpDcmFindSCU = Q_NULLPTR;
    }
    if (mpTLSLayer)
    {
        delete mpTLSLayer;
        mpTLSLayer = Q_NULLPTR;
    }
}

void DicomFindSCU::SetServerIpPort(const QString& serverIP, const unsigned int& serverPort)
{
    mServerIP = serverIP;
    mServerPort = serverPort;
}

void DicomFindSCU::SetAppEntityTitle(const QString& selfTitle, const QString& peerTitle)
{
    mAppEntityTitle = selfTitle;
    mPeerEntityTitle = peerTitle;
}

void DicomFindSCU::InitializeNetwork()
{
    OFString errorString;
    OFCondition condition = mpDcmFindSCU->initializeNetwork(mTimeoutSeconds);
    if (condition.bad())
    {
        OFLOG_ERROR(findscuLogger, DimseCondition::dump(errorString, condition));
        emit SigErrorString(QString(errorString.c_str()));
    }
}

void DicomFindSCU::DropNetwork()
{
    OFString errorString;
    OFCondition condition = mpDcmFindSCU->dropNetwork();
    if (condition.bad())
    {
        OFLOG_ERROR(findscuLogger, DimseCondition::dump(errorString, condition));
        emit SigErrorString(QString(errorString.c_str()));
    }
}

void DicomFindSCU::PerformQuery()
{
    OFString errorString;

    //OFString abstractSyntax = UID_FINDModalityWorklistInformationModel;
    //OFString abstractSyntax = UID_FINDPatientRootQueryRetrieveInformationModel;
    OFString abstractSyntax = UID_FINDStudyRootQueryRetrieveInformationModel;
    E_TransferSyntax transferSyntax = EXS_Unknown;
    T_DIMSE_BlockingMode  blockMode = DIMSE_BLOCKING;
    DcmFindSCUExtractMode extractMode = FEM_none;
    OFCmdSignedInt        cancelAfterNResponses = -1;
    OFBool                secureConnection = OFFalse;
    OFBool                abortAssociation = OFFalse;

    OFList<OFString>      fileNameList;
    for each (QString key in mFilenameList)
    {
        fileNameList.push_back(key.toStdString().c_str());
    }

    OFList<OFString>      overrideKeys;
    for each (QString key in mOverrideKes)
    {
        overrideKeys.push_back(key.toStdString().c_str());
    }

    OFCondition condition = mpDcmFindSCU->performQuery(
        mServerIP.toStdString().c_str(), mServerPort,
        mAppEntityTitle.toStdString().c_str(), mPeerEntityTitle.toStdString().c_str(),
        abstractSyntax.c_str(), transferSyntax, blockMode, mTimeoutSeconds, mMaxReceivePDU,
        secureConnection, abortAssociation, 1, extractMode, cancelAfterNResponses,
        &overrideKeys, NULL, &fileNameList,
        mOutputDirectory.toStdString().c_str(), mOutputXmlFilename.toStdString().c_str()
    );
    if (condition.bad())
    {
        OFLOG_ERROR(findscuLogger, DimseCondition::dump(errorString, condition));
        emit SigErrorString(QString(errorString.c_str()));
    }
}
