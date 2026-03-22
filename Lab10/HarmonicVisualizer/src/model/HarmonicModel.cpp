#include "HarmonicModel.h"

HarmonicModel::HarmonicModel(QObject *parent)
{
}

void HarmonicModel::AddOscillator(double amplitude, double frequency, double phase, FunctionType type)
{
    m_oscillators.append(HarmonicOscillator(amplitude, frequency, phase, type));
    emit OscillatorAdded(m_oscillators.last());
    emit HarmonicPointsChanged();
}

void HarmonicModel::RemoveOscillator(const QUuid& id)
{
    for(int i = 0; i < m_oscillators.size(); ++i)
    {
        if(m_oscillators[i].GetId() == id)
        {
            m_oscillators.removeAt(i);
            emit OscillatorRemoved(id);
            emit HarmonicPointsChanged();
            return;
        }
    }

    qWarning() << "There is no oscillator with current id";
}

void HarmonicModel::UpdateOscillator(const QUuid& id, double amplitude, double frequency, double phase, FunctionType type)
{
    for(HarmonicOscillator& osc: m_oscillators)
    {
        if(osc.GetId() == id)
        {
            osc.SetAmplitude(amplitude);
            osc.SetFrequency(frequency);
            osc.SetPhase(phase);
            osc.SetType(type);
            emit OscillatorUpdated(osc);
            emit HarmonicPointsChanged();
            return;
        }
    }

    qWarning() << "There is no oscillator with current id";
}

std::optional<HarmonicOscillator> HarmonicModel::GetOscillator(const QUuid& id)
{
    for(const HarmonicOscillator& osc: m_oscillators)
    {
        if(osc.GetId() == id)
        {
            return osc;
        }
    }

    return std::nullopt;
}


const QVector<HarmonicOscillator>& HarmonicModel::GetOscillators() const
{
    return m_oscillators;
}

QVector<QPointF> HarmonicModel::GetHarmonicPoints(double xMin, double xMax, int pointsCount) const
{
    QVector<QPointF> points;
    points.reserve(pointsCount);

    if(pointsCount < 2)
    {
        return points;
    }

    double step = (xMax - xMin)/ (pointsCount - 1);
    double x, ySum;

    for(int i = 0; i < pointsCount; ++i)
    {
        x = xMin + i * step;
        ySum = 0.0;

        for(const HarmonicOscillator& osc: m_oscillators)
        {
            ySum += osc.Calculate(x);
        }

        points.append(QPointF(x, ySum));
    }

    return points;
}
