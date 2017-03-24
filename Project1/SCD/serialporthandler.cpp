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
    buffer = new QBuffer(readData);
    buffer->open(QBuffer::ReadWrite);

    connect(serialPort, &QSerialPort::readyRead, this, &SerialPortHandler::handleReadyRead);
    connect(serialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &SerialPortHandler::handleError);


}

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

QString SerialPortHandler::getReadData()
{
    if(buffer->bytesAvailable() > 0)
    {
        return QString(buffer->readAll());
    }
    return QString("");
}

bool SerialPortHandler::startReadingWriting()
{
    if(serialPort->open(QSerialPort::ReadWrite)){
        return true;
    }

    return false;
}

void SerialPortHandler::handleReadyRead()
{
    readData->append(serialPort->readAll());
}

void SerialPortHandler::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        qDebug() << QObject::tr("An I/O error occurred while reading the data from port %1, error: %2").arg(serialPort->portName()).arg(serialPort->errorString()) << endl;
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
