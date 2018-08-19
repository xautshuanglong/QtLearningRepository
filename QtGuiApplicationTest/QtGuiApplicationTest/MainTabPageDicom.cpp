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
    , mpImageData(Q_NULLPTR)
    , mRescaleSlope(1)
    , mRescaleIntercept(0)
    , mWindowCenter(128)
    , mWindowWidth(256)
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

    QStringList header;
    header << "DcmTagKey" << "DcmTagName" << "ElementValue";
    ui.tableDcmTag->setColumnCount(3);
    ui.tableDcmTag->setHorizontalHeaderLabels(header);
    ui.tableDcmTag->verticalHeader()->setVisible(false);
    ui.tableDcmTag->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.tableDcmTag->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.tableDcmTag->horizontalHeader()->setSectionsClickable(false);
    ui.tableDcmTag->setFocusPolicy(Qt::NoFocus);
}

MainTabPageDicom::~MainTabPageDicom()
{
    ClearDataBuffer(&mpImageData);
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
        this->LoadDicomFile(dcmFileName);
        this->GetDicomElementImage(mDcmImage);
        this->UpdateDcmTabTableContent();
        if (!mDcmImage.isNull())
        {
            ui.labelImage->setGeometry(0, 0, mDcmImage.width(), mDcmImage.height());
            ui.labelImage->setPixmap(QPixmap::fromImage(mDcmImage));
        }
    }
}

void MainTabPageDicom::on_action_save()
{
    int i = 0;
}

void MainTabPageDicom::LoadDicomFile(const QString& inDcmFilename)
{
    OFFilename dcmFilename(inDcmFilename.toStdString().c_str());
    //OFFilename dcmFilename("e:\\temp\\dicomtesting\\srdoc10\\image12.dcm");
    if (OFStandard::fileExists(dcmFilename))
    {
        OFCondition resCondition = EC_Normal;
        DcmFileFormat dcmFileFormat;
        resCondition = dcmFileFormat.loadFile(dcmFilename);
        if (resCondition.good())
        {
            DcmDataset *pDcmDataSet = dcmFileFormat.getDataset();
            E_TransferSyntax xfer = pDcmDataSet->getOriginalXfer();
            pDcmDataSet->findAndGetUint16(DCM_BitsStored, mBitStored); //获取像素的位数 bit
            pDcmDataSet->findAndGetUint16(DCM_SamplesPerPixel, mSamplePerPixel);
            pDcmDataSet->findAndGetUint16(DCM_Columns, mImageWidth);
            pDcmDataSet->findAndGetUint16(DCM_Rows, mImageHeight);

            const char *pRescaleSlope = Q_NULLPTR;
            const char *pRescaleIntercept = Q_NULLPTR;
            pDcmDataSet->findAndGetString(DCM_RescaleSlope, pRescaleSlope);
            pDcmDataSet->findAndGetString(DCM_RescaleIntercept, pRescaleIntercept);
            mRescaleSlope = pRescaleSlope == Q_NULLPTR ? 1 : atoi(pRescaleSlope);
            mRescaleIntercept = pRescaleIntercept == Q_NULLPTR ? 0 : atoi(pRescaleIntercept);

            const char *pWindowCenter = Q_NULLPTR;
            const char *pWindowWidth = Q_NULLPTR;
            pDcmDataSet->findAndGetString(DCM_WindowCenter, pWindowCenter);
            pDcmDataSet->findAndGetString(DCM_WindowWidth, pWindowWidth);
            mWindowCenter = pWindowCenter == Q_NULLPTR ? 128 : atoi(pWindowCenter);
            mWindowWidth = pWindowWidth == Q_NULLPTR ? 256 : atoi(pWindowWidth);

            // 遍历 DICOM 所有元素
            DcmObject *pTempObj = pDcmDataSet->nextInContainer(nullptr);
            DcmTag tempTag(0, 0);
            DcmElement *pTempElement = Q_NULLPTR;
            std::string tempString = "null";
            DcmElementInfo tempDcmEleInfo;
            mListDcmElementInfo.clear();

            while (pTempObj != Q_NULLPTR)
            {
                tempTag = pTempObj->getTag();
                tempDcmEleInfo.strDcmTagKey = tempTag.toString().c_str();
                tempDcmEleInfo.strDcmTagName = QString("[%1] %2").arg(tempTag.getVRName()).arg(tempTag.getTagName());
                this->GetDicomElementValue(tempDcmEleInfo.strDcmElementValue, pTempObj);
                mListDcmElementInfo.append(tempDcmEleInfo);

                pTempObj = pDcmDataSet->nextInContainer(pTempObj);
            }

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
            DcmElement* tempElement = NULL;
            OFCondition result = pDcmDataSet->findAndGetElement(DCM_PixelData, tempElement);
            if (result.good() || tempElement != NULL)
            {
                Uint32 pixDataLen = tempElement->getLengthField();
                ClearDataBuffer(&mpImageData);
                try
                {
                    mpImageData = new unsigned char[pixDataLen];
                }
                catch (std::bad_alloc* e)
                {
                    LogUtil::Error(CODE_LOCATION, "allocate memory failed: %s", e->what());
                }
                if (8 == mBitStored)
                {
                    Uint8 *pTempImgData = Q_NULLPTR;
                    result = tempElement->getUint8Array(pTempImgData);
                    if (result.good())
                    {
                        memcpy(mpImageData, pTempImgData, pixDataLen);
                    }
                }
                else if (16 == mBitStored)
                {
                    Uint16 *pTempImgData = Q_NULLPTR;
                    result = tempElement->getUint16Array(pTempImgData);
                    if (result.good())
                    {
                        memcpy(mpImageData, pTempImgData, pixDataLen);
                    }
                }
                else if (32 == mBitStored)
                {
                    Uint32 *pTempImgData = Q_NULLPTR;
                    result = tempElement->getUint32Array(pTempImgData);
                    if (result.good())
                    {
                        memcpy(mpImageData, pTempImgData, pixDataLen);
                    }
                }
            }
            if (result.bad())
            {
                LogUtil::Error(CODE_LOCATION, "Load file failed: %s", dcmFilename.getCharPointer());
            }
        }
        else 
        {
            LogUtil::Error(CODE_LOCATION, "Load file failed: %s", dcmFilename.getCharPointer());
        }
    }
}

