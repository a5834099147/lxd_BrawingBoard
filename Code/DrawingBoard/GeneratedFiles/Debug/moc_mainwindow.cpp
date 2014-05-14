/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../GUI/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      34,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   12,   11,   11, 0x05,
      66,   53,   11,   11, 0x05,
     109,   97,   11,   11, 0x05,
     155,  141,   11,   11, 0x05,
     206,  191,   11,   11, 0x05,
     248,  239,   11,   11, 0x05,
     267,  239,   11,   11, 0x05,
     294,  285,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     318,   11,   11,   11, 0x08,
     344,   11,   11,   11, 0x08,
     370,   11,   11,   11, 0x08,
     397,   11,   11,   11, 0x08,
     428,   11,   11,   11, 0x08,
     456,   11,   11,   11, 0x08,
     486,   11,   11,   11, 0x08,
     518,   11,   11,   11, 0x08,
     549,   11,   11,   11, 0x08,
     581,   11,   11,   11, 0x08,
     612,   11,   11,   11, 0x08,
     645,   11,   11,   11, 0x08,
     677,   11,   11,   11, 0x08,
     711,   11,   11,   11, 0x08,
     744,   11,   11,   11, 0x08,
     778,   11,   11,   11, 0x08,
     811,   11,   11,   11, 0x08,
     852,  846,   11,   11, 0x08,
     877,  846,   11,   11, 0x08,
     904,   11,   11,   11, 0x08,
     930,   11,   11,   11, 0x08,
     958,   11,   11,   11, 0x08,
     984,   11,   11,   11, 0x08,
    1009,   11,   11,   11, 0x08,
    1042, 1025,   11,   11, 0x08,
    1111, 1078,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0newShape\0"
    "changeCurrentShape(Shape::Code)\0"
    "newPenColour\0changePenColour(Shape::Colour)\0"
    "newPenStyle\0changePenStyle(Shape::PenStyle)\0"
    "newBrushStyle\0changeBrushStyle(Shape::BrushStyle)\0"
    "newBurshColour\0changeBrushColour(Shape::Colour)\0"
    "fileName\0writeFile(QString)\0"
    "readFile(QString)\0usbState\0"
    "changeUsbState(QString)\0"
    "drawLineActionTriggered()\0"
    "drawRectActionTriggered()\0"
    "drawRoundActionTriggered()\0"
    "drawRoundRectActionTriggered()\0"
    "drawPointsActionTriggered()\0"
    "colourPenRedActionTriggered()\0"
    "colourPenGreenActionTriggered()\0"
    "colourPenBlueActionTriggered()\0"
    "colourPenBlackActionTriggered()\0"
    "colourPenNoneActionTriggered()\0"
    "colourPenCustomActionTriggered()\0"
    "colourBrushRedActionTriggered()\0"
    "colourBrushGreenActionTriggered()\0"
    "colourBrushBlueActionTriggered()\0"
    "colourBrushBlackActionTriggered()\0"
    "colourBrushNoneActionTriggered()\0"
    "colourBrushCustomActionTriggered()\0"
    "style\0findPenStyleChanged(int)\0"
    "findBrushStyleChanged(int)\0"
    "on_openAction_triggered()\0"
    "on_saveAsAction_triggered()\0"
    "on_saveAction_triggered()\0"
    "on_newAction_triggered()\0dateIsChanged()\0"
    "account,password\0sand_login(std::string,std::string)\0"
    "account,password,userName,pinYin\0"
    "sand_register(std::string,std::string,std::string,std::string)\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->changeCurrentShape((*reinterpret_cast< Shape::Code(*)>(_a[1]))); break;
        case 1: _t->changePenColour((*reinterpret_cast< Shape::Colour(*)>(_a[1]))); break;
        case 2: _t->changePenStyle((*reinterpret_cast< Shape::PenStyle(*)>(_a[1]))); break;
        case 3: _t->changeBrushStyle((*reinterpret_cast< Shape::BrushStyle(*)>(_a[1]))); break;
        case 4: _t->changeBrushColour((*reinterpret_cast< Shape::Colour(*)>(_a[1]))); break;
        case 5: _t->writeFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->readFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->changeUsbState((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->drawLineActionTriggered(); break;
        case 9: _t->drawRectActionTriggered(); break;
        case 10: _t->drawRoundActionTriggered(); break;
        case 11: _t->drawRoundRectActionTriggered(); break;
        case 12: _t->drawPointsActionTriggered(); break;
        case 13: _t->colourPenRedActionTriggered(); break;
        case 14: _t->colourPenGreenActionTriggered(); break;
        case 15: _t->colourPenBlueActionTriggered(); break;
        case 16: _t->colourPenBlackActionTriggered(); break;
        case 17: _t->colourPenNoneActionTriggered(); break;
        case 18: _t->colourPenCustomActionTriggered(); break;
        case 19: _t->colourBrushRedActionTriggered(); break;
        case 20: _t->colourBrushGreenActionTriggered(); break;
        case 21: _t->colourBrushBlueActionTriggered(); break;
        case 22: _t->colourBrushBlackActionTriggered(); break;
        case 23: _t->colourBrushNoneActionTriggered(); break;
        case 24: _t->colourBrushCustomActionTriggered(); break;
        case 25: _t->findPenStyleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->findBrushStyleChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->on_openAction_triggered(); break;
        case 28: _t->on_saveAsAction_triggered(); break;
        case 29: _t->on_saveAction_triggered(); break;
        case 30: _t->on_newAction_triggered(); break;
        case 31: _t->dateIsChanged(); break;
        case 32: _t->sand_login((*reinterpret_cast< std::string(*)>(_a[1])),(*reinterpret_cast< std::string(*)>(_a[2]))); break;
        case 33: _t->sand_register((*reinterpret_cast< std::string(*)>(_a[1])),(*reinterpret_cast< std::string(*)>(_a[2])),(*reinterpret_cast< std::string(*)>(_a[3])),(*reinterpret_cast< std::string(*)>(_a[4]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 34)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 34;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::changeCurrentShape(Shape::Code _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::changePenColour(Shape::Colour _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainWindow::changePenStyle(Shape::PenStyle _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainWindow::changeBrushStyle(Shape::BrushStyle _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MainWindow::changeBrushColour(Shape::Colour _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MainWindow::writeFile(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MainWindow::readFile(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void MainWindow::changeUsbState(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
QT_END_MOC_NAMESPACE
