#include "JcbTextEdit.h"

#include <QPainter>
#include <QPaintEvent>
#include <QComboBox>

#include <LogUtil.h>

JcbTextEdit::JcbTextEdit(QWidget *parent)
    : QTextEdit(parent)
    , m_pKeywordOption(new QComboBox(this))
{
    // 关键字选项测试
    m_pKeywordOption->setVisible(false);
    m_pKeywordOption->addItem("top");
    m_pKeywordOption->addItem("bottom");
    m_pKeywordOption->addItem("left");
    m_pKeywordOption->addItem("right");
}

JcbTextEdit::~JcbTextEdit()
{
}

void JcbTextEdit::paintEvent(QPaintEvent* e)
{
    //const QRect& paintRect = e->rect(); // 选中、输入、删除等都会触发重绘，矩形为局部渲染区域
    const QRect &geometryRect = this->geometry();
    QRect paintRect = QRect(geometryRect.topLeft(), geometryRect.bottomRight());
    paintRect.moveTo(0, 0);

    //LogUtil::Debug(CODE_LOCATION, "paintEvent paintRect (%d, %d, %d, %d)", paintRect.left(), paintRect.top(), paintRect.width(), paintRect.height());

    QRectF flagRect(0, 0, 200, 200);
    QPointF offset((paintRect.width() - flagRect.width()) / 2, (paintRect.height() - flagRect.height()) / 2);
    flagRect.moveTo(offset);

    QPainter painter(this->viewport());
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(qRgb(255, 255, 0));
    painter.drawEllipse(flagRect);
    painter.setPen(qRgb(255, 0, 0));
    painter.drawArc(flagRect, 0, 90 * 16);
    painter.setPen(qRgb(0, 0, 255));
    painter.drawArc(flagRect, 180 * 16, 90 * 16);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(qRgb(229, 49, 32), Qt::SolidPattern));

    const qreal horizontalHeight = 10.0f;
    QRectF horizontal(flagRect);
    horizontal.setHeight(horizontalHeight);
    horizontal.moveTo(flagRect.left(), flagRect.height() / 2 + flagRect.top() - horizontalHeight / 2);
    painter.drawRoundedRect(horizontal, 5, 5);

    const qreal verticalWidth = 10.0f;
    QRectF vertical(flagRect);
    vertical.setWidth(verticalWidth);
    vertical.moveTo(flagRect.width() / 2 + flagRect.left() - verticalWidth / 2, flagRect.top());
    painter.drawRoundedRect(vertical, 5, 5);

    painter.restore();
    QTextEdit::paintEvent(e);
}

void JcbTextEdit::mousePressEvent(QMouseEvent* e)
{
    const QPoint clickPoint = e->pos();
    m_pKeywordOption->move(clickPoint);
    m_pKeywordOption->setVisible(true);
    LogUtil::Debug(CODE_LOCATION, "JcbTextEdit mouse click pos : (%d, %d)", clickPoint.x(), clickPoint.y());
    QTextEdit::mousePressEvent(e);
}
