#include "serialportreader.h"
#include <QDebug>

SerialPortReader::SerialPortReader(QSerialPort *_serialPort, QObject *parent) : QObject(parent)
{
    serialPort = _serialPort;
    readData = new QByteArray();
    standardOutput = new QTextStream(stdout);
    timer = new QTimer();

    connect(serialPort, &QSerialPort::readyRead, this, &SerialPortReader::handleReadyRead);
    connect(serialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &SerialPortReader::handleError);
    connect(timer, &QTimer::timeout, this, &SerialPortReader::handleTimeout);

    serialPort->open(QIODevice::ReadWrite);
    timer->start(5000);
}

SerialPortReader::~SerialPortReader()
{
    delete serialPort;
    delete readData;
    delete standardOutput;
    delete timer;
}

void SerialPortReader::handleReadyRead()
{
    readData->append(serialPort->readAll());

    if (!timer->isActive())
    {
        timer->start(5000);
    }
}

void SerialPortReader::handleTimeout()
{
    if (readData->isEmpty()) {
        *standardOutput << QObject::tr("No data was currently available for reading from port %1").arg(serialPort->portName()) << endl;
    } else {
        *standardOutput << QObject::tr("Data successfully received from port %1").arg(serialPort->portName()) << endl;
        *standardOutput << *readData << endl;
    }
}

void SerialPortReader::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        *standardOutput << QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(serialPort->portName()).arg(serialPort->errorString()) << endl;
    }
}
