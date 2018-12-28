#include "DicomServerBrowserWidget.h"
#include "ui_DicomServerBrowserWidget.h"

// DCMTK Headers
#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmnet/dimse.h>
#include <dcmtk/dcmnet/diutil.h>
#include <dcmtk/dcmnet/dfindscu.h>
#include <dcmtk/dcmtls/tlsopt.h>

#include <LogUtil.h>
#include "TitleBarWidget.h"

static char rcsid[] = "$dcmtk: DicomNetworkTest v" OFFIS_DCMTK_VERSION " " OFFIS_DCMTK_RELEASEDATE " $";

/* default application titles */
#define APPLICATIONTITLE     "DebugTest"
#define PEERAPPLICATIONTITLE "MGIUSDICOM"

/* DICOM standard transfer syntaxes */
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

DicomServerBrowserWidget::DicomServerBrowserWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DicomServerBrowserWidget)
{
    ui->setupUi(this);

    QIcon winIcon(":/AppImages/Resources/images/dicom_xray.ico");
    this->setWindowIcon(winIcon);
    mpTitleBarWidget = new TitleBarWidget(this);
}

DicomServerBrowserWidget::~DicomServerBrowserWidget()
{
    delete ui;
}

void DicomServerBrowserWidget::on_btnEchoTest_clicked()
{
    DCM_dcmnetLogger.setLogLevel(OFLogger::DEBUG_LOG_LEVEL);

    DcmTLSOptions         tlsOptions(NET_REQUESTOR);
    T_ASC_Network        *pNetwork = nullptr;
    T_ASC_Parameters     *pAscParams = nullptr;
    T_ASC_Association    *pAssociation = nullptr;
    T_DIMSE_BlockingMode  blockMode = DIMSE_BLOCKING; // DIMSE_BLOCKING or DIMSE_NONBLOCKING
    OFString              errorString;
    OFCmdUnsignedInt      numXferSyntaxes = 1;

    OFCondition cond = ASC_initializeNetwork(NET_REQUESTOR, 0, 100, &pNetwork);
    if (cond.bad())
    {
        DimseCondition::dump(errorString, cond);
        LogUtil::Error(CODE_LOCATION, "Error: %s", errorString.c_str());
        return;
    }
    // 设置关联参数
    cond = ASC_createAssociationParameters(&pAscParams, ASC_DEFAULTMAXPDU);
    if (cond.bad())
    {
        DimseCondition::dump(errorString, cond);
        LogUtil::Error(CODE_LOCATION, "Error: %s", errorString.c_str());
        return;
    }
    ASC_setAPTitles(pAscParams, APPLICATIONTITLE, PEERAPPLICATIONTITLE, NULL);
    cond = ASC_setPresentationAddresses(pAscParams, OFStandard::getHostName().c_str(), "localhost:5678");
    if (cond.bad())
    {
        DimseCondition::dump(errorString, cond);
        LogUtil::Error(CODE_LOCATION, "Error: %s", errorString.c_str());
        return;
    }
    int presentationContextID = 1;
    int numPresentationCtx = 1;
    for (unsigned long ii = 0; ii < numPresentationCtx; ii++)
    {
        cond = ASC_addPresentationContext(pAscParams, presentationContextID, UID_VerificationSOPClass,
                                          transferSyntaxes, OFstatic_cast(int, numXferSyntaxes));
        presentationContextID += 2;
        if (cond.bad())
        {
            DimseCondition::dump(errorString, cond);
            LogUtil::Error(CODE_LOCATION, "Error: %s", errorString.c_str());
            return;
        }
    }

    ASC_dumpParameters(errorString, pAscParams, ASC_ASSOC_RQ);
    LogUtil::Debug(CODE_LOCATION, "Request Parameters below:\n%s", errorString.c_str());

    cond = ASC_requestAssociation(pNetwork, pAscParams, &pAssociation);
    if (cond.bad())
    {
        if (cond == DUL_ASSOCIATIONREJECTED)
        {
            T_ASC_RejectParameters rejParam;
            ASC_getRejectParameters(pAscParams, &rejParam);
            ASC_printRejectParameters(errorString, &rejParam);
            LogUtil::Error(CODE_LOCATION, "Association Rejected: %s", errorString.c_str());
            return;
        }
        else
        {
            LogUtil::Error(CODE_LOCATION, "Association Request Failed: ", DimseCondition::dump(errorString, cond).c_str());
            return;
        }
    }
    int nAcceptedCount = ASC_countAcceptedPresentationContexts(pAscParams);
    LogUtil::Debug(CODE_LOCATION, "%d parameters are accepted ...", nAcceptedCount);

    ASC_dumpParameters(errorString, pAscParams, ASC_ASSOC_AC);
    LogUtil::Debug(CODE_LOCATION, "Association Parameters Negotiated below:\n%s", errorString.c_str());

    cond = EC_Normal;
    int numRepeat = 5;
    while (cond.good() && numRepeat--)
    {
        DIC_US msgId = pAssociation->nextMsgID++;
        DIC_US status;
        DcmDataset *statusDetail = NULL;

        LogUtil::Info(CODE_LOCATION, "Sending Echo Request (MsgID %d)", msgId);
        cond = DIMSE_echoUser(pAssociation, msgId, blockMode, 300, &status, &statusDetail);

        if (cond.good())
        {
            LogUtil::Info(CODE_LOCATION, "Received Echo Response (%s)", DU_cechoStatusString(status));
        }
        else
        {
            LogUtil::Error(CODE_LOCATION, "Echo Failed: %s", DimseCondition::dump(errorString, cond).c_str());
        }

        if (statusDetail != NULL)
        {
            OFStringStream strStream;
            strStream << DcmObject::PrintHelper(*statusDetail);
            LogUtil::Debug(CODE_LOCATION, "Status Detail (should never be any): %s", strStream.str().c_str());
            //OFOStringStream oss;
            //oss << DcmObject::PrintHelper(*statusDetail);
            //LogUtil::Debug(CODE_LOCATION, "Status Detail (should never be any): %s", oss.str().c_str());
            delete statusDetail;
        }
    }

    if (cond == EC_Normal)
    {
        cond = ASC_releaseAssociation(pAssociation);
        if (cond.bad())
        {
            LogUtil::Error(CODE_LOCATION, "Association Release Failed: %s", DimseCondition::dump(errorString, cond).c_str());
            return;
        }
    }
    else if (cond == DUL_PEERREQUESTEDRELEASE)
    {
        cond = ASC_abortAssociation(pAssociation);
        if (cond.bad())
        {
            LogUtil::Error(CODE_LOCATION, "Association Abort Failed: %s", DimseCondition::dump(errorString, cond).c_str());
            return;
        }
    }
    else if (cond == DUL_PEERABORTEDASSOCIATION)
    {
        LogUtil::Info(CODE_LOCATION, "Peer Aborted Association -----");
    }
    else
    {
        LogUtil::Error(CODE_LOCATION, "Echo SCU Failed: %s", DimseCondition::dump(errorString, cond).c_str());
        LogUtil::Info(CODE_LOCATION, "Aborting Association ...");
        cond = ASC_abortAssociation(pAssociation);
        if (cond.bad())
        {
            LogUtil::Error(CODE_LOCATION, "Association Abort Failed: %s", DimseCondition::dump(errorString, cond).c_str());
            return;
        }
    }

    // 清理网络环境
    cond = ASC_destroyAssociation(&pAssociation);
    if (cond.bad())
    {
        LogUtil::Error(CODE_LOCATION, "Error: %s", DimseCondition::dump(errorString, cond).c_str());
        return;
    }
    cond = ASC_dropNetwork(&pNetwork);
    if (cond.bad())
    {
        LogUtil::Error(CODE_LOCATION, "Error: %s", DimseCondition::dump(errorString, cond).c_str());
        return;
    }
}

