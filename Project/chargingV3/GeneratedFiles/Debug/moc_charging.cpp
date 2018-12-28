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
    QByteArrayData data[97];
    char stringdata[1342];
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
QT_MOC_LITERAL(12, 116, 11),
QT_MOC_LITERAL(13, 128, 9),
QT_MOC_LITERAL(14, 138, 5),
QT_MOC_LITERAL(15, 144, 12),
QT_MOC_LITERAL(16, 157, 13),
QT_MOC_LITERAL(17, 171, 18),
QT_MOC_LITERAL(18, 190, 18),
QT_MOC_LITERAL(19, 209, 24),
QT_MOC_LITERAL(20, 234, 6),
QT_MOC_LITERAL(21, 241, 21),
QT_MOC_LITERAL(22, 263, 22),
QT_MOC_LITERAL(23, 286, 21),
QT_MOC_LITERAL(24, 308, 4),
QT_MOC_LITERAL(25, 313, 15),
QT_MOC_LITERAL(26, 329, 14),
QT_MOC_LITERAL(27, 344, 15),
QT_MOC_LITERAL(28, 360, 14),
QT_MOC_LITERAL(29, 375, 8),
QT_MOC_LITERAL(30, 384, 6),
QT_MOC_LITERAL(31, 391, 15),
QT_MOC_LITERAL(32, 407, 24),
QT_MOC_LITERAL(33, 432, 16),
QT_MOC_LITERAL(34, 449, 9),
QT_MOC_LITERAL(35, 459, 10),
QT_MOC_LITERAL(36, 470, 10),
QT_MOC_LITERAL(37, 481, 6),
QT_MOC_LITERAL(38, 488, 9),
QT_MOC_LITERAL(39, 498, 28),
QT_MOC_LITERAL(40, 527, 23),
QT_MOC_LITERAL(41, 551, 10),
QT_MOC_LITERAL(42, 562, 14),
QT_MOC_LITERAL(43, 577, 14),
QT_MOC_LITERAL(44, 592, 19),
QT_MOC_LITERAL(45, 612, 13),
QT_MOC_LITERAL(46, 626, 23),
QT_MOC_LITERAL(47, 650, 12),
QT_MOC_LITERAL(48, 663, 14),
QT_MOC_LITERAL(49, 678, 8),
QT_MOC_LITERAL(50, 687, 15),
QT_MOC_LITERAL(51, 703, 9),
QT_MOC_LITERAL(52, 713, 21),
QT_MOC_LITERAL(53, 735, 14),
QT_MOC_LITERAL(54, 750, 15),
QT_MOC_LITERAL(55, 766, 9),
QT_MOC_LITERAL(56, 776, 13),
QT_MOC_LITERAL(57, 790, 7),
QT_MOC_LITERAL(58, 798, 16),
QT_MOC_LITERAL(59, 815, 5),
QT_MOC_LITERAL(60, 821, 3),
QT_MOC_LITERAL(61, 825, 20),
QT_MOC_LITERAL(62, 846, 9),
QT_MOC_LITERAL(63, 856, 21),
QT_MOC_LITERAL(64, 878, 11),
QT_MOC_LITERAL(65, 890, 26),
QT_MOC_LITERAL(66, 917, 17),
QT_MOC_LITERAL(67, 935, 5),
QT_MOC_LITERAL(68, 941, 4),
QT_MOC_LITERAL(69, 946, 7),
QT_MOC_LITERAL(70, 954, 13),
QT_MOC_LITERAL(71, 968, 13),
QT_MOC_LITERAL(72, 982, 17),
QT_MOC_LITERAL(73, 1000, 11),
QT_MOC_LITERAL(74, 1012, 11),
QT_MOC_LITERAL(75, 1024, 15),
QT_MOC_LITERAL(76, 1040, 17),
QT_MOC_LITERAL(77, 1058, 8),
QT_MOC_LITERAL(78, 1067, 15),
QT_MOC_LITERAL(79, 1083, 12),
QT_MOC_LITERAL(80, 1096, 18),
QT_MOC_LITERAL(81, 1115, 5),
QT_MOC_LITERAL(82, 1121, 6),
QT_MOC_LITERAL(83, 1128, 16),
QT_MOC_LITERAL(84, 1145, 26),
QT_MOC_LITERAL(85, 1172, 8),
QT_MOC_LITERAL(86, 1181, 22),
QT_MOC_LITERAL(87, 1204, 12),
QT_MOC_LITERAL(88, 1217, 14),
QT_MOC_LITERAL(89, 1232, 7),
QT_MOC_LITERAL(90, 1240, 17),
QT_MOC_LITERAL(91, 1258, 17),
QT_MOC_LITERAL(92, 1276, 12),
QT_MOC_LITERAL(93, 1289, 7),
QT_MOC_LITERAL(94, 1297, 13),
QT_MOC_LITERAL(95, 1311, 14),
QT_MOC_LITERAL(96, 1326, 15)
    },
    "charging\0ExitApp\0\0RefreshState\0"
    "enRefreshType\0type\0index\0AddCommamdIntoQueue\0"
    "stCommand\0strCommand\0printfed\0strPrintf\0"
    "nDebugLevel\0refreshUI\0strID\0setDebugInfo\0"
    "OnBtnSysClose\0OnBtnHideDebugInfo\0"
    "OnBtnShowDebugInfo\0OnCheckAutoShowDebugInfo\0"
    "nState\0OnAddCommamdIntoQueue\0"
    "OnReadyGetBatteryState\0OnAdjustDebugInfoRect\0"
    "rect\0OnClickClosetNo\0OnClickPrePage\0"
    "OnClickNextPage\0OnClickMenuCom\0QAction*\0"
    "action\0onOpenOptionWin\0OnClickMenuDebugInfoLeve\0"
    "onOpenDataDialog\0timer_out\0readSerial\0"
    "strContent\0iError\0onReadCAN\0"
    "OnBtnChargingOrStopCharging1\0"
    "OnBtnDisChargingOrStop1\0OnBtnLevel\0"
    "updateTextEdit\0detectTextEdit\0"
    "detectHideDebugInfo\0detectRecvXOR\0"
    "getBatteryIdRelatedInfo\0strBatteryID\0"
    "MAP_CLOSET_IT&\0itCloset\0MAP_BATTERY_IT&\0"
    "itBattery\0MAP_BATTERY_MODEL_IT&\0"
    "itBatteryModel\0MAP_CHARGER_IT&\0itCharger\0"
    "MAP_LEVEL_IT&\0itLevel\0getCanDJIBattery\0"
    "CANID\0pos\0chargerIDtoBatteryId\0chargerId\0"
    "batteryIDtoArrayIndex\0onRefreshUI\0"
    "releaseBatteryStateRefresh\0chargingByLocalID\0"
    "strId\0int*\0iResult\0showDebugInfo\0"
    "stopByLocalID\0createClosetRadio\0"
    "createMenus\0serial_scan\0createChargGrid\0"
    "createDebugInfoUI\0adjustUI\0getBatteryState\0"
    "indexBattery\0showTipsMessagebox\0iType\0"
    "strMsg\0refreshCurrentUI\0"
    "updateListviewBatteryModel\0indexMem\0"
    "onOpenOrCloseCanDevice\0bOpenOrClose\0"
    "onSetDebugInfo\0strInfo\0allClosetCharging\0"
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
      58,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  304,    2, 0x06 /* Public */,
       3,    2,  305,    2, 0x06 /* Public */,
       7,    1,  310,    2, 0x06 /* Public */,
      10,    2,  313,    2, 0x06 /* Public */,
      13,    1,  318,    2, 0x06 /* Public */,
      15,    1,  321,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      16,    0,  324,    2, 0x0a /* Public */,
      17,    0,  325,    2, 0x0a /* Public */,
      18,    0,  326,    2, 0x0a /* Public */,
      19,    1,  327,    2, 0x0a /* Public */,
      21,    1,  330,    2, 0x0a /* Public */,
      22,    1,  333,    2, 0x0a /* Public */,
      23,    1,  336,    2, 0x0a /* Public */,
      25,    0,  339,    2, 0x0a /* Public */,
      26,    0,  340,    2, 0x0a /* Public */,
      27,    0,  341,    2, 0x0a /* Public */,
      28,    1,  342,    2, 0x0a /* Public */,
      31,    0,  345,    2, 0x0a /* Public */,
      32,    1,  346,    2, 0x0a /* Public */,
      33,    0,  349,    2, 0x0a /* Public */,
      34,    0,  350,    2, 0x0a /* Public */,
      35,    3,  351,    2, 0x0a /* Public */,
      38,    1,  358,    2, 0x0a /* Public */,
      39,    0,  361,    2, 0x0a /* Public */,
      40,    0,  362,    2, 0x0a /* Public */,
      41,    0,  363,    2, 0x0a /* Public */,
      42,    2,  364,    2, 0x0a /* Public */,
      43,    0,  369,    2, 0x0a /* Public */,
      44,    0,  370,    2, 0x0a /* Public */,
      45,    1,  371,    2, 0x0a /* Public */,
      46,    6,  374,    2, 0x0a /* Public */,
      58,    2,  387,    2, 0x0a /* Public */,
      61,    1,  392,    2, 0x0a /* Public */,
      63,    1,  395,    2, 0x0a /* Public */,
      64,    1,  398,    2, 0x0a /* Public */,
      65,    0,  401,    2, 0x0a /* Public */,
      66,    3,  402,    2, 0x0a /* Public */,
      66,    2,  409,    2, 0x2a /* Public | MethodCloned */,
      71,    1,  414,    2, 0x0a /* Public */,
      72,    0,  417,    2, 0x0a /* Public */,
      73,    0,  418,    2, 0x0a /* Public */,
      74,    1,  419,    2, 0x0a /* Public */,
      75,    0,  422,    2, 0x0a /* Public */,
      76,    0,  423,    2, 0x0a /* Public */,
      77,    0,  424,    2, 0x0a /* Public */,
      78,    1,  425,    2, 0x0a /* Public */,
      80,    2,  428,    2, 0x0a /* Public */,
      83,    0,  433,    2, 0x0a /* Public */,
      84,    1,  434,    2, 0x0a /* Public */,
      84,    0,  437,    2, 0x2a /* Public | MethodCloned */,
      86,    1,  438,    2, 0x0a /* Public */,
      88,    1,  441,    2, 0x0a /* Public */,
      90,    0,  444,    2, 0x0a /* Public */,
      91,    0,  445,    2, 0x0a /* Public */,
      92,    1,  446,    2, 0x0a /* Public */,
      94,    1,  449,    2, 0x0a /* Public */,
      95,    1,  452,    2, 0x0a /* Public */,
      96,    0,  455,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, QMetaType::Int,    5,    6,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   11,   12,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QRect,   24,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 29,   30,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 29,   30,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int,    5,   36,   37,
    QMetaType::Void, QMetaType::QString,   36,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    2,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString,   36,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 48, 0x80000000 | 50, 0x80000000 | 52, 0x80000000 | 54, 0x80000000 | 56,   47,   49,   51,   53,   55,   57,
    QMetaType::Int, QMetaType::Int, QMetaType::Int,   59,   60,
    QMetaType::Int, QMetaType::Int,   62,
    QMetaType::Int, QMetaType::QString,   14,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 68, QMetaType::Bool,   67,   69,   70,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 68,   67,   69,
    QMetaType::Bool, QMetaType::QString,   67,
    QMetaType::Void,
    0x80000000 | 29,
    0x80000000 | 29, 0x80000000 | 29,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QString, QMetaType::Int,   79,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   81,   82,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   85,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   87,
    QMetaType::Void, QMetaType::QString,   89,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   93,
    QMetaType::Void, QMetaType::Bool,   93,
    QMetaType::Void, QMetaType::Bool,   93,
    QMetaType::Void,

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
        case 3: _t->printfed((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->refreshUI((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->setDebugInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->OnBtnSysClose(); break;
        case 7: _t->OnBtnHideDebugInfo(); break;
        case 8: _t->OnBtnShowDebugInfo(); break;
        case 9: _t->OnCheckAutoShowDebugInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->OnAddCommamdIntoQueue((*reinterpret_cast< stCommand(*)>(_a[1]))); break;
        case 11: _t->OnReadyGetBatteryState((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->OnAdjustDebugInfoRect((*reinterpret_cast< QRect(*)>(_a[1]))); break;
        case 13: _t->OnClickClosetNo(); break;
        case 14: _t->OnClickPrePage(); break;
        case 15: _t->OnClickNextPage(); break;
        case 16: _t->OnClickMenuCom((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 17: _t->onOpenOptionWin(); break;
        case 18: _t->OnClickMenuDebugInfoLeve((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 19: _t->onOpenDataDialog(); break;
        case 20: _t->timer_out(); break;
        case 21: _t->readSerial((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 22: _t->onReadCAN((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: _t->OnBtnChargingOrStopCharging1(); break;
        case 24: _t->OnBtnDisChargingOrStop1(); break;
        case 25: _t->OnBtnLevel(); break;
        case 26: _t->updateTextEdit((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 27: _t->detectTextEdit(); break;
        case 28: _t->detectHideDebugInfo(); break;
        case 29: { bool _r = _t->detectRecvXOR((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 30: { bool _r = _t->getBatteryIdRelatedInfo((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< MAP_CLOSET_IT(*)>(_a[2])),(*reinterpret_cast< MAP_BATTERY_IT(*)>(_a[3])),(*reinterpret_cast< MAP_BATTERY_MODEL_IT(*)>(_a[4])),(*reinterpret_cast< MAP_CHARGER_IT(*)>(_a[5])),(*reinterpret_cast< MAP_LEVEL_IT(*)>(_a[6])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 31: { int _r = _t->getCanDJIBattery((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 32: { int _r = _t->chargerIDtoBatteryId((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 33: { int _r = _t->batteryIDtoArrayIndex((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 34: _t->onRefreshUI((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 35: _t->releaseBatteryStateRefresh(); break;
        case 36: { bool _r = _t->chargingByLocalID((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 37: { bool _r = _t->chargingByLocalID((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 38: { bool _r = _t->stopByLocalID((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 39: _t->createClosetRadio(); break;
        case 40: { QAction* _r = _t->createMenus();
            if (_a[0]) *reinterpret_cast< QAction**>(_a[0]) = _r; }  break;
        case 41: { QAction* _r = _t->serial_scan((*reinterpret_cast< QAction*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QAction**>(_a[0]) = _r; }  break;
        case 42: _t->createChargGrid(); break;
        case 43: _t->createDebugInfoUI(); break;
        case 44: _t->adjustUI(); break;
        case 45: { QString _r = _t->getBatteryState((*reinterpret_cast< int(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 46: _t->showTipsMessagebox((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 47: _t->refreshCurrentUI(); break;
        case 48: _t->updateListviewBatteryModel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 49: _t->updateListviewBatteryModel(); break;
        case 50: _t->onOpenOrCloseCanDevice((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 51: _t->onSetDebugInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 52: _t->allClosetCharging(); break;
        case 53: _t->oneClosetCharging(); break;
        case 54: _t->onPauseScand((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 55: _t->onPauseSubmit((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 56: _t->onBtnCanDevice((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 57: _t->onOneKeyCharger(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 16:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        case 18:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        case 41:
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
            typedef void (charging::*_t)(QString , int );
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
        {
            typedef void (charging::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&charging::setDebugInfo)) {
                *result = 5;
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
        if (_id < 58)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 58;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 58)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 58;
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
void charging::printfed(QString _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void charging::refreshUI(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void charging::setDebugInfo(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
