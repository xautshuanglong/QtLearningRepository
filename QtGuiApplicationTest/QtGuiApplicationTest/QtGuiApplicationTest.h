#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtGuiApplicationTest.h"

class QtGuiApplicationTest : public QMainWindow
{
    Q_OBJECT

public:
    QtGuiApplicationTest(QWidget *parent = Q_NULLPTR);
    ~QtGuiApplicationTest();

private:
    Ui::QtGuiApplicationTestClass ui;

public:
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
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void closeEvent(QCloseEvent *event) override;
};
