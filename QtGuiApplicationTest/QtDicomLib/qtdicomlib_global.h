#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(QTDICOMLIB_LIB)
#  define QTDICOMLIB_EXPORT Q_DECL_EXPORT
# else
#  define QTDICOMLIB_EXPORT Q_DECL_IMPORT
# endif
#else
# define QTDICOMLIB_EXPORT
#endif
