#pragma once

class XmlReportGenerator
{
public:
    XmlReportGenerator();
    ~XmlReportGenerator();

    void CreateReport();
    void SaveReportAsXml();

private:
    void CreateReportHeader();
    void CreateReportBody();
    void CreateReportFooter();
};
