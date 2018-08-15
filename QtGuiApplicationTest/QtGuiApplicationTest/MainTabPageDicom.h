#ifndef MAIN_TAB_PAGE_DICOM_H
#define MAIN_TAB_PAGE_DICOM_H

#include "qwidget.h"
#include "ui_MainTabPageDicom.h"

class QSplitter;

class MainTabPageDicom : public QWidget
{
    Q_OBJECT

private:
    Ui::MainTabPageDicom ui;
    QSplitter           *mpMainSpliter;

public:
    explicit MainTabPageDicom(QWidget *parent = Q_NULLPTR);
    ~MainTabPageDicom();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_action_open();
    void on_action_save();

private:
    void ConvertDicomToQImage(QString &dcmFilename, QImage **ppOutImage);
};

#endif MAIN_TAB_PAGE_DICOM_H
