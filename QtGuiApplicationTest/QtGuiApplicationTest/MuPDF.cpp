#include "MuPDF.h"

#include <QFile>

#include <LogUtil.h>

/*
*    / a b 0 \
*    | c d 0 | normally represented as[a b c d e f].
*    \ e f 1 /
*/

MuPDF::MuPDF()
    : mPageSize(380, 540)
    , mIsOpened(false)
    , mPdfContext(NULL)
    , mPdfDocument(NULL)
    , mPageNumberTotal(0)
    , mPageNumberCurrent(0)
    , mViewPort{ 0.0f, 0.0f, 380.0f, 540.0f }            // 默认大小 100x100 没有偏移
    , mTranslateMatrix{ 1.0f,0.0f,0.0f,1.0f,0.0f,0.0f }  // 单位阵 【不旋转、不平移、不缩放】
{
    mCurPageImage = QImage(mPageSize, QImage::Format_RGB888);
    mCurPageImage.fill(Qt::gray);
}

MuPDF::MuPDF(QSize pageSize)
    : mIsOpened(false)
    , mPdfContext(NULL)
    , mPdfDocument(NULL)
    , mPageNumberTotal(0)
    , mPageNumberCurrent(0)
    , mTranslateMatrix{ 1.0f,0.0f,0.0f,1.0f,0.0f,0.0f }  // 单位阵 【不旋转、不平移、不缩放】
{
    mPageSize = pageSize;
    mViewPort = ::fz_make_rect(0.0f, 0.0f, pageSize.width(), pageSize.height());
    mCurPageImage = QImage(mPageSize, QImage::Format_RGB888);
    mCurPageImage.fill(Qt::gray);
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
    ;
}

QImage MuPDF::GetPage(int pageNumber)
{
    QImage retVlue(mPageSize, QImage::Format_RGB888);
    return retVlue;
}

QImage MuPDF::PageFirst()
{
    mPageNumberCurrent = 0;
    return this->GetPageImageByIndex(0);
}

QImage MuPDF::PageLast()
{
    mPageNumberCurrent = mPageNumberTotal - 1;
    return this->GetPageImageByIndex(mPageNumberCurrent);;
}

QImage MuPDF::PagePrevious()
{
    return mCurPageImage;
}

QImage MuPDF::PageNext()
{
    return mCurPageImage;
}

QImage MuPDF::PageRotate(float degree)
{
    return mCurPageImage;
}

QImage MuPDF::PageTranslate(float translateX, float translateY)
{
    return mCurPageImage;
}

QImage MuPDF::PageScale(float scaleX, float scaleY)
{
    QImage retValue(100, 100, QImage::Format_RGB888);
    return retValue;
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
        fz_pixmap *pixmap = NULL;

        //pageBox = ::fz_make_rect(0, 0, 100, 100);
        //mTranslateMatrix = ::fz_transform_page(mViewPort, 96, 0);
        //fz_matrix newMatrix = ::fz_post_scale(mTranslateMatrix, 0.62f, 0.62f);
        //::fz_pre_rotate(ctm, rotate);
        pixmap = ::fz_new_pixmap_from_page_number(mPdfContext, mPdfDocument, mPageNumberCurrent, mTranslateMatrix, ::fz_device_rgb(mPdfContext), 0);

        unsigned char *samples = pixmap->samples;
        int width = ::fz_pixmap_width(mPdfContext, pixmap);
        int height = ::fz_pixmap_height(mPdfContext, pixmap);
        QImage retVlue(samples, width, height, pixmap->stride, QImage::Format_RGB888);

        return retVlue;
    }

    return mCurPageImage;
}

void MuPDF::ResetMatrix()
{
    mTranslateMatrix = ::fz_make_matrix(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
}