void DicomServerBrowserWidget::on_btnFindTest_clicked()
{
    OFString errorString;
    int timeOut = 30;
    unsigned int dcmServerPort = 5678;
    OFString dcmServerHost = "localhost:5678";
    OFString ourTitle = APPLICATIONTITLE;
    OFString dcmServerTitle = PEERAPPLICATIONTITLE;
    //OFString abstractSyntax = UID_FINDModalityWorklistInformationModel;
    //OFString abstractSyntax = UID_FINDPatientRootQueryRetrieveInformationModel;
    OFString abstractSyntax = UID_FINDStudyRootQueryRetrieveInformationModel;
    E_TransferSyntax transferSyntax = EXS_Unknown;
    T_DIMSE_BlockingMode  blockMode = DIMSE_BLOCKING;
    DcmFindSCUExtractMode extractMode = FEM_none;
    OFString              extractXMLFilename;
    OFString              outputDirectory = ".";
    OFCmdUnsignedInt      maxReceivePDULength = ASC_DEFAULTMAXPDU; // 32768 Conquest DICOM Server
    OFCmdSignedInt        cancelAfterNResponses = -1;
    OFBool                secureConnection = OFFalse;
    OFBool                abortAssociation = OFFalse;

    OFList<OFString>      fileNameList;

    OFList<OFString>      overrideKeys;
    overrideKeys.push_back("PatientName=HEAD EXP2");
    overrideKeys.push_back("QueryRetrieveLevel=STUDY");
    overrideKeys.push_back("InstanceNumber");
    overrideKeys.push_back("PatientID");
    overrideKeys.push_back("StudyInstanceUID");
    overrideKeys.push_back("SeriesInstanceUID");
    overrideKeys.push_back("SeriesNumber");
    overrideKeys.push_back("NumberOfStudyRelatedInstances");


    DcmFindSCU dcmFindScu;
    OFCondition condition = dcmFindScu.initializeNetwork(timeOut);
    if (condition.bad())
    {
        LogUtil::Error(CODE_LOCATION, "Error: %s", DimseCondition::dump(errorString, condition).c_str());
        return;
    }

    dcmFindScu.performQuery(dcmServerHost.c_str(),
                            dcmServerPort,
                            ourTitle.c_str(),
                            dcmServerTitle.c_str(),
                            abstractSyntax.c_str(),
                            transferSyntax,
                            blockMode,
                            timeOut,
                            maxReceivePDULength,
                            secureConnection,
                            abortAssociation,
                            1,
                            extractMode,
                            cancelAfterNResponses,
                            &overrideKeys,
                            NULL,
                            &fileNameList,
                            outputDirectory.c_str(),
                            extractXMLFilename.c_str());

    condition = dcmFindScu.dropNetwork();
    if (condition.bad())
    {
        LogUtil::Error(CODE_LOCATION, "Error: %s", DimseCondition::dump(errorString, condition).c_str());
        return;
    }
}

void DicomServerBrowserWidget::on_btnGetTest_clicked()
{
}

void DicomServerBrowserWidget::on_btnMoveTest_clicked()
{
}

void DicomServerBrowserWidget::on_btnStoreTest_clicked()
{
}
