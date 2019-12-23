#include "MiscellaneousQObjectCast.h"
#include "ui_MiscellaneousQObjectCast.h"

#include <QObjectCastTest.h>

MiscellaneousQObjectCast::MiscellaneousQObjectCast(QWidget *parent)
	: MiscellaneousBase(parent)
	, ui(new Ui::MiscellaneousQObjectCast())
{
	ui->setupUi(this);

	m_pBase = new CastDeriveQObject();
	m_pBaseExtra = new CastDeriveQObjectExtra();
	m_spBase = QSharedPointer<CastDeriveQObject>(new CastDeriveQObject);
	m_spBaseExtra = QSharedPointer<CastDeriveQObjectExtra>(new CastDeriveQObjectExtra);

	m_pDllBase = new DllCastDeriveQObject();
	m_pDllBaseExtra = new DllCastDeriveQObjectExtra();
	m_spDllBase = QSharedPointer<DllCastDeriveQObject>(new DllCastDeriveQObject);
	m_spDllBaseExtra = QSharedPointer<DllCastDeriveQObjectExtra>(new DllCastDeriveQObjectExtra);
}

MiscellaneousQObjectCast::~MiscellaneousQObjectCast()
{
	delete ui;
}

void MiscellaneousQObjectCast::on_btnSameModule_clicked()
{
	/*---------------------------- Extend QObject ------------------------*/
	CastDeriveQObject *pDerived = qobject_cast<CastDeriveQObject *>(m_pBase);
	if (Q_NULLPTR != pDerived)
	{
		int i = 0;
	}

	CastDeriveQObject *pDerivedCopy = dynamic_cast<CastDeriveQObject *>(m_pBase);
	if (Q_NULLPTR != pDerivedCopy)
	{
		int i = 0;
	}

	CastDeriveQObject *spDerived = qobject_cast<CastDeriveQObject *>(m_spBase.data());
	if (Q_NULLPTR != spDerived)
	{
		int i = 0;
	}

	CastDeriveQObject *spDerivedCopy = dynamic_cast<CastDeriveQObject *>(m_spBase.data());
	if (Q_NULLPTR != spDerivedCopy)
	{
		int i = 0;
	}

	int i = 0;
}

void MiscellaneousQObjectCast::on_btnSameModuleExtra_clicked()
{
	/*---------------------------- Extend QObject and Extra ------------------------*/
	CastDeriveQObjectExtra *pDerivedExtra = qobject_cast<CastDeriveQObjectExtra *>(m_pBaseExtra);
	if (Q_NULLPTR != pDerivedExtra)
	{
		int i = 0;
	}

	CastDeriveQObjectExtra *pDerivedExtraCopy = dynamic_cast<CastDeriveQObjectExtra *>(m_pBaseExtra);
	if (Q_NULLPTR != pDerivedExtraCopy)
	{
		int i = 0;
	}

	CastDeriveQObjectExtra *spDerivedExtra = qobject_cast<CastDeriveQObjectExtra *>(m_spBaseExtra.data());
	if (Q_NULLPTR != spDerivedExtra)
	{
		int i = 0;
	}

	CastDeriveQObjectExtra *spDerivedExtraCopy = dynamic_cast<CastDeriveQObjectExtra *>(m_spBaseExtra.data());
	if (Q_NULLPTR != spDerivedExtraCopy)
	{
		int i = 0;
	}

	int i = 0;
}

void MiscellaneousQObjectCast::on_btnDllQObject_clicked()
{
	/*---------------------------- Extend QObject ------------------------*/
	DllCastDeriveQObject *pDerived = qobject_cast<DllCastDeriveQObject *>(m_pDllBase);
	if (Q_NULLPTR != pDerived)
	{
		int i = 0;
	}

	DllCastDeriveQObject *pDerivedCopy = dynamic_cast<DllCastDeriveQObject *>(m_pDllBase);
	if (Q_NULLPTR != pDerivedCopy)
	{
		int i = 0;
	}

	DllCastDeriveQObject *spDerived = qobject_cast<DllCastDeriveQObject *>(m_spDllBase.data());
	if (Q_NULLPTR != spDerived)
	{
		int i = 0;
	}

	DllCastDeriveQObject *spDerivedCopy = dynamic_cast<DllCastDeriveQObject *>(m_spDllBase.data());
	if (Q_NULLPTR != spDerivedCopy)
	{
		int i = 0;
	}

	int i = 0;
}

void MiscellaneousQObjectCast::on_btnDllQObjectExtra_clicked()
{
	/*---------------------------- Extend QObject and Extra ------------------------*/
	DllCastDeriveQObjectExtra *pDerivedExtra = qobject_cast<DllCastDeriveQObjectExtra *>(m_pDllBaseExtra);
	if (Q_NULLPTR != pDerivedExtra)
	{
		pDerivedExtra->TestFunc();
		int i = 0;
	}

	DllCastDeriveQObjectExtra *pDerivedExtraCopy = dynamic_cast<DllCastDeriveQObjectExtra *>(m_pDllBaseExtra);
	if (Q_NULLPTR != pDerivedExtraCopy)
	{
		pDerivedExtraCopy->TestFunc();
		int i = 0;
	}

	DllCastDeriveQObjectExtra *spDerivedExtra = qobject_cast<DllCastDeriveQObjectExtra *>(m_spDllBaseExtra.data());
	if (Q_NULLPTR != spDerivedExtra)
	{
		spDerivedExtra->TestFunc();
		int i = 0;
	}

	DllCastDeriveQObjectExtra *spDerivedExtraCopy = dynamic_cast<DllCastDeriveQObjectExtra *>(m_spDllBaseExtra.data());
	if (Q_NULLPTR != spDerivedExtraCopy)
	{
		spDerivedExtraCopy->TestFunc();
		int i = 0;
	}

	int i = 0;
}

QString MiscellaneousQObjectCast::GetTitle()
{
	return QObject::tr("Widget QObject Cast");
}

QString MiscellaneousQObjectCast::GetTitleTooltip()
{
	return QObject::tr("How to cast QObject between parent and child.");
}

MiscellaneousTestGroup MiscellaneousQObjectCast::GetGroupID()
{
	return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousQObjectCast::GetItemID()
{
	return MiscellaneousTestItem::QT_Object_Cast;
}
