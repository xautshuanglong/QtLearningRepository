#pragma once

#include <QStyledItemDelegate>

enum AppListItemStatus
{
    ITEMSTATUS_RED,
    ITEMSTATUS_BLUE,
    ITEMSTATUS_YELLOW
};

struct AppListItemData
{
    QString name;
    QString description;
    int     nStar;
};
Q_DECLARE_METATYPE(AppListItemData)

class AppListItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    AppListItemDelegate(QObject *parent);
    ~AppListItemDelegate();

    // painting
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    // editing
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    QObject    *mpParent;
};
