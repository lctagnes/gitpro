/****************************************************************************
** Meta object code from reading C++ file 'tcpclient.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "tcp/tcpclient.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CTCPClient[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      19,   12,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      41,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CTCPClient[] = {
    "CTCPClient\0\0strMsg\0sRecvTCPMsg(QString&)\0"
    "ReadMsgSlot()\0"
};

void CTCPClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CTCPClient *_t = static_cast<CTCPClient *>(_o);
        switch (_id) {
        case 0: _t->sRecvTCPMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->ReadMsgSlot(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CTCPClient::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CTCPClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CTCPClient,
      qt_meta_data_CTCPClient, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CTCPClient::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CTCPClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CTCPClient::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CTCPClient))
        return static_cast<void*>(const_cast< CTCPClient*>(this));
    return QObject::qt_metacast(_clname);
}

int CTCPClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void CTCPClient::sRecvTCPMsg(QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
