#include "MiscellaneousStringUtil.h"

#include <QStyledItemDelegate>
#include <QListWidget>
#include <QCheckBox>

#include <StringUtil.h>

MiscellaneousStringUtil::MiscellaneousStringUtil(QWidget *parent)
    : MiscellaneousBase(parent)
    , m_bSelected(false)
{
    ui.setupUi(this);

    // QComboBox
    this->InitCharacterTypeComboBox();
}

MiscellaneousStringUtil::~MiscellaneousStringUtil()
{
}

QString MiscellaneousStringUtil::GetTitle()
{
    return QObject::tr("StringUtil");
}

QString MiscellaneousStringUtil::GetTitleTooltip()
{
    return QObject::tr("StringUtil methods test entry.");
}

MiscellaneousTestGroup MiscellaneousStringUtil::GetGroupID()
{
    return MiscellaneousTestGroup::Utility_Tools;
}

MiscellaneousTestItem MiscellaneousStringUtil::GetItemID()
{
    return MiscellaneousTestItem::Utility_StringUtil;
}

void MiscellaneousStringUtil::InitCharacterTypeComboBox()
{
    ui.cbCharacterTypes->setCurrentIndex(0);
    QStyledItemDelegate *pItemDelegate = new QStyledItemDelegate();
    ui.cbCharacterTypes->setItemDelegate(pItemDelegate);
    QWidget *pShadow = qobject_cast<QWidget*>(ui.cbCharacterTypes->view()->parent());
    if (pShadow != Q_NULLPTR)
    {
        pShadow->setWindowFlags(pShadow->windowFlags() | Qt::NoDropShadowWindowHint);
        //pShadow->setAttribute(Qt::WA_TranslucentBackground);
    }
    m_pListWidget = new QListWidget(this);
    m_pLineEdit = new QLineEdit(this);
    for (int i = 0; i < 5; ++i)
    {
        QCheckBox *pCheckBox = new QCheckBox(this);
        pCheckBox->setText(QStringLiteral("Qteaaaaaaaaaaaaaaaaaaaaaa%1").arg(i));
        pCheckBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        //pCheckBox->setStyleSheet("background-color: green;");
        QListWidgetItem *pItem = new QListWidgetItem(m_pListWidget);
        pItem->setData(Qt::UserRole, i);
        m_pListWidget->addItem(pItem);
        m_pListWidget->setItemWidget(pItem, pCheckBox);
        connect(pCheckBox, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
    }
    ui.cbCharacterTypes->setModel(m_pListWidget->model());
    ui.cbCharacterTypes->setView(m_pListWidget);
    ui.cbCharacterTypes->setLineEdit(m_pLineEdit);
    ui.cbCharacterTypes->setIconSize(QSize(10, 10));
    m_pLineEdit->setReadOnly(true);
}

void MiscellaneousStringUtil::stateChanged(int state)
{
    m_bSelected = true;
    QString strSelectedData("");
    m_strSelectedText.clear();
    QObject *object = QObject::sender();
    QCheckBox *pSenderCheckBox = static_cast<QCheckBox*>(object);
    QSize hitSize = pSenderCheckBox->sizeHint();
    QRect genomertyRect = pSenderCheckBox->geometry();
    int nCount = m_pListWidget->count();
    for (int i = 0; i < nCount; ++i)
    {
        QListWidgetItem *pItem = m_pListWidget->item(i);
        QWidget *pWidget = m_pListWidget->itemWidget(pItem);
        QCheckBox *pCheckBox = (QCheckBox *)pWidget;
        if (pCheckBox->isChecked())
        {
            QString strText = pCheckBox->text();
            strSelectedData.append(strText).append(";");
        }
        //所点击的复选框
        if (pSenderCheckBox == pCheckBox)
        {
            int nData = pItem->data(Qt::UserRole).toInt();
        }
    }
    if (strSelectedData.endsWith(";"))
        strSelectedData.remove(strSelectedData.count() - 1, 1);
    if (!strSelectedData.isEmpty())
    {
        //ui.comboBox->setEditText(strSelectedData);
        m_strSelectedText = strSelectedData;
        m_pLineEdit->setText(strSelectedData);
        m_pLineEdit->setToolTip(strSelectedData);
    }
    else
    {
        m_pLineEdit->clear();
        //ui.comboBox->setEditText("");
    }
    m_bSelected = false;
}

void MiscellaneousStringUtil::on_btnCharacterShiftLeft_clicked()
{
    QString target = ui.leShiftTarget->text();
    m_strShifted = StringUtil::CharacterShiftLeft(target, 5);
}

void MiscellaneousStringUtil::on_btnCharacterShiftRight_clicked()
{
    QString original = StringUtil::CharacterShiftRight(m_strShifted, 5);
}

void MiscellaneousStringUtil::on_btnBase64ShiftEncode_clicked()
{
    QString target = ui.leEncodeTarget->text();
    QString m_strEncoded = StringUtil::Base64ShiftEncode(target);
}

void MiscellaneousStringUtil::on_btnBase64ShiftDecode_clicked()
{
    QString original = StringUtil::Base64ShiftDecode(m_strEncoded);
}

void MiscellaneousStringUtil::on_btnRandomPassword_clicked()
{
    QString password = StringUtil::GetRandomPassword(CAPITAL, 1);
    ui.leEncodeTarget->setText(password);
    ui.leShiftTarget->setText(password);
}
