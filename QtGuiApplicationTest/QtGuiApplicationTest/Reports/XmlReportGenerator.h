#pragma once

#include <QString>
#include <QStringList>
#include <QPair>

class QDomDocument;
class QDomElement;
template <class Key, class T> class QMap;

/************************************************************************/
/* 功能描述： 将 UI 诊断报告内容序列化到 xml，以便 fop 对其进行排版。       */
/*     报告大致分为三个部分：头部（Header）、正文(Body)、尾部(Footer):     */
/*     头部（Header）: logo、主标题、副标题                               */
/*     正文(Body)                                                       */
/*     尾部(Footer): 打印时间、签名                                      */
/************************************************************************/

class XmlReportGenerator
{
public:
    enum ReportXmlTag
    {
        TAG_ROOT,                               // 根标签
        TAG_HEADER,                             // 报告头部
        TAG_HEADER_HospitalLogo,                // 医院标志图片路径
        TAG_HEADER_MainTitle,                   // 报告主标题
        TAG_HEADER_SubTitle,                    // 报告副标题
        TAG_BODY,                               // 报告正文
        TAG_BODY_PatientInfo,                   // 病人信息
        TAG_BODY_PatientInfo_Title,             // 病人信息-板块标题
        TAG_BODY_PatientInfo_Name,              // 病人信息-姓名
        TAG_BODY_PatientInfo_PatientNumber,     // 病人信息-门诊号
        TAG_BODY_PatientInfo_Method,            // 病人信息-检查方法
        TAG_BODY_PatientInfo_Date,              // 病人信息-检查日期
        TAG_BODY_PatientInfo_Sex,               // 病人信息-性别
        TAG_BODY_PatientInfo_Nation,            // 病人信息-民族
        TAG_BODY_PatientInfo_IDCard,            // 病人信息-身份证号
        TAG_BODY_PatientInfo_PhoneNumber,       // 病人信息-联系电话
        TAG_BODY_PatientInfo_Stature,           // 病人信息-身高
        TAG_BODY_PatientInfo_Weight,            // 病人信息-体重
        TAG_BODY_PatientInfo_InpatientArea,     // 病人信息-病区
        TAG_BODY_PatientInfo_Mainpulator,       // 病人信息-操作者
        TAG_BODY_PatientInfo_Age,               // 病人信息-年龄
        TAG_BODY_PatientInfo_Physician,         // 病人信息-记录医师
        TAG_BODY_PatientInfo_Consultant,        // 病人信息-会诊医生
        TAG_BODY_PatientInfo_ExamDescription,   // 病人信息-检查描述
        TAG_BODY_PatientInfo_Comment,           // 病人信息-备注
        TAG_BODY_PatientInfo_Keyword,           // 病人信息-关键字
        TAG_BODY_PatientInfo_Value,             // 病人信息-值
        TAG_BODY_UltrasoundImages,              // 超声图像
        TAG_BODY_UltrasoundImages_Title,        // 超声图像-板块标题
        TAG_BODY_UltrasoundImages_CountPerLine, // 超声图像-每行图像数
        TAG_BODY_UltrasoundImages_USImage,      // 超声图像-图像路径
        TAG_BODY_ExaminationFindings,           // 检查所见
        TAG_BODY_ExaminationFindings_Title,     // 检查所见-板块标题
        TAG_BODY_ExaminationFindings_Content,   // 检查所见-内容
        TAG_BODY_ExaminationConclusion,         // 诊断结果
        TAG_BODY_ExaminationConclusion_Title,   // 诊断结果-板块标题
        TAG_BODY_ExaminationConclusion_Content, // 诊断结果-内容
        TAG_FOOTER,                             // 报告尾部
        TAG_FOOTER_PrintTime,                   // 报告打印时间
        TAG_FOOTER_Keyword,                     // 打印时间-关键字
        TAG_FOOTER_Value,                       // 打印时间-值
        TAG_FOOTER_DoctorSignature,             // 医生签名文件路径
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
