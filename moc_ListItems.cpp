/****************************************************************************
** Meta object code from reading C++ file 'ListItems.h'
**
** Created: Sat Feb 22 16:21:45 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "headers/ListItems.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ListItems.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ListItems[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x08,
      18,   10,   10,   10, 0x08,
      26,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ListItems[] = {
    "ListItems\0\0quit()\0color()\0"
    "colorSelectedItems()\0"
};

void ListItems::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ListItems *_t = static_cast<ListItems *>(_o);
        switch (_id) {
        case 0: _t->quit(); break;
        case 1: _t->color(); break;
        case 2: _t->colorSelectedItems(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData ListItems::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ListItems::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ListItems,
      qt_meta_data_ListItems, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ListItems::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ListItems::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ListItems::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ListItems))
        return static_cast<void*>(const_cast< ListItems*>(this));
    return QDialog::qt_metacast(_clname);
}

int ListItems::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
