#pragma once

#include <QMessageBox>
#include "ui_JcbMessageBox.h"

class JcbMessageBox : public QMessageBox
{
    Q_OBJECT

public:
    JcbMessageBox(QWidget *parent = Q_NULLPTR);
    ~JcbMessageBox();

private:
    Ui::JcbMessageBox ui;
};