void MainTabPageDicom::GetDicomElementImage(QImage& outImage)
{
    int pixValueMin = mWindowCenter - mWindowWidth / 2;
    int pixValueMax = mWindowCenter + mWindowWidth / 2;
    bool pixWindowHandleFlag = (mWindowCenter != 128 || mWindowWidth != 256); // 是否需要处理像素值窗口（非默认值）
    if (mBitStored == 8)
    {
        outImage = QImage(mImageWidth, mImageHeight, QImage::Format_RGB32);
        int *pRgbValue = new int[mSamplePerPixel];
        for (int i = 0; i < mImageHeight; ++i)
        {
            for (int j = 0; j < mImageWidth; ++j)
            {
                for (int k = 0; k < mSamplePerPixel; ++k)
                {
                    pRgbValue[k] = mpImageData[i * mImageWidth * mSamplePerPixel + j * mSamplePerPixel + k] * mRescaleSlope + mRescaleIntercept;
                    if (pixWindowHandleFlag)
                    {
                        if (pRgbValue[k] <= pixValueMin)
                        {
                            pRgbValue[k] = 0;
                        }
                        else if (pRgbValue[k] >= pixValueMax)
                        {
                            pRgbValue[k] = 255;
                        }
                        else
                        {
                            pRgbValue[k] = (int)((pRgbValue[k] - pixValueMin) * 255.0f / mWindowWidth);
                        }
                    }
                }
                outImage.setPixelColor(j, i, QColor(pRgbValue[0], pRgbValue[1], pRgbValue[2], 255));
            }
        }
        delete[]pRgbValue;
    }
    else if (mBitStored == 16)
    {
        ;
    }
    else if (mBitStored == 32)
    {
    }
}

