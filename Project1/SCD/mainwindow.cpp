#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Connect slots and signals */
    connect(ui->actionSCD, SIGNAL(triggered(bool)), this, SLOT(about(void)));

    aboutUi = new Ui::Dialog();
    aboutDialog = new QDialog(0, 0);
    aboutUi->setupUi(aboutDialog);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete aboutUi;
    delete aboutDialog;
}

void MainWindow::about(void)
{
    aboutDialog->show();
}
