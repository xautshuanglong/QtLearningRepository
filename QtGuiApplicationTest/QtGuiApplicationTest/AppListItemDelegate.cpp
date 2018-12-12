#include "AppListItemDelegate.h"

#include <QSpinBox>
#include <QPainter>

AppListItemDelegate::AppListItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
    , mpParent(parent)
{
}

AppListItemDelegate::~AppListItemDelegate()
{
}

void AppListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //QStyledItemDelegate::paint(painter, option, index);

    if (!index.isValid()) return;

    painter->save();

    AppListItemStatus status = (AppListItemStatus)(index.data(Qt::UserRole).toInt());
    QVariant itemDataVariant = index.data(Qt::UserRole + 1);
    AppListItemData itemData = itemDataVariant.value<AppListItemData>();

    QRectF itemRectF;
    itemRectF.setX(option.rect.x() + 5);
    itemRectF.setY(option.rect.y() + 5);
    itemRectF.setWidth(option.rect.width() - 10);
    itemRectF.setHeight(option.rect.height() - 10);

    const qreal radius = 10;
    QPen itemBoderPen(Qt::gray);
    itemBoderPen.setWidth(1);

    if (option.state.testFlag(QStyle::State_Selected))
    {
        itemBoderPen.setColor(Qt::blue);
        painter->setPen(itemBoderPen);
        painter->setBrush(QColor(120, 170, 220));
    }
    else if (option.state.testFlag(QStyle::State_MouseOver))
    {
        itemBoderPen.setColor(Qt::green);
        painter->setPen(itemBoderPen);
        painter->setBrush(QColor(229, 241, 255));
    }
    else
    {
        itemBoderPen.setColor(Qt::gray);
        painter->setPen(itemBoderPen);
        painter->setBrush(Qt::NoBrush);
    }
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawRoundedRect(itemRectF, radius, radius);

    QRect NameRect = QRect(itemRectF.left() + 10, itemRectF.top() + 10, itemRectF.width() - 30, 20);
    QRect circle = QRect(NameRect.right(), itemRectF.top() + 10, 10, 10);
    QRect telRect = QRect(itemRectF.left() + 10, itemRectF.bottom() - 25, itemRectF.width() - 10, 20);

    switch (status)
    {
    case ITEMSTATUS_RED:
        painter->setBrush(Qt::red);
        painter->setPen(QPen(Qt::red));
        break;
    case ITEMSTATUS_BLUE:
        painter->setBrush(Qt::blue);
        painter->setPen(QPen(Qt::blue));
        break;
    case ITEMSTATUS_YELLOW:
        painter->setBrush(Qt::yellow);
        painter->setPen(QPen(Qt::yellow));
        break;
    }

    painter->drawEllipse(circle);

    painter->setPen(QPen(Qt::black));
    painter->setFont(QFont("Times", 12, QFont::Bold));
    painter->drawText(NameRect, Qt::AlignLeft, itemData.name);

    painter->setPen(QPen(Qt::gray));
    painter->setFont(QFont("Times", 10));
    painter->drawText(telRect, Qt::AlignLeft, QString::number(itemData.nStar));

    painter->restore();
}

QSize AppListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(160, 60);

    QSize retValue = QStyledItemDelegate::sizeHint(option, index);
    return retValue;
}

QWidget *AppListItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return nullptr;
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void AppListItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QStyledItemDelegate::setEditorData(editor, index);

    //int value = index.model()->data(index, Qt::EditRole).toInt();
    //QSpinBox *pSpinBox = static_cast<QSpinBox*>(editor);
    //pSpinBox->setValue(value);
}

void AppListItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QStyledItemDelegate::setModelData(editor, model, index);

    //QSpinBox *pSpinBox = static_cast<QSpinBox*>(editor);
    //pSpinBox->interpretText();
    //int value = pSpinBox->value();
    //model->setData(index, value, Qt::EditRole);
}

void AppListItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::updateEditorGeometry(editor, option, index);
    //editor->setGeometry(option.rect);
}
