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

    // ����
    PatientInfoItem name;
    name.strRemark = "name";
    name.strLabel = QStringLiteral("������");
    name.strValue = QStringLiteral("����");
    this->AddPatientInfo(name);

    // �����
    PatientInfoItem patientNumber;
    patientNumber.strRemark = "patient_number";
    patientNumber.strLabel = QStringLiteral("����ţ�");
    patientNumber.strValue = "123456";
    this->AddPatientInfo(patientNumber);

    // ��鷽��
    PatientInfoItem method;
    method.strRemark = "method";
    method.strLabel = QStringLiteral("��鷽����");
    method.strValue = QStringLiteral("Զ�̳���");
    this->AddPatientInfo(method);

    // �������
    PatientInfoItem date;
    date.strRemark = "date";
    date.strLabel = QStringLiteral("���ڣ�");
    date.strValue = QStringLiteral("2018/11/20");
    this->AddPatientInfo(date);

    // �Ա�
    PatientInfoItem sex;
    sex.strRemark = "sex";
    sex.strLabel = QStringLiteral("�Ա�");
    sex.strValue = QStringLiteral("��");
    this->AddPatientInfo(sex);

    // ����
    PatientInfoItem nation;
    nation.strRemark = "nation";
    nation.strLabel = QStringLiteral("���壺");
    nation.strValue = QStringLiteral("��");
    this->AddPatientInfo(nation);

    // ���֤��
    PatientInfoItem IdNumber;
    IdNumber.strRemark = "id_number";
    IdNumber.strLabel = QStringLiteral("���֤�ţ�");
    IdNumber.strValue = "651227199011045178";
    this->AddPatientInfo(IdNumber);

    // ��ϵ�绰
    PatientInfoItem phone;
    phone.strRemark = "phone";
    phone.strLabel = QStringLiteral("��ϵ�绰��");
    phone.strValue = QStringLiteral("13892441234");
    this->AddPatientInfo(phone);

    // ���
    PatientInfoItem stature;
    stature.strRemark = "stature";
    stature.strLabel = QStringLiteral("��ߣ�");
    stature.strValue = QStringLiteral("175cm");
    this->AddPatientInfo(stature);

    // ����
    PatientInfoItem weight;
    weight.strRemark = "weight";
    weight.strLabel = QStringLiteral("���أ�");
    weight.strValue = QStringLiteral("70kg");
    this->AddPatientInfo(weight);

    // ����
    PatientInfoItem inpatientArea;
    inpatientArea.strRemark = "inpatient_area";
    inpatientArea.strLabel = QStringLiteral("������");
    inpatientArea.strValue = QStringLiteral("A30");
    this->AddPatientInfo(inpatientArea);

    // ������
    PatientInfoItem manipulator;
    manipulator.strRemark = "manipulator";
    manipulator.strLabel = QStringLiteral("�����ߣ�");
    manipulator.strValue = QStringLiteral("�ŷ�");
    this->AddPatientInfo(manipulator);

    // ����
    PatientInfoItem age;
    age.strRemark = "age";
    age.strLabel = QStringLiteral("���䣺");
    age.strValue = QStringLiteral("28");
    this->AddPatientInfo(age);

    // ��¼ҽʦ
    PatientInfoItem physician;
    physician.strRemark = "physician";
    physician.strLabel = QStringLiteral("��¼ҽʦ��");
    physician.strValue = QStringLiteral("����");
    this->AddPatientInfo(physician);

    // ����ҽ��
    PatientInfoItem consultant;
    consultant.strRemark = "consultant";
    consultant.strLabel = QStringLiteral("����ҽ����");
    consultant.strValue = QStringLiteral("����");
    this->AddPatientInfo(consultant);

    // �������
    PatientInfoItem examDescription;
    examDescription.strRemark = "exam_description";
    examDescription.strLabel = QStringLiteral("���������");
    examDescription.strValue = QStringLiteral("������� ������� ������� �������");
    this->AddPatientInfo(examDescription);

    // ��ע
    PatientInfoItem comment;
    comment.strRemark = "manipulator";
    comment.strLabel = QStringLiteral("��ע��");
    comment.strValue = QStringLiteral("��ע�ı����� ��ע�ı����� ��ע�ı����� ��ע�ı�����");
    this->AddPatientInfo(comment);

    mListUsImagePath.append("image_us_1.png");
    mListUsImagePath.append("image_us_2.png");
    mListUsImagePath.append("image_us_3.png");
    mListUsImagePath.append("image_us_4.png");

    // ǩ��ͼƬ
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

    // ҽԺ logo
    QDomElement logo = pXmlDoc->createElement("HospitalInfo");
    logo.setAttribute("remark", "logo");
    logo.appendChild(pXmlDoc->createTextNode("image_logo.png"));
    hospitalInfos.appendChild(logo);
    // ������
    QDomElement mainTtle = pXmlDoc->createElement("HospitalInfo");
    mainTtle.setAttribute("remark", "title");
    mainTtle.appendChild(pXmlDoc->createTextNode(QStringLiteral("xxx ҽԺ")));
    hospitalInfos.appendChild(mainTtle);
    // ������
    QDomElement subTitle = pXmlDoc->createElement("HospitalInfo");
    subTitle.setAttribute("remark", "subtitle");
    subTitle.appendChild(pXmlDoc->createTextNode(QStringLiteral("��״�ٳ�����鱨��")));
    hospitalInfos.appendChild(subTitle);
}

