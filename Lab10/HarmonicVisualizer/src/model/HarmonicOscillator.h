#pragma once

#include <QUuid>
#include <QtMath>

enum class FunctionType
{
    Sin,
    Cos
};

class HarmonicOscillator
{
public:
    HarmonicOscillator(double amplitude, double frequency, double phase, FunctionType type)
        : m_id(QUuid::createUuid()), m_amplitude(amplitude), m_frequency(frequency), m_phase(phase), m_type(type)
    {
    }

    double Calculate(double x) const
    {
        double argument = m_frequency * x + m_phase;
        if (m_type == FunctionType::Sin)
        {
            return m_amplitude * qSin(argument);
        }
        else
        {
            return m_amplitude * qCos(argument);
        }
    }

    QUuid GetId() const
    {
        return m_id;
    }

    double GetAmplitude() const
    {
        return m_amplitude;
    }

    double GetFrequency() const
    {
        return m_frequency;
    }

    double GetPhase() const
    {
        return m_phase;
    }

    FunctionType GetFunctionType() const
    {
        return m_type;
    }

    void SetAmplitude(double amplitude)
    {
        m_amplitude = amplitude;
    }

    void SetFrequency(double frequency)
    {
        m_frequency = frequency;
    }

    void SetPhase(double phase)
    {
        m_phase = phase;
    }

    void SetType(FunctionType type)
    {
        m_type = type;
    }

private:
    QUuid m_id;
    double m_amplitude;
    double m_frequency;
    double m_phase;
    FunctionType m_type;
};
