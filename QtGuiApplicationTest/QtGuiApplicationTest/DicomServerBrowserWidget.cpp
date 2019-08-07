#include "DicomServerBrowserWidget.h"
#include "ui_DicomServerBrowserWidget.h"

// QT Headers
#include <QApplication>
#include <QTime>
#include <QFile>
#include <QFileInfo>

// DCMKT
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcuid.h>
#include <dcmtk/dcmsr/dsrtypes.h>

// Self Headers
#include <LogUtil.h>
#include <DicomTest.h>
#include <DicomClient.h>

#include "TitleBarWidget.h"

DicomServerBrowserWidget::DicomServerBrowserWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DicomServerBrowserWidget)
    , mpDicomClient(new DicomClient(this))
{
    ui->setupUi(this);

    QIcon winIcon(":/AppImages/Resources/images/dicom_xray.ico");
    this->setWindowIcon(winIcon);
    mpTitleBarWidget = new TitleBarWidget(this);

    mpDicomClient->SetAppAETitle("MGIUSCLIENT");
    mpDicomClient->SetPeerAETitle("MGIUSDICOM");
    mpDicomClient->SetPeerHostName("10.225.12.64");
    mpDicomClient->SetPeerPort(7893);
    mpDicomClient->SetMaxReceivePDULength(32768);
}

DicomServerBrowserWidget::~DicomServerBrowserWidget()
{
    delete ui;
}

void DicomServerBrowserWidget::on_btnEchoTest_clicked()
{
    mpDicomClient->PerformEcho();

    //DicomTest dcmTest;
    //dcmTest.DcmtkTestEcho();

    //DicomEchoSCU echoSCU;
    //echoSCU.PerformEcho();
}

void DicomServerBrowserWidget::on_btnFindTest_clicked()
{
    mpDicomClient->PerformFind();

    //DicomTest dcmTest;
    //dcmTest.DcmtkTestFind();

    //DicomFindSCU findSCU;
    //findSCU.InitializeNetwork();
    //findSCU.SetBlockingMode(false); // 非阻塞模式 select
    //findSCU.SetAppEntityTitle("DebugTest", "DICOM");
    //findSCU.AppendOverrideKey("QueryRetrieveLevel=STUDY");
    //findSCU.AppendOverrideKey("PatientName");
    //findSCU.AppendOverrideKey("PatientID");
    //findSCU.AppendOverrideKey("NumberOfStudyRelatedInstances");
    //findSCU.PerformQuery();
    //findSCU.DropNetwork();
}

void DicomServerBrowserWidget::on_btnGetTest_clicked()
{
    mpDicomClient->PerformGet();

    //DicomTest dcmTest;
    //dcmTest.DcmtkTestGet();
}

void DicomServerBrowserWidget::on_btnMoveTest_clicked()
{
    mpDicomClient->PerformMove();
}

void DicomServerBrowserWidget::on_btnStoreTest_clicked()
{
    QString dcmFilename = ui->leDcmFilename->text();
    if (!dcmFilename.isEmpty())
    {
        mpDicomClient->MakeStoreTask(dcmFilename);
    }
    else
    {
        // TODO: UI 输入错误
    }
}