void ReportXmlData::CreatePatientInfos(QDomDocument *pXmlDoc, QDomElement *pRootElement)
{
    QDomElement patientInfos = pXmlDoc->createElement("PatientInfos");
    pRootElement->appendChild(patientInfos);

    // ���� ���� ������Ϣ
    QDomElement title = pXmlDoc->createElement("Title");
    title.appendChild(pXmlDoc->createTextNode(QStringLiteral("������Ϣ��")));
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

    // ���� ���� ����ͼƬ
    QDomElement title = pXmlDoc->createElement("Title");
    title.appendChild(pXmlDoc->createTextNode(QStringLiteral("����ͼƬ��")));
    ultrasoundImage.appendChild(title);

    // ͼƬ����
    QDomElement countPerLine = pXmlDoc->createElement("CountPerLine");
    countPerLine.appendChild(pXmlDoc->createTextNode("2"));
    ultrasoundImage.appendChild(countPerLine);

    // ͼƬ·��
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

    // ���� ���� �������
    QDomElement title = pXmlDoc->createElement("Title");
    title.appendChild(pXmlDoc->createTextNode(QStringLiteral("���������")));
    examFindings.appendChild(title);

    // �����������
    QDomElement content = pXmlDoc->createElement("Content");
    content.appendChild(pXmlDoc->createTextNode(QStringLiteral("����������� ����������� ����������� �����������")));
    examFindings.appendChild(content);
}

void ReportXmlData::CreateExaminationConclusion(QDomDocument *pXmlDoc, QDomElement *pRootElement)
{
    QDomElement examConclusion = pXmlDoc->createElement("ExaminationConclusion");
    pRootElement->appendChild(examConclusion);

    // ���� ���� ��Ͻ��
    QDomElement title = pXmlDoc->createElement("Title");
    title.appendChild(pXmlDoc->createTextNode(QStringLiteral("��Ͻ����")));
    examConclusion.appendChild(title);

    // �����������
    QDomElement content = pXmlDoc->createElement("Content");
    content.appendChild(pXmlDoc->createTextNode(QStringLiteral("��Ͻ������ ��Ͻ������ ��Ͻ������ ��Ͻ������")));
    examConclusion.appendChild(content);
}

void ReportXmlData::CreatePrintTime(QDomDocument *pXmlDoc, QDomElement *pRootElement)
{
    // ��ӡʱ��
    QDomElement printTime = pXmlDoc->createElement("PrintTime");
    pRootElement->appendChild(printTime);

    QDomElement printTimeLabel = pXmlDoc->createElement("Label");
    printTimeLabel.appendChild(pXmlDoc->createTextNode(QStringLiteral("��ӡʱ�䣺")));
    printTime.appendChild(printTimeLabel);

    QDomElement printTimeValue = pXmlDoc->createElement("Value");
    printTimeValue.appendChild(pXmlDoc->createTextNode(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm")));
    printTime.appendChild(printTimeValue);
}

void ReportXmlData::CreateDoctorSignature(QDomDocument *pXmlDoc, QDomElement *pRootElement)
{
    // ҽ��ǩ��
    QDomElement signature = pXmlDoc->createElement("DoctorSignature");
    pRootElement->appendChild(signature);

    QDomElement signatureLabel = pXmlDoc->createElement("Label");
    signatureLabel.appendChild(pXmlDoc->createTextNode(QStringLiteral("ҽ��ǩ����")));
    signature.appendChild(signatureLabel);

    QDomElement signatureValue = pXmlDoc->createElement("Value");
    signatureValue.appendChild(pXmlDoc->createTextNode(mStrDoctorSignature));
    signature.appendChild(signatureValue);
}
