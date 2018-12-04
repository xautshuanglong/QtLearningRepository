#ifndef DEBUG_PANEL_H
#define DEBUG_PANEL_H

#include <QWidget>
#include <QMutex>

class QMenu;
class QMenuBar;
class QListWidget;
class QStackedWidget;
class QSplitter;
class DebugInfoBaseWidget;
class TitleBarWidget;

namespace Ui
{
    class DebugPanel;
}

/************************************************************************/
/* 调试面板：统一管理调试信息展示。                                       */
/* 注意：为方便调用采用了单例模式，要确保应用退出自行释放静态实例前关闭面板，*/
/* 调用QWidget::close。                                                 */
/* 监听主界面按键事件，并在主界面关闭时（closeEvent）将调试面板也关闭。     */
/************************************************************************/
class DebugPanel : public QWidget
{
    Q_OBJECT

public:
    enum MenuID
    {
        // 文件菜单
        FILE,
        FILE_SAVE,
        FILE_SAVE_AS,
        // 视图菜单
        VIEW,
        VIEW_LIST,
        VIEW_TAB,
        // 设置菜单
        SETTING,
        SETTING_DEBUG,
        SETTING_DEBUG_TEST1,
        SETTING_DEBUG_TEST2,
        SETTING_REGION,
        SETTING_REGION_CN,
        SETTING_REGION_EN
    };
    Q_ENUM(MenuID)

public:
    explicit DebugPanel(QWidget *parent = 0);
    ~DebugPanel();

    static DebugPanel* GetInstance();
    void ListenKeyboard(QObject *pTarget);
    void AddDebugInfoWidget(QString topic, DebugInfoBaseWidget *pWidget);

private:
    void InitMenu();
    void InitDebugInfoWidgets();
    void HandleKeyPressEvent(QKeyEvent *event);
    void ShowOrHideDebugPanel();

private slots:
    void SlotMenuMapped(int menuID);

protected:
    virtual void closeEvent(QCloseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual bool eventFilter(QObject *obj, QEvent *event) override;

private:
    static QAtomicPointer<DebugPanel> mInstance;
    static QMutex                     mMutexInstance;
    Ui::DebugPanel    *ui;
    TitleBarWidget    *mpTitleWidget;
    QMenu             *pMenuFile;
    QMenuBar          *mpMenuBar;
    QListWidget       *mpListWidget;
    QStackedWidget    *mpStackedWidget;
    QSplitter         *mpSpliter;
};

#endif // DEBUG_PANEL_H
