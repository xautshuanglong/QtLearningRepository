#pragma once

#include "qtlibrarycoretest_global.h"
#include <QObject>

class QTLIBRARYCORETEST_EXPORT DllCastBaseQObject : public QObject
{
	Q_OBJECT

public:
	DllCastBaseQObject();
	virtual ~DllCastBaseQObject();
};

class QTLIBRARYCORETEST_EXPORT DllCastDeriveQObject : public DllCastBaseQObject
{
	Q_OBJECT

public:
	DllCastDeriveQObject();
	virtual ~DllCastDeriveQObject();
};

class QTLIBRARYCORETEST_EXPORT DllCastExtra
{
public:
	DllCastExtra();
	virtual ~DllCastExtra();
};

class QTLIBRARYCORETEST_EXPORT DllCastExtraInterface
{
public:
	DllCastExtraInterface() {}
	virtual ~DllCastExtraInterface() {}
	virtual void TestFunc() = 0;
};

class QTLIBRARYCORETEST_EXPORT DllCastBaseQObjectExtra : public QObject, public DllCastExtraInterface
{
	Q_OBJECT

public:
	DllCastBaseQObjectExtra();
	virtual ~DllCastBaseQObjectExtra();

	virtual void TestFunc() override;
};

class QTLIBRARYCORETEST_EXPORT DllCastDeriveQObjectExtra : public DllCastBaseQObjectExtra
{
	Q_OBJECT

public:
	DllCastDeriveQObjectExtra();
	virtual ~DllCastDeriveQObjectExtra();

	virtual void TestFunc() override;
};
