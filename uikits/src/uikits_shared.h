#ifndef UIKITS_SHARED_H
#define UIKITS_SHARED_H

#include <QtCore/QtGlobal>

#if defined(UIKITS_SHARED_EXPORTS)
#  define UIKITS_SHARED Q_DECL_EXPORT
#elif defined (UIKITS_SHARED_IMPORTS)
#  define UIKITS_SHARED Q_DECL_IMPORT
#else
#  define UIKITS_SHARED   /**/
#endif

#endif // UIKITS_SHARED_H

