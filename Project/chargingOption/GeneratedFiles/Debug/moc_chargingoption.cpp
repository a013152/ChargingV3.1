/****************************************************************************
** Meta object code from reading C++ file 'chargingoption.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../chargingoption.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chargingoption.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_chargingOption_t {
    QByteArrayData data[14];
    char stringdata[242];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_chargingOption_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_chargingOption_t qt_meta_stringdata_chargingOption = {
    {
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 17),
QT_MOC_LITERAL(2, 33, 0),
QT_MOC_LITERAL(3, 34, 22),
QT_MOC_LITERAL(4, 57, 18),
QT_MOC_LITERAL(5, 76, 23),
QT_MOC_LITERAL(6, 100, 13),
QT_MOC_LITERAL(7, 114, 18),
QT_MOC_LITERAL(8, 133, 17),
QT_MOC_LITERAL(9, 151, 15),
QT_MOC_LITERAL(10, 167, 17),
QT_MOC_LITERAL(11, 185, 16),
QT_MOC_LITERAL(12, 202, 18),
QT_MOC_LITERAL(13, 221, 20)
    },
    "chargingOption\0cellchangeBattery\0\0"
    "cellchangeBatteryModel\0cellChangedBattery\0"
    "cellChangedBatteryModel\0srocllBattery\0"
    "srocllBatteryModel\0textChangeBattery\0"
    "textChangeDrone\0textChangeBalance\0"
    "textChangeCloset\0textChangeScanTime\0"
    "textChangeSubmitTime"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_chargingOption[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    4,   74,    2, 0x0a /* Public */,
       3,    4,   83,    2, 0x0a /* Public */,
       4,    2,   92,    2, 0x0a /* Public */,
       5,    2,   97,    2, 0x0a /* Public */,
       6,    1,  102,    2, 0x0a /* Public */,
       7,    1,  105,    2, 0x0a /* Public */,
       8,    1,  108,    2, 0x0a /* Public */,
       9,    1,  111,    2, 0x0a /* Public */,
      10,    1,  114,    2, 0x0a /* Public */,
      11,    1,  117,    2, 0x0a /* Public */,
      12,    1,  120,    2, 0x0a /* Public */,
      13,    1,  123,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void chargingOption::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        chargingOption *_t = static_cast<chargingOption *>(_o);
        switch (_id) {
        case 0: _t->cellchangeBattery((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 1: _t->cellchangeBatteryModel((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 2: _t->cellChangedBattery((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->cellChangedBatteryModel((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->srocllBattery((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->srocllBatteryModel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->textChangeBattery((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->textChangeDrone((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->textChangeBalance((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->textChangeCloset((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->textChangeScanTime((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->textChangeSubmitTime((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject chargingOption::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_chargingOption.data,
      qt_meta_data_chargingOption,  qt_static_metacall, 0, 0}
};


const QMetaObject *chargingOption::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *chargingOption::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_chargingOption.stringdata))
        return static_cast<void*>(const_cast< chargingOption*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int chargingOption::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
