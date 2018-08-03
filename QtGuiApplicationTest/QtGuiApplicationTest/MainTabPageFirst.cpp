#include "MainTabPageFirst.h"

#include <QString>
#include <QImage>
#include <QFileDialog>

#include <LogUtil.h>

#include <dcmtk/ofstd/offile.h>
#include <dcmtk/oflog/oflog.h>
#include <dcmtk/dcmjpeg/djdecode.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcmetinf.h>
#include <dcmtk/dcmdata/dcuid.h>
#include <dcmtk/dcmdata/dcrledrg.h>
#include <dcmtk/dcmdata/dcpixel.h>
#include <dcmtk/dcmdata/dcpixseq.h>
#include <dcmtk/dcmdata/dcpxitem.h>
#include <dcmtk/dcmdata/libi2d/i2djpgs.h>

static OFLogger gLogger = OFLog::getLogger("DicomTestLog");

MainTabPageFirst::MainTabPageFirst(QWidget *parent /* = Q_NULLPTR */)
    : QWidget(parent)
{
    ui.setupUi(this);

    ui.imgTitleValue->setText("image_filename_test");
}

MainTabPageFirst::~MainTabPageFirst()
{
}

bool MainTabPageFirst::event(QEvent *event)
{
    //LogUtil::Debug(CODE_LOCATION, "eventType: %d", event->type());
    switch (event->type())
    {
    case QEvent::Show:
        break;
    default:
        break;
    }
    return QWidget::event(event);
}

void MainTabPageFirst::showEvent(QShowEvent *event)
{
    return QWidget::showEvent(event);
}

void MainTabPageFirst::on_btnBrowserDcm_clicked()
{
    QString curAppPath = QCoreApplication::applicationDirPath();
    QString dcmFileName = QFileDialog::getOpenFileName(this, tr("Open File"), curAppPath, tr("DICOM (*.dcm)"));
    ShowDicomImage(dcmFileName);
}

void MainTabPageFirst::on_btnBrowserImg_clicked()
{
    QString curAppPath = QCoreApplication::applicationDirPath();
    QString imgFileName = QFileDialog::getOpenFileName(this, tr("Open File"), curAppPath, tr("Images (*.jpg)"));

    if (!imgFileName.isEmpty())
    {
        QImage imgTest(imgFileName);

        int width = imgTest.width();
        int height = imgTest.height();
        int depth = imgTest.depth();
        int byteCount = imgTest.byteCount();
        bool allGray = imgTest.allGray();
        const uchar* imgBits = imgTest.constBits();
        QImage::Format imgFormat = imgTest.format();

        ui.imgContent->setPixmap(QPixmap::fromImage(imgTest.scaled(ui.imgContent->size(), Qt::KeepAspectRatio)));

        int indexOfDot = imgFileName.lastIndexOf(".");
        QString saveFilename = imgFileName.left(indexOfDot) + ".dcm";

        OFCondition status;
        DcmFileFormat *pFileFormat = new DcmFileFormat();
        DcmDataset *pDataSet = pFileFormat->getDataset();

        const int constWidth = 50;
        const int constHeight = 50;
        Uint8 testImgBuf[constWidth*constHeight] = { 0 };
        for (int i = 0; i < constHeight; ++i)
        {
            for (int j = 0; j < constWidth; ++j)
            {
                if (i == j)
                {
                    testImgBuf[i*constWidth + j] = 0;
                }
                else
                {
                    testImgBuf[i*constWidth + j] = 255;
                }
            }
        }

        pDataSet->putAndInsertUint16(DCM_AccessionNumber, 0);
        pDataSet->putAndInsertString(DCM_PatientName, "Testing", true);
        pDataSet->putAndInsertString(DCM_PatientID, "110119");
        pDataSet->putAndInsertString(DCM_PatientBirthDate, "20180803");
        pDataSet->putAndInsertString(DCM_PatientSex, "M");

        /*	���Study��Ϣ	*/
        pDataSet->putAndInsertString(DCM_StudyDate, "20180803");
        pDataSet->putAndInsertString(DCM_StudyTime, "182230");
        char uid[100];
        dcmGenerateUniqueIdentifier(uid, SITE_STUDY_UID_ROOT);
        pDataSet->putAndInsertString(DCM_StudyID, uid);
        dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT);
        pDataSet->putAndInsertString(DCM_StudyInstanceUID, uid);


        /*	���Series��Ϣ	*/
        pDataSet->putAndInsertString(DCM_SeriesDate, "20141221");
        pDataSet->putAndInsertString(DCM_SeriesTime, "195411");
        memset(uid, 0, sizeof(char) * 100);
        dcmGenerateUniqueIdentifier(uid, SITE_SERIES_UID_ROOT);
        pDataSet->putAndInsertString(DCM_SeriesInstanceUID, uid);

        /*	���Image��Ϣ	*/
        pDataSet->putAndInsertString(DCM_ImageType, "ORIGINAL\\PRIMARY\\AXIAL");
        pDataSet->putAndInsertString(DCM_ContentDate, "20141221");
        pDataSet->putAndInsertString(DCM_ContentTime, "200700");
        pDataSet->putAndInsertString(DCM_InstanceNumber, "1");
        pDataSet->putAndInsertString(DCM_SamplesPerPixel, "1");
        pDataSet->putAndInsertString(DCM_PhotometricInterpretation, "MONOCHROME2");
        pDataSet->putAndInsertString(DCM_PixelSpacing, "0.3\\0.3");
        pDataSet->putAndInsertString(DCM_Modality, "US");
        pDataSet->putAndInsertUint16(DCM_SamplesPerPixel, 1);
        pDataSet->putAndInsertString(DCM_NumberOfFrames, "1");
        pDataSet->putAndInsertUint16(DCM_Rows, constHeight);
        pDataSet->putAndInsertUint16(DCM_Columns, constWidth);
        pDataSet->putAndInsertUint16(DCM_BitsAllocated, 8);
        pDataSet->putAndInsertUint16(DCM_BitsStored, 8);
        pDataSet->putAndInsertUint16(DCM_HighBit, 7);
        pDataSet->putAndInsertUint8Array(DCM_PixelData, (Uint8*)testImgBuf, constWidth*constHeight);

        /* ��� MicroDICOM */
        dcmGenerateUniqueIdentifier(uid, SITE_STUDY_UID_ROOT);
        //pDataSet->putAndInsertString(DCM_SOPClassUID)

        status = pFileFormat->saveFile(saveFilename.toStdString().c_str(), EXS_LittleEndianImplicit, EET_UndefinedLength, EGL_recalcGL, EPD_noChange, 0, 0, EWM_dataset);
        if (status.bad())
        {
            LogUtil::Error(CODE_LOCATION, "Error: %s", status.text());
        }

        delete pFileFormat;
    }
}

