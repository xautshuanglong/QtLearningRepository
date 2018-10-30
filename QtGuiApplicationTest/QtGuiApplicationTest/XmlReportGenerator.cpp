#include "XmlReportGenerator.h"

#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QTextCodec>

//const QString XmlReportGenerator::TAG_ROOT                               = "MGIUS_Report";          // 跟标签
//const QString XmlReportGenerator::TAG_HEADER                             = "Header";                // 报告头部
//const QString XmlReportGenerator::TAG_HEADER_HospitalLogo                = "HospitalLogo";          // 医院标志图片路径
//const QString XmlReportGenerator::TAG_HEADER_MainTitle                   = "MainTitle";             // 报告主标题
//const QString XmlReportGenerator::TAG_HEADER_SubTitle                    = "SubTitle";              // 报告副标题
//const QString XmlReportGenerator::TAG_BODY                               = "Body";                  // 报告正文
//const QString XmlReportGenerator::TAG_BODY_PatientInfo                   = "PatientInfo";           // 病人信息
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Title             = "Title";                 // 病人信息-板块标题
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Name              = "Name";                  // 病人信息-姓名
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_PatientNumber     = "PatientNumber";         // 病人信息-门诊号
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Method            = "Method";                // 病人信息-检查方法
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Date              = "Date";                  // 病人信息-检查日期
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Sex               = "Sex";                   // 病人信息-性别
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Nation            = "Nation";                // 病人信息-民族
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_IDCard            = "IDCard";                // 病人信息-身份证号
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_PhoneNumber       = "PhoneNumber";           // 病人信息-联系电话
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Stature           = "Stature";               // 病人信息-身高
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Weight            = "Weight";                // 病人信息-体重
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_InpatientArea     = "InpatientArea";         // 病人信息-病区
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Mainpulator       = "Mainpulator";           // 病人信息-操作者
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Age               = "Age";                   // 病人信息-年龄
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Physician         = "Physician";             // 病人信息-记录医师
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Consultant        = "Consultant";            // 病人信息-会诊医生
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_ExamDescription   = "ExamDescription";       // 病人信息-检查描述
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Comment           = "Comment";               // 病人信息-备注
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Keyword           = "Keyword";               // 病人信息-关键字
//const QString XmlReportGenerator::TAG_BODY_PatientInfo_Value             = "Value";                 // 病人信息-值
//const QString XmlReportGenerator::TAG_BODY_UltrasoundImages              = "UltrasoundImages";      // 超声图像
//const QString XmlReportGenerator::TAG_BODY_UltrasoundImages_Title        = "Title";                 // 超声图像-板块标题
//const QString XmlReportGenerator::TAG_BODY_UltrasoundImages_CountPerLine = "CountPerLine";          // 超声图像-每行图像数
//const QString XmlReportGenerator::TAG_BODY_UltrasoundImages_USImage      = "USImage";               // 超声图像-图像路径
//const QString XmlReportGenerator::TAG_BODY_ExaminationFindings           = "ExaminationFindings";   // 检查所见
//const QString XmlReportGenerator::TAG_BODY_ExaminationFindings_Title     = "Title";                 // 检查所见-板块标题
//const QString XmlReportGenerator::TAG_BODY_ExaminationFindings_Content   = "Content";               // 检查所见-内容
//const QString XmlReportGenerator::TAG_BODY_ExaminationConclusion         = "ExaminationConclusion"; // 诊断结果-板块标题
//const QString XmlReportGenerator::TAG_BODY_ExaminationConclusion_Title   = "Title";                 // 诊断结果-板块标题
//const QString XmlReportGenerator::TAG_BODY_ExaminationConclusion_Content = "Content";               // 诊断结果-内容
//const QString XmlReportGenerator::TAG_FOOTER                             = "Footer";                // 报告尾部
//const QString XmlReportGenerator::TAG_FOOTER_PrintTime                   = "PrintTime";             // 报告打印时间
//const QString XmlReportGenerator::TAG_FOOTER_PrintTime_Keyword           = "Keyword";               // 打印时间-关键字
//const QString XmlReportGenerator::TAG_FOOTER_PrintTime_Value             = "Value";                 // 打印时间-值
//const QString XmlReportGenerator::TAG_FOOTER_DoctorSignature             = "DoctorSignature";       // 医生签名文件路径
//const QString XmlReportGenerator::TAG_FOOTER_DoctorSignature_Keyword     = "Keyword";               // 医生签名-关键字
//const QString XmlReportGenerator::TAG_FOOTER_DoctorSignature_Value       = "Value";                 // 医生签名-值

