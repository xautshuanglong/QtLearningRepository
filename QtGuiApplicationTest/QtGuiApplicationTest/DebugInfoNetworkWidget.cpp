#include "DebugInfoNetworkWidget.h"
#include "ui_DebugInfoNetworkWidget.h"

// QT Headers
#include <QMetaEnum>

// Self Headers
#include <LogUtil.h>
#include "DebugMenuEvent.h"
#include "DebugPanel.h"

DebugInfoNetworkWidget::DebugInfoNetworkWidget(QWidget *parent)
    : DebugInfoBaseWidget(parent)
    , ui(new Ui::DebugInfoNetworkWidget())
{
    ui->setupUi(this);
}

DebugInfoNetworkWidget::~DebugInfoNetworkWidget()
{
    delete ui;
}

bool DebugInfoNetworkWidget::OnDebugMenuEvent(DebugMenuEvent *event)
{
    int menuID = event->GetMenuID();
    QMetaEnum menuIDEnum = QMetaEnum::fromType<DebugPanel::MenuID>();
    LogUtil::Info(CODE_LOCATION, "Do not specify MenuID = %s handler ...", menuIDEnum.valueToKey(menuID));

    return true;
}

void DebugInfoNetworkWidget::OnUpdateDebugInfo()
{
    ;
}

void DebugInfoNetworkWidget::resizeEvent(QResizeEvent *event)
{
    ;
}

void DebugInfoNetworkWidget::on_btnPingTest_clicked()
{
    int i = 0;
}