void MainTabPageFirst::ShowDicomImage(QString &dcmFileName)
{
    int imgWidth = 512;
    int imgHeight = 512;

    OFFilename dcmFilename(dcmFileName.toStdString().c_str());
    //    OFFilename dcmFilename("e:\\temp\\dicomtesting\\srdoc10\\image12.dcm");
    if (OFStandard::fileExists(dcmFilename))
    {
        OFLOG_DEBUG(gLogger, "Found file: " << dcmFilename);
        DcmFileFormat dcmFileFormat;
        OFCondition loadRes = dcmFileFormat.loadFile(dcmFilename);
        if (loadRes.good())
        {
            OFLOG_DEBUG(gLogger, "Load successfully: " << dcmFilename);
            DcmDataset *pDcmDataSet = dcmFileFormat.getDataset();

            pDcmDataSet->print(std::cout, DCMTypes::PF_shortenLongTagValues);

            E_TransferSyntax xfer = pDcmDataSet->getOriginalXfer();
            OFLOG_DEBUG(gLogger, "E_TransferSyntax " << xfer);

            OFString patientName;
            OFCondition findRes = pDcmDataSet->findAndGetOFString(DCM_PatientName, patientName);
            if (findRes.good())
            {
                OFLOG_DEBUG(gLogger, "PatientName: " << patientName);
            }

            unsigned short bitStored = 0;
            pDcmDataSet->findAndGetUint16(DCM_BitsStored, bitStored); //��ȡ���ص�λ�� bit
            OFLOG_DEBUG(gLogger, "bitStored: " << bitStored);

            OFString photoInterpretation;
            pDcmDataSet->findAndGetOFString(DCM_PhotometricInterpretation, photoInterpretation); //DCMͼƬ��ͼ��ģʽ
            OFLOG_DEBUG(gLogger, "photoInterpretation: " << photoInterpretation);

            unsigned short samplePerPixel = 0;
            pDcmDataSet->findAndGetUint16(DCM_SamplesPerPixel, samplePerPixel);
            OFLOG_DEBUG(gLogger, "samplePerPixel: " << samplePerPixel);

            unsigned short mImgWidth; //��ȡͼ��Ĵ����
            unsigned short mImgHeight;
            pDcmDataSet->findAndGetUint16(DCM_Columns, mImgWidth);
            pDcmDataSet->findAndGetUint16(DCM_Rows, mImgHeight);
            OFLOG_DEBUG(gLogger, "imageSize: " << mImgWidth << "x" << mImgHeight);
            imgWidth = mImgWidth;
            imgHeight = mImgHeight;

            const char *pRescaleSlope = Q_NULLPTR;
            const char *pRescaleIntercept = Q_NULLPTR;
            pDcmDataSet->findAndGetString(DCM_RescaleSlope, pRescaleSlope);
            pDcmDataSet->findAndGetString(DCM_RescaleIntercept, pRescaleIntercept);
            int rescaleSlope = pRescaleSlope == Q_NULLPTR ? 0 : atoi(pRescaleSlope);
            int rescaleIntercept = pRescaleIntercept == Q_NULLPTR ? 0 : atoi(pRescaleIntercept);

            const char *pWindowCenter = Q_NULLPTR;
            const char *pWindowWidth = Q_NULLPTR;
            pDcmDataSet->findAndGetString(DCM_WindowCenter, pWindowCenter);
            pDcmDataSet->findAndGetString(DCM_WindowWidth, pWindowWidth);
            int windowCenter = pWindowCenter == Q_NULLPTR ? 0 : atoi(pWindowCenter);
            int windowWidth = pWindowWidth == Q_NULLPTR ? 0 : atoi(pWindowWidth);

            const char* transferSyntax = NULL;
            findRes = dcmFileFormat.getMetaInfo()->findAndGetString(DCM_TransferSyntaxUID, transferSyntax);
            if (findRes.good())
            {
                OFLOG_DEBUG(gLogger, "TransferSyntaxUID: " << transferSyntax);
                if (OFString(transferSyntax) == OFString(UID_JPEGProcess14SV1TransferSyntax)
                    || OFString(transferSyntax) == OFString(UID_JPEGProcess2_4TransferSyntax)
                    || OFString(transferSyntax) == OFString(UID_JPEGProcess14TransferSyntax)
                    || OFString(transferSyntax) == OFString(UID_JPEGProcess1TransferSyntax))
                {
                    DJDecoderRegistration::registerCodecs();
                    pDcmDataSet->chooseRepresentation(EXS_LittleEndianExplicit, NULL); //��ѹ����ͼ�����ؽ��н�ѹ
                    DJDecoderRegistration::cleanup();
                }
                else if (OFString(transferSyntax) == OFString(UID_RLELosslessTransferSyntax))
                {
                    DcmRLEDecoderRegistration::registerCodecs();
                    pDcmDataSet->chooseRepresentation(EXS_LittleEndianExplicit, NULL);
                    DcmRLEDecoderRegistration::cleanup();
                }
                else
                {
                    pDcmDataSet->chooseRepresentation(xfer, NULL);
                }
            }
            else
            {
                OFLOG_ERROR(gLogger, "Find DCM_TransferSyntaxUID error: " << findRes.text());
                pDcmDataSet->chooseRepresentation(EXS_Unknown, NULL);
            }
            DcmElement* element = NULL;
            OFCondition result = pDcmDataSet->findAndGetElement(DCM_PixelData, element);
            if (result.bad() || element == NULL)
                return;
            Uint8  *pImgData8 = Q_NULLPTR;
            Uint16 *pImgData16 = Q_NULLPTR;
            Uint32 *pImgData32 = Q_NULLPTR;
            if (8 == bitStored)
            {
                result = element->getUint8Array(pImgData8);
            }
            else if (8 < bitStored && bitStored <= 16)
            {
                result = element->getUint16Array(pImgData16);
            }
            else
            {
                result = element->getUint32Array(pImgData32);
            }

            if (pImgData16 != Q_NULLPTR)
            {
                int minSize = imgWidth < imgHeight ? imgWidth : imgHeight;
                QImage imgTest(imgWidth, imgHeight, QImage::Format_RGB32);
                Sint16 pixelValue = 0;
                int rValue = 0, gValue = 0, bValue = 0;
                const int msgLen = 512 * 10;
                char tempMsg[msgLen] = { 0 };
                char tempPixelValue[10] = { 0 };
                for (int i = 0; i < imgHeight; ++i)
                {
                    ZeroMemory(tempMsg, msgLen);
                    for (int j = 0; j < imgWidth; ++j)
                    {
                        // CT ֵ�ļ��㹫ʽΪ�� Hu = pixel * slope + intercept
                        pixelValue = pImgData16[i * imgWidth + j] * rescaleSlope + rescaleIntercept;
                        //rValue = pixelValue >> 11;
                        //gValue = pixelValue >> 5 & 0x3F;
                        //bValue = pixelValue & 0x1F;
                        //imgTest.setPixelColor(j, i, QColor(rValue, gValue, bValue, 255));

                        sprintf_s(tempPixelValue, "\t%d", pixelValue);
                        strcat_s(tempMsg, tempPixelValue);

                        if (pixelValue > 200)
                        {
                            pixelValue = 255;
                        }
                        else if (pixelValue < -600)
                        {
                            pixelValue = 0;
                        }
                        else
                        {
                            pixelValue = (pixelValue + 600) * 255.0f / 800;
                        }
                        imgTest.setPixelColor(j, i, QColor(pixelValue, pixelValue, pixelValue, 255));
                    }
                }
                ui.imgContent->setPixmap(QPixmap::fromImage(imgTest));
            }
            else if (pImgData8 != Q_NULLPTR)
            {
                int minSize = imgWidth < imgHeight ? imgWidth : imgHeight;
                QImage imgTest(imgWidth, imgHeight, QImage::Format_RGB32);
                Uint8 pixelValue = 0;
                int rValue = 0, gValue = 0, bValue = 0;
                for (int i = 0; i < imgHeight; ++i)
                {
                    for (int j = 0; j < imgWidth; ++j)
                    {
                        pixelValue = pImgData8[i * imgWidth + j];
                        imgTest.setPixelColor(j, i, QColor(pixelValue, pixelValue, pixelValue, 255));
                    }
                }
                ui.imgContent->setGeometry(ui.imgContent->x(), ui.imgContent->y(), imgWidth, imgHeight);
                ui.imgContent->setPixmap(QPixmap::fromImage(imgTest));
            }
            else if (pImgData32 != Q_NULLPTR)
            {
            }

        }
        else
        {
            OFLOG_ERROR(gLogger, "Load file failed: " << dcmFilename);


            int minSize = imgWidth < imgHeight ? imgWidth : imgHeight;
            QImage imgTest(imgWidth, imgHeight, QImage::Format_RGB16);
            for (int i = 0; i < imgHeight; ++i)
            {
                for (int j = 0; j < imgWidth; ++j)
                {
                    if (i == j || i + j == minSize || i == imgHeight / 2 || i == imgHeight - 1 || i == 0 || j == 0 || j == imgWidth / 2 || j == imgWidth - 1)
                    {
                        imgTest.setPixelColor(i, j, QColor(255, 0, 0, 255));
                    }
                    else
                    {
                        imgTest.setPixelColor(i, j, QColor(255, 255, 255, 255));
                    }
                }
            }
            ui.imgContent->setPixmap(QPixmap::fromImage(imgTest));
        }
    }
}

