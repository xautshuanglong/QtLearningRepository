#include "AppListItemWidget.h"
#include "ui_AppListItemWidget.h"

// QT Headers
#include <QMouseEvent>
#include <QPropertyAnimation>

#include <LogUtil.h>

AppListItemWidget::AppListItemWidget(QWidget *parent)
    : QWidget(parent)
{
    ui = new Ui::AppListItemWidget();
    ui->setupUi(this);
    ui->lbAppName->setText(tr("AppNameTest"));
    ui->maskWidget->setStyleSheet("background-color: lightblue;");

    mpAnimateEnter = new QPropertyAnimation(ui->maskWidget, "pos", this);
    mpAnimateLeave = new QPropertyAnimation(ui->maskWidget, "pos", this);
}

AppListItemWidget::~AppListItemWidget()
{
    delete ui;
}

void AppListItemWidget::SetIconPath(const QString &appIconPath)
{
    ui->lbAppIcon->setPixmap(QPixmap(appIconPath));
}

void AppListItemWidget::SetAppName(const QString &appName)
{
    ui->lbAppName->setText(appName);
}

void AppListItemWidget::SetAppDescription(const QString &appDescription)
{
    ui->lbAppDescription->setText(appDescription);
}

void AppListItemWidget::mousePressEvent(QMouseEvent *event)
{
    LogUtil::Debug(CODE_LOCATION, "mouse pressed");
    event->ignore();
}

void AppListItemWidget::mouseReleaseEvent(QMouseEvent *event)
{
    LogUtil::Debug(CODE_LOCATION, "mouse released");
    event->ignore();
}

void AppListItemWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    LogUtil::Debug(CODE_LOCATION, "mouse double clicked");
    event->ignore();
}

void AppListItemWidget::enterEvent(QEvent *event)
{
    mpAnimateLeave->stop();
    mpAnimateEnter->start();
    event->ignore();
}

void AppListItemWidget::leaveEvent(QEvent *event)
{
    mpAnimateEnter->stop();
    mpAnimateLeave->start();
    event->ignore();
}

void AppListItemWidget::resizeEvent(QResizeEvent *event)
{
    int hideY = -1 * event->size().height();
    ui->maskWidget->resize(event->size());
    ui->maskWidget->move(0, hideY);

    mpAnimateEnter->setDuration(300);
    mpAnimateEnter->setStartValue(QPoint(0, hideY));
    mpAnimateEnter->setEndValue(QPoint(0, 0));
    mpAnimateLeave->setDuration(300);
    mpAnimateLeave->setStartValue(QPoint(0, 0));
    mpAnimateLeave->setEndValue(QPoint(0, hideY));
}
