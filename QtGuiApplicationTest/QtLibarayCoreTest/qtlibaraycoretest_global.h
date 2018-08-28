#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(QTLIBARAYCORETEST_LIB)
#  define QTLIBARAYCORETEST_EXPORT Q_DECL_EXPORT
# else
#  define QTLIBARAYCORETEST_EXPORT Q_DECL_IMPORT
# endif
#else
# define QTLIBARAYCORETEST_EXPORT
#endif
