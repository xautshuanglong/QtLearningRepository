#include "DebugEvent.h"

const QEvent::Type DebugEvent::mgEventType = (QEvent::Type)QEvent::registerEventType(QEvent::User + 1);

DebugEvent::DebugEvent()
    : QEvent(mgEventType)
{
}

DebugEvent::~DebugEvent()
{
}

QEvent::Type DebugEvent::StaticEventType()
{
    return mgEventType;
}
