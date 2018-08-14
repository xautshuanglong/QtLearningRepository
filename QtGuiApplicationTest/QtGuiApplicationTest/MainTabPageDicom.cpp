#include "MainTabPageDicom.h"

#include <QSplitter>
#include <QLabel>

MainTabPageDicom::MainTabPageDicom(QWidget *parent /* = Q_NULLPTR */)
    : QWidget(parent)
{
    ui.setupUi(this);
    this->setStyleSheet("background: green;");
}

MainTabPageDicom::~MainTabPageDicom()
{
}
