#include "ReportXmlData.h"

#include <QDomDocument>
#include <QTextStream>
#include <QDateTime>
#include <QFile>

ReportXmlData::ReportXmlData()
{
}

ReportXmlData::~ReportXmlData()
{
}

void ReportXmlData::SaveReportAsXml(const QString& xmlFilename)
{
    QDomDocument xmlDoc;
    this->CreateReport(&xmlDoc);

    QDomElement rootElement = xmlDoc.createElement("MGIUS_Report");
    xmlDoc.appendChild(rootElement);

    this->CreateHospitalInfos(&xmlDoc, &rootElement);
    this->CreatePatientInfos(&xmlDoc, &rootElement);
    this->CreateUltrasoundImages(&xmlDoc, &rootElement);
    this->CreateExaminationFindings(&xmlDoc, &rootElement);
    this->CreateExaminationConclusion(&xmlDoc, &rootElement);
    this->CreatePrintTime(&xmlDoc, &rootElement);
    this->CreateDoctorSignature(&xmlDoc, &rootElement);

    QFile xmlFile(xmlFilename);
    if (xmlFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        QTextStream outXmlStream(&xmlFile);
        xmlDoc.save(outXmlStream, 4, QDomNode::EncodingFromDocument);
        outXmlStream.flush();
        xmlFile.close();
    }
}

void ReportXmlData::MakeDefaultData()
{
    this->ClearPatientInfo();

    // 姓名
    PatientInfoItem name;
    name.strRemark = "name";
    name.strLabel = QStringLiteral("姓名：");
    name.strValue = QStringLiteral("张三");
    this->AddPatientInfo(name);

    // 门诊号
    PatientInfoItem patientNumber;
    patientNumber.strRemark = "patient_number";
    patientNumber.strLabel = QStringLiteral("门诊号：");
    patientNumber.strValue = "123456";
    this->AddPatientInfo(patientNumber);

    // 检查方法
    PatientInfoItem method;
    method.strRemark = "method";
    method.strLabel = QStringLiteral("检查方法：");
    method.strValue = QStringLiteral("远程超声");
    this->AddPatientInfo(method);

    // 检查日期
    PatientInfoItem date;
    date.strRemark = "date";
    date.strLabel = QStringLiteral("日期：");
    date.strValue = QStringLiteral("2018/11/20");
    this->AddPatientInfo(date);

    // 性别
    PatientInfoItem sex;
    sex.strRemark = "sex";
    sex.strLabel = QStringLiteral("性别：");
    sex.strValue = QStringLiteral("男");
    this->AddPatientInfo(sex);

    // 民族
    PatientInfoItem nation;
    nation.strRemark = "nation";
    nation.strLabel = QStringLiteral("民族：");
    nation.strValue = QStringLiteral("汉");
    this->AddPatientInfo(nation);

    // 身份证号
    PatientInfoItem IdNumber;
    IdNumber.strRemark = "id_number";
    IdNumber.strLabel = QStringLiteral("身份证号：");
    IdNumber.strValue = "651227199011045178";
    this->AddPatientInfo(IdNumber);

    // 联系电话
    PatientInfoItem phone;
    phone.strRemark = "phone";
    phone.strLabel = QStringLiteral("联系电话：");
    phone.strValue = QStringLiteral("13892441234");
    this->AddPatientInfo(phone);

    // 身高
    PatientInfoItem stature;
    stature.strRemark = "stature";
    stature.strLabel = QStringLiteral("身高：");
    stature.strValue = QStringLiteral("175cm");
    this->AddPatientInfo(stature);

    // 体重
    PatientInfoItem weight;
    weight.strRemark = "weight";
    weight.strLabel = QStringLiteral("体重：");
    weight.strValue = QStringLiteral("70kg");
    this->AddPatientInfo(weight);

    // 病区
    PatientInfoItem inpatientArea;
    inpatientArea.strRemark = "inpatient_area";
    inpatientArea.strLabel = QStringLiteral("病区：");
    inpatientArea.strValue = QStringLiteral("A30");
    this->AddPatientInfo(inpatientArea);

    // 操作者
    PatientInfoItem manipulator;
    manipulator.strRemark = "manipulator";
    manipulator.strLabel = QStringLiteral("操作者：");
    manipulator.strValue = QStringLiteral("张飞");
    this->AddPatientInfo(manipulator);

    // 年龄
    PatientInfoItem age;
    age.strRemark = "age";
    age.strLabel = QStringLiteral("年龄：");
    age.strValue = QStringLiteral("28");
    this->AddPatientInfo(age);

    // 记录医师
    PatientInfoItem physician;
    physician.strRemark = "physician";
    physician.strLabel = QStringLiteral("记录医师：");
    physician.strValue = QStringLiteral("赵云");
    this->AddPatientInfo(physician);

    // 会诊医生
    PatientInfoItem consultant;
    consultant.strRemark = "consultant";
    consultant.strLabel = QStringLiteral("会诊医生：");
    consultant.strValue = QStringLiteral("麻子");
    this->AddPatientInfo(consultant);

    // 检查描述
    PatientInfoItem examDescription;
    examDescription.strRemark = "exam_description";
    examDescription.strLabel = QStringLiteral("检查描述：");
    examDescription.strValue = QStringLiteral("检查描述 检查描述 检查描述 检查描述");
    this->AddPatientInfo(examDescription);

    // 备注
    PatientInfoItem comment;
    comment.strRemark = "manipulator";
    comment.strLabel = QStringLiteral("备注：");
    comment.strValue = QStringLiteral("备注文本内容 备注文本内容 备注文本内容 备注文本内容");
    this->AddPatientInfo(comment);

    mListUsImagePath.append("image_us_1.png");
    mListUsImagePath.append("image_us_2.png");
    mListUsImagePath.append("image_us_3.png");
    mListUsImagePath.append("image_us_4.png");

    // 签名图片
    mStrDoctorSignature = "image_doctor_signature.png";
}

