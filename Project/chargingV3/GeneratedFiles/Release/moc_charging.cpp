/****************************************************************************
** Meta object code from reading C++ file 'charging.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../charging.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'charging.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CConnectDBThread_t {
    QByteArrayData data[3];
    char stringdata[34];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CConnectDBThread_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CConnectDBThread_t qt_meta_stringdata_CConnectDBThread = {
    {
QT_MOC_LITERAL(0, 0, 16),
QT_MOC_LITERAL(1, 17, 15),
QT_MOC_LITERAL(2, 33, 0)
    },
    "CConnectDBThread\0connectDBResult\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CConnectDBThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void CConnectDBThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CConnectDBThread *_t = static_cast<CConnectDBThread *>(_o);
        switch (_id) {
        case 0: _t->connectDBResult((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CConnectDBThread::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CConnectDBThread::connectDBResult)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CConnectDBThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CConnectDBThread.data,
      qt_meta_data_CConnectDBThread,  qt_static_metacall, 0, 0}
};


const QMetaObject *CConnectDBThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CConnectDBThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CConnectDBThread.stringdata))
        return static_cast<void*>(const_cast< CConnectDBThread*>(this));
    return QThread::qt_metacast(_clname);
}

int CConnectDBThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void CConnectDBThread::connectDBResult(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_charging_t {
    QByteArrayData data[67];
    char stringdata[935];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_charging_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_charging_t qt_meta_stringdata_charging = {
    {
QT_MOC_LITERAL(0, 0, 8),
QT_MOC_LITERAL(1, 9, 7),
QT_MOC_LITERAL(2, 17, 0),
QT_MOC_LITERAL(3, 18, 12),
QT_MOC_LITERAL(4, 31, 13),
QT_MOC_LITERAL(5, 45, 4),
QT_MOC_LITERAL(6, 50, 5),
QT_MOC_LITERAL(7, 56, 19),
QT_MOC_LITERAL(8, 76, 9),
QT_MOC_LITERAL(9, 86, 10),
QT_MOC_LITERAL(10, 97, 9),
QT_MOC_LITERAL(11, 107, 5),
QT_MOC_LITERAL(12, 113, 13),
QT_MOC_LITERAL(13, 127, 21),
QT_MOC_LITERAL(14, 149, 22),
QT_MOC_LITERAL(15, 172, 15),
QT_MOC_LITERAL(16, 188, 14),
QT_MOC_LITERAL(17, 203, 15),
QT_MOC_LITERAL(18, 219, 11),
QT_MOC_LITERAL(19, 231, 8),
QT_MOC_LITERAL(20, 240, 14),
QT_MOC_LITERAL(21, 255, 6),
QT_MOC_LITERAL(22, 262, 15),
QT_MOC_LITERAL(23, 278, 24),
QT_MOC_LITERAL(24, 303, 9),
QT_MOC_LITERAL(25, 313, 10),
QT_MOC_LITERAL(26, 324, 10),
QT_MOC_LITERAL(27, 335, 6),
QT_MOC_LITERAL(28, 342, 28),
QT_MOC_LITERAL(29, 371, 23),
QT_MOC_LITERAL(30, 395, 14),
QT_MOC_LITERAL(31, 410, 14),
QT_MOC_LITERAL(32, 425, 13),
QT_MOC_LITERAL(33, 439, 23),
QT_MOC_LITERAL(34, 463, 12),
QT_MOC_LITERAL(35, 476, 14),
QT_MOC_LITERAL(36, 491, 8),
QT_MOC_LITERAL(37, 500, 15),
QT_MOC_LITERAL(38, 516, 9),
QT_MOC_LITERAL(39, 526, 21),
QT_MOC_LITERAL(40, 548, 14),
QT_MOC_LITERAL(41, 563, 15),
QT_MOC_LITERAL(42, 579, 9),
QT_MOC_LITERAL(43, 589, 20),
QT_MOC_LITERAL(44, 610, 9),
QT_MOC_LITERAL(45, 620, 21),
QT_MOC_LITERAL(46, 642, 11),
QT_MOC_LITERAL(47, 654, 26),
QT_MOC_LITERAL(48, 681, 17),
QT_MOC_LITERAL(49, 699, 5),
QT_MOC_LITERAL(50, 705, 4),
QT_MOC_LITERAL(51, 710, 7),
QT_MOC_LITERAL(52, 718, 13),
QT_MOC_LITERAL(53, 732, 13),
QT_MOC_LITERAL(54, 746, 17),
QT_MOC_LITERAL(55, 764, 11),
QT_MOC_LITERAL(56, 776, 15),
QT_MOC_LITERAL(57, 792, 8),
QT_MOC_LITERAL(58, 801, 15),
QT_MOC_LITERAL(59, 817, 12),
QT_MOC_LITERAL(60, 830, 16),
QT_MOC_LITERAL(61, 847, 16),
QT_MOC_LITERAL(62, 864, 17),
QT_MOC_LITERAL(63, 882, 17),
QT_MOC_LITERAL(64, 900, 12),
QT_MOC_LITERAL(65, 913, 7),
QT_MOC_LITERAL(66, 921, 13)
    },
    "charging\0ExitApp\0\0RefreshState\0"
    "enRefreshType\0type\0index\0AddCommamdIntoQueue\0"
    "stCommand\0strCommand\0refreshUI\0strID\0"
    "OnBtnSysClose\0OnAddCommamdIntoQueue\0"
    "OnReadyGetBatteryState\0OnClickClosetNo\0"
    "OnClickPrePage\0OnClickNextPage\0"
    "serial_scan\0QAction*\0OnClickMenuCom\0"
    "action\0onOpenOptionWin\0OnClickMenuDebugInfoLeve\0"
    "timer_out\0readSerial\0strContent\0iError\0"
    "OnBtnChargingOrStopCharging1\0"
    "OnBtnDisChargingOrStop1\0updateTextEdit\0"
    "detectTextEdit\0detectRecvXOR\0"
    "getBatteryIdRelatedInfo\0strBatteryID\0"
    "MAP_CLOSET_IT&\0itCloset\0MAP_BATTERY_IT&\0"
    "itBattery\0MAP_BATTERY_MODEL_IT&\0"
    "itBatteryModel\0MAP_CHARGER_IT&\0itCharger\0"
    "chargerIDtoBatteryId\0chargerId\0"
    "batteryIDtoArrayIndex\0onRefreshUI\0"
    "releaseBatteryStateRefresh\0chargingByLocalID\0"
    "strId\0int*\0iResult\0showDebugInfo\0"
    "stopByLocalID\0createClosetRadio\0"
    "createMenus\0createChargGrid\0adjustUI\0"
    "getBatteryState\0indexBattery\0"
    "testConnectMysql\0refreshCurrentUI\0"
    "allClosetCharging\0oneClosetCharging\0"
    "onPauseScand\0checked\0onPauseSubmit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_charging[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      40,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  214,    2, 0x06 /* Public */,
       3,    2,  215,    2, 0x06 /* Public */,
       7,    1,  220,    2, 0x06 /* Public */,
      10,    1,  223,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,  226,    2, 0x0a /* Public */,
      13,    1,  227,    2, 0x0a /* Public */,
      14,    1,  230,    2, 0x0a /* Public */,
      15,    0,  233,    2, 0x08 /* Private */,
      16,    0,  234,    2, 0x08 /* Private */,
      17,    0,  235,    2, 0x08 /* Private */,
      18,    0,  236,    2, 0x08 /* Private */,
      20,    1,  237,    2, 0x08 /* Private */,
      22,    0,  240,    2, 0x08 /* Private */,
      23,    1,  241,    2, 0x08 /* Private */,
      24,    0,  244,    2, 0x08 /* Private */,
      25,    3,  245,    2, 0x08 /* Private */,
      28,    0,  252,    2, 0x08 /* Private */,
      29,    0,  253,    2, 0x08 /* Private */,
      30,    1,  254,    2, 0x08 /* Private */,
      31,    0,  257,    2, 0x08 /* Private */,
      32,    1,  258,    2, 0x08 /* Private */,
      33,    5,  261,    2, 0x08 /* Private */,
      43,    1,  272,    2, 0x08 /* Private */,
      45,    1,  275,    2, 0x08 /* Private */,
      46,    1,  278,    2, 0x08 /* Private */,
      47,    0,  281,    2, 0x08 /* Private */,
      48,    3,  282,    2, 0x08 /* Private */,
      48,    2,  289,    2, 0x28 /* Private | MethodCloned */,
      53,    1,  294,    2, 0x08 /* Private */,
      54,    0,  297,    2, 0x08 /* Private */,
      55,    0,  298,    2, 0x08 /* Private */,
      56,    0,  299,    2, 0x08 /* Private */,
      57,    0,  300,    2, 0x08 /* Private */,
      58,    1,  301,    2, 0x08 /* Private */,
      60,    0,  304,    2, 0x08 /* Private */,
      61,    0,  305,    2, 0x08 /* Private */,
      62,    0,  306,    2, 0x0a /* Public */,
      63,    0,  307,    2, 0x0a /* Public */,
      64,    1,  308,    2, 0x0a /* Public */,
      66,    1,  311,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, QMetaType::Int,    5,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QString,   11,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 19,
    QMetaType::Void, 0x80000000 | 19,   21,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,   21,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int,    5,   26,   27,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString,   26,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 35, 0x80000000 | 37, 0x80000000 | 39, 0x80000000 | 41,   34,   36,   38,   40,   42,
    QMetaType::Int, QMetaType::Int,   44,
    QMetaType::Int, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 50, QMetaType::Bool,   49,   51,   52,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 50,   49,   51,
    QMetaType::Bool, QMetaType::QString,   49,
    QMetaType::Void,
    0x80000000 | 19,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QString, QMetaType::Int,   59,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   65,
    QMetaType::Void, QMetaType::Bool,   65,

       0        // eod
};