void MainTabPageDicom::ConvertDicomToQImage(QString &inFilename, QImage **ppOutImage)
{
    int imgWidth = 512;
    int imgHeight = 512;
    QImage *pOutImage = Q_NULLPTR;

    OFFilename dcmFilename(inFilename.toStdString().c_str());
    //OFFilename dcmFilename("e:\\temp\\dicomtesting\\srdoc10\\image12.dcm");
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

            // 遍历 DICOM 所有元素
            DcmObject *pTempObj = pDcmDataSet->nextInContainer(nullptr);
            DcmTag tempTag(0, 0);
            DcmElement *pTempElement = Q_NULLPTR;
            std::string tempString = "null";
            DcmElementInfo tempDcmEleInfo;
            mListDcmElementInfo.clear();

            while (pTempObj != Q_NULLPTR)
            {
                tempTag = pTempObj->getTag();
                tempDcmEleInfo.strDcmTagKey = tempTag.toString().c_str();
                tempDcmEleInfo.strDcmTagName = QString("[%1] %2").arg(tempTag.getVRName()).arg(tempTag.getTagName());
                this->GetDicomElementValue(tempDcmEleInfo.strDcmElementValue, pTempObj);
                mListDcmElementInfo.append(tempDcmEleInfo);

                pTempObj = pDcmDataSet->nextInContainer(pTempObj);
            }

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
                pOutImage = new QImage(imgWidth, imgHeight, QImage::Format_RGB32);
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
                pOutImage = new QImage(imgWidth, imgHeight, QImage::Format_RGB32);
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

            pOutImage = new QImage(imgWidth, imgHeight, QImage::Format_RGB32);
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
    *ppOutImage = pOutImage;
}

