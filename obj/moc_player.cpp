/****************************************************************************
** Meta object code from reading C++ file 'player.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../src/player.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'player.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_player_t {
    QByteArrayData data[18];
    char stringdata0[120];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_player_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_player_t qt_meta_stringdata_player = {
    {
QT_MOC_LITERAL(0, 0, 6), // "player"
QT_MOC_LITERAL(1, 7, 16), // "position_changed"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 3), // "pos"
QT_MOC_LITERAL(4, 29, 14), // "length_changed"
QT_MOC_LITERAL(5, 44, 3), // "len"
QT_MOC_LITERAL(6, 48, 6), // "paused"
QT_MOC_LITERAL(7, 55, 3), // "EOM"
QT_MOC_LITERAL(8, 59, 14), // "position_proxy"
QT_MOC_LITERAL(9, 74, 4), // "play"
QT_MOC_LITERAL(10, 79, 4), // "stop"
QT_MOC_LITERAL(11, 84, 5), // "pause"
QT_MOC_LITERAL(12, 90, 4), // "seek"
QT_MOC_LITERAL(13, 95, 3), // "sec"
QT_MOC_LITERAL(14, 99, 3), // "set"
QT_MOC_LITERAL(15, 103, 4), // "file"
QT_MOC_LITERAL(16, 108, 9), // "setVolume"
QT_MOC_LITERAL(17, 118, 1) // "v"

    },
    "player\0position_changed\0\0pos\0"
    "length_changed\0len\0paused\0EOM\0"
    "position_proxy\0play\0stop\0pause\0seek\0"
    "sec\0set\0file\0setVolume\0v"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_player[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       4,    1,   72,    2, 0x06 /* Public */,
       6,    1,   75,    2, 0x06 /* Public */,
       7,    0,   78,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   79,    2, 0x08 /* Private */,
       9,    0,   80,    2, 0x0a /* Public */,
      10,    0,   81,    2, 0x0a /* Public */,
      11,    0,   82,    2, 0x0a /* Public */,
      12,    1,   83,    2, 0x0a /* Public */,
      14,    1,   86,    2, 0x0a /* Public */,
      16,    1,   89,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QTime,    3,
    QMetaType::Void, QMetaType::QTime,    5,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Short,   13,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, QMetaType::UShort,   17,

       0        // eod
};

void player::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<player *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->position_changed((*reinterpret_cast< QTime(*)>(_a[1]))); break;
        case 1: _t->length_changed((*reinterpret_cast< QTime(*)>(_a[1]))); break;
        case 2: _t->paused((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->EOM(); break;
        case 4: _t->position_proxy(); break;
        case 5: _t->play(); break;
        case 6: _t->stop(); break;
        case 7: _t->pause(); break;
        case 8: _t->seek((*reinterpret_cast< short(*)>(_a[1]))); break;
        case 9: _t->set((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->setVolume((*reinterpret_cast< ushort(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (player::*)(QTime );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&player::position_changed)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (player::*)(QTime );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&player::length_changed)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (player::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&player::paused)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (player::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&player::EOM)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject player::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_player.data,
    qt_meta_data_player,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *player::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *player::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_player.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int player::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void player::position_changed(QTime _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void player::length_changed(QTime _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void player::paused(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void player::EOM()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
