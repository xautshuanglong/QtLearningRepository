#include "XmlReportGenerator.h"

#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QTextCodec>

//const QString XmlReportGenerator::TAG_ROOT                               = "MGIUS_Report";          // ����ǩ
//const QString XmlReportGenerator::TAG_HEADER                             = "Header";                // ����ͷ��
//const QString XmlReportGenerator::TAG_HEADER_HospitalLogo                = "HospitalLogo";          // ҽԺ��־ͼƬ·��
//const QString XmlReportGenerator::TAG_HEADER_MainTitle                   = "MainTitle";             // ����������
//const QString XmlReportGenerator::TAG_HEADER_SubTitle                    = "SubTitle";              // ���渱����
//const QString XmlReportGenerator::TAG_BODY                               = "Body";                  // ��������
//const QString XmlReportGenerator::TAG_BODY_PatientInfo                   = "PatientInfo";           // ������Ϣ
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Title             = "Title";                 // ������Ϣ-������
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Name              = "Name";                  // ������Ϣ-����
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_PatientNumber     = "PatientNumber";         // ������Ϣ-�����
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Method            = "Method";                // ������Ϣ-��鷽��
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Date              = "Date";                  // ������Ϣ-�������
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Sex               = "Sex";                   // ������Ϣ-�Ա�
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Nation            = "Nation";                // ������Ϣ-����
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_IDCard            = "IDCard";                // ������Ϣ-���֤��
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_PhoneNumber       = "PhoneNumber";           // ������Ϣ-��ϵ�绰
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Stature           = "Stature";               // ������Ϣ-���
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Weight            = "Weight";                // ������Ϣ-����
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_InpatientArea     = "InpatientArea";         // ������Ϣ-����
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Mainpulator       = "Mainpulator";           // ������Ϣ-������
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Age               = "Age";                   // ������Ϣ-����
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Physician         = "Physician";             // ������Ϣ-��¼ҽʦ
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Consultant        = "Consultant";            // ������Ϣ-����ҽ��
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_ExamDescription   = "ExamDescription";       // ������Ϣ-�������
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Comment           = "Comment";               // ������Ϣ-��ע
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Keyword           = "Keyword";               // ������Ϣ-�ؼ���
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Value             = "Value";                 // ������Ϣ-ֵ
//const QString XmlReportGenerator::TAG_BODY_UltrasoundImages              = "UltrasoundImages";      // ����ͼ��
//const QString XmlReportGenerator::TAG_BODY_UltrasoundImages_Title        = "Title";                 // ����ͼ��-������
//const QString XmlReportGenerator::TAG_BODY_UltrasoundImages_CountPerLine = "CountPerLine";          // ����ͼ��-ÿ��ͼ����
//const QString XmlReportGenerator::TAG_BODY_UltrasoundImages_USImage      = "USImage";               // ����ͼ��-ͼ��·��
//const QString XmlReportGenerator::TAG_BODY_ExaminationFindings           = "ExaminationFindings";   // �������
//const QString XmlReportGenerator::TAG_BODY_ExaminationFindings_Title     = "Title";                 // �������-������
//const QString XmlReportGenerator::TAG_BODY_ExaminationFindings_Content   = "Content";               // �������-����
//const QString XmlReportGenerator::TAG_BODY_ExaminationConclusion         = "ExaminationConclusion"; // ��Ͻ��-������
//const QString XmlReportGenerator::TAG_BODY_ExaminationConclusion_Title   = "Title";                 // ��Ͻ��-������
//const QString XmlReportGenerator::TAG_BODY_ExaminationConclusion_Content = "Content";               // ��Ͻ��-����
//const QString XmlReportGenerator::TAG_FOOTER                             = "Footer";                // ����β��
//const QString XmlReportGenerator::TAG_FOOTER_PrintTime                   = "PrintTime";             // �����ӡʱ��
//const QString XmlReportGenerator::TAG_FOOTER_PrintTime_Keyword           = "Keyword";               // ��ӡʱ��-�ؼ���
//const QString XmlReportGenerator::TAG_FOOTER_PrintTime_Value             = "Value";                 // ��ӡʱ��-ֵ
//const QString XmlReportGenerator::TAG_FOOTER_DoctorSignature             = "DoctorSignature";       // ҽ��ǩ���ļ�·��
//const QString XmlReportGenerator::TAG_FOOTER_DoctorSignature_Keyword     = "Keyword";               // ҽ��ǩ��-�ؼ���
//const QString XmlReportGenerator::TAG_FOOTER_DoctorSignature_Value       = "Value";                 // ҽ��ǩ��-ֵ

