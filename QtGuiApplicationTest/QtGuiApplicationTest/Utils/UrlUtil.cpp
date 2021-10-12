#include "UrlUtil.h"

#include <QApplication>

namespace Shuanglong::Utils
{
    QString UrlUtil::applicationPath = "";

    QString UrlUtil::GetApplicationPath()
    {
        if (applicationPath.isEmpty())
        {
            applicationPath = QApplication::applicationDirPath();
        }
        return applicationPath;
    }

    QString UrlUtil::GetAppIconPath(int appID)
    {
        return GetApplicationPath() + QString("/resource/icon/%1.png").arg(appID);
    }

    QString UrlUtil::GetConfigPath()
    {
        return GetApplicationPath().append("/config");
    }
}
