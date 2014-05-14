/****************************************************************************
** Meta object code from reading C++ file 'paintwidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../GUI/paintwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'paintwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PaintWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      31,   29,   12,   12, 0x0a,
      62,   60,   12,   12, 0x0a,
      83,   29,   12,   12, 0x0a,
     112,   29,   12,   12, 0x0a,
     147,  145,   12,   12, 0x0a,
     175,   12,   12,   12, 0x0a,
     219,  210,  205,   12, 0x0a,
     238,  210,  205,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PaintWidget[] = {
    "PaintWidget\0\0dateIsChanged()\0s\0"
    "setCurrentShape(Shape::Code)\0r\0"
    "setPenRoughNess(int)\0setPenStyle(Shape::PenStyle)\0"
    "setBrushStyle(Shape::BrushStyle)\0c\0"
    "setPenColour(Shape::Colour)\0"
    "setBrushColour(Shape::Colour)\0bool\0"
    "fileName\0writeFile(QString)\0"
    "readFile(QString)\0"
};

void PaintWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        PaintWidget *_t = static_cast<PaintWidget *>(_o);
        switch (_id) {
        case 0: _t->dateIsChanged(); break;
        case 1: _t->setCurrentShape((*reinterpret_cast< Shape::Code(*)>(_a[1]))); break;
        case 2: _t->setPenRoughNess((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->setPenStyle((*reinterpret_cast< Shape::PenStyle(*)>(_a[1]))); break;
        case 4: _t->setBrushStyle((*reinterpret_cast< Shape::BrushStyle(*)>(_a[1]))); break;
        case 5: _t->setPenColour((*reinterpret_cast< Shape::Colour(*)>(_a[1]))); break;
        case 6: _t->setBrushColour((*reinterpret_cast< Shape::Colour(*)>(_a[1]))); break;
        case 7: { bool _r = _t->writeFile((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: { bool _r = _t->readFile((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData PaintWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PaintWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PaintWidget,
      qt_meta_data_PaintWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PaintWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PaintWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PaintWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PaintWidget))
        return static_cast<void*>(const_cast< PaintWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int PaintWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void PaintWidget::dateIsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