// ���޷���֤����������Ӧ��ϵ����ͨ�� tagIndex �ֶμ����Աȡ�
const XmlReportGenerator::TagKeyValue XmlReportGenerator::gMapTagKeyValue[] =
{
    { TAG_ROOT,                               "MGIUS_Report",          QStringLiteral(""), QStringLiteral("") },
    { TAG_HEADER,                             "Header",                QStringLiteral(""), QStringLiteral("") },
    { TAG_HEADER_HospitalLogo,                "HospitalLogo",          QStringLiteral(""), QStringLiteral("image_logo.png") },
    { TAG_HEADER_MainTitle,                   "MainTitle",             QStringLiteral(""), QStringLiteral("������ϲ��Ա���") },
    { TAG_HEADER_SubTitle,                    "SubTitle",              QStringLiteral(""), QStringLiteral("��״��") },
    { TAG_BODY,                               "Body",                  QStringLiteral(""), QStringLiteral("") },
    { TAG_BODY_PatientInfo,                   "PatientInfo",           QStringLiteral(""), QStringLiteral("") },
    { TAG_BODY_PatientInfo_Title,             "Title",                 QStringLiteral(""), QStringLiteral("������Ϣ��") },
    { TAG_BODY_PatientInfo_Name,              "Name",                  QStringLiteral("������"), QStringLiteral("����") },
    { TAG_BODY_PatientInfo_PatientNumber,     "PatientNumber",         QStringLiteral("����ţ�"), "123456" },
    { TAG_BODY_PatientInfo_Method,            "Method",                QStringLiteral("��鷽����"), QStringLiteral("Զ�̳���") },
    { TAG_BODY_PatientInfo_Date,              "Date",                  QStringLiteral("������ڣ�"), QStringLiteral("2018/10/12") },
    { TAG_BODY_PatientInfo_Sex,               "Sex",                   QStringLiteral("�Ա�"),     QStringLiteral("��") },
    { TAG_BODY_PatientInfo_Nation,            "Nation",                QStringLiteral("���壺"),     QStringLiteral("��") },
    { TAG_BODY_PatientInfo_IDCard,            "IDCard",                QStringLiteral("���֤�ţ�"), QStringLiteral("351227189244521546") },
    { TAG_BODY_PatientInfo_PhoneNumber,       "PhoneNumber",           QStringLiteral("��ϵ�绰��"), QStringLiteral("13892331539") },
    { TAG_BODY_PatientInfo_Stature,           "Stature",               QStringLiteral("��ߣ�"),     QStringLiteral("175cm") },
    { TAG_BODY_PatientInfo_Weight,            "Weight",                QStringLiteral("���أ�"),     QStringLiteral("70kg") },
    { TAG_BODY_PatientInfo_InpatientArea,     "InpatientArea",         QStringLiteral("������"),     QStringLiteral("A30") },
    { TAG_BODY_PatientInfo_Mainpulator,       "Manipulator",           QStringLiteral("�����ߣ�"),   QStringLiteral("�ŷ�") },
    { TAG_BODY_PatientInfo_Age,               "Age",                   QStringLiteral("���䣺"),     QStringLiteral("28") },
    { TAG_BODY_PatientInfo_Physician,         "Physician",             QStringLiteral("��¼ҽʦ��"), QStringLiteral("����") },
    { TAG_BODY_PatientInfo_Consultant,        "Consultant",            QStringLiteral("����ҽ����"), QStringLiteral("����") },
    { TAG_BODY_PatientInfo_ExamDescription,   "ExamDescription",       QStringLiteral("���������"), QStringLiteral("�����������") },
    { TAG_BODY_PatientInfo_Comment,           "Comment",               QStringLiteral("��ע��"), QStringLiteral("��ע�ı�����") },
    { TAG_BODY_PatientInfo_Keyword,           "Keyword",               QStringLiteral(""), QStringLiteral("") },
    { TAG_BODY_PatientInfo_Value,             "Value",                 QStringLiteral(""), QStringLiteral("") },
    { TAG_BODY_UltrasoundImages,              "UltrasoundImages",      QStringLiteral(""), QStringLiteral("") },
    { TAG_BODY_UltrasoundImages_Title,        "Title",                 QStringLiteral(""), QStringLiteral("����ͼƬ��") },
    { TAG_BODY_UltrasoundImages_CountPerLine, "CountPerLine",          QStringLiteral(""), QStringLiteral("2") },
    { TAG_BODY_UltrasoundImages_USImage,      "USImage",               QStringLiteral(""), QStringLiteral("image_us_1.png,image_us_2.png,image_us_3.png,image_us_4.png,") },
    { TAG_BODY_ExaminationFindings,           "ExaminationFindings",   QStringLiteral(""), QStringLiteral("") },
    { TAG_BODY_ExaminationFindings_Title,     "Title",                 QStringLiteral(""), QStringLiteral("���������") },
    { TAG_BODY_ExaminationFindings_Content,   "Content",               QStringLiteral(""), QStringLiteral("�����������") },
    { TAG_BODY_ExaminationConclusion,         "ExaminationConclusion", QStringLiteral(""), QStringLiteral("") },
    { TAG_BODY_ExaminationConclusion_Title,   "Title",                 QStringLiteral(""), QStringLiteral("��Ͻ����") },
    { TAG_BODY_ExaminationConclusion_Content, "Content",               QStringLiteral(""), QStringLiteral("��Ͻ������") },
    { TAG_FOOTER,                             "Footer",                QStringLiteral(""), QStringLiteral("") },
    { TAG_FOOTER_PrintTime,                   "PrintTime",             QStringLiteral("��ӡʱ�䣺"), QStringLiteral("2018-10-15 20:55") },
    { TAG_FOOTER_Keyword,                     "Keyword",               QStringLiteral(""), QStringLiteral("") },
    { TAG_FOOTER_Value,                       "Value",                 QStringLiteral(""), QStringLiteral("") },
    { TAG_FOOTER_DoctorSignature,             "DoctorSignature",       QStringLiteral("���ҽ����"), QStringLiteral("image_doctor_signature.png") }
};

