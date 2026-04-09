/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../src/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[19];
    char stringdata0[230];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 10), // "loadSchema"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 22), // "resetSelectedToDefault"
QT_MOC_LITERAL(4, 46, 5), // "about"
QT_MOC_LITERAL(5, 52, 18), // "loadSchemaFromPath"
QT_MOC_LITERAL(6, 71, 4), // "path"
QT_MOC_LITERAL(7, 76, 11), // "saveProfile"
QT_MOC_LITERAL(8, 88, 11), // "loadProfile"
QT_MOC_LITERAL(9, 100, 17), // "resetAllToDefault"
QT_MOC_LITERAL(10, 118, 18), // "onParameterChanged"
QT_MOC_LITERAL(11, 137, 4), // "name"
QT_MOC_LITERAL(12, 142, 5), // "value"
QT_MOC_LITERAL(13, 148, 18), // "onCategorySelected"
QT_MOC_LITERAL(14, 167, 11), // "QModelIndex"
QT_MOC_LITERAL(15, 179, 5), // "index"
QT_MOC_LITERAL(16, 185, 19), // "onSearchTextChanged"
QT_MOC_LITERAL(17, 205, 4), // "text"
QT_MOC_LITERAL(18, 210, 19) // "onParameterSelected"

    },
    "MainWindow\0loadSchema\0\0resetSelectedToDefault\0"
    "about\0loadSchemaFromPath\0path\0saveProfile\0"
    "loadProfile\0resetAllToDefault\0"
    "onParameterChanged\0name\0value\0"
    "onCategorySelected\0QModelIndex\0index\0"
    "onSearchTextChanged\0text\0onParameterSelected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
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
       3,    0,   70,    2, 0x08 /* Private */,
       4,    0,   71,    2, 0x08 /* Private */,
       5,    1,   72,    2, 0x08 /* Private */,
       7,    0,   75,    2, 0x08 /* Private */,
       8,    0,   76,    2, 0x08 /* Private */,
       9,    0,   77,    2, 0x08 /* Private */,
      10,    2,   78,    2, 0x08 /* Private */,
      13,    1,   83,    2, 0x08 /* Private */,
      16,    1,   86,    2, 0x08 /* Private */,
      18,    1,   89,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariant,   11,   12,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, 0x80000000 | 14,   15,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->loadSchema(); break;
        case 1: _t->resetSelectedToDefault(); break;
        case 2: _t->about(); break;
        case 3: _t->loadSchemaFromPath((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->saveProfile(); break;
        case 5: _t->loadProfile(); break;
        case 6: _t->resetAllToDefault(); break;
        case 7: _t->onParameterChanged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QVariant(*)>(_a[2]))); break;
        case 8: _t->onCategorySelected((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 9: _t->onSearchTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->onParameterSelected((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
