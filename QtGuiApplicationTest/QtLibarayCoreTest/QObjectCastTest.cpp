#include "QObjectCastTest.h"

DllCastBaseQObject::DllCastBaseQObject()
{
}

DllCastBaseQObject::~DllCastBaseQObject()
{
}

DllCastDeriveQObject::DllCastDeriveQObject()
{
}

DllCastDeriveQObject::~DllCastDeriveQObject()
{
}
DllCastExtra::DllCastExtra()
{}

DllCastExtra::~DllCastExtra()
{}

DllCastBaseQObjectExtra::DllCastBaseQObjectExtra()
{
}

DllCastBaseQObjectExtra::~DllCastBaseQObjectExtra()
{
}

void DllCastBaseQObjectExtra::TestFunc()
{
	int i = 0;
}

DllCastDeriveQObjectExtra::DllCastDeriveQObjectExtra()
{
}

DllCastDeriveQObjectExtra::~DllCastDeriveQObjectExtra()
{
}
