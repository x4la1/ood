#include "view/MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    HarmonicModel model;
    HarmonicViewModel viewModel(&model);
    MainWindow w(&viewModel);

    w.show();
    return a.exec();
}