const XmlReportGenerator::TagKeyValue XmlReportGenerator::gMapTagKeyValue[] =
{
    { TAG_ROOT,                               "MGIUS_Report",          QStringLiteral(""), QStringLiteral("") },
    { TAG_HEADER,                             "Header",                QStringLiteral(""), QStringLiteral("") },
    { TAG_HEADER_HospitalLogo,                "HospitalLogo",          QStringLiteral(""), QStringLiteral("image_logo.png") },
    { TAG_HEADER_MainTitle,                   "MainTitle",             QStringLiteral(""), QStringLiteral("超声诊断测试报告") },
    { TAG_HEADER_SubTitle,                    "SubTitle",              QStringLiteral(""), QStringLiteral("甲状腺") },
    { TAG_BODY,                               "Body",                  QStringLiteral(""), QStringLiteral("") },
    { TAG_BODY_PatientInfo,                   "PatientInfo",           QStringLiteral(""), QStringLiteral("") },
    { TAG_BODY_PatientInfo_Title,             "Title",                 QStringLiteral(""), QStringLiteral("病人信息：") },
    { TAG_BODY_PatientInfo_Name,              "Name",                  QStringLiteral("姓名："), QStringLiteral("张三") },
    { TAG_BODY_PatientInfo_PatientNumber,     "PatientNumber",         QStringLiteral("门诊号："), "123456" },
    { TAG_BODY_PatientInfo_Method,            "Method",                QStringLiteral("检查方法："), QStringLiteral("远程超声") },
    { TAG_BODY_PatientInfo_Date,              "Date",                  QStringLiteral("检查日期："), QStringLiteral("2018/10/12") },
    { TAG_BODY_PatientInfo_Sex,               "Sex",                   QStringLiteral("性别："), QStringLiteral("男") },
    { TAG_BODY_PatientInfo_Nation,            "Nation",                QStringLiteral("民族："), QStringLiteral("汉") },
    { TAG_BODY_PatientInfo_IDCard,            "IDCard",                QStringLiteral("身份证号："), QStringLiteral("351227189244521546") },
    { TAG_BODY_PatientInfo_PhoneNumber,       "PhoneNumber",           QStringLiteral("联系电话："), QStringLiteral("13892331539") },
    { TAG_BODY_PatientInfo_Stature,           "Stature",               QStringLiteral("身高："), QStringLiteral("175cm") },
    { TAG_BODY_PatientInfo_Weight,            "Weight",                QStringLiteral("体重："), QStringLiteral("70kg") },
    { TAG_BODY_PatientInfo_InpatientArea,     "InpatientArea",         QStringLiteral("病区："), QStringLiteral("A30") },
    { TAG_BODY_PatientInfo_Mainpulator,       "Manipulator",           QStringLiteral("操作者："), QStringLiteral("张飞") },
    { TAG_BODY_PatientInfo_Age,               "Age",                   QStringLiteral("年龄："), QStringLiteral("28") },
    { TAG_BODY_PatientInfo_Physician,         "Physician",             QStringLiteral("记录医师："), QStringLiteral("王二") },
    { TAG_BODY_PatientInfo_Consultant,        "Consultant",            QStringLiteral("会诊医生："), QStringLiteral("麻子") },
    { TAG_BODY_PatientInfo_ExamDescription,   "ExamDescription",       QStringLiteral("检查描述："), QStringLiteral("检查描述内容") },
    { TAG_BODY_PatientInfo_Comment,           "Comment",               QStringLiteral("备注："), QStringLiteral("备注文本内容") },
    { TAG_BODY_PatientInfo_Keyword,           "Keyword",               QStringLiteral(""), QStringLiteral("") },
    { TAG_BODY_PatientInfo_Value,             "Value",                 QStringLiteral(""), QStringLiteral("") },
    { TAG_BODY_UltrasoundImages,              "UltrasoundImages",      QStringLiteral(""), QStringLiteral("") },
    { TAG_BODY_UltrasoundImages_Title,        "Title",                 QStringLiteral(""), QStringLiteral("超声图片：") },
    { TAG_BODY_UltrasoundImages_CountPerLine, "CountPerLine",          QStringLiteral(""), QStringLiteral("2") },
    { TAG_BODY_UltrasoundImages_USImage,      "USImage",               QStringLiteral(""), QStringLiteral("image_us_1.png,image_us_2.png,image_us_3.png,image_us_4.png,") },
    { TAG_BODY_ExaminationFindings,           "ExaminationFindings",   QStringLiteral(""), QStringLiteral("") },
    { TAG_BODY_ExaminationFindings_Title,     "Title",                 QStringLiteral(""), QStringLiteral("检查所见：") },
    { TAG_BODY_ExaminationFindings_Content,   "Content",               QStringLiteral(""), QStringLiteral("检查所见内容") },
    { TAG_BODY_ExaminationConclusion,         "ExaminationConclusion", QStringLiteral(""), QStringLiteral("") },
    { TAG_BODY_ExaminationConclusion_Title,   "Title",                 QStringLiteral(""), QStringLiteral("诊断结果：") },
    { TAG_BODY_ExaminationConclusion_Content, "Content",               QStringLiteral(""), QStringLiteral("诊断结果内容") },
    { TAG_FOOTER,                             "Footer",                QStringLiteral(""), QStringLiteral("") },
    { TAG_FOOTER_PrintTime,                   "PrintTime",             QStringLiteral("打印时间："), QStringLiteral("2018-10-15 20:55") },
    { TAG_FOOTER_Keyword,                     "Keyword",               QStringLiteral(""), QStringLiteral("") },
    { TAG_FOOTER_Value,                       "Value",                 QStringLiteral(""), QStringLiteral("") },
    { TAG_FOOTER_DoctorSignature,             "DoctorSignature",       QStringLiteral("检查医生："), QStringLiteral("image_doctor_signature.png") }
};

