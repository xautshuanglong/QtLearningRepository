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
    int          mPageNumberTotal;
    int          mPageNumberCurrent;
    bool         mIsOpened;
    fz_context  *mPdfContext;
    fz_document *mPdfDocument;

public:
    MuPDF();
    ~MuPDF();

    bool Open(QString filename);
    void Close();
    QImage GetPage(int pageNumber);
    QImage PageFirst();
    QImage PageLast();
    QImage PagePrevious();
    QImage PageNext();
    int PageNumberCurrent();
    int PageNumberTotal();

signals:
    void SignalErrorOccurred(MuPDF::ErrorCode errorCode, QString errorString);
};

#endif // MU_PDF_H
