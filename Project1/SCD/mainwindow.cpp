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

    aboutUi = new Ui::Dialog();
    aboutDialog = new QDialog(0, 0);
    aboutUi->setupUi(aboutDialog);

    serialBuffer = new QByteArray();
    timer = new QTimer();

    seriesLineMode1 = new QLineSeries();
    seriesLineMode2 = new QLineSeries();
    seriesPointsMode1 = new QScatterSeries();
    seriesPointsMode2 = new QScatterSeries();

    //seriesPointsMode1->setMarkerSize(5);

    /* Creates the chart of the mode 1 */
    chartMode1 = new QChart();
    chartMode1->addSeries(seriesLineMode1);
    //chartMode1->addSeries(seriesPointsMode1);

    chartMode1->createDefaultAxes();
    chartMode1->axisX()->setRange(0,180);
    chartMode1->axisY()->setRange(0,100);
    chartMode1->legend()->hide();
    //chartMode1->setMargins(QMargins(0,0,0,0));
    //chartMode1->layout()->setContentsMargins(0,0,0,0);

    chartMode2 = new QChart();
    chartMode2->legend()->hide();


    chartMode2->addSeries(seriesLineMode2);
    chartMode2->addSeries(seriesPointsMode2);


    chartMode1->setTitle("Mode 1");
    chartView = new QChartView(chartMode1);
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
        QString message(QString("Connected through the port %1 | System stopped ").arg(sph->getPortName()));
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

    timer->start(TIMER_VALUE);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete aboutUi;
    delete aboutDialog;
    delete chartMode1;
    delete chartMode2;
    delete chartView;
    delete layout;
    delete statusMessage;
    delete sph;
    delete seriesLineMode1;
    delete seriesLineMode2;
    delete seriesPointsMode1;
    delete seriesPointsMode2;
    delete serialBuffer;
    delete timer;
}

void MainWindow::nextPoint(float x, float y)
{
    seriesPointsMode1->append(x,y);
}

void MainWindow::bufferAdd(char c)
{
    serialBuffer->append(c);
}

void MainWindow::about(void)
{
    aboutDialog->show();
}

void MainWindow::handleTimeout()
{
    int t = 0, v = 0, x = 0;

    if(sph->isConnected())
    {
        QString bufferedData = sph->getReadData().trimmed();
        QStringList dataSet = bufferedData.split("+");

        //qDebug() << QString("Recived: ") << bufferedData;

        if(bufferedData.contains('-'))
        {
            seriesLineMode1->clear();
            seriesPointsMode1->clear();
            ui->lineEditTime->setText(QString("0 sec"));
            ui->lineEditSensor->setText(QString("0 %"));
            ui->lineEditPWM->setText(QString("0 %"));

            QString message(QString("Connected through the port %1 | System stopped").arg(sph->getPortName()));
            statusMessage->setText(message);
        } else if(bufferedData.contains('+'))
        {
            QString message(QString("Connected through the port %1 | System running").arg(sph->getPortName()));
            statusMessage->setText(message);

        }

        if(dataSet.size() > 1)
        {
            foreach(QString data, dataSet)
            {
                QStringList valuesStr = data.split(";");
                if(valuesStr.size() == 3) {
                    t = valuesStr[0].toInt();
                    x = valuesStr[1].toInt();
                    v = valuesStr[2].toInt();
                    seriesLineMode1->append(t, v);
                    seriesPointsMode1->append(t ,v);
                    ui->lineEditTime->setText(QString("%1 sec").arg(QString::number(t)));
                    ui->lineEditSensor->setText(QString("%1 %").arg(QString::number(x)));
                    ui->lineEditPWM->setText(QString("%1 %").arg(QString::number(v)));

                    //qDebug() << QString("Recived t = %1, x = %2 and v = %3").arg(QString::number(t), QString::number(x), QString::number(v));

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
        qDebug() << "Disconnected";
    }


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
    }
}

void MainWindow::setMode2()
{
    if(sph->isConnected())
    {
        sph->writeData("2");
    }
}
