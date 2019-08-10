/****************************************************************************
** Meta object code from reading C++ file 'mwindow.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../src/gui/mwindow.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mwindow.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_mwindow_t {
    QByteArrayData data[58];
    char stringdata0[614];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_mwindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_mwindow_t qt_meta_stringdata_mwindow = {
    {
QT_MOC_LITERAL(0, 0, 7), // "mwindow"
QT_MOC_LITERAL(1, 8, 11), // "player_play"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 10), // "player_set"
QT_MOC_LITERAL(4, 32, 4), // "file"
QT_MOC_LITERAL(5, 37, 11), // "player_stop"
QT_MOC_LITERAL(6, 49, 12), // "player_pause"
QT_MOC_LITERAL(7, 62, 13), // "player_setvol"
QT_MOC_LITERAL(8, 76, 3), // "vol"
QT_MOC_LITERAL(9, 80, 11), // "player_seek"
QT_MOC_LITERAL(10, 92, 3), // "sec"
QT_MOC_LITERAL(11, 96, 11), // "uilock_flip"
QT_MOC_LITERAL(12, 108, 10), // "clearcover"
QT_MOC_LITERAL(13, 119, 16), // "selectionchanged"
QT_MOC_LITERAL(14, 136, 4), // "item"
QT_MOC_LITERAL(15, 141, 12), // "coverchanged"
QT_MOC_LITERAL(16, 154, 8), // "QPixmap*"
QT_MOC_LITERAL(17, 163, 5), // "cover"
QT_MOC_LITERAL(18, 169, 10), // "libchanged"
QT_MOC_LITERAL(19, 180, 13), // "QSqlDatabase*"
QT_MOC_LITERAL(20, 194, 2), // "db"
QT_MOC_LITERAL(21, 197, 8), // "plappend"
QT_MOC_LITERAL(22, 206, 1), // "l"
QT_MOC_LITERAL(23, 208, 11), // "convhandler"
QT_MOC_LITERAL(24, 220, 12), // "QStringList*"
QT_MOC_LITERAL(25, 233, 5), // "files"
QT_MOC_LITERAL(26, 239, 6), // "plnext"
QT_MOC_LITERAL(27, 246, 6), // "plprev"
QT_MOC_LITERAL(28, 253, 11), // "mediastatus"
QT_MOC_LITERAL(29, 265, 6), // "status"
QT_MOC_LITERAL(30, 272, 13), // "volumechanged"
QT_MOC_LITERAL(31, 286, 13), // "toolbar_pause"
QT_MOC_LITERAL(32, 300, 12), // "toolbar_play"
QT_MOC_LITERAL(33, 313, 12), // "toolbar_stop"
QT_MOC_LITERAL(34, 326, 12), // "toolbar_next"
QT_MOC_LITERAL(35, 339, 12), // "toolbar_prev"
QT_MOC_LITERAL(36, 352, 12), // "toolbar_menu"
QT_MOC_LITERAL(37, 365, 12), // "config_spawn"
QT_MOC_LITERAL(38, 378, 11), // "about_spawn"
QT_MOC_LITERAL(39, 390, 16), // "transcoder_spawn"
QT_MOC_LITERAL(40, 407, 7), // "discard"
QT_MOC_LITERAL(41, 415, 15), // "tageditor_spawn"
QT_MOC_LITERAL(42, 431, 4), // "play"
QT_MOC_LITERAL(43, 436, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(44, 453, 6), // "select"
QT_MOC_LITERAL(45, 460, 4), // "show"
QT_MOC_LITERAL(46, 465, 13), // "child_resized"
QT_MOC_LITERAL(47, 479, 16), // "progslider_moved"
QT_MOC_LITERAL(48, 496, 1), // "x"
QT_MOC_LITERAL(49, 498, 15), // "volslider_moved"
QT_MOC_LITERAL(50, 514, 14), // "player_respond"
QT_MOC_LITERAL(51, 529, 14), // "uilock_respond"
QT_MOC_LITERAL(52, 544, 13), // "on_player_set"
QT_MOC_LITERAL(53, 558, 13), // "on_player_EOM"
QT_MOC_LITERAL(54, 572, 15), // "progslider_sync"
QT_MOC_LITERAL(55, 588, 3), // "pos"
QT_MOC_LITERAL(56, 592, 14), // "progslider_set"
QT_MOC_LITERAL(57, 607, 6) // "length"

    },
    "mwindow\0player_play\0\0player_set\0file\0"
    "player_stop\0player_pause\0player_setvol\0"
    "vol\0player_seek\0sec\0uilock_flip\0"
    "clearcover\0selectionchanged\0item\0"
    "coverchanged\0QPixmap*\0cover\0libchanged\0"
    "QSqlDatabase*\0db\0plappend\0l\0convhandler\0"
    "QStringList*\0files\0plnext\0plprev\0"
    "mediastatus\0status\0volumechanged\0"
    "toolbar_pause\0toolbar_play\0toolbar_stop\0"
    "toolbar_next\0toolbar_prev\0toolbar_menu\0"
    "config_spawn\0about_spawn\0transcoder_spawn\0"
    "discard\0tageditor_spawn\0play\0"
    "QTreeWidgetItem*\0select\0show\0child_resized\0"
    "progslider_moved\0x\0volslider_moved\0"
    "player_respond\0uilock_respond\0"
    "on_player_set\0on_player_EOM\0progslider_sync\0"
    "pos\0progslider_set\0length"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mwindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      39,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      17,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  209,    2, 0x06 /* Public */,
       3,    1,  210,    2, 0x06 /* Public */,
       5,    0,  213,    2, 0x06 /* Public */,
       6,    0,  214,    2, 0x06 /* Public */,
       7,    1,  215,    2, 0x06 /* Public */,
       9,    1,  218,    2, 0x06 /* Public */,
      11,    0,  221,    2, 0x06 /* Public */,
      12,    0,  222,    2, 0x06 /* Public */,
      13,    1,  223,    2, 0x06 /* Public */,
      15,    1,  226,    2, 0x06 /* Public */,
      18,    1,  229,    2, 0x06 /* Public */,
      21,    1,  232,    2, 0x06 /* Public */,
      23,    1,  235,    2, 0x06 /* Public */,
      26,    0,  238,    2, 0x06 /* Public */,
      27,    0,  239,    2, 0x06 /* Public */,
      28,    1,  240,    2, 0x06 /* Public */,
      30,    1,  243,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      31,    0,  246,    2, 0x0a /* Public */,
      32,    0,  247,    2, 0x0a /* Public */,
      33,    0,  248,    2, 0x0a /* Public */,
      34,    0,  249,    2, 0x0a /* Public */,
      35,    0,  250,    2, 0x0a /* Public */,
      36,    0,  251,    2, 0x0a /* Public */,
      37,    0,  252,    2, 0x0a /* Public */,
      38,    0,  253,    2, 0x0a /* Public */,
      39,    2,  254,    2, 0x0a /* Public */,
      41,    1,  259,    2, 0x0a /* Public */,
      42,    1,  262,    2, 0x0a /* Public */,
      44,    1,  265,    2, 0x0a /* Public */,
      45,    0,  268,    2, 0x0a /* Public */,
      46,    0,  269,    2, 0x0a /* Public */,
      47,    1,  270,    2, 0x08 /* Private */,
      49,    1,  273,    2, 0x08 /* Private */,
      50,    1,  276,    2, 0x08 /* Private */,
      51,    0,  279,    2, 0x08 /* Private */,
      52,    1,  280,    2, 0x08 /* Private */,
      53,    0,  283,    2, 0x08 /* Private */,
      54,    1,  284,    2, 0x08 /* Private */,
      56,    1,  287,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt,    8,
    QMetaType::Void, QMetaType::Short,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   14,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, QMetaType::QStringList,   22,
    QMetaType::Void, 0x80000000 | 24,   25,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   29,
    QMetaType::Void, QMetaType::Int,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 24, QMetaType::Bool,   22,   40,
    QMetaType::Void, 0x80000000 | 24,   22,
    QMetaType::Void, 0x80000000 | 43,   14,
    QMetaType::Void, 0x80000000 | 43,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   48,
    QMetaType::Void, QMetaType::Int,   48,
    QMetaType::Void, QMetaType::Int,   29,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QTime,   55,
    QMetaType::Void, QMetaType::QTime,   57,

       0        // eod
};

void mwindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<mwindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->player_play(); break;
        case 1: _t->player_set((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->player_stop(); break;
        case 3: _t->player_pause(); break;
        case 4: _t->player_setvol((*reinterpret_cast< uint(*)>(_a[1]))); break;
        case 5: _t->player_seek((*reinterpret_cast< short(*)>(_a[1]))); break;
        case 6: _t->uilock_flip(); break;
        case 7: _t->clearcover(); break;
        case 8: _t->selectionchanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->coverchanged((*reinterpret_cast< QPixmap*(*)>(_a[1]))); break;
        case 10: _t->libchanged((*reinterpret_cast< QSqlDatabase*(*)>(_a[1]))); break;
        case 11: _t->plappend((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 12: _t->convhandler((*reinterpret_cast< QStringList*(*)>(_a[1]))); break;
        case 13: _t->plnext(); break;
        case 14: _t->plprev(); break;
        case 15: _t->mediastatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->volumechanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->toolbar_pause(); break;
        case 18: _t->toolbar_play(); break;
        case 19: _t->toolbar_stop(); break;
        case 20: _t->toolbar_next(); break;
        case 21: _t->toolbar_prev(); break;
        case 22: _t->toolbar_menu(); break;
        case 23: _t->config_spawn(); break;
        case 24: _t->about_spawn(); break;
        case 25: _t->transcoder_spawn((*reinterpret_cast< QStringList*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 26: _t->tageditor_spawn((*reinterpret_cast< QStringList*(*)>(_a[1]))); break;
        case 27: _t->play((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 28: _t->select((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 29: _t->show(); break;
        case 30: _t->child_resized(); break;
        case 31: _t->progslider_moved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 32: _t->volslider_moved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->player_respond((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 34: _t->uilock_respond(); break;
        case 35: _t->on_player_set((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 36: _t->on_player_EOM(); break;
        case 37: _t->progslider_sync((*reinterpret_cast< QTime(*)>(_a[1]))); break;
        case 38: _t->progslider_set((*reinterpret_cast< QTime(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (mwindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mwindow::player_play)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (mwindow::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mwindow::player_set)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (mwindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mwindow::player_stop)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (mwindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mwindow::player_pause)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (mwindow::*)(uint );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mwindow::player_setvol)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (mwindow::*)(short );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mwindow::player_seek)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (mwindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mwindow::uilock_flip)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (mwindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mwindow::clearcover)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (mwindow::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mwindow::selectionchanged)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (mwindow::*)(QPixmap * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mwindow::coverchanged)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (mwindow::*)(QSqlDatabase * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mwindow::libchanged)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (mwindow::*)(QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mwindow::plappend)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (mwindow::*)(QStringList * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mwindow::convhandler)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (mwindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mwindow::plnext)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (mwindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mwindow::plprev)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (mwindow::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mwindow::mediastatus)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (mwindow::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mwindow::volumechanged)) {
                *result = 16;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject mwindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_mwindow.data,
    qt_meta_data_mwindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *mwindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mwindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_mwindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int mwindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 39)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 39;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 39)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 39;
    }
    return _id;
}

// SIGNAL 0
void mwindow::player_play()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void mwindow::player_set(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void mwindow::player_stop()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void mwindow::player_pause()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void mwindow::player_setvol(uint _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void mwindow::player_seek(short _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void mwindow::uilock_flip()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void mwindow::clearcover()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void mwindow::selectionchanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void mwindow::coverchanged(QPixmap * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void mwindow::libchanged(QSqlDatabase * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void mwindow::plappend(QStringList _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void mwindow::convhandler(QStringList * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void mwindow::plnext()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void mwindow::plprev()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void mwindow::mediastatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void mwindow::volumechanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
