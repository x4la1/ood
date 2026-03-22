#pragma once
#include "../model/HarmonicModel.h"


class HarmonicViewModel : public QObject
{
    Q_OBJECT

public:
    HarmonicViewModel(HarmonicModel* model, QObject* parent = nullptr);

    QStringList GetHarmonicStrings();
    QVector<QPointF> GetChartPoints() const;

    void SelectHarmonic(int index);
    QUuid GetSelectedId() const;

    void SetChartRange(double xMin, double xMax, int pointsCount);

public slots:
    void AddHarmonic(double amplitude, double frequency, double phase, FunctionType type);
    void RemoveSelectedHarmonic();
    void UpdateSelectedHarmonic(double amplitude, double frequency, double phase, FunctionType type);

private slots:
    void OnModelOscillatorsChanged();
    void OnModelPointsChanged();

signals:
    void HarmonicsListChanged();

    void ChartDataChanged();

    void SelectedHarmonicPropertiesChanged(double amplitude, double frequency, double phase, FunctionType type);

private:
    QString FormatHarmonicString(const HarmonicOscillator& osc) const;

    HarmonicModel *m_model;

    QUuid m_selectedId;
    QList<QUuid> m_idMap;

    double m_xMin = 0.0;
    double m_xMax = 5.0;
    int m_pointsCount = 1000;
};
