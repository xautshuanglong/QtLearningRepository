#include "AppListItemWidget.h"
#include "ui_AppListItemWidget.h"

// QT Headers
#include <QMouseEvent>

#include <LogUtil.h>

AppListItemWidget::AppListItemWidget(QWidget *parent)
    : QWidget(parent)
{
    ui = new Ui::AppListItemWidget();
    ui->setupUi(this);
    ui->lbAppName->setText(tr("AppNameTest"));
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
