/****************************************************************************
** Meta object code from reading C++ file 'HarmonicModel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/model/HarmonicModel.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HarmonicModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN13HarmonicModelE_t {};
} // unnamed namespace

template <> constexpr inline auto HarmonicModel::qt_create_metaobjectdata<qt_meta_tag_ZN13HarmonicModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "HarmonicModel",
        "OscillatorAdded",
        "",
        "HarmonicOscillator",
        "oscillator",
        "OscillatorRemoved",
        "QUuid",
        "id",
        "OscillatorUpdated",
        "HarmonicPointsChanged"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'OscillatorAdded'
        QtMocHelpers::SignalData<void(const HarmonicOscillator &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'OscillatorRemoved'
        QtMocHelpers::SignalData<void(const QUuid &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 6, 7 },
        }}),
        // Signal 'OscillatorUpdated'
        QtMocHelpers::SignalData<void(const HarmonicOscillator &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'HarmonicPointsChanged'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<HarmonicModel, qt_meta_tag_ZN13HarmonicModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject HarmonicModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13HarmonicModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13HarmonicModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13HarmonicModelE_t>.metaTypes,
    nullptr
} };

void HarmonicModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<HarmonicModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->OscillatorAdded((*reinterpret_cast<std::add_pointer_t<HarmonicOscillator>>(_a[1]))); break;
        case 1: _t->OscillatorRemoved((*reinterpret_cast<std::add_pointer_t<QUuid>>(_a[1]))); break;
        case 2: _t->OscillatorUpdated((*reinterpret_cast<std::add_pointer_t<HarmonicOscillator>>(_a[1]))); break;
        case 3: _t->HarmonicPointsChanged(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (HarmonicModel::*)(const HarmonicOscillator & )>(_a, &HarmonicModel::OscillatorAdded, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (HarmonicModel::*)(const QUuid & )>(_a, &HarmonicModel::OscillatorRemoved, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (HarmonicModel::*)(const HarmonicOscillator & )>(_a, &HarmonicModel::OscillatorUpdated, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (HarmonicModel::*)()>(_a, &HarmonicModel::HarmonicPointsChanged, 3))
            return;
    }
}

const QMetaObject *HarmonicModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HarmonicModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13HarmonicModelE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int HarmonicModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void HarmonicModel::OscillatorAdded(const HarmonicOscillator & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void HarmonicModel::OscillatorRemoved(const QUuid & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void HarmonicModel::OscillatorUpdated(const HarmonicOscillator & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void HarmonicModel::HarmonicPointsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
