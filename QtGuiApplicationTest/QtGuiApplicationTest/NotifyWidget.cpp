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
    , m_pAnimationShow(new QPropertyAnimation(this, "geometry", this))
    , m_pAnimationHide(new QPropertyAnimation(this, "geometry", this))
    , m_pTimerHide(new QTimer(this))
{
    ui = new Ui::NotifyWidget();
    ui->setupUi(this);
    this->setWindowFlags(Qt::ToolTip);
    this->setVisible(true);
    
    m_pTimerHide->setInterval(200);
    this->connect(m_pTimerHide, SIGNAL(timeout()), this, SLOT(SlotHideBoxTimeOut()));

    QRect desktopRect = qApp->desktop()->screenGeometry(qApp->activeWindow());
    QPoint showPoint;
    showPoint.rx() = desktopRect.right() - this->width();
    showPoint.ry() = desktopRect.bottom() - this->height() - 60;
    QPoint hidePoint;
    hidePoint.rx() = desktopRect.right() - this->width();
    hidePoint.ry() = desktopRect.bottom();
    this->move(hidePoint.x(), hidePoint.y());
    //this->move(desktopRect.right() - this->width(), desktopRect.bottom() - this->height());

    m_pAnimationShow->setDuration(500);
    m_pAnimationShow->setStartValue(QRect(hidePoint.x(), hidePoint.y(), this->width(), this->height()));
    m_pAnimationShow->setEndValue(QRect(showPoint.x(), showPoint.y(), this->width(), this->height()));

    m_pAnimationHide->setDuration(500);
    m_pAnimationHide->setStartValue(QRect(showPoint.x(), showPoint.y(), this->width(), this->height()));
    m_pAnimationHide->setEndValue(QRect(hidePoint.x(), hidePoint.y(), this->width(), this->height()));
}

NotifyWidget::~NotifyWidget()
{
    m_pTimerHide->stop();
    delete ui;
}

void NotifyWidget::CreateSingleInstance()
{
#ifdef Q_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE
    if (m_gInstance.testAndSetOrdered(Q_NULLPTR, Q_NULLPTR))
    {
        QMutexLocker locker(&m_instanceMutex);
        m_gInstance.testAndSetOrdered(Q_NULLPTR, new NotifyWidget());
    }
#else
    if (m_gInstance == Q_NULLPTR)
    {
        QMutexLocker locker(&m_instanceMutex);
        if (m_gInstance == Q_NULLPTR)
        {
            m_gInstance = new NotifyWidget();
        }
    }
#endif
}

void NotifyWidget::ShowInformation(const QString& information)
{
    NotifyWidget::CreateSingleInstance();
    m_gInstance.load()->SetMessage(information);
    m_gInstance.load()->ShowBox();
}

void NotifyWidget::ShowBox()
{
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
    if (m_showFlag && currentTime - m_startTime > 5000)
    {
        m_showFlag = false;
        m_pTimerHide->stop();
        m_pAnimationHide->start();
    }
}
