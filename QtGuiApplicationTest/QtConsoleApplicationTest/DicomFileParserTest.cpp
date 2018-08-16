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
                            << " NumOfValues:" << pTempObj->getNumberOfValues()
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

void DicomFileParserTest::GetDcmElementStringValue(DcmObject *pInDcmObj, std::string& outString)
{
    Uint16 valueLength = 0;
    DcmEVR dcmObjVR = pInDcmObj->getVR();
    switch (dcmObjVR)
    {
        // byte strings:
    case EVR_AE:
    {
        DcmApplicationEntity *pDcmAppEntity = OFstatic_cast(DcmApplicationEntity *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_AS:
    {
        DcmAgeString *pDcmAppEntity = OFstatic_cast(DcmAgeString *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_CS:
    {
        DcmCodeString *pDcmAppEntity = OFstatic_cast(DcmCodeString *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_DA:
    {
        DcmDate *pDcmAppEntity = OFstatic_cast(DcmDate *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_DS:
    {
        DcmDecimalString *pDcmAppEntity = OFstatic_cast(DcmDecimalString *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_DT:
    {
        DcmDateTime *pDcmAppEntity = OFstatic_cast(DcmDateTime *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_IS:
    {
        DcmIntegerString *pDcmAppEntity = OFstatic_cast(DcmIntegerString *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_TM:
    {
        DcmTime *pDcmAppEntity = OFstatic_cast(DcmTime *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_UI:
    {
        DcmUniqueIdentifier *pDcmAppEntity = OFstatic_cast(DcmUniqueIdentifier *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_UR:
    {
        DcmUniversalResourceIdentifierOrLocator *pDcmAppEntity = OFstatic_cast(DcmUniversalResourceIdentifierOrLocator *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;

        // character strings:
    case EVR_LO:
    {
        DcmLongString *pDcmAppEntity = OFstatic_cast(DcmLongString *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_LT:
    {
        DcmLongText *pDcmAppEntity = OFstatic_cast(DcmLongText *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_PN:
    {
        DcmPersonName *pDcmAppEntity = OFstatic_cast(DcmPersonName *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_SH:
    {
        DcmShortString *pDcmAppEntity = OFstatic_cast(DcmShortString *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_ST:
    {
        DcmShortText *pDcmAppEntity = OFstatic_cast(DcmShortText *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_UC:
    {
        DcmUnlimitedCharacters *pDcmAppEntity = OFstatic_cast(DcmUnlimitedCharacters *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_UT:
    {
        DcmUnlimitedText *pDcmAppEntity = OFstatic_cast(DcmUnlimitedText *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;

        // dependent on byte order:
    case EVR_AT:
    {
        DcmAttributeTag *pDcmAppEntity = OFstatic_cast(DcmAttributeTag *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_SS:
    {
        DcmSignedShort *pDcmAppEntity = OFstatic_cast(DcmSignedShort *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_xs: // according to DICOM standard
    case EVR_US:
    {
        DcmUnsignedShort *pDcmAppEntity = OFstatic_cast(DcmUnsignedShort *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_SL:
    {
        DcmSignedLong *pDcmAppEntity = OFstatic_cast(DcmSignedLong *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_up: // for (0004,eeee) according to DICOM standard
    case EVR_UL:
    {
        DcmTag ulupTag(pInDcmObj->getTag().getXTag());
        if (ulupTag.getEVR() == EVR_up)
        {
            DcmUnsignedLongOffset *pDcmULongOffset = OFstatic_cast(DcmUnsignedLongOffset *, pInDcmObj);
            valueLength = pDcmULongOffset->getLength();
            Uint32 tempValue;
            pDcmULongOffset->getUint32(tempValue);
        }
        else
        {
            DcmUnsignedLong *pDcmULong = OFstatic_cast(DcmUnsignedLong *, pInDcmObj);
            valueLength = pDcmULong->getLength();
            Uint32 tempValue;
            pDcmULong->getUint32(tempValue);
        }
    }
    break;
    case EVR_OL:
    {
        DcmOtherLong *pDcmAppEntity = OFstatic_cast(DcmOtherLong *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_FL:
    {
        DcmFloatingPointSingle *pDcmAppEntity = OFstatic_cast(DcmFloatingPointSingle *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_FD:
    {
        DcmFloatingPointDouble *pDcmAppEntity = OFstatic_cast(DcmFloatingPointDouble *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_OF:
    {
        DcmOtherFloat *pDcmAppEntity = OFstatic_cast(DcmOtherFloat *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_OD:
    {
        DcmOtherDouble *pDcmAppEntity = OFstatic_cast(DcmOtherDouble *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_SQ:
    {
        DcmSequenceOfItems *pDcmAppEntity = OFstatic_cast(DcmSequenceOfItems *, pInDcmObj);
        valueLength = pDcmAppEntity->getLength();
    }
        break;
    case EVR_na:
        //if (tag.getXTag() == DCM_Item)
        //    l_error = EC_InvalidTag;
        //else if (tag.getXTag() == DCM_SequenceDelimitationItem)
        //    l_error = EC_SequEnd;
        //else if (tag.getXTag() == DCM_ItemDelimitationItem)
        //    l_error = EC_ItemEnd;
        //else
        //    l_error = EC_InvalidTag;
        break;
    //case EVR_ox:
    //{
    //    DcmTag tag = pInDcmObj->getTag();
    //    if (tag == DCM_PixelData)
    //        DcmPixelData *pDcmAppEntity = OFstatic_cast(DcmPixelData *, pInDcmObj);
    //    else if (tag.getBaseTag() == DCM_OverlayData)
    //        DcmOverlayData *pDcmAppEntity = OFstatic_cast(DcmOverlayData *, pInDcmObj);
    //    else
    //        DcmOtherByteOtherWord *pDcmAppEntity = OFstatic_cast(DcmOtherByteOtherWord *, pInDcmObj);
    //    valueLength = pDcmAppEntity->getLength();
    //}
    //    break;

    //    // This case should only occur if we encounter an element with an invalid
    //    // "Pi" VR. Make sure this does not cause problems later on
    //case EVR_PixelData:
    //{
    //    DcmApplicationEntity *pDcmAppEntity = OFstatic_cast(DcmApplicationEntity *, pInDcmObj);
    //    valueLength = pDcmAppEntity->getLength();
    //}
    //    newElement = new DcmPixelData(tag, length);
    //    // set VR to OW to make sure that we never write/send the internal VR
    //    if (newElement) newElement->setVR(EVR_OW);
    //    break;

    //    // This case should only occur if we encounter an element with an invalid
    //    // "Ov" VR. Make sure this does not cause problems later on
    //case EVR_OverlayData:
    //{
    //    DcmApplicationEntity *pDcmAppEntity = OFstatic_cast(DcmApplicationEntity *, pInDcmObj);
    //    valueLength = pDcmAppEntity->getLength();
    //}
    //    newElement = new DcmOverlayData(tag, length);
    //    // set VR to OW to make sure that we never write/send the internal VR
    //    if (newElement) newElement->setVR(EVR_OW);
    //    break;

    //case EVR_lt:
    //{
    //    DcmApplicationEntity *pDcmAppEntity = OFstatic_cast(DcmApplicationEntity *, pInDcmObj);
    //    valueLength = pDcmAppEntity->getLength();
    //}
    //    newElement = new DcmOtherByteOtherWord(tag, length);
    //    break;

    //case EVR_OB:
    //case EVR_OW:
    //{
    //    DcmApplicationEntity *pDcmAppEntity = OFstatic_cast(DcmApplicationEntity *, pInDcmObj);
    //    valueLength = pDcmAppEntity->getLength();
    //}
    //    if (tag == DCM_PixelData)
    //        newElement = new DcmPixelData(tag, length);
    //    else if (tag.getBaseTag() == DCM_OverlayData)
    //        newElement = new DcmOverlayData(tag, length);
    //    else if ((tag == DCM_VOILUTSequence) && (length != DCM_UndefinedLength))
    //    {
    //        // this is an incorrectly encoded VOI LUT Sequence.
    //        // Real-world examples of this issue have been reported in 2016.
    //        if (dcmConvertVOILUTSequenceOWtoSQ.get())
    //        {
    //            // Silently fix the error by interpreting as a sequence.
    //            DcmTag newTag(tag);
    //            newTag.setVR(DcmVR(EVR_SQ)); // on writing we will handle this element as SQ, not OB/OW
    //            newElement = new DcmSequenceOfItems(newTag, length);
    //        }
    //        else {

    //            if (dcmIgnoreParsingErrors.get())
    //            {
    //                // ignore parse error, keep VR unchanged
    //                DCMDATA_WARN("DcmItem: VOI LUT Sequence with VR=OW and explicit length encountered.");
    //                newElement = new DcmOtherByteOtherWord(tag, length);
    //            }
    //            else
    //            {
    //                // bail out with an error
    //                DCMDATA_ERROR("DcmItem: VOI LUT Sequence with VR=OW and explicit length encountered.");
    //                l_error = EC_VOI_LUT_OBOW;
    //            }
    //        }
    //    }
    //    else
    //        if (length == DCM_UndefinedLength)
    //        {
    //            // The attribute is OB or OW but is encoded with undefined
    //            // length, and it is not Pixel Data. This is illegal.
    //            if (dcmConvertUndefinedLengthOBOWtoSQ.get())
    //            {
    //                // Assume that this is in fact a sequence so that we can
    //                // catch the sequence delimitation item.
    //                DcmTag newTag(tag);
    //                newTag.setVR(DcmVR(EVR_SQ)); // on writing we will handle this element as SQ, not OB/OW
    //                newElement = new DcmSequenceOfItems(newTag, length);
    //            }
    //            else {
    //                if (dcmIgnoreParsingErrors.get())
    //                {
    //                    // ignore parse error, keep VR unchanged
    //                    OFCondition tempcond = EC_UndefinedLengthOBOW;
    //                    DCMDATA_WARN("DcmItem: Parse error in " << tag << ": " << tempcond.text());
    //                    newElement = new DcmSequenceOfItems(tag, length);
    //                }
    //                else
    //                {
    //                    // bail out with an error
    //                    l_error = EC_UndefinedLengthOBOW;
    //                    DCMDATA_ERROR("DcmItem: Parse error in " << tag << ": " << l_error.text());
    //                }
    //            }
    //        }
    //        else {
    //            newElement = new DcmOtherByteOtherWord(tag, length);
    //        }
    //        break;

    //        // read unknown types as byte string:
    //case EVR_UNKNOWN:
    //case EVR_UNKNOWN2B:
    //case EVR_UN:
    //{
    //    DcmApplicationEntity *pDcmAppEntity = OFstatic_cast(DcmApplicationEntity *, pInDcmObj);
    //    valueLength = pDcmAppEntity->getLength();
    //}
    //    if (length == DCM_UndefinedLength)
    //    {
    //        // The attribute VR is UN with undefined length. Assume it is really
    //        // a sequence so that we can catch the sequence delimitation item.
    //        DcmTag newTag(tag);
    //        newTag.setVR(DcmVR(EVR_SQ)); // on writing we will handle this element as SQ, not UN
    //        if (dcmEnableCP246Support.get())
    //        {
    //            DCMDATA_WARN("Found element " << newTag << " with VR UN and undefined length, "
    //                         << "reading a sequence with transfer syntax LittleEndianImplicit (CP-246)");
    //        }
    //        else {
    //            DCMDATA_WARN("Found element " << newTag << " with VR UN and undefined length");
    //        }
    //        newElement = new DcmSequenceOfItems(newTag, length, dcmEnableCP246Support.get());
    //    }
    //    else {
    //        // defined length UN element, treat like OB
    //        newElement = new DcmOtherByteOtherWord(tag, length);
    //    }
    //    break;
    case EVR_item:
    case EVR_metainfo:
    case EVR_dataset:
    case EVR_fileFormat:
    case EVR_dicomDir:
    case EVR_dirRecord:
    case EVR_pixelSQ:
    case EVR_pixelItem:
        break;
    }
}
