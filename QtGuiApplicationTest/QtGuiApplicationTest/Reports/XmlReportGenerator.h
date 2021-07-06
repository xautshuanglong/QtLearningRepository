#pragma once

#include <QString>
#include <QStringList>
#include <QPair>

class QDomDocument;
class QDomElement;
template <class Key, class T> class QMap;

/************************************************************************/
/* ���������� �� UI ��ϱ����������л��� xml���Ա� fop ��������Ű档       */
/*     ������·�Ϊ�������֣�ͷ����Header��������(Body)��β��(Footer):     */
/*     ͷ����Header��: logo�������⡢������                               */
/*     ����(Body)                                                       */
/*     β��(Footer): ��ӡʱ�䡢ǩ��                                      */
/************************************************************************/

class XmlReportGenerator
{
public:
    enum ReportXmlTag
    {
        TAG_ROOT,                               // ����ǩ
        TAG_HEADER,                             // ����ͷ��
        TAG_HEADER_HospitalLogo,                // ҽԺ��־ͼƬ·��
        TAG_HEADER_MainTitle,                   // ����������
        TAG_HEADER_SubTitle,                    // ���渱����
        TAG_BODY,                               // ��������
        TAG_BODY_PatientInfo,                   // ������Ϣ
        TAG_BODY_PatientInfo_Title,             // ������Ϣ-������
        TAG_BODY_PatientInfo_Name,              // ������Ϣ-����
        TAG_BODY_PatientInfo_PatientNumber,     // ������Ϣ-�����
        TAG_BODY_PatientInfo_Method,            // ������Ϣ-��鷽��
        TAG_BODY_PatientInfo_Date,              // ������Ϣ-�������
        TAG_BODY_PatientInfo_Sex,               // ������Ϣ-�Ա�
        TAG_BODY_PatientInfo_Nation,            // ������Ϣ-����
        TAG_BODY_PatientInfo_IDCard,            // ������Ϣ-���֤��
        TAG_BODY_PatientInfo_PhoneNumber,       // ������Ϣ-��ϵ�绰
        TAG_BODY_PatientInfo_Stature,           // ������Ϣ-���
        TAG_BODY_PatientInfo_Weight,            // ������Ϣ-����
        TAG_BODY_PatientInfo_InpatientArea,     // ������Ϣ-����
        TAG_BODY_PatientInfo_Mainpulator,       // ������Ϣ-������
        TAG_BODY_PatientInfo_Age,               // ������Ϣ-����
        TAG_BODY_PatientInfo_Physician,         // ������Ϣ-��¼ҽʦ
        TAG_BODY_PatientInfo_Consultant,        // ������Ϣ-����ҽ��
        TAG_BODY_PatientInfo_ExamDescription,   // ������Ϣ-�������
        TAG_BODY_PatientInfo_Comment,           // ������Ϣ-��ע
        TAG_BODY_PatientInfo_Keyword,           // ������Ϣ-�ؼ���
        TAG_BODY_PatientInfo_Value,             // ������Ϣ-ֵ
        TAG_BODY_UltrasoundImages,              // ����ͼ��
        TAG_BODY_UltrasoundImages_Title,        // ����ͼ��-������
        TAG_BODY_UltrasoundImages_CountPerLine, // ����ͼ��-ÿ��ͼ����
        TAG_BODY_UltrasoundImages_USImage,      // ����ͼ��-ͼ��·��
        TAG_BODY_ExaminationFindings,           // �������
        TAG_BODY_ExaminationFindings_Title,     // �������-������
        TAG_BODY_ExaminationFindings_Content,   // �������-����
        TAG_BODY_ExaminationConclusion,         // ��Ͻ��
        TAG_BODY_ExaminationConclusion_Title,   // ��Ͻ��-������
        TAG_BODY_ExaminationConclusion_Content, // ��Ͻ��-����
        TAG_FOOTER,                             // ����β��
        TAG_FOOTER_PrintTime,                   // �����ӡʱ��
        TAG_FOOTER_Keyword,                     // ��ӡʱ��-�ؼ���
        TAG_FOOTER_Value,                       // ��ӡʱ��-ֵ
        TAG_FOOTER_DoctorSignature,             // ҽ��ǩ���ļ�·��
        TAG_MAX
    };
    typedef struct tagTagKeyValue
    {
        ReportXmlTag  tagIndex;
        QString       tagString;
        QString       tagKeyword;
        QString       tagValue;
    }TagKeyValue;

    XmlReportGenerator();
    ~XmlReportGenerator();

    void SaveReportAsXml(const QString& xmlFilename);

    void SetTagString(const ReportXmlTag tag, const QString& tagString);
    QString GetTagString(const ReportXmlTag tag);

    void SetTagKeyword(const ReportXmlTag tag, const QString& tagKeyword);
    QString GetTagKeyword(const ReportXmlTag tag);

    void SetTagValue(const ReportXmlTag tag, const QString& tagValue);
    QString GetTagValue(const ReportXmlTag tag);

    void SetTag(const ReportXmlTag tag, const TagKeyValue& tagKeyValue);
    void GetTag(const ReportXmlTag tag, TagKeyValue& outTagKeyValue);

    void ResetTagKeyValue();

private:
    void CreateReport(QDomDocument *pXmlDoc);
    void CreateReportHeader(QDomDocument *pXmlDoc, QDomElement *pRootElement);
    void CreateReportBody(QDomDocument *pXmlDoc, QDomElement *pRootElement);
    void CreateReportFooter(QDomDocument *pXmlDoc, QDomElement *pRootElement);

private:
    static const TagKeyValue gMapTagKeyValue[0];
    TagKeyValue                               mMapTagKeyValue[TAG_MAX];
};
