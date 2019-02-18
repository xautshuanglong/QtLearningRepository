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
    int          mPageNumberTotal;    // PDF �ĵ���ҳ��
    int          mPageNumberCurrent;  // PDF �ĵ���ǰ���ҳ���
    bool         mIsOpened;           // ָʾ PDF �ĵ��Ƿ�Ϊ��״̬
    fz_context  *mPdfContext;         // MuPDF ������
    fz_document *mPdfDocument;        // MuPDF �ĵ�
    fz_rect      mViewPort;           // �ӿڴ�С��Ԥ���ؼ���С
    fz_matrix    mMatrixComposition;  // ����ת���������š���ת��ƽ�Ƶ���Ϻ�ı任Ч����
    fz_matrix    mMatrixScale;        // ���ű任
    fz_matrix    mMatrixRotate;       // ��ת�任
    fz_matrix    mMatrixTranslate;    // ƽ�Ʊ任

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
    void ResetMatrix(); // ����Ϊ��λ�� ������ת����ƽ�ơ������š�
    int PageNumberCurrent();
    int PageNumberTotal();

private:
    QImage GetPageImageByIndex(int index);
    QImage MakeQImageFromData(unsigned char* pImgData, int width, int height, QImage::Format colorFormat);

signals:
    void SignalErrorOccurred(MuPDF::ErrorCode errorCode, QString errorString);
};

#endif // MU_PDF_H
