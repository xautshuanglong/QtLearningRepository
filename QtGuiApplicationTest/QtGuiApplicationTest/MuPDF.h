#ifndef MU_PDF_H
#define MU_PDF_H

#include <QObject>
#include <QString>
#include <QImage>

#include <mupdf/fitz.h>

class MuPDF : public QObject
{
    Q_OBJECT

public:
    enum ErrorCode
    {
        ECODE_Sccess,
        ECODE_NotFound,
        ECODE_NewContextFailed,
        ECODE_Unknown
    };
    Q_ENUM(ErrorCode)

private:
    QString      mFilename;
    QSize        mPageSize;
    QImage       mCurPageImage;
    int          mPageNumberTotal;    // PDF 文档总页数
    int          mPageNumberCurrent;  // PDF 文档当前浏览页编号
    bool         mIsOpened;           // 指示 PDF 文档是否为打开状态
    fz_context  *mPdfContext;         // MuPDF 上下文
    fz_document *mPdfDocument;        // MuPDF 文档
    fz_rect      mViewPort;           // 视口大小，预览控件大小
    fz_matrix    mMatrixComposition;  // 复合转换矩阵：缩放、旋转、平移等组合后的变换效果。
    fz_matrix    mMatrixScale;        // 缩放变换
    fz_matrix    mMatrixRotate;       // 旋转变换
    fz_matrix    mMatrixTranslate;    // 平移变换

public:
    MuPDF();
    MuPDF(QSize pageSize);
    ~MuPDF();

    bool Open(QString filename);
    void Close();
    void Print();
    QImage GetPage(int pageNumber);
    QImage GetPageFirst();
    QImage GetPageLast();
    QImage GetPagePrevious();
    QImage GetPageNext();
    void PageRotate(float degree);
    void PageTranslate(float translateX, float translateY);
    void PageScale(float scaleX, float scaleY);
    void ResetMatrix(); // 重置为单位阵 【不旋转、不平移、不缩放】
    int PageNumberCurrent();
    int PageNumberTotal();

private:
    QImage GetPageImageByIndex(int index);
    QImage MakeQImageFromData(unsigned char* pImgData, int width, int height, QImage::Format colorFormat);

signals:
    void SignalErrorOccurred(MuPDF::ErrorCode errorCode, QString errorString);
};

#endif // MU_PDF_H
