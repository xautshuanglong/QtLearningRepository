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
    QString strDcmTagName;        // Tag 的值表示与名称（描述）
    QString strDcmElementValue;   // Tag 对应的元素的值（数字或字符串）
};

class MainTabPageDicom : public QWidget
{
    Q_OBJECT

private:
    Ui::MainTabPageDicom    ui;
    QList<DcmElementInfo>   mListDcmElementInfo;
    QSplitter              *mpMainSpliter;
    unsigned char          *mpImageData;               // 像素数据
    unsigned short          mBitAllocated;             // 申请位数
    unsigned short          mBitStored;                // 存储位数
    unsigned short          mSamplePerPixel;           // 像素采样
    unsigned short          mImageWidth, mImageHeight; // 图像尺寸(根据情况缩放)
    //unsigned short          mDcmImageWidth, mDcmImageHeight; // DCM图像原始尺寸(通常比较大)
    int                     mRescaleSlope, mRescaleIntercept; // 斜率 截距
    int                     mWindowCenter, mWindowWidth;
    QImage                  mDcmImage;
    QString                 mDcmPhotometricInterpretation; // 广度表示 RGB/MONOCHROME1/MONOCHROME2

public:
    explicit MainTabPageDicom(QWidget *parent = Q_NULLPTR);
    ~MainTabPageDicom();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_action_open();
    void on_action_save();
    void on_action_previous();
    void on_action_next();

private:
    void LoadDicomFile(const QString& inDcmFilename);
    void GetDicomElementImage(QImage& outImage);
    void ConvertDicomToQImage(QString &dcmFilename, QImage **ppOutImage);
    void GetPixelValueFromPhotometric(int& pixelValue, int pixelValueMin, int pixelValueMax, int windowWidth);
    void GetDicomElementValue(QString &outStrValue, DcmObject *pInDcmObj);
    void ShowDicomImage();
    void UpdateDcmTabTableContent();
    void ClearDataBuffer(unsigned char **pDataBuffer);
};

#endif MAIN_TAB_PAGE_DICOM_H
