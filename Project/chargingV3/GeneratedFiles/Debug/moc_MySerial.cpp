/****************************************************************************
** Meta object code from reading C++ file 'MySerial.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../MySerial.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MySerial.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MySerial_t {
    QByteArrayData data[14];
    char stringdata[127];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MySerial_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MySerial_t qt_meta_stringdata_MySerial = {
    {
QT_MOC_LITERAL(0, 0, 8),
QT_MOC_LITERAL(1, 9, 9),
QT_MOC_LITERAL(2, 19, 0),
QT_MOC_LITERAL(3, 20, 10),
QT_MOC_LITERAL(4, 31, 4),
QT_MOC_LITERAL(5, 36, 7),
QT_MOC_LITERAL(6, 44, 10),
QT_MOC_LITERAL(7, 55, 10),
QT_MOC_LITERAL(8, 66, 9),
QT_MOC_LITERAL(9, 76, 11),
QT_MOC_LITERAL(10, 88, 11),
QT_MOC_LITERAL(11, 100, 7),
QT_MOC_LITERAL(12, 108, 10),
QT_MOC_LITERAL(13, 119, 7)
    },
    "MySerial\0readReady\0\0serialOpen\0port\0"
    "bitRate\0dataLength\0stopLength\0parityNum\0"
    "serialClose\0serialWrite\0message\0"
    "serialRead\0readNow"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MySerial[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    5,   70,    2, 0x0a /* Public */,
       3,    4,   81,    2, 0x2a /* Public | MethodCloned */,
       3,    3,   90,    2, 0x2a /* Public | MethodCloned */,
       3,    2,   97,    2, 0x2a /* Public | MethodCloned */,
       3,    1,  102,    2, 0x2a /* Public | MethodCloned */,
       3,    0,  105,    2, 0x2a /* Public | MethodCloned */,
       9,    0,  106,    2, 0x0a /* Public */,
      10,    1,  107,    2, 0x0a /* Public */,
      12,    0,  110,    2, 0x0a /* Public */,
      13,    0,  111,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    4,    5,    6,    7,    8,
    QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::QString,    4,    5,    6,    7,
    QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::QString,    4,    5,    6,
    QMetaType::Int, QMetaType::QString, QMetaType::QString,    4,    5,
    QMetaType::Int, QMetaType::QString,    4,
    QMetaType::Int,
    QMetaType::Int,
    QMetaType::Int, QMetaType::QByteArray,   11,
    QMetaType::QByteArray,
    QMetaType::Void,

       0        // eod
};

void MySerial::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MySerial *_t = static_cast<MySerial *>(_o);
        switch (_id) {
        case 0: _t->readReady(); break;
        case 1: { int _r = _t->serialOpen((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])),(*reinterpret_cast< QString(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: { int _r = _t->serialOpen((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< QString(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: { int _r = _t->serialOpen((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: { int _r = _t->serialOpen((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 5: { int _r = _t->serialOpen((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 6: { int _r = _t->serialOpen();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 7: { int _r = _t->serialClose();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 8: { int _r = _t->serialWrite((*reinterpret_cast< QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 9: { QByteArray _r = _t->serialRead();
            if (_a[0]) *reinterpret_cast< QByteArray*>(_a[0]) = _r; }  break;
        case 10: _t->readNow(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MySerial::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MySerial::readReady)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject MySerial::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MySerial.data,
      qt_meta_data_MySerial,  qt_static_metacall, 0, 0}
};


const QMetaObject *MySerial::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MySerial::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MySerial.stringdata))
        return static_cast<void*>(const_cast< MySerial*>(this));
    return QObject::qt_metacast(_clname);
}

int MySerial::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void MySerial::readReady()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
