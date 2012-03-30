#include "MainWindow.hpp"
#include "ui_MainWindow.h"

namespace Gecon {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

} // namespace Gecon
