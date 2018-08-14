#include "MainTabPageFirst.h"

#include <QString>
#include <QImage>
#include <QTime>
#include <QDateTime>
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
#include <dcmtk/dcmdata/libi2d/i2dbmps.h>

#include "DicomWindow.h"

static OFLogger gLogger = OFLog::getLogger("DicomTestLog");

MainTabPageFirst::MainTabPageFirst(QWidget *parent /* = Q_NULLPTR */)
    : QWidget(parent)
{
    ui.setupUi(this);

    ui.imgTitleValue->setText("image_filename_test");


    pDcmWidget = new DicomWindow();
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
    if (pDcmWidget->isVisible())
    {
        pDcmWidget->hide();
    }
    else
    {
        pDcmWidget->show();
    }
    return;

    //this->SelfPaintImage8Bit();
    //this->ReadJpegAndCopyToDicom();
    //this->ReadImageByQImage();
    this->ReadImageByQImageMulti();
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
                ui.imgTitleValue->setText(QString::fromLocal8Bit(patientName.c_str()));
            }

            unsigned short bitStored = 0;
            pDcmDataSet->findAndGetUint16(DCM_BitsStored, bitStored); //获取像素的位数 bit
            OFLOG_DEBUG(gLogger, "bitStored: " << bitStored);

            OFString photoInterpretation;
            pDcmDataSet->findAndGetOFString(DCM_PhotometricInterpretation, photoInterpretation); //DCM图片的图像模式
            OFLOG_DEBUG(gLogger, "photoInterpretation: " << photoInterpretation);

            unsigned short samplePerPixel = 0;
            pDcmDataSet->findAndGetUint16(DCM_SamplesPerPixel, samplePerPixel);
            OFLOG_DEBUG(gLogger, "samplePerPixel: " << samplePerPixel);

            unsigned short mImgWidth; //获取图像的窗宽高
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
                    pDcmDataSet->chooseRepresentation(EXS_LittleEndianExplicit, NULL); //对压缩的图像像素进行解压
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
                        // CT 值的计算公式为： Hu = pixel * slope + intercept
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
                int *pRgbValue = new int[samplePerPixel];
                for (int i = 0; i < imgHeight; ++i)
                {
                    for (int j = 0; j < imgWidth; ++j)
                    {
                        for (int k = 0; k < samplePerPixel; ++k)
                        {
                            pRgbValue[k] = pImgData8[i * imgWidth * samplePerPixel + j * samplePerPixel + k];
                        }
                        //pixelValue = pImgData8[i * imgWidth + j];
                        imgTest.setPixelColor(j, i, QColor(pRgbValue[0], pRgbValue[1], pRgbValue[2], 255));
                    }
                }
                delete []pRgbValue;
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

