#include "JcbMessageBox.h"

JcbMessageBox::JcbMessageBox(QWidget *parent)
    : QMessageBox(parent)
{
    ui.setupUi(this);
}

JcbMessageBox::~JcbMessageBox()
{
}
