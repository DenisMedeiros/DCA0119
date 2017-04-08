#include "serialporthandler.h"
#include <QDebug>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTextStream>
#include <QCoreApplication>
#include <QFile>
#include <QStringList>
#include "mainwindow.h"

SerialPortHandler::SerialPortHandler(QObject *parent) : QObject(parent)
{

    portName = new QString(getAtmegaSerialPort());
    serialPort = new QSerialPort(this);
    serialPort->setPortName(*portName);

    /* Configure parameters for USART. */
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);

    readData = new QByteArray();
    buffer = new QBuffer(readData);
    buffer->open(QBuffer::ReadWrite);

    connect(serialPort, &QSerialPort::readyRead, this, &SerialPortHandler::handleReadyRead);
    connect(serialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &SerialPortHandler::handleError);

}

/* Open the connection and start to read and write data. */
bool SerialPortHandler::startReadingWriting()
{
    if(!portName->isEmpty())
    {
        if(serialPort->open(QSerialPort::ReadWrite)){
            return true;
        }
    }

    return false;
}

/* Close the connection and stop to read and write data. */
bool SerialPortHandler::stopReadingWriting()
{
    serialPort->close();
    return true;
}

SerialPortHandler::~SerialPortHandler()
{
    delete serialPort;
    delete readData;
    delete buffer;
}

/* Returns the port name. */
const QString SerialPortHandler::getPortName()
{
    return *portName;
}

/* Return read data. */
const QString SerialPortHandler::getReadData()
{
    if(buffer->bytesAvailable() > 0)
    {
        return QString(buffer->readAll());
    }
    return QString("");
}

/* Check if the microcontroller is connected. */
bool SerialPortHandler::isConnected()
{
    return serialPort->isOpen();
}

/* Handle signal when data has been arrive. */
void SerialPortHandler::handleReadyRead()
{
    readData->append(serialPort->readAll());
}

/* Handle error with the connection. */
void SerialPortHandler::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        qDebug() << QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(serialPort->portName()).arg(serialPort->errorString()) << endl;
    }
}

/* Returns the Atmega serial port name (e.g. COM3, ttyUSB0). */
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

/* Write data on the serial port buffer. */
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