void MainTabPageFirst::ReadImageByQImage()
{
    QString curAppPath = QCoreApplication::applicationDirPath();
    QString imgFileName = QFileDialog::getOpenFileName(this, tr("Open File"), curAppPath, tr("Images (*.jpg *.bmp);;All files (*.*)"));

    if (!imgFileName.isEmpty())
    {
        QImage imgTest(imgFileName);

        int width = imgTest.width();
        int height = imgTest.height();
        int oldSamplePerPixel = imgTest.depth() / 8;
        int newSamplePerPixel = oldSamplePerPixel > 3 ? 3 : oldSamplePerPixel; // DICOM 最多 3 通道
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

        OFString filename = OFString(imgFileName.toStdString().c_str());

        pDataSet->putAndInsertString(DCM_ProtocolName, "Page - Full");

        /* 添加 Patient 信息 */
        pDataSet->putAndInsertString(DCM_AccessionNumber, "MGI001");
        pDataSet->putAndInsertString(DCM_PatientName, "Testing");
        pDataSet->putAndInsertString(DCM_PatientID, "110119");
        pDataSet->putAndInsertString(DCM_PatientBirthDate, "20180803");
        pDataSet->putAndInsertString(DCM_PatientSex, "M");
        pDataSet->putAndInsertString(DCM_PatientOrientation, "");
        pDataSet->putAndInsertString(DCM_Laterality, "");
        pDataSet->putAndInsertString(DCM_ImageComments, "");

        /* sop class uid */
        char uid[100];
        dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT);
        pDataSet->putAndInsertString(DCM_SOPInstanceUID, uid);
        pDataSet->putAndInsertString(DCM_SOPClassUID, UID_UltrasoundImageStorage);

        /* 添加 Study 信息 */
        pDataSet->putAndInsertString(DCM_StudyDate, "20180803");
        pDataSet->putAndInsertString(DCM_ContentDate, "20180803");
        pDataSet->putAndInsertString(DCM_StudyTime, "182230");
        pDataSet->putAndInsertString(DCM_ContentTime, "182230");
        dcmGenerateUniqueIdentifier(uid, SITE_STUDY_UID_ROOT);
        pDataSet->putAndInsertString(DCM_StudyID, uid);
        dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT);
        pDataSet->putAndInsertString(DCM_StudyInstanceUID, uid);

        /* 添加 Series 信息 */
        pDataSet->putAndInsertString(DCM_SeriesDate, "20141221");
        pDataSet->putAndInsertString(DCM_SeriesTime, "195411");
        memset(uid, 0, sizeof(char) * 100);
        dcmGenerateUniqueIdentifier(uid, SITE_SERIES_UID_ROOT);
        pDataSet->putAndInsertString(DCM_SeriesInstanceUID, uid);

        /* 添加 厂商 信息 */
        pDataSet->putAndInsertString(DCM_Manufacturer, "Nova Microsonics");
        pDataSet->putAndInsertString(DCM_ReferringPhysicianName, "ReferringName");

        /* 添加 Stage 信息 */
        pDataSet->putAndInsertString(DCM_StageName, "StageTest");
        pDataSet->putAndInsertString(DCM_StageNumber, "1");
        pDataSet->putAndInsertString(DCM_NumberOfStages, "1");
        pDataSet->putAndInsertString(DCM_ViewNumber, "1");
        pDataSet->putAndInsertString(DCM_NumberOfViewsInStage, "1");

        /* 添加 Image 信息 */
        status = pDataSet->putAndInsertString(DCM_ImageType, "ORIGINAL\\PRIMARY\\AXIAL");
        status = pDataSet->putAndInsertString(DCM_ContentDate, "20141221");
        status = pDataSet->putAndInsertString(DCM_ContentTime, "200700");
        status = pDataSet->putAndInsertString(DCM_SeriesNumber, "0");
        status = pDataSet->putAndInsertString(DCM_InstanceNumber, "0");
        status = pDataSet->putAndInsertUint16(DCM_SamplesPerPixel, newSamplePerPixel);
        //status = pDataSet->putAndInsertString(DCM_PixelSpacing, "0.3\\0.3");
        status = pDataSet->putAndInsertString(DCM_Modality, "US");
        status = pDataSet->putAndInsertUint16(DCM_PlanarConfiguration, 0);
        status = pDataSet->putAndInsertString(DCM_NumberOfFrames, "1");
        status = pDataSet->putAndInsertUint16(DCM_Rows, height);
        status = pDataSet->putAndInsertUint16(DCM_Columns, width);
        status = pDataSet->putAndInsertUint16(DCM_BitsAllocated, 8);
        status = pDataSet->putAndInsertUint16(DCM_BitsStored, 8);
        status = pDataSet->putAndInsertUint16(DCM_HighBit, 7);
        if (newSamplePerPixel>=3)
        {
            status = pDataSet->putAndInsertOFStringArray(DCM_PhotometricInterpretation, "RGB");
        }
        //status = pDataSet->putAndInsertString(DCM_PixelAspectRatio, "1\\1");
        status = pDataSet->putAndInsertUint16(DCM_PixelRepresentation, 0);


        // 自行组织 像素格式
        int newPixelDataLen = width * height * newSamplePerPixel;
        Uint8 *pImgPixelValues = new Uint8[newPixelDataLen];
        int oldPixelCntPerLine = width * oldSamplePerPixel;
        int newPixelCntPerLine = width * newSamplePerPixel;
        int pixelIndex = 0;
        QRgb tempPixelValue = 0;
        for (int i=0; i<height; ++i)
        {
            for (int j=0; j<width; ++j)
            {
                tempPixelValue = imgTest.pixel(j, i);
                pixelIndex = i*newPixelCntPerLine + j*newSamplePerPixel;
                pImgPixelValues[pixelIndex + 0] = qRed(tempPixelValue);
                pImgPixelValues[pixelIndex + 1] = qGreen(tempPixelValue);
                pImgPixelValues[pixelIndex + 2] = qBlue(tempPixelValue);

                //for (int k = 0; k < newSamplePerPixel; ++k)
                //{
                //    pImgPixelValues[i*newPixelCntPerLine + j*newSamplePerPixel + k] = imgBits[i*oldPixelCntPerLine + j*oldSamplePerPixel + newSamplePerPixel - k - 1];
                //}
            }
        }
        status = pDataSet->putAndInsertUint8Array(DCM_PixelData, (Uint8*)pImgPixelValues, newPixelDataLen);
        delete []pImgPixelValues;

        status = pFileFormat->saveFile(saveFilename.toStdString().c_str(), EXS_LittleEndianImplicit);
        if (status.bad())
        {
            LogUtil::Error(CODE_LOCATION, "Error: %s", status.text());
        }
    }
}

