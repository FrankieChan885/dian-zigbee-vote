/****************************************************************************
** Meta object code from reading C++ file 'histgram.h'
**
** Created: Tue Apr 26 06:23:53 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../histgram.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'histgram.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Histgram[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   10,    9,    9, 0x0a,
      42,    9,    9,    9, 0x0a,
      65,    9,    9,    9, 0x0a,
      88,    9,    9,    9, 0x0a,
     112,    9,    9,    9, 0x0a,
     140,  135,    9,    9, 0x0a,
     163,    9,    9,    9, 0x0a,
     186,  135,    9,    9, 0x0a,
     227,  213,    9,    9, 0x0a,
     257,  253,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Histgram[] = {
    "Histgram\0\0optionName\0updateData(QString&)\0"
    "setyAxisDashLineFlag()\0setSolidPatternStyle()\0"
    "setDense5PatternStyle()\0setCrossPatternStyle()\0"
    "flag\0setRectangelStyle(int)\0"
    "setUsetCorrectAnswer()\0"
    "setUsetCorrectAnswer(bool)\0correctAnswer\0"
    "setCorrectAnswer(QString)\0set\0"
    "setColorSet(QList<QString>&)\0"
};

const QMetaObject Histgram::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Histgram,
      qt_meta_data_Histgram, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Histgram::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Histgram::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Histgram::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Histgram))
        return static_cast<void*>(const_cast< Histgram*>(this));
    return QWidget::qt_metacast(_clname);
}

int Histgram::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: updateData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: setyAxisDashLineFlag(); break;
        case 2: setSolidPatternStyle(); break;
        case 3: setDense5PatternStyle(); break;
        case 4: setCrossPatternStyle(); break;
        case 5: setRectangelStyle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: setUsetCorrectAnswer(); break;
        case 7: setUsetCorrectAnswer((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: setCorrectAnswer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: setColorSet((*reinterpret_cast< QList<QString>(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
