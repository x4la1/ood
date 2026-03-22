#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QRadioButton>
#include <QLabel>
#include <QPushButton>
#include "../model/HarmonicOscillator.h"

class AddOscillatorDialog : public QDialog
{
    Q_OBJECT

public:
    AddOscillatorDialog(QWidget *parent = nullptr);

    double GetAmplitude() const;
    double GetFrequency() const;
    double GetPhase() const;
    FunctionType GetType() const;

private slots:
    void UpdatePreview();

private:
    void SetupUi();

    QLineEdit *m_ampEdit;
    QLineEdit *m_freqEdit;
    QLineEdit *m_phaseEdit;
    QRadioButton *m_sinRadio;
    QRadioButton *m_cosRadio;
    QLabel *m_previewLabel;
};
