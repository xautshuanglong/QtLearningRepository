#pragma once

#include <QtWidgets/QMainWindow>
#include <QSystemTrayIcon>
#include "ui_QtGuiApplicationTest.h"

class FramelessWindowHelper;
class MainTabPageFirst;
class MainTabPageDicom;
class MainTabPageSetting;
class QSystemTrayIcon;

class MyMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MyMainWindow(QWidget *parent = Q_NULLPTR);
    ~MyMainWindow();

private:
    Ui::QtGuiApplicationTestClass    ui;
    FramelessWindowHelper           *mpFramelessWindow;
    MainTabPageFirst                *mpPageFirst;
    MainTabPageDicom                *mpPageDicom;
    MainTabPageSetting              *mpPageSetting;
    QSystemTrayIcon                 *mpSystemTray;

protected:
    virtual bool event(QEvent *event) override;

    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;

    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

    virtual void paintEvent(QPaintEvent *event) override;
    virtual void moveEvent(QMoveEvent *event) override;
    virtual void closeEvent(QCloseEvent *event) override;

    virtual void resizeEvent(QResizeEvent *event) override;

private slots:
    void  on_systemTrayIconActivated(QSystemTrayIcon::ActivationReason reason);
};
