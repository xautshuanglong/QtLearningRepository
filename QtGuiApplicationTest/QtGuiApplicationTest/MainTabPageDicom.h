#ifndef MAIN_TAB_PAGE_DICOM_H
#define MAIN_TAB_PAGE_DICOM_H

#include "qwidget.h"
#include "ui_MainTabPageDicom.h"

class MainTabPageDicom : public QWidget
{
private:
    Ui::MainTabPageDicom ui;
public:
    explicit MainTabPageDicom(QWidget *parent = Q_NULLPTR);
    ~MainTabPageDicom();
};

#endif MAIN_TAB_PAGE_DICOM_H
