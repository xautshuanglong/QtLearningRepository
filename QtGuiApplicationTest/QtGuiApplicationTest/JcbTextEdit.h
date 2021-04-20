#pragma once

#include <QTextEdit>

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
