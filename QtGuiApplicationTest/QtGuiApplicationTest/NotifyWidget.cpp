#include "NotifyWidget.h"
#include "ui_NotifyWidget.h"

#include <QMutexLocker>
#include <QDesktopWidget>
#include <QPropertyAnimation>
#include <QSize>
#include <QTimer>
#include <QDateTime>

QAtomicPointer<NotifyWidget> NotifyWidget::m_gInstance = Q_NULLPTR;
QMutex                       NotifyWidget::m_instanceMutex;

NotifyWidget::NotifyWidget(QWidget *parent)
    : QWidget(parent)
    , m_startTime(0)
    , m_showFlag(false)
    , m_initAnimationFlag(false)
    , m_pAnimationShow(new QPropertyAnimation(this, "geometry", this))
    , m_pAnimationHide(new QPropertyAnimation(this, "geometry", this))
    , m_pTimerHide(new QTimer(this))
{
    ui = new Ui::NotifyWidget();
    ui->setupUi(this);
    this->setWindowFlags(Qt::ToolTip);
    this->setVisible(false);
    
    m_pTimerHide->setInterval(200);
    this->connect(m_pTimerHide, SIGNAL(timeout()), this, SLOT(SlotHideBoxTimeOut()));
}

NotifyWidget::~NotifyWidget()
{
    m_pTimerHide->stop();
    delete ui;
}

void NotifyWidget::CreateInstance()
{
#ifdef Q_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE
    if (m_gInstance.testAndSetOrdered(Q_NULLPTR, Q_NULLPTR))
    {
        m_gInstance.testAndSetOrdered(Q_NULLPTR, new NotifyWidget());
    }
#else
    if (m_gInstance == Q_NULLPTR)
    {
        QMutexLocker locker(&m_instanceMutex);
        if (m_gInstance == Q_NULLPTR)
        {
            m_gInstance = new NotifyWidget();
            m_gInstance.load()->show();
        }
    }
#endif
}

void NotifyWidget::DestroyInstance()
{
#ifdef Q_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE
    NotifyWidget *pNotify = m_gInstance.loadAcquire();
    if (pNotify != Q_NULLPTR)
    {
        if (m_gInstance.testAndSetOrdered(pNotify, Q_NULLPTR))
        {
            delete pNotify;
        }
    }
#else
    if (m_gInstance != Q_NULLPTR)
    {
        QMutexLocker locker(&m_instanceMutex);
        if (m_gInstance != Q_NULLPTR)
        {
            delete m_gInstance;
            m_gInstance = Q_NULLPTR;
        }
    }
#endif
}

void NotifyWidget::ShowInformation(const QString& information)
{
    m_gInstance.load()->SetMessage(information);
    m_gInstance.load()->ShowBox();
}

void NotifyWidget::ShowBox()
{
    if (!m_initAnimationFlag)
    {
        // 此处为了延迟 UI 坐标计算，在 main 函数中过早创建该类实例，qApp 没有主窗口，无法获取桌面尺寸。
        m_initAnimationFlag = true;
        this->setVisible(true);
        QRect desktopRect = qApp->desktop()->screenGeometry(qApp->activeWindow());
        QPoint showPoint;
        showPoint.rx() = desktopRect.right() - this->width();
        showPoint.ry() = desktopRect.bottom() - this->height();
        QPoint hidePoint;
        hidePoint.rx() = desktopRect.right() - this->width();
        hidePoint.ry() = desktopRect.bottom() + 1;
        this->move(hidePoint.x(), hidePoint.y());

        m_pAnimationShow->setDuration(500);
        m_pAnimationShow->setStartValue(QRect(hidePoint.x(), hidePoint.y(), this->width(), this->height()));
        m_pAnimationShow->setEndValue(QRect(showPoint.x(), showPoint.y(), this->width(), this->height()));

        m_pAnimationHide->setDuration(500);
        m_pAnimationHide->setStartValue(QRect(showPoint.x(), showPoint.y(), this->width(), this->height()));
        m_pAnimationHide->setEndValue(QRect(hidePoint.x(), hidePoint.y(), this->width(), this->height()));
    }
    if (!m_showFlag)
    {
        m_showFlag = true;
        m_pAnimationShow->start();
        m_pTimerHide->start();
    }
    m_startTime = QDateTime::currentMSecsSinceEpoch();
}

void NotifyWidget::SetMessage(const QString& message)
{
    ui->lbMessage->setText(message);
}

void NotifyWidget::on_btnNotifyClose_clicked()
{
    if (m_showFlag)
    {
        m_showFlag = false;
        m_pAnimationHide->start();
        m_pTimerHide->stop();
    }
}

void NotifyWidget::SlotHideBoxTimeOut()
{
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch();
    if (m_showFlag && currentTime - m_startTime > 3000)
    {
        m_showFlag = false;
        m_pTimerHide->stop();
        m_pAnimationHide->start();
    }
}
