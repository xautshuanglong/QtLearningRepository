#include "MainTabPageFirst.h"

#include <QImage>
#include <LogUtil.h>

#include <dcmtk/ofstd/offile.h>
#include <dcmtk/oflog/oflog.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcmetinf.h>
#include <dcmtk/dcmdata/dcuid.h>
#include <dcmtk/dcmdata/dcrledrg.h>
#include <dcmtk/dcmjpeg/djdecode.h>

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
    int imgWidth = 512;
    int imgHeight = 512;

    OFFilename dcmFilename("e:\\temp\\dicomtesting\\srdoc10\\image09.dcm");
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
            int rescaleSlope = atoi(pRescaleSlope);
            int rescaleIntercept = atoi(pRescaleIntercept);

            const char *pWindowCenter = Q_NULLPTR;
            const char *pWindowWidth = Q_NULLPTR;
            pDcmDataSet->findAndGetString(DCM_WindowCenter, pWindowCenter);
            pDcmDataSet->findAndGetString(DCM_WindowWidth, pWindowWidth);
            int windowCenter = atoi(pWindowCenter);
            int windowWidth = atoi(pWindowWidth);

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

                pDcmDataSet->chooseRepresentation(xfer, NULL);
                DcmElement* element = NULL;
                OFCondition result = pDcmDataSet->findAndGetElement(DCM_PixelData, element);
                if (result.bad() || element == NULL)
                    return;
                Uint8  *pImgData8 = nullptr;
                Uint16 *pImgData16 = nullptr;
                if (8 == bitStored)
                {
                    result = element->getUint8Array(pImgData8);
                }
                else if (8 < bitStored)
                {
                    result = element->getUint16Array(pImgData16);
                }

                if (pImgData16 != NULL)
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
                            pixelValue = pImgData16[i * imgWidth + j] *rescaleSlope + rescaleIntercept;
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
                        LogUtil::CollectData(CODE_LOCATION, LOG_COLLECTION_TYPE_0, tempMsg);
                    }
                    ui.imgContent->setPixmap(QPixmap::fromImage(imgTest));
                }
                else if (pImgData8 != NULL)
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


    return QWidget::showEvent(event);
}
