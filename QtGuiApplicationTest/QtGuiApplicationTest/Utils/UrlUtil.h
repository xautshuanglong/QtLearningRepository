#pragma once

#include <QString>

namespace Shuanglong::Utils
{
    class UrlUtil
    {
    private:
        static QString applicationPath;

    public:
        UrlUtil() = delete;
        ~UrlUtil() = delete;

        static QString GetApplicationPath();
        static QString GetAppIconPath(int appID);
        static QString GetConfigPath();
    };
}

