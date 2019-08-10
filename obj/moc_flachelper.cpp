/****************************************************************************
** Meta object code from reading C++ file 'flachelper.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../src/gui/helpers/flachelper.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'flachelper.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_flachelper_t {
    QByteArrayData data[8];
    char stringdata0[86];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_flachelper_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_flachelper_t qt_meta_stringdata_flachelper = {
    {
QT_MOC_LITERAL(0, 0, 10), // "flachelper"
QT_MOC_LITERAL(1, 11, 16), // "refresh_enclevel"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 5), // "value"
QT_MOC_LITERAL(4, 35, 13), // "flip_encsrate"
QT_MOC_LITERAL(5, 49, 14), // "flip_encbdepth"
QT_MOC_LITERAL(6, 64, 12), // "flip_encpath"
QT_MOC_LITERAL(7, 77, 8) // "pick_enc"

    },
    "flachelper\0refresh_enclevel\0\0value\0"
    "flip_encsrate\0flip_encbdepth\0flip_encpath\0"
    "pick_enc"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_flachelper[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x08 /* Private */,
       4,    0,   42,    2, 0x08 /* Private */,
       5,    0,   43,    2, 0x08 /* Private */,
       6,    0,   44,    2, 0x08 /* Private */,
       7,    0,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void flachelper::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<flachelper *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->refresh_enclevel((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->flip_encsrate(); break;
        case 2: _t->flip_encbdepth(); break;
        case 3: _t->flip_encpath(); break;
        case 4: _t->pick_enc(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject flachelper::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_flachelper.data,
    qt_meta_data_flachelper,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *flachelper::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *flachelper::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_flachelper.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int flachelper::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
