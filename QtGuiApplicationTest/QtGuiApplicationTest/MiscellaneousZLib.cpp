#include "MiscellaneousZLib.h"

#include <QFileDialog>

#define CHUNK 16384
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <zlib/zlib.h>
#  include <fcntl.h>
#  include <io.h>
#  include <assert.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#include <LogUtil.h>

MiscellaneousZLib::MiscellaneousZLib(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousZLib::~MiscellaneousZLib()
{
}

QString MiscellaneousZLib::GetTitle()
{
    return QObject::tr("ZLib");
}

QString MiscellaneousZLib::GetTitleTooltip()
{
    return QObject::tr("How to use zlib make archive files.");
}

MiscellaneousTestGroup MiscellaneousZLib::GetGroupID()
{
    return MiscellaneousTestGroup::Others;
}

MiscellaneousTestItem MiscellaneousZLib::GetItemID()
{
    return MiscellaneousTestItem::Others_ZLib;
}

/* Compress from file source to file dest until EOF on source.
def() returns Z_OK on success, Z_MEM_ERROR if memory could not be
allocated for processing, Z_STREAM_ERROR if an invalid compression
level is supplied, Z_VERSION_ERROR if the version of zlib.h and the
version of the library linked do not match, or Z_ERRNO if there is
an error reading or writing the files. */
int MiscellaneousZLib::def(FILE *source, FILE *dest, int level)
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
        return ret;

    /* compress until end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
        }
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        /* run deflate() on input until output buffer not full, finish
        compression if all of source has been read in */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)deflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);     /* all input will be used */

                                        /* done when last data in file processed */
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);        /* stream will be complete */

                                        /* clean up and return */
    (void)deflateEnd(&strm);
    return Z_OK;
}

/* Decompress from file source to file dest until stream ends or EOF.
inf() returns Z_OK on success, Z_MEM_ERROR if memory could not be
allocated for processing, Z_DATA_ERROR if the deflate data is
invalid or incomplete, Z_VERSION_ERROR if the version of zlib.h and
the version of the library linked do not match, or Z_ERRNO if there
is an error reading or writing the files. */
int MiscellaneousZLib::inf(FILE *source, FILE *dest)
{
    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)inflateEnd(&strm);
            return Z_ERRNO;
        }
        if (strm.avail_in == 0)
            break;
        strm.next_in = in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)inflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

/* report a zlib or i/o error */
void MiscellaneousZLib::zerr(int ret)
{
    fputs("zpipe: ", stderr);
    switch (ret) {
    case Z_ERRNO:
        if (ferror(stdin))
        {
            fputs("error reading stdin\n", stderr);
            LogUtil::Error(CODE_LOCATION, "error reading stdin");
        }
        if (ferror(stdout))
        {
            fputs("error writing stdout\n", stderr);
            LogUtil::Error(CODE_LOCATION, "error writing stdin");
        }
        break;
    case Z_STREAM_ERROR:
        fputs("invalid compression level\n", stderr);
        LogUtil::Error(CODE_LOCATION, "invalid compression level");
        break;
    case Z_DATA_ERROR:
        fputs("invalid or incomplete deflate data\n", stderr);
        LogUtil::Error(CODE_LOCATION, "invalid or incomplete deflate data");
        break;
    case Z_MEM_ERROR:
        fputs("out of memory\n", stderr);
        LogUtil::Error(CODE_LOCATION, "out of memory");
        break;
    case Z_VERSION_ERROR:
        fputs("zlib version mismatch!\n", stderr);
        LogUtil::Error(CODE_LOCATION, "zlib version mismatch!");
        break;
    default:
        break;
    }
}

void MiscellaneousZLib::on_btnBrowseFiles_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open files"), qApp->applicationDirPath(), "Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml);;All(*)");
    ui.leFilenames->setText(files.join(";"));
}

void MiscellaneousZLib::on_btnBrowseDirectory_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), qApp->applicationDirPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui.leDirectory->setText(dir);
}

void MiscellaneousZLib::on_btnFilesDeflate_clicked()
{
    QString inputFilesStr = ui.leFilenames->text();
    if (inputFilesStr.isEmpty()) return;

    QStringList files = inputFilesStr.split(";", QString::SkipEmptyParts, Qt::CaseInsensitive);
    if (files.count() <= 0) return;

    FILE *pInFile = fopen(files[0].toUtf8().data(), "r");
    FILE *pOutFile = fopen("E:\\zlib_files_deflate_test.zip", "w");

    SET_BINARY_MODE(pInFile);
    SET_BINARY_MODE(pOutFile);

    int ret = def(pInFile, pOutFile, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK)
        zerr(ret);
    fclose(pInFile);
    fclose(pOutFile);
}

void MiscellaneousZLib::on_btnFilesInflate_clicked()
{
    QString inputFilesStr = ui.leFilenames->text();
    if (inputFilesStr.isEmpty()) return;

    QStringList files = inputFilesStr.split(";", QString::SkipEmptyParts, Qt::CaseInsensitive);
    if (files.count() <= 0) return;

    FILE *pInFile = fopen(files[0].toUtf8().data(), "r");
    FILE *pOutFile = fopen("E:\\zlib_files_deflate_test.jpg", "w");

    SET_BINARY_MODE(pInFile);
    SET_BINARY_MODE(pOutFile);

    int ret = inf(pInFile, pOutFile);
    if (ret != Z_OK)
        zerr(ret);

    fclose(pInFile);
    fclose(pOutFile);
}

void MiscellaneousZLib::on_btnDirectoryDeflate_clicked()
{
    int i = 0;
}

void MiscellaneousZLib::on_btnDirectoryInflate_clicked()
{
    int i = 0;
}
