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
struct qt_meta_stringdata_CThreadShowOrHideDebugInfo_t {
    QByteArrayData data[4];
    char stringdata[50];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CThreadShowOrHideDebugInfo_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CThreadShowOrHideDebugInfo_t qt_meta_stringdata_CThreadShowOrHideDebugInfo = {
    {
QT_MOC_LITERAL(0, 0, 26),
QT_MOC_LITERAL(1, 27, 16),
QT_MOC_LITERAL(2, 44, 0),
QT_MOC_LITERAL(3, 45, 4)
    },
    "CThreadShowOrHideDebugInfo\0signalAdjustRect\0"
    "\0rect"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CThreadShowOrHideDebugInfo[] = {

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
    QMetaType::Void, QMetaType::QRect,    3,

       0        // eod
};

void CThreadShowOrHideDebugInfo::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CThreadShowOrHideDebugInfo *_t = static_cast<CThreadShowOrHideDebugInfo *>(_o);
        switch (_id) {
        case 0: _t->signalAdjustRect((*reinterpret_cast< QRect(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CThreadShowOrHideDebugInfo::*_t)(QRect );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CThreadShowOrHideDebugInfo::signalAdjustRect)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CThreadShowOrHideDebugInfo::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CThreadShowOrHideDebugInfo.data,
      qt_meta_data_CThreadShowOrHideDebugInfo,  qt_static_metacall, 0, 0}
};


const QMetaObject *CThreadShowOrHideDebugInfo::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CThreadShowOrHideDebugInfo::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CThreadShowOrHideDebugInfo.stringdata))
        return static_cast<void*>(const_cast< CThreadShowOrHideDebugInfo*>(this));
    return QThread::qt_metacast(_clname);
}

