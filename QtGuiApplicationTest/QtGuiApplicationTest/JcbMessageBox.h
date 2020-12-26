#pragma once

#include <QMessageBox>

class JcbMessageBox : public QMessageBox
{
    Q_OBJECT

public:
    JcbMessageBox(QWidget *parent = Q_NULLPTR);
    ~JcbMessageBox();
};
