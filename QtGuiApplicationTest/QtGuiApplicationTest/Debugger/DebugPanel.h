#ifndef DEBUG_PANEL_H
#define DEBUG_PANEL_H

#include <QWidget>
#include <QMutex>
#include <QMap>
#include <typeinfo>
#include <typeindex>
#include "JCB_Logger/LogUtil.h"

class QMenu;
class QMenuBar;
class QListWidget;
class QStackedWidget;
class QSplitter;
class QTimer;
class DebugInfoBaseWidget;
class TitleBarWidget;

namespace Ui
{
    class DebugPanel;
}

/************************************************************************/
/* 调试面板：统一管理调试信息展示。                                       */
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
        VIEW_UPDATE,         // 更新频率
        VIEW_UPDATE_HIGH,    // 更新频率 - 高
        VIEW_UPDATE_MIDDLE,  // 更新频率 - 中
        VIEW_UPDATE_LOW,     // 更新频率 - 低
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
    static void ClearInstance();
    void ListenKeyboard(QObject *pTarget);
    void AddDebugInfoWidget(QString topic, DebugInfoBaseWidget *pWidget);
    template <typename T> T* GetDebugInfoWidget()
    {
        const std::type_info &typeInfo = typeid(T);
        LogUtil::Debug(CODE_LOCATION, "Class: %s  HashCode: %llu  TypIndex: %d", typeInfo.name(), typeInfo.hash_code(), std::type_index(typeInfo));
        DebugInfoBaseWidget *pWidget = mMapTypeWidget[typeid(T).hash_code()];
        T* pRetValue = dynamic_cast<T*>(pWidget);
        return pRetValue;
    }

private:
    void InitMenu();
    void InitDebugInfoWidgets();
    void HandleKeyPressEvent(QKeyEvent *event);
    void HandleUpdateRate(int menuID);
    void ShowOrHideDebugPanel();

private slots:
    void SlotMenuMapped(int menuID);
    void SlotUpdateDebugWidget();

protected:
    virtual void closeEvent(QCloseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void hideEvent(QHideEvent *event) override;

private:
    static QAtomicPointer<DebugPanel>    mInstance;
    static QMutex                        mMutexInstance;
    Ui::DebugPanel                      *ui;
    TitleBarWidget                      *mpTitleWidget;
    QMenuBar                            *mpMenuBar;
    QListWidget                         *mpListWidget;
    QStackedWidget                      *mpStackedWidget;
    QSplitter                           *mpSpliter;
    QTimer                              *mpUpdateTimer;
    QMap<quint64, DebugInfoBaseWidget *> mMapTypeWidget;
    QMap<int, QObject*>                  mMapMenuIdItem;
};

#endif // DEBUG_PANEL_H
