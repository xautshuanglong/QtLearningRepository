#ifndef MISCELLANEOUS_IMAGE_QPIXMAP_H
#define MISCELLANEOUS_IMAGE_QPIXMAP_H

#include "MiscellaneousBase.h"

namespace Ui {class MiscellaneousImageQPixmap;};

class QMenu;

class MiscellaneousImageQPixmap : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousImageQPixmap(QWidget *parent = 0);
    ~MiscellaneousImageQPixmap();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private:
    void InitCustomContextMenu();

private slots:
    void SlotStackedWidgetYuvFile();
    void SlotStackedWidgetYuvStream();
    void SlotStackedWidgetYuvSharedMemory();
    void SlotStackedWidgetSwitchPage();
    void SlotStackedWidgetProperties();
    void on_stackedWidget_customContextMenuRequested(const QPoint &position);
    void on_stackedWidget_currentChanged(int index);

private:
    Ui::MiscellaneousImageQPixmap *ui;
    QMenu                         *mpCustomContext;
};

#endif // MISCELLANEOUS_IMAGE_QPIXMAP_H