void MainTabPageDicom::GetDicomElementValue(QString &outStrValue, DcmObject *pInDcmObj)
{
    char *pTempString = Q_NULLPTR;
    Float32 tempFloat32;
    Float64 tempFloat64;
    Sint16 tempSint16;
    Sint32 tempSint32;
    Uint16 tempUint16;
    Uint32 tempUint32;
    DcmTag tempTag = pInDcmObj->getTag();
    DcmElement *pTempElement = OFstatic_cast(DcmElement *, pInDcmObj);
    OFCondition res = EC_Normal;
    outStrValue = "unknown";
    switch (tempTag.getEVR())
    {
    case EVR_AE: /// application entity title
    case EVR_AS: /// age string
    case EVR_AT: /// attribute tag
    case EVR_CS: /// code string
    case EVR_DA: /// date string
    case EVR_DS: /// decimal string
    case EVR_DT: /// date time string
    case EVR_IS: /// integer string
    case EVR_LO: /// long string
    case EVR_LT: /// long text
    case EVR_OB: /// other byte
    case EVR_PN: /// person name
    case EVR_SH: /// short string
    case EVR_ST: /// short text
    case EVR_TM: /// time string
    case EVR_UC: /// unlimited characters
    case EVR_UT: /// unlimited text
    case EVR_UI: /// unique identifier
        res = pTempElement->getString(pTempString);
        res.good() ? outStrValue = pTempString : LogUtil::Debug(CODE_LOCATION, "getString error: %s", res.text());
        break;
    case EVR_FL: /// float single-precision
    case EVR_OF: /// other float
        res = pTempElement->getFloat32(tempFloat32);
        res.good() ? outStrValue = QString("%1").arg(tempFloat32) : LogUtil::Debug(CODE_LOCATION, "getFloat32 error: %s", res.text());
        break;
    case EVR_FD: /// float double-precision
    case EVR_OD: /// other double
        res = pTempElement->getFloat64(tempFloat64);
        res.good() ? outStrValue = QString("%1").arg(tempFloat64) : LogUtil::Debug(CODE_LOCATION, "getFloat64 error: %s", res.text());
        break;
    case EVR_SS: /// signed short
        res = pTempElement->getSint16(tempSint16);
        res.good() ? outStrValue = QString("%1").arg(tempSint16) : LogUtil::Debug(CODE_LOCATION, "getSint16 error: %s", res.text());
        break;
    case EVR_OL: /// other long
    case EVR_SL: /// signed long
        res = pTempElement->getSint32(tempSint32);
        res.good() ? outStrValue = QString("%1").arg(tempFloat32) : LogUtil::Debug(CODE_LOCATION, "getSint32 error: %s", res.text());
        break;
    case EVR_US: /// unsigned short
        res = pTempElement->getUint16(tempUint16);
        res.good() ? outStrValue = QString("%1").arg(tempUint16) : LogUtil::Debug(CODE_LOCATION, "getUint16 error: %s", res.text());
        break;
    case EVR_UL: /// unsigned long
        res = pTempElement->getUint32(tempUint32);
        res.good() ? outStrValue = QString("%1").arg(tempUint32) : LogUtil::Debug(CODE_LOCATION, "getUint32 error: %s", res.text());
        break;
    case EVR_OW: /// other word
    case EVR_SQ: /// sequence of items
    case EVR_UR: /// universal resource identifier or universal resource locator (URI/URL)
    case EVR_ox: /// OB or OW depending on context
    case EVR_xs: /// SS or US depending on context
    case EVR_lt: /// US, SS or OW depending on context, used for LUT Data (thus the name)
    case EVR_na: /// na="not applicable", for data which has no VR
    case EVR_up: /// up="unsigned pointer", used internally for DICOMDIR support
    case EVR_item: /// used internally for items
    case EVR_metainfo: /// used internally for meta info datasets
    case EVR_dataset: /// used internally for datasets
    case EVR_fileFormat: /// used internally for DICOM files
    case EVR_dicomDir: /// used internally for DICOMDIR objects
    case EVR_dirRecord: /// used internally for DICOMDIR records
    case EVR_pixelSQ: /// used internally for pixel sequences in a compressed image
    case EVR_pixelItem: /// used internally for pixel items in a compressed image
    case EVR_UNKNOWN: /// used internally for elements with unknown VR (encoded with 4-byte length field in explicit VR)
    case EVR_UN: /// unknown value representation
    case EVR_PixelData: /// used internally for uncompressed pixel data
    case EVR_OverlayData: /// used internally for overlay data
    case EVR_UNKNOWN2B: /// used internally for elements with unknown VR with 2-byte length field in explicit VR
        break;
    default:
        break;
    }
}

void MainTabPageDicom::UpdateDcmTabTableContent()
{
    ui.tableDcmTag->clearContents();
    int listCount = mListDcmElementInfo.count();
    int rowCount = ui.tableDcmTag->rowCount();
    for (int i = 0; i < listCount; ++i)
    {
        if (i >= rowCount)
        {
            ui.tableDcmTag->insertRow(i);
        }
        ui.tableDcmTag->setItem(i, 0, new QTableWidgetItem(mListDcmElementInfo.at(i).strDcmTagKey));
        ui.tableDcmTag->setItem(i, 1, new QTableWidgetItem(mListDcmElementInfo.at(i).strDcmTagName));
        ui.tableDcmTag->setItem(i, 2, new QTableWidgetItem(mListDcmElementInfo.at(i).strDcmElementValue));
    }
    for (int i = rowCount;i >= listCount; --i)
    {
        ui.tableDcmTag->removeRow(i);
    }
}

void MainTabPageDicom::ClearDataBuffer(unsigned char **pDataBuffer)
{
    if (pDataBuffer != Q_NULLPTR && *pDataBuffer != Q_NULLPTR)
    {
        delete[] * pDataBuffer;
        *pDataBuffer = Q_NULLPTR;
    }
}
