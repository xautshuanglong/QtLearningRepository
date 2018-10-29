#include "XmlReportGenerator.h"

#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QTextCodec>

const QString XmlReportGenerator::TAG_ROOT                               = "MGIUS_Report";          // ����ǩ
const QString XmlReportGenerator::TAG_HEADER                             = "Header";                // ����ͷ��
const QString XmlReportGenerator::TAG_HEADER_HospitalLogo                = "HospitalLogo";          // ҽԺ��־ͼƬ·��
const QString XmlReportGenerator::TAG_HEADER_MainTitle                   = "MainTitle";             // ����������
const QString XmlReportGenerator::TAG_HEADER_SubTitle                    = "SubTitle";              // ���渱����
const QString XmlReportGenerator::TAG_BODY                               = "Body";                  // ��������
const QString XmlReportGenerator::TAG_BODY_PatientInfo                   = "PatientInfo";           // ������Ϣ
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Title             = "Title";                 // ������Ϣ-������
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Name              = "Name";                  // ������Ϣ-����
const QString XmlReportGenerator::TAG_BODY_PatientInfo_PatientNumber     = "PatientNumber";         // ������Ϣ-�����
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Method            = "Method";                // ������Ϣ-��鷽��
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Date              = "Date";                  // ������Ϣ-�������
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Sex               = "Sex";                   // ������Ϣ-�Ա�
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Nation            = "Nation";                // ������Ϣ-����
const QString XmlReportGenerator::TAG_BODY_PatientInfo_IDCard            = "IDCard";                // ������Ϣ-���֤��
const QString XmlReportGenerator::TAG_BODY_PatientInfo_PhoneNumber       = "PhoneNumber";           // ������Ϣ-��ϵ�绰
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Stature           = "Stature";               // ������Ϣ-���
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Weight            = "Weight";                // ������Ϣ-����
const QString XmlReportGenerator::TAG_BODY_PatientInfo_InpatientArea     = "InpatientArea";         // ������Ϣ-����
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Mainpulator       = "Mainpulator";           // ������Ϣ-������
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Age               = "Age";                   // ������Ϣ-����
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Physician         = "Physician";             // ������Ϣ-��¼ҽʦ
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Consultant        = "Consultant";            // ������Ϣ-����ҽ��
const QString XmlReportGenerator::TAG_BODY_PatientInfo_ExamDescription   = "ExamDescription";       // ������Ϣ-�������
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Comment           = "Comment";               // ������Ϣ-��ע
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Keyword           = "Keyword";               // ������Ϣ-�ؼ���
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Value             = "Value";                 // ������Ϣ-ֵ
const QString XmlReportGenerator::TAG_BODY_UltrasoundImages              = "UltrasoundImages";      // ����ͼ��
const QString XmlReportGenerator::TAG_BODY_UltrasoundImages_Title        = "Title";                 // ����ͼ��-������
const QString XmlReportGenerator::TAG_BODY_UltrasoundImages_CountPerLine = "CountPerLine";          // ����ͼ��-ÿ��ͼ����
const QString XmlReportGenerator::TAG_BODY_UltrasoundImages_USImage      = "USImage";               // ����ͼ��-ͼ��·��
const QString XmlReportGenerator::TAG_BODY_ExaminationFindings           = "ExaminationFindings";   // �������
const QString XmlReportGenerator::TAG_BODY_ExaminationFindings_Title     = "Title";                 // �������-������
const QString XmlReportGenerator::TAG_BODY_ExaminationFindings_Content   = "Content";               // �������-����
const QString XmlReportGenerator::TAG_BODY_ExaminationConclusion         = "ExaminationConclusion"; // ��Ͻ��-������
const QString XmlReportGenerator::TAG_BODY_ExaminationConclusion_Title   = "Title";                 // ��Ͻ��-������
const QString XmlReportGenerator::TAG_BODY_ExaminationConclusion_Content = "Content";               // ��Ͻ��-����
const QString XmlReportGenerator::TAG_FOOTER                             = "Footer";                // ����β��
const QString XmlReportGenerator::TAG_FOOTER_PrintTime                   = "PrintTime";             // �����ӡʱ��
const QString XmlReportGenerator::TAG_FOOTER_PrintTime_Keyword           = "Keyword";               // ��ӡʱ��-�ؼ���
const QString XmlReportGenerator::TAG_FOOTER_PrintTime_Value             = "Value";                 // ��ӡʱ��-ֵ
const QString XmlReportGenerator::TAG_FOOTER_DoctorSignature             = "DoctorSignature";       // ҽ��ǩ���ļ�·��
const QString XmlReportGenerator::TAG_FOOTER_DoctorSignature_Keyword     = "Keyword";               // ҽ��ǩ��-�ؼ���
const QString XmlReportGenerator::TAG_FOOTER_DoctorSignature_Value       = "Value";                 // ҽ��ǩ��-ֵ

