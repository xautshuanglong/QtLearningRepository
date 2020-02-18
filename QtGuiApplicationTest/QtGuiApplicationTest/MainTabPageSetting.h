#pragma once

#include <QWidget>
#include "ui_MainTabPageSetting.h"

class MainTabPageSetting : public QWidget
{
    Q_OBJECT

private:
    Ui::MainTabPageSetting ui;

private slots:
    void SlotListVScrollBarRangeChanged(int min, int max);
    void SlotListVScrollBarValueChanged(int value);
    void SlotDropMenuActionTriggeredAdd();
    void SlotDropMenuActionTriggeredDelete();
    void on_btnGeneratePDF_clicked();
    void on_btnParseDocument_clicked();
    void on_btnChangeItemCount_clicked();
    void on_btnShowVerticalScrollBar_clicked();
    void on_btnDropMenu_clicked();
    void on_btnDropMenu_customContextMenuRequested(const QPoint &pos);

public:
    MainTabPageSetting(QWidget *parent = Q_NULLPTR);
    ~MainTabPageSetting();
};
