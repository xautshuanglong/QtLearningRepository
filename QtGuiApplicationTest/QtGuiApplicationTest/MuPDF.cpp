#include "MuPDF.h"

#include <QFile>

#include <LogUtil.h>

MuPDF::MuPDF()
    : mPageSize(0, 0)
    , mIsOpened(false)
    , mPdfContext(NULL)
    , mPdfDocument(NULL)
{
}

MuPDF::~MuPDF()
{
}

bool MuPDF::Open(QString filename)
{
    mFilename = filename;
    if (!QFile::exists(mFilename))
    {
        emit SignalErrorOccurred(ECODE_NotFound, QString("Specified filename does not exist: %1").arg(mFilename));
        return false;
    }

    if (mIsOpened)
    {
        this->Close();
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

QImage MuPDF::GetPage(int pageNumber)
{
    QImage retVlue(mPageSize, QImage::Format_RGB888);
    return retVlue;
}

QImage MuPDF::PageFirst()
{
    if (mIsOpened)
    {
        float zoom, rotate;
        int page_number;
        fz_pixmap *pixmap = NULL;
        fz_rect pageBox;
        fz_matrix ctm;

        page_number = 0;
        zoom = 100;
        rotate = 0;

        pageBox = ::fz_make_rect(0, 0, 100, 100);
        ctm = ::fz_transform_page(pageBox, 96, 0);
        ::fz_pre_rotate(ctm, rotate);
        pixmap = ::fz_new_pixmap_from_page_number(mPdfContext, mPdfDocument, page_number, ctm, ::fz_device_rgb(mPdfContext), 0);

        unsigned char *samples = pixmap->samples;
        int width = ::fz_pixmap_width(mPdfContext, pixmap);
        int height = ::fz_pixmap_height(mPdfContext, pixmap);
        QImage retVlue(samples, width, height, pixmap->stride, QImage::Format_RGB888);

        return retVlue;
    }
    
    return QImage();
}

QImage MuPDF::PageLast()
{
    QImage retVlue(mPageSize, QImage::Format_RGB888);
    return retVlue;
}

QImage MuPDF::PagePrevious()
{
    QImage retVlue(mPageSize, QImage::Format_RGB888);
    return retVlue;
}

QImage MuPDF::PageNext()
{
    QImage retVlue(mPageSize, QImage::Format_RGB888);
    return retVlue;
}

int MuPDF::PageNumberCurrent()
{
    return mPageNumberCurrent;
}

int MuPDF::PageNumberTotal()
{
    return mPageNumberTotal;
}
