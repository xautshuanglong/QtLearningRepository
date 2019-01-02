#include "DicomTest.h"

// DCMTK Headers
#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmnet/dimse.h>
#include <dcmtk/dcmnet/diutil.h>
#include <dcmtk/dcmnet/dfindscu.h>
#include <dcmtk/dcmnet/scu.h>
#include <dcmtk/dcmdata/dcpath.h>
#include <dcmtk/dcmtls/tlsopt.h>
#include <dcmtk/oflog/oflog.h>
#ifdef WITH_OPENSSL
#include "dcmtk/dcmtls/tlstrans.h"
#include "dcmtk/dcmtls/tlslayer.h"
#endif

#include <LogUtil.h>

static char rcsid[] = "$dcmtk: DicomNetworkTest v" OFFIS_DCMTK_VERSION " " OFFIS_DCMTK_RELEASEDATE " $";

/* default application titles */
#define APPLICATIONTITLE     "DebugTest"
#define PEERAPPLICATIONTITLE "MGIUSDICOM"

#define OFFIS_CONSOLE_APPLICATION "QtStoreTest"

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

static void GetTest_PrepareTS(E_TransferSyntax ts, OFList<OFString>& syntaxes);
static void GetTest_ApplyOverrideKeys(DcmDataset *dataset, const OFList<OFString> &overrideKeys);

static OFBool opt_showPresentationContexts = OFFalse;
static OFBool opt_abortAssociation = OFFalse;
static OFCmdUnsignedInt opt_maxReceivePDULength = ASC_DEFAULTMAXPDU;
static OFCmdUnsignedInt opt_maxSendPDULength = 0;
static E_TransferSyntax opt_networkTransferSyntax = EXS_Unknown;
static E_FileReadMode opt_readMode = ERM_autoDetect;
static OFBool opt_scanDir = OFFalse;
static OFBool opt_recurse = OFFalse;
static const char *opt_scanPattern = "";
static OFBool opt_haltOnUnsuccessfulStore = OFTrue;
static OFBool unsuccessfulStoreEncountered = OFFalse;
static int lastStatusCode = STATUS_Success;
static OFBool opt_proposeOnlyRequiredPresentationContexts = OFFalse;
static OFBool opt_combineProposedTransferSyntaxes = OFFalse;
static OFCmdUnsignedInt opt_repeatCount = 1;
static OFCmdUnsignedInt opt_inventPatientCount = 25;
static OFCmdUnsignedInt opt_inventStudyCount = 50;
static OFCmdUnsignedInt opt_inventSeriesCount = 100;
static OFBool opt_inventSOPInstanceInformation = OFFalse;
static OFBool opt_correctUIDPadding = OFFalse;
static OFString patientNamePrefix("OFFIS^TEST_PN_");   // PatientName is PN (maximum 16 chars)
static OFString patientIDPrefix("PID_"); // PatientID is LO (maximum 64 chars)
static OFString studyIDPrefix("SID_");   // StudyID is SH (maximum 16 chars)
static OFString accessionNumberPrefix;   // AccessionNumber is SH (maximum 16 chars)
static OFBool opt_secureConnection = OFFalse; /* default: no secure connection */
static const char *opt_configFile = NULL;
static const char *opt_profileName = NULL;
T_DIMSE_BlockingMode opt_blockMode = DIMSE_BLOCKING;
int opt_dimse_timeout = 0;
int opt_acse_timeout = 30;
OFCmdSignedInt opt_socket_timeout = 60;

#ifdef WITH_ZLIB
static OFCmdUnsignedInt opt_compressionLevel = 0;
#endif
#ifdef WITH_OPENSSL
static int         opt_keyFileFormat = SSL_FILETYPE_PEM;
static OFBool      opt_doAuthenticate = OFFalse;
static const char *opt_privateKeyFile = NULL;
static const char *opt_certificateFile = NULL;
static const char *opt_passwd = NULL;
#if OPENSSL_VERSION_NUMBER >= 0x0090700fL
static OFString    opt_ciphersuites(TLS1_TXT_RSA_WITH_AES_128_SHA ":" SSL3_TXT_RSA_DES_192_CBC3_SHA);
#else
static OFString    opt_ciphersuites(SSL3_TXT_RSA_DES_192_CBC3_SHA);
#endif
static const char *opt_readSeedFile = NULL;
static const char *opt_writeSeedFile = NULL;
static DcmCertificateVerification opt_certVerification = DCV_requireCertificate;
static const char *opt_dhparam = NULL;
#endif

// User Identity Negotiation
static T_ASC_UserIdentityNegotiationMode opt_identMode = ASC_USER_IDENTITY_NONE;
static OFString opt_user;
static OFString opt_password;
static OFString opt_identFile;
static OFBool opt_identResponse = OFFalse;

static OFLogger storescuLogger = OFLog::getLogger("dcmtk.apps." OFFIS_CONSOLE_APPLICATION);

static void StoreTest_ProgressCallback(void *callbackData, T_DIMSE_StoreProgress *progress, T_DIMSE_C_StoreRQ * req);
static OFCondition StoreTest_AddStoragePresentationContexts(T_ASC_Parameters *params, OFList<OFString> &sopClasses);
static OFCondition StoreTest_CStore(T_ASC_Association *assoc, const OFString &fname);
static OFBool StoreTest_FindSOPClassAndInstanceInFile(const char *fname, char *sopClass, char *sopInstance);
static OFCondition StoreTest_ConfigureUserIdentityRequest(T_ASC_Parameters *params);
static OFCondition StoreTest_CheckUserIdentityResponse(T_ASC_Parameters *params);

DicomTest::DicomTest(QObject *parent /* = Q_NULLPTR */)
    : QObject(parent)
{
}

DicomTest::~DicomTest()
{
}

