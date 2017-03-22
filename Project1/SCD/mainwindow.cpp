#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include "serialporthandler.h"

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

    QLineSeries *series = new QLineSeries();
    series->append(0, 0);
    series->append(1, 0);
    series->append(2, 1);
    series->append(3, 4);
    series->append(4, 2);
    series->append(5, 0);

    /* Creates the chart of the mode 1 */
    chartMode1 = new QChart();
    chartMode1->legend()->hide();
    chartMode1->addSeries(series);
    chartMode1->createDefaultAxes();
    chartMode1->setTitle("Mode 1");
    chartView = new QChartView(chartMode1);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout = new QVBoxLayout();
    layout->addWidget(chartView);
    layout->setMargin(0);


    ui->chartFrame->setLayout(layout);

    sph = new SerialPortHandler();

    if(!sph->startReadingWriting())
    {
        QMessageBox::information(
                this,
                tr("SCD"),
                tr("Couldn't connect to the microcontroller!")
        );
        QTimer::singleShot(0, this, SLOT(close()));

    }

}

MainWindow::~MainWindow()
{
    delete ui;
    delete aboutUi;
    delete aboutDialog;
    delete chartMode1;
    delete chartView;
    delete layout;
    delete sph;
}

void MainWindow::about(void)
{
    aboutDialog->show();
}


