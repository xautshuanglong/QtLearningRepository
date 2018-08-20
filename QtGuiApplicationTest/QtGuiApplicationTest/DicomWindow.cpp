#include "DicomWindow.h"

#include "FramelessWindowHelper.h"

DicomWindow::DicomWindow(QWidget *parent /* = Q_NULLPTR */)
    : QMainWindow(parent)
    , mpFramelessWindow(Q_NULLPTR)
{
    ui.setupUi(this);

    QIcon winIcon("test.ico");
    this->setWindowIcon(winIcon);

    mpFramelessWindow = new FramelessWindowHelper(this);
}

DicomWindow::~DicomWindow()
{
}