void ReportXmlData::ClearPatientInfo()
{
    mListPatientInfo.clear();
}

void ReportXmlData::AddPatientInfo(PatientInfoItem& info)
{
    mListPatientInfo.append(info);
}

void ReportXmlData::CreateReport(QDomDocument *pXmlDoc)
{
    QDomProcessingInstruction instruction = pXmlDoc->createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    pXmlDoc->appendChild(instruction);
}

void ReportXmlData::CreateHospitalInfos(QDomDocument *pXmlDoc, QDomElement *pRootElement)
{
    QDomElement hospitalInfos = pXmlDoc->createElement("HospitalInfos");
    pRootElement->appendChild(hospitalInfos);

    // 医院 logo
    QDomElement logo = pXmlDoc->createElement("HospitalInfo");
    logo.setAttribute("remark", "logo");
    logo.appendChild(pXmlDoc->createTextNode("image_logo.png"));
    hospitalInfos.appendChild(logo);
    // 主标题
    QDomElement mainTtle = pXmlDoc->createElement("HospitalInfo");
    mainTtle.setAttribute("remark", "title");
    mainTtle.appendChild(pXmlDoc->createTextNode(QStringLiteral("xxx 医院")));
    hospitalInfos.appendChild(mainTtle);
    // 副标题
    QDomElement subTitle = pXmlDoc->createElement("HospitalInfo");
    subTitle.setAttribute("remark", "subtitle");
    subTitle.appendChild(pXmlDoc->createTextNode(QStringLiteral("甲状腺超声检查报告")));
    hospitalInfos.appendChild(subTitle);
}

void ReportXmlData::CreatePatientInfos(QDomDocument *pXmlDoc, QDomElement *pRootElement)
{
    QDomElement patientInfos = pXmlDoc->createElement("PatientInfos");
    pRootElement->appendChild(patientInfos);

    // 标题 —— 病人信息
    QDomElement title = pXmlDoc->createElement("Title");
    title.appendChild(pXmlDoc->createTextNode(QStringLiteral("病人信息：")));
    patientInfos.appendChild(title);

    for each (PatientInfoItem patient in mListPatientInfo)
    {
        QDomElement info = pXmlDoc->createElement("PatientInfo");
        info.setAttribute("remark", patient.strRemark);
        QDomElement infoLabel = pXmlDoc->createElement("Label");
        infoLabel.appendChild(pXmlDoc->createTextNode(patient.strLabel));
        QDomElement infoValue = pXmlDoc->createElement("Value");
        infoValue.appendChild(pXmlDoc->createTextNode(patient.strValue));
        info.appendChild(infoLabel);
        info.appendChild(infoValue);
        patientInfos.appendChild(info);
    }
}

