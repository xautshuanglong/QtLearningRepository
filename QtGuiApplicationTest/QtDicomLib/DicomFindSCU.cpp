#include "DicomFindSCU.h"

#include <dcmtk/dcmnet/dfindscu.h>
#include <dcmtk/dcmnet/diutil.h>
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
    , DcmFindSCUCallback()
    , mExtractMode(FEM_none)
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
        &overrideKeys, this, &fileNameList,
        mOutputDirectory.toStdString().c_str(), mOutputXmlFilename.toStdString().c_str()
    );
    if (condition.bad())
    {
        OFLOG_ERROR(findscuLogger, DimseCondition::dump(errorString, condition));
        emit SigErrorString(QString(errorString.c_str()));
    }
}

void DicomFindSCU::callback(T_DIMSE_C_FindRQ *request, int responseCount, T_DIMSE_C_FindRSP *rsp, DcmDataset *responseIdentifiers)
{
    OFLogger rspLogger = OFLog::getLogger("dcmtk.dcmnet.responses");
    if (DCM_dcmnetLogger.isEnabledFor(OFLogger::DEBUG_LOG_LEVEL))
    {
        OFString temp_str;
        DCMNET_INFO("Received Find Response " << responseCount);
        DCMNET_DEBUG(DIMSE_dumpMessage(temp_str, *rsp, DIMSE_INCOMING));
        if (rspLogger.isEnabledFor(OFLogger::INFO_LOG_LEVEL))
        {
            DCMNET_DEBUG("Response Identifiers:" << OFendl << DcmObject::PrintHelper(*responseIdentifiers));
        }
    }
    else if (rspLogger.isEnabledFor(OFLogger::INFO_LOG_LEVEL))
    {
        OFLOG_INFO(rspLogger, "---------------------------");
        OFLOG_INFO(rspLogger, "Find Response: " << responseCount << " (" << DU_cfindStatusString(rsp->DimseStatus) << ")");
        OFLOG_INFO(rspLogger, DcmObject::PrintHelper(*responseIdentifiers));
    }
    else
    {
        DCMNET_INFO("Received Find Response " << responseCount << " (" << DU_cfindStatusString(rsp->DimseStatus) << ")");
    }

    if (mExtractMode != FEM_none)
    {
        this->ExtractResponseToFile(responseCount, responseIdentifiers);
    }
}

void DicomFindSCU::ExtractResponseToFile(int responseCount, DcmDataset *pResponseIdentifiers)
{
    if (mExtractMode == FEM_dicomFile)
    {
        OFString outputFilename;
        char rspIdsFileName[16];
        sprintf(rspIdsFileName, "rsp%04d.dcm", responseCount);
        OFStandard::combineDirAndFilename(outputFilename, mOutputDirectory.toStdString().c_str(), rspIdsFileName, OFTrue);
        DCMNET_INFO("Writing response dataset to file: " << outputFilename);
        DcmFindSCU::writeToFile(outputFilename.c_str(), pResponseIdentifiers);
    }
    else if (mExtractMode == FEM_xmlFile)
    {
        OFString outputFilename;
        char rspIdsFileName[16];
        sprintf(rspIdsFileName, "rsp%04d.xml", responseCount);
        OFStandard::combineDirAndFilename(outputFilename, mOutputDirectory.toStdString().c_str(), rspIdsFileName, OFTrue);
        DCMNET_INFO("Writing response dataset to file: " << outputFilename);
        DcmFindSCU::writeToXMLFile(outputFilename.c_str(), pResponseIdentifiers);
    }
    //    else if (mExtractMode == FEM_singleXMLFile)
    //    {
    //        if (outputStream_ != NULL)
    //        {
    //            OFCondition cond = EC_Normal;
    //            size_t writeFlags = 0;
    //            DCMNET_DEBUG("Writing response dataset to XML file");
    //            /* expect that (0008,0005) is set if extended characters are used */
    //            if (responseIdentifiers->tagExistsWithValue(DCM_SpecificCharacterSet))
    //            {
    //#ifdef DCMTK_ENABLE_CHARSET_CONVERSION
    //                DCMNET_DEBUG("Converting all element values that are affected by SpecificCharacterSet (0008,0005) to UTF-8");
    //                cond = responseIdentifiers->convertToUTF8();
    //#else
    //                if (responseIdentifiers->containsExtendedCharacters(OFFalse /*checkAllStrings*/))
    //                {
    //                    DCMNET_WARN("No support for character set conversion available ... quoting non-ASCII characters");
    //                    /* make sure that non-ASCII characters are quoted appropriately */
    //                    writeFlags |= DCMTypes::XF_convertNonASCII;
    //                }
    //                else {
    //                    DCMNET_DEBUG("No support for character set conversion available");
    //                }
    //#endif
    //            }
    //            if (cond.good())
    //                cond = responseIdentifiers->writeXML(*outputStream_, writeFlags);
    //            if (cond.bad())
    //                DCMNET_ERROR("Writing XML file: " << cond.text());
    //        }
    //    }
}
