/****************************************************************************
** Meta object code from reading C++ file 'DoubleClickedButton.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../DoubleClickedButton.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DoubleClickedButton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DoubleClickedButton_t {
    QByteArrayData data[6];
    char stringdata[99];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DoubleClickedButton_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DoubleClickedButton_t qt_meta_stringdata_DoubleClickedButton = {
    {
QT_MOC_LITERAL(0, 0, 19),
QT_MOC_LITERAL(1, 20, 13),
QT_MOC_LITERAL(2, 34, 0),
QT_MOC_LITERAL(3, 35, 13),
QT_MOC_LITERAL(4, 49, 24),
QT_MOC_LITERAL(5, 74, 24)
    },
    "DoubleClickedButton\0singleClicked\0\0"
    "doubleClicked\0timer_out_single_clicked\0"
    "timer_out_double_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DoubleClickedButton[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    0,   35,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   36,    2, 0x08 /* Private */,
       5,    0,   37,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void DoubleClickedButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DoubleClickedButton *_t = static_cast<DoubleClickedButton *>(_o);
        switch (_id) {
        case 0: _t->singleClicked(); break;
        case 1: _t->doubleClicked(); break;
        case 2: _t->timer_out_single_clicked(); break;
        case 3: _t->timer_out_double_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DoubleClickedButton::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DoubleClickedButton::singleClicked)) {
                *result = 0;
            }
        }
        {
            typedef void (DoubleClickedButton::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DoubleClickedButton::doubleClicked)) {
                *result = 1;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject DoubleClickedButton::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_DoubleClickedButton.data,
      qt_meta_data_DoubleClickedButton,  qt_static_metacall, 0, 0}
};


const QMetaObject *DoubleClickedButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DoubleClickedButton::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DoubleClickedButton.stringdata))
        return static_cast<void*>(const_cast< DoubleClickedButton*>(this));
    return QPushButton::qt_metacast(_clname);
}

int DoubleClickedButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void DoubleClickedButton::singleClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void DoubleClickedButton::doubleClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
