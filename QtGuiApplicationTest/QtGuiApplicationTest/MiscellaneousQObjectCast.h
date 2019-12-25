#ifndef MISCELLANEOUSQ_OBJECT_CAST_H
#define MISCELLANEOUSQ_OBJECT_CAST_H

#include "MiscellaneousBase.h"
#include <QSharedPointer>

namespace Ui { class MiscellaneousQObjectCast; };

class CastBaseQObject;
class CastBaseQObjectExtra;

class DllCastBaseQObject;
class DllCastDeriveQObject;
class DllCastExtra;
class DllCastBaseQObjectExtra;
class DllCastDeriveQObjectExtra;

class MiscellaneousQObjectCast : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousQObjectCast(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousQObjectCast();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

    private slots:
    void on_btnSameModule_clicked();
    void on_btnSameModuleExtra_clicked();
    void on_btnDllQObject_clicked();
    void on_btnDllQObjectExtra_clicked();

private:
    Ui::MiscellaneousQObjectCast *ui;
    CastBaseQObject                        *m_pBase;
    CastBaseQObjectExtra                   *m_pBaseExtra;
    QSharedPointer<CastBaseQObject>         m_spBase;
    QSharedPointer<CastBaseQObjectExtra>    m_spBaseExtra;
    DllCastBaseQObject                     *m_pDllBase;
    DllCastBaseQObjectExtra                *m_pDllBaseExtra;
    QSharedPointer<DllCastBaseQObject>      m_spDllBase;
    QSharedPointer<DllCastBaseQObjectExtra> m_spDllBaseExtra;
};

#endif // !MISCELLANEOUSQ_OBJECT_CAST_H

class CastBaseQObject : public QObject
{
    Q_OBJECT

public:
    CastBaseQObject()
    {
    }

    virtual ~CastBaseQObject()
    {
    }
};

class CastDeriveQObject : public CastBaseQObject
{
    Q_OBJECT

public:
    CastDeriveQObject()
    {
    }

    virtual ~CastDeriveQObject()
    {
    }
};

class CastExtra
{
public:
    CastExtra()
    {}

    ~CastExtra()
    {}
};

class CastBaseQObjectExtra : public QObject, public CastExtra
{
    Q_OBJECT

public:
    CastBaseQObjectExtra()
    {
    }

    virtual ~CastBaseQObjectExtra()
    {
    }
};

class CastDeriveQObjectExtra : public CastBaseQObjectExtra
{
    Q_OBJECT

public:
    CastDeriveQObjectExtra()
    {
    }

    virtual ~CastDeriveQObjectExtra()
    {
    }
};
