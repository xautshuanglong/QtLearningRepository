﻿#include "DebugPanel.h"
#include "ui_DebugPanel.h"

#include <QKeyEvent>
#include <QMetaEnum>

#include <LogUtil.h>

DebugPanel::DebugPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DebugPanel)
{
    ui->setupUi(this);

    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
}

DebugPanel::~DebugPanel()
{
    delete ui;
}

void DebugPanel::closeEvent(QCloseEvent *event)
{
    QWidget::close();
}

void DebugPanel::keyPressEvent(QKeyEvent *event)
{
    //int keyValue = event->key();
    //Qt::KeyboardModifiers modifiers = event->modifiers();
    //QMetaEnum KeyboardModifierEnum = QMetaEnum::fromType<Qt::KeyboardModifiers>();
    //QByteArray modifierArray = KeyboardModifierEnum.valueToKeys(modifiers);
    //LogUtil::Debug(CODE_LOCATION, "%s + %d", modifierArray.toStdString().c_str(), keyValue);
    
    if (QKeySequence("Ctrl+Shift+D")== QKeySequence(event->modifiers() | event->key()))
    {
        this->ShowOrHideDebugPanel();
    }
}

void DebugPanel::keyReleaseEvent(QKeyEvent *event)
{
    int keyValue = event->key();
    Qt::KeyboardModifiers modifiers = event->modifiers();
    QMetaEnum KeyboardModifierEnum = QMetaEnum::fromType<Qt::KeyboardModifiers>();
    QByteArray modifierArray = KeyboardModifierEnum.valueToKeys(modifiers);
    //LogUtil::Debug(CODE_LOCATION, "%s + %d", modifierArray.toStdString().c_str(), keyValue);
}

void DebugPanel::on_pushButton_clicked()
{
    static int count = 0;
    ui->label->setText(QString("Test %1").arg(++count));
}

void DebugPanel::ShowOrHideDebugPanel()
{
    if (this->isVisible())
    {
        this->hide();
    }
}
