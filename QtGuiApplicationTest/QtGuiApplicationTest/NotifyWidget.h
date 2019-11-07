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
    static void ShowInformation(const QString& information, qint64 duration = 5000);

private:
    NotifyWidget(QWidget *parent = 0);
    void ShowBox();
    void SetMessage(const QString& message);
    void SetDuration(qint64 duration){ m_showDuration = duration; }

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
    qint64                              m_showDuration;
    bool                                m_showFlag;
    bool                                m_initAnimationFlag;
};

#endif // NOTIFY_WIDGET_H
