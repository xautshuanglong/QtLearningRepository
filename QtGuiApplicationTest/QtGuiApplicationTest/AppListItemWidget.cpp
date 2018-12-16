#include "AppListItemWidget.h"
#include "ui_AppListItemWidget.h"

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
    ;
}
