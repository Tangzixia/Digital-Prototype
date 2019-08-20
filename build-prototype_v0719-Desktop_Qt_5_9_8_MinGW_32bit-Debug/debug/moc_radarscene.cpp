/****************************************************************************
** Meta object code from reading C++ file 'radarscene.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../prototype_v0719/radarscene.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'radarscene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RadarScene_t {
    QByteArrayData data[20];
    char stringdata0[210];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RadarScene_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RadarScene_t qt_meta_stringdata_RadarScene = {
    {
QT_MOC_LITERAL(0, 0, 10), // "RadarScene"
QT_MOC_LITERAL(1, 11, 9), // "signal_xy"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 1), // "x"
QT_MOC_LITERAL(4, 24, 1), // "y"
QT_MOC_LITERAL(5, 26, 12), // "itemInserted"
QT_MOC_LITERAL(6, 39, 12), // "DiagramItem*"
QT_MOC_LITERAL(7, 52, 4), // "item"
QT_MOC_LITERAL(8, 57, 12), // "textInserted"
QT_MOC_LITERAL(9, 70, 18), // "QGraphicsTextItem*"
QT_MOC_LITERAL(10, 89, 12), // "itemSelected"
QT_MOC_LITERAL(11, 102, 14), // "QGraphicsItem*"
QT_MOC_LITERAL(12, 117, 7), // "setMode"
QT_MOC_LITERAL(13, 125, 4), // "Mode"
QT_MOC_LITERAL(14, 130, 4), // "mode"
QT_MOC_LITERAL(15, 135, 11), // "setItemType"
QT_MOC_LITERAL(16, 147, 24), // "DiagramItem::DiagramType"
QT_MOC_LITERAL(17, 172, 4), // "type"
QT_MOC_LITERAL(18, 177, 15), // "editorLostFocus"
QT_MOC_LITERAL(19, 193, 16) // "DiagramTextItem*"

    },
    "RadarScene\0signal_xy\0\0x\0y\0itemInserted\0"
    "DiagramItem*\0item\0textInserted\0"
    "QGraphicsTextItem*\0itemSelected\0"
    "QGraphicsItem*\0setMode\0Mode\0mode\0"
    "setItemType\0DiagramItem::DiagramType\0"
    "type\0editorLostFocus\0DiagramTextItem*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RadarScene[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x06 /* Public */,
       5,    1,   54,    2, 0x06 /* Public */,
       8,    1,   57,    2, 0x06 /* Public */,
      10,    1,   60,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    1,   63,    2, 0x0a /* Public */,
      15,    1,   66,    2, 0x0a /* Public */,
      18,    1,   69,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,    7,
    QMetaType::Void, 0x80000000 | 11,    7,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, 0x80000000 | 19,    7,

       0        // eod
};

void RadarScene::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RadarScene *_t = static_cast<RadarScene *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signal_xy((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->itemInserted((*reinterpret_cast< DiagramItem*(*)>(_a[1]))); break;
        case 2: _t->textInserted((*reinterpret_cast< QGraphicsTextItem*(*)>(_a[1]))); break;
        case 3: _t->itemSelected((*reinterpret_cast< QGraphicsItem*(*)>(_a[1]))); break;
        case 4: _t->setMode((*reinterpret_cast< Mode(*)>(_a[1]))); break;
        case 5: _t->setItemType((*reinterpret_cast< DiagramItem::DiagramType(*)>(_a[1]))); break;
        case 6: _t->editorLostFocus((*reinterpret_cast< DiagramTextItem*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QGraphicsTextItem* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QGraphicsItem* >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DiagramTextItem* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (RadarScene::*_t)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RadarScene::signal_xy)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RadarScene::*_t)(DiagramItem * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RadarScene::itemInserted)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (RadarScene::*_t)(QGraphicsTextItem * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RadarScene::textInserted)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (RadarScene::*_t)(QGraphicsItem * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RadarScene::itemSelected)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject RadarScene::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_RadarScene.data,
      qt_meta_data_RadarScene,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RadarScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RadarScene::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RadarScene.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsScene::qt_metacast(_clname);
}

int RadarScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void RadarScene::signal_xy(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RadarScene::itemInserted(DiagramItem * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RadarScene::textInserted(QGraphicsTextItem * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RadarScene::itemSelected(QGraphicsItem * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
