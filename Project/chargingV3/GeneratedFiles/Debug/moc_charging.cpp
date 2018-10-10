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
    QByteArrayData data[90];
    char stringdata[1268];
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
QT_MOC_LITERAL(10, 97, 8),
QT_MOC_LITERAL(11, 106, 9),
QT_MOC_LITERAL(12, 116, 9),
QT_MOC_LITERAL(13, 126, 5),
QT_MOC_LITERAL(14, 132, 13),
QT_MOC_LITERAL(15, 146, 18),
QT_MOC_LITERAL(16, 165, 18),
QT_MOC_LITERAL(17, 184, 24),
QT_MOC_LITERAL(18, 209, 6),
QT_MOC_LITERAL(19, 216, 21),
QT_MOC_LITERAL(20, 238, 22),
QT_MOC_LITERAL(21, 261, 21),
QT_MOC_LITERAL(22, 283, 4),
QT_MOC_LITERAL(23, 288, 15),
QT_MOC_LITERAL(24, 304, 14),
QT_MOC_LITERAL(25, 319, 15),
QT_MOC_LITERAL(26, 335, 14),
QT_MOC_LITERAL(27, 350, 8),
QT_MOC_LITERAL(28, 359, 6),
QT_MOC_LITERAL(29, 366, 15),
QT_MOC_LITERAL(30, 382, 24),
QT_MOC_LITERAL(31, 407, 17),
QT_MOC_LITERAL(32, 425, 9),
QT_MOC_LITERAL(33, 435, 10),
QT_MOC_LITERAL(34, 446, 10),
QT_MOC_LITERAL(35, 457, 6),
QT_MOC_LITERAL(36, 464, 9),
QT_MOC_LITERAL(37, 474, 28),
QT_MOC_LITERAL(38, 503, 23),
QT_MOC_LITERAL(39, 527, 10),
QT_MOC_LITERAL(40, 538, 14),
QT_MOC_LITERAL(41, 553, 14),
QT_MOC_LITERAL(42, 568, 19),
QT_MOC_LITERAL(43, 588, 13),
QT_MOC_LITERAL(44, 602, 23),
QT_MOC_LITERAL(45, 626, 12),
QT_MOC_LITERAL(46, 639, 14),
QT_MOC_LITERAL(47, 654, 8),
QT_MOC_LITERAL(48, 663, 15),
QT_MOC_LITERAL(49, 679, 9),
QT_MOC_LITERAL(50, 689, 21),
QT_MOC_LITERAL(51, 711, 14),
QT_MOC_LITERAL(52, 726, 15),
QT_MOC_LITERAL(53, 742, 9),
QT_MOC_LITERAL(54, 752, 13),
QT_MOC_LITERAL(55, 766, 7),
QT_MOC_LITERAL(56, 774, 20),
QT_MOC_LITERAL(57, 795, 9),
QT_MOC_LITERAL(58, 805, 21),
QT_MOC_LITERAL(59, 827, 11),
QT_MOC_LITERAL(60, 839, 26),
QT_MOC_LITERAL(61, 866, 17),
QT_MOC_LITERAL(62, 884, 5),
QT_MOC_LITERAL(63, 890, 4),
QT_MOC_LITERAL(64, 895, 7),
QT_MOC_LITERAL(65, 903, 13),
QT_MOC_LITERAL(66, 917, 13),
QT_MOC_LITERAL(67, 931, 17),
QT_MOC_LITERAL(68, 949, 11),
QT_MOC_LITERAL(69, 961, 11),
QT_MOC_LITERAL(70, 973, 15),
QT_MOC_LITERAL(71, 989, 17),
QT_MOC_LITERAL(72, 1007, 8),
QT_MOC_LITERAL(73, 1016, 15),
QT_MOC_LITERAL(74, 1032, 12),
QT_MOC_LITERAL(75, 1045, 18),
QT_MOC_LITERAL(76, 1064, 5),
QT_MOC_LITERAL(77, 1070, 6),
QT_MOC_LITERAL(78, 1077, 16),
QT_MOC_LITERAL(79, 1094, 26),
QT_MOC_LITERAL(80, 1121, 8),
QT_MOC_LITERAL(81, 1130, 22),
QT_MOC_LITERAL(82, 1153, 12),
QT_MOC_LITERAL(83, 1166, 17),
QT_MOC_LITERAL(84, 1184, 17),
QT_MOC_LITERAL(85, 1202, 12),
QT_MOC_LITERAL(86, 1215, 7),
QT_MOC_LITERAL(87, 1223, 13),
QT_MOC_LITERAL(88, 1237, 14),
QT_MOC_LITERAL(89, 1252, 15)
    },
    "charging\0ExitApp\0\0RefreshState\0"
    "enRefreshType\0type\0index\0AddCommamdIntoQueue\0"
    "stCommand\0strCommand\0printfed\0strPrintf\0"
    "refreshUI\0strID\0OnBtnSysClose\0"
    "OnBtnHideDebugInfo\0OnBtnShowDebugInfo\0"
    "OnCheckAutoShowDebugInfo\0nState\0"
    "OnAddCommamdIntoQueue\0OnReadyGetBatteryState\0"
    "OnAdjustDebugInfoRect\0rect\0OnClickClosetNo\0"
    "OnClickPrePage\0OnClickNextPage\0"
    "OnClickMenuCom\0QAction*\0action\0"
    "onOpenOptionWin\0OnClickMenuDebugInfoLeve\0"
    "onOpenLoginDialog\0timer_out\0readSerial\0"
    "strContent\0iError\0onReadCAN\0"
    "OnBtnChargingOrStopCharging1\0"
    "OnBtnDisChargingOrStop1\0OnBtnLevel\0"
    "updateTextEdit\0detectTextEdit\0"
    "detectHideDebugInfo\0detectRecvXOR\0"
    "getBatteryIdRelatedInfo\0strBatteryID\0"
    "MAP_CLOSET_IT&\0itCloset\0MAP_BATTERY_IT&\0"
    "itBattery\0MAP_BATTERY_MODEL_IT&\0"
    "itBatteryModel\0MAP_CHARGER_IT&\0itCharger\0"
    "MAP_LEVEL_IT&\0itLevel\0chargerIDtoBatteryId\0"
    "chargerId\0batteryIDtoArrayIndex\0"
    "onRefreshUI\0releaseBatteryStateRefresh\0"
    "chargingByLocalID\0strId\0int*\0iResult\0"
    "showDebugInfo\0stopByLocalID\0"
    "createClosetRadio\0createMenus\0serial_scan\0"
    "createChargGrid\0createDebugInfoUI\0"
    "adjustUI\0getBatteryState\0indexBattery\0"
    "showTipsMessagebox\0iType\0strMsg\0"
    "refreshCurrentUI\0updateListviewBatteryModel\0"
    "indexMem\0onOpenOrCloseCanDevice\0"
    "bOpenOrClose\0allClosetCharging\0"
    "oneClosetCharging\0onPauseScand\0checked\0"
    "onPauseSubmit\0onBtnCanDevice\0"
    "onOneKeyCharger"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_charging[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      55,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  289,    2, 0x06 /* Public */,
       3,    2,  290,    2, 0x06 /* Public */,
       7,    1,  295,    2, 0x06 /* Public */,
      10,    1,  298,    2, 0x06 /* Public */,
      12,    1,  301,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    0,  304,    2, 0x0a /* Public */,
      15,    0,  305,    2, 0x0a /* Public */,
      16,    0,  306,    2, 0x0a /* Public */,
      17,    1,  307,    2, 0x0a /* Public */,
      19,    1,  310,    2, 0x0a /* Public */,
      20,    1,  313,    2, 0x0a /* Public */,
      21,    1,  316,    2, 0x0a /* Public */,
      23,    0,  319,    2, 0x0a /* Public */,
      24,    0,  320,    2, 0x0a /* Public */,
      25,    0,  321,    2, 0x0a /* Public */,
      26,    1,  322,    2, 0x0a /* Public */,
      29,    0,  325,    2, 0x0a /* Public */,
      30,    1,  326,    2, 0x0a /* Public */,
      31,    0,  329,    2, 0x0a /* Public */,
      32,    0,  330,    2, 0x0a /* Public */,
      33,    3,  331,    2, 0x0a /* Public */,
      36,    1,  338,    2, 0x0a /* Public */,
      37,    0,  341,    2, 0x0a /* Public */,
      38,    0,  342,    2, 0x0a /* Public */,
      39,    0,  343,    2, 0x0a /* Public */,
      40,    1,  344,    2, 0x0a /* Public */,
      41,    0,  347,    2, 0x0a /* Public */,
      42,    0,  348,    2, 0x0a /* Public */,
      43,    1,  349,    2, 0x0a /* Public */,
      44,    6,  352,    2, 0x0a /* Public */,
      56,    1,  365,    2, 0x0a /* Public */,
      58,    1,  368,    2, 0x0a /* Public */,
      59,    1,  371,    2, 0x0a /* Public */,
      60,    0,  374,    2, 0x0a /* Public */,
      61,    3,  375,    2, 0x0a /* Public */,
      61,    2,  382,    2, 0x2a /* Public | MethodCloned */,
      66,    1,  387,    2, 0x0a /* Public */,
      67,    0,  390,    2, 0x0a /* Public */,
      68,    0,  391,    2, 0x0a /* Public */,
      69,    1,  392,    2, 0x0a /* Public */,
      70,    0,  395,    2, 0x0a /* Public */,
      71,    0,  396,    2, 0x0a /* Public */,
      72,    0,  397,    2, 0x0a /* Public */,
      73,    1,  398,    2, 0x0a /* Public */,
      75,    2,  401,    2, 0x0a /* Public */,
      78,    0,  406,    2, 0x0a /* Public */,
      79,    1,  407,    2, 0x0a /* Public */,
      79,    0,  410,    2, 0x2a /* Public | MethodCloned */,
      81,    1,  411,    2, 0x0a /* Public */,
      83,    0,  414,    2, 0x0a /* Public */,
      84,    0,  415,    2, 0x0a /* Public */,
      85,    1,  416,    2, 0x0a /* Public */,
      87,    1,  419,    2, 0x0a /* Public */,
      88,    1,  422,    2, 0x0a /* Public */,
      89,    1,  425,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, QMetaType::Int,    5,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::QString,   13,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QRect,   22,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 27,   28,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 27,   28,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int,    5,   34,   35,
    QMetaType::Void, QMetaType::QString,   34,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString,   34,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 46, 0x80000000 | 48, 0x80000000 | 50, 0x80000000 | 52, 0x80000000 | 54,   45,   47,   49,   51,   53,   55,
    QMetaType::Int, QMetaType::Int,   57,
    QMetaType::Int, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 63, QMetaType::Bool,   62,   64,   65,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 63,   62,   64,
    QMetaType::Bool, QMetaType::QString,   62,
    QMetaType::Void,
    0x80000000 | 27,
    0x80000000 | 27, 0x80000000 | 27,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QString, QMetaType::Int,   74,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   76,   77,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   80,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   82,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   86,
    QMetaType::Void, QMetaType::Bool,   86,
    QMetaType::Void, QMetaType::Bool,   86,
    QMetaType::Void, QMetaType::Bool,   86,

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
        case 3: _t->printfed((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->refreshUI((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->OnBtnSysClose(); break;
        case 6: _t->OnBtnHideDebugInfo(); break;
        case 7: _t->OnBtnShowDebugInfo(); break;
        case 8: _t->OnCheckAutoShowDebugInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->OnAddCommamdIntoQueue((*reinterpret_cast< stCommand(*)>(_a[1]))); break;
        case 10: _t->OnReadyGetBatteryState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->OnAdjustDebugInfoRect((*reinterpret_cast< QRect(*)>(_a[1]))); break;
        case 12: _t->OnClickClosetNo(); break;
        case 13: _t->OnClickPrePage(); break;
        case 14: _t->OnClickNextPage(); break;
        case 15: _t->OnClickMenuCom((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 16: _t->onOpenOptionWin(); break;
        case 17: _t->OnClickMenuDebugInfoLeve((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 18: _t->onOpenLoginDialog(); break;
        case 19: _t->timer_out(); break;
        case 20: _t->readSerial((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 21: _t->onReadCAN((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 22: _t->OnBtnChargingOrStopCharging1(); break;
        case 23: _t->OnBtnDisChargingOrStop1(); break;
        case 24: _t->OnBtnLevel(); break;
        case 25: _t->updateTextEdit((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 26: _t->detectTextEdit(); break;
        case 27: _t->detectHideDebugInfo(); break;
        case 28: { bool _r = _t->detectRecvXOR((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 29: { bool _r = _t->getBatteryIdRelatedInfo((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< MAP_CLOSET_IT(*)>(_a[2])),(*reinterpret_cast< MAP_BATTERY_IT(*)>(_a[3])),(*reinterpret_cast< MAP_BATTERY_MODEL_IT(*)>(_a[4])),(*reinterpret_cast< MAP_CHARGER_IT(*)>(_a[5])),(*reinterpret_cast< MAP_LEVEL_IT(*)>(_a[6])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 30: { int _r = _t->chargerIDtoBatteryId((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 31: { int _r = _t->batteryIDtoArrayIndex((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 32: _t->onRefreshUI((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 33: _t->releaseBatteryStateRefresh(); break;
        case 34: { bool _r = _t->chargingByLocalID((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 35: { bool _r = _t->chargingByLocalID((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 36: { bool _r = _t->stopByLocalID((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 37: _t->createClosetRadio(); break;
        case 38: { QAction* _r = _t->createMenus();
            if (_a[0]) *reinterpret_cast< QAction**>(_a[0]) = _r; }  break;
        case 39: { QAction* _r = _t->serial_scan((*reinterpret_cast< QAction*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QAction**>(_a[0]) = _r; }  break;
        case 40: _t->createChargGrid(); break;
        case 41: _t->createDebugInfoUI(); break;
        case 42: _t->adjustUI(); break;
        case 43: { QString _r = _t->getBatteryState((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 44: _t->showTipsMessagebox((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 45: _t->refreshCurrentUI(); break;
        case 46: _t->updateListviewBatteryModel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 47: _t->updateListviewBatteryModel(); break;
        case 48: _t->onOpenOrCloseCanDevice((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 49: _t->allClosetCharging(); break;
        case 50: _t->oneClosetCharging(); break;
        case 51: _t->onPauseScand((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 52: _t->onPauseSubmit((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 53: _t->onBtnCanDevice((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 54: _t->onOneKeyCharger((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 15:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        case 39:
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
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&charging::printfed)) {
                *result = 3;
            }
        }
        {
            typedef void (charging::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&charging::refreshUI)) {
                *result = 4;
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
        if (_id < 55)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 55;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 55)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 55;
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
void charging::printfed(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void charging::refreshUI(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