XmlReportGenerator::XmlReportGenerator()
{
    this->ResetTagKeyValue();
}

XmlReportGenerator::~XmlReportGenerator()
{
}

void XmlReportGenerator::SetTagString(const ReportXmlTag tag, const QString& tagString)
{
    
}

QString XmlReportGenerator::GetTagString(const ReportXmlTag tag)
{
    return mMapTagKeyValue[tag].tagString;
}

void XmlReportGenerator::SetTagKeyword(const ReportXmlTag tag, const QString& tagKeyword)
{
    mMapTagKeyValue[tag].tagKeyword = tagKeyword;
}

QString XmlReportGenerator::GetTagKeyword(const ReportXmlTag tag)
{
    return mMapTagKeyValue[tag].tagKeyword;
}

void XmlReportGenerator::SetTagValue(const ReportXmlTag tag, const QString& tagValue)
{
    mMapTagKeyValue[tag].tagValue = tagValue;
}

QString XmlReportGenerator::GetTagValue(const ReportXmlTag tag)
{
    return mMapTagKeyValue[tag].tagValue;
}

void XmlReportGenerator::SetTag(const ReportXmlTag tag, const TagKeyValue& tagKeyValue)
{
    mMapTagKeyValue[tag].tagString = tagKeyValue.tagString;
    mMapTagKeyValue[tag].tagKeyword = tagKeyValue.tagKeyword;
    mMapTagKeyValue[tag].tagValue = tagKeyValue.tagValue;
}

void XmlReportGenerator::GetTag(const ReportXmlTag tag, TagKeyValue& outTagKeyValue)
{
    outTagKeyValue.tagString = mMapTagKeyValue[tag].tagString;
    outTagKeyValue.tagKeyword = mMapTagKeyValue[tag].tagKeyword;
    outTagKeyValue.tagValue = mMapTagKeyValue[tag].tagValue;
}