void DicomServerBrowserWidget::on_btnSaveReport_clicked()
{
    DcmFileFormat *pFileFormat = new DcmFileFormat();
    DcmDataset *pDataSet = pFileFormat->getDataset();
    OFCondition status = EC_Normal;

    /* 防止中文乱码 */
    pDataSet->putAndInsertString(DCM_SpecificCharacterSet, "ISO_IR 100");

    /* 添加 Patient 信息 */
    pDataSet->putAndInsertString(DCM_PatientName, "Shuanglong");
    pDataSet->putAndInsertString(DCM_PatientID, "123456");
    pDataSet->putAndInsertString(DCM_PatientBirthDate, "19901104");
    pDataSet->putAndInsertString(DCM_PatientSex, "M");
    pDataSet->putAndInsertString(DCM_BodyPartExamined, "Thyroid");

    /* 添加 SOP class 信息 */
    char uid[100];
    dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT);
    pDataSet->putAndInsertString(DCM_SOPInstanceUID, uid);
    pDataSet->putAndInsertString(DCM_SOPClassUID, UID_EnhancedSRStorage); // 结构化报告

    /* 添加 Study 信息 */
    pDataSet->putAndInsertString(DCM_StudyDate, "20190802");
    pDataSet->putAndInsertString(DCM_ContentDate, "20190802");
    pDataSet->putAndInsertString(DCM_StudyTime, "1359");
    pDataSet->putAndInsertString(DCM_ContentTime, "1359");
    pDataSet->putAndInsertString(DCM_StudyID, "123456789");
    dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT);
    pDataSet->putAndInsertString(DCM_StudyInstanceUID, uid);

    /* 添加 Series 信息 */
    pDataSet->putAndInsertString(DCM_SeriesDate, "20190802");
    pDataSet->putAndInsertString(DCM_SeriesTime, "1359");
    memset(uid, 0, sizeof(char) * 100);
    dcmGenerateUniqueIdentifier(uid, SITE_SERIES_UID_ROOT);
    pDataSet->putAndInsertString(DCM_SeriesInstanceUID, uid);
    pDataSet->putAndInsertString(DCM_SeriesNumber, "1");
    pDataSet->putAndInsertString(DCM_InstanceNumber, "1");

    /* 添加 厂商 信息 */
    pDataSet->putAndInsertString(DCM_Manufacturer, "MGIUS-R3");

    pDataSet->putAndInsertString(DCM_Modality, "SR");
    pDataSet->insertSequenceItem(DCM_ReferencedPerformedProcedureStepSequence, new DcmItem());
    pDataSet->insertSequenceItem(DCM_PerformedProcedureCodeSequence, new DcmItem());
    pDataSet->putAndInsertString(DCM_AccessionNumber, "110");
    pDataSet->putAndInsertString(DCM_ReferringPhysicianName, "Shuanglong");
    pDataSet->putAndInsertString(DCM_ContinuityOfContent, "SEPARATE");

    /* 报告内容序列 */
    DcmItem *pConceptNameSequence = NULL;
    DcmItem *pContentSequence = NULL;
    DcmItem *pDcmItem = NULL;

    pDataSet->putAndInsertString(DCM_ValueType, "CONTAINER");
    pDataSet->putAndInsertString(DCM_CompletionFlag, "PARTIAL");
    pDataSet->putAndInsertString(DCM_VerificationFlag, "UNVERIFIED");

    pConceptNameSequence = new DcmItem();
    pDataSet->insertSequenceItem(DCM_ConceptNameCodeSequence, pConceptNameSequence);
    pConceptNameSequence->putAndInsertString(DCM_CodeValue, "DT.06");
    pConceptNameSequence->putAndInsertString(DCM_CodingSchemeDesignator, OFFIS_CODING_SCHEME_DESIGNATOR);
    pConceptNameSequence->putAndInsertString(DCM_CodeMeaning, "Consultation Report");
    pConceptNameSequence->putAndInsertString(DCM_CodingSchemeUID, "1.2.276.0.7230010.3.0.0.1");

    pContentSequence = new DcmItem();
    pDataSet->insertSequenceItem(DCM_ContentSequence, pContentSequence);
    pContentSequence->putAndInsertString(DCM_RelationshipType, "HAS OBS CONTEXT");
    pContentSequence->putAndInsertString(DCM_ValueType, "PNAME");
    pContentSequence->putAndInsertString(DCM_PersonName, "双龙");

    pConceptNameSequence = new DcmItem();
    pContentSequence->insertSequenceItem(DCM_ConceptNameCodeSequence, pConceptNameSequence);
    pConceptNameSequence->putAndInsertString(DCM_CodeValue, "IHE.04");
    pConceptNameSequence->putAndInsertString(DCM_CodingSchemeDesignator, OFFIS_CODING_SCHEME_DESIGNATOR);
    pConceptNameSequence->putAndInsertString(DCM_CodeMeaning, "Observer Name");
    pConceptNameSequence->putAndInsertString(DCM_CodingSchemeUID, "1.2.276.0.7230010.3.0.0.1");

    QString filename = QApplication::applicationDirPath() + "/StructureReport.dcm";
    OFFilename newSaveFilename = filename.toUtf8().data();
    status = pFileFormat->saveFile(newSaveFilename, EXS_LittleEndianImplicit);
    if (status.bad())
    {
        LogUtil::Error(CODE_LOCATION, "Error: %s", status.text());
    }
    pDataSet->clear();
    delete pFileFormat;

    // QT 文件读取测试
    QTime timeCost, timeTotalCost;
    timeTotalCost.start();

    for (int i = 0; i < 10000; ++i)
    {
        timeCost.start();
        QByteArray bytes;
        QFile pdfTestFile(QString("E:/Temp/DicomTesting/DcmtkBin/PDF_Reports/report_%1.pdf").arg(i % 100 + 1));
        if (pdfTestFile.open(QIODevice::ReadOnly))
        {
            bytes.clear();
            bytes = pdfTestFile.readAll();
        }
        pdfTestFile.close();
        int sigleTimeCost = timeCost.elapsed();
        LogUtil::Debug(CODE_LOCATION, "SingleTimeCost: %d Size: %d", timeCost.elapsed(), bytes.size());
    }
    int totalTimeCost = timeTotalCost.elapsed();
    LogUtil::Debug(CODE_LOCATION, "TotalTimeCost: %d", totalTimeCost);


    LogUtil::Debug("\n\n\n abcd");

    timeTotalCost.start();
    for (int i = 0; i < 10000; ++i)
    {
        timeCost.start();
        QString filename = QString("E:/Temp/DicomTesting/DcmtkBin/PDF_Reports/report_%1.pdf").arg(i % 100 + 1);
        QFileInfo fileInfo(filename);
        QFile pdfTestFile(filename);
        if (pdfTestFile.open(QIODevice::ReadOnly))
        {
            uchar *pBuffer = pdfTestFile.map(0, fileInfo.size());
            if (pBuffer != Q_NULLPTR)
            {
                pdfTestFile.unmap(pBuffer);
            }
        }
        pdfTestFile.close();
        int sigleTimeCost = timeCost.elapsed();
        LogUtil::Debug(CODE_LOCATION, "SingleTimeCost: %d Size: %lld", timeCost.elapsed(), fileInfo.size());
    }
    totalTimeCost = timeTotalCost.elapsed();
    LogUtil::Debug(CODE_LOCATION, "TotalTimeCost: %d", totalTimeCost);
}
