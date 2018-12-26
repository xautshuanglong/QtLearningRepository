#pragma once

#include <QWidget>

class TitleBarWidget;

namespace Ui
{
    class DicomServerBrowserWidget;
};

class DicomServerBrowserWidget : public QWidget
{
    Q_OBJECT

public:
    DicomServerBrowserWidget(QWidget *parent = Q_NULLPTR);
    ~DicomServerBrowserWidget();

private slots:
    void on_btnEchoTest_clicked();
    void on_btnFindTest_clicked();
    void on_btnGetTest_clicked();
    void on_btnMoveTest_clicked();
    void on_btnStoreTest_clicked();

private:
    Ui::DicomServerBrowserWidget *ui;
    TitleBarWidget               *mpTitleBarWidget;
};