void MainTabPageFirst::ReadImageByQImageMulti()
{
    QString curAppPath = QCoreApplication::applicationDirPath();
    //QString imgFileName = QFileDialog::getOpenFileName(this, tr("Open File"), curAppPath, tr("Images (*.jpg *.bmp);;All files (*.*)"));
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), curAppPath,
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    const int imgCount = 100;

    if (!dir.isEmpty())
    {
        int width = 0, height = 0;
        int samplePerPixel = 1;
        int byteCount = 0;
        const uchar* imgBits = Q_NULLPTR;

        Uint8 *pImgPixelValues[imgCount] = { Q_NULLPTR };
        Uint8 *pAllImageData = Q_NULLPTR;
        QRgb tempPixelValue = 0;
        int byteCountPerLine = 0;
        int pixelIndex = 0;

        QString tempImgName;
        QImage imgTest;
        for (int imgID = 0; imgID < imgCount; ++imgID)
        {
            tempImgName = QString("%1/%2.bmp").arg(dir).arg(imgID+1);
            imgTest.load(tempImgName);

            width = imgTest.width();
            height = imgTest.height();
            samplePerPixel = imgTest.depth() / 8;
            samplePerPixel = samplePerPixel > 3 ? 3 : samplePerPixel; // DICOM 最多 3 通道
            byteCount += imgTest.byteCount();
            imgBits = imgTest.constBits();

            // 自行组织 像素格式
            int newPixelDataLen = width * height * samplePerPixel;
            pImgPixelValues[imgID] = new Uint8[newPixelDataLen];
            byteCountPerLine = width * samplePerPixel;
            pixelIndex = 0;
            tempPixelValue = 0;
            for (int i = 0; i < height; ++i)
            {
                for (int j = 0; j < width; ++j)
                {
                    tempPixelValue = imgTest.pixel(j, i);
                    pixelIndex = i*byteCountPerLine + j*samplePerPixel;
                    pImgPixelValues[imgID][pixelIndex + 0] = qRed(tempPixelValue);
                    pImgPixelValues[imgID][pixelIndex + 1] = qGreen(tempPixelValue);
                    pImgPixelValues[imgID][pixelIndex + 2] = qBlue(tempPixelValue);
                }
            }
        }

        QString saveFilename = dir + "/" + "AllInOne.dcm";

        OFCondition status;
        DcmFileFormat *pFileFormat = new DcmFileFormat();
        DcmDataset *pDataSet = pFileFormat->getDataset();

        pDataSet->putAndInsertString(DCM_ProtocolName, "Page - Full");
        pDataSet->putAndInsertString(DCM_SpecificCharacterSet, "GB18030");

        /* sop class uid */
        char uid[100];
        dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT);
        pDataSet->putAndInsertString(DCM_SOPInstanceUID, uid);
        pDataSet->putAndInsertString(DCM_SOPClassUID, UID_UltrasoundMultiframeImageStorage);

        /* 添加 Patient 信息 */
        pDataSet->putAndInsertString(DCM_AccessionNumber, "MGI001");
        pDataSet->putAndInsertString(DCM_PatientName, "test测试中文");
        pDataSet->putAndInsertString(DCM_PatientID, "110119");
        pDataSet->putAndInsertString(DCM_PatientBirthDate, "20180803");
        pDataSet->putAndInsertString(DCM_PatientSex, "M");
        pDataSet->putAndInsertString(DCM_PatientOrientation, "");
        pDataSet->putAndInsertString(DCM_Laterality, "");
        pDataSet->putAndInsertString(DCM_ImageComments, "");

        QTime curTime = QTime::currentTime();
        QString curTimeString = curTime.toString("hhmmss.zzz");
        QString curDateString = curTime.toString("yyyyMMdd");

        QDateTime curDateTime = QDateTime::currentDateTime();
        QString curTimeStringTest = curDateTime.toString("hhmmss.zzz");
        QString curDateStringTest = curDateTime.toString("yyyyMMdd");

        /* 添加 Study 信息 */
        pDataSet->putAndInsertString(DCM_StudyDate, curDateStringTest.toStdString().c_str());
        pDataSet->putAndInsertString(DCM_ContentDate, curDateStringTest.toStdString().c_str());
        pDataSet->putAndInsertString(DCM_StudyTime, curTimeStringTest.toStdString().c_str());
        pDataSet->putAndInsertString(DCM_ContentTime, curTimeStringTest.toStdString().c_str());
        dcmGenerateUniqueIdentifier(uid, SITE_STUDY_UID_ROOT);
        pDataSet->putAndInsertString(DCM_StudyID, uid);
        dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT);
        pDataSet->putAndInsertString(DCM_StudyInstanceUID, uid);

        /* 添加 Series 信息 */
        pDataSet->putAndInsertString(DCM_SeriesDate, "20141221");
        pDataSet->putAndInsertString(DCM_SeriesTime, "195411");
        memset(uid, 0, sizeof(char) * 100);
        dcmGenerateUniqueIdentifier(uid, SITE_SERIES_UID_ROOT);
        pDataSet->putAndInsertString(DCM_SeriesInstanceUID, uid);

        /* 添加 厂商 信息 */
        pDataSet->putAndInsertString(DCM_Manufacturer, "Nova Microsonics");
        pDataSet->putAndInsertString(DCM_ReferringPhysicianName, "ReferringName");

        /* 添加 Stage 信息 */
        pDataSet->putAndInsertString(DCM_StageName, "StageTest");
        pDataSet->putAndInsertString(DCM_StageNumber, "1");
        pDataSet->putAndInsertString(DCM_NumberOfStages, "1");
        pDataSet->putAndInsertString(DCM_ViewNumber, "1");
        pDataSet->putAndInsertString(DCM_NumberOfViewsInStage, "1");

        /* 添加 Image 信息 */
        status = pDataSet->putAndInsertString(DCM_ImageType, "ORIGINAL\\PRIMARY\\AXIAL");
        status = pDataSet->putAndInsertString(DCM_ContentDate, "20141221");
        status = pDataSet->putAndInsertString(DCM_ContentTime, "200700");
        status = pDataSet->putAndInsertString(DCM_SeriesNumber, "0");
        status = pDataSet->putAndInsertString(DCM_InstanceNumber, "0");
        status = pDataSet->putAndInsertString(DCM_Modality, "US");
        status = pDataSet->putAndInsertUint16(DCM_PlanarConfiguration, 0);
        status = pDataSet->putAndInsertString(DCM_NumberOfFrames, QString("%1").arg(imgCount).toStdString().c_str());
        status = pDataSet->putAndInsertUint16(DCM_BitsAllocated, 8);
        status = pDataSet->putAndInsertUint16(DCM_BitsStored, 8);
        status = pDataSet->putAndInsertUint16(DCM_HighBit, 7);

        status = pDataSet->putAndInsertUint16(DCM_SamplesPerPixel, samplePerPixel);
        status = pDataSet->putAndInsertUint16(DCM_Rows, height);
        status = pDataSet->putAndInsertUint16(DCM_Columns, width);

        if (samplePerPixel >= 3)
        {
            status = pDataSet->putAndInsertOFStringArray(DCM_PhotometricInterpretation, "RGB");
        }
        status = pDataSet->putAndInsertUint16(DCM_PixelRepresentation, 0);

        int bytesPerImage = width * height * samplePerPixel;
        pAllImageData = new Uint8[bytesPerImage*imgCount];
        Uint8 *pTempImageData = pAllImageData;
        for (int imgID=0; imgID<imgCount; ++imgID)
        {
            if (pImgPixelValues[imgID] != Q_NULLPTR)
            {
                memcpy_s(pTempImageData, bytesPerImage, pImgPixelValues[imgID], bytesPerImage);
                pTempImageData += bytesPerImage;
                delete[]pImgPixelValues[imgID];
                pImgPixelValues[imgID] = Q_NULLPTR;
            }
        }
        status = pDataSet->putAndInsertUint8Array(DCM_PixelData, (Uint8*)pAllImageData, bytesPerImage*imgCount);
        delete[]pAllImageData;

        status = pFileFormat->saveFile(saveFilename.toStdString().c_str(), EXS_LittleEndianImplicit);
        if (status.bad())
        {
            LogUtil::Error(CODE_LOCATION, "Error: %s", status.text());
        }
    }
}

