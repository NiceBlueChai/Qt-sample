#pragma once

#include <QtGlobal>

#ifndef NBC_NAMESPACE


#   define NBC_BEGIN_NAMESPACE
#   define NBC_END_NAMESPACE

#else

#   define NBC_BEGIN_NAMESPACE namespace NBC_NAMESPACE {
#   define NBC_END_NAMESPACE }

#endif // ifndef NBC_NAMESPACE


#if defined(NBC_BUILD_LIB)
#   define NBC_EXPORT Q_DECL_EXPORT
#else
#   define NBC_EXPORT Q_DECL_IMPORT
#endif


#ifdef NBC_NAMESPACE
namespace nbc {
#else
class nbc {
public:
#endif


#ifdef NBC_NAMESPACE
};
#else
};
#endif
