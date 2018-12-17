#pragma once

#include <QWidget>

namespace Ui
{
    class AppListItemWidget;
};

class AppListItemWidget : public QWidget
{
    Q_OBJECT

public:
    AppListItemWidget(QWidget *parent = Q_NULLPTR);
    ~AppListItemWidget();

    void SetIconPath(const QString &appIconPath);
    void SetAppName(const QString &appName);
    void SetAppDescription(const QString &appDescription);

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    Ui::AppListItemWidget *ui;
};
