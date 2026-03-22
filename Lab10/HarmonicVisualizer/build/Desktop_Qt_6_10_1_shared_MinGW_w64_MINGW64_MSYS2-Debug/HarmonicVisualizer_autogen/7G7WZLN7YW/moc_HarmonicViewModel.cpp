/****************************************************************************
** Meta object code from reading C++ file 'HarmonicViewModel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/viewmodel/HarmonicViewModel.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HarmonicViewModel.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN17HarmonicViewModelE_t {};
} // unnamed namespace

template <> constexpr inline auto HarmonicViewModel::qt_create_metaobjectdata<qt_meta_tag_ZN17HarmonicViewModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "HarmonicViewModel",
        "HarmonicsListChanged",
        "",
        "ChartDataChanged",
        "SelectedHarmonicPropertiesChanged",
        "amplitude",
        "frequency",
        "phase",
        "FunctionType",
        "type",
        "AddHarmonic",
        "RemoveSelectedHarmonic",
        "UpdateSelectedHarmonic",
        "OnModelOscillatorsChanged",
        "OnModelPointsChanged"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'HarmonicsListChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'ChartDataChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'SelectedHarmonicPropertiesChanged'
        QtMocHelpers::SignalData<void(double, double, double, FunctionType)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 5 }, { QMetaType::Double, 6 }, { QMetaType::Double, 7 }, { 0x80000000 | 8, 9 },
        }}),
        // Slot 'AddHarmonic'
        QtMocHelpers::SlotData<void(double, double, double, FunctionType)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 5 }, { QMetaType::Double, 6 }, { QMetaType::Double, 7 }, { 0x80000000 | 8, 9 },
        }}),
        // Slot 'RemoveSelectedHarmonic'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'UpdateSelectedHarmonic'
        QtMocHelpers::SlotData<void(double, double, double, FunctionType)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 5 }, { QMetaType::Double, 6 }, { QMetaType::Double, 7 }, { 0x80000000 | 8, 9 },
        }}),
        // Slot 'OnModelOscillatorsChanged'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'OnModelPointsChanged'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<HarmonicViewModel, qt_meta_tag_ZN17HarmonicViewModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject HarmonicViewModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17HarmonicViewModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17HarmonicViewModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN17HarmonicViewModelE_t>.metaTypes,
    nullptr
} };

void HarmonicViewModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<HarmonicViewModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->HarmonicsListChanged(); break;
        case 1: _t->ChartDataChanged(); break;
        case 2: _t->SelectedHarmonicPropertiesChanged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<FunctionType>>(_a[4]))); break;
        case 3: _t->AddHarmonic((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<FunctionType>>(_a[4]))); break;
        case 4: _t->RemoveSelectedHarmonic(); break;
        case 5: _t->UpdateSelectedHarmonic((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<FunctionType>>(_a[4]))); break;
        case 6: _t->OnModelOscillatorsChanged(); break;
        case 7: _t->OnModelPointsChanged(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (HarmonicViewModel::*)()>(_a, &HarmonicViewModel::HarmonicsListChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (HarmonicViewModel::*)()>(_a, &HarmonicViewModel::ChartDataChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (HarmonicViewModel::*)(double , double , double , FunctionType )>(_a, &HarmonicViewModel::SelectedHarmonicPropertiesChanged, 2))
            return;
    }
}

const QMetaObject *HarmonicViewModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HarmonicViewModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN17HarmonicViewModelE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int HarmonicViewModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void HarmonicViewModel::HarmonicsListChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void HarmonicViewModel::ChartDataChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void HarmonicViewModel::SelectedHarmonicPropertiesChanged(double _t1, double _t2, double _t3, FunctionType _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2, _t3, _t4);
}
QT_WARNING_POP
