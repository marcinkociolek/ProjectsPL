/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../HipsViever/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[16];
    char stringdata0[449];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 35), // "on_pushButtonSelectInFolder_c..."
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 37), // "on_ListWidgetFiles_currentTex..."
QT_MOC_LITERAL(4, 86, 11), // "currentText"
QT_MOC_LITERAL(5, 98, 34), // "on_spinBoxMinShowGray_valueCh..."
QT_MOC_LITERAL(6, 133, 4), // "arg1"
QT_MOC_LITERAL(7, 138, 34), // "on_spinBoxMaxShowGray_valueCh..."
QT_MOC_LITERAL(8, 173, 37), // "on_CheckBoxShowInputImageGray..."
QT_MOC_LITERAL(9, 211, 7), // "checked"
QT_MOC_LITERAL(10, 219, 35), // "on_CheckBoxShowInputImagePC_t..."
QT_MOC_LITERAL(11, 255, 41), // "on_spinBoxMinShowPseudoColor_..."
QT_MOC_LITERAL(12, 297, 41), // "on_spinBoxMaxShowPseudoColor_..."
QT_MOC_LITERAL(13, 339, 38), // "on_spinBoxMinShowGradient_val..."
QT_MOC_LITERAL(14, 378, 38), // "on_spinBoxMaxShowGradient_val..."
QT_MOC_LITERAL(15, 417, 31) // "on_CheckBoxShowGradient_toggled"

    },
    "MainWindow\0on_pushButtonSelectInFolder_clicked\0"
    "\0on_ListWidgetFiles_currentTextChanged\0"
    "currentText\0on_spinBoxMinShowGray_valueChanged\0"
    "arg1\0on_spinBoxMaxShowGray_valueChanged\0"
    "on_CheckBoxShowInputImageGray_toggled\0"
    "checked\0on_CheckBoxShowInputImagePC_toggled\0"
    "on_spinBoxMinShowPseudoColor_valueChanged\0"
    "on_spinBoxMaxShowPseudoColor_valueChanged\0"
    "on_spinBoxMinShowGradient_valueChanged\0"
    "on_spinBoxMaxShowGradient_valueChanged\0"
    "on_CheckBoxShowGradient_toggled"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x08 /* Private */,
       3,    1,   70,    2, 0x08 /* Private */,
       5,    1,   73,    2, 0x08 /* Private */,
       7,    1,   76,    2, 0x08 /* Private */,
       8,    1,   79,    2, 0x08 /* Private */,
      10,    1,   82,    2, 0x08 /* Private */,
      11,    1,   85,    2, 0x08 /* Private */,
      12,    1,   88,    2, 0x08 /* Private */,
      13,    1,   91,    2, 0x08 /* Private */,
      14,    1,   94,    2, 0x08 /* Private */,
      15,    1,   97,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Bool,    9,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_pushButtonSelectInFolder_clicked(); break;
        case 1: _t->on_ListWidgetFiles_currentTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->on_spinBoxMinShowGray_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_spinBoxMaxShowGray_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_CheckBoxShowInputImageGray_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->on_CheckBoxShowInputImagePC_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->on_spinBoxMinShowPseudoColor_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->on_spinBoxMaxShowPseudoColor_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_spinBoxMinShowGradient_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_spinBoxMaxShowGradient_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->on_CheckBoxShowGradient_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
