#include "MainTabPageFirst.h"

MainTabPageFirst::MainTabPageFirst(QWidget *parent /* = Q_NULLPTR */)
    : QWidget(parent)
{
    ui.setupUi(this);

    ui.imgTitleValue->setText("image_filename_test");
}

MainTabPageFirst::~MainTabPageFirst()
{
}
