#ifndef SERIALREADER_H
#define SERIALREADER_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QTextStream>
#include <QTimer>
#include <QByteArray>


class SerialPortReader : public QObject
{
    Q_OBJECT

private:
    QSerialPort *serialPort;
    QByteArray  *readData;
    QTextStream *standardOutput;
    QTimer      *timer;

public:
    explicit SerialPortReader(QSerialPort *_serialPort, QObject *parent = 0);
    ~SerialPortReader();

signals:

private slots:
    void handleReadyRead();
    void handleTimeout();
    void handleError(QSerialPort::SerialPortError error);

public slots:
};

#endif // SERIALREADER_H
