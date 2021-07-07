#include "DicomWindow.h"

// QT Headers
#include <QAction>

#include <LogUtil.h>
#include <DicomEnv.h>

#include "Core/FramelessWindowHelper.h"
#include "DICOM/DicomServerBrowserWidget.h"
#include "DICOM/DicomDownloadWigdet.h"
#include "Debugger/DebugPanel.h"

DicomWindow::DicomWindow(QWidget *parent /* = Q_NULLPTR */)
    : QMainWindow(parent)
    , mPointerDcmServerBroswer(new DicomServerBrowserWidget)
    , mPointerDcmDownload(new DicomDownloadWigdet)
{
    ui.setupUi(this);

    QIcon winIcon(":/AppImages/Resources/images/dicom_xray.ico");
    this->setWindowIcon(winIcon);
    mpFramelessWindow = new FramelessWindowHelper(this);

    //mpDicomServerBroswer->setVisible(false);
    //mpDicomServerBroswer->setStyleSheet("background-color: green;");

    this->InitToolBar();
    DebugPanel::GetInstance()->ListenKeyboard(this); // ���ڴ򿪵������
    DicomEnv::Initialize(LogUtil::DefaultLogHandler);
    DicomEnv::SetPrivateKeyPasswd("mgius");
    DicomEnv::SetPrivateKeyFile(".\\certs\\dicom_client_pw.key");
    DicomEnv::SetCertificateFile(".\\certs\\dicom_client_pw.crt");
    DicomEnv::AddTrustedCertificateFile(".\\certs\\RootCA.crt");
}

DicomWindow::~DicomWindow()
{
    DicomServerBrowserWidget *pServerBrowserWidget = mPointerDcmServerBroswer.data();
    delete pServerBrowserWidget;
    mPointerDcmServerBroswer.clear();

    DicomDownloadWigdet *pTempDownloadWidget = mPointerDcmDownload.data();
    delete pTempDownloadWidget;
    mPointerDcmDownload.clear();

    DicomEnv::Uninitialize();
}

void DicomWindow::InitToolBar()
{
    mpToolBar = new QToolBar(this);
    ui.VLayoutTitleBar->addWidget(mpToolBar);

    QAction *pActionOpen = new QAction(QIcon(":/AppImages/Resources/images/open.png"), "Open", mpToolBar);            // �򿪱���ͼ��
    QAction *pActionSave = new QAction(QIcon(":/AppImages/Resources/images/save.png"), "Save", mpToolBar);            // ���浱ǰͼ��
    QAction *pActionPull = new QAction(QIcon(":/AppImages/Resources/images/dicom_download.png"), "Pull", mpToolBar);  // �����ƶ�ͼ��
    mpToolBar->addAction(pActionOpen);
    mpToolBar->addAction(pActionSave);
    mpToolBar->addAction(pActionPull);
    this->connect(pActionOpen, SIGNAL(triggered()), SLOT(on_action_dicom_open()));
    this->connect(pActionSave, SIGNAL(triggered()), SLOT(on_action_dicom_save()));
    this->connect(pActionPull, SIGNAL(triggered()), SLOT(on_action_dicom_pull()));
}

void DicomWindow::closeEvent(QCloseEvent *event)
{
    mPointerDcmServerBroswer->close();
    mPointerDcmDownload->close();
    emit SignalClosed();
}

void DicomWindow::resizeEvent(QResizeEvent *event)
{
}

void DicomWindow::on_action_dicom_open()
{
    LogUtil::Debug(CODE_LOCATION, "Dicom open ...");
}

void DicomWindow::on_action_dicom_save()
{
    LogUtil::Debug(CODE_LOCATION, "Dicom save ...");
}

void DicomWindow::on_action_dicom_pull()
{
    //LogUtil::Debug(CODE_LOCATION, "Dicom pull ...");
    mPointerDcmServerBroswer->show();
}