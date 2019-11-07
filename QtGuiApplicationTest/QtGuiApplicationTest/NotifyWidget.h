#ifndef NOTIFY_WIDGET_H
#define NOTIFY_WIDGET_H

#include <QWidget>
#include <QMutex>
#include <QAtomicPointer>

class QPropertyAnimation;
class QTimer;

namespace Ui { class NotifyWidget; };

class NotifyWidget : public QWidget
{
    Q_OBJECT

public:
    ~NotifyWidget();
    static void CreateInstance();
    static void DestroyInstance();
    static void ShowInformation(const QString& information);

private:
    NotifyWidget(QWidget *parent = 0);
    void ShowBox();
    void SetMessage(const QString& message);

private slots:
    void on_btnNotifyClose_clicked();
    void SlotHideBoxTimeOut();

private:
    static QMutex                       m_instanceMutex;
    static QAtomicPointer<NotifyWidget> m_gInstance;
    Ui::NotifyWidget                   *ui;
    QPropertyAnimation                 *m_pAnimationShow;
    QPropertyAnimation                 *m_pAnimationHide;
    QTimer                             *m_pTimerHide;
    qint64                              m_startTime;
    bool                                m_showFlag;
    bool                                m_initAnimationFlag;
};

#endif // NOTIFY_WIDGET_H
