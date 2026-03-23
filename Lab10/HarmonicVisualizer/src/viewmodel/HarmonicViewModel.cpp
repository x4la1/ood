#include "HarmonicViewModel.h"

HarmonicViewModel::HarmonicViewModel(HarmonicModel *model, QObject *parent)
    : QObject(parent),
      m_model(model)
{
    Q_ASSERT(m_model != nullptr);

    connect(m_model, &HarmonicModel::OscillatorAdded, this, &HarmonicViewModel::OnModelOscillatorsChanged);
    connect(m_model, &HarmonicModel::OscillatorRemoved, this, &HarmonicViewModel::OnModelOscillatorsChanged);
    connect(m_model, &HarmonicModel::OscillatorUpdated, this, &HarmonicViewModel::OnModelOscillatorsChanged);
    connect(m_model, &HarmonicModel::HarmonicPointsChanged, this, &HarmonicViewModel::OnModelPointsChanged);
}

QStringList HarmonicViewModel::GetHarmonicStrings()
{
    QStringList strings;
    m_idMap.clear();

    const auto& oscillators = m_model->GetOscillators();
    for (const auto& osc : oscillators)
    {
        strings.append(FormatHarmonicString(osc));
        m_idMap.append(osc.GetId());
    }

    return strings;
}

QVector<QPointF> HarmonicViewModel::GetChartPoints() const
{
    return m_model->GetHarmonicPoints(m_xMin, m_xMax, m_pointsCount);
}

void HarmonicViewModel::SelectHarmonic(int index)
{
    if (index < 0 || index >= m_idMap.size())
    {
        m_selectedId = QUuid();
        return;
    }

    m_selectedId = m_idMap[index];

    if (auto osc = m_model->GetOscillator(m_selectedId))
    {
        emit SelectedHarmonicPropertiesChanged(
                    osc->GetAmplitude(),
                    osc->GetFrequency(),
                    osc->GetPhase(),
                    osc->GetFunctionType()
                    );
    }
}

QUuid HarmonicViewModel::GetSelectedId() const
{
    return m_selectedId;
}

void HarmonicViewModel::AddHarmonic(double amplitude, double frequency, double phase, FunctionType type)
{
    m_model->AddOscillator(amplitude, frequency, phase, type);
}

void HarmonicViewModel::RemoveSelectedHarmonic()
{
    if (!m_selectedId.isNull())
    {
        m_model->RemoveOscillator(m_selectedId);
        m_selectedId = QUuid();
    }
}

void HarmonicViewModel::UpdateSelectedHarmonic(double amplitude, double frequency, double phase, FunctionType type)
{
    if (!m_selectedId.isNull())
    {
        m_model->UpdateOscillator(m_selectedId, amplitude, frequency, phase, type);
    }
}

void HarmonicViewModel::OnModelOscillatorsChanged()
{
    emit HarmonicsListChanged();
}

void HarmonicViewModel::OnModelPointsChanged()
{
    emit ChartDataChanged();
}

QString HarmonicViewModel::FormatHarmonicString(const HarmonicOscillator& osc) const
{
    QString funcName = (osc.GetFunctionType() == FunctionType::Sin) ? "sin" : "cos";

    QString freqStr = QString::number(osc.GetFrequency());
    if (osc.GetFrequency() == 1.0)
    {
        freqStr = "x";
    }
    else if (osc.GetFrequency() == -1.0)
    {
        freqStr = "-x";
    }
    else
    {
        freqStr += "*x";
    }

    QString phaseStr = "";
    if (osc.GetPhase() > 0)
    {
        phaseStr = " + " + QString::number(osc.GetPhase());
    }
    else if (osc.GetPhase() < 0)
    {
        phaseStr = " - " + QString::number(qAbs(osc.GetPhase()));
    }

    return QString::number(osc.GetAmplitude()) + " * " + funcName + "(" + freqStr + phaseStr + ")";
}
