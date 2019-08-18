/****************************************************************************
** Meta object code from reading C++ file 'Notification.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../src/libs/libnotify-qt/Notification.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Notification.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Notification_t {
    QByteArrayData data[9];
    char stringdata0[92];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Notification_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Notification_t qt_meta_stringdata_Notification = {
    {
QT_MOC_LITERAL(0, 0, 12), // "Notification"
QT_MOC_LITERAL(1, 13, 6), // "closed"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 6), // "reason"
QT_MOC_LITERAL(4, 28, 13), // "actionInvoked"
QT_MOC_LITERAL(5, 42, 9), // "actionKey"
QT_MOC_LITERAL(6, 52, 20), // "onNotificationClosed"
QT_MOC_LITERAL(7, 73, 2), // "id"
QT_MOC_LITERAL(8, 76, 15) // "onActionInvoked"

    },
    "Notification\0closed\0\0reason\0actionInvoked\0"
    "actionKey\0onNotificationClosed\0id\0"
    "onActionInvoked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Notification[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    1,   37,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   40,    2, 0x08 /* Private */,
       8,    2,   45,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::UInt,    3,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::UInt, QMetaType::UInt,    7,    3,
    QMetaType::Void, QMetaType::UInt, QMetaType::QString,    7,    5,

       0        // eod
};

void Notification::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Notification *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->closed((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        case 1: _t->actionInvoked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->onNotificationClosed((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< quint32(*)>(_a[2]))); break;
        case 3: _t->onActionInvoked((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Notification::*)(quint32 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Notification::closed)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Notification::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Notification::actionInvoked)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Notification::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_Notification.data,
    qt_meta_data_Notification,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Notification::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Notification::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Notification.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Notification::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void Notification::closed(quint32 _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Notification::actionInvoked(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
