#include "DicomWindow.h"

#include "FramelessWindowHelper.h"

DicomWindow::DicomWindow(QWidget *parent /* = Q_NULLPTR */)
    : QMainWindow(parent)
    , mpFramelessWindow(Q_NULLPTR)
{
    ui.setupUi(this);

    QIcon winIcon(":/AppImages/Resources/images/XRay_DICOM.ico");
    this->setWindowIcon(winIcon);

    mpFramelessWindow = new FramelessWindowHelper(this);
}

DicomWindow::~DicomWindow()
{
    int i = 0;
}

void DicomWindow::closeEvent(QCloseEvent *event)
{
    emit SignalClosed();
}
