/****************************************************************************
** Meta object code from reading C++ file 'OscillatorViewModel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/viewmodel/OscillatorViewModel.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'OscillatorViewModel.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN19OscillatorViewModelE_t {};
} // unnamed namespace

template <> constexpr inline auto OscillatorViewModel::qt_create_metaobjectdata<qt_meta_tag_ZN19OscillatorViewModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "OscillatorViewModel",
        "AmplitudeChanged",
        "",
        "FrequencyChanged",
        "PhaseChanged",
        "FunctionTypeChanged",
        "OscillatorDeleted",
        "SetAmplitude",
        "amplitude",
        "SetFrequency",
        "frequency",
        "SetPhase",
        "phase",
        "SetFunctionType",
        "index",
        "waveTypeIndex"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'AmplitudeChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'FrequencyChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'PhaseChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'FunctionTypeChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'OscillatorDeleted'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'SetAmplitude'
        QtMocHelpers::SlotData<void(double)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 8 },
        }}),
        // Slot 'SetFrequency'
        QtMocHelpers::SlotData<void(double)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 10 },
        }}),
        // Slot 'SetPhase'
        QtMocHelpers::SlotData<void(double)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 12 },
        }}),
        // Slot 'SetFunctionType'
        QtMocHelpers::SlotData<void(int)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 14 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'amplitude'
        QtMocHelpers::PropertyData<double>(8, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable, 0),
        // property 'frequency'
        QtMocHelpers::PropertyData<double>(10, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable, 1),
        // property 'phase'
        QtMocHelpers::PropertyData<double>(12, QMetaType::Double, QMC::DefaultPropertyFlags | QMC::Writable, 2),
        // property 'waveTypeIndex'
        QtMocHelpers::PropertyData<int>(15, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable, 3),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<OscillatorViewModel, qt_meta_tag_ZN19OscillatorViewModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject OscillatorViewModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19OscillatorViewModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19OscillatorViewModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN19OscillatorViewModelE_t>.metaTypes,
    nullptr
} };

void OscillatorViewModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<OscillatorViewModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->AmplitudeChanged(); break;
        case 1: _t->FrequencyChanged(); break;
        case 2: _t->PhaseChanged(); break;
        case 3: _t->FunctionTypeChanged(); break;
        case 4: _t->OscillatorDeleted(); break;
        case 5: _t->SetAmplitude((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 6: _t->SetFrequency((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 7: _t->SetPhase((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 8: _t->SetFunctionType((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (OscillatorViewModel::*)()>(_a, &OscillatorViewModel::AmplitudeChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (OscillatorViewModel::*)()>(_a, &OscillatorViewModel::FrequencyChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (OscillatorViewModel::*)()>(_a, &OscillatorViewModel::PhaseChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (OscillatorViewModel::*)()>(_a, &OscillatorViewModel::FunctionTypeChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (OscillatorViewModel::*)()>(_a, &OscillatorViewModel::OscillatorDeleted, 4))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<double*>(_v) = _t->GetAmplitude(); break;
        case 1: *reinterpret_cast<double*>(_v) = _t->GetFrequency(); break;
        case 2: *reinterpret_cast<double*>(_v) = _t->GetPhase(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->GetFunctionTypeIndex(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->SetAmplitude(*reinterpret_cast<double*>(_v)); break;
        case 1: _t->SetFrequency(*reinterpret_cast<double*>(_v)); break;
        case 2: _t->SetPhase(*reinterpret_cast<double*>(_v)); break;
        case 3: _t->SetFunctionType(*reinterpret_cast<int*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *OscillatorViewModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OscillatorViewModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19OscillatorViewModelE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int OscillatorViewModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void OscillatorViewModel::AmplitudeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void OscillatorViewModel::FrequencyChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void OscillatorViewModel::PhaseChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void OscillatorViewModel::FunctionTypeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void OscillatorViewModel::OscillatorDeleted()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
