#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

#include "serialportreader.h"

#include <QtSerialPort/QSerialPort>

#include <QTextStream>
#include <QCoreApplication>
#include <QFile>
#include <QStringList>

#include <QtSerialPort/QSerialPortInfo>

QTextStream cinput(stdin);
QTextStream coutput(stdout);





/**
 * @brief WritePort
 * @param port
 * @param writeData
 *
 * Recebe o endereço do objeto QSerialPort usado e o ByteArray do comandos
 * a ser enviado e escreve na porta serial.
 *
 */
void MainWindow::writePort(QSerialPort *port, const QByteArray &writeData)
{

    /* Descomentar a linha abaixo apenas para debug da transmissão serial */
    //coutput << "TX data :>" << writeData << endl;

    /* Baseado no endereço do QSerialPort recebido em *port, eu vou escrever na serial
     * com port->write() e passando como parametro o QByteArray writeData e como retorno
     * ele irá me dar o numero de bytes enviados
     */
    qint64 bytesWritten = port->write(writeData);

    /* Faço apenas uma checagem basica onde se retornou -1 deu erro ao enviar os dados e se retornar um numero
     * diferente do retornado alguma informação foi perdida durante a escrita
     */
    if (bytesWritten == -1) {
        coutput << QObject::tr("Falha ao escrever os dados na porta %1, error: %2").arg(port->portName()).arg(port->errorString()) << endl;
        QCoreApplication::exit(1);
    } else if (bytesWritten != writeData.length()) {
        coutput << QObject::tr("Falha ao escrever uma parte dos dados na porta %1, error: %2").arg(port->portName()).arg(port->errorString()) << endl;
        QCoreApplication::exit(1);
    }

}

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

    //QSerialPort *serial = new QSerialPort(this);
    //serial->setPortName("COM3");
    //serial->setBaudRate(QSerialPort::Baud1200);

    /*
    serial.setParity(QSerialPort::Parity);
    serial.setDataBits(p.dataBits);
    serial.setStopBits(p.stopBits);
    serial.setFlowControl(p.flowControl);
    */

    //if (!serial->open(QIODevice::ReadOnly)) {
   //     qDebug() << "could not oopepn";
   // } else {
   //     qDebug() << "could not open";
   // }

    QString port = getAtmegaSerialPort();
    QSerialPort *serialPort = new QSerialPort(this);
    serialPort->setPortName(port);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);

    //if (!serialPort.open(QIODevice::ReadWrite)) {
    //    qDebug() << QObject::tr("Falha ao abrir porta %1, erro: %2").arg(port).arg(serialPort.errorString()) << endl;
    //} else {
    //    qDebug() << "abriu com sucesso";
    //}

    //QByteArray data;
    //while(true)
    //{
    //   data = readPort(&serialPort);
    //   qDebug() << data;
//
    //}

    SerialPortReader *serialPortReader = new SerialPortReader(serialPort);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete aboutUi;
    delete aboutDialog;
    delete chartMode1;
    delete chartView;
    delete layout;
}

void MainWindow::about(void)
{
    aboutDialog->show();
}

QString MainWindow::getAtmegaSerialPort()
{
    foreach (const QSerialPortInfo &devinfo, QSerialPortInfo::availablePorts()) {
        QSerialPort devserial;
        devserial.setPort(devinfo);
        if (devserial.open(QIODevice::ReadWrite)) {
            devserial.close();
            return devinfo.portName();
        }
    }
    return "";
}

QByteArray MainWindow::readPort(QSerialPort *port)
{
    QByteArray readData;
    /* Vou aguardar X ms, no caso 75, para ver os dados disponiveis e começar a leitura
        (-1) não possuira timeout.
    */
    while (port->waitForReadyRead(75)) {
        readData.append(port->readAll());
    }

    /* Descomentar a linha abaixo apenas para debug da recepçao serial */
    //coutput << "RX data :> " << readData <<  endl;
    return readData;
}
