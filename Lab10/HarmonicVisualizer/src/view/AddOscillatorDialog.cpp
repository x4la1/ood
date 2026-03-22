#include "AddOscillatorDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QDoubleValidator>

AddOscillatorDialog::AddOscillatorDialog(QWidget *parent) : QDialog(parent)
{
    SetupUi();
    setWindowTitle("Add new harmonic");

    auto* validator = new QDoubleValidator(this);
    validator->setNotation(QDoubleValidator::StandardNotation);
    m_ampEdit->setValidator(validator);
    m_freqEdit->setValidator(validator);
    m_phaseEdit->setValidator(validator);

    connect(m_ampEdit, &QLineEdit::textChanged, this, &AddOscillatorDialog::UpdatePreview);
    connect(m_freqEdit, &QLineEdit::textChanged, this, &AddOscillatorDialog::UpdatePreview);
    connect(m_phaseEdit, &QLineEdit::textChanged, this, &AddOscillatorDialog::UpdatePreview);
    connect(m_sinRadio, &QRadioButton::toggled, this, &AddOscillatorDialog::UpdatePreview);
    connect(m_cosRadio, &QRadioButton::toggled, this, &AddOscillatorDialog::UpdatePreview);

    UpdatePreview();
}

void AddOscillatorDialog::SetupUi()
{
    auto *mainLayout = new QVBoxLayout(this);
    auto *formLayout = new QFormLayout();

    m_ampEdit = new QLineEdit("1");
    m_freqEdit = new QLineEdit("1");
    m_phaseEdit = new QLineEdit("0");

    auto *radioLayout = new QHBoxLayout();
    m_sinRadio = new QRadioButton("Sin");
    m_cosRadio = new QRadioButton("Cos");
    m_sinRadio->setChecked(true);
    radioLayout->addWidget(m_sinRadio);
    radioLayout->addWidget(m_cosRadio);

    formLayout->addRow("Amplitude:", m_ampEdit);
    formLayout->addRow(radioLayout);
    formLayout->addRow("Frequency:", m_freqEdit);
    formLayout->addRow("Phase:", m_phaseEdit);

    m_previewLabel = new QLabel();
    m_previewLabel->setStyleSheet("font-weight: bold; margin: 10px 0; color: #2c3e50;");

    auto *btnLayout = new QHBoxLayout();
    auto *okBtn = new QPushButton("OK");
    auto *cancelBtn = new QPushButton("Cancel");
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(m_previewLabel);
    mainLayout->addLayout(btnLayout);

    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void AddOscillatorDialog::UpdatePreview()
{
    QString func = m_sinRadio->isChecked() ? "Sin" : "Cos";
    QString preview = QString("%1 * %2(%3 * x + %4)")
            .arg(m_ampEdit->text().isEmpty() ? "0" : m_ampEdit->text())
            .arg(func)
            .arg(m_freqEdit->text().isEmpty() ? "0" : m_freqEdit->text())
            .arg(m_phaseEdit->text().isEmpty() ? "0" : m_phaseEdit->text());

    m_previewLabel->setText(preview);
}

double AddOscillatorDialog::GetAmplitude() const
{
    return m_ampEdit->text().toDouble();
}

double AddOscillatorDialog::GetFrequency() const
{
    return m_freqEdit->text().toDouble();
}

double AddOscillatorDialog::GetPhase() const
{
    return m_phaseEdit->text().toDouble();
}

FunctionType AddOscillatorDialog::GetType() const
{
    return m_sinRadio->isChecked() ? FunctionType::Sin : FunctionType::Cos;
}