void XmlReportGenerator::ResetTagKeyValue()
{
    for (int i = 0; i < TAG_MAX; ++i)
    {
        mMapTagKeyValue[i].tagIndex = (ReportXmlTag)i;
        mMapTagKeyValue[i].tagString  = XmlReportGenerator::gMapTagKeyValue[i].tagString;
        mMapTagKeyValue[i].tagKeyword = XmlReportGenerator::gMapTagKeyValue[i].tagKeyword;
        mMapTagKeyValue[i].tagValue   = XmlReportGenerator::gMapTagKeyValue[i].tagValue;
    }
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

    QDomElement rootElement = xmlDoc.createElement(mMapTagKeyValue[TAG_ROOT].tagString);
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
    QDomElement header = pXmlDoc->createElement(mMapTagKeyValue[TAG_HEADER].tagString);
    pRootElement->appendChild(header);

    // ҽԺ logo
    QDomElement hospitalLogo = pXmlDoc->createElement(mMapTagKeyValue[TAG_HEADER_HospitalLogo].tagString);
    hospitalLogo.setAttribute("src", mMapTagKeyValue[TAG_HEADER_HospitalLogo].tagValue);
    header.appendChild(hospitalLogo);
    // ������
    QDomElement mainTtle = pXmlDoc->createElement(mMapTagKeyValue[TAG_HEADER_MainTitle].tagString);
    mainTtle.appendChild(pXmlDoc->createTextNode(mMapTagKeyValue[TAG_HEADER_MainTitle].tagValue));
    header.appendChild(mainTtle);
    // ������
    QDomElement subTitle = pXmlDoc->createElement(mMapTagKeyValue[TAG_HEADER_SubTitle].tagString);
    subTitle.appendChild(pXmlDoc->createTextNode(mMapTagKeyValue[TAG_HEADER_SubTitle].tagValue));
    header.appendChild(subTitle);
}

void XmlReportGenerator::CreateReportBody(QDomDocument *pXmlDoc, QDomElement *pRootElement)
{
    QDomElement body = pXmlDoc->createElement(mMapTagKeyValue[TAG_BODY].tagString);
    pRootElement->appendChild(body);

    // ������Ϣ
    QDomElement patientInfo = pXmlDoc->createElement(mMapTagKeyValue[TAG_BODY_PatientInfo].tagString);
    body.appendChild(patientInfo);
    QDomElement patientInfoTitle = pXmlDoc->createElement(mMapTagKeyValue[TAG_BODY_PatientInfo_Title].tagString);
    patientInfoTitle.appendChild(pXmlDoc->createTextNode(mMapTagKeyValue[TAG_BODY_PatientInfo_Title].tagValue));
    patientInfo.appendChild(patientInfoTitle);

    QDomElement tempElement;
    QDomElement tempKeyword;
    QDomElement tempValue;

    for (int i = TAG_BODY_PatientInfo_Name; i < TAG_BODY_PatientInfo_Comment; ++i)
    {
        tempKeyword = pXmlDoc->createElement(mMapTagKeyValue[TAG_BODY_PatientInfo_Keyword].tagString);
        tempKeyword.appendChild(pXmlDoc->createTextNode(mMapTagKeyValue[i].tagKeyword));
        tempValue = pXmlDoc->createElement(mMapTagKeyValue[TAG_BODY_PatientInfo_Value].tagString);
        tempValue.appendChild(pXmlDoc->createTextNode(mMapTagKeyValue[i].tagValue));
        tempElement = pXmlDoc->createElement(mMapTagKeyValue[i].tagString);
        tempElement.appendChild(tempKeyword);
        tempElement.appendChild(tempValue);
        patientInfo.appendChild(tempElement);
    }

    QDomElement usImagesTitle = pXmlDoc->createElement(mMapTagKeyValue[TAG_BODY_UltrasoundImages_Title].tagString);
    usImagesTitle.appendChild(pXmlDoc->createTextNode(mMapTagKeyValue[TAG_BODY_UltrasoundImages_Title].tagValue));
    QDomElement countPerLine = pXmlDoc->createElement(mMapTagKeyValue[TAG_BODY_UltrasoundImages_CountPerLine].tagString);
    countPerLine.appendChild(pXmlDoc->createTextNode(mMapTagKeyValue[TAG_BODY_UltrasoundImages_CountPerLine].tagValue));
    QDomElement usImages = pXmlDoc->createElement(mMapTagKeyValue[TAG_BODY_UltrasoundImages].tagString);
    usImages.appendChild(usImagesTitle);
    usImages.appendChild(countPerLine);
    body.appendChild(usImages);

    QStringList usImgSrcList = mMapTagKeyValue[TAG_BODY_UltrasoundImages_USImage].tagValue.split(",", QString::SkipEmptyParts);
    for (int i=0; i<usImgSrcList.count(); ++i)
    {
        QDomElement usImgList = pXmlDoc->createElement(mMapTagKeyValue[TAG_BODY_UltrasoundImages_USImage].tagString);
        usImgList.appendChild(pXmlDoc->createTextNode(usImgSrcList[i]));
        usImages.appendChild(usImgList);
    }

    QDomElement examFindingsTitle = pXmlDoc->createElement(mMapTagKeyValue[TAG_BODY_ExaminationFindings_Title].tagString);
    examFindingsTitle.appendChild(pXmlDoc->createTextNode(mMapTagKeyValue[TAG_BODY_ExaminationFindings_Title].tagValue));
    QDomElement examFindingsValue = pXmlDoc->createElement(mMapTagKeyValue[TAG_BODY_ExaminationFindings_Content].tagString);
    examFindingsValue.appendChild(pXmlDoc->createTextNode(mMapTagKeyValue[TAG_BODY_ExaminationFindings_Content].tagValue));
    QDomElement examFindings = pXmlDoc->createElement(mMapTagKeyValue[TAG_BODY_ExaminationFindings].tagString);
    examFindings.appendChild(examFindingsTitle);
    examFindings.appendChild(examFindingsValue);
    body.appendChild(examFindings);

    QDomElement examConclusionTitle = pXmlDoc->createElement(mMapTagKeyValue[TAG_BODY_ExaminationConclusion_Title].tagString);
    examConclusionTitle.appendChild(pXmlDoc->createTextNode(mMapTagKeyValue[TAG_BODY_ExaminationConclusion_Title].tagValue));
    QDomElement examConclusionValue = pXmlDoc->createElement(mMapTagKeyValue[TAG_BODY_ExaminationConclusion_Content].tagString);
    examConclusionValue.appendChild(pXmlDoc->createTextNode(mMapTagKeyValue[TAG_BODY_ExaminationConclusion_Content].tagValue));
    QDomElement examConclusion = pXmlDoc->createElement(mMapTagKeyValue[TAG_BODY_ExaminationConclusion].tagString);
    examConclusion.appendChild(examConclusionTitle);
    examConclusion.appendChild(examConclusionValue);
    body.appendChild(examConclusion);
}

