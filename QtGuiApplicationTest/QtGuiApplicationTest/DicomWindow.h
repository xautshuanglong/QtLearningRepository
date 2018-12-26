#ifndef DICOM_WINDOW_H
#define DICOM_WINDOW_H

#include <QMainWindow>
#include "ui_DicomWindow.h"

class FramelessWindowHelper;

class DicomWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::DicomWindow              ui;
    FramelessWindowHelper       *mpFramelessWindow;

public:
    explicit DicomWindow(QWidget *parent = Q_NULLPTR);
    ~DicomWindow();

protected:
    virtual void closeEvent(QCloseEvent *event) override;

signals:
    void SignalClosed();
};

#endif // DICOM_WINDOW_H
