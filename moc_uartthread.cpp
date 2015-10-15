/****************************************************************************
** Meta object code from reading C++ file 'uartthread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "uart/uartthread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'uartthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CUartThread_t {
    QByteArrayData data[5];
    char stringdata[38];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CUartThread_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CUartThread_t qt_meta_stringdata_CUartThread = {
    {
QT_MOC_LITERAL(0, 0, 11), // "CUartThread"
QT_MOC_LITERAL(1, 12, 11), // "sSerialData"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 5), // "char*"
QT_MOC_LITERAL(4, 31, 6) // "strBuf"

    },
    "CUartThread\0sSerialData\0\0char*\0strBuf"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CUartThread[] = {

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
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void CUartThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CUartThread *_t = static_cast<CUartThread *>(_o);
        switch (_id) {
        case 0: _t->sSerialData((*reinterpret_cast< char*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CUartThread::*_t)(char * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CUartThread::sSerialData)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CUartThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CUartThread.data,
      qt_meta_data_CUartThread,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CUartThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CUartThread::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CUartThread.stringdata))
        return static_cast<void*>(const_cast< CUartThread*>(this));
    return QThread::qt_metacast(_clname);
}

int CUartThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CUartThread::sSerialData(char * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_CUartThread2_t {
    QByteArrayData data[7];
    char stringdata[63];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CUartThread2_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CUartThread2_t qt_meta_stringdata_CUartThread2 = {
    {
QT_MOC_LITERAL(0, 0, 12), // "CUartThread2"
QT_MOC_LITERAL(1, 13, 11), // "sSerialData"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 5), // "char*"
QT_MOC_LITERAL(4, 32, 6), // "strBuf"
QT_MOC_LITERAL(5, 39, 15), // "writeSerialSlot"
QT_MOC_LITERAL(6, 55, 7) // "wstrBuf"

    },
    "CUartThread2\0sSerialData\0\0char*\0strBuf\0"
    "writeSerialSlot\0wstrBuf"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CUartThread2[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   27,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    6,

       0        // eod
};

void CUartThread2::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CUartThread2 *_t = static_cast<CUartThread2 *>(_o);
        switch (_id) {
        case 0: _t->sSerialData((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 1: _t->writeSerialSlot((*reinterpret_cast< char*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CUartThread2::*_t)(char * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CUartThread2::sSerialData)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CUartThread2::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CUartThread2.data,
      qt_meta_data_CUartThread2,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CUartThread2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CUartThread2::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CUartThread2.stringdata))
        return static_cast<void*>(const_cast< CUartThread2*>(this));
    return QThread::qt_metacast(_clname);
}

int CUartThread2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void CUartThread2::sSerialData(char * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