void XmlReportGenerator::CreateReportFooter(QDomDocument *pXmlDoc, QDomElement *pRootElement)
{
    QDomElement footer = pXmlDoc->createElement(mMapTagKeyValue[TAG_FOOTER].tagString);
    pRootElement->appendChild(footer);

    QDomElement printTimeKey = pXmlDoc->createElement(mMapTagKeyValue[TAG_FOOTER_Keyword].tagString);
    printTimeKey.appendChild(pXmlDoc->createTextNode(mMapTagKeyValue[TAG_FOOTER_PrintTime].tagKeyword));
    QDomElement printTimeValue = pXmlDoc->createElement(mMapTagKeyValue[TAG_FOOTER_Value].tagString);
    printTimeValue.appendChild(pXmlDoc->createTextNode(mMapTagKeyValue[TAG_FOOTER_PrintTime].tagValue));
    QDomElement printTime = pXmlDoc->createElement(mMapTagKeyValue[TAG_FOOTER_PrintTime].tagString);
    printTime.appendChild(printTimeKey);
    printTime.appendChild(printTimeValue);
    footer.appendChild(printTime);

    QDomElement signatureKey = pXmlDoc->createElement(mMapTagKeyValue[TAG_FOOTER_Keyword].tagString);
    signatureKey.appendChild(pXmlDoc->createTextNode(mMapTagKeyValue[TAG_FOOTER_DoctorSignature].tagKeyword));
    QDomElement signatureValue = pXmlDoc->createElement(mMapTagKeyValue[TAG_FOOTER_Value].tagString);
    signatureValue.appendChild(pXmlDoc->createTextNode(mMapTagKeyValue[TAG_FOOTER_DoctorSignature].tagValue));
    QDomElement doctorSignature = pXmlDoc->createElement(mMapTagKeyValue[TAG_FOOTER_DoctorSignature].tagString);
    doctorSignature.appendChild(signatureKey);
    doctorSignature.appendChild(signatureValue);
    footer.appendChild(doctorSignature);
}
