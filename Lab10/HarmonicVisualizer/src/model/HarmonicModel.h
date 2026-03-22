#pragma once

#include <QObject>
#include <QVector>
#include <QUuid>
#include <QPointF>
#include <QDebug>
#include <QStringList>
#include "HarmonicOscillator.h"
#include <optional>

class HarmonicModel : public QObject
{
    Q_OBJECT

public:
    HarmonicModel(QObject *parent = nullptr);

    void AddOscillator(double amplitude, double frequency, double phase, FunctionType type);
    void RemoveOscillator(const QUuid& id);
    void UpdateOscillator(const QUuid& id, double amplitude, double frequency, double phase, FunctionType type);
    std::optional<HarmonicOscillator> GetOscillator(const QUuid& id);

    const QVector<HarmonicOscillator>& GetOscillators() const;

    QVector<QPointF> GetHarmonicPoints(double xMin, double xMax, int numPoints) const;

signals:
    void OscillatorAdded(const HarmonicOscillator& oscillator);
    void OscillatorRemoved(const QUuid& id);
    void OscillatorUpdated(const HarmonicOscillator& oscillator);
    void HarmonicPointsChanged();

private:
    QVector<HarmonicOscillator> m_oscillators;
};
