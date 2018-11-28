#ifndef DEBUG_PANEL_H
#define DEBUG_PANEL_H

#include <QWidget>

namespace Ui {
class DebugPanel;
}

class DebugPanel : public QWidget
{
    Q_OBJECT

public:
    explicit DebugPanel(QWidget *parent = 0);
    ~DebugPanel();

private:
    void ShowOrHideDebugPanel();

private slots:
    void on_pushButton_clicked();

protected:
    virtual void closeEvent(QCloseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

private:
    Ui::DebugPanel *ui;
};

#endif // DEBUG_PANEL_H