int CThreadShowOrHideDebugInfo::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CThreadShowOrHideDebugInfo::signalAdjustRect(QRect _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_charging_t {
    QByteArrayData data[82];
    char stringdata[1166];
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
QT_MOC_LITERAL(13, 127, 18),
QT_MOC_LITERAL(14, 146, 18),
QT_MOC_LITERAL(15, 165, 24),
QT_MOC_LITERAL(16, 190, 6),
QT_MOC_LITERAL(17, 197, 21),
QT_MOC_LITERAL(18, 219, 22),
QT_MOC_LITERAL(19, 242, 21),
QT_MOC_LITERAL(20, 264, 4),
QT_MOC_LITERAL(21, 269, 15),
QT_MOC_LITERAL(22, 285, 14),
QT_MOC_LITERAL(23, 300, 15),
QT_MOC_LITERAL(24, 316, 14),
QT_MOC_LITERAL(25, 331, 8),
QT_MOC_LITERAL(26, 340, 6),
QT_MOC_LITERAL(27, 347, 15),
QT_MOC_LITERAL(28, 363, 24),
QT_MOC_LITERAL(29, 388, 17),
QT_MOC_LITERAL(30, 406, 9),
QT_MOC_LITERAL(31, 416, 10),
QT_MOC_LITERAL(32, 427, 10),
QT_MOC_LITERAL(33, 438, 6),
QT_MOC_LITERAL(34, 445, 28),
QT_MOC_LITERAL(35, 474, 23),
QT_MOC_LITERAL(36, 498, 10),
QT_MOC_LITERAL(37, 509, 14),
QT_MOC_LITERAL(38, 524, 14),
QT_MOC_LITERAL(39, 539, 19),
QT_MOC_LITERAL(40, 559, 13),
QT_MOC_LITERAL(41, 573, 23),
QT_MOC_LITERAL(42, 597, 12),
QT_MOC_LITERAL(43, 610, 14),
QT_MOC_LITERAL(44, 625, 8),
QT_MOC_LITERAL(45, 634, 15),
QT_MOC_LITERAL(46, 650, 9),
QT_MOC_LITERAL(47, 660, 21),
QT_MOC_LITERAL(48, 682, 14),
QT_MOC_LITERAL(49, 697, 15),
QT_MOC_LITERAL(50, 713, 9),
QT_MOC_LITERAL(51, 723, 20),
QT_MOC_LITERAL(52, 744, 9),
QT_MOC_LITERAL(53, 754, 21),
QT_MOC_LITERAL(54, 776, 11),
QT_MOC_LITERAL(55, 788, 26),
QT_MOC_LITERAL(56, 815, 17),
QT_MOC_LITERAL(57, 833, 5),
QT_MOC_LITERAL(58, 839, 4),
QT_MOC_LITERAL(59, 844, 7),
QT_MOC_LITERAL(60, 852, 13),
QT_MOC_LITERAL(61, 866, 13),
QT_MOC_LITERAL(62, 880, 17),
QT_MOC_LITERAL(63, 898, 11),
QT_MOC_LITERAL(64, 910, 11),
QT_MOC_LITERAL(65, 922, 15),
QT_MOC_LITERAL(66, 938, 17),
QT_MOC_LITERAL(67, 956, 8),
QT_MOC_LITERAL(68, 965, 15),
QT_MOC_LITERAL(69, 981, 12),
QT_MOC_LITERAL(70, 994, 18),
QT_MOC_LITERAL(71, 1013, 5),
QT_MOC_LITERAL(72, 1019, 6),
QT_MOC_LITERAL(73, 1026, 16),
QT_MOC_LITERAL(74, 1043, 26),
QT_MOC_LITERAL(75, 1070, 8),
QT_MOC_LITERAL(76, 1079, 17),
QT_MOC_LITERAL(77, 1097, 17),
QT_MOC_LITERAL(78, 1115, 12),
QT_MOC_LITERAL(79, 1128, 7),
QT_MOC_LITERAL(80, 1136, 13),
QT_MOC_LITERAL(81, 1150, 15)
    },
    "charging\0ExitApp\0\0RefreshState\0"
    "enRefreshType\0type\0index\0AddCommamdIntoQueue\0"
    "stCommand\0strCommand\0refreshUI\0strID\0"
    "OnBtnSysClose\0OnBtnHideDebugInfo\0"
    "OnBtnShowDebugInfo\0OnCheckAutoShowDebugInfo\0"
    "nState\0OnAddCommamdIntoQueue\0"
    "OnReadyGetBatteryState\0OnAdjustDebugInfoRect\0"
    "rect\0OnClickClosetNo\0OnClickPrePage\0"
    "OnClickNextPage\0OnClickMenuCom\0QAction*\0"
    "action\0onOpenOptionWin\0OnClickMenuDebugInfoLeve\0"
    "onOpenLoginDialog\0timer_out\0readSerial\0"
    "strContent\0iError\0OnBtnChargingOrStopCharging1\0"
    "OnBtnDisChargingOrStop1\0OnBtnLevel\0"
    "updateTextEdit\0detectTextEdit\0"
    "detectHideDebugInfo\0detectRecvXOR\0"
    "getBatteryIdRelatedInfo\0strBatteryID\0"
    "MAP_CLOSET_IT&\0itCloset\0MAP_BATTERY_IT&\0"
    "itBattery\0MAP_BATTERY_MODEL_IT&\0"
    "itBatteryModel\0MAP_CHARGER_IT&\0itCharger\0"
    "chargerIDtoBatteryId\0chargerId\0"
    "batteryIDtoArrayIndex\0onRefreshUI\0"
    "releaseBatteryStateRefresh\0chargingByLocalID\0"
    "strId\0int*\0iResult\0showDebugInfo\0"
    "stopByLocalID\0createClosetRadio\0"
    "createMenus\0serial_scan\0createChargGrid\0"
    "createDebugInfoUI\0adjustUI\0getBatteryState\0"
    "indexBattery\0showTipsMessagebox\0iType\0"
    "strMsg\0refreshCurrentUI\0"
    "updateListviewBatteryModel\0indexMem\0"
    "allClosetCharging\0oneClosetCharging\0"
    "onPauseScand\0checked\0onPauseSubmit\0"
    "onOpenCanDevice"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_charging[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      51,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  269,    2, 0x06 /* Public */,
       3,    2,  270,    2, 0x06 /* Public */,
       7,    1,  275,    2, 0x06 /* Public */,
      10,    1,  278,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,  281,    2, 0x0a /* Public */,
      13,    0,  282,    2, 0x0a /* Public */,
      14,    0,  283,    2, 0x0a /* Public */,
      15,    1,  284,    2, 0x0a /* Public */,
      17,    1,  287,    2, 0x0a /* Public */,
      18,    1,  290,    2, 0x0a /* Public */,
      19,    1,  293,    2, 0x0a /* Public */,
      21,    0,  296,    2, 0x08 /* Private */,
      22,    0,  297,    2, 0x08 /* Private */,
      23,    0,  298,    2, 0x08 /* Private */,
      24,    1,  299,    2, 0x08 /* Private */,
      27,    0,  302,    2, 0x08 /* Private */,
      28,    1,  303,    2, 0x08 /* Private */,
      29,    0,  306,    2, 0x08 /* Private */,
      30,    0,  307,    2, 0x08 /* Private */,
      31,    3,  308,    2, 0x08 /* Private */,
      34,    0,  315,    2, 0x08 /* Private */,
      35,    0,  316,    2, 0x08 /* Private */,
      36,    0,  317,    2, 0x08 /* Private */,
      37,    1,  318,    2, 0x08 /* Private */,
      38,    0,  321,    2, 0x08 /* Private */,
      39,    0,  322,    2, 0x08 /* Private */,
      40,    1,  323,    2, 0x08 /* Private */,
      41,    5,  326,    2, 0x08 /* Private */,
      51,    1,  337,    2, 0x08 /* Private */,
      53,    1,  340,    2, 0x08 /* Private */,
      54,    1,  343,    2, 0x08 /* Private */,
      55,    0,  346,    2, 0x08 /* Private */,
      56,    3,  347,    2, 0x08 /* Private */,
      56,    2,  354,    2, 0x28 /* Private | MethodCloned */,
      61,    1,  359,    2, 0x08 /* Private */,
      62,    0,  362,    2, 0x08 /* Private */,
      63,    0,  363,    2, 0x08 /* Private */,
      64,    1,  364,    2, 0x08 /* Private */,
      65,    0,  367,    2, 0x08 /* Private */,
      66,    0,  368,    2, 0x08 /* Private */,
      67,    0,  369,    2, 0x08 /* Private */,
      68,    1,  370,    2, 0x08 /* Private */,
      70,    2,  373,    2, 0x08 /* Private */,
      73,    0,  378,    2, 0x08 /* Private */,
      74,    1,  379,    2, 0x08 /* Private */,
      74,    0,  382,    2, 0x28 /* Private | MethodCloned */,
      76,    0,  383,    2, 0x0a /* Public */,
      77,    0,  384,    2, 0x0a /* Public */,
      78,    1,  385,    2, 0x0a /* Public */,
      80,    1,  388,    2, 0x0a /* Public */,
      81,    1,  391,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, QMetaType::Int,    5,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QString,   11,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QRect,   20,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 25,   26,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 25,   26,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int,    5,   32,   33,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString,   32,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 43, 0x80000000 | 45, 0x80000000 | 47, 0x80000000 | 49,   42,   44,   46,   48,   50,
    QMetaType::Int, QMetaType::Int,   52,
    QMetaType::Int, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 58, QMetaType::Bool,   57,   59,   60,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 58,   57,   59,
    QMetaType::Bool, QMetaType::QString,   57,
    QMetaType::Void,
    0x80000000 | 25,
    0x80000000 | 25, 0x80000000 | 25,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QString, QMetaType::Int,   69,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   71,   72,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   75,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   79,
    QMetaType::Void, QMetaType::Bool,   79,
    QMetaType::Void, QMetaType::Bool,   79,

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
        case 5: _t->OnBtnHideDebugInfo(); break;
        case 6: _t->OnBtnShowDebugInfo(); break;
        case 7: _t->OnCheckAutoShowDebugInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->OnAddCommamdIntoQueue((*reinterpret_cast< stCommand(*)>(_a[1]))); break;
        case 9: _t->OnReadyGetBatteryState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->OnAdjustDebugInfoRect((*reinterpret_cast< QRect(*)>(_a[1]))); break;
        case 11: _t->OnClickClosetNo(); break;
        case 12: _t->OnClickPrePage(); break;
        case 13: _t->OnClickNextPage(); break;
        case 14: _t->OnClickMenuCom((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 15: _t->onOpenOptionWin(); break;
        case 16: _t->OnClickMenuDebugInfoLeve((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 17: _t->onOpenLoginDialog(); break;
        case 18: _t->timer_out(); break;
        case 19: _t->readSerial((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 20: _t->OnBtnChargingOrStopCharging1(); break;
        case 21: _t->OnBtnDisChargingOrStop1(); break;
        case 22: _t->OnBtnLevel(); break;
        case 23: _t->updateTextEdit((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 24: _t->detectTextEdit(); break;
        case 25: _t->detectHideDebugInfo(); break;
        case 26: { bool _r = _t->detectRecvXOR((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 27: { bool _r = _t->getBatteryIdRelatedInfo((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< MAP_CLOSET_IT(*)>(_a[2])),(*reinterpret_cast< MAP_BATTERY_IT(*)>(_a[3])),(*reinterpret_cast< MAP_BATTERY_MODEL_IT(*)>(_a[4])),(*reinterpret_cast< MAP_CHARGER_IT(*)>(_a[5])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 28: { int _r = _t->chargerIDtoBatteryId((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 29: { int _r = _t->batteryIDtoArrayIndex((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 30: _t->onRefreshUI((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 31: _t->releaseBatteryStateRefresh(); break;
        case 32: { bool _r = _t->chargingByLocalID((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 33: { bool _r = _t->chargingByLocalID((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 34: { bool _r = _t->stopByLocalID((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 35: _t->createClosetRadio(); break;
        case 36: { QAction* _r = _t->createMenus();
            if (_a[0]) *reinterpret_cast< QAction**>(_a[0]) = _r; }  break;
        case 37: { QAction* _r = _t->serial_scan((*reinterpret_cast< QAction*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QAction**>(_a[0]) = _r; }  break;
        case 38: _t->createChargGrid(); break;
        case 39: _t->createDebugInfoUI(); break;
        case 40: _t->adjustUI(); break;
        case 41: { QString _r = _t->getBatteryState((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 42: _t->showTipsMessagebox((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 43: _t->refreshCurrentUI(); break;
        case 44: _t->updateListviewBatteryModel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 45: _t->updateListviewBatteryModel(); break;
        case 46: _t->allClosetCharging(); break;
        case 47: _t->oneClosetCharging(); break;
        case 48: _t->onPauseScand((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 49: _t->onPauseSubmit((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 50: _t->onOpenCanDevice((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 14:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        case 16:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        case 37:
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
        if (_id < 51)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 51;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 51)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 51;
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
