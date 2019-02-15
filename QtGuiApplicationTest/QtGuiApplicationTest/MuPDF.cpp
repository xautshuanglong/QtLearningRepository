#include "MuPDF.h"

#include <QFile>
#include <QPainter>
#include <QPrinter>

#include <LogUtil.h>

/*
*    / a b 0 \
*    | c d 0 | normally represented as[a b c d e f].
*    \ e f 1 /
*/

MuPDF::MuPDF()
    : mPageSize(100, 100)
    , mIsOpened(false)
    , mPdfContext(NULL)
    , mPdfDocument(NULL)
    , mPageNumberTotal(0)
    , mPageNumberCurrent(0)
    , mViewPort{ 0.0f, 0.0f, 100.0f, 100.0f }            // 默认大小 100x100 没有偏移
{
    mCurPageImage = QImage(mPageSize, QImage::Format_RGB888);
    mCurPageImage.fill(Qt::gray);
    this->ResetMatrix();
}

MuPDF::MuPDF(QSize pageSize)
    : mIsOpened(false)
    , mPdfContext(NULL)
    , mPdfDocument(NULL)
    , mPageNumberTotal(0)
    , mPageNumberCurrent(0)
{
    mPageSize = pageSize;
    mViewPort = ::fz_make_rect(0.0f, 0.0f, pageSize.width(), pageSize.height());
    mCurPageImage = QImage(mPageSize, QImage::Format_RGB888);
    mCurPageImage.fill(Qt::gray);
    this->ResetMatrix();
}

MuPDF::~MuPDF()
{
}

bool MuPDF::Open(QString filename)
{
    if (mIsOpened)
    {
        this->Close();
    }

    mFilename = filename;
    if (!QFile::exists(mFilename))
    {
        emit SignalErrorOccurred(ECODE_NotFound, QString("Specified filename does not exist: %1").arg(mFilename));
        return false;
    }

    std::string inputString = mFilename.toStdString();
    const char *input = inputString.c_str();

    mPdfContext = ::fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    if (mPdfContext == NULL)
    {
        emit SignalErrorOccurred(ECODE_NewContextFailed, "fz_new_contex failed!");
        LogUtil::Error(CODE_LOCATION, "fz_new_contex failed!");
        return false;
    }
    fz_try(mPdfContext)
    {
        ::fz_register_document_handlers(mPdfContext);
        mPdfDocument = ::fz_open_document(mPdfContext, input);
        mPageNumberTotal = ::fz_count_pages(mPdfContext, mPdfDocument);
    }
    fz_catch(mPdfContext)
    {
        LogUtil::Debug(CODE_LOCATION, "MuPDF error: %s", ::fz_caught_message(mPdfContext));
        this->Close();
        return false;
    }

    mIsOpened = true;
    return true;
}

void MuPDF::Close()
{
    if (mPdfContext != NULL)
    {
        if (mPdfDocument != NULL)
        {
            ::fz_drop_document(mPdfContext, mPdfDocument);
            mPdfDocument = NULL;
        }
        ::fz_drop_context(mPdfContext);
        mPdfContext = NULL;
    }
    mIsOpened = false;
}

void MuPDF::Print()
{
    if (mIsOpened)
    {
        QPrinter::Margins margins = { 0.0f, 0.0f, 0.0f, 0.0f };
        QPrinter imagePrinter(QPrinter::HighResolution);
        imagePrinter.setPageSize(QPagedPaintDevice::A4);
        imagePrinter.setMargins(margins);
        imagePrinter.setPageMargins(0.0, 0.0, 0.0, 0.0, QPrinter::Millimeter);
        //imagePrinter.setFullPage(true);
        //imagePrinter.setOutputFormat(QPrinter::PdfFormat);
        //imagePrinter.setOutputFileName("E:/Temp/FopTest/MGI_ReportTest_image_print.pdf");

        int printerResolution = imagePrinter.resolution();

        QPainter imagePainter;
        imagePainter.begin(&imagePrinter);
        QRect painterRect = imagePainter.viewport();

        fz_try(mPdfContext)
        {
            fz_pixmap *pixmap = NULL;
            fz_rect pageBox;
            fz_matrix printMatrix;
            QSizeF pageSizeMM = imagePrinter.pageSizeMM();

            pageBox = ::fz_make_rect(0, 0, pageSizeMM.width(), pageSizeMM.height());
            printMatrix = ::fz_transform_page(pageBox, printerResolution, 0);

            unsigned char *pImageData = NULL;
            int pixmapWidth=0, pixmapHeight=0;

            for (int pageIndex = 0; pageIndex < mPageNumberTotal; ++pageIndex)
            {
                pixmap = ::fz_new_pixmap_from_page_number(mPdfContext, mPdfDocument, pageIndex, printMatrix, fz_device_rgb(mPdfContext), 0);
                pImageData = pixmap->samples;
                pixmapWidth = ::fz_pixmap_width(mPdfContext, pixmap);
                pixmapHeight = ::fz_pixmap_height(mPdfContext, pixmap);

                QImage image = this->MakeQImageFromData(pImageData, pixmapWidth, pixmapHeight, QImage::Format_RGB888);
                imagePainter.drawImage(painterRect, image);
                ::fz_drop_pixmap(mPdfContext, pixmap);
            }
        }
        fz_catch(mPdfContext)
        {
            LogUtil::Debug(CODE_LOCATION, "MuPDF error: %s", ::fz_caught_message(mPdfContext));
        }

        imagePainter.end();
    }
}

