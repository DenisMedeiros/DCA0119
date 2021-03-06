#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTimer>

#include "serialporthandler.h"

#define TIMER_VALUE 1000

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Default operation mode is 1. */
    mode = 1;

    /* Configure all widgets. */
    aboutUi = new Ui::Dialog();
    aboutDialog = new QDialog(0, 0);
    aboutUi->setupUi(aboutDialog);

    serialBuffer = new QByteArray();
    timer = new QTimer();

    seriesLine = new QLineSeries();
    seriesPoints = new QScatterSeries();
    seriesPoints->setMarkerSize(10);

    /* Creates the chart of the mode 1 */
    chart = new QChart();
    chart->addSeries(seriesLine);
    chart->addSeries(seriesPoints);

    chart->createDefaultAxes();
    chart->axisX()->setRange(0,180);
    chart->axisY()->setRange(0,100);
    chart->legend()->hide();

    chart->axisY()->setTitleText("Speed of motor (%)");
    chart->axisX()->setTitleText("Time (seconds)");

    chart->setTitle("System Output");
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    layout = new QVBoxLayout();
    layout->addWidget(chartView);
    layout->setMargin(0);

    ui->chartFrame->setLayout(layout);

    sph = new SerialPortHandler();

    statusMessage = new QLabel();
    statusMessage->setStyleSheet("QLabel {color: red}");
    statusMessage->setMargin(5);
    statusBar()->addWidget(statusMessage);

    if(sph->startReadingWriting())
    {
        QString message(QString("Connected through the port %1 | System stopped | Mode %2 ").arg(sph->getPortName(), QString::number(mode)));
        statusMessage->setText(message);
    }
    else
    {
        QString message(QString("Disconnected."));
        statusMessage->setText(message);
    }

    /* Connect slots and signals */
    connect(ui->actionSCD, SIGNAL(triggered(bool)), this, SLOT(about(void)));
    connect(timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    connect(ui->actionStart, SIGNAL(triggered(bool)), this, SLOT(startSystem()));
    connect(ui->actionStop, SIGNAL(triggered(bool)), this, SLOT(stopSystem()));
    connect(ui->actionMode1, SIGNAL(triggered(bool)), this, SLOT(setMode1()));
    connect(ui->actionMode2, SIGNAL(triggered(bool)), this, SLOT(setMode2()));

    /* Start the timer that checks if data has been received and does the plotting. */
    timer->start(TIMER_VALUE);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete aboutUi;
    delete aboutDialog;
    delete chart;
    delete chartView;
    delete layout;
    delete statusMessage;
    delete sph;
    delete seriesLine;
    delete seriesPoints;
    delete serialBuffer;
    delete timer;
}

void MainWindow::about(void)
{
    aboutDialog->show();
}

/* When the timer has been finished (after 1 second), then check for new
 * data and plot it. */
void MainWindow::handleTimeout()
{
    int t = 0, v = 0, x = 0;

    if(sph->isConnected())
    {
        QString bufferedData = sph->getReadData().trimmed();
        QStringList dataSet = bufferedData.split("+");

        /* If the system is stopped. */
        if(bufferedData.contains('-'))
        {
            seriesLine->clear();
            seriesPoints->clear();
            ui->lineEditTime->setText(QString("0 sec"));
            ui->lineEditSensor->setText(QString("0 %"));
            ui->lineEditPWM->setText(QString("0 %"));
            QString message(QString("Connected through the port %1 | System stopped | Mode %2 ").arg(sph->getPortName(), QString::number(mode)));
            statusMessage->setText(message);
        } else if(bufferedData.contains('+')) /* If the system is running. */
        {
            QString message(QString("Connected through the port %1 | System running | Mode %2 ").arg(sph->getPortName(), QString::number(mode)));
            statusMessage->setText(message);
        }

        /* If the system sent data about the current operation. */
        if(dataSet.size() > 1)
        {
            foreach(QString data, dataSet)
            {
                QStringList valuesStr = data.split(";");
                if(valuesStr.size() == 3) {

                    /* Handle the data. */

                    t = valuesStr[0].toInt();
                    x = valuesStr[1].toInt();
                    v = valuesStr[2].toInt();

                    /* Plot the data. */
                    seriesLine->append(t, v);
                    if(mode == 1)
                    {
                        seriesPoints->append(t, dryer_mode1(t));
                    }
                    else
                    {
                        seriesPoints->append(t, dryer_mode2(t));
                    }
                    ui->lineEditTime->setText(QString("%1 sec").arg(QString::number(t)));
                    ui->lineEditSensor->setText(QString("%1 %").arg(QString::number(x)));
                    ui->lineEditPWM->setText(QString("%1 %").arg(QString::number(v)));
                }
            }
        }
        else
        {
            //qDebug() << "System is stopped";
        }
    }
    else
    {
        //qDebug() << "Disconnected";
    }

    // Restart the timer.
    timer->start(TIMER_VALUE);

}


void MainWindow::startSystem()
{
    if(sph->isConnected())
    {
        sph->writeData("+");
    }
}

void MainWindow::stopSystem()
{
    if(sph->isConnected())
    {
        sph->writeData("-");
    }
}

void MainWindow::setMode1()
{
    if(sph->isConnected())
    {
        sph->writeData("1");
        mode = 1;
    }
}

void MainWindow::setMode2()
{
    if(sph->isConnected())
    {
        sph->writeData("2");
        mode = 2;
    }
}


/* Define how the dryer is going to work (default mode, needs 3 minutes). */
int MainWindow::dryer_mode1(int time)
{
    float result_float = 0;
    uint8_t result_int = 0;

    if(time <= 30)
    {
        result_int = time;
    }
    else if(time > 30 && time <= 60)
    {
        result_int = 30;
    }
    else if(time > 60 && time <= 90)
    {
        result_float = 1.5 * time - 60;
        result_int = roundf(result_float);
    }
    else if(time > 90 && time <= 120)
    {
        result_int = 75;
    }
    else if(time > 120 && time <= 180)
    {
        result_float = -1.25 * time + 225;
        result_int = roundf(result_float);
    }
    else
    {
        result_int = 255;
    }

    return result_int;
}

/* Define how the dryer is going to work (fast mode, needs 1 minute). */
int MainWindow::dryer_mode2(int time)
{
    float result_float = 0;
    uint8_t result_int = 0;

    if(time <= 10)
    {
        result_int = 10*time;
    }
    else if(time > 10 && time < 50)
    {
        result_int = 100;
    }
    else
    {
        result_float = -10 * time + 600;
        result_int = roundf(result_float);
    }

    return result_int;
}
