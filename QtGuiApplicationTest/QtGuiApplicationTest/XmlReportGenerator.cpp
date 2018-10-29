#include "XmlReportGenerator.h"

#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QTextCodec>

const QString XmlReportGenerator::TAG_ROOT                               = "MGIUS_Report";          // 跟标签
const QString XmlReportGenerator::TAG_HEADER                             = "Header";                // 报告头部
const QString XmlReportGenerator::TAG_HEADER_HospitalLogo                = "HospitalLogo";          // 医院标志图片路径
const QString XmlReportGenerator::TAG_HEADER_MainTitle                   = "MainTitle";             // 报告主标题
const QString XmlReportGenerator::TAG_HEADER_SubTitle                    = "SubTitle";              // 报告副标题
const QString XmlReportGenerator::TAG_BODY                               = "Body";                  // 报告正文
const QString XmlReportGenerator::TAG_BODY_PatientInfo                   = "PatientInfo";           // 病人信息
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Title             = "Title";                 // 病人信息-板块标题
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Name              = "Name";                  // 病人信息-姓名
const QString XmlReportGenerator::TAG_BODY_PatientInfo_PatientNumber     = "PatientNumber";         // 病人信息-门诊号
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Method            = "Method";                // 病人信息-检查方法
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Date              = "Date";                  // 病人信息-检查日期
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Sex               = "Sex";                   // 病人信息-性别
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Nation            = "Nation";                // 病人信息-民族
const QString XmlReportGenerator::TAG_BODY_PatientInfo_IDCard            = "IDCard";                // 病人信息-身份证号
const QString XmlReportGenerator::TAG_BODY_PatientInfo_PhoneNumber       = "PhoneNumber";           // 病人信息-联系电话
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Stature           = "Stature";               // 病人信息-身高
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Weight            = "Weight";                // 病人信息-体重
const QString XmlReportGenerator::TAG_BODY_PatientInfo_InpatientArea     = "InpatientArea";         // 病人信息-病区
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Mainpulator       = "Mainpulator";           // 病人信息-操作者
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Age               = "Age";                   // 病人信息-年龄
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Physician         = "Physician";             // 病人信息-记录医师
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Consultant        = "Consultant";            // 病人信息-会诊医生
const QString XmlReportGenerator::TAG_BODY_PatientInfo_ExamDescription   = "ExamDescription";       // 病人信息-检查描述
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Comment           = "Comment";               // 病人信息-备注
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Keyword           = "Keyword";               // 病人信息-关键字
const QString XmlReportGenerator::TAG_BODY_PatientInfo_Value             = "Value";                 // 病人信息-值
const QString XmlReportGenerator::TAG_BODY_UltrasoundImages              = "UltrasoundImages";      // 超声图像
const QString XmlReportGenerator::TAG_BODY_UltrasoundImages_Title        = "Title";                 // 超声图像-板块标题
const QString XmlReportGenerator::TAG_BODY_UltrasoundImages_CountPerLine = "CountPerLine";          // 超声图像-每行图像数
const QString XmlReportGenerator::TAG_BODY_UltrasoundImages_USImage      = "USImage";               // 超声图像-图像路径
const QString XmlReportGenerator::TAG_BODY_ExaminationFindings           = "ExaminationFindings";   // 检查所见
const QString XmlReportGenerator::TAG_BODY_ExaminationFindings_Title     = "Title";                 // 检查所见-板块标题
const QString XmlReportGenerator::TAG_BODY_ExaminationFindings_Content   = "Content";               // 检查所见-内容
const QString XmlReportGenerator::TAG_BODY_ExaminationConclusion         = "ExaminationConclusion"; // 诊断结果-板块标题
const QString XmlReportGenerator::TAG_BODY_ExaminationConclusion_Title   = "Title";                 // 诊断结果-板块标题
const QString XmlReportGenerator::TAG_BODY_ExaminationConclusion_Content = "Content";               // 诊断结果-内容
const QString XmlReportGenerator::TAG_FOOTER                             = "Footer";                // 报告尾部
const QString XmlReportGenerator::TAG_FOOTER_PrintTime                   = "PrintTime";             // 报告打印时间
const QString XmlReportGenerator::TAG_FOOTER_PrintTime_Keyword           = "Keyword";               // 打印时间-关键字
const QString XmlReportGenerator::TAG_FOOTER_PrintTime_Value             = "Value";                 // 打印时间-值
const QString XmlReportGenerator::TAG_FOOTER_DoctorSignature             = "DoctorSignature";       // 医生签名文件路径
const QString XmlReportGenerator::TAG_FOOTER_DoctorSignature_Keyword     = "Keyword";               // 医生签名-关键字
const QString XmlReportGenerator::TAG_FOOTER_DoctorSignature_Value       = "Value";                 // 医生签名-值

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
    this->SetTagValue(TAG_HEADER_MainTitle, QStringLiteral("超声诊断测试报告"));
    this->SetTagValue(TAG_HEADER_SubTitle, QStringLiteral("甲状腺"));

    // Body-病人信息
    this->SetTagValue(TAG_BODY_PatientInfo_Title, QStringLiteral("病人信息："));
    this->SetTagKeyValue(TAG_BODY_PatientInfo_Name,
                         QPair<QString, QString>(QStringLiteral("姓名："), QStringLiteral("张三")));
    this->SetTagKeyValue(TAG_BODY_PatientInfo_PatientNumber,
                         QPair<QString, QString>(QStringLiteral("门诊号："), QStringLiteral("123456")));
    // Body-超声图像
    //this->SetTagValue(TAG_BODY_UltrasoundImages_Title, QStringLiteral("超声图像："));
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

    // 医院 logo
    QDomElement hospitalLogo = pXmlDoc->createElement(TAG_HEADER_HospitalLogo);
    hospitalLogo.setAttribute("src", this->GetTagValue(TAG_HEADER_HospitalLogo));
    header.appendChild(hospitalLogo);
    // 主标题
    QDomElement mainTtle = pXmlDoc->createElement(TAG_HEADER_MainTitle);
    mainTtle.appendChild(pXmlDoc->createTextNode(this->GetTagValue(TAG_HEADER_MainTitle)));
    header.appendChild(mainTtle);
    // 副标题
    QDomElement subTitle = pXmlDoc->createElement(TAG_HEADER_SubTitle);
    subTitle.appendChild(pXmlDoc->createTextNode(this->GetTagValue(TAG_HEADER_SubTitle)));
    header.appendChild(subTitle);
}

void XmlReportGenerator::CreateReportBody(QDomDocument *pXmlDoc, QDomElement *pRootElement)
{
    QDomElement body = pXmlDoc->createElement(TAG_BODY);
    pRootElement->appendChild(body);

    // 病人信息
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
