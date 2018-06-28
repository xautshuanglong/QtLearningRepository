#include <QCoreApplication>

#include <QDebug>
#include <QThread>

#include <LogUtil.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
#ifdef _DEBUG
    LogUtil::Init(LOG_LEVEL_DEBUG);
#elif
    MGI::LogUtil::Init(MGI::LOG_LEVEL_INFO);
#endif
    LogUtil::Debug(CODE_LOCATION, "%s -> %d", "number", 123);

    int count = 0;
    while (count < 1000000)
    {
        LogUtil::Debug(CODE_LOCATION, "count = %d", count);
        ++count;
    }

    return a.exec();
}