XmlReportGenerator::XmlReportGenerator()
{
    mpMapTagValue = new QMap<QString, QString>();
    mpMapTagKeyValue = new QMap<QString, QPair<QString, QString>>();

    //mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Name);
    //mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_PatientNumber);
    //mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Method);
    //mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Date);
    //mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Sex);
    //mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Nation);
    //mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_IDCard);
    //mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_PhoneNumber);
    //mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Stature);
    //mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Weight);
    //mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_InpatientArea);
    //mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Mainpulator);
    //mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Age);
    //mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Physician);
    //mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Consultant);
    //mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_ExamDescription);
    //mListPatientInfoKeyValue.append(TAG_BODY_PatientInfo_Comment);
    this->InitDefaultValues();
}

XmlReportGenerator::~XmlReportGenerator()
{
    delete mpMapTagValue;
    delete mpMapTagKeyValue;
}

void XmlReportGenerator::SetDefaultValues()
{
    ;
}

void XmlReportGenerator::InitDefaultValues()
{
    //// Header
    //this->SetTagValue(TAG_HEADER_HospitalLogo, "image_logo.png");
    //this->SetTagValue(TAG_HEADER_MainTitle, QStringLiteral("超声诊断测试报告"));
    //this->SetTagValue(TAG_HEADER_SubTitle, QStringLiteral("甲状腺"));

    //// Body-病人信息
    //this->SetTagValue(TAG_BODY_PatientInfo_Title, QStringLiteral("病人信息："));
    //this->SetTagKeyValue(TAG_BODY_PatientInfo_Name,
    //                     QPair<QString, QString>(QStringLiteral("姓名："), QStringLiteral("张三")));
    //this->SetTagKeyValue(TAG_BODY_PatientInfo_PatientNumber,
    //                     QPair<QString, QString>(QStringLiteral("门诊号："), QStringLiteral("123456")));
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

    QDomElement rootElement = xmlDoc.createElement(gMapTagKeyValue[TAG_ROOT].tagString);
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
    QDomElement header = pXmlDoc->createElement(gMapTagKeyValue[TAG_HEADER].tagString);
    pRootElement->appendChild(header);

    // 医院 logo
    QDomElement hospitalLogo = pXmlDoc->createElement(gMapTagKeyValue[TAG_HEADER_HospitalLogo].tagString);
    hospitalLogo.setAttribute("src", gMapTagKeyValue[TAG_HEADER_HospitalLogo].tagValue);
    header.appendChild(hospitalLogo);
    // 主标题
    QDomElement mainTtle = pXmlDoc->createElement(gMapTagKeyValue[TAG_HEADER_MainTitle].tagString);
    mainTtle.appendChild(pXmlDoc->createTextNode(gMapTagKeyValue[TAG_HEADER_MainTitle].tagValue));
    header.appendChild(mainTtle);
    // 副标题
    QDomElement subTitle = pXmlDoc->createElement(gMapTagKeyValue[TAG_HEADER_SubTitle].tagString);
    subTitle.appendChild(pXmlDoc->createTextNode(gMapTagKeyValue[TAG_HEADER_SubTitle].tagValue));
    header.appendChild(subTitle);
}