void DicomTest::DcmtkTestEcho()
{
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

void DicomTest::DcmtkTestFind()
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

void DicomTest::DcmtkTestGet()
{
    OFString                errormsg;
    OFCmdUnsignedInt        maxPDU = 32768; // ASC_DEFAULTMAXPDU;
    E_TransferSyntax        storeTransferSyntax = EXS_Unknown;
    E_TransferSyntax        getTransferSyntax = EXS_Unknown;
    DcmStorageMode          storageMode = DCMSCU_STORAGE_DISK;
    OFBool                  showPresentationContexts = OFFalse;
    OFBool                  abortAssociation = OFFalse;
    OFCmdUnsignedInt        repeatCount = 1;
    OFCmdUnsignedInt        dcmServerPort = 5678;
    T_DIMSE_BlockingMode    blockMode = DIMSE_BLOCKING;
    int                     dimseTimeout = 0;
    int                     acseTimeout = 30;
    OFString                outputDirectory = ".";
    OFList<OFString>        fileNameList;
    OFList<OFString>        overrideKeys;

    overrideKeys.push_back("PatientID=KHIS001");

    OFList<OFString> syntaxes;
    GetTest_PrepareTS(getTransferSyntax, syntaxes);

    DcmSCU getSCU;
    getSCU.setMaxReceivePDULength(maxPDU);
    getSCU.setACSETimeout(acseTimeout);
    getSCU.setDIMSEBlockingMode(blockMode);
    getSCU.setDIMSETimeout(dimseTimeout);
    getSCU.setAETitle(APPLICATIONTITLE);
    getSCU.setPeerHostName("localhost");
    getSCU.setPeerPort(OFstatic_cast(Uint16, dcmServerPort));
    getSCU.setPeerAETitle(PEERAPPLICATIONTITLE);
    getSCU.setVerbosePCMode(showPresentationContexts);
    getSCU.addPresentationContext(UID_GETPatientRootQueryRetrieveInformationModel, syntaxes);

    syntaxes.clear();
    GetTest_PrepareTS(storeTransferSyntax, syntaxes);
    for (Uint16 j = 0; j < numberOfDcmLongSCUStorageSOPClassUIDs; j++)
    {
        getSCU.addPresentationContext(dcmLongSCUStorageSOPClassUIDs[j], syntaxes, ASC_SC_ROLE_SCP);
    }

    getSCU.setStorageMode(storageMode);
    if (storageMode != DCMSCU_STORAGE_IGNORE)
    {
        getSCU.setStorageDir(outputDirectory);
    }

    /* initialize network and negotiate association */
    OFCondition cond = getSCU.initNetwork();
    if (cond.bad())
    {
        LogUtil::Error(CODE_LOCATION, "Error: %s", DimseCondition::dump(errormsg, cond));
        return;
    }
    cond = getSCU.negotiateAssociation();
    if (cond.bad())
    {
        LogUtil::Error(CODE_LOCATION, "No Acceptable Presentation Contexts");
        return;
    }
    cond = EC_Normal;
    T_ASC_PresentationContextID pcid = getSCU.findPresentationContextID(UID_GETPatientRootQueryRetrieveInformationModel, "");
    if (pcid == 0)
    {
        LogUtil::Error(CODE_LOCATION, "No adequate Presentation Contexts for sending C-GET");
        return;
    }

    for (Uint16 repeat = 0; repeat < repeatCount; repeat++)
    {
        size_t numRuns = 1;
        DcmFileFormat dcmff;
        DcmDataset *dset = dcmff.getDataset();
        OFListConstIterator(OFString) it;
        if (!fileNameList.empty())
        {
            numRuns = fileNameList.size();
            it = fileNameList.begin();
            cond = dcmff.loadFile((*it).c_str());
            if (cond.bad())
            {
                LogUtil::Error(CODE_LOCATION, "Error: %s", DimseCondition::dump(errormsg, cond));
                return;
            }
            dset = dcmff.getDataset();
        }
        OFList<RetrieveResponse*> responses;
        for (Uint16 i = 0; i < numRuns; i++)
        {
            GetTest_ApplyOverrideKeys(dset, overrideKeys);
            cond = getSCU.sendCGETRequest(pcid, dset, &responses);
            if (cond.bad())
            {
                return;
            }
            if (numRuns > 1)
            {
                it++;
                cond = dcmff.loadFile((*it).c_str());
                if (cond.bad())
                {
                    LogUtil::Error(CODE_LOCATION, "Error: %s", DimseCondition::dump(errormsg, cond).c_str());
                    return;
                }
                dset = dcmff.getDataset();
            }
        }
        if (!responses.empty())
        {
            LogUtil::Debug(CODE_LOCATION, "Final status report from last C-GET message:");
            (*(--responses.end()))->print();
            OFListIterator(RetrieveResponse*) iter = responses.begin();
            OFListConstIterator(RetrieveResponse*) last = responses.end();
            while (iter != last)
            {
                delete (*iter);
                iter = responses.erase(iter);
            }
        }
    }

    if (cond == EC_Normal)
    {
        if (abortAssociation)
            getSCU.abortAssociation();
        else
            getSCU.releaseAssociation();
    }
    else
    {
        if (cond == DUL_PEERREQUESTEDRELEASE)
            getSCU.closeAssociation(DCMSCU_PEER_REQUESTED_RELEASE);
        else if (cond == DUL_PEERABORTEDASSOCIATION)
            getSCU.closeAssociation(DCMSCU_PEER_ABORTED_ASSOCIATION);
        else
        {
            LogUtil::Error(CODE_LOCATION, "Get SCU Failed: %s", DimseCondition::dump(errormsg, cond).c_str());
            getSCU.abortAssociation();
        }
    }
}

void DicomTest::DcmtkTestMove()
{
}

void DicomTest::DcmtkTestStore()
{
    OFCondition cond = EC_Normal;
    const int repeatCount = 1;
    OFOStringStream optStream;

    const char *opt_peer = NULL;
    OFCmdUnsignedInt opt_port = 104;
    const char *opt_peerTitle = PEERAPPLICATIONTITLE;
    const char *opt_ourTitle = APPLICATIONTITLE;

    OFList<OFString> fileNameList;
    OFList<OFString> sopClassUIDList;
    OFList<OFString> sopInstanceUIDList;

    OFString temp_str;
    OFConsoleApplication app(OFFIS_CONSOLE_APPLICATION, "DICOM storage (C-STORE) SCU", rcsid);
    OFCommandLine cmd;

    T_ASC_Network *net;
    T_ASC_Parameters *params;
    DIC_NODENAME peerHost;
    T_ASC_Association *assoc;
    DcmAssociationConfiguration asccfg;

    cond = ASC_initializeNetwork(NET_REQUESTOR, 0, opt_acse_timeout, &net);
    if (cond.bad())
    {
        LogUtil::Error(CODE_LOCATION, "%s", DimseCondition::dump(temp_str, cond).c_str());
    }

#if 0 // #ifdef WITH_OPENSSL

    DcmTLSTransportLayer *tLayer = NULL;
    if (opt_secureConnection)
    {
        tLayer = new DcmTLSTransportLayer(DICOM_APPLICATION_REQUESTOR, opt_readSeedFile);
        if (tLayer == NULL)
        {
            OFLOG_FATAL(storescuLogger, "unable to create TLS transport layer");
            exit(1);
        }

        if (cmd.findOption("--add-cert-file", 0, OFCommandLine::FOM_First))
        {
            const char *current = NULL;
            do
            {
                app.checkValue(cmd.getValue(current));
                if (TCS_ok != tLayer->addTrustedCertificateFile(current, opt_keyFileFormat))
                    OFLOG_WARN(storescuLogger, "unable to load certificate file '" << current << "', ignoring");
            } while (cmd.findOption("--add-cert-file", 0, OFCommandLine::FOM_Next));
        }

        if (cmd.findOption("--add-cert-dir", 0, OFCommandLine::FOM_First))
        {
            const char *current = NULL;
            do
            {
                app.checkValue(cmd.getValue(current));
                if (TCS_ok != tLayer->addTrustedCertificateDir(current, opt_keyFileFormat))
                    OFLOG_WARN(storescuLogger, "unable to load certificates from directory '" << current << "', ignoring");
            } while (cmd.findOption("--add-cert-dir", 0, OFCommandLine::FOM_Next));
        }

        if (opt_dhparam && !(tLayer->setTempDHParameters(opt_dhparam)))
            OFLOG_WARN(storescuLogger, "unable to load temporary DH parameter file '" << opt_dhparam << "', ignoring");

        if (opt_doAuthenticate)
        {
            if (opt_passwd) tLayer->setPrivateKeyPasswd(opt_passwd);

            if (TCS_ok != tLayer->setPrivateKeyFile(opt_privateKeyFile, opt_keyFileFormat))
            {
                OFLOG_ERROR(storescuLogger, "unable to load private TLS key from '" << opt_privateKeyFile << "'");
                return;
            }
            if (TCS_ok != tLayer->setCertificateFile(opt_certificateFile, opt_keyFileFormat))
            {
                OFLOG_ERROR(storescuLogger, "unable to load certificate from '" << opt_certificateFile << "'");
                return;
            }
            if (!tLayer->checkPrivateKeyMatchesCertificate())
            {
                OFLOG_ERROR(storescuLogger, "private key '" << opt_privateKeyFile << "' and certificate '" << opt_certificateFile << "' do not match");
                return;
            }
        }

        if (TCS_ok != tLayer->setCipherSuites(opt_ciphersuites.c_str()))
        {
            OFLOG_ERROR(storescuLogger, "unable to set selected cipher suites");
            return;
        }

        tLayer->setCertificateVerification(opt_certVerification);


        cond = ASC_setTransportLayer(net, tLayer, 0);
        if (cond.bad())
        {
            OFLOG_FATAL(storescuLogger, DimseCondition::dump(temp_str, cond));
            return;
        }
    }

#endif

    /* initialize asscociation parameters, i.e. create an instance of T_ASC_Parameters*. */
    cond = ASC_createAssociationParameters(&params, opt_maxReceivePDULength);
    if (cond.bad())
    {
        OFLOG_FATAL(storescuLogger, DimseCondition::dump(temp_str, cond));
        return;
    }
    /* sets this application's title and the called application's title in the params */
    /* structure. The default values to be set here are "STORESCU" and "ANY-SCP". */
    ASC_setAPTitles(params, opt_ourTitle, opt_peerTitle, NULL);

    /* Set the transport layer type (type of network connection) in the params */
    /* strucutre. The default is an insecure connection; where OpenSSL is  */
    /* available the user is able to request an encrypted,secure connection. */
    cond = ASC_setTransportLayerType(params, opt_secureConnection);
    if (cond.bad()) {
        OFLOG_FATAL(storescuLogger, DimseCondition::dump(temp_str, cond));
        return;
    }

    /* Figure out the presentation addresses and copy the */
    /* corresponding values into the association parameters.*/
    sprintf(peerHost, "%s:%d", opt_peer, OFstatic_cast(int, opt_port));
    ASC_setPresentationAddresses(params, OFStandard::getHostName().c_str(), peerHost);

    /* Configure User Identity Negotiation*/
    if (opt_identMode != ASC_USER_IDENTITY_NONE)
    {
        cond = StoreTest_ConfigureUserIdentityRequest(params);
        if (cond.bad())
            return;
    }

    if (opt_profileName)
    {
        /* perform name mangling for config file key */
        OFString sprofile;
        const unsigned char *c = OFreinterpret_cast(const unsigned char *, opt_profileName);
        while (*c)
        {
            if (!isspace(*c)) sprofile += OFstatic_cast(char, toupper(*c));
            ++c;
        }

        /* set presentation contexts as defined in config file */
        cond = asccfg.setAssociationParameters(sprofile.c_str(), *params);
    }
    else
    {
        /* Set the presentation contexts which will be negotiated */
        /* when the network connection will be established */
        cond = StoreTest_AddStoragePresentationContexts(params, sopClassUIDList);
    }

    if (cond.bad()) {
        OFLOG_FATAL(storescuLogger, DimseCondition::dump(temp_str, cond));
        return;
    }

    /* dump presentation contexts if required */
    if (opt_showPresentationContexts)
        OFLOG_INFO(storescuLogger, "Request Parameters:" << OFendl << ASC_dumpParameters(temp_str, params, ASC_ASSOC_RQ));
    else
        OFLOG_DEBUG(storescuLogger, "Request Parameters:" << OFendl << ASC_dumpParameters(temp_str, params, ASC_ASSOC_RQ));

    /* create association, i.e. try to establish a network connection to another */
    /* DICOM application. This call creates an instance of T_ASC_Association*. */
    OFLOG_INFO(storescuLogger, "Requesting Association");
    cond = ASC_requestAssociation(net, params, &assoc);
    if (cond.bad()) {
        if (cond == DUL_ASSOCIATIONREJECTED) {
            T_ASC_RejectParameters rej;

            ASC_getRejectParameters(params, &rej);
            OFLOG_FATAL(storescuLogger, "Association Rejected:" << OFendl << ASC_printRejectParameters(temp_str, &rej));
            return;
        }
        else {
            OFLOG_FATAL(storescuLogger, "Association Request Failed: " << DimseCondition::dump(temp_str, cond));
            return;
        }
    }

    /* dump the connection parameters if in debug mode*/
    OFLOG_DEBUG(storescuLogger, ASC_dumpConnectionParameters(temp_str, assoc));

    /* dump the presentation contexts which have been accepted/refused */
    if (opt_showPresentationContexts)
        OFLOG_INFO(storescuLogger, "Association Parameters Negotiated:" << OFendl << ASC_dumpParameters(temp_str, params, ASC_ASSOC_AC));
    else
        OFLOG_DEBUG(storescuLogger, "Association Parameters Negotiated:" << OFendl << ASC_dumpParameters(temp_str, params, ASC_ASSOC_AC));

    /* count the presentation contexts which have been accepted by the SCP */
    /* If there are none, finish the execution */
    if (ASC_countAcceptedPresentationContexts(params) == 0) {
        OFLOG_FATAL(storescuLogger, "No Acceptable Presentation Contexts");
        return;
    }

    /* check user authentication response (if applicable) */
    cond = StoreTest_CheckUserIdentityResponse(params);
    if (cond.bad())
    {
        OFLOG_FATAL(storescuLogger, DimseCondition::dump(temp_str, cond));
        return;
    }
    /* dump general information concerning the establishment of the network connection if required */
    OFLOG_INFO(storescuLogger, "Association Accepted (Max Send PDV: " << assoc->sendPDVLength << ")");

    /* do the real work, i.e. for all files which were specified in the */
    /* command line, transmit the encapsulated DICOM objects to the SCP. */
    cond = EC_Normal;
    OFListIterator(OFString) iter = fileNameList.begin();
    OFListIterator(OFString) enditer = fileNameList.end();

    while ((iter != enditer) && cond.good())
    {
        cond = StoreTest_CStore(assoc, *iter);
        ++iter;
    }

    /* tear down association, i.e. terminate network connection to SCP */
    if (cond == EC_Normal)
    {
        if (opt_abortAssociation) {
            OFLOG_INFO(storescuLogger, "Aborting Association");
            cond = ASC_abortAssociation(assoc);
            if (cond.bad()) {
                OFLOG_ERROR(storescuLogger, "Association Abort Failed: " << DimseCondition::dump(temp_str, cond));
                return;
            }
        }
        else {
            /* release association */
            OFLOG_INFO(storescuLogger, "Releasing Association");
            cond = ASC_releaseAssociation(assoc);
            if (cond.bad())
            {
                OFLOG_ERROR(storescuLogger, "Association Release Failed: " << DimseCondition::dump(temp_str, cond));
                return;
            }
        }
    }
    else if (cond == DUL_PEERREQUESTEDRELEASE)
    {
        OFLOG_ERROR(storescuLogger, "Protocol Error: Peer requested release (Aborting)");
        OFLOG_INFO(storescuLogger, "Aborting Association");
        cond = ASC_abortAssociation(assoc);
        if (cond.bad()) {
            OFLOG_ERROR(storescuLogger, "Association Abort Failed: " << DimseCondition::dump(temp_str, cond));
            return;
        }
    }
    else if (cond == DUL_PEERABORTEDASSOCIATION)
    {
        OFLOG_INFO(storescuLogger, "Peer Aborted Association");
    }
    else
    {
        OFLOG_ERROR(storescuLogger, "Store SCU Failed: " << DimseCondition::dump(temp_str, cond));
        OFLOG_INFO(storescuLogger, "Aborting Association");
        cond = ASC_abortAssociation(assoc);
        if (cond.bad()) {
            OFLOG_ERROR(storescuLogger, "Association Abort Failed: " << DimseCondition::dump(temp_str, cond));
            return;
        }
    }

    /* destroy the association, i.e. free memory of T_ASC_Association* structure. This */
    /* call is the counterpart of ASC_requestAssociation(...) which was called above. */
    cond = ASC_destroyAssociation(&assoc);
    if (cond.bad()) {
        OFLOG_FATAL(storescuLogger, DimseCondition::dump(temp_str, cond));
        return;
    }
    /* drop the network, i.e. free memory of T_ASC_Network* structure. This call */
    /* is the counterpart of ASC_initializeNetwork(...) which was called above. */
    cond = ASC_dropNetwork(&net);
    if (cond.bad())
    {
        LogUtil::Error(CODE_LOCATION, "%s", DimseCondition::dump(temp_str, cond).c_str());
    }

#if 0 // #ifdef WITH_OPENSSL
    if (tLayer && opt_writeSeedFile)
    {
        if (tLayer->canWriteRandomSeed())
        {
            if (!tLayer->writeRandomSeed(opt_writeSeedFile))
                OFLOG_WARN(storescuLogger, "cannot write random seed file '" << opt_writeSeedFile << "', ignoring");
        }
        else
            OFLOG_WARN(storescuLogger, "cannot write random seed, ignoring");
    }
    delete tLayer;
#endif

#ifdef _DEBUG
    dcmDataDict.clear();  /* useful for debugging with dmalloc */
#endif
}

static void GetTest_PrepareTS(E_TransferSyntax ts, OFList<OFString>& syntaxes)
{
    switch (ts)
    {
    case EXS_LittleEndianImplicit:
        syntaxes.push_back(UID_LittleEndianExplicitTransferSyntax);
        break;
    case EXS_LittleEndianExplicit:
        syntaxes.push_back(UID_LittleEndianExplicitTransferSyntax);
        syntaxes.push_back(UID_BigEndianExplicitTransferSyntax);
        syntaxes.push_back(UID_LittleEndianImplicitTransferSyntax);
        break;
    case EXS_BigEndianExplicit:
        syntaxes.push_back(UID_BigEndianExplicitTransferSyntax);
        syntaxes.push_back(UID_LittleEndianExplicitTransferSyntax);
        syntaxes.push_back(UID_LittleEndianImplicitTransferSyntax);
        break;
#ifdef WITH_ZLIB
    case EXS_DeflatedLittleEndianExplicit:
        /* we prefer Deflated Little Endian Explicit */
        syntaxes.push_back(UID_DeflatedExplicitVRLittleEndianTransferSyntax);
        syntaxes.push_back(UID_LittleEndianExplicitTransferSyntax);
        syntaxes.push_back(UID_BigEndianExplicitTransferSyntax);
        syntaxes.push_back(UID_LittleEndianImplicitTransferSyntax);
        break;
#endif
    default:
        DcmXfer xfer(ts);
        if (xfer.isEncapsulated())
        {
            syntaxes.push_back(xfer.getXferID());
        }
        if (gLocalByteOrder == EBO_LittleEndian)
        {
            syntaxes.push_back(UID_LittleEndianExplicitTransferSyntax);
            syntaxes.push_back(UID_BigEndianExplicitTransferSyntax);
        }
        else
        {
            syntaxes.push_back(UID_BigEndianExplicitTransferSyntax);
            syntaxes.push_back(UID_LittleEndianExplicitTransferSyntax);
        }
        syntaxes.push_back(UID_LittleEndianImplicitTransferSyntax);
        break;
    }
}

static void GetTest_ApplyOverrideKeys(DcmDataset *dataset, const OFList<OFString> &overrideKeys)
{
    /* replace specific keys by those in overrideKeys */
    OFListConstIterator(OFString) path = overrideKeys.begin();
    OFListConstIterator(OFString) endOfList = overrideKeys.end();
    DcmPathProcessor proc;
    proc.setItemWildcardSupport(OFFalse);
    proc.checkPrivateReservations(OFFalse);
    OFCondition cond;
    while (path != endOfList)
    {
        cond = proc.applyPathWithValue(dataset, *path);
        if (cond.bad())
        {
            LogUtil::Error(CODE_LOCATION, "Bad override key/path: %s %s", (*path).c_str(), cond.text());
        }
        path++;
    }
}

static void StoreTest_ProgressCallback(void *callbackData, T_DIMSE_StoreProgress *progress, T_DIMSE_C_StoreRQ * req)
{
    if (progress->state == DIMSE_StoreBegin)
    {
        OFString str;
        LogUtil::Debug(CODE_LOCATION, "%s", DIMSE_dumpMessage(str, *req, DIMSE_OUTGOING).c_str());
    }

    switch (progress->state)
    {
    case DIMSE_StoreBegin:
        LogUtil::Debug(CODE_LOCATION, "StoreBegin ========================");
        break;
    case DIMSE_StoreProgressing:
        LogUtil::Debug(CODE_LOCATION, "StoreProgressing: %d / %d", progress->progressBytes, progress->totalBytes);
        break;
    case DIMSE_StoreEnd:
        LogUtil::Debug(CODE_LOCATION, "StoreEnd ========================");
        break;
    default:
        break;
    }
}

static OFBool isaListMember(OFList<OFString> &lst, OFString &s)
{
    OFListIterator(OFString) cur = lst.begin();
    OFListIterator(OFString) end = lst.end();

    OFBool found = OFFalse;
    while (cur != end && !found) {
        found = (s == *cur);
        ++cur;
    }

    return found;
}

static OFCondition addPresentationContext(T_ASC_Parameters *params,
                                          int presentationContextId,
                                          const OFString &abstractSyntax,
                                          const OFString &transferSyntax,
                                          T_ASC_SC_ROLE proposedRole = ASC_SC_ROLE_DEFAULT)
{
    const char *c_p = transferSyntax.c_str();
    OFCondition cond = ASC_addPresentationContext(params, presentationContextId,
                                                  abstractSyntax.c_str(), &c_p, 1, proposedRole);
    return cond;
}

static OFCondition addPresentationContext(T_ASC_Parameters *params,
                                          int presentationContextId,
                                          const OFString &abstractSyntax,
                                          const OFList<OFString> &transferSyntaxList,
                                          T_ASC_SC_ROLE proposedRole = ASC_SC_ROLE_DEFAULT)
{
    // create an array of supported/possible transfer syntaxes
    const char **transferSyntaxes = new const char*[transferSyntaxList.size()];
    int transferSyntaxCount = 0;
    OFListConstIterator(OFString) s_cur = transferSyntaxList.begin();
    OFListConstIterator(OFString) s_end = transferSyntaxList.end();
    while (s_cur != s_end) {
        transferSyntaxes[transferSyntaxCount++] = (*s_cur).c_str();
        ++s_cur;
    }

    OFCondition cond = ASC_addPresentationContext(params, presentationContextId,
                                                  abstractSyntax.c_str(), transferSyntaxes, transferSyntaxCount, proposedRole);

    delete[] transferSyntaxes;
    return cond;
}

static OFCondition StoreTest_AddStoragePresentationContexts(T_ASC_Parameters *params, OFList<OFString> &sopClasses)
{
    /*
    * Each SOP Class will be proposed in two presentation contexts (unless
    * the opt_combineProposedTransferSyntaxes global variable is true).
    * The command line specified a preferred transfer syntax to use.
    * This prefered transfer syntax will be proposed in one
    * presentation context and a set of alternative (fallback) transfer
    * syntaxes will be proposed in a different presentation context.
    *
    * Generally, we prefer to use Explicitly encoded transfer syntaxes
    * and if running on a Little Endian machine we prefer
    * LittleEndianExplicitTransferSyntax to BigEndianTransferSyntax.
    * Some SCP implementations will just select the first transfer
    * syntax they support (this is not part of the standard) so
    * organise the proposed transfer syntaxes to take advantage
    * of such behaviour.
    */

    // Which transfer syntax was preferred on the command line
    OFString preferredTransferSyntax;
    if (opt_networkTransferSyntax == EXS_Unknown) {
        /* gLocalByteOrder is defined in dcxfer.h */
        if (gLocalByteOrder == EBO_LittleEndian) {
            /* we are on a little endian machine */
            preferredTransferSyntax = UID_LittleEndianExplicitTransferSyntax;
        }
        else {
            /* we are on a big endian machine */
            preferredTransferSyntax = UID_BigEndianExplicitTransferSyntax;
        }
    }
    else {
        DcmXfer xfer(opt_networkTransferSyntax);
        preferredTransferSyntax = xfer.getXferID();
    }

    OFListIterator(OFString) s_cur;
    OFListIterator(OFString) s_end;

    OFList<OFString> fallbackSyntaxes;
    // - If little endian implicit is preferred, we don't need any fallback syntaxes
    //   because it is the default transfer syntax and all applications must support it.
    // - If MPEG2, MPEG4 or HEVC is preferred, we don't want to propose any fallback solution
    //   because this is not required and we cannot decompress the movie anyway.
    if ((opt_networkTransferSyntax != EXS_LittleEndianImplicit) &&
        (opt_networkTransferSyntax != EXS_MPEG2MainProfileAtMainLevel) &&
        (opt_networkTransferSyntax != EXS_MPEG2MainProfileAtHighLevel) &&
        (opt_networkTransferSyntax != EXS_MPEG4HighProfileLevel4_1) &&
        (opt_networkTransferSyntax != EXS_MPEG4BDcompatibleHighProfileLevel4_1) &&
        (opt_networkTransferSyntax != EXS_MPEG4HighProfileLevel4_2_For2DVideo) &&
        (opt_networkTransferSyntax != EXS_MPEG4HighProfileLevel4_2_For3DVideo) &&
        (opt_networkTransferSyntax != EXS_MPEG4StereoHighProfileLevel4_2) &&
        (opt_networkTransferSyntax != EXS_HEVCMainProfileLevel5_1) &&
        (opt_networkTransferSyntax != EXS_HEVCMain10ProfileLevel5_1))
    {
        fallbackSyntaxes.push_back(UID_LittleEndianExplicitTransferSyntax);
        fallbackSyntaxes.push_back(UID_BigEndianExplicitTransferSyntax);
        fallbackSyntaxes.push_back(UID_LittleEndianImplicitTransferSyntax);
        // Remove the preferred syntax from the fallback list
        fallbackSyntaxes.remove(preferredTransferSyntax);
    }

    // create a list of transfer syntaxes combined from the preferred and fallback syntaxes
    OFList<OFString> combinedSyntaxes;
    s_cur = fallbackSyntaxes.begin();
    s_end = fallbackSyntaxes.end();
    combinedSyntaxes.push_back(preferredTransferSyntax);
    while (s_cur != s_end)
    {
        if (!isaListMember(combinedSyntaxes, *s_cur)) combinedSyntaxes.push_back(*s_cur);
        ++s_cur;
    }

    if (!opt_proposeOnlyRequiredPresentationContexts) {
        // add the (short list of) known storage SOP classes to the list
        // the array of Storage SOP Class UIDs comes from dcuid.h
        for (int i = 0; i < numberOfDcmShortSCUStorageSOPClassUIDs; i++)
            sopClasses.push_back(dcmShortSCUStorageSOPClassUIDs[i]);
    }

    // thin out the SOP classes to remove any duplicates
    OFList<OFString> sops;
    s_cur = sopClasses.begin();
    s_end = sopClasses.end();
    while (s_cur != s_end) {
        if (!isaListMember(sops, *s_cur)) {
            sops.push_back(*s_cur);
        }
        ++s_cur;
    }

    // add a presentations context for each SOP class / transfer syntax pair
    OFCondition cond = EC_Normal;
    int pid = 1; // presentation context id
    s_cur = sops.begin();
    s_end = sops.end();
    while (s_cur != s_end && cond.good()) {

        if (pid > 255) {
            OFLOG_ERROR(storescuLogger, "Too many presentation contexts");
            return ASC_BADPRESENTATIONCONTEXTID;
        }

        if (opt_combineProposedTransferSyntaxes) {
            cond = addPresentationContext(params, pid, *s_cur, combinedSyntaxes);
            pid += 2;   /* only odd presentation context id's */
        }
        else {

            // SOP class with preferred transfer syntax
            cond = addPresentationContext(params, pid, *s_cur, preferredTransferSyntax);
            pid += 2;   /* only odd presentation context id's */

            if (fallbackSyntaxes.size() > 0) {
                if (pid > 255) {
                    OFLOG_ERROR(storescuLogger, "Too many presentation contexts");
                    return ASC_BADPRESENTATIONCONTEXTID;
                }

                // SOP class with fallback transfer syntax
                cond = addPresentationContext(params, pid, *s_cur, fallbackSyntaxes);
                pid += 2; /* only odd presentation context id's */
            }
        }
        ++s_cur;
    }

    return cond;
}

static int
secondsSince1970()
{
    time_t t = time(NULL);
    return OFstatic_cast(int, t);
}

static OFString
intToString(int i)
{
    char numbuf[32];
    sprintf(numbuf, "%d", i);
    return numbuf;
}

static OFString makeUID(OFString basePrefix, int counter)
{
    OFString prefix = basePrefix + "." + intToString(counter);
    char uidbuf[65];
    OFString uid = dcmGenerateUniqueIdentifier(uidbuf, prefix.c_str());
    return uid;
}

static OFBool updateStringAttributeValue(DcmItem *dataset, const DcmTagKey &key, OFString &value)
{
    DcmStack stack;
    DcmTag tag(key);

    OFCondition cond = EC_Normal;
    cond = dataset->search(key, stack, ESM_fromHere, OFFalse);
    if (cond != EC_Normal) {
        OFLOG_ERROR(storescuLogger, "updateStringAttributeValue: cannot find: " << tag.getTagName()
                    << " " << key << ": " << cond.text());
        return OFFalse;
    }

    DcmElement *elem = OFstatic_cast(DcmElement *, stack.top());

    DcmVR vr(elem->ident());
    if (elem->getLength() > vr.getMaxValueLength()) {
        OFLOG_ERROR(storescuLogger, "updateStringAttributeValue: INTERNAL ERROR: " << tag.getTagName()
                    << " " << key << ": value too large (max " << vr.getMaxValueLength()
                    << ") for " << vr.getVRName() << " value: " << value);
        return OFFalse;
    }

    cond = elem->putOFStringArray(value);
    if (cond != EC_Normal) {
        OFLOG_ERROR(storescuLogger, "updateStringAttributeValue: cannot put string in attribute: " << tag.getTagName()
                    << " " << key << ": " << cond.text());
        return OFFalse;
    }

    return OFTrue;
}

static void StoreTest_ReplaceSOPInstanceInformation(DcmDataset *dataset)
{
    static OFCmdUnsignedInt patientCounter = 0;
    static OFCmdUnsignedInt studyCounter = 0;
    static OFCmdUnsignedInt seriesCounter = 0;
    static OFCmdUnsignedInt imageCounter = 0;
    static OFString seriesInstanceUID;
    static OFString seriesNumber;
    static OFString studyInstanceUID;
    static OFString studyID;
    static OFString accessionNumber;
    static OFString patientID;
    static OFString patientName;

    if (seriesInstanceUID.empty()) seriesInstanceUID = makeUID(SITE_SERIES_UID_ROOT, OFstatic_cast(int, seriesCounter));
    if (seriesNumber.empty()) seriesNumber = intToString(OFstatic_cast(int, seriesCounter));
    if (studyInstanceUID.empty()) studyInstanceUID = makeUID(SITE_STUDY_UID_ROOT, OFstatic_cast(int, studyCounter));
    if (studyID.empty()) studyID = studyIDPrefix + intToString(OFstatic_cast(int, secondsSince1970())) + intToString(OFstatic_cast(int, studyCounter));
    if (accessionNumber.empty()) accessionNumber = accessionNumberPrefix + intToString(secondsSince1970()) + intToString(OFstatic_cast(int, studyCounter));
    if (patientID.empty()) patientID = patientIDPrefix + intToString(secondsSince1970()) + intToString(OFstatic_cast(int, patientCounter));
    if (patientName.empty()) patientName = patientNamePrefix + intToString(secondsSince1970()) + intToString(OFstatic_cast(int, patientCounter));

    if (imageCounter >= opt_inventSeriesCount) {
        imageCounter = 0;
        seriesCounter++;
        seriesInstanceUID = makeUID(SITE_SERIES_UID_ROOT, OFstatic_cast(int, seriesCounter));
        seriesNumber = intToString(OFstatic_cast(int, seriesCounter));
    }
    if (seriesCounter >= opt_inventStudyCount) {
        seriesCounter = 0;
        studyCounter++;
        studyInstanceUID = makeUID(SITE_STUDY_UID_ROOT, OFstatic_cast(int, studyCounter));
        studyID = studyIDPrefix + intToString(secondsSince1970()) + intToString(OFstatic_cast(int, studyCounter));
        accessionNumber = accessionNumberPrefix + intToString(secondsSince1970()) + intToString(OFstatic_cast(int, studyCounter));
    }
    if (studyCounter >= opt_inventPatientCount) {
        // we create as many patients as necessary */
        studyCounter = 0;
        patientCounter++;
        patientID = patientIDPrefix + intToString(secondsSince1970()) + intToString(OFstatic_cast(int, patientCounter));
        patientName = patientNamePrefix + intToString(secondsSince1970()) + intToString(OFstatic_cast(int, patientCounter));
    }

    OFString sopInstanceUID = makeUID(SITE_INSTANCE_UID_ROOT, OFstatic_cast(int, imageCounter));
    OFString imageNumber = intToString(OFstatic_cast(int, imageCounter));

    OFLOG_INFO(storescuLogger, "Inventing Identifying Information ("
               << "pa" << patientCounter << ", st" << studyCounter
               << ", se" << seriesCounter << ", im" << imageCounter << "):");
    OFLOG_INFO(storescuLogger, "  PatientName=" << patientName);
    OFLOG_INFO(storescuLogger, "  PatientID=" << patientID);
    OFLOG_INFO(storescuLogger, "  StudyInstanceUID=" << studyInstanceUID);
    OFLOG_INFO(storescuLogger, "  StudyID=" << studyID);
    OFLOG_INFO(storescuLogger, "  SeriesInstanceUID=" << seriesInstanceUID);
    OFLOG_INFO(storescuLogger, "  SeriesNumber=" << seriesNumber);
    OFLOG_INFO(storescuLogger, "  SOPInstanceUID=" << sopInstanceUID);
    OFLOG_INFO(storescuLogger, "  ImageNumber=" << imageNumber);

    updateStringAttributeValue(dataset, DCM_PatientName, patientName);
    updateStringAttributeValue(dataset, DCM_PatientID, patientID);
    updateStringAttributeValue(dataset, DCM_StudyInstanceUID, studyInstanceUID);
    updateStringAttributeValue(dataset, DCM_StudyID, studyID);
    updateStringAttributeValue(dataset, DCM_SeriesInstanceUID, seriesInstanceUID);
    updateStringAttributeValue(dataset, DCM_SeriesNumber, seriesNumber);
    updateStringAttributeValue(dataset, DCM_SOPInstanceUID, sopInstanceUID);
    updateStringAttributeValue(dataset, DCM_InstanceNumber, imageNumber);

    imageCounter++;
}

static OFCondition StoreTest_StoreSCU(T_ASC_Association *assoc, const char *fname)
/*
* This function will read all the information from the given file,
* figure out a corresponding presentation context which will be used
* to transmit the information over the network to the SCP, and it
* will finally initiate the transmission of all data to the SCP.
*
* Parameters:
*   assoc - [in] The association (network connection to another DICOM application).
*   fname - [in] Name of the file which shall be processed.
*/
{
    DIC_US msgId = assoc->nextMsgID++;
    T_ASC_PresentationContextID presID;
    T_DIMSE_C_StoreRQ req;
    T_DIMSE_C_StoreRSP rsp;
    DIC_UI sopClass;
    DIC_UI sopInstance;
    DcmDataset *statusDetail = NULL;

    unsuccessfulStoreEncountered = OFTrue; // assumption

    OFLOG_INFO(storescuLogger, "Sending file: " << fname);

    /* read information from file. After the call to DcmFileFormat::loadFile(...) the information */
    /* which is encapsulated in the file will be available through the DcmFileFormat object. */
    /* In detail, it will be available through calls to DcmFileFormat::getMetaInfo() (for */
    /* meta header information) and DcmFileFormat::getDataset() (for data set information). */
    DcmFileFormat dcmff;
    OFCondition cond = dcmff.loadFile(fname, EXS_Unknown, EGL_noChange, DCM_MaxReadLength, opt_readMode);

    /* figure out if an error occured while the file was read*/
    if (cond.bad()) {
        OFLOG_ERROR(storescuLogger, "Bad DICOM file: " << fname << ": " << cond.text());
        return cond;
    }

    /* if required, invent new SOP instance information for the current data set (user option) */
    if (opt_inventSOPInstanceInformation)
    {
        StoreTest_ReplaceSOPInstanceInformation(dcmff.getDataset());
    }

    /* figure out which SOP class and SOP instance is encapsulated in the file */
    if (!DU_findSOPClassAndInstanceInDataSet(dcmff.getDataset(),
                                             sopClass, sopInstance,
                                             opt_correctUIDPadding))
    {
        OFLOG_ERROR(storescuLogger, "No SOP Class or Instance UID in file: " << fname);
        return DIMSE_BADDATA;
    }

    /* figure out which of the accepted presentation contexts should be used */
    DcmXfer filexfer(dcmff.getDataset()->getOriginalXfer());

    /* special case: if the file uses an unencapsulated transfer syntax (uncompressed
    * or deflated explicit VR) and we prefer deflated explicit VR, then try
    * to find a presentation context for deflated explicit VR first.
    */
    if (filexfer.isNotEncapsulated() &&
        opt_networkTransferSyntax == EXS_DeflatedLittleEndianExplicit)
    {
        filexfer = EXS_DeflatedLittleEndianExplicit;
    }

    if (filexfer.getXfer() != EXS_Unknown)
        presID = ASC_findAcceptedPresentationContextID(assoc, sopClass, filexfer.getXferID());
    else
        presID = ASC_findAcceptedPresentationContextID(assoc, sopClass);
    if (presID == 0) {
        const char *modalityName = dcmSOPClassUIDToModality(sopClass);
        if (!modalityName) modalityName = dcmFindNameOfUID(sopClass);
        if (!modalityName) modalityName = "unknown SOP class";
        OFLOG_ERROR(storescuLogger, "No presentation context for: (" << modalityName << ") " << sopClass);
        return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
    }

    T_ASC_PresentationContext pc;
    ASC_findAcceptedPresentationContext(assoc->params, presID, &pc);
    DcmXfer netTransfer(pc.acceptedTransferSyntax);

    /* if required, dump general information concerning transfer syntaxes */
    if (storescuLogger.isEnabledFor(OFLogger::INFO_LOG_LEVEL)) {
        DcmXfer fileTransfer(dcmff.getDataset()->getOriginalXfer());
        OFLOG_INFO(storescuLogger, "Converting transfer syntax: " << fileTransfer.getXferName()
                   << " -> " << netTransfer.getXferName());
    }

#ifdef ON_THE_FLY_COMPRESSION
    dcmff.getDataset()->chooseRepresentation(netTransfer.getXfer(), NULL);
#endif

    /* prepare the transmission of data */
    bzero(OFreinterpret_cast(char *, &req), sizeof(req));
    req.MessageID = msgId;
    strcpy(req.AffectedSOPClassUID, sopClass);
    strcpy(req.AffectedSOPInstanceUID, sopInstance);
    req.DataSetType = DIMSE_DATASET_PRESENT;
    req.Priority = DIMSE_PRIORITY_MEDIUM;

    /* if required, dump some more general information */
    OFLOG_INFO(storescuLogger, "Sending Store Request (MsgID " << msgId << ", "
               << dcmSOPClassUIDToModality(sopClass, "OT") << ")");

    /* finally conduct transmission of data */
    cond = DIMSE_storeUser(assoc, presID, &req,
                           NULL, dcmff.getDataset(), StoreTest_ProgressCallback, NULL,
                           opt_blockMode, opt_dimse_timeout,
                           &rsp, &statusDetail, NULL, OFstatic_cast(long, OFStandard::getFileSize(fname)));

    /*
    * If store command completed normally, with a status
    * of success or some warning then the image was accepted.
    */
    if (cond == EC_Normal && (rsp.DimseStatus == STATUS_Success || DICOM_WARNING_STATUS(rsp.DimseStatus))) {
        unsuccessfulStoreEncountered = OFFalse;
    }

    /* remember the response's status for later transmissions of data */
    lastStatusCode = rsp.DimseStatus;

    /* dump some more general information */
    if (cond == EC_Normal)
    {
        OFString temp_str;
        if (storescuLogger.isEnabledFor(OFLogger::DEBUG_LOG_LEVEL))
        {
            OFLOG_INFO(storescuLogger, "Received Store Response");
            OFLOG_DEBUG(storescuLogger, DIMSE_dumpMessage(temp_str, rsp, DIMSE_INCOMING, NULL, presID));
        }
        else {
            OFLOG_INFO(storescuLogger, "Received Store Response (" << DU_cstoreStatusString(rsp.DimseStatus) << ")");
        }
    }
    else
    {
        OFString temp_str;
        OFLOG_ERROR(storescuLogger, "Store Failed, file: " << fname << ":" << OFendl << DimseCondition::dump(temp_str, cond));
    }

    /* dump status detail information if there is some */
    if (statusDetail != NULL) {
        OFLOG_DEBUG(storescuLogger, "Status Detail:" << OFendl << DcmObject::PrintHelper(*statusDetail));
        delete statusDetail;
    }
    /* return */
    return cond;
}


static OFCondition StoreTest_CStore(T_ASC_Association *assoc, const OFString &fname)
/*
* This function will process the given file as often as is specified by opt_repeatCount.
* "Process" in this case means "read file, send C-STORE-RQ, receive C-STORE-RSP".
*
* Parameters:
*   assoc - [in] The association (network connection to another DICOM application).
*   fname - [in] Name of the file which shall be processed.
*/
{
    OFCondition cond = EC_Normal;

    /* opt_repeatCount specifies how many times a certain file shall be processed */
    int n = OFstatic_cast(int, opt_repeatCount);

    /* as long as no error occured and the counter does not equal 0 */
    while ((cond.good()) && n-- && !(opt_haltOnUnsuccessfulStore && unsuccessfulStoreEncountered))
    {
        /* process file (read file, send C-STORE-RQ, receive C-STORE-RSP) */
        cond = StoreTest_StoreSCU(assoc, fname.c_str());
    }

    // we don't want to return an error code if --no-halt was specified.
    if (!opt_haltOnUnsuccessfulStore)
    {
        cond = EC_Normal;
    }

    /* return result value */
    return cond;
}


static OFBool StoreTest_FindSOPClassAndInstanceInFile(const char *fname, char *sopClass, char *sopInstance)
{
    DcmFileFormat ff;
    if (!ff.loadFile(fname, EXS_Unknown, EGL_noChange, DCM_MaxReadLength, opt_readMode).good())
        return OFFalse;

    /* look in the meta-header first */
    OFBool found = DU_findSOPClassAndInstanceInDataSet(ff.getMetaInfo(), sopClass, sopInstance, opt_correctUIDPadding);

    if (!found)
        found = DU_findSOPClassAndInstanceInDataSet(ff.getDataset(), sopClass, sopInstance, opt_correctUIDPadding);

    return found;
}

static OFCondition StoreTest_ConfigureUserIdentityRequest(T_ASC_Parameters *params)
{
    OFCondition cond = EC_Normal;
    switch (opt_identMode)
    {
    case ASC_USER_IDENTITY_USER:
    {
        cond = ASC_setIdentRQUserOnly(params, opt_user, opt_identResponse);
        return cond;
    }
    case ASC_USER_IDENTITY_USER_PASSWORD:
    {
        cond = ASC_setIdentRQUserPassword(params, opt_user, opt_password, opt_identResponse);
        return cond;
    }
    case ASC_USER_IDENTITY_KERBEROS:
    case ASC_USER_IDENTITY_SAML:
    {
        OFFile identFile;
        if (!identFile.fopen(opt_identFile.c_str(), "rb"))
        {
            OFString openerror;
            identFile.getLastErrorString(openerror);
            OFLOG_ERROR(storescuLogger, "Unable to open Kerberos or SAML file: " << openerror);
            return EC_IllegalCall;
        }
        // determine file size
        offile_off_t result = identFile.fseek(0, SEEK_END);
        if (result != 0)
            return EC_IllegalParameter;
        offile_off_t filesize = identFile.ftell();
        identFile.rewind();
        if (filesize > 65535)
        {
            OFLOG_INFO(storescuLogger, "Kerberos or SAML file is larger than 65535 bytes, bytes after that position are ignored");
            filesize = 65535;
        }

        char *buf = new char[OFstatic_cast(unsigned int, filesize)];
        size_t bytesRead = identFile.fread(buf, 1, OFstatic_cast(size_t, filesize));
        identFile.fclose();
        if (bytesRead == 0)
        {
            OFLOG_ERROR(storescuLogger, "Unable to read Kerberos or SAML info from file: File empty?");
            delete[] buf;
            return EC_IllegalCall;
        }
        // Casting to Uint16 should be safe since it is checked above that file
        // size does not exceed 65535 bytes.
        if (opt_identMode == ASC_USER_IDENTITY_KERBEROS)
            cond = ASC_setIdentRQKerberos(params, buf, OFstatic_cast(Uint16, bytesRead), opt_identResponse);
        else
            cond = ASC_setIdentRQSaml(params, buf, OFstatic_cast(Uint16, bytesRead), opt_identResponse);
        delete[] buf;
        break;
    }
    default:
    {
        cond = EC_IllegalCall;
    }
    }
    if (cond.bad())
    {
        OFString temp_str;
        OFLOG_FATAL(storescuLogger, DimseCondition::dump(temp_str, cond));
    }
    return cond;
}

static OFCondition StoreTest_CheckUserIdentityResponse(T_ASC_Parameters *params)
{
    if (params == NULL)
        return ASC_NULLKEY;

    /* So far it is only checked whether a requested, positive response was
    actually received */

    // In case we sent no user identity request, there are no checks at all
    if ((opt_identMode == ASC_USER_IDENTITY_NONE) || (!opt_identResponse))
        return EC_Normal;

    // If positive response was requested, we expect a corresponding response
    if ((opt_identMode == ASC_USER_IDENTITY_USER) || (opt_identMode == ASC_USER_IDENTITY_USER_PASSWORD))
    {
        UserIdentityNegotiationSubItemAC *rsp = params->DULparams.ackUserIdentNeg;
        if (rsp == NULL)
        {
            OFLOG_ERROR(storescuLogger, "User Identity Negotiation failed: Positive response requested but none received");
            return ASC_USERIDENTIFICATIONFAILED;
        }
    }
    return EC_Normal;
}
