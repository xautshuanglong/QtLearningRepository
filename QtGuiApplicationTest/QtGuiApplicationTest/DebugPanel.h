#ifndef DEBUG_PANEL_H
#define DEBUG_PANEL_H

#include <QWidget>

class QMenuBar;

namespace Ui {
class DebugPanel;
}

class DebugPanel : public QWidget
{
    Q_OBJECT

public:
    ~DebugPanel();

    static DebugPanel* GetInstance();
    void ListenKeyboard(QObject *pTarget);

private:
    explicit DebugPanel(QWidget *parent = 0);
    void HandleKeyPressEvent(QKeyEvent *event);
    void ShowOrHideDebugPanel();

private slots:

protected:
    virtual void closeEvent(QCloseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::DebugPanel    *ui;
    QMenuBar          *mpMenuBar;
};

#endif // DEBUG_PANEL_H
