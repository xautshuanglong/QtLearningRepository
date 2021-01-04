#include "MiscellaneousCustomTextEdit.h"

#include <windows.h>

MiscellaneousCustomTextEdit::MiscellaneousCustomTextEdit(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousCustomTextEdit::~MiscellaneousCustomTextEdit()
{
}

QString MiscellaneousCustomTextEdit::GetTitle()
{
    return QObject::tr("Custom TextEdit");
}

QString MiscellaneousCustomTextEdit::GetTitleTooltip()
{
    return QObject::tr("Custom text edit for right click menu.");
}

MiscellaneousTestGroup MiscellaneousCustomTextEdit::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousCustomTextEdit::GetItemID()
{
    return MiscellaneousTestItem::QT_Custom_TextEdit;
}
 
void MiscellaneousCustomTextEdit::paintEvent(QPaintEvent* pEvent)
{
    QWidget::paintEvent(pEvent);
}

void MiscellaneousCustomTextEdit::on_btnAppendText_clicked()
{
    //DWORD fileAttributes = ::GetFileAttributesA("E:/Temp/CreateFileA_Test/");
    ////HANDLE hDevice = ::CreateFileA("E:/test.json", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    //HANDLE hDevice = ::CreateFileA("E:\\Temp\\CreateFileA_Test", GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    ////HANDLE hDevice = ::CreateFileA("\\\\.\\PHYSICALDRIVE0", GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    //if (hDevice == INVALID_HANDLE_VALUE)
    //{
    //    DWORD resCode = ::GetLastError();
    //    int i = 0;
    //}
    //else
    //{
    //    ::CloseHandle(hDevice);
    //}
    //return;

    HWND pHwnd = NULL;
    HWND ppHwnd = NULL;
    HWND pppHwnd = NULL;
    HWND ppppHwnd = NULL;
    HWND pppppHwnd = NULL;
    HWND hwnd = (HWND)this->winId();
    QString pObjName;
    QString ppObjName;
    QString pppObjName;
    QString ppppObjName;
    QString pppppObjName;
    QString objName = this->objectName();
    if (this->parentWidget())
    {
        pHwnd = (HWND)this->parentWidget()->winId();
        pObjName = this->parentWidget()->objectName();

        if (this->parentWidget()->parentWidget())
        {
            ppHwnd = (HWND)this->parentWidget()->parentWidget()->winId();
            ppObjName = this->parentWidget()->parentWidget()->objectName();

            if (this->parentWidget()->parentWidget()->parentWidget())
            {
                pppHwnd = (HWND)this->parentWidget()->parentWidget()->parentWidget()->winId();
                pppObjName= this->parentWidget()->parentWidget()->parentWidget()->objectName();

                if (this->parentWidget()->parentWidget()->parentWidget()->parentWidget())
                {
                    ppppHwnd = (HWND)this->parentWidget()->parentWidget()->parentWidget()->parentWidget()->winId();
                    pppppObjName = this->parentWidget()->parentWidget()->parentWidget()->parentWidget()->objectName();

                    if (this->parentWidget()->parentWidget()->parentWidget()->parentWidget()->parentWidget())
                    {
                        pppppHwnd = (HWND)this->parentWidget()->parentWidget()->parentWidget()->parentWidget()->parentWidget()->winId();
                        pppppObjName = this->parentWidget()->parentWidget()->parentWidget()->parentWidget()->parentWidget()->objectName();
                    }
                }
            }
        }
    }
    QWindow *pThisWindow = this->windowHandle();

    QString appendText = ui.leAppendText->text();
    appendText.replace("\\n", "\n");
    appendText.replace("\\t", "\t");
    if (!appendText.isEmpty())
    {
        ui.teOriginal->append(appendText);
        ui.teCustom->append(appendText);
    }
}
