#pragma once

#include <QString>

class UrlUtil
{
private:
    static QString applicationPath;

public:
    UrlUtil() = delete;
    ~UrlUtil() = delete;

    static QString GetApplicationPath();
    static QString GetAppIcon(int appID);
    static QString GetConfigPath();
};

