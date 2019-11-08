#pragma once

#include <QMainWindow>

class FramelessWindowHelper;

namespace Ui
{
    class MiscellaneousTesting;
}

class MiscellaneousTesting : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MiscellaneousTesting   *ui;
    FramelessWindowHelper      *mpFramelessHelper;

public:
    explicit MiscellaneousTesting(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousTesting();

protected:
    virtual void closeEvent(QCloseEvent *event) override;

signals:
    void SignalClosed();
};
