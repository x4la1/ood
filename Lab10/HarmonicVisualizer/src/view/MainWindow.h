#pragma once
#include <QMainWindow>
#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "../viewmodel/HarmonicViewModel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(HarmonicViewModel* viewModel, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void UpdateHarmonicsList();
    void UpdateEditFields(double amp, double freq, double phase, FunctionType type);
    void UpdateChart();

    void OnListSelectionChanged();
    void OnApplyChangesClicked();
    void OnAddClicked();
    void OnRemoveClicked();

private:
    void SetupUi();
    void MakeConnections();

    HarmonicViewModel *m_viewModel;

    QListWidget *m_listWidget;
    QLineEdit *m_ampEdit, *m_freqEdit, *m_phaseEdit;
    QRadioButton *m_sinRadio, *m_cosRadio;

    QChartView *m_chartView;
    QLineSeries *m_series;

    Ui::MainWindow *ui;
};

