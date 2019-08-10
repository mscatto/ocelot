/****************************************************************************
** Meta object code from reading C++ file 'libtree.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../src/gui/cwidgets/libtree.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'libtree.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_libtree_t {
    QByteArrayData data[17];
    char stringdata0[171];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_libtree_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_libtree_t qt_meta_stringdata_libtree = {
    {
QT_MOC_LITERAL(0, 0, 7), // "libtree"
QT_MOC_LITERAL(1, 8, 8), // "dispatch"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 12), // "QStringList*"
QT_MOC_LITERAL(4, 31, 1), // "l"
QT_MOC_LITERAL(5, 33, 15), // "transc_dispatch"
QT_MOC_LITERAL(6, 49, 7), // "discard"
QT_MOC_LITERAL(7, 57, 13), // "transc_append"
QT_MOC_LITERAL(8, 71, 14), // "transc_replace"
QT_MOC_LITERAL(9, 86, 14), // "media_dispatch"
QT_MOC_LITERAL(10, 101, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(11, 118, 4), // "item"
QT_MOC_LITERAL(12, 123, 6), // "column"
QT_MOC_LITERAL(13, 130, 8), // "populate"
QT_MOC_LITERAL(14, 139, 13), // "QSqlDatabase*"
QT_MOC_LITERAL(15, 153, 2), // "db"
QT_MOC_LITERAL(16, 156, 14) // "gatherselected"

    },
    "libtree\0dispatch\0\0QStringList*\0l\0"
    "transc_dispatch\0discard\0transc_append\0"
    "transc_replace\0media_dispatch\0"
    "QTreeWidgetItem*\0item\0column\0populate\0"
    "QSqlDatabase*\0db\0gatherselected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_libtree[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       5,    2,   62,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   67,    2, 0x08 /* Private */,
       7,    0,   70,    2, 0x28 /* Private | MethodCloned */,
       8,    0,   71,    2, 0x08 /* Private */,
       9,    2,   72,    2, 0x08 /* Private */,
       9,    1,   77,    2, 0x28 /* Private | MethodCloned */,
      13,    1,   80,    2, 0x0a /* Public */,
      16,    0,   83,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3, QMetaType::Bool,    4,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10, QMetaType::Int,   11,   12,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void,

       0        // eod
};

void libtree::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<libtree *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dispatch((*reinterpret_cast< QStringList*(*)>(_a[1]))); break;
        case 1: _t->transc_dispatch((*reinterpret_cast< QStringList*(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->transc_append((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->transc_append(); break;
        case 4: _t->transc_replace(); break;
        case 5: _t->media_dispatch((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->media_dispatch((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 7: _t->populate((*reinterpret_cast< QSqlDatabase*(*)>(_a[1]))); break;
        case 8: _t->gatherselected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (libtree::*)(QStringList * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&libtree::dispatch)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (libtree::*)(QStringList * , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&libtree::transc_dispatch)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject libtree::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_libtree.data,
    qt_meta_data_libtree,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *libtree::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *libtree::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_libtree.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int libtree::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void libtree::dispatch(QStringList * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void libtree::transc_dispatch(QStringList * _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