void MainTabPageFirst::ReadJpegAndCopyToDicom()
{
    QString curAppPath = QCoreApplication::applicationDirPath();
    QString imgFileName = QFileDialog::getOpenFileName(this, tr("Open File"), curAppPath, tr("Images (*.jpg *.bmp);;All files (*.*)"));

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

        OFString filename = OFString(imgFileName.toStdString().c_str());

        I2DImgSource *pImgSource = Q_NULLPTR;
        if (imgFileName.endsWith(".jpg", Qt::CaseInsensitive))
        {
            pImgSource = new I2DJpegSource();
        }
        else if (imgFileName.endsWith(".bmp", Qt::CaseInsensitive))
        {
            pImgSource = new I2DBmpSource();
        }
        if (pImgSource != Q_NULLPTR)
        {
            pImgSource->setImageFile(filename);
            char* pixData = NULL;
            Uint16 rows, cols, samplePerPixel, bitsAlloc, bitsStored, highBit, pixelRpr, planConf, pixAspectH, pixAspectV;
            OFString photoMetrInt;
            Uint32 length;
            E_TransferSyntax ts = EXS_Unknown;
            status = pImgSource->readPixelData(rows, cols, samplePerPixel, photoMetrInt, bitsAlloc, bitsStored, highBit, pixelRpr, planConf, pixAspectH, pixAspectV, pixData, length, ts);
            if (status.bad())
            {
                LogUtil::Error(CODE_LOCATION, "read pixel failed: %s", status.text());
            }

            pDataSet->putAndInsertString(DCM_ProtocolName, "Page - Full");

            /* 添加 Patient 信息 */
            pDataSet->putAndInsertString(DCM_AccessionNumber, "MGI001");
            pDataSet->putAndInsertString(DCM_PatientName, "Testing");
            pDataSet->putAndInsertString(DCM_PatientID, "110119");
            pDataSet->putAndInsertString(DCM_PatientBirthDate, "20180803");
            pDataSet->putAndInsertString(DCM_PatientSex, "M");
            pDataSet->putAndInsertString(DCM_PatientOrientation, "");
            pDataSet->putAndInsertString(DCM_Laterality, "");
            pDataSet->putAndInsertString(DCM_ImageComments, "");

            /* 添加 Study 信息 */
            pDataSet->putAndInsertString(DCM_StudyDate, "20180803");
            pDataSet->putAndInsertString(DCM_ContentDate, "20180803");
            pDataSet->putAndInsertString(DCM_StudyTime, "182230");
            pDataSet->putAndInsertString(DCM_ContentTime, "182230");
            char uid[100];
            dcmGenerateUniqueIdentifier(uid, SITE_STUDY_UID_ROOT);
            pDataSet->putAndInsertString(DCM_StudyID, uid);
            dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT);
            pDataSet->putAndInsertString(DCM_StudyInstanceUID, uid);

            /* 添加 Series 信息 */
            pDataSet->putAndInsertString(DCM_SeriesDate, "20141221");
            pDataSet->putAndInsertString(DCM_SeriesTime, "195411");
            memset(uid, 0, sizeof(char) * 100);
            dcmGenerateUniqueIdentifier(uid, SITE_SERIES_UID_ROOT);
            pDataSet->putAndInsertString(DCM_SeriesInstanceUID, uid);

            /* 添加 厂商 信息 */
            pDataSet->putAndInsertString(DCM_Manufacturer, "Nova Microsonics");
            pDataSet->putAndInsertString(DCM_ReferringPhysicianName, "ReferringName");

            /* 添加 Stage 信息 */
            pDataSet->putAndInsertString(DCM_StageName, "StageTest");
            pDataSet->putAndInsertString(DCM_StageNumber, "1");
            pDataSet->putAndInsertString(DCM_NumberOfStages, "1");
            pDataSet->putAndInsertString(DCM_ViewNumber, "1");
            pDataSet->putAndInsertString(DCM_NumberOfViewsInStage, "1");

            /* 添加 Image 信息 */
            status = pDataSet->putAndInsertString(DCM_ImageType, "ORIGINAL\\PRIMARY\\AXIAL");
            status = pDataSet->putAndInsertString(DCM_ContentDate, "20141221");
            status = pDataSet->putAndInsertString(DCM_ContentTime, "200700");
            status = pDataSet->putAndInsertString(DCM_SeriesNumber, "0");
            status = pDataSet->putAndInsertString(DCM_InstanceNumber, "0");
            status = pDataSet->putAndInsertUint16(DCM_SamplesPerPixel, samplePerPixel);
            //status = pDataSet->putAndInsertString(DCM_PixelSpacing, "0.3\\0.3");
            status = pDataSet->putAndInsertString(DCM_Modality, "US");
            status = pDataSet->putAndInsertUint16(DCM_PlanarConfiguration, 0);
            status = pDataSet->putAndInsertString(DCM_NumberOfFrames, "1");
            status = pDataSet->putAndInsertUint16(DCM_Rows, rows);
            status = pDataSet->putAndInsertUint16(DCM_Columns, cols);
            status = pDataSet->putAndInsertUint16(DCM_BitsAllocated, bitsAlloc);
            status = pDataSet->putAndInsertUint16(DCM_BitsStored, bitsStored);
            status = pDataSet->putAndInsertUint16(DCM_HighBit, highBit);
            status = pDataSet->putAndInsertOFStringArray(DCM_PhotometricInterpretation, photoMetrInt);
            //status = pDataSet->putAndInsertString(DCM_PixelAspectRatio, "1\\1");
            status = pDataSet->putAndInsertUint16(DCM_PixelRepresentation, 0);
            status = pDataSet->putAndInsertUint8Array(DCM_PixelData, (Uint8*)pixData, length);

            status = pFileFormat->saveFile(saveFilename.toStdString().c_str(), ts);
            if (status.bad())
            {
                LogUtil::Error(CODE_LOCATION, "Error: %s", status.text());
            }

            delete pImgSource;
        }

        delete pFileFormat;
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

        /*	添加Study信息	*/
        pDataSet->putAndInsertString(DCM_StudyDate, "20141221");
        pDataSet->putAndInsertString(DCM_StudyTime, "195411");
        char uid[100];
        dcmGenerateUniqueIdentifier(uid, SITE_STUDY_UID_ROOT);
        pDataSet->putAndInsertString(DCM_StudyInstanceUID, uid);
        pDataSet->putAndInsertString(DCM_StudyID, "1111");


        /*	添加Series信息	*/
        pDataSet->putAndInsertString(DCM_SeriesDate, "20141221");
        pDataSet->putAndInsertString(DCM_SeriesTime, "195411");
        memset(uid, 0, sizeof(char) * 100);
        dcmGenerateUniqueIdentifier(uid, SITE_SERIES_UID_ROOT);
        pDataSet->putAndInsertString(DCM_SeriesInstanceUID, uid);

        /*	添加Image信息	*/
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

