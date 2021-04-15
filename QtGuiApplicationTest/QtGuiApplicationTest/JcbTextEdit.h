#pragma once

#include <QTextEdit>
//#include <QtWidgets/private/qtextedit_p.h>

class QComboBox;

class JcbTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    JcbTextEdit(QWidget *parent);
    ~JcbTextEdit();

protected:
    virtual void paintEvent(QPaintEvent* e) override;
    virtual void mousePressEvent(QMouseEvent* e) override;

private:
    QComboBox                 *m_pKeywordOption;
};

//class JcbTextEditPrivate : public QTextEditPrivate
//{
//public:
//    JcbTextEditPrivate();
//    ~JcbTextEditPrivate();
//
//private:
//
//};
//
//JcbTextEditPrivate::JcbTextEditPrivate()
//{
//}
//
//JcbTextEditPrivate::~JcbTextEditPrivate()
//{
//}