void MainTabPageFirst::BackupFunction()
{
    QString curAppPath = QCoreApplication::applicationDirPath();
    QString imgFileName = QFileDialog::getOpenFileName(this, tr("Open File"), curAppPath, tr("Images (*.jpg)"));

    if (!imgFileName.isEmpty())
    {
        QImage imgTest(imgFileName);

        int width = imgTest.width();
        int height = imgTest.height();
        int depth = imgTest.depth();
        int byteCount = imgTest.byteCount();
        bool allGray = imgTest.allGray();
        const uchar* imgBits = imgTest.constBits();
        QImage::Format imgFormat = imgTest.format();

        ui.imgContent->setPixmap(QPixmap::fromImage(imgTest.scaled(ui.imgContent->size(), Qt::KeepAspectRatio)));

        int indexOfDot = imgFileName.lastIndexOf(".");
        QString saveFilename = imgFileName.left(indexOfDot) + ".dcm";

        DcmFileFormat *pFileFormat = new DcmFileFormat();
        DcmDataset *pDataSet = pFileFormat->getDataset();
        //DcmPixelSequence *pDcmPixelSeq = new DcmPixelSequence(DcmTag(DCM_PixelData, EVR_OB));
        DcmPixelSequence *pDcmPixelSeq = new DcmPixelSequence(DCM_PixelSequenceTag);

        Uint32 dataLength = 0;
        OFCondition status;

        OFString filename = OFString(imgFileName.toStdString().c_str());
        I2DJpegSource* bmpSource = new I2DJpegSource();
        bmpSource->setImageFile(filename);

        char* pixData = NULL;
        Uint16 rows, cols, samplePerPixel, bitsAlloc, bitsStored, highBit, pixelRpr, planConf, pixAspectH, pixAspectV;
        OFString photoMetrInt;
        Uint32 length;
        E_TransferSyntax ts = EXS_Unknown;
        status = bmpSource->readPixelData(rows, cols, samplePerPixel, photoMetrInt, bitsAlloc, bitsStored, highBit, pixelRpr, planConf, pixAspectH, pixAspectV, pixData, length, ts);
        if (status.bad())
        {
            LogUtil::Error(CODE_LOCATION, "read pixel failed: %s", status.text());
        }

        DcmXfer transport(ts);
        if (transport.isEncapsulated())
        {
            LogUtil::Debug(CODE_LOCATION, "encapsulated");
        }
        else
        {
            LogUtil::Debug(CODE_LOCATION, "is not encapsulated");
        }

        //DcmPixelItem *newItem = new DcmPixelItem(DcmTag(DCM_Item, EVR_OB));
        DcmPixelItem *newItem = new DcmPixelItem(DCM_PixelItemTag);
        if (newItem != NULL)
        {
            pDcmPixelSeq->insert(newItem);
            status = newItem->putUint8Array((Uint8*)pixData, length);
            if (status.bad())
            {
                LogUtil::Error(CODE_LOCATION, "put pixel data failed: %s", status.text());
            }
        }

        pDataSet->putAndInsertUint16(DCM_AccessionNumber, 0);
        pDataSet->putAndInsertString(DCM_PatientName, "zssure", true);
        pDataSet->putAndInsertString(DCM_PatientID, "2234");
        pDataSet->putAndInsertString(DCM_PatientBirthDate, "20141221");
        pDataSet->putAndInsertString(DCM_PatientSex, "M");

        /*	���Study��Ϣ	*/
        pDataSet->putAndInsertString(DCM_StudyDate, "20141221");
        pDataSet->putAndInsertString(DCM_StudyTime, "195411");
        char uid[100];
        dcmGenerateUniqueIdentifier(uid, SITE_STUDY_UID_ROOT);
        pDataSet->putAndInsertString(DCM_StudyInstanceUID, uid);
        pDataSet->putAndInsertString(DCM_StudyID, "1111");


        /*	���Series��Ϣ	*/
        pDataSet->putAndInsertString(DCM_SeriesDate, "20141221");
        pDataSet->putAndInsertString(DCM_SeriesTime, "195411");
        memset(uid, 0, sizeof(char) * 100);
        dcmGenerateUniqueIdentifier(uid, SITE_SERIES_UID_ROOT);
        pDataSet->putAndInsertString(DCM_SeriesInstanceUID, uid);

        /*	���Image��Ϣ	*/
        pDataSet->putAndInsertString(DCM_ImageType, "ORIGINAL\\PRIMARY\\AXIAL");
        pDataSet->putAndInsertString(DCM_ContentDate, "20141221");
        pDataSet->putAndInsertString(DCM_ContentTime, "200700");
        pDataSet->putAndInsertString(DCM_InstanceNumber, "1");
        pDataSet->putAndInsertString(DCM_SamplesPerPixel, "1");
        pDataSet->putAndInsertString(DCM_PhotometricInterpretation, "MONOCHROME2");
        pDataSet->putAndInsertString(DCM_PixelSpacing, "0.3\\0.3");

        pDataSet->putAndInsertUint16(DCM_SamplesPerPixel, samplePerPixel);
        pDataSet->putAndInsertString(DCM_NumberOfFrames, "1");
        pDataSet->putAndInsertUint16(DCM_Rows, rows);
        pDataSet->putAndInsertUint16(DCM_Columns, cols);
        pDataSet->putAndInsertUint16(DCM_BitsAllocated, bitsAlloc);
        pDataSet->putAndInsertUint16(DCM_BitsStored, bitsStored);
        pDataSet->putAndInsertUint16(DCM_HighBit, highBit);
        pDataSet->putAndInsertOFStringArray(DCM_PhotometricInterpretation, photoMetrInt);
        pDataSet->insert(pDcmPixelSeq, OFFalse, OFFalse);

        status = pFileFormat->saveFile(saveFilename.toStdString().c_str(), ts);
        if (status.bad())
        {
            LogUtil::Error(CODE_LOCATION, "Error: %s", status.text());
        }

        delete bmpSource;
        delete pFileFormat;
    }
}