void XmlReportGenerator::CreateReportBody(QDomDocument *pXmlDoc, QDomElement *pRootElement)
{
    QDomElement body = pXmlDoc->createElement(gMapTagKeyValue[TAG_BODY].tagString);
    pRootElement->appendChild(body);

    // 病人信息
    QDomElement patientInfo = pXmlDoc->createElement(gMapTagKeyValue[TAG_BODY_PatientInfo].tagString);
    body.appendChild(patientInfo);
    QDomElement patientInfoTitle = pXmlDoc->createElement(gMapTagKeyValue[TAG_BODY_PatientInfo_Title].tagString);
    patientInfoTitle.appendChild(pXmlDoc->createTextNode(gMapTagKeyValue[TAG_BODY_PatientInfo_Title].tagValue));
    patientInfo.appendChild(patientInfoTitle);

    QDomElement tempElement;
    QDomElement tempKeyword;
    QDomElement tempValue;

    for (int i = TAG_BODY_PatientInfo_Name; i < TAG_BODY_PatientInfo_Comment; ++i)
    {
        tempKeyword = pXmlDoc->createElement(gMapTagKeyValue[TAG_BODY_PatientInfo_Keyword].tagString);
        tempKeyword.appendChild(pXmlDoc->createTextNode(gMapTagKeyValue[i].tagKeyword));
        tempValue = pXmlDoc->createElement(gMapTagKeyValue[TAG_BODY_PatientInfo_Value].tagString);
        tempValue.appendChild(pXmlDoc->createTextNode(gMapTagKeyValue[i].tagValue));
        tempElement = pXmlDoc->createElement(gMapTagKeyValue[i].tagString);
        tempElement.appendChild(tempKeyword);
        tempElement.appendChild(tempValue);
        patientInfo.appendChild(tempElement);
    }

    QDomElement usImagesTitle = pXmlDoc->createElement(gMapTagKeyValue[TAG_BODY_UltrasoundImages_Title].tagString);
    usImagesTitle.appendChild(pXmlDoc->createTextNode(gMapTagKeyValue[TAG_BODY_UltrasoundImages_Title].tagValue));
    QDomElement countPerLine = pXmlDoc->createElement(gMapTagKeyValue[TAG_BODY_UltrasoundImages_CountPerLine].tagString);
    countPerLine.appendChild(pXmlDoc->createTextNode(gMapTagKeyValue[TAG_BODY_UltrasoundImages_CountPerLine].tagValue));
    QDomElement usImages = pXmlDoc->createElement(gMapTagKeyValue[TAG_BODY_UltrasoundImages].tagString);
    usImages.appendChild(usImagesTitle);
    usImages.appendChild(countPerLine);
    body.appendChild(usImages);

    QStringList usImgSrcList = gMapTagKeyValue[TAG_BODY_UltrasoundImages_USImage].tagValue.split(",", QString::SkipEmptyParts);
    for (int i=0; i<usImgSrcList.count(); ++i)
    {
        QDomElement usImgList = pXmlDoc->createElement(gMapTagKeyValue[TAG_BODY_UltrasoundImages_USImage].tagString);
        usImgList.appendChild(pXmlDoc->createTextNode(usImgSrcList[i]));
        usImages.appendChild(usImgList);
    }

    QDomElement examFindingsTitle = pXmlDoc->createElement(gMapTagKeyValue[TAG_BODY_ExaminationFindings_Title].tagString);
    examFindingsTitle.appendChild(pXmlDoc->createTextNode(gMapTagKeyValue[TAG_BODY_ExaminationFindings_Title].tagValue));
    QDomElement examFindingsValue = pXmlDoc->createElement(gMapTagKeyValue[TAG_BODY_ExaminationFindings_Content].tagString);
    examFindingsValue.appendChild(pXmlDoc->createTextNode(gMapTagKeyValue[TAG_BODY_ExaminationFindings_Content].tagValue));
    QDomElement examFindings = pXmlDoc->createElement(gMapTagKeyValue[TAG_BODY_ExaminationFindings].tagString);
    examFindings.appendChild(examFindingsTitle);
    examFindings.appendChild(examFindingsValue);
    body.appendChild(examFindings);

    QDomElement examConclusionTitle = pXmlDoc->createElement(gMapTagKeyValue[TAG_BODY_ExaminationConclusion_Title].tagString);
    examConclusionTitle.appendChild(pXmlDoc->createTextNode(gMapTagKeyValue[TAG_BODY_ExaminationConclusion_Title].tagValue));
    QDomElement examConclusionValue = pXmlDoc->createElement(gMapTagKeyValue[TAG_BODY_ExaminationConclusion_Content].tagString);
    examConclusionValue.appendChild(pXmlDoc->createTextNode(gMapTagKeyValue[TAG_BODY_ExaminationConclusion_Content].tagValue));
    QDomElement examConclusion = pXmlDoc->createElement(gMapTagKeyValue[TAG_BODY_ExaminationConclusion].tagString);
    examConclusion.appendChild(examConclusionTitle);
    examConclusion.appendChild(examConclusionValue);
    body.appendChild(examConclusion);
}

