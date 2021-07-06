#include "MiscellaneousImageQPixmap.h"
#include "ui_MiscellaneousImageQPixmap.h"

#include <QMenu>
#include <QSignalMapper>

MiscellaneousImageQPixmap::MiscellaneousImageQPixmap(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousImageQPixmap())
{
    ui->setupUi(this);
    ui->stackedWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    // Custom Context Menu
    this->InitCustomContextMenu();
}

MiscellaneousImageQPixmap::~MiscellaneousImageQPixmap()
{
    delete ui;
}

QString MiscellaneousImageQPixmap::GetTitle()
{
    return QObject::tr("Image QPixmap");
}

QString MiscellaneousImageQPixmap::GetTitleTooltip()
{
    return QObject::tr("QPixmap operations: copy assign and others.");
}

MiscellaneousTestGroup MiscellaneousImageQPixmap::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousImageQPixmap::GetItemID()
{
    return MiscellaneousTestItem::QT_Image_QPixmap;
}

void MiscellaneousImageQPixmap::InitCustomContextMenu()
{
    QMenu *pOpenOption = new QMenu("Open", this);
    pOpenOption->addAction("YUV File", this, SLOT(SlotStackedWidgetYuvFile()));
    pOpenOption->addAction("YUV Stream", this, SLOT(SlotStackedWidgetYuvStream()));
    pOpenOption->addAction("YUV Shared Memory", this, SLOT(SlotStackedWidgetYuvSharedMemory()));
    pOpenOption->setWindowFlags(pOpenOption->windowFlags() | Qt::NoDropShadowWindowHint);

    // Ò³ÃæÇÐ»»Ó³Éä
    QSignalMapper *pSignalMapper = new QSignalMapper(this);
    this->connect(pSignalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(SlotStackedWidgetSwitchPage(QWidget*)));
    QMenu *pStackSwitch = new QMenu("Switch Page", this);
    QAction *pIamgePixmap = pStackSwitch->addAction("Pixmap", pSignalMapper, SLOT(map()));
    QAction *pIamgeOpenGL = pStackSwitch->addAction("OpenGL", pSignalMapper, SLOT(map()));
    QAction *pIamgeGraphicsView = pStackSwitch->addAction("Graphics View", pSignalMapper, SLOT(map()));
    pSignalMapper->setMapping(pIamgePixmap, ui->imgPixmap);
    pSignalMapper->setMapping(pIamgeOpenGL, ui->imgOpenGL);
    pSignalMapper->setMapping(pIamgeGraphicsView, ui->imgGraphicsView);
    pStackSwitch->setWindowFlags(pStackSwitch->windowFlags() | Qt::NoDropShadowWindowHint);

    mpCustomContext = new QMenu("Test", this);
    mpCustomContext->addMenu(pOpenOption);
    mpCustomContext->addMenu(pStackSwitch);
    mpCustomContext->addAction("properties", this, SLOT(SlotStackedWidgetProperties()));
    mpCustomContext->setWindowFlags(mpCustomContext->windowFlags() | Qt::NoDropShadowWindowHint);
}

void MiscellaneousImageQPixmap::SlotStackedWidgetYuvFile()
{
    int i = 0;
}

void MiscellaneousImageQPixmap::SlotStackedWidgetYuvStream()
{
    int i = 0;
}

void MiscellaneousImageQPixmap::SlotStackedWidgetYuvSharedMemory()
{
    int i = 0;
}

void MiscellaneousImageQPixmap::SlotStackedWidgetSwitchPage(QWidget *pCurWidget)
{
    int i = 0;
    ui->stackedWidget->setCurrentWidget(pCurWidget);
}

void MiscellaneousImageQPixmap::SlotStackedWidgetProperties()
{
    int i = 0;
}

void MiscellaneousImageQPixmap::on_stackedWidget_customContextMenuRequested(const QPoint &position)
{
    mpCustomContext->popup(ui->stackedWidget->mapToGlobal(position));
}

void MiscellaneousImageQPixmap::on_stackedWidget_currentChanged(int index)
{
    int i = 0;
}
