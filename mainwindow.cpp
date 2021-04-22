#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*bool MainWindow::checkoper()
{
    if (ui->backspaceButton->isChecked())
    return true;
    if (ui->pointButton->isChecked())
    return true;
    return false;
}
*/
