#include "DicomNetworkTest.h"

#include <fstream>
#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmnet/dimse.h>
#include <dcmtk/dcmnet/diutil.h>
#include <dcmtk/dcmtls/tlsopt.h>

#include <LogUtil.h>

static char rcsid[] = "$dcmtk: DicomNetworkTest v" OFFIS_DCMTK_VERSION " " OFFIS_DCMTK_RELEASEDATE " $";

/* default application titles */
#define APPLICATIONTITLE     "ECHOSCU"
#define PEERAPPLICATIONTITLE "ANY-SCP"

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

DicomNetworkTest::DicomNetworkTest()
{
}

DicomNetworkTest::~DicomNetworkTest()
{
}

void DicomNetworkTest::TestEnter()
{
    this->EchoSCU();
}

void DicomNetworkTest::TestExit()
{
    ;
}

void DicomNetworkTest::EchoSCU()
{
    LogUtil::Debug(CODE_LOCATION, "----------------- EchoSCU -----------------");
    LogUtil::Info(CODE_LOCATION, "----------------- %s -----------------", rcsid);

    DcmTLSOptions         tlsOptions(NET_REQUESTOR);
    T_ASC_Network        *pNetwork = nullptr;
    T_ASC_Parameters     *pAscParams = nullptr;
    T_ASC_Association    *pAssociation = nullptr;
    T_DIMSE_BlockingMode  blockMode = DIMSE_BLOCKING; // DIMSE_BLOCKING or DIMSE_NONBLOCKING
    OFString              errorString;
    OFCmdUnsignedInt      numXferSyntaxes = 1;

    // 初始化网络环境
    OFStandard::initializeNetwork();
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
    cond = ASC_setPresentationAddresses(pAscParams, OFStandard::getHostName().c_str(), "localhost:12345");
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
    LogUtil::Debug(CODE_LOCATION, "Association Parameters below:\n%s", errorString.c_str());

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
    LogUtil::Debug(CODE_LOCATION, "%d parameters are accepted ...");

    cond = EC_Normal;
    int numRepeat = 1;
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
    OFStandard::shutdownNetwork();
}
