/****************************************************************************
** Meta object code from reading C++ file 'playlistview.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../src/gui/cwidgets/playlistview.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'playlistview.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_playlistview_t {
    QByteArrayData data[27];
    char stringdata0[224];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_playlistview_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_playlistview_t qt_meta_stringdata_playlistview = {
    {
QT_MOC_LITERAL(0, 0, 12), // "playlistview"
QT_MOC_LITERAL(1, 13, 8), // "clearsel"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 7), // "clearpl"
QT_MOC_LITERAL(4, 31, 8), // "exportpl"
QT_MOC_LITERAL(5, 40, 10), // "viewappend"
QT_MOC_LITERAL(6, 51, 1), // "f"
QT_MOC_LITERAL(7, 53, 7), // "context"
QT_MOC_LITERAL(8, 61, 1), // "p"
QT_MOC_LITERAL(9, 63, 7), // "playing"
QT_MOC_LITERAL(10, 71, 9), // "refreshpl"
QT_MOC_LITERAL(11, 81, 4), // "next"
QT_MOC_LITERAL(12, 86, 4), // "prev"
QT_MOC_LITERAL(13, 91, 10), // "medistatus"
QT_MOC_LITERAL(14, 102, 6), // "status"
QT_MOC_LITERAL(15, 109, 8), // "swapitem"
QT_MOC_LITERAL(16, 118, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(17, 135, 4), // "item"
QT_MOC_LITERAL(18, 140, 7), // "toggler"
QT_MOC_LITERAL(19, 148, 7), // "checked"
QT_MOC_LITERAL(20, 156, 9), // "tab_close"
QT_MOC_LITERAL(21, 166, 5), // "index"
QT_MOC_LITERAL(22, 172, 10), // "tab_switch"
QT_MOC_LITERAL(23, 183, 10), // "tab_rename"
QT_MOC_LITERAL(24, 194, 3), // "key"
QT_MOC_LITERAL(25, 198, 10), // "renamer_ok"
QT_MOC_LITERAL(26, 209, 14) // "renamer_cancel"

    },
    "playlistview\0clearsel\0\0clearpl\0exportpl\0"
    "viewappend\0f\0context\0p\0playing\0refreshpl\0"
    "next\0prev\0medistatus\0status\0swapitem\0"
    "QTreeWidgetItem*\0item\0toggler\0checked\0"
    "tab_close\0index\0tab_switch\0tab_rename\0"
    "key\0renamer_ok\0renamer_cancel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_playlistview[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x06 /* Public */,
       3,    0,  100,    2, 0x06 /* Public */,
       4,    0,  101,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,  102,    2, 0x0a /* Public */,
       7,    1,  105,    2, 0x0a /* Public */,
       9,    1,  108,    2, 0x0a /* Public */,
      10,    0,  111,    2, 0x0a /* Public */,
      11,    0,  112,    2, 0x0a /* Public */,
      12,    0,  113,    2, 0x0a /* Public */,
      13,    1,  114,    2, 0x0a /* Public */,
      15,    1,  117,    2, 0x0a /* Public */,
      18,    1,  120,    2, 0x08 /* Private */,
      20,    1,  123,    2, 0x08 /* Private */,
      22,    1,  126,    2, 0x08 /* Private */,
      23,    1,  129,    2, 0x08 /* Private */,
      25,    0,  132,    2, 0x08 /* Private */,
      26,    0,  133,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QStringList,    6,
    QMetaType::Void, QMetaType::QPoint,    8,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, QMetaType::Bool,   19,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::QString,   24,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void playlistview::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<playlistview *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clearsel(); break;
        case 1: _t->clearpl(); break;
        case 2: _t->exportpl(); break;
        case 3: _t->viewappend((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 4: _t->context((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 5: _t->playing((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->refreshpl(); break;
        case 7: _t->next(); break;
        case 8: _t->prev(); break;
        case 9: _t->medistatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->swapitem((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 11: _t->toggler((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->tab_close((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->tab_switch((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->tab_rename((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->renamer_ok(); break;
        case 16: _t->renamer_cancel(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (playlistview::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&playlistview::clearsel)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (playlistview::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&playlistview::clearpl)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (playlistview::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&playlistview::exportpl)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject playlistview::staticMetaObject = { {
    &QTabWidget::staticMetaObject,
    qt_meta_stringdata_playlistview.data,
    qt_meta_data_playlistview,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *playlistview::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *playlistview::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_playlistview.stringdata0))
        return static_cast<void*>(this);
    return QTabWidget::qt_metacast(_clname);
}

int playlistview::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTabWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void playlistview::clearsel()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void playlistview::clearpl()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void playlistview::exportpl()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
