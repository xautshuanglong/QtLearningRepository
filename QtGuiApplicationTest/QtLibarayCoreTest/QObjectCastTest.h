#pragma once

#include "qtlibaraycoretest_global.h"
#include <QObject>

class QTLIBARAYCORETEST_EXPORT DllCastBaseQObject : public QObject
{
	Q_OBJECT

public:
	DllCastBaseQObject();
	virtual ~DllCastBaseQObject();
};

class QTLIBARAYCORETEST_EXPORT DllCastDeriveQObject : public DllCastBaseQObject
{
	Q_OBJECT

public:
	DllCastDeriveQObject();
	virtual ~DllCastDeriveQObject();
};

class QTLIBARAYCORETEST_EXPORT DllCastExtra
{
public:
	DllCastExtra();
	virtual ~DllCastExtra();
};

class QTLIBARAYCORETEST_EXPORT DllCastExtraInterface
{
public:
	DllCastExtraInterface() {}
	virtual ~DllCastExtraInterface() {}
	virtual void TestFunc() = 0;
};

class QTLIBARAYCORETEST_EXPORT DllCastBaseQObjectExtra : public QObject, public DllCastExtraInterface
{
	Q_OBJECT

public:
	DllCastBaseQObjectExtra();
	virtual ~DllCastBaseQObjectExtra();

	virtual void TestFunc() override;
};

class QTLIBARAYCORETEST_EXPORT DllCastDeriveQObjectExtra : public DllCastBaseQObjectExtra
{
	Q_OBJECT

public:
	DllCastDeriveQObjectExtra();
	virtual ~DllCastDeriveQObjectExtra();
};
