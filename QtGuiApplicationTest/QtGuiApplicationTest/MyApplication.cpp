#include "MyApplication.h"

#include <QFile>

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
}

MyApplication::~MyApplication()
{
}
