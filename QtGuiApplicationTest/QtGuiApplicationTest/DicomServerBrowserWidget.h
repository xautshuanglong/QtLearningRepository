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

private:
    Ui::DicomServerBrowserWidget *ui;
    TitleBarWidget               *mpTitleBarWidget;
};
