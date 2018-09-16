#include "MyApplication.h"

#include <QFile>
#include <QIcon>

MyApplication::MyApplication(int &argc, char **argv, int appFlag /* = ApplicationFlags */)
    : QApplication(argc, argv, appFlag)
{
    QFile mainTabStyle(":/QtGuiApplicationTest/Resources/qss/white.qss");
    //QFile mainTabStyle(":/QtGuiApplicationTest/Resources/qss/dark_orange.qss");
    //QFile mainTabStyle(":/QtGuiApplicationTest/Resources/qss/black.qss");
    if (mainTabStyle.open(QFile::ReadOnly))
    {
        QString styleSheet = QLatin1String(mainTabStyle.readAll());
        this->setStyleSheet(styleSheet);
        mainTabStyle.close();
    }
    else
    {
        QString error = mainTabStyle.errorString();
    }

    QIcon winIcon(":/AppImages/Resources/images/app.ico");
    this->setWindowIcon(winIcon);
}

MyApplication::~MyApplication()
{
}
