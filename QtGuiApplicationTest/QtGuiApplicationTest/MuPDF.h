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
    fz_matrix    mTranslateMatrix;    // ��¼ת������ת�����š�ƽ�Ƶȣ����������ڴ˻������ۼ�

public:
    MuPDF();
    MuPDF(QSize pageSize);
    ~MuPDF();

    bool Open(QString filename);
    void Close();
    void Print();
    QImage GetPage(int pageNumber);
    QImage PageFirst();
    QImage PageLast();
    QImage PagePrevious();
    QImage PageNext();
    QImage PageRotate(float degree);
    QImage PageTranslate(float translateX, float translateY);
    QImage PageScale(float scaleX, float scaleY);
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
