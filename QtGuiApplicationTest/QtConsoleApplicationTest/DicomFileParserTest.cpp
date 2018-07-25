#include "DicomFileParserTest.h"

#include <dcmtk/ofstd/offile.h>
#include <dcmtk/oflog/oflog.h>
#include <dcmtk/dcmdata/dcfilefo.h>

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
        OFLOG_TRACE(gLogger, "Found file: " << dcmFilename);
        DcmFileFormat dcmFileFormat;
        OFCondition loadRes = dcmFileFormat.loadFile(dcmFilename);
        if (loadRes.good())
        {
            OFLOG_INFO(gLogger, "Load successfully: " << dcmFilename);
            DcmDataset *pDcmDataSet = dcmFileFormat.getDataset();
            pDcmDataSet->print(std::cout, DCMTypes::PF_shortenLongTagValues);
        }
        else
        {
            OFLOG_ERROR(gLogger, "Load file failed: " << dcmFilename);
        }
    }
}
