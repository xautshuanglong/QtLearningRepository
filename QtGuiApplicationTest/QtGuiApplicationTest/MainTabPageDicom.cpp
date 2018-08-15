#include "MainTabPageDicom.h"

#include <QSplitter>
#include <QLabel>
#include <QToolBar>
#include <QFileDialog>
#include <QImage>

// DICOM  dcmtk
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

#include <LogUtil.h>

MainTabPageDicom::MainTabPageDicom(QWidget *parent /* = Q_NULLPTR */)
    : QWidget(parent)
    , mpMainSpliter(Q_NULLPTR)
{
    ui.setupUi(this);

    //this->setStyleSheet("background: green;");
    //ui.labelImage->setStyleSheet("background: rgb(12,23,56);");
    //ui.tableDcmTag->setStyleSheet("background:red;");

    QAction *pActionOpen = new QAction(QIcon(":/AppImages/Resources/images/open.png"), "Open");
    QAction *pActionSave = new QAction(QIcon(":/AppImages/Resources/images/save.png"), "Open");

    QToolBar *pDcmToolBar = new QToolBar();
    pDcmToolBar->addAction(pActionOpen);
    pDcmToolBar->addAction(pActionSave);
    ui.toolbarHLayout->insertWidget(0, pDcmToolBar);

    mpMainSpliter = new QSplitter(Qt::Horizontal, this);
    mpMainSpliter->setGeometry(this->frameGeometry());
    mpMainSpliter->addWidget(ui.leftWidget);
    mpMainSpliter->addWidget(ui.tableDcmTag);
    mpMainSpliter->setStretchFactor(0, 8);
    mpMainSpliter->setStretchFactor(0, 1);

    this->connect(pActionOpen, SIGNAL(triggered()), SLOT(on_action_open()));
    this->connect(pActionSave, SIGNAL(triggered()), SLOT(on_action_save()));
}

MainTabPageDicom::~MainTabPageDicom()
{
}

void MainTabPageDicom::resizeEvent(QResizeEvent *event)
{
    mpMainSpliter->setGeometry(this->frameGeometry());
}

void MainTabPageDicom::on_action_open()
{
    QString curAppPath = QCoreApplication::applicationDirPath();
    QString dcmFileName = QFileDialog::getOpenFileName(this, tr("Open File"), curAppPath, tr("DICOM (*.dcm)"));
    if (!dcmFileName.isEmpty())
    {
        QImage dcmImage(100, 100, QImage::Format_RGB32);
        ConvertDicomToQImage(dcmFileName, &dcmImage);
        int imgWidth = dcmImage.width();
        int imgHeight = dcmImage.height();
        ui.labelImage->setGeometry(0, 0, dcmImage.width(), dcmImage.height());
        ui.labelImage->setPixmap(QPixmap::fromImage(dcmImage));
    }
}

void MainTabPageDicom::on_action_save()
{
    int i = 0;
}

void MainTabPageDicom::ConvertDicomToQImage(QString &inFilename, QImage *pOutImage)
{
    int imgWidth = 512;
    int imgHeight = 512;

    OFFilename dcmFilename(inFilename.toStdString().c_str());
    //    OFFilename dcmFilename("e:\\temp\\dicomtesting\\srdoc10\\image12.dcm");
    if (OFStandard::fileExists(dcmFilename))
    {
        LogUtil::Debug(CODE_LOCATION, "Found file: %s", dcmFilename.getCharPointer());
        OFCondition resCondition = EC_Normal;
        DcmFileFormat dcmFileFormat;
        resCondition = dcmFileFormat.loadFile(dcmFilename);
        if (resCondition.good())
        {
            LogUtil::Debug(CODE_LOCATION, "Load successfully: ", dcmFilename.getCharPointer());
            DcmDataset *pDcmDataSet = dcmFileFormat.getDataset();

            pDcmDataSet->print(std::cout, DCMTypes::PF_shortenLongTagValues);

            E_TransferSyntax xfer = pDcmDataSet->getOriginalXfer();
            LogUtil::Debug(CODE_LOCATION, "E_TransferSyntax = %d", xfer);

            unsigned short bitStored = 0;
            pDcmDataSet->findAndGetUint16(DCM_BitsStored, bitStored); //获取像素的位数 bit
            LogUtil::Debug(CODE_LOCATION, "bitStored: %d", bitStored);

            OFString photoInterpretation;
            pDcmDataSet->findAndGetOFString(DCM_PhotometricInterpretation, photoInterpretation); //DCM图片的图像模式
            LogUtil::Debug(CODE_LOCATION, "photoInterpretation: %s", photoInterpretation.c_str());

            unsigned short samplePerPixel = 0;
            pDcmDataSet->findAndGetUint16(DCM_SamplesPerPixel, samplePerPixel);
            LogUtil::Debug(CODE_LOCATION, "samplePerPixel: %d", samplePerPixel);

            unsigned short mImgWidth; //获取图像的窗宽高
            unsigned short mImgHeight;
            pDcmDataSet->findAndGetUint16(DCM_Columns, mImgWidth);
            pDcmDataSet->findAndGetUint16(DCM_Rows, mImgHeight);
            LogUtil::Debug(CODE_LOCATION, "imageSize: %d x %d", mImgWidth, mImgHeight);
            imgWidth = mImgWidth;
            imgHeight = mImgHeight;
            pOutImage->scaled(imgWidth, imgHeight);

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
            resCondition = dcmFileFormat.getMetaInfo()->findAndGetString(DCM_TransferSyntaxUID, transferSyntax);
            if (resCondition.good())
            {
                LogUtil::Debug(CODE_LOCATION, "TransferSyntaxUID: %s", transferSyntax);
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
                LogUtil::Debug(CODE_LOCATION, "Find DCM_TransferSyntaxUID error: %s", resCondition.text());
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
                        pOutImage->setPixelColor(j, i, QColor(pixelValue, pixelValue, pixelValue, 255));
                    }
                }
            }
            else if (pImgData8 != Q_NULLPTR)
            {
                int minSize = imgWidth < imgHeight ? imgWidth : imgHeight;
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
                        pOutImage->setPixelColor(j, i, QColor(pRgbValue[0], pRgbValue[1], pRgbValue[2], 255));
                    }
                }
                delete[]pRgbValue;
            }
            else if (pImgData32 != Q_NULLPTR)
            {
            }

        }
        else
        {
            LogUtil::Error(CODE_LOCATION, "Load file failed: %s", dcmFilename.getCharPointer());

            int minSize = imgWidth < imgHeight ? imgWidth : imgHeight;
            for (int i = 0; i < imgHeight; ++i)
            {
                for (int j = 0; j < imgWidth; ++j)
                {
                    if (i == j || i + j == minSize || i == imgHeight / 2 || i == imgHeight - 1 || i == 0 || j == 0 || j == imgWidth / 2 || j == imgWidth - 1)
                    {
                        pOutImage->setPixelColor(i, j, QColor(255, 0, 0, 255));
                    }
                    else
                    {
                        pOutImage->setPixelColor(i, j, QColor(255, 255, 255, 255));
                    }
                }
            }
        }
    }
}
