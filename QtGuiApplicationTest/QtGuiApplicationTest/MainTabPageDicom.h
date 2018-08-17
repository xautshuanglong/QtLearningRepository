#ifndef MAIN_TAB_PAGE_DICOM_H
#define MAIN_TAB_PAGE_DICOM_H

#include "qwidget.h"
#include "ui_MainTabPageDicom.h"

#include <QList>

class QSplitter;
class DcmObject;

struct DcmElementInfo
{
    QString strDcmTagKey;         // DcmTagKey
    QString strDcmTagName;        // Tag ��ֵ��ʾ�����ƣ�������
    QString strDcmElementValue;   // Tag ��Ӧ��Ԫ�ص�ֵ�����ֻ��ַ�����
};

class MainTabPageDicom : public QWidget
{
    Q_OBJECT

private:
    Ui::MainTabPageDicom    ui;
    QList<DcmElementInfo>   mListDcmElementInfo;
    QSplitter              *mpMainSpliter;
    unsigned char          *mpImageData;              // ��������
    unsigned short          mBitStored;                // �洢λ��
    unsigned short          mSamplePerPixel;           // ���ز���
    unsigned short          mImageWidth, mImageHeight; // ͼ��ߴ�
    int                     mRescaleSlope, mRescaleIntercept; // б�� �ؾ�
    int                     mWindowCenter, mWindowWidth;
    QImage                  mDcmImage;

public:
    explicit MainTabPageDicom(QWidget *parent = Q_NULLPTR);
    ~MainTabPageDicom();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_action_open();
    void on_action_save();

private:
    void LoadDicomFile(const QString& inDcmFilename);
    void GetDicomElementImage(QImage& outImage);
    void ConvertDicomToQImage(QString &dcmFilename, QImage **ppOutImage);
    void GetDicomElementValue(QString &outStrValue, DcmObject *pInDcmObj);
    void UpdateDcmTabTableContent();
    void ClearDataBuffer(unsigned char **pDataBuffer);
};

#endif MAIN_TAB_PAGE_DICOM_H