XmlReportGenerator::XmlReportGenerator()
{
    mpMapTagValue = new QMap<QString, QString>();
    mpMapTagKeyValue = new QMap<QString, QPair<QString, QString>>();

    mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Name);
    mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_PatientNumber);
    mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Method);
    mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Date);
    mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Sex);
    mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Nation);
    mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_IDCard);
    mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_PhoneNumber);
    mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Stature);
    mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Weight);
    mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_InpatientArea);
    mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Mainpulator);
    mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Age);
    mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Physician);
    mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Consultant);
    mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_ExamDescription);
    mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Comment);
    this->InitDefaultValues();
}

XmlReportGenerator::~XmlReportGenerator()
{
    delete mpMapTagValue;
    delete mpMapTagKeyValue;
}

void XmlReportGenerator::InitDefaultValues()
{
    // Header
    this->SetTagValue(TAG_HEADER_HospitalLogo, "image_logo.png");
    this->SetTagValue(TAG_HEADER_MainTitle, QStringLiteral("������ϲ��Ա���"));
    this->SetTagValue(TAG_HEADER_SubTitle, QStringLiteral("��״��"));

    // Body-������Ϣ
    this->SetTagValue(TAG_BODY_PatientInfo_Title, QStringLiteral("������Ϣ��"));
    this->SetTagKeyValue(TAG_BODY_PatientInfo_Name,
                         QPair<QString, QString>(QStringLiteral("������"), QStringLiteral("����")));
    this->SetTagKeyValue(TAG_BODY_PatientInfo_PatientNumber,
                         QPair<QString, QString>(QStringLiteral("����ţ�"), QStringLiteral("123456")));
    // Body-����ͼ��
    //this->SetTagValue(TAG_BODY_UltrasoundImages_Title, QStringLiteral("����ͼ��"));
}

void XmlReportGenerator::CreateReport(QDomDocument *pXmlDoc)
{
    QDomProcessingInstruction instruction = pXmlDoc->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    pXmlDoc->appendChild(instruction);
}

void XmlReportGenerator::SaveReportAsXml(const QString& xmlFilename)
{
    QDomDocument xmlDoc;
    this->CreateReport(&xmlDoc);

    QDomElement rootElement = xmlDoc.createElement(TAG_ROOT);
    xmlDoc.appendChild(rootElement);

    this->CreateReportHeader(&xmlDoc, &rootElement);
    this->CreateReportBody(&xmlDoc, &rootElement);
    this->CreateReportFooter(&xmlDoc, &rootElement);

    QFile xmlFile(xmlFilename);
    if (xmlFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        QTextStream outXmlStream(&xmlFile);
        xmlDoc.save(outXmlStream, 4, QDomNode::EncodingFromDocument);
        outXmlStream.flush();
        xmlFile.close();
    }
}

void XmlReportGenerator::CreateReportHeader(QDomDocument *pXmlDoc, QDomElement *pRootElement)
{
    QDomElement header = pXmlDoc->createElement(TAG_HEADER);
    pRootElement->appendChild(header);

    // ҽԺ logo
    QDomElement hospitalLogo = pXmlDoc->createElement(TAG_HEADER_HospitalLogo);
    hospitalLogo.setAttribute("src", this->GetTagValue(TAG_HEADER_HospitalLogo));
    header.appendChild(hospitalLogo);
    // ������
    QDomElement mainTtle = pXmlDoc->createElement(TAG_HEADER_MainTitle);
    mainTtle.appendChild(pXmlDoc->createTextNode(this->GetTagValue(TAG_HEADER_MainTitle)));
    header.appendChild(mainTtle);
    // ������
    QDomElement subTitle = pXmlDoc->createElement(TAG_HEADER_SubTitle);
    subTitle.appendChild(pXmlDoc->createTextNode(this->GetTagValue(TAG_HEADER_SubTitle)));
    header.appendChild(subTitle);
}

