#include "DicomFileParserTest.h"

#include <ostream>

#include <dcmtk/ofstd/ofcast.h>
#include <dcmtk/ofstd/offile.h>
#include <dcmtk/oflog/oflog.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcmetinf.h>
#include <dcmtk/dcmdata/dcuid.h>
#include <dcmtk/dcmdata/dcrledrg.h>
#include <dcmtk/dcmdata/dcpixel.h>
#include <dcmtk/dcmdata/dcovlay.h>
#include <dcmtk/dcmjpeg/djdecode.h>

#include <dcmtk/dcmdata/dcvrae.h>
#include <dcmtk/dcmdata/dcvras.h>
#include <dcmtk/dcmdata/dcvrat.h>
#include <dcmtk/dcmdata/dcvrcs.h>
#include <dcmtk/dcmdata/dcvrda.h>
#include <dcmtk/dcmdata/dcvrds.h>
#include <dcmtk/dcmdata/dcvrdt.h>
#include <dcmtk/dcmdata/dcvrfd.h>
#include <dcmtk/dcmdata/dcvrfl.h>
#include <dcmtk/dcmdata/dcvris.h>
#include <dcmtk/dcmdata/dcvrlo.h>
#include <dcmtk/dcmdata/dcvrlt.h>
#include <dcmtk/dcmdata/dcvrobow.h>
#include <dcmtk/dcmdata/dcvrod.h>
#include <dcmtk/dcmdata/dcvrof.h>
#include <dcmtk/dcmdata/dcvrol.h>
#include <dcmtk/dcmdata/dcvrpn.h>
#include <dcmtk/dcmdata/dcvrpobw.h>
#include <dcmtk/dcmdata/dcvrsh.h>
#include <dcmtk/dcmdata/dcvrsl.h>
#include <dcmtk/dcmdata/dcvrss.h>
#include <dcmtk/dcmdata/dcvrst.h>
#include <dcmtk/dcmdata/dcvrtm.h>
#include <dcmtk/dcmdata/dcvruc.h>
#include <dcmtk/dcmdata/dcvrui.h>
#include <dcmtk/dcmdata/dcvrul.h>
#include <dcmtk/dcmdata/dcvrulup.h>
#include <dcmtk/dcmdata/dcvrur.h>
#include <dcmtk/dcmdata/dcvrus.h>
#include <dcmtk/dcmdata/dcvrut.h>

static OFLogger gLogger = OFLog::getLogger("DicomTestLog");

DicomFileParserTest::DicomFileParserTest()
{
}

DicomFileParserTest::~DicomFileParserTest()
{
}

void DicomFileParserTest::TestEnter()
{
    //OFLog::configure(OFLogger::TRACE_LOG_LEVEL);
    OFLog::configure(OFLogger::DEBUG_LOG_LEVEL);

    this->ParseDicomFile();
}

void DicomFileParserTest::TestExit()
{
    ;
}

void DicomFileParserTest::ParseDicomFile()
{
    OFFilename dcmFilename("e:\\temp\\dicomtesting\\srdoc10\\image09.dcm");
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

            // Tag 遍历测试
            DcmObject *pTempObj = pDcmDataSet->nextInContainer(nullptr);
            DcmTag tempTag(0, 0);
            DcmElement *pTempElement = nullptr;
            std::string tempString = "null";
            char *pTempBuffer = nullptr;

            while (pTempObj != nullptr)
            {
                tempTag = pTempObj->getTag();
                if (tempTag.getEVR() == EVR_AS || tempTag.getEVR() == EVR_CS || tempTag.getEVR() == EVR_DA)
                {
                    pTempElement = OFstatic_cast(DcmElement *, pTempObj);
                    pTempElement->getString(pTempBuffer);
                }
                OFLOG_DEBUG(gLogger, " " << tempTag.toString() << " " << tempTag.getVRName() << " --> " << tempTag.getTagName()
                            << "  " << (pTempBuffer ? pTempBuffer : "null")
                            << "[len=" << pTempObj->getLengthField() << "]"
                );
                pTempBuffer = nullptr;

                //this->GetDcmElementStringValue(pTempObj, tempString);

                pTempObj = pDcmDataSet->nextInContainer(pTempObj);
            }

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

                if (pImgData8 != NULL || pImgData16 != NULL)
                {
                    //if (1 == samplePerPixel) //1 byte per pixel
                    //{
                    //    cv::Mat dst(m_height, m_width, CV_8UC1, cv::Scalar::all(0));
                    //    unsigned char* data = nullptr;
                    //    for (int i = 0; i < m_height; i++)
                    //    {
                    //        data = dst.ptr<unsigned char>(i);	//取得每一行的头指针 也可使用dst2.at<unsigned short>(i, j) = ?
                    //        for (int j = 0; j < m_width; j++)
                    //        {
                    //            *data++ = (unsigned char)((float)image16_data[i*m_width + j] * 255.0 / std::pow(2.0, bit_count) + 0.5);
                    //        }
                    //    }
                    //    cv::imshow("gray", dst);
                    //    cv::waitKey(0);
                    //}
                    //else if (3 == samplePerPixel) //3 bytes per pixel
                    //{
                    //    cv::Mat dst(m_height, m_width, CV_8UC3, cv::Scalar::all(0));
                    //    unsigned short* data = nullptr;
                    //    for (int i = 0; i < m_height; i++)
                    //    {
                    //        for (int j = 0; j < m_width; j++)
                    //        {
                    //            dst.at<cv::Vec3b>(i, j)[0] = *(image_data + i*m_width * 3 + j * 3 + 2);	//B	channel
                    //            dst.at<cv::Vec3b>(i, j)[1] = *(image_data + i*m_width * 3 + j * 3 + 1);	//G channel
                    //            dst.at<cv::Vec3b>(i, j)[2] = *(image_data + i*m_width * 3 + j * 3);		//R channel
                    //        }
                    //    }
                    //    cv::imshow("colored", dst);
                    //    cv::waitKey(0);
                    //}
                }
            }
            
        }
        else
        {
            OFLOG_ERROR(gLogger, "Load file failed: " << dcmFilename);
        }
    }
}
