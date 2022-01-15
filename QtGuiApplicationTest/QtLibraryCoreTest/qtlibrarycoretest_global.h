#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(QTLIBRARYCORETEST_LIB)
#  define QTLIBRARYCORETEST_EXPORT Q_DECL_EXPORT
# else
#  define QTLIBRARYCORETEST_EXPORT Q_DECL_IMPORT
# endif
#else
# define QTLIBRARYCORETEST_EXPORT
#endif
