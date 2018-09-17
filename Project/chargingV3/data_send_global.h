#ifndef DATA_SEND_GLOBAL_H
#define DATA_SEND_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef DATA_SEND_LIB
# define DATA_SEND_EXPORT Q_DECL_EXPORT
#else
# define DATA_SEND_EXPORT Q_DECL_IMPORT
#endif

#endif // DATA_SEND_GLOBAL_H
