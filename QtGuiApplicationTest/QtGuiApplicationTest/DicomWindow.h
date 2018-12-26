#ifndef DICOM_WINDOW_H
#define DICOM_WINDOW_H

#include <QMainWindow>
#include "ui_DicomWindow.h"

class FramelessWindowHelper;
class QToolBar;

class DicomWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::DicomWindow              ui;
    FramelessWindowHelper       *mpFramelessWindow;

public:
    explicit DicomWindow(QWidget *parent = Q_NULLPTR);
    ~DicomWindow();

private:
    void InitToolBar();

protected:
    virtual void closeEvent(QCloseEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

signals:
    void SignalClosed();

private slots:
    void on_action_dicom_open();
    void on_action_dicom_save();
    void on_action_dicom_pull();

private:
    QToolBar             *mpToolBar;
};

#endif // DICOM_WINDOW_H
