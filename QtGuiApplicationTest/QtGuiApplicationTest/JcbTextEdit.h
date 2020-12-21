#pragma once

#include <QTextEdit>

class JcbTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    JcbTextEdit(QWidget *parent);
    ~JcbTextEdit();
};