void ReportXmlData::CreateUltrasoundImages(QDomDocument *pXmlDoc, QDomElement *pRootElement)
{
    QDomElement ultrasoundImage = pXmlDoc->createElement("UltrasoundImages");
    pRootElement->appendChild(ultrasoundImage);

    // 标题 —— 超声图片
    QDomElement title = pXmlDoc->createElement("Title");
    title.appendChild(pXmlDoc->createTextNode(QStringLiteral("超声图片：")));
    ultrasoundImage.appendChild(title);

    // 图片列数
    QDomElement countPerLine = pXmlDoc->createElement("CountPerLine");
    countPerLine.appendChild(pXmlDoc->createTextNode("2"));
    ultrasoundImage.appendChild(countPerLine);

    // 图片路径
    for each (QString imagePath in mListUsImagePath)
    {
        QDomElement usImage = pXmlDoc->createElement("USImage");
        usImage.appendChild(pXmlDoc->createTextNode(imagePath));
        ultrasoundImage.appendChild(usImage);
    }
}

void ReportXmlData::CreateExaminationFindings(QDomDocument *pXmlDoc, QDomElement *pRootElement)
{
    QDomElement examFindings = pXmlDoc->createElement("ExaminationFindings");
    pRootElement->appendChild(examFindings);

    // 标题 —— 检查所见
    QDomElement title = pXmlDoc->createElement("Title");
    title.appendChild(pXmlDoc->createTextNode(QStringLiteral("检查所见：")));
    examFindings.appendChild(title);

    // 检查所见内容
    QDomElement content = pXmlDoc->createElement("Content");
    content.appendChild(pXmlDoc->createTextNode(QStringLiteral("检查所见内容 检查所见内容 检查所见内容 检查所见内容")));
    examFindings.appendChild(content);
}

void ReportXmlData::CreateExaminationConclusion(QDomDocument *pXmlDoc, QDomElement *pRootElement)
{
    QDomElement examConclusion = pXmlDoc->createElement("ExaminationConclusion");
    pRootElement->appendChild(examConclusion);

    // 标题 —— 诊断结果
    QDomElement title = pXmlDoc->createElement("Title");
    title.appendChild(pXmlDoc->createTextNode(QStringLiteral("诊断结果：")));
    examConclusion.appendChild(title);

    // 检查所见内容
    QDomElement content = pXmlDoc->createElement("Content");
    content.appendChild(pXmlDoc->createTextNode(QStringLiteral("诊断结果内容 诊断结果内容 诊断结果内容 诊断结果内容")));
    examConclusion.appendChild(content);
}

void ReportXmlData::CreatePrintTime(QDomDocument *pXmlDoc, QDomElement *pRootElement)
{
    // 打印时间
    QDomElement printTime = pXmlDoc->createElement("PrintTime");
    pRootElement->appendChild(printTime);

    QDomElement printTimeLabel = pXmlDoc->createElement("Label");
    printTimeLabel.appendChild(pXmlDoc->createTextNode(QStringLiteral("打印时间：")));
    printTime.appendChild(printTimeLabel);

    QDomElement printTimeValue = pXmlDoc->createElement("Value");
    printTimeValue.appendChild(pXmlDoc->createTextNode(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm")));
    printTime.appendChild(printTimeValue);
}

void ReportXmlData::CreateDoctorSignature(QDomDocument *pXmlDoc, QDomElement *pRootElement)
{
    // 医生签名
    QDomElement signature = pXmlDoc->createElement("DoctorSignature");
    pRootElement->appendChild(signature);

    QDomElement signatureLabel = pXmlDoc->createElement("Label");
    signatureLabel.appendChild(pXmlDoc->createTextNode(QStringLiteral("医生签名：")));
    signature.appendChild(signatureLabel);

    QDomElement signatureValue = pXmlDoc->createElement("Value");
    signatureValue.appendChild(pXmlDoc->createTextNode(mStrDoctorSignature));
    signature.appendChild(signatureValue);
}
