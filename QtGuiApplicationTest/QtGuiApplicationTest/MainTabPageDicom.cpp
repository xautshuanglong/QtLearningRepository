#include "MainTabPageDicom.h"

#include <QSplitter>
#include <QLabel>

MainTabPageDicom::MainTabPageDicom(QWidget *parent /* = Q_NULLPTR */)
    : QWidget(parent)
{
    ui.setupUi(this);
    //this->setStyleSheet("background: green;");

    ui.dockDcmTagContents->setStyleSheet("background:green;");
    ui.tableWidgetDcmTag->setStyleSheet("background:red;");
}

MainTabPageDicom::~MainTabPageDicom()
{
}

void MainTabPageDicom::on_testPushButton_clicked()
{
    if (!ui.dockDcmTag->isVisible())
    {
        ui.dockDcmTag->show();
    }
}