void XmlReportGenerator::CreateReportBody(QDomDocument *pXmlDoc, QDomElement *pRootElement)
{
    QDomElement body = pXmlDoc->createElement(TAG_BODY);
    pRootElement->appendChild(body);

    // ������Ϣ
    QDomElement patientInfo = pXmlDoc->createElement(TAG_BODY_PatientInfo);
    body.appendChild(patientInfo);
    QDomElement patientInfoTitle = pXmlDoc->createElement(TAG_BODY_PatientInfo_Title);
    patientInfoTitle.appendChild(pXmlDoc->createTextNode(this->GetTagValue(TAG_BODY_PatientInfo_Title)));
    patientInfo.appendChild(patientInfoTitle);

    QString tempTag = "";
    QDomElement tempElement;
    QDomElement tempKeyword;
    QDomElement tempValue;
    QPair<QString, QString> keyValuePair;
    int i = 0;
    for (i = 0; i < mListPatientInfoKeyValue.count(); ++i)
    {
        tempTag = mListPatientInfoKeyValue.at(i);
        keyValuePair = this->GetTagKeyValue(tempTag);

        tempKeyword = pXmlDoc->createElement(TAG_BODY_PatientInfo_Keyword);
        tempKeyword.appendChild(pXmlDoc->createTextNode(keyValuePair.first));
        tempValue = pXmlDoc->createElement(TAG_BODY_PatientInfo_Value);
        tempValue.appendChild(pXmlDoc->createTextNode(keyValuePair.second));
        tempElement = pXmlDoc->createElement(tempTag);
        tempElement.appendChild(tempKeyword);
        tempElement.appendChild(tempValue);
        patientInfo.appendChild(tempElement);
    }

    QDomElement usImagesTitle = pXmlDoc->createElement(TAG_BODY_UltrasoundImages_Title);
    usImagesTitle.appendChild(pXmlDoc->createTextNode(this->GetTagValue(TAG_BODY_UltrasoundImages_Title)));
    QDomElement countPerLine = pXmlDoc->createElement(TAG_BODY_UltrasoundImages_CountPerLine);
    countPerLine.appendChild(pXmlDoc->createTextNode(this->GetTagValue(TAG_BODY_UltrasoundImages_CountPerLine)));
    QDomElement usImages = pXmlDoc->createElement(TAG_BODY_UltrasoundImages);
    usImages.appendChild(usImagesTitle);
    usImages.appendChild(countPerLine);
    body.appendChild(usImages);

    QDomElement examFindings = pXmlDoc->createElement(TAG_BODY_ExaminationFindings);
    body.appendChild(examFindings);

    QDomElement examConclusion = pXmlDoc->createElement(TAG_BODY_ExaminationConclusion);
    body.appendChild(examConclusion);
}

void XmlReportGenerator::CreateReportFooter(QDomDocument *pXmlDoc, QDomElement *pRootElement)
{
    QDomElement footer = pXmlDoc->createElement(TAG_FOOTER);
    pRootElement->appendChild(footer);

    QPair<QString, QString> printTimePair = this->GetTagKeyValue(TAG_FOOTER_PrintTime);
    QDomElement printTimeKey = pXmlDoc->createElement(TAG_FOOTER_PrintTime_Keyword);
    printTimeKey.appendChild(pXmlDoc->createTextNode(printTimePair.first));
    QDomElement printTimeValue = pXmlDoc->createElement(TAG_FOOTER_PrintTime_Value);
    printTimeValue.appendChild(pXmlDoc->createTextNode(printTimePair.second));
    QDomElement printTime = pXmlDoc->createElement(TAG_FOOTER_PrintTime);
    printTime.appendChild(printTimeKey);
    printTime.appendChild(printTimeValue);
    footer.appendChild(printTime);

    QPair<QString, QString> signaturePair = this->GetTagKeyValue(TAG_FOOTER_DoctorSignature);
    QDomElement signatureKey = pXmlDoc->createElement(TAG_FOOTER_PrintTime_Keyword);
    signatureKey.appendChild(pXmlDoc->createTextNode(printTimePair.first));
    QDomElement signatureValue = pXmlDoc->createElement(TAG_FOOTER_PrintTime_Value);
    signatureValue.appendChild(pXmlDoc->createTextNode(printTimePair.second));
    QDomElement doctorSignature = pXmlDoc->createElement(TAG_FOOTER_DoctorSignature);
    doctorSignature.appendChild(signatureKey);
    doctorSignature.appendChild(signatureValue);
    footer.appendChild(doctorSignature);
}

//// Hader
//void XmlReportGenerator::SetLogoImageSrc(const QString& logoSrc)
//{
//}
//
//// Body-PatientInfo
//void XmlReportGenerator::AddPatientInfo(const QString& tagName, const QString& keyword, const QString& value)
//{}
//
//// Body-UltrasoundImages
//void XmlReportGenerator::SetUltralsoundTitle(const QString& usTitle)
//{}
//
//void XmlReportGenerator::SetUsImageCountPerLine(int countPerLine)
//{}
//
//void XmlReportGenerator::AddUltrasoundImages(const QString& usImageSrc)
//{}
//
//void XmlReportGenerator::ClearUltrasoundImages()
//{}
//
//// Body-ExaminationFindings
//void XmlReportGenerator::SetExaminationFindings(const QString& examFindings)
//{}
//
//// Body-ExaminationConclusion
//void XmlReportGenerator::SetExaminationConclusion(const QString& examConclusion)
//{}
//
//// Footer
//void XmlReportGenerator::SetSignatureSrc(QString signatureSrc)
//{}

void XmlReportGenerator::SetTagValue(const QString& tag, const QString& value)
{
    mpMapTagValue->insert(tag, value);
}

QString XmlReportGenerator::GetTagValue(const QString& tag)
{
    return (*mpMapTagValue)[tag];
}

void XmlReportGenerator::SetTagKeyValue(const QString& tag, const QPair<QString, QString>& value)
{
    mpMapTagKeyValue->insert(tag, value);
}

QPair<QString, QString> XmlReportGenerator::GetTagKeyValue(const QString& tag)
{
    return (*mpMapTagKeyValue)[tag];
}
