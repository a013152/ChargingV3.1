/****************************************************************************
** Meta object code from reading C++ file 'CommandQueue.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../CommandQueue.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CommandQueue.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CCommandQueue_t {
    QByteArrayData data[10];
    char stringdata[101];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CCommandQueue_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CCommandQueue_t qt_meta_stringdata_CCommandQueue = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 6),
QT_MOC_LITERAL(2, 21, 0),
QT_MOC_LITERAL(3, 22, 7),
QT_MOC_LITERAL(4, 30, 11),
QT_MOC_LITERAL(5, 42, 6),
QT_MOC_LITERAL(6, 49, 8),
QT_MOC_LITERAL(7, 58, 11),
QT_MOC_LITERAL(8, 70, 20),
QT_MOC_LITERAL(9, 91, 9)
    },
    "CCommandQueue\0readed\0\0strType\0recvContend\0"
    "iError\0printfed\0writContend\0"
    "readyGetBatteryState\0nClosetId"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CCommandQueue[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   29,    2, 0x06 /* Public */,
       6,    1,   36,    2, 0x06 /* Public */,
       8,    1,   39,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int,    3,    4,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::Int,    9,

       0        // eod
};

void CCommandQueue::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CCommandQueue *_t = static_cast<CCommandQueue *>(_o);
        switch (_id) {
        case 0: _t->readed((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: _t->printfed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->readyGetBatteryState((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CCommandQueue::*_t)(QString , QString , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CCommandQueue::readed)) {
                *result = 0;
            }
        }
        {
            typedef void (CCommandQueue::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CCommandQueue::printfed)) {
                *result = 1;
            }
        }
        {
            typedef void (CCommandQueue::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CCommandQueue::readyGetBatteryState)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject CCommandQueue::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CCommandQueue.data,
      qt_meta_data_CCommandQueue,  qt_static_metacall, 0, 0}
};


const QMetaObject *CCommandQueue::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CCommandQueue::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CCommandQueue.stringdata))
        return static_cast<void*>(const_cast< CCommandQueue*>(this));
    return QThread::qt_metacast(_clname);
}

int CCommandQueue::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void CCommandQueue::readed(QString _t1, QString _t2, int _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CCommandQueue::printfed(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CCommandQueue::readyGetBatteryState(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
