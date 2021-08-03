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
    unsigned char          *mpImageData;               // ��������
    unsigned short          mBitAllocated;             // ����λ��
    unsigned short          mBitStored;                // �洢λ��
    unsigned short          mSamplePerPixel;           // ���ز���
    unsigned short          mImageWidth, mImageHeight; // ͼ��ߴ�(�����������)
    //unsigned short          mDcmImageWidth, mDcmImageHeight; // DCMͼ��ԭʼ�ߴ�(ͨ���Ƚϴ�)
    int                     mRescaleSlope, mRescaleIntercept; // б�� �ؾ�
    int                     mWindowCenter, mWindowWidth;
    int                     mDcmFrameIndex, mDcmFrameCount; // ��֡ DCM �ļ��ĵ�ǰ��ʾ֡����֡��
    QImage                  mDcmImage;
    QString                 mDcmPhotometricInterpretation;  // ��ȶȱ�ʾ RGB/MONOCHROME1/MONOCHROME2
    QAction                *mpActionOpen;     // ��
    QAction                *mpActionSave;     // ����
    QAction                *mpActionPrevious; // ��һ֡
    QAction                *mpActionNext;     // ��һ֡

public:
    explicit MainTabPageDicom(QWidget *parent = Q_NULLPTR);
    ~MainTabPageDicom();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void slot_action_open();
    void slot_action_save();
    void slot_action_previous();
    void slot_action_next();

private:
    void LoadDicomFile(const QString& inDcmFilename);
    void GetDicomElementImage(QImage& outImage, int frameIndex = 0, int frameCount = 1);
    void ConvertDicomToQImage(QString &dcmFilename, QImage **ppOutImage);
    void GetPixelValueFromPhotometric(int& pixelValue, int pixelValueMin, int pixelValueMax, int windowWidth);
    void GetDicomElementValue(QString &outStrValue, DcmObject *pInDcmObj);
    void ShowDicomImage();
    void UpdateDcmTabTableContent();
    void UpdateActionButtonState();
    void ClearDataBuffer(unsigned char **pDataBuffer);
};

#endif MAIN_TAB_PAGE_DICOM_H