void charging::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        charging *_t = static_cast<charging *>(_o);
        switch (_id) {
        case 0: _t->ExitApp(); break;
        case 1: _t->RefreshState((*reinterpret_cast< enRefreshType(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->AddCommamdIntoQueue((*reinterpret_cast< stCommand(*)>(_a[1]))); break;
        case 3: _t->refreshUI((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->OnBtnSysClose(); break;
        case 5: _t->OnAddCommamdIntoQueue((*reinterpret_cast< stCommand(*)>(_a[1]))); break;
        case 6: _t->OnReadyGetBatteryState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->OnClickClosetNo(); break;
        case 8: _t->OnClickPrePage(); break;
        case 9: _t->OnClickNextPage(); break;
        case 10: { QAction* _r = _t->serial_scan();
            if (_a[0]) *reinterpret_cast< QAction**>(_a[0]) = _r; }  break;
        case 11: _t->OnClickMenuCom((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 12: _t->onOpenOptionWin(); break;
        case 13: _t->OnClickMenuDebugInfoLeve((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 14: _t->timer_out(); break;
        case 15: _t->readSerial((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 16: _t->OnBtnChargingOrStopCharging1(); break;
        case 17: _t->OnBtnDisChargingOrStop1(); break;
        case 18: _t->updateTextEdit((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 19: _t->detectTextEdit(); break;
        case 20: { bool _r = _t->detectRecvXOR((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 21: { bool _r = _t->getBatteryIdRelatedInfo((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< MAP_CLOSET_IT(*)>(_a[2])),(*reinterpret_cast< MAP_BATTERY_IT(*)>(_a[3])),(*reinterpret_cast< MAP_BATTERY_MODEL_IT(*)>(_a[4])),(*reinterpret_cast< MAP_CHARGER_IT(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 22: { int _r = _t->chargerIDtoBatteryId((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 23: { int _r = _t->batteryIDtoArrayIndex((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 24: _t->onRefreshUI((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 25: _t->releaseBatteryStateRefresh(); break;
        case 26: { bool _r = _t->chargingByLocalID((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 27: { bool _r = _t->chargingByLocalID((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 28: { bool _r = _t->stopByLocalID((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 29: _t->createClosetRadio(); break;
        case 30: { QAction* _r = _t->createMenus();
            if (_a[0]) *reinterpret_cast< QAction**>(_a[0]) = _r; }  break;
        case 31: _t->createChargGrid(); break;
        case 32: _t->adjustUI(); break;
        case 33: { QString _r = _t->getBatteryState((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 34: _t->testConnectMysql(); break;
        case 35: _t->refreshCurrentUI(); break;
        case 36: _t->allClosetCharging(); break;
        case 37: _t->oneClosetCharging(); break;
        case 38: _t->onPauseScand((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 39: _t->onPauseSubmit((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 11:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        case 13:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (charging::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&charging::ExitApp)) {
                *result = 0;
            }
        }
        {
            typedef void (charging::*_t)(enRefreshType , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&charging::RefreshState)) {
                *result = 1;
            }
        }
        {
            typedef void (charging::*_t)(stCommand );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&charging::AddCommamdIntoQueue)) {
                *result = 2;
            }
        }
        {
            typedef void (charging::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&charging::refreshUI)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject charging::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_charging.data,
      qt_meta_data_charging,  qt_static_metacall, 0, 0}
};


const QMetaObject *charging::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *charging::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_charging.stringdata))
        return static_cast<void*>(const_cast< charging*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int charging::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 40)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 40;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 40)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 40;
    }
    return _id;
}

// SIGNAL 0
void charging::ExitApp()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void charging::RefreshState(enRefreshType _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void charging::AddCommamdIntoQueue(stCommand _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void charging::refreshUI(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