void MainTabPageFirst::SelfPaintImage8Bit()
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

        pDataSet->putAndInsertString(DCM_ProtocolName, "Page - Full");

        /* 添加 Patient 信息 */
        pDataSet->putAndInsertString(DCM_AccessionNumber, "MGI001");
        pDataSet->putAndInsertString(DCM_PatientName, "Testing");
        pDataSet->putAndInsertString(DCM_PatientID, "110119");
        pDataSet->putAndInsertString(DCM_PatientBirthDate, "20180803");
        pDataSet->putAndInsertString(DCM_PatientSex, "M");
        pDataSet->putAndInsertString(DCM_PatientOrientation, "");
        pDataSet->putAndInsertString(DCM_Laterality, "");
        pDataSet->putAndInsertString(DCM_ImageComments, "");

        /* 添加 Study 信息 */
        pDataSet->putAndInsertString(DCM_StudyDate, "20180803");
        pDataSet->putAndInsertString(DCM_ContentDate, "20180803");
        pDataSet->putAndInsertString(DCM_StudyTime, "182230");
        pDataSet->putAndInsertString(DCM_ContentTime, "182230");
        char uid[100];
        dcmGenerateUniqueIdentifier(uid, SITE_STUDY_UID_ROOT);
        pDataSet->putAndInsertString(DCM_StudyID, uid);
        dcmGenerateUniqueIdentifier(uid, SITE_INSTANCE_UID_ROOT);
        pDataSet->putAndInsertString(DCM_StudyInstanceUID, uid);

        /* 添加 Series 信息 */
        pDataSet->putAndInsertString(DCM_SeriesDate, "20141221");
        pDataSet->putAndInsertString(DCM_SeriesTime, "195411");
        memset(uid, 0, sizeof(char) * 100);
        dcmGenerateUniqueIdentifier(uid, SITE_SERIES_UID_ROOT);
        pDataSet->putAndInsertString(DCM_SeriesInstanceUID, uid);

        /* 添加 厂商 信息 */
        pDataSet->putAndInsertString(DCM_Manufacturer, "Nova Microsonics");
        pDataSet->putAndInsertString(DCM_ReferringPhysicianName, "ReferringName");

        /* 添加 Stage 信息 */
        pDataSet->putAndInsertString(DCM_StageName, "StageTest");
        pDataSet->putAndInsertString(DCM_StageNumber, "1");
        pDataSet->putAndInsertString(DCM_NumberOfStages, "1");
        pDataSet->putAndInsertString(DCM_ViewNumber, "1");
        pDataSet->putAndInsertString(DCM_NumberOfViewsInStage, "1");

        /* 添加 Image 信息 */
        pDataSet->putAndInsertString(DCM_ImageType, "");
        pDataSet->putAndInsertString(DCM_ContentDate, "20141221");
        pDataSet->putAndInsertString(DCM_ContentTime, "200700");
        pDataSet->putAndInsertString(DCM_SeriesNumber, "0");
        pDataSet->putAndInsertString(DCM_InstanceNumber, "0");
        pDataSet->putAndInsertString(DCM_SamplesPerPixel, "1");
        pDataSet->putAndInsertString(DCM_PhotometricInterpretation, "MONOCHROME2");
        //pDataSet->putAndInsertString(DCM_PixelSpacing, "0.3\\0.3");
        pDataSet->putAndInsertString(DCM_Modality, "US");
        pDataSet->putAndInsertUint16(DCM_SamplesPerPixel, 1);
        //pDataSet->putAndInsertString(DCM_NumberOfFrames, "1");
        pDataSet->putAndInsertUint16(DCM_PlanarConfiguration, 0);
        pDataSet->putAndInsertUint16(DCM_Rows, constHeight);
        pDataSet->putAndInsertUint16(DCM_Columns, constWidth);
        pDataSet->putAndInsertUint16(DCM_BitsAllocated, 8);
        pDataSet->putAndInsertUint16(DCM_BitsStored, 8);
        pDataSet->putAndInsertUint16(DCM_HighBit, 7);
        pDataSet->putAndInsertString(DCM_PixelAspectRatio, "1\\1");
        pDataSet->putAndInsertUint16(DCM_PixelRepresentation, 0);
        pDataSet->putAndInsertUint8Array(DCM_PixelData, (Uint8*)testImgBuf, constWidth*constHeight);

        /* 配合 MicroDICOM */
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
