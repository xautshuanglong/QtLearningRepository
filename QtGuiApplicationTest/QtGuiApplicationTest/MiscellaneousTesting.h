#pragma once

#include <QMainWindow>

class FramelessWindowHelper;

namespace Ui
{
    class MiscellaneousTesting;
}

class MiscellaneousTesting : public QMainWindow
{
private:
    Ui::MiscellaneousTesting   *ui;
    FramelessWindowHelper      *mpFramelessHelper;

public:
    explicit MiscellaneousTesting(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousTesting();
};
