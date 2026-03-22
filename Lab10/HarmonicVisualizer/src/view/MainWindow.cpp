#pragma once
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "AddOscillatorDialog.h"
#include <QFormLayout>

MainWindow::MainWindow(HarmonicViewModel *viewModel, QWidget *parent)
    : QMainWindow(parent), m_viewModel(viewModel)
{
    SetupUi();
    MakeConnections();

    UpdateHarmonicsList();
}

void MainWindow::SetupUi()
{
    auto *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto *mainLayout = new QVBoxLayout(centralWidget);
    auto *topLayout = new QHBoxLayout();

    auto *leftLayout = new QVBoxLayout();
    m_listWidget = new QListWidget();

    auto *listButtonsLayout = new QHBoxLayout();
    auto *addBtn = new QPushButton("Add New");
    auto *removeBtn = new QPushButton("Remove Selected");
    listButtonsLayout->addWidget(addBtn);
    listButtonsLayout->addWidget(removeBtn);

    leftLayout->addWidget(new QLabel("Harmonics List:"));
    leftLayout->addWidget(m_listWidget);
    leftLayout->addLayout(listButtonsLayout);

    // --- ПРАВАЯ КОЛОНКА (Редактирование) ---
    auto *rightLayout = new QFormLayout();
    m_ampEdit = new QLineEdit();
    m_freqEdit = new QLineEdit();
    m_phaseEdit = new QLineEdit();

    auto *typeLayout = new QHBoxLayout();
    m_sinRadio = new QRadioButton("Sin");
    m_cosRadio = new QRadioButton("Cos");
    typeLayout->addWidget(m_sinRadio);
    typeLayout->addWidget(m_cosRadio);

    auto *applyBtn = new QPushButton("Apply Changes");

    rightLayout->addRow(new QLabel("Edit Selected:"));
    rightLayout->addRow("Function:", typeLayout);
    rightLayout->addRow("Amplitude:", m_ampEdit);
    rightLayout->addRow("Frequency:", m_freqEdit);
    rightLayout->addRow("Phase:", m_phaseEdit);
    rightLayout->addRow(applyBtn);

    // Собираем верхнюю часть
    topLayout->addLayout(leftLayout, 1);  // Вес 1
    topLayout->addLayout(rightLayout, 1); // Вес 1

    // --- НИЖНЯЯ ЧАСТЬ (График) ---
    m_series = new QLineSeries();
    auto *chart = new QChart();
    chart->addSeries(m_series);
    chart->createDefaultAxes();
    chart->legend()->hide();
    chart->setTitle("Summed Harmonic Oscillations");

    m_chartView = new QChartView(chart);
    m_chartView->setRenderHint(QPainter::Antialiasing); // Чтобы линия была гладкой

    // Собираем всё в главное окно
    mainLayout->addLayout(topLayout, 1);
    mainLayout->addWidget(m_chartView, 2); // График занимает больше места

    // Базовые коннекты кнопок
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::OnAddClicked);
    connect(removeBtn, &QPushButton::clicked, this, &MainWindow::OnRemoveClicked);
    connect(applyBtn, &QPushButton::clicked, this, &MainWindow::OnApplyChangesClicked);
}

void MainWindow::MakeConnections()
{
    connect(m_viewModel, &HarmonicViewModel::HarmonicsListChanged, this, &MainWindow::UpdateHarmonicsList);

    connect(m_viewModel, &HarmonicViewModel::SelectedHarmonicPropertiesChanged, this, &MainWindow::UpdateEditFields);

    connect(m_viewModel, &HarmonicViewModel::ChartDataChanged, this, &MainWindow::UpdateChart);

    connect(m_listWidget, &QListWidget::currentRowChanged, this, &MainWindow::OnListSelectionChanged);
}

void MainWindow::UpdateHarmonicsList()
{
    m_listWidget->clear();
    m_listWidget->addItems(m_viewModel->GetHarmonicStrings());
}

void MainWindow::UpdateEditFields(double amplitude, double frequency, double phase, FunctionType type)
{
    m_ampEdit->setText(QString::number(amplitude));
    m_freqEdit->setText(QString::number(frequency));
    m_phaseEdit->setText(QString::number(phase));

    if (type == FunctionType::Sin)
    {
        m_sinRadio->setChecked(true);
    }
    else
    {
        m_cosRadio->setChecked(true);
    }
}

void MainWindow::UpdateChart()
{
    auto points = m_viewModel->GetChartPoints();
        m_series->replace(points);

        if (!points.isEmpty())
        {
            m_chartView->chart()->axes(Qt::Horizontal).first()->setRange(0, 5);
            m_chartView->chart()->axes(Qt::Vertical).first()->setRange(-10, 10);
        }
}

void MainWindow::OnListSelectionChanged()
{
    int index = m_listWidget->currentRow();
    m_viewModel->SelectHarmonic(index);
}

void MainWindow::OnAddClicked()
{
    AddOscillatorDialog dlg(this);

    if (dlg.exec() == QDialog::Accepted)
    {
        m_viewModel->AddHarmonic
        (
            dlg.GetAmplitude(),
            dlg.GetFrequency(),
            dlg.GetPhase(),
            dlg.GetType()
        );
    }
}

void MainWindow::OnApplyChangesClicked()
{
    if (m_listWidget->currentRow() == -1) return;

    double amp = m_ampEdit->text().toDouble();
    double freq = m_freqEdit->text().toDouble();
    double phase = m_phaseEdit->text().toDouble();
    FunctionType type = m_sinRadio->isChecked() ? FunctionType::Sin : FunctionType::Cos;

    m_viewModel->UpdateSelectedHarmonic(amp, freq, phase, type);
}

void MainWindow::OnRemoveClicked()
{
    m_viewModel->RemoveSelectedHarmonic();
}

MainWindow::~MainWindow()
{
    delete ui;
}