void XmlReportGenerator::CreateReportFooter(QDomDocument *pXmlDoc, QDomElement *pRootElement)
{
    QDomElement footer = pXmlDoc->createElement(gMapTagKeyValue[TAG_FOOTER].tagString);
    pRootElement->appendChild(footer);

    QDomElement printTimeKey = pXmlDoc->createElement(gMapTagKeyValue[TAG_FOOTER_Keyword].tagString);
    printTimeKey.appendChild(pXmlDoc->createTextNode(gMapTagKeyValue[TAG_FOOTER_PrintTime].tagKeyword));
    QDomElement printTimeValue = pXmlDoc->createElement(gMapTagKeyValue[TAG_FOOTER_Value].tagString);
    printTimeValue.appendChild(pXmlDoc->createTextNode(gMapTagKeyValue[TAG_FOOTER_PrintTime].tagValue));
    QDomElement printTime = pXmlDoc->createElement(gMapTagKeyValue[TAG_FOOTER_PrintTime].tagString);
    printTime.appendChild(printTimeKey);
    printTime.appendChild(printTimeValue);
    footer.appendChild(printTime);

    QDomElement signatureKey = pXmlDoc->createElement(gMapTagKeyValue[TAG_FOOTER_Keyword].tagString);
    signatureKey.appendChild(pXmlDoc->createTextNode(gMapTagKeyValue[TAG_FOOTER_DoctorSignature].tagKeyword));
    QDomElement signatureValue = pXmlDoc->createElement(gMapTagKeyValue[TAG_FOOTER_Value].tagString);
    signatureValue.appendChild(pXmlDoc->createTextNode(gMapTagKeyValue[TAG_FOOTER_DoctorSignature].tagValue));
    QDomElement doctorSignature = pXmlDoc->createElement(gMapTagKeyValue[TAG_FOOTER_DoctorSignature].tagString);
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