QImage MuPDF::GetPage(int pageNumber)
{
    mPageNumberCurrent = pageNumber;
    mCurPageImage = this->GetPageImageByIndex(mPageNumberCurrent);
    return mCurPageImage;
}

QImage MuPDF::PageFirst()
{
    mPageNumberCurrent = 0;
    mCurPageImage = this->GetPageImageByIndex(0);
    return mCurPageImage;
}

QImage MuPDF::PageLast()
{
    mPageNumberCurrent = mPageNumberTotal - 1;
    mCurPageImage = this->GetPageImageByIndex(mPageNumberCurrent);
    return mCurPageImage;
}

QImage MuPDF::PagePrevious()
{
    if (mPageNumberCurrent > 0)
    {
        --mPageNumberCurrent;
        mCurPageImage = this->GetPageImageByIndex(mPageNumberCurrent);
    }
    return mCurPageImage;
}

QImage MuPDF::PageNext()
{
    if (mPageNumberCurrent < mPageNumberTotal - 1)
    {
        ++mPageNumberCurrent;
        mCurPageImage = this->GetPageImageByIndex(mPageNumberCurrent);
    }
    return mCurPageImage;
}

void MuPDF::PageRotate(float degree)
{
    mMatrixRotate = ::fz_rotate(degree);
}

void MuPDF::PageTranslate(float translateX, float translateY)
{
    mMatrixTranslate = ::fz_translate(translateX, translateY);
}

void MuPDF::PageScale(float scaleX, float scaleY)
{
    mMatrixScale = ::fz_scale(scaleX, scaleY);
}

int MuPDF::PageNumberCurrent()
{
    return mPageNumberCurrent;
}

int MuPDF::PageNumberTotal()
{
    return mPageNumberTotal;
}

QImage MuPDF::GetPageImageByIndex(int index)
{
    if (mIsOpened)
    {
        QImage retValue;
        fz_try(mPdfContext)
        {
            mMatrixComposition = ::fz_concat(mMatrixScale, fz_concat(mMatrixRotate, mMatrixTranslate));
            fz_pixmap *pixmap = ::fz_new_pixmap_from_page_number(mPdfContext, mPdfDocument, index, mMatrixComposition, ::fz_device_rgb(mPdfContext), 0);

            unsigned char *samples = pixmap->samples;
            int width = ::fz_pixmap_width(mPdfContext, pixmap);
            int height = ::fz_pixmap_height(mPdfContext, pixmap);
            retValue = this->MakeQImageFromData(samples, width, height, QImage::Format_RGB888);
            ::fz_drop_pixmap(mPdfContext, pixmap);
        }
        fz_catch(mPdfContext)
        {
            LogUtil::Error(CODE_LOCATION, "MuPDF error: %s", ::fz_caught_message(mPdfContext));
        }
        if (!retValue.isNull())
        {
            return retValue;
        }
    }

    return mCurPageImage;
}

QImage MuPDF::MakeQImageFromData(unsigned char* pImgData, int width, int height, QImage::Format colorFormat)
{
    QImage retValue(width, height, QImage::Format_RGB888);
    unsigned char *pCurPixel;
    int colorR = 0, colorG = 0, colorB = 0;
    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            pCurPixel = pImgData + (row * width + col) * 3;
            colorR = *(pCurPixel + 0);
            colorG = *(pCurPixel + 1);
            colorB = *(pCurPixel + 2);
            retValue.setPixelColor(col, row, QColor(colorR, colorG, colorB));
        }
    }
    return retValue;
}

void MuPDF::ResetMatrix()
{
    mMatrixScale = ::fz_scale(1, 1);
    mMatrixRotate = ::fz_rotate(0);
    mMatrixTranslate = ::fz_translate(0, 0);
    mMatrixComposition = ::fz_concat(mMatrixScale, ::fz_concat(mMatrixRotate, mMatrixTranslate));
}
