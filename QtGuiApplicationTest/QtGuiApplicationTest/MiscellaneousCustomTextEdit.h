#ifndef MISCELLANEOUS_CUSTOM_TEXTEDIT_H
#define MISCELLANEOUS_CUSTOM_TEXTEDIT_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousCustomTextEdit.h"

class QPaintEvent;

class MiscellaneousCustomTextEdit : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousCustomTextEdit(QWidget *parent = 0);
    ~MiscellaneousCustomTextEdit();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private:
    void AppendRichText(QTextEdit* pTextEdit);

protected:
    virtual void paintEvent(QPaintEvent* pEvent) override;

private slots:
    void on_teCustom_cursorPositionChanged();
    void on_teCustom_currentCharFormatChanged(const QTextCharFormat& format);
    void on_btnAppendText_clicked();
    void on_btnTestEntry_clicked();

private:
    Ui::MiscellaneousCustomTextEdit ui;
};

#endif // MISCELLANEOUS_CUSTOM_TEXTEDIT_H
