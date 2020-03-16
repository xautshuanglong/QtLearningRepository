#include "MiscellaneousStringUtil.h"

#include <QStyledItemDelegate>
#include <QListWidget>
#include <QCheckBox>

#include <StringUtil.h>

struct CharacterTypes
{
    uint TypeID;
    QString TypeShortText;
    QString TeypeShowText;
};

static CharacterTypes gCharacterTypes[] = {
    {CandidateCharacterType::CAPITAL,      "A-Z",     QStringLiteral("´óÐ´×ÖÄ¸        ")},
    {CandidateCharacterType::LOWERCASE,    "a-z",     QStringLiteral("Ð¡Ð´×ÖÄ¸        ")},
    {CandidateCharacterType::DIGITAL,      "0-9",     QStringLiteral("Êý×Ö            ")},
    {CandidateCharacterType::SHARP,        "#",       QStringLiteral("# ¾®ºÅ          ")},
    {CandidateCharacterType::ASTERISK,     "*",       QStringLiteral("* ÐÇºÅ          ")},
    {CandidateCharacterType::PLUS,         "+",       QStringLiteral("+ ¼ÓºÅ          ")},
    {CandidateCharacterType::MINUS,        "-",       QStringLiteral("- ¼õºÅ          ")},
    {CandidateCharacterType::SLASH,        "/ or \\", QStringLiteral("/ Ð±¸Ü \\ ·´Ð±¸Ü")},
    {CandidateCharacterType::QUOTE,        "'",       QStringLiteral("' µ¥ÒýºÅ         ")},
    {CandidateCharacterType::DOUBLE_QUOTE, "\"",      QStringLiteral("\" Ë«ÒýºÅ         ")},
    {CandidateCharacterType::BACK_QUOTE,   "`",       QStringLiteral("` ·´ÒýºÅ         ")},
    {CandidateCharacterType::TILDE,        "~",       QStringLiteral("~ ²¨ÀËºÅ         ")},
    {CandidateCharacterType::EXCLAM,       "!",       QStringLiteral("! ¸ÐÌ¾ºÅ         ")},
    {CandidateCharacterType::AT,           "@",       QStringLiteral("@ at ·ûºÅ        ")},
    {CandidateCharacterType::DOLLAR,       "$",       QStringLiteral("$ ÃÀÔªºÅ         ")},
    {CandidateCharacterType::PERCENT,      "%",       QStringLiteral("% °Ù·ÖºÅ         ")},
    {CandidateCharacterType::CARET,        "^",       QStringLiteral("^ ÍÑ×Ö·û         ")},
    {CandidateCharacterType::AMPERSAND,    "&",       QStringLiteral("& And ·ûºÅ       ")},
    {CandidateCharacterType::PAREN,        "()",      QStringLiteral("() Ô²À¨ºÅ        ")},
    {CandidateCharacterType::UNDERSCORE,   "_",       QStringLiteral("_ ÏÂ»®Ïß         ")},
    {CandidateCharacterType::EQUAL,        "=",       QStringLiteral("= µÈÓÚºÅ         ")},
    {CandidateCharacterType::BRACKET,      "[]",      QStringLiteral("[] ·½À¨ºÅ        ")},
    {CandidateCharacterType::BRACE,        "{}",      QStringLiteral("{} ´óÀ¨ºÅ        ")},
    {CandidateCharacterType::SEMICOLON,    ";",       QStringLiteral("; ·ÖºÅ           ")},
    {CandidateCharacterType::COLON,        ":",       QStringLiteral(": Ã°ºÅ           ")},
    {CandidateCharacterType::BAR,          "|",       QStringLiteral("| ÊúÏß           ")},
    {CandidateCharacterType::COMMA,        ",",       QStringLiteral(", ¶ººÅ           ")},
    {CandidateCharacterType::LESS,         "<",       QStringLiteral("< Ð¡ÓÚºÅ         ")},
    {CandidateCharacterType::GREATER,      ">",       QStringLiteral("> ´óÓÚºÅ         ")},
    {CandidateCharacterType::PERIOD,       ".",       QStringLiteral(". ¾äºÅ           ")},
    {CandidateCharacterType::QUESTION,     "?",       QStringLiteral("? ÎÊºÅ           ")},
    {CandidateCharacterType::SPACE,        " ",       QStringLiteral(" ¿Õ¸ñ            ")}
};
const int typesCount = sizeof(gCharacterTypes) / sizeof(CharacterTypes);

MiscellaneousStringUtil::MiscellaneousStringUtil(QWidget *parent)
    : MiscellaneousBase(parent)
    , m_bSelected(false)
    , m_characterTypes(0)
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
    for (int i = 0; i < typesCount; ++i)
    {
        QCheckBox *pCheckBox = new QCheckBox(this);
        pCheckBox->setText(QStringLiteral("%1").arg(gCharacterTypes[i].TeypeShowText));
        pCheckBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        //pCheckBox->setStyleSheet("background-color: green;");
        QListWidgetItem *pItem = new QListWidgetItem(m_pListWidget);
        pItem->setData(Qt::UserRole, gCharacterTypes[i].TypeID);
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

void MiscellaneousStringUtil::GetCharacterTypeInfo(uint typeID, QString& shortText, QString& showText)
{
    for (int i = 0; i < typesCount; ++i)
    {
        if (typeID == gCharacterTypes[i].TypeID)
        {
            showText = gCharacterTypes[i].TeypeShowText;
            shortText = gCharacterTypes[i].TypeShortText;
            break;
        }
    }
}

void MiscellaneousStringUtil::stateChanged(int state)
{
    m_bSelected = true;
    QString strSelectedData("");
    m_strSelectedText.clear();
    QObject *object = QObject::sender();
    QCheckBox *pSenderCheckBox = static_cast<QCheckBox*>(object);
    //QSize hitSize = pSenderCheckBox->sizeHint();
    //QRect genomertyRect = pSenderCheckBox->geometry();
    m_characterTypes = 0;
    int nCount = m_pListWidget->count();
    for (int i = 0; i < nCount; ++i)
    {
        QListWidgetItem *pItem = m_pListWidget->item(i);
        QWidget *pWidget = m_pListWidget->itemWidget(pItem);
        QCheckBox *pCheckBox = (QCheckBox *)pWidget;
        if (pCheckBox->isChecked())
        {
            uint typeID = pItem->data(Qt::UserRole).toUInt();
            m_characterTypes |= typeID;
            QString showText, shortText;
            this->GetCharacterTypeInfo(typeID, shortText, showText);
            strSelectedData.append(shortText).append(";");
        }
    }
    if (strSelectedData.endsWith(";"))
        strSelectedData.remove(strSelectedData.count() - 1, 1);
    if (!strSelectedData.isEmpty())
    {
        m_strSelectedText = strSelectedData;
        m_pLineEdit->setText(strSelectedData);
        m_pLineEdit->setToolTip(strSelectedData);
    }
    else
    {
        m_pLineEdit->clear();
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
    QString password = StringUtil::GetRandomPassword(m_characterTypes, 10);
    ui.leEncodeTarget->setText(password);
    ui.leShiftTarget->setText(password);
}
