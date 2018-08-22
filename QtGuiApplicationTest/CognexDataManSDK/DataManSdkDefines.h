#ifndef DATAMAN_SDK_DEFINES_H
#define DATAMAN_SDK_DEFINES_H

#ifndef COGNEX_EXPORT
#if defined(COGNEX_SHARED_DLL)
#define COGNEX_EXPORT __declspec(dllexport)
#else
#define COGNEX_EXPORT __declspec(dllimport)
#endif // COGNEX_SHARED_DLL
#endif // COGNEX_EXPORT

#endif // DATAMAN_SDK_DEFINES_H
