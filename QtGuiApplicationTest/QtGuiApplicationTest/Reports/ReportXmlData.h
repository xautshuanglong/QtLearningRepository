#ifndef REPORT_XML_DATA_H
#define REPORT_XML_DATA_H

#include <QString>
#include <QList>
#include <QStringList>

class QDomDocument;
class QDomElement;

/************************************************************************/
/* ���������� �� UI ��ϱ����������л��� xml���Ա� fop ��������Ű档       */
/************************************************************************/
class ReportXmlData
{
    typedef struct tagPatientInfoItem
    {
        QString strRemark;
        QString strLabel;
        QString strValue;
    }PatientInfoItem;

public:
    ReportXmlData();
    ~ReportXmlData();

    void SaveReportAsXml(const QString& xmlFilename);
    void MakeDefaultData();

    void ClearPatientInfo();
    void AddPatientInfo(PatientInfoItem& info);

private:
    void CreateReport(QDomDocument *pXmlDoc);
    void CreateHospitalInfos(QDomDocument *pXmlDoc, QDomElement *pRootElement);
    void CreatePatientInfos(QDomDocument *pXmlDoc, QDomElement *pRootElement);
    void CreateUltrasoundImages(QDomDocument *pXmlDoc, QDomElement *pRootElement);
    void CreateExaminationFindings(QDomDocument *pXmlDoc, QDomElement *pRootElement);
    void CreateExaminationConclusion(QDomDocument *pXmlDoc, QDomElement *pRootElement);
    void CreatePrintTime(QDomDocument *pXmlDoc, QDomElement *pRootElement);
    void CreateDoctorSignature(QDomDocument *pXmlDoc, QDomElement *pRootElement);

private:
    QList<PatientInfoItem> mListPatientInfo;
    QStringList            mListUsImagePath;
    QString                mStrDoctorSignature;

};

#endif // REPORT_XML_DATA_H
