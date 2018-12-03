#include "DebugMenuEvent.h"

const QEvent::Type DebugMenuEvent::mgEventType = (QEvent::Type)QEvent::registerEventType(QEvent::User + 1);

DebugMenuEvent::DebugMenuEvent()
    : QEvent(mgEventType)
{
}

DebugMenuEvent::~DebugMenuEvent()
{
}

QEvent::Type DebugMenuEvent::StaticEventType()
{
    return mgEventType;
}
