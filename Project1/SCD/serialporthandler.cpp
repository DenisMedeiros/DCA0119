#include "serialporthandler.h"
#include <QDebug>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTextStream>
#include <QCoreApplication>
#include <QFile>
#include <QStringList>

SerialPortHandler::SerialPortHandler(QObject *parent) : QObject(parent)
{

    QString port = getAtmegaSerialPort();
    serialPort = new QSerialPort(this);
    serialPort->setPortName(port);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);

    readData = new QByteArray();
    standardOutput = new QTextStream(stdout);
    timer = new QTimer();

    connect(serialPort, &QSerialPort::readyRead, this, &SerialPortHandler::handleReadyRead);
    connect(serialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &SerialPortHandler::handleError);
    connect(timer, &QTimer::timeout, this, &SerialPortHandler::handleTimeout);

}

bool SerialPortHandler::startReadingWriting()
{
    if(serialPort->open(QIODevice::ReadWrite))
    {
        timer->start(5000);
        return true;
    }
    return false;
}

bool SerialPortHandler::stopReadingWriting()
{
    timer->stop();
    serialPort->close();
    return true;
}

SerialPortHandler::~SerialPortHandler()
{
    delete serialPort;
    delete readData;
    delete standardOutput;
    delete timer;
}

void SerialPortHandler::handleReadyRead()
{
    readData->append(serialPort->readAll());

    if (!timer->isActive())
    {
        timer->start(5000);
    }
}

void SerialPortHandler::handleTimeout()
{
    if (readData->isEmpty()) {
        *standardOutput << QObject::tr("No data was currently available for reading from port %1").arg(serialPort->portName()) << endl;
    } else {
        *standardOutput << QObject::tr("Data successfully received from port %1").arg(serialPort->portName()) << endl;
        *standardOutput << *readData << endl;
    }
}

void SerialPortHandler::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        *standardOutput << QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(serialPort->portName()).arg(serialPort->errorString()) << endl;
    }
}


QString SerialPortHandler::getAtmegaSerialPort()
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

QByteArray SerialPortHandler::readPort(QSerialPort *port)
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

int SerialPortHandler::writeData(QByteArray data)
{
    qint64 bytesWritten = serialPort->write(data);

    if (bytesWritten == -1)
    {
        /* Failed to write the data to port */
        return -1;
    }
    else if (bytesWritten != data.size())
    {
        /* Failed to write all the data to port */;
        return -2;
    }
    else if (!serialPort->waitForBytesWritten(5000))
    {
        /* Operation timed out or an error occurred */
        return -3;
    }
    else {
        return 0;
    }
}
